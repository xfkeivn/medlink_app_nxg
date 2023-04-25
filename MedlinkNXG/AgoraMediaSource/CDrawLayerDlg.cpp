// CDrawLayerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "CDrawLayerDlg.h"
#include "afxdialogex.h"
#include "VideoDlg.h"
// CDrawLayerDlg 消息处理程序
#include "AgoraMediaSource.h"
#include "MessageProtocal.h"

// CDrawLayerDlg 对话框
DWORD lastMoveTick;


#define SCALE(DEST,SRC,X) (((DEST)*(X)/(SRC)))


IMPLEMENT_DYNAMIC(CDrawLayerDlg, CDialogEx)


CDrawLayerDlg::~CDrawLayerDlg()
{
}

void CDrawLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BOOL CDrawLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG nExStyle = ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE);	nExStyle |= WS_EX_LAYERED;
	::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, nExStyle);	//设置窗口：用透明色来实现透明窗口。（还可以通过透明度来设置透明窗口：LWA_ALPHA）	
	::SetLayeredWindowAttributes(this->m_hWnd, theApp.m_clrWndTransparencyColorKey, 255, LWA_COLORKEY);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


extern CAgoraMediaSourceApp theApp;



BEGIN_MESSAGE_MAP(CDrawLayerDlg, CWnd)

	//ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	
	ON_WM_PAINT()


	ON_WM_LBUTTONDOWN()

	ON_WM_MOUSEMOVE()

	ON_WM_LBUTTONUP()

	//ON_COMMAND(ID_OPERATION_CLOSE, &CDrawLayerDlg::OnOperationClose)

	ON_COMMAND(ID_OPERATION_CLEAR, &CDrawLayerDlg::onCleanScreenBtnClicked)

	ON_COMMAND(ID_COLOR_RED, &CDrawLayerDlg::OnColorRed)

	ON_COMMAND(ID_COLOR_YELLOW, &CDrawLayerDlg::OnColorYellow)

	ON_COMMAND(ID_COLOR_GREEN, &CDrawLayerDlg::OnColorGreen)

	ON_COMMAND(ID_COLOR_BLACK, &CDrawLayerDlg::OnColorBlack)

	ON_COMMAND(ID_COLOR_WHITE, &CDrawLayerDlg::OnColorWhite)

	ON_COMMAND(ID_LINEWIDTH_1, &CDrawLayerDlg::OnLinewidth1)

	ON_COMMAND(ID_LINEWIDTH_2, &CDrawLayerDlg::OnLinewidth2)

	ON_COMMAND(ID_LINEWIDTH_5, &CDrawLayerDlg::OnLinewidth5)

	ON_COMMAND(ID_LINEWIDTH_10, &CDrawLayerDlg::OnLinewidth10)

	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CDrawLayerDlg::OnUpdateColorRed)

	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CDrawLayerDlg::OnUpdateColorGreen)

	ON_UPDATE_COMMAND_UI(ID_COLOR_YELLOW, &CDrawLayerDlg::OnUpdateColorYellow)

	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CDrawLayerDlg::OnUpdateColorBlack)

	ON_UPDATE_COMMAND_UI(ID_COLOR_WHITE, &CDrawLayerDlg::OnUpdateColorWhite)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_1, &CDrawLayerDlg::OnUpdateLinewidth1)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_10, &CDrawLayerDlg::OnUpdateLinewidth10)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_2, &CDrawLayerDlg::OnUpdateLinewidth2)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_5, &CDrawLayerDlg::OnUpdateLinewidth5)


END_MESSAGE_MAP()




	BOOL CDrawLayerDlg::PreTranslateMessage(MSG* pMsg)
	{



		//When FullScreen and DrawDlg will cover the full screen and the ESC can not be delivered to VideoDlg and cannot be restore
		if (pMsg->message == WM_KEYDOWN) {

			switch (pMsg->wParam) {
			case VK_RETURN:
				return FALSE;
			case VK_ESCAPE:
				//::SendMessage(GetParent()->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
				::SendMessage(GetParent()->m_hWnd, WM_VK_ESCAPE, 0, 0);
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
			if (((CVideoDlg*)GetParent())->GetMeetingStatus() == OTHER_ANNOTATION)
			{
				return TRUE;
			}
			BOOL isvalid = IsInVisualWindow(posx,posy);
			return !isvalid;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
}

bool CDrawLayerDlg::IsInVisualWindow(int screenX, int screenY)
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


CDrawLayerDlg::CDrawLayerDlg(CWnd* parent)

{
	
	
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











void CDrawLayerDlg::OnPaint()

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





void CDrawLayerDlg::OnLButtonDown(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//鼠标左键按下标志
	// is mouse down inside the visual region
	
	m_blMouseDown = TRUE;

	//当处于MouseTrack or ScreenShots状态时
	
	if (m_blMouseTrack ) {

		m_ptCurrent = m_ptPrev = point;
	}

	CVideoDlg * parent = (CVideoDlg*)this->GetParent();
	if (parent->GetMeetingStatus() == SELF_ANNOTATION)
	{
		RECT rect;
		GetClientRect(&rect);
		CommandManager::GetInstance()->BroadcastAnnotationMouseLDownEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
	}
	if (currentLine.points.size() != 0)
	{
		historyLines.push_back(currentLine);
		currentLine.points.clear();
	}
	currentLine.points.push_back(point);
	currentLine.color = m_clrLineColor;
	currentLine.sickness = m_nLineWidth;

	CDialogEx::OnLButtonDown(nFlags, point);

}





void CDrawLayerDlg::OnMouseMove(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//MouseDown

	if (((CVideoDlg *)GetParent())->GetMeetingStatus() == SELF_ANNOTATION)
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

	CVideoDlg * parent = (CVideoDlg*)this->GetParent();
	if (parent->GetMeetingStatus() == SELF_ANNOTATION)
	{
		RECT rect;
		GetClientRect(&rect);
		CommandManager::GetInstance()->BroadcastAnnotationMouseMoveEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
		
	}

	CDialogEx::OnMouseMove(nFlags, point);

}





void CDrawLayerDlg::OnLButtonUp(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//鼠标左键释放标志

	m_blMouseDown = FALSE;

	CVideoDlg * parent = (CVideoDlg*)this->GetParent();
	if (parent->GetMeetingStatus() == SELF_ANNOTATION)
	{
		RECT rect;
		GetClientRect(&rect);
		CommandManager::GetInstance()->BroadcastAnnotationMouseLUpEvent(rect.right - rect.left, rect.bottom - rect.top, point.x, point.y);
	}


	currentLine.points.push_back(point);
	historyLines.push_back(currentLine);
	currentLine.points.clear();

	CDialogEx::OnLButtonUp(nFlags, point);

}

void CDrawLayerDlg::OnOperationClear()

{

	// TODO: 在此添加命令处理程序代码

	CClientDC dc(this);

	historyLines.clear();

	ClearScreen(&dc);



}





void CDrawLayerDlg::OnColorRed()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 0, 0);

}





void CDrawLayerDlg::OnColorYellow()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 255, 0);

}





void CDrawLayerDlg::OnColorGreen()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(0, 255, 0);

}





void CDrawLayerDlg::OnColorBlack()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(0, 0, 0);

}





void CDrawLayerDlg::OnColorWhite()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 255, 255);

}





void CDrawLayerDlg::OnLinewidth1()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 1;

}





void CDrawLayerDlg::OnLinewidth2()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 2;



}





void CDrawLayerDlg::OnLinewidth5()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 5;



}





void CDrawLayerDlg::OnLinewidth10()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 10;



}





void CDrawLayerDlg::OnUpdateColorRed(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 0, 0));

}





void CDrawLayerDlg::OnUpdateColorGreen(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(0, 255, 0));

}





void CDrawLayerDlg::OnUpdateColorYellow(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 255, 0));

}





void CDrawLayerDlg::OnUpdateColorBlack(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(0, 0, 0));

}





void CDrawLayerDlg::OnUpdateColorWhite(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 255, 255));

}


void CDrawLayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	GetVisualWindow(cx, cy, &visualWidth, &visualHeight, &widthPadding, &heightPadding);

}


void CDrawLayerDlg::OnUpdateLinewidth1(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 1);

}





void CDrawLayerDlg::OnUpdateLinewidth10(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 10);

}





void CDrawLayerDlg::OnUpdateLinewidth2(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 2);

}





void CDrawLayerDlg::OnUpdateLinewidth5(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 5);

}


void CDrawLayerDlg::CleanScreen()
{

	OnOperationClear();
}

void CDrawLayerDlg::onCleanScreenBtnClicked()
{
	HWND parent_hwnd = GetParent()->GetSafeHwnd();
	if (parent_hwnd != NULL)
	{
		::PostMessage(parent_hwnd, WM_CLEANSCREEN_CMD, 0, 0);
	}
	OnOperationClear();
}

void CDrawLayerDlg::ClearScreen(CDC *pDC)

{

	// TODO: 在此处添加实现代码.



	CRect rect;

	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, theApp.m_clrWndTransparencyColorKey);

}



void CDrawLayerDlg::KeepNoTransparent(CDC *pDC) {

	CRect rect;

	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, theApp.m_clrWndNoTransparencyColorKey);



}

bool CDrawLayerDlg::CalAnnotationPos(int sourcePosx, int sourcePosy, int sourceWidth, int sourceHeight, LONG *posx, LONG *posy)
{
	int srcVisualW, srcVisualH, srcWpadding, srcHpadding;
	GetVisualWindow(sourceWidth, sourceHeight, &srcVisualW, &srcVisualH, &srcWpadding, &srcHpadding);
	*posx = (visualWidth*1.0 / srcVisualW)*(sourcePosx - srcWpadding) + widthPadding;
	*posy = (visualHeight*1.0 / srcVisualH)*(sourcePosy - srcHpadding) + heightPadding;
	return TRUE;
}


void CDrawLayerDlg::processRemoteAnnotationEvent(RTMMessagEvent &messageEvent)
{
	RECT rect;
	GetClientRect(&rect);
	CPoint point;
	//logInfo("processRemoteAnnotationEvent:messageEvent.source_window_width=" + to_string(messageEvent.source_window_width)
	//	+ ",messageEvent.source_window_height=" + to_string(messageEvent.source_window_height) + ",x=" + to_string(messageEvent.mouse_x) + ",y=" + to_string(messageEvent.mouse_y));
	
	
	//point.x = SCALE(rect.right - rect.left, messageEvent.source_window_width,messageEvent.mouse_x);
	//point.y = SCALE(rect.bottom - rect.top, messageEvent.source_window_height,messageEvent.mouse_y);

	CalAnnotationPos(messageEvent.mouse_x, messageEvent.mouse_y, messageEvent.source_window_width, messageEvent.source_window_height, &point.x, &point.y);

	if (messageEvent.msgtype == MOUSE_MOVE)
	{
		//logInfo("Process MOUSE_MOVE event.");
		OnMouseMove(0, point);

	}

	else if (messageEvent.msgtype == MOUSE_LEFT_DOWN)
	{
		//logInfo("Process MOUSE_LEFT_DOWN event.");
		OnLButtonDown(0,  point);

		
	}

	else if (messageEvent.msgtype == MOUSE_LEFT_UP)
	{
		//logInfo("Process MOUSE_LEFT_UP event.");
		OnLButtonUp(0,  point);

	}
	

}
void CDrawLayerDlg::processSelfAnnotationEvent()
{

}
void CDrawLayerDlg::GetVisualWindow(int winWidth, int winHeight, int *visualwidth, int *visualheight, int *widpadding, int *heightpadding)
{
	RECT rect;
	
	int height=winHeight, width=winWidth;
	float whratio = (winWidth*1.0) / winHeight;

	if (whratio < REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO)
	{
		height = width / REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO;
		int hpadding = int((winHeight - height) / 2.0);
		*visualwidth = width;
		*visualheight = height;
		*heightpadding = hpadding;
		*widpadding = 0;
		
	}
	if (whratio > REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO)
	{
		width = height * REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO;
		int wpadding = int((winWidth - width) / 2.0);
		*visualwidth = width;
		*visualheight = height;
		*widpadding = wpadding;
		*heightpadding = 0;
	}

}

