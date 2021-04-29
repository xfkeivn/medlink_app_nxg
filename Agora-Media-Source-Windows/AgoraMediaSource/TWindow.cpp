#include "stdafx.h"
#include "TWindow.h"

//#include <afxwin.h>

#include"StdAfx.h"

#include "TWindow.h"


#include"resource.h"
#include "AgoraMediaSource.h"

extern CAgoraMediaSourceApp theApp;


BEGIN_MESSAGE_MAP(CMainFrame, CWnd)

	ON_WM_CONTEXTMENU()

	ON_WM_CREATE()

	ON_WM_PAINT()

	ON_WM_NCCALCSIZE()

	ON_WM_LBUTTONDOWN()

	ON_WM_MOUSEMOVE()

	ON_WM_LBUTTONUP()

	ON_COMMAND(ID_OPERATION_CLOSE, &CMainFrame::OnOperationClose)

	ON_COMMAND(ID_OPERATION_CLEAR, &CMainFrame::OnOperationClear)

	ON_COMMAND(ID_COLOR_RED, &CMainFrame::OnColorRed)

	ON_COMMAND(ID_COLOR_YELLOW, &CMainFrame::OnColorYellow)

	ON_COMMAND(ID_COLOR_GREEN, &CMainFrame::OnColorGreen)

	ON_COMMAND(ID_COLOR_BLACK, &CMainFrame::OnColorBlack)

	ON_COMMAND(ID_COLOR_WHITE, &CMainFrame::OnColorWhite)

	ON_COMMAND(ID_LINEWIDTH_1, &CMainFrame::OnLinewidth1)

	ON_COMMAND(ID_LINEWIDTH_2, &CMainFrame::OnLinewidth2)

	ON_COMMAND(ID_LINEWIDTH_5, &CMainFrame::OnLinewidth5)

	ON_COMMAND(ID_LINEWIDTH_10, &CMainFrame::OnLinewidth10)

	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CMainFrame::OnUpdateColorRed)

	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CMainFrame::OnUpdateColorGreen)

	ON_UPDATE_COMMAND_UI(ID_COLOR_YELLOW, &CMainFrame::OnUpdateColorYellow)

	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CMainFrame::OnUpdateColorBlack)

	ON_UPDATE_COMMAND_UI(ID_COLOR_WHITE, &CMainFrame::OnUpdateColorWhite)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_1, &CMainFrame::OnUpdateLinewidth1)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_10, &CMainFrame::OnUpdateLinewidth10)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_2, &CMainFrame::OnUpdateLinewidth2)

	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_5, &CMainFrame::OnUpdateLinewidth5)

	ON_COMMAND(ID_MOUSEMOVE, &CMainFrame::OnMouseTrack)

	ON_UPDATE_COMMAND_UI(ID_MOUSEMOVE, &CMainFrame::OnUpdateMouseTrack)

	ON_COMMAND(ID_TEXTOUT, &CMainFrame::OnTextout)

	ON_UPDATE_COMMAND_UI(ID_TEXTOUT, &CMainFrame::OnUpdateTextout)

	ON_COMMAND(ID_TEXTCOLOR_BLACK, &CMainFrame::OnTextcolorBlack)

	ON_UPDATE_COMMAND_UI(ID_TEXTCOLOR_BLACK, &CMainFrame::OnUpdateTextcolorBlack)

	ON_COMMAND(ID_TEXTCOLOR_BLUE, &CMainFrame::OnTextcolorBlue)

	ON_UPDATE_COMMAND_UI(ID_TEXTCOLOR_BLUE, &CMainFrame::OnUpdateTextcolorBlue)

	ON_COMMAND(ID_TEXTCOLOR_GREEN, &CMainFrame::OnTextcolorGreen)

	ON_UPDATE_COMMAND_UI(ID_TEXTCOLOR_GREEN, &CMainFrame::OnUpdateTextcolorGreen)

	ON_COMMAND(ID_TEXTCOLOR_RED, &CMainFrame::OnTextcolorRed)

	ON_UPDATE_COMMAND_UI(ID_TEXTCOLOR_RED, &CMainFrame::OnUpdateTextcolorRed)

	ON_COMMAND(ID_TEXTCOLOR_WHITE, &CMainFrame::OnTextcolorWhite)

	ON_UPDATE_COMMAND_UI(ID_TEXTCOLOR_WHITE, &CMainFrame::OnUpdateTextcolorWhite)

	ON_WM_CHAR()

	ON_COMMAND(ID_TEXTPOINT_125, &CMainFrame::OnTextpoint125)

	ON_UPDATE_COMMAND_UI(ID_TEXTPOINT_125, &CMainFrame::OnUpdateTextpoint125)

	ON_COMMAND(ID_TEXTPOINT_150, &CMainFrame::OnTextpoint150)

	ON_UPDATE_COMMAND_UI(ID_TEXTPOINT_150, &CMainFrame::OnUpdateTextpoint150)

	ON_COMMAND(ID_TEXTPOINT_250, &CMainFrame::OnTextpoint250)

	ON_UPDATE_COMMAND_UI(ID_TEXTPOINT_250, &CMainFrame::OnUpdateTextpoint250)

	ON_COMMAND(ID_TEXTPOINT_350, &CMainFrame::OnTextpoint350)

	ON_UPDATE_COMMAND_UI(ID_TEXTPOINT_350, &CMainFrame::OnUpdateTextpoint350)

	ON_COMMAND(ID_SCREENSHOTS, &CMainFrame::OnScreenshots)

	ON_UPDATE_COMMAND_UI(ID_SCREENSHOTS, &CMainFrame::OnUpdateScreenshots)

	ON_COMMAND(ID_OPERATION_MINIMIZED, &CMainFrame::OnOperationMinimized)

	ON_WM_GETMINMAXINFO()

	ON_COMMAND(ID_OPERATION_MAXMIZED, &CMainFrame::OnOperationMaxmized)

	ON_COMMAND(ID_OPERATION_NORMAL, &CMainFrame::OnOperationNormal)

END_MESSAGE_MAP()







CMainFrame::CMainFrame(CWnd* parent)

{

	m_cxChar = 30;

	m_cyChar = 70;

	m_blWndFloating = FALSE;

	m_rtScreenShot.left = m_rtScreenShot.top = 0;

	m_rtScreenShot.right = m_rtScreenShot.bottom = 0;

	m_strFontName = _T("宋体");

	m_ptCaretLineBeginning.x = 0;

	m_ptCaretLineBeginning.y = 0;

	m_nFontSize = 350;

	m_blScreenShots = FALSE;

	m_blMouseTrack = TRUE;

	m_blTextOut = FALSE;

	m_blMouseDown = FALSE;

	m_nLineWidth = 5;

	m_clrLineColor = RGB(255, 0, 0);

	m_clrFontColor = RGB(255, 0, 0);

	m_bAutoMenuEnable = FALSE;



	CString strWndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,

		theApp.LoadStandardCursor(IDC_ARROW),

		(HBRUSH)(COLOR_3DFACE + 1),

		theApp.LoadIcon(IDI_SIMPLEICON));

	Create(strWndClass, NULL, WS_CHILD,rectDefault,parent,NULL);

	
	LONG nExStyle = ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE);	nExStyle |= WS_EX_LAYERED;
	::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, nExStyle);	//设置窗口：用透明色来实现透明窗口。（还可以通过透明度来设置透明窗口：LWA_ALPHA）	
	::SetLayeredWindowAttributes(this->m_hWnd, theApp.m_clrWndTransparencyColorKey, 255, LWA_COLORKEY);


}





CMainFrame::~CMainFrame()

{

}





int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)

{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)

		return -1;



	// TODO:  在此添加您专用的创建代码

	ModifyStyle(WS_CAPTION, 0);



	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_DRAWFRAME);





	//初始化字符的宽度和高度值

	SetCharCXCY(m_nFontSize, m_strFontName);







	return 0;

}







void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point)

{

	// TODO: 在此处添加消息处理程序代码

	CRect rect;

	GetClientRect(&rect);

	//因为point是屏幕坐标，所以需要把客户区坐标转化为屏幕坐标

	ClientToScreen(&rect);



	if (rect.PtInRect(point)) {

		CMenu popMenu;

		popMenu.LoadMenu(MAKEINTRESOURCE(IDR_MENU_CONTEXT));





		int nID = (popMenu.GetSubMenu(0)->TrackPopupMenu(

			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,

			point.x,

			point.y,

			this));



		SendMessage(WM_COMMAND, nID);



	}





}











void CMainFrame::OnPaint()

{

	////////////////////////////////////////////////////////////////////////////

	//填充指定的透明色，实现窗口透明化。

	//在OnPaint()函数里最好使用CPaintDC，不要使用CClientDC（因为它会闪烁和其它不良结果）。

	CPaintDC dc(this);

	if (!m_blWndFloating)

		ClearScreen(&dc);

	else

		KeepNoTransparent(&dc);







	////////////////////////////////////////////////////////////////////////////





}





void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//鼠标左键按下标志

	m_blMouseDown = TRUE;

	//当处于MouseTrack or ScreenShots状态时

	if (m_blMouseTrack || m_blScreenShots) {

		m_ptCurrent = m_ptPrev = point;

		if (m_blScreenShots) {

			//要截取矩形的左上角坐标为鼠标左键按下时的鼠标点坐标

			m_rtScreenShot.left = point.x;

			m_rtScreenShot.top = point.y;



		}

	}



	//TextOut

	if (m_blTextOut) {

		m_strInputText.Empty();

		CreateMyCaret(point);





	}





	CFrameWnd::OnLButtonDown(nFlags, point);

}





void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//MouseDown

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

		//ScreenShots

		else if (m_blScreenShots) {

			//始终按ROP2的R2_NOT模式画线

				//先擦除之前画的线

			int OldRop2 = dc.SetROP2(R2_NOT);

			dc.MoveTo(m_ptPrev);

			dc.LineTo(m_ptPrev.x, m_ptCurrent.y);

			dc.LineTo(m_ptCurrent);

			dc.LineTo(m_ptCurrent.x, m_ptPrev.y);

			dc.LineTo(m_ptPrev);



			//再画从起点到当前鼠标点的线

			dc.MoveTo(m_ptPrev);

			dc.LineTo(m_ptPrev.x, point.y);

			dc.LineTo(point);

			dc.LineTo(point.x, m_ptPrev.y);

			dc.LineTo(m_ptPrev);

			dc.SetROP2(OldRop2);



			//更新当前点的坐标为当前鼠标点坐标

			m_ptCurrent = point;



		}

		dc.SelectObject(pPen);





	}









	CFrameWnd::OnMouseMove(nFlags, point);

}





void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//鼠标左键释放标志

	m_blMouseDown = FALSE;



	//ScreenShots

	if (m_blScreenShots) {

		//建立屏幕DC（参数为NULL时就是屏幕DC）

		CClientDC dc(NULL);

		CRect rect;

		GetClientRect(&rect);



		//要截取的矩形右下角坐标为当前鼠标点坐标

		m_rtScreenShot.right = point.x;

		m_rtScreenShot.bottom = point.y;



		//把鼠标移动指定的矩形区域放大到整个屏幕

		dc.StretchBlt(

			rect.left, rect.top,

			rect.Width(), rect.Height(),

			&dc,

			m_rtScreenShot.left, m_rtScreenShot.top,

			m_rtScreenShot.Width(), m_rtScreenShot.Height(),

			SRCCOPY);

		//重新初始化要截取的矩形值为0

		m_rtScreenShot.left = m_rtScreenShot.top = 0;

		m_rtScreenShot.right = m_rtScreenShot.bottom = 0;



	}





	CFrameWnd::OnLButtonUp(nFlags, point);

}





void CMainFrame::OnOperationMinimized()

{

	// TODO: 在此添加命令处理程序代码

	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);

}









void CMainFrame::OnOperationClose()

{

	// TODO: 在此添加命令处理程序代码

	SendMessage(WM_SYSCOMMAND, SC_CLOSE);

}





void CMainFrame::OnOperationClear()

{

	// TODO: 在此添加命令处理程序代码

	CClientDC dc(this);

	ClearScreen(&dc);



}





void CMainFrame::OnColorRed()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 0, 0);

}





void CMainFrame::OnColorYellow()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 255, 0);

}





void CMainFrame::OnColorGreen()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(0, 255, 0);

}





void CMainFrame::OnColorBlack()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(0, 0, 0);

}





void CMainFrame::OnColorWhite()

{

	// TODO: 在此添加命令处理程序代码

	m_clrLineColor = RGB(255, 255, 255);

}





void CMainFrame::OnLinewidth1()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 1;

}





void CMainFrame::OnLinewidth2()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 2;



}





void CMainFrame::OnLinewidth5()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 5;



}





void CMainFrame::OnLinewidth10()

{

	// TODO: 在此添加命令处理程序代码

	m_nLineWidth = 10;



}





void CMainFrame::OnUpdateColorRed(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 0, 0));

}





void CMainFrame::OnUpdateColorGreen(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(0, 255, 0));

}





void CMainFrame::OnUpdateColorYellow(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 255, 0));

}





void CMainFrame::OnUpdateColorBlack(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(0, 0, 0));

}





void CMainFrame::OnUpdateColorWhite(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrLineColor == RGB(255, 255, 255));

}





void CMainFrame::OnUpdateLinewidth1(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 1);

}





void CMainFrame::OnUpdateLinewidth10(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 10);

}





void CMainFrame::OnUpdateLinewidth2(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 2);

}





void CMainFrame::OnUpdateLinewidth5(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nLineWidth == 5);

}





void CMainFrame::OnMouseTrack()

{

	// TODO: 在此添加命令处理程序代码

	m_blMouseTrack = TRUE;



	if (m_blTextOut) {

		DestroyMyCaret();

	}

	m_blTextOut = FALSE;

	m_blScreenShots = FALSE;

}





void CMainFrame::OnUpdateMouseTrack(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_blMouseTrack);

}





void CMainFrame::OnTextout()

{

	// TODO: 在此添加命令处理程序代码

	m_blTextOut = TRUE;

	m_blScreenShots = FALSE;

	m_blMouseTrack = FALSE;











}





void CMainFrame::OnUpdateTextout(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_blTextOut);

}





void CMainFrame::OnTextcolorBlack()

{

	// TODO: 在此添加命令处理程序代码

	m_clrFontColor = RGB(0, 0, 0);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextcolorBlack(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrFontColor == RGB(0, 0, 0));



}





void CMainFrame::OnTextcolorBlue()

{

	// TODO: 在此添加命令处理程序代码

	m_clrFontColor = RGB(0, 0, 255);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextcolorBlue(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrFontColor == RGB(0, 0, 255));



}





void CMainFrame::OnTextcolorGreen()

{

	// TODO: 在此添加命令处理程序代码

	m_clrFontColor = RGB(0, 255, 0);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextcolorGreen(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrFontColor == RGB(0, 255, 0));



}





void CMainFrame::OnTextcolorRed()

{

	// TODO: 在此添加命令处理程序代码

	m_clrFontColor = RGB(255, 0, 0);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextcolorRed(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrFontColor == RGB(255, 0, 0));



}





void CMainFrame::OnTextcolorWhite()

{

	// TODO: 在此添加命令处理程序代码

	m_clrFontColor = RGB(255, 255, 255);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextcolorWhite(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_clrFontColor == RGB(255, 255, 255));

}





void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//字符分情况处理

	CClientDC dc(this);

	CRect rect;

	GetClientRect(&rect);

	CSize size;

	switch (nChar)

	{

	case VK_RETURN:

		m_strInputText.Empty();

		m_ptCaretLineBeginning.y += (WCHAR)m_cyChar;

		break;

	case VK_BACK:



		break;



	default:



		m_strInputText += (WCHAR)nChar;

		break;

	}



	HideCaret();

	DrawInputText(&dc);

	SetCaretPosition(&dc);

	ShowCaret();





	CFrameWnd::OnChar(nChar, nRepCnt, nFlags);

}





void CMainFrame::OnTextpoint125()

{

	// TODO: 在此添加命令处理程序代码

	m_nFontSize = 125;

	SetCharCXCY(125, m_strFontName);

	ReSetTextAndCaretPos();



}





void CMainFrame::OnUpdateTextpoint125(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nFontSize == 125);

}















void CMainFrame::OnTextpoint150()

{

	// TODO: 在此添加命令处理程序代码

	m_nFontSize = 150;

	SetCharCXCY(150, m_strFontName);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextpoint150(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nFontSize == 150);

}





void CMainFrame::OnTextpoint250()

{

	// TODO: 在此添加命令处理程序代码

	m_nFontSize = 250;

	SetCharCXCY(250, m_strFontName);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextpoint250(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nFontSize == 250);

}







void CMainFrame::OnTextpoint350()

{

	// TODO: 在此添加命令处理程序代码

	m_nFontSize = 350;

	SetCharCXCY(350, m_strFontName);

	ReSetTextAndCaretPos();

}





void CMainFrame::OnUpdateTextpoint350(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_nFontSize == 350);

}





void CMainFrame::ReSetTextAndCaretPos()

{

	// TODO: 在此处添加实现代码.

	m_strInputText.Empty();

	m_ptCaretLineBeginning = GetCaretPos();

}





void CMainFrame::SetCharCXCY(int nFontSize, LPCTSTR strFont)

{

	// TODO: 在此处添加实现代码.

	CClientDC dc(this);

	m_fontMain.DeleteObject();

	m_fontMain.CreatePointFont(nFontSize, strFont);

	CFont *pOldFont = dc.SelectObject(&m_fontMain);

	TEXTMETRIC tm;

	dc.GetTextMetrics(&tm);

	m_cxChar = tm.tmAveCharWidth;

	m_cyChar = tm.tmHeight + tm.tmExternalLeading;

	dc.SelectObject(pOldFont);



}





void CMainFrame::DrawInputText(CDC *pDC) {

	CFont *pOldFont = pDC->SelectObject(&m_fontMain);

	COLORREF clrOldTextColor = pDC->SetTextColor(m_clrFontColor);

	pDC->TextOutW(m_ptCaretLineBeginning.x, m_ptCaretLineBeginning.y, m_strInputText);

	pDC->SetTextColor(clrOldTextColor);

	pDC->SelectObject(pOldFont);

}

void CMainFrame::SetCaretPosition(CDC *pDC) {

	CFont *pOldFont = pDC->SelectObject(&m_fontMain);

	CPoint point = m_ptCaretLineBeginning;

	point.x += pDC->GetTextExtent(m_strInputText).cx;

	SetCaretPos(point);

	pDC->SelectObject(pOldFont);



}









void CMainFrame::OnScreenshots()

{

	// TODO: 在此添加命令处理程序代码

	m_blScreenShots = TRUE;

	m_blMouseTrack = FALSE;

	if (m_blTextOut) {

		DestroyMyCaret();

	}

	m_blTextOut = FALSE;

}





void CMainFrame::OnUpdateScreenshots(CCmdUI *pCmdUI)

{

	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->SetCheck(m_blScreenShots);

}











void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)

{

	// TODO: 在此添加消息处理程序代码和/或调用默认值



	CClientDC dc(this);

	WINDOWPLACEMENT wp;

	GetWindowPlacement(&wp);

	wp.rcNormalPosition.left = 0;

	wp.rcNormalPosition.top = dc.GetDeviceCaps(VERTRES) / 4 * 3;

	wp.rcNormalPosition.right = 50;

	wp.rcNormalPosition.bottom = dc.GetDeviceCaps(VERTRES) / 4 * 3 + 40;



	SetWindowPlacement(&wp);







	CFrameWnd::OnGetMinMaxInfo(lpMMI);

}





void CMainFrame::ClearScreen(CDC *pDC)

{

	// TODO: 在此处添加实现代码.



	CRect rect;

	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, theApp.m_clrWndTransparencyColorKey);

}



void CMainFrame::KeepNoTransparent(CDC *pDC) {

	CRect rect;

	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, theApp.m_clrWndNoTransparencyColorKey);



}





void CMainFrame::OnOperationMaxmized()

{

	// TODO: 在此添加命令处理程序代码

	m_blWndFloating = FALSE;

	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

}





void CMainFrame::OnOperationNormal()

{

	// TODO: 在此添加命令处理程序代码

	m_blWndFloating = TRUE;

	SendMessage(WM_SYSCOMMAND, SC_RESTORE);









}











void CMainFrame::CreateMyCaret(CPoint point)

{

	// TODO: 在此处添加实现代码.



	CreateSolidCaret(m_cxChar / 7, m_cyChar);

	m_ptCaretLineBeginning = point;

	SetCaretPos(m_ptCaretLineBeginning);

	ShowCaret();

}





void CMainFrame::DestroyMyCaret()

{

	// TODO: 在此处添加实现代码.

	HideCaret();

	m_ptCaretLineBeginning = GetCaretPos();

	::DestroyCaret();

}





void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)

{

	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (lpncsp != NULL && GetWindowLong(GetSafeHwnd(), GWL_STYLE)&WS_THICKFRAME)

	{

		int xborder = ::GetSystemMetrics(SM_CXSIZEFRAME);

		int yborder = ::GetSystemMetrics(SM_CYSIZEFRAME);

		lpncsp->rgrc[0].left -= xborder-2;

		lpncsp->rgrc[0].top -= yborder;

		lpncsp->rgrc[0].right += xborder-2;

		lpncsp->rgrc[0].bottom += yborder-2;

	}

	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

}
