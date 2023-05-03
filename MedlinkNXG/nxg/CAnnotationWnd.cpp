// CDrawLayerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CAnnotationWnd.h"
#include "afxdialogex.h"
#include "VideoSourceMgr.h"
#include "AGVideoWndNXG.h"
#define MOUSE_MOVE_EVENT_DELAY_INTERVAL 10

// CDrawLayerDlg 对话框



#define SCALE(DEST,SRC,X) (((DEST)*(X)/(SRC)))


IMPLEMENT_DYNAMIC(CAnnotationCwnd, CDialogEx)


CAnnotationCwnd::~CAnnotationCwnd()
{
}

void CAnnotationCwnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BOOL CAnnotationCwnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG nExStyle = ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE);	nExStyle |= WS_EX_LAYERED;
	::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, nExStyle);	//设置窗口：用透明色来实现透明窗口。（还可以通过透明度来设置透明窗口：LWA_ALPHA）


	//缺省的不透明色
	::SetLayeredWindowAttributes(this->m_hWnd, m_clrWndTransparencyColorKey, 255, LWA_COLORKEY);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

/*
BOOL CAnnotationCwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CWnd::OnCreate(lpCreateStruct);

	// TODO:  在此添加额外的初始化
	//LONG nExStyle = ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE);	nExStyle |= WS_EX_LAYERED;
	//::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, nExStyle);	//设置窗口：用透明色来实现透明窗口。（还可以通过透明度来设置透明窗口：LWA_ALPHA）
	

	//缺省的不透明色
	//::SetLayeredWindowAttributes(this->m_hWnd, m_clrWndTransparencyColorKey, 255, LWA_COLORKEY);
	
	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
*/




BEGIN_MESSAGE_MAP(CAnnotationCwnd, CDialogEx)

	//ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	
	ON_WM_PAINT()

	//ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()

	ON_WM_MOUSEMOVE()

	ON_WM_LBUTTONUP()

	//ON_COMMAND(ID_OPERATION_CLOSE, &CDrawLayerDlg::OnOperationClose)

	ON_COMMAND(ID_OPERATION_CLEAR, &CAnnotationCwnd::onCleanScreenBtnClicked)

	ON_COMMAND(ID_COLOR_RED, &CAnnotationCwnd::OnColorRed)

	ON_COMMAND(ID_COLOR_YELLOW, &CAnnotationCwnd::OnColorYellow)

	ON_COMMAND(ID_COLOR_GREEN, &CAnnotationCwnd::OnColorGreen)

	ON_COMMAND(ID_COLOR_BLACK, &CAnnotationCwnd::OnColorBlack)

	ON_COMMAND(ID_COLOR_WHITE, &CAnnotationCwnd::OnColorWhite)

	ON_COMMAND(ID_LINEWIDTH_1, &CAnnotationCwnd::OnLinewidth1)

	ON_COMMAND(ID_LINEWIDTH_2, &CAnnotationCwnd::OnLinewidth2)

	ON_COMMAND(ID_LINEWIDTH_5, &CAnnotationCwnd::OnLinewidth5)

	ON_COMMAND(ID_LINEWIDTH_10, &CAnnotationCwnd::OnLinewidth10)

	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CAnnotationCwnd::OnUpdateColorRed)

	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CAnnotationCwnd::OnUpdateColorGreen)

	ON_UPDATE_COMMAND_UI(ID_COLOR_YELLOW, &CAnnotationCwnd::OnUpdateColorYellow)

	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CAnnotationCwnd::OnUpdateColorBlack)

	ON_UPDATE_COMMAND_UI(ID_COLOR_WHITE, &CAnnotationCwnd::OnUpdateColorWhite)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_1, &CAnnotationCwnd::OnUpdateLinewidth1)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_10, &CAnnotationCwnd::OnUpdateLinewidth10)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_2, &CAnnotationCwnd::OnUpdateLinewidth2)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_5, &CAnnotationCwnd::OnUpdateLinewidth5)


END_MESSAGE_MAP()

void CAnnotationCwnd::setParent(CAGVideoWndNXG* parent)
{
	m_parent = parent;
}


	BOOL CAnnotationCwnd::PreTranslateMessage(MSG* pMsg)
	{
		//When FullScreen and DrawDlg will cover the full screen and the ESC can not be delivered to VideoDlg and cannot be restore
		if (pMsg->message == WM_KEYDOWN) {

			switch (pMsg->wParam) {
			case VK_RETURN:
				return FALSE;
			case VK_ESCAPE:
				//::SendMessage(GetParent()->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
				if (this->m_drawing_observer != nullptr)
				{
					this->m_drawing_observer->on_drawing_panel_vkey_pressed();
				}
				
				//::SendMessage(GetParent()->m_hWnd, WM_VK_ESCAPE, 0, 0);
				return FALSE;
			}
		}

		else if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONUP || pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_RBUTTONUP)
		{
			CPoint mospoint = pMsg->pt;
			HWND hwnd = pMsg->hwnd;
			int posx = mospoint.x;
			int posy = mospoint.y;
			int visualwidth = 0;
			int visualheight = 0;
			int wPadding = 0;
			int hPadding = 0;
			if (m_drawing_status == REMOTE_DRAWING)
			{
				return TRUE;
			}
			BOOL isvalid = IsInVisualWindow(posx,posy);
			return !isvalid;
		}
		return CWnd::PreTranslateMessage(pMsg);
}

bool CAnnotationCwnd::IsInVisualWindow(int screenX, int screenY)
{
	RECT rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	int posx = screenX - rect.left;
	int posy = screenY - rect.top;

	if (widthPadding != 0)
	{
		if (posx < widthPadding || posx > visualWidth + widthPadding)
		{
			return false;
		}
	
	}

	if (heightPadding != 0)
	{
		if (posy < heightPadding || posy > visualHeight + heightPadding)
		{
			return false;
		}

	}

	return true;


}


CAnnotationCwnd::CAnnotationCwnd()

{
	m_clrWndTransparencyColorKey = RGB(147, 147, 148);

	//缺省的不透明色

	m_clrWndNoTransparencyColorKey = RGB(255, 0, 19);
	
	
	m_blWndFloating = FALSE;

	
	m_blMouseTrack = TRUE;

	

	m_blMouseDown = FALSE;

	m_nLineWidth = 5;

	m_clrLineColor = RGB(255, 0, 0);

	m_clrFontColor = RGB(255, 0, 0);

	visualWidth = 0;
	visualHeight = 0;
	widthPadding = 0;
	heightPadding = 0;

	m_parent = NULL;
}
















//void CDrawLayerDlg::OnContextMenu(CWnd* pWnd, CPoint point)
//
//{
//
//	// TODO: 在此处添加消息处理程序代码
//
//	CRect rect;
//
//	GetClientRect(&rect);
//
//	//因为point是屏幕坐标，所以需要把客户区坐标转化为屏幕坐标
//
//	ClientToScreen(&rect);
//
//
//
//	if (rect.PtInRect(point)) {
//
//		CMenu popMenu;
//
//		popMenu.LoadMenu(MAKEINTRESOURCE(IDR_MENU_CONTEXT));
//
//
//
//
//
//		int nID = (popMenu.GetSubMenu(0)->TrackPopupMenu(
//
//			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
//
//			point.x,
//
//			point.y,
//
//			this));
//
//
//
//		SendMessage(WM_COMMAND, nID);
//
//
//
//	}
//
//
//
//
//
//}











void CAnnotationCwnd::OnPaint()

{

	////////////////////////////////////////////////////////////////////////////

	//填充指定的透明色，实现窗口透明化。

	//在OnPaint()函数里最好使用CPaintDC，不要使用CClientDC（因为它会闪烁和其它不良结果）。

	CPaintDC dc(this);

	if (!m_blWndFloating)

		ClearScreen(&dc);

	else

		KeepNoTransparent(&dc);

	if (historyLines.size() != 0)
	{ 
		PolyLines::iterator itor = historyLines.begin();
		while (itor != historyLines.end())
		{
			PolyLine &line = *itor;
			CPen pen(PS_SOLID, line.sickness, line.color);
			CPen *pPen = dc.SelectObject(&pen);
			CPoint *point = line.points.data();
			dc.Polyline(point, line.points.size());
			dc.SelectObject(pPen);
			itor++;
		}
	}
}





void CAnnotationCwnd::OnLButtonDown(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//鼠标左键按下标志
	// is mouse down inside the visual region
	
	m_blMouseDown = TRUE;

	//当处于MouseTrack or ScreenShots状态时
	
	if (m_blMouseTrack ) {

		m_ptCurrent = m_ptPrev = point;
	}

	
	if (m_drawing_status == LOCAL_DRAWING)
	{
		RECT rect;
		GetClientRect(&rect);
		DrawingEvent drawing_event;
		drawing_event.drawing_window_width = rect.right - rect.left;
		drawing_event.drawing_window_height = rect.bottom - rect.top;
		drawing_event.mouse_point_x = point.x;
		drawing_event.mouse_point_y = point.y;
		drawing_event.drawing_event = DRAWING_MOUSE_LEFT_DOWN;
		if (this->m_drawing_observer != nullptr)
		{
			m_drawing_observer->on_local_drawing_event(drawing_event);
		}
		//CommandManager::GetInstance()->BroadcastAnnotationMouseLDownEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
	}
	if (currentLine.points.size() != 0)
	{
		historyLines.push_back(currentLine);
		currentLine.points.clear();
	}
	currentLine.points.push_back(point);
	currentLine.color = m_clrLineColor;
	currentLine.sickness = m_nLineWidth;

	CWnd::OnLButtonDown(nFlags, point);

}





void CAnnotationCwnd::OnMouseMove(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//MouseDown

	if (m_drawing_status == LOCAL_DRAWING)
	{
		if (GetTickCount() - lastMoveTick < MOUSE_MOVE_EVENT_DELAY_INTERVAL)
			return;
		lastMoveTick = GetTickCount();
	}


	

	if (m_blMouseDown) {



		CClientDC dc(this);

		CPen pen(PS_SOLID, m_nLineWidth, m_clrLineColor);

		CPen *pPen = dc.SelectObject(&pen);

		//MouseTrack

		if (m_blMouseTrack) {

			dc.MoveTo(m_ptPrev);

			m_ptCurrent = point;

			dc.LineTo(m_ptCurrent);

			m_ptPrev = m_ptCurrent;


		}

	
		dc.SelectObject(pPen);
		currentLine.points.push_back(point);

	}


	if (m_drawing_status == LOCAL_DRAWING)
	{
		RECT rect;
		GetClientRect(&rect);
		DrawingEvent drawing_event;
		drawing_event.drawing_window_width = rect.right - rect.left;
		drawing_event.drawing_window_height = rect.bottom - rect.top;
		drawing_event.mouse_point_x = point.x;
		drawing_event.mouse_point_y = point.y;
		drawing_event.drawing_event = DRAWING_MOUSE_MOVE;
		//CommandManager::GetInstance()->BroadcastAnnotationMouseMoveEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
		if (this->m_drawing_observer != nullptr)
		{
			this->m_drawing_observer->on_local_drawing_event(drawing_event);
		}
	}

	CWnd::OnMouseMove(nFlags, point);

}





void CAnnotationCwnd::OnLButtonUp(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//鼠标左键释放标志

	m_blMouseDown = FALSE;

	//CVideoDlg * parent = (CVideoDlg*)this->GetParent();

	if (m_drawing_status == LOCAL_DRAWING)
	{
		RECT rect;
		GetClientRect(&rect);
		DrawingEvent drawing_event;
		drawing_event.drawing_window_width = rect.right - rect.left;
		drawing_event.drawing_window_height = rect.bottom - rect.top;
		drawing_event.mouse_point_x = point.x;
		drawing_event.mouse_point_y = point.y;
		drawing_event.drawing_event = DRAWING_MOUSE_LEFT_UP;
		//CommandManager::GetInstance()->BroadcastAnnotationMouseMoveEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
		if (this->m_drawing_observer != nullptr)
		{
			this->m_drawing_observer->on_local_drawing_event(drawing_event);
		}//CommandManager::GetInstance()->BroadcastAnnotationMouseLUpEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
	}


	currentLine.points.push_back(point);
	historyLines.push_back(currentLine);
	currentLine.points.clear();

	CWnd::OnLButtonUp(nFlags, point);

}

void CAnnotationCwnd::OnOperationClear()

{

	// TODO: 在此添加命令处理程序代码

	CClientDC dc(this);

	historyLines.clear();

	ClearScreen(&dc);



}





void CAnnotationCwnd::OnColorRed()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 0, 0);

}





void CAnnotationCwnd::OnColorYellow()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 255, 0);

}





void CAnnotationCwnd::OnColorGreen()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(0, 255, 0);

}





void CAnnotationCwnd::OnColorBlack()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(0, 0, 0);

}





void CAnnotationCwnd::OnColorWhite()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 255, 255);

}





void CAnnotationCwnd::OnLinewidth1()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 1;

}





void CAnnotationCwnd::OnLinewidth2()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 2;



}





void CAnnotationCwnd::OnLinewidth5()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 5;



}





void CAnnotationCwnd::OnLinewidth10()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 10;



}





void CAnnotationCwnd::OnUpdateColorRed(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 0, 0));

}





void CAnnotationCwnd::OnUpdateColorGreen(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(0, 255, 0));

}





void CAnnotationCwnd::OnUpdateColorYellow(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 255, 0));

}





void CAnnotationCwnd::OnUpdateColorBlack(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(0, 0, 0));

}





void CAnnotationCwnd::OnUpdateColorWhite(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 255, 255));

}


void CAnnotationCwnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (GetSafeHwnd()!=NULL && ::IsWindowVisible(GetSafeHwnd()))
		GetVisualWindow(cx, cy, &visualWidth, &visualHeight, &widthPadding, &heightPadding);

}


void CAnnotationCwnd::OnUpdateLinewidth1(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 1);

}





void CAnnotationCwnd::OnUpdateLinewidth10(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 10);

}





void CAnnotationCwnd::OnUpdateLinewidth2(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 2);

}





void CAnnotationCwnd::OnUpdateLinewidth5(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 5);

}


void CAnnotationCwnd::CleanScreen()
{

	OnOperationClear();
}

void CAnnotationCwnd::onCleanScreenBtnClicked()
{
	/*
	HWND parent_hwnd = GetParent()->GetSafeHwnd();
	if (parent_hwnd != NULL)
	{
		::PostMessage(parent_hwnd, WM_CLEANSCREEN_CMD, 0, 0);
	}
	*/
	OnOperationClear();
}

void CAnnotationCwnd::ClearScreen(CDC *pDC)

{

	// TODO: 在此处添加实现代码.



	CRect rect;

	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, m_clrWndTransparencyColorKey);

}



void CAnnotationCwnd::KeepNoTransparent(CDC *pDC) {

	CRect rect;

	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, m_clrWndNoTransparencyColorKey);



}

bool CAnnotationCwnd::CalAnnotationPos(int sourcePosx, int sourcePosy, int sourceWidth, int sourceHeight, LONG *posx, LONG *posy)
{
	int srcVisualW, srcVisualH, srcWpadding, srcHpadding;
	GetVisualWindow(sourceWidth, sourceHeight, &srcVisualW, &srcVisualH, &srcWpadding, &srcHpadding);
	*posx = (visualWidth*1.0 / srcVisualW)*(sourcePosx - srcWpadding) + widthPadding;
	*posy = (visualHeight*1.0 / srcVisualH)*(sourcePosy - srcHpadding) + heightPadding;
	return TRUE;
}


void CAnnotationCwnd::processRemoteAnnotationEvent(DrawingEvent messageEvent)
{
	RECT rect;
	GetClientRect(&rect);
	CPoint point;
	CalAnnotationPos(messageEvent.mouse_point_x, messageEvent.mouse_point_y, messageEvent.drawing_window_width, messageEvent.drawing_window_height, &point.x, &point.y);

	if (messageEvent.drawing_event == DRAWING_MOUSE_MOVE)
	{
		//logInfo("Process MOUSE_MOVE event.");
		OnMouseMove(0, point);

	}

	else if (messageEvent.drawing_event == DRAWING_MOUSE_LEFT_DOWN)
	{
		//logInfo("Process MOUSE_LEFT_DOWN event.");
		OnLButtonDown(0,  point);

		
	}

	else if (messageEvent.drawing_event == DRAWING_MOUSE_LEFT_UP)
	{
		//logInfo("Process MOUSE_LEFT_UP event.");
		OnLButtonUp(0,  point);

	}
	

}
void CAnnotationCwnd::processSelfAnnotationEvent()
{

}
void CAnnotationCwnd::GetVisualWindow(int winWidth, int winHeight, int *visualwidth, int *visualheight, int *widpadding, int *heightpadding)
{


	RECT rect;
	int height=winHeight, width=winWidth;
	float whratio = (winWidth*1.0) / winHeight;
	//VideoSource * vs = m_video_source_mgr->get_main_video_source();
	//if (vs == nullptr)
	//	return;

	VideoSource *vs = this->m_parent->GetVideoSource();

	float target_ratio = vs->getSourceWidth()*1.0 / vs->getSourceHeigth()*1.0;
	if (whratio < target_ratio)
	{
		height = width / target_ratio;
		int hpadding = int((winHeight - height) / 2.0);
		*visualwidth = width;
		*visualheight = height;
		*heightpadding = hpadding;
		*widpadding = 0;
		
	}
	if (whratio > target_ratio)
	{
		width = height * target_ratio;
		int wpadding = int((winWidth - width) / 2.0);
		*visualwidth = width;
		*visualheight = height;
		*widpadding = wpadding;
		*heightpadding = 0;
	}

}

void CAnnotationCwnd::setDrawingStatus(DRAWING_STATUS status)
{
	m_drawing_status = status;
	if (status == NO_DRAWING)
		ShowWindow(FALSE);
	else
		ShowWindow(TRUE);
}