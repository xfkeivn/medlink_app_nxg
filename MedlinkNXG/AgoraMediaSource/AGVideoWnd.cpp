// AGVideoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "AGVideoWnd.h"
#include "VideoDlg.h"
#include "HIDControl.h"
IMPLEMENT_DYNAMIC(CAGInfoWnd, CWnd)
__int64 last_tick = 0;

#define MASK_MOUSE_RIGHT  1
#define MASK_MOUSE_MIDDLE 2
#define MASK_MOUSE_LEFT 4

#define MASK_KEY_WIN 1
#define MASK_KEY_SHIFT 2
#define MASK_KEY_ALT 4
#define MASK_KEY_CTRL 8


CAGInfoWnd::CAGInfoWnd()
: m_bShowTip(TRUE)
, m_nWidth(0)
, m_nHeight(0)
, m_nFps(0)
, m_nBitrate(0)
{
	m_brBack.CreateSolidBrush(RGB(0x00, 0xA0, 0xE9));
}

CAGInfoWnd::~CAGInfoWnd()
{
	m_brBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGInfoWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CAGInfoWnd::ShowTips(BOOL bShow)
{
	m_bShowTip = bShow;

	if (bShow)
		ShowWindow(SW_SHOW);
	else 
		ShowWindow(SW_HIDE);

	Invalidate(FALSE);
}

void CAGInfoWnd::SetVideoResolution(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	if (m_bShowTip) {
		Invalidate(TRUE);
		UpdateWindow();
	}
}

void CAGInfoWnd::SetFrameRateInfo(int nFPS)
{
	m_nFps = nFPS;

	if (m_bShowTip) {
		Invalidate(TRUE);
		UpdateWindow();
	}
}

void CAGInfoWnd::SetBitrateInfo(int nBitrate)
{
	m_nBitrate = nBitrate;

	if (m_bShowTip) {
		Invalidate(TRUE);
		UpdateWindow();
	}
}

void CAGInfoWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect	rcClient;
	CString strTip;

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	
	if (m_bShowTip) {
		// 640x480,15fps,400k
		GetClientRect(&rcClient);
		rcClient.top += 4;
		strTip.Format(_T("%dx%d, %dfps, %dK"), m_nWidth, m_nHeight, m_nFps, m_nBitrate);
		dc.DrawText(strTip, &rcClient, DT_VCENTER | DT_CENTER);
	}
}

BOOL CAGInfoWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;

	GetClientRect(&rcClient);
	pDC->FillRect(&rcClient, &m_brBack);
	
	return TRUE;
}

// CAGVideoWnd

IMPLEMENT_DYNAMIC(CAGVideoWnd, CWnd)

CAGVideoWnd::CAGVideoWnd()
: m_nUID(0)
, m_crBackColor(RGB(0x58, 0x58, 0x58))
, m_bShowVideoInfo(TRUE)
, m_bBigShow(FALSE)
, m_bBackground(FALSE)
{
	//m_pMainWnd = new CMainFrame(this);	////////////////////////////////////////////////////////////////////////////	//设置窗口启动时最大化，且成为活动窗口。 	
	//m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
	//m_pMainWnd->UpdateWindow(); 	////////////////////////////////////////////////////////////////////////////	//设置窗口的扩展样式：WS_EX_LAYERED,以形成透明窗口风格。 	


}

CAGVideoWnd::~CAGVideoWnd()
{
	m_imgBackGround.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CAGVideoWnd, CWnd)
	ON_WM_HOTKEY()
	ON_WM_ERASEBKGND()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_MBUTTONDOWN()
	//ON_WM_MBUTTONUP()
	//ON_WM_MOUSEHWHEEL()
	//ON_WM_LBUTTONUP()
	//ON_WM_RBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_WM_KEYDOWN()
	//ON_WM_KEYUP()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
    ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CAGVideoWnd 消息处理程序
BOOL CAGVideoWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect		rcClient;
	CPoint		ptDraw;
	IMAGEINFO	imgInfo;

	GetClientRect(&rcClient);

	pDC->FillSolidRect(&rcClient, m_crBackColor);
	if (!m_imgBackGround.GetImageInfo(0, &imgInfo))
		return TRUE;
	ptDraw.SetPoint((rcClient.Width() - imgInfo.rcImage.right) / 2, (rcClient.Height() - imgInfo.rcImage.bottom) / 2);
	if (ptDraw.x < 0)
		ptDraw.x = 0;
	if (ptDraw.y <= 0)
		ptDraw.y = 0;

	m_imgBackGround.Draw(pDC, 0, ptDraw, ILD_NORMAL);
	return TRUE;
}

void CAGVideoWnd::SetUID(UINT nUID)
{
	m_nUID = nUID;

	if (m_nUID == 0)
		m_wndInfo.ShowWindow(SW_HIDE);
	else
		m_wndInfo.ShowWindow(SW_SHOW);
}

UINT CAGVideoWnd::GetUID()
{
	return m_nUID;
}

BOOL CAGVideoWnd::IsWndFree()
{
	return m_nUID == 0 ? TRUE : FALSE;
}

BOOL CAGVideoWnd::SetBackImage(UINT nID, UINT nWidth, UINT nHeight, COLORREF crMask)
{
	CBitmap bmBackImage;

	if (!bmBackImage.LoadBitmap(nID))
		return FALSE;

	m_imgBackGround.DeleteImageList();

	m_imgBackGround.Create(nWidth, nHeight, ILC_COLOR24 | ILC_MASK, 1, 1);
	m_imgBackGround.Add(&bmBackImage, crMask);
	bmBackImage.DeleteObject();

	Invalidate(TRUE);

	return TRUE;
}

void CAGVideoWnd::ShowBackground(BOOL bBackground)
{
    m_bBackground = bBackground;

    Invalidate(TRUE);
}

void CAGVideoWnd::SetFaceColor(COLORREF crBackColor)
{
	m_crBackColor = crBackColor;

	Invalidate(TRUE);
}

void CAGVideoWnd::SetVideoResolution(UINT nWidth, UINT nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_wndInfo.SetVideoResolution(nWidth, nHeight);
}

void CAGVideoWnd::GetVideoResolution(UINT *nWidth, UINT *nHeight)
{
	*nWidth = m_nWidth;
	*nHeight = m_nHeight;
}

void CAGVideoWnd::SetBitrateInfo(int nReceivedBitrate)
{
	m_nBitRate = nReceivedBitrate;
	m_wndInfo.SetBitrateInfo(nReceivedBitrate);
}

void CAGVideoWnd::SetFrameRateInfo(int nReceiveFrameRate)
{
	m_nFrameRate = nReceiveFrameRate;
	m_wndInfo.SetFrameRateInfo(nReceiveFrameRate);
}


bool  CAGVideoWnd::GetPositionRelativeToVisualWindow(int *mospointx, int *mospointy, int *visualwidth, int *visualheight)
{
	RECT rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	int posx = *mospointx - rect.left;
	int posy = *mospointy - rect.top;

	int height = winHeight;
	int width = winWidth;
	
	if (whratio < REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO)
	{
		height = width / REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO;
		int hpadding = int((winHeight - height) / 2.0);
		*visualwidth = width;
		*visualheight = height;
		if (posy < hpadding || posy > height + hpadding)
			return false;
		else
		{
			posy -= hpadding;
			*mospointx = posx;
			*mospointy = posy;
			return true;
		}
	}
	if (whratio > REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO)
	{
		width = height * REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO;
		int wpadding = int((winWidth - width) / 2.0);
		*visualwidth = width;
		*visualheight = height;
		if (posx < wpadding || posx > width+ wpadding)
			return false;
		else
		{
			posx -= wpadding;
			*mospointx = posx;
			*mospointy = posy;
			return true;

		}


	}

}
/*
void CAGVideoWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	::SendMessage(GetParent()->GetSafeHwnd(), WM_SHOWBIG, (WPARAM)this, (LPARAM)m_nUID);

	

	CWnd::OnLButtonDown(nFlags, point);
}


void CAGVideoWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	::SendMessage(GetParent()->GetSafeHwnd(), WM_SHOWMODECHANGED, (WPARAM)this, (LPARAM)m_nUID);
	

	CWnd::OnRButtonDown(nFlags, point);
}*/


 
 

int CAGVideoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_wndInfo.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 192, 28), this, IDC_STATIC);

	return 0;
}


void CAGVideoWnd::ShowVideoInfo(BOOL bShow)
{
	m_bShowVideoInfo = bShow;

	m_wndInfo.ShowTips(bShow);
	Invalidate(TRUE);

/*	if (!bShow) {
		CRect rcTip;
		m_wndInfo.GetWindowRect(&rcTip);
		
	}
	*/

}

void CAGVideoWnd::SetBigShowFlag(BOOL bBigShow)
{
	CRect	rcClient;

	m_bBigShow = bBigShow;
	GetClientRect(&rcClient);

	int x = (rcClient.Width()- 192) / 2;
	int y = rcClient.Height() - 24;
	
	if (m_wndInfo.GetSafeHwnd() != NULL) {
		if (m_bBigShow)
			y -= 4;

		m_wndInfo.MoveWindow(x, y, 192, 24);
	}
};


void CAGVideoWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int x = (cx - 192) / 2;
	int y = cy - 24;
	// TODO:  在此处添加消息处理程序代码
	if (m_wndInfo.GetSafeHwnd() != NULL) {
		if (m_bBigShow)
			y -= 4;

		m_wndInfo.MoveWindow(x, y, 192, 24);
	}
	winWidth = cx;
	winHeight = cy;
	whratio = (cx*1.0) / cy;
}

BOOL CAGVideoWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class


	if (m_bBigShow && ((CVideoDlg*)GetParent())->GetMeetingStatus() == SELF_REMOTE_CONTROL)
	{
		UINT message = pMsg->message;
		WPARAM wparam = pMsg->wParam;
		LPARAM lparm = pMsg->lParam;
		CPoint mospoint = pMsg->pt;
		HWND hwnd = pMsg->hwnd;
		int posx = mospoint.x;
		int posy = mospoint.y;
		int visualwidth = 0;
		int visualheight = 0;
		

		if (pMsg->message == WM_MOUSEMOVE)
		{
			//Only process if interval is greater than 20ms
			__int64 t1 = GetTickCount();
			if (t1 - last_tick > MOUSE_MOVE_EVENT_DELAY_INTERVAL)
			{

				bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
				if (!inVisualWindow)
					return TRUE;
				if( !CAgoraObject::GetAgoraObject()->GetSelfHost())
					

					CommandManager::GetInstance()->sendRemoteControlMouseMoveEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
				else
				{
					HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_MOVE, visualwidth, visualheight, posx, posy, mouseButtonStatus));
				}

				last_tick = t1;
			}
		}
	
		else if (pMsg->message == WM_LBUTTONDOWN)
		{
			mouseButtonStatus |= MASK_MOUSE_LEFT;

			bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
			if (!inVisualWindow)
				return TRUE;
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlMouseLDownEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_LEFT_DOWN, visualwidth, visualheight, posx, posy, mouseButtonStatus));
		}
		else if (pMsg->message == WM_RBUTTONDOWN)
		{
			mouseButtonStatus |= MASK_MOUSE_RIGHT;

			bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
			if (!inVisualWindow)
				return TRUE;
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlMouseRDownEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_RIGHT_DOWN, visualwidth, visualheight, posx, posy, mouseButtonStatus));

		}
		else if (pMsg->message == WM_MBUTTONDOWN)
		{
			mouseButtonStatus |= MASK_MOUSE_MIDDLE;

			bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
			if (!inVisualWindow)
				return TRUE;
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlMouseMDownEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_MIDDLE_DOWN, visualwidth, visualheight, posx, posy, mouseButtonStatus));
		}
		else if (pMsg->message == WM_LBUTTONUP)
		{
			mouseButtonStatus &= (~MASK_MOUSE_LEFT);

			bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
			if (!inVisualWindow)
				return TRUE;
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlMouseLUpEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_LEFT_UP, visualwidth, visualheight, posx, posy, mouseButtonStatus));
		}
		else if (pMsg->message == WM_RBUTTONUP)
		{
			mouseButtonStatus &= (~MASK_MOUSE_RIGHT);

			bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
			if (!inVisualWindow)
				return TRUE;
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlMouseRUpEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_RIGHT_UP, visualwidth, visualheight, posx, posy, mouseButtonStatus));
		}
		else if (pMsg->message == WM_MBUTTONUP)
		{
			mouseButtonStatus &= (~MASK_MOUSE_MIDDLE);
			bool inVisualWindow = GetPositionRelativeToVisualWindow(&posx, &posy, &visualwidth, &visualheight);
			if (!inVisualWindow)
				return TRUE;
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlMouseMUpEvent(visualwidth, visualheight, posx, posy, mouseButtonStatus);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::MOUSE_MIDDLE_UP, visualwidth, visualheight, posx, posy, mouseButtonStatus));
		}
		else if (pMsg->message == WM_MOUSEWHEEL)
		{

		}
		else if (pMsg->message == WM_KEYDOWN)
		{
			int keycode = pMsg->wParam;
			switch (pMsg->wParam)
			{
				//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A) 不区分大小写

			case VK_LWIN:
				modifierKey |= MASK_KEY_WIN;
				break;
			case VK_CONTROL:
				modifierKey |= MASK_KEY_CTRL;
				break;
			case VK_SHIFT:
				modifierKey |= MASK_KEY_SHIFT;
				break;
			case LVKF_ALT:
				modifierKey |= MASK_KEY_ALT;
				break;
			}
			RECT rect;
			GetClientRect(&rect);
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlKeyDownEvent(rect.right - rect.left, rect.bottom - rect.top, keycode, modifierKey);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::KEY_DOWN, rect.right - rect.left, rect.bottom - rect.top, keycode, modifierKey));
		}
		else if (pMsg->message == WM_KEYUP)
		{
			int keycode = pMsg->wParam;
			switch (pMsg->wParam)
			{
				//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A) 不区分大小写

			case VK_LWIN:
				modifierKey &= (~MASK_KEY_WIN);
				break;
			case VK_CONTROL:
				modifierKey &= (~MASK_KEY_CTRL);
				break;
			case VK_SHIFT:
				modifierKey &= (~MASK_KEY_SHIFT);
				break;
			case LVKF_ALT:
				modifierKey &= (~!MASK_KEY_ALT);
				break;
			}
			RECT rect;
			GetClientRect(&rect);
			if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
				CommandManager::GetInstance()->sendRemoteControlKeyUpEvent(rect.right - rect.left, rect.bottom - rect.top, keycode, modifierKey);
			else
				HIDControl::GetInstance()->SendEventToHID(RTMMessagEvent(MSG_EVENT_TYPE::KEY_UP, rect.right - rect.left, rect.bottom - rect.top, keycode, modifierKey));
		}
		else
			return CWnd::PreTranslateMessage(pMsg);
	}

	return CWnd::PreTranslateMessage(pMsg);
	

}

void CAGVideoWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//::SendMessage(GetParent()->GetSafeHwnd(), WM_SHOWMODECHANGED, (WPARAM)this, (LPARAM)m_nUID);
	if (m_bBigShow && ((CVideoDlg*)GetParent())->GetMeetingStatus() == SELF_REMOTE_CONTROL)
	{
		SetFocus();
		
	}
	

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CAGVideoWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO:  在此处添加消息处理程序代码
    // 不为绘图消息调用 CWnd::OnPaint()

    if (m_bBackground) {
        CRect		rcClient;
        CPoint		ptDraw;
        IMAGEINFO	imgInfo;

        GetClientRect(&rcClient);

        dc.FillSolidRect(&rcClient, m_crBackColor);
        if (!m_imgBackGround.GetImageInfo(0, &imgInfo))
            return;

        ptDraw.SetPoint((rcClient.Width() - imgInfo.rcImage.right) / 2, (rcClient.Height() - imgInfo.rcImage.bottom) / 2);
        if (ptDraw.x < 0)
            ptDraw.x = 0;
        if (ptDraw.y <= 0)
            ptDraw.y = 0;

        m_imgBackGround.Draw(&dc, 0, ptDraw, ILD_NORMAL);
    }
    else
        return CWnd::OnPaint();
}

void CAGVideoWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	RECT rect;
	GetClientRect(&rect);
	int cx = rect.right - rect.left;
	int cy = rect.bottom - rect.top;
	winWidth = cx;
	winHeight = cy;
	whratio = (cx*1.0) / cy;

}

