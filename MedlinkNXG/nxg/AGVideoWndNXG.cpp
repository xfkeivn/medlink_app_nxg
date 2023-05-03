// AGVideoWnd.cpp : implement file
#include "stdafx.h"
#include "AGVideoWndNXG.h"
#include <Gdiplus.h>
#include "VideoSourceMgr.h"
#include "FrameRecorder.h"
#include "CustomMessages.h"
#include "CAnnotationWnd.h"
using namespace std;
#include <vector>
#define IDD_PLAY_TIMER 1001010
#define IDD_RECORDING_5S 1001011


#define WM_PARAM_RECORD_LOADED 1
#define WM_PARAM_REVIEW_FRAME_CHANGED 2
#define WM_PARAM_REVIEW_FRAME_TO_END 3
#define WM_PARAM_FREEZE_FRAME_TAKEN_AND_LOAED 4


IMPLEMENT_DYNAMIC(CAGInfoWndNXG, CWnd)

CAGInfoWndNXG::CAGInfoWndNXG()
	: m_bShowTip(TRUE)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nFps(0)
	, m_nVideoBitrate(0)
{
	m_brBack.CreateSolidBrush(RGB(0x58, 0x58, 0x58));
}

CAGInfoWndNXG::~CAGInfoWndNXG()
{
	m_brBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGInfoWndNXG, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()









void CAGInfoWndNXG::ShowTips(BOOL bShow, BOOL bIsRemote)
{
	if (m_bShowTip != bShow) {
		if (bShow)
			ShowWindow(SW_SHOW);
		else
			ShowWindow(SW_HIDE);
	}
	
	m_bShowTip = bShow;
	m_isRemote = bIsRemote;
	Invalidate(TRUE);
}

void CAGInfoWndNXG::SetVideoResolution(UINT nWidth, UINT nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}

void CAGInfoWndNXG::SetFrameRateInfo(UINT nFPS)
{
	m_nFps = nFPS;

	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}

void CAGInfoWndNXG::SetVideoBitrate(UINT nBitrate)
{
	m_nVideoBitrate = nBitrate;

	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}


void CAGInfoWndNXG::SetAudioBitrate(UINT bitrate)
{
	m_nAudioBitrate = bitrate;

	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}


void CAGInfoWndNXG::SetVideoLossRate(UINT lossRate)
{
	m_nVideoLossRate = lossRate;

	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}


void CAGInfoWndNXG::SetAudioLossRate(UINT lossRate)
{
	m_nAudioLossRate = lossRate;

	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}


void CAGInfoWndNXG::SetAudioDelay(UINT delay)
{
	m_nAudioDelay = delay;
	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}


void CAGInfoWndNXG::SetVideoDelay(UINT delay)
{
	m_nVideoDelay = delay;
	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}


void CAGInfoWndNXG::Reset()
{
	m_nWidth = m_nHeight = m_nFps = m_nVideoBitrate
		= m_nVideoLossRate = m_nVideoDelay
		= m_nAudioBitrate = m_nAudioLossRate
		= m_nAudioDelay = 0;
	if (m_bShowTip) {
		Invalidate(TRUE);
	}
}

void CAGInfoWndNXG::OnPaint()
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
		if (m_isRemote) {
			strTip.Format(_T("%dx%d, %dfps\nVRecv: %dkbps\nVLossRate: %d£¥\nVDelay: %dms\nARecv: %dkbps\nALossRate: %d£¥\nADelay: %dms"),
				m_nWidth, m_nHeight, m_nFps, m_nVideoBitrate, m_nVideoLossRate, m_nVideoDelay, m_nAudioBitrate, m_nAudioLossRate, m_nAudioDelay);
		}
		else {
			strTip.Format(_T("%dx%d, %dfps\nVSent: %dkbps\nVLossRate: %d£¥\nASent: %dkbps\nALossRate: %d£¥"),
				m_nWidth, m_nHeight, m_nFps, m_nVideoBitrate, m_nVideoLossRate, m_nAudioBitrate, m_nAudioLossRate);
		}
		
		dc.DrawText(strTip, &rcClient, DT_VCENTER | DT_CENTER);
	}
}

BOOL CAGInfoWndNXG::OnEraseBkgnd(CDC* pDC)
{

	// TODO:   add message handle code and /or call defalut values here
	CRect rcClient;

	GetClientRect(&rcClient);
	pDC->FillRect(&rcClient, &m_brBack);

	return TRUE;
}

// CAGVideoWnd

IMPLEMENT_DYNAMIC(CAGVideoWndNXG, CWnd)


CAGVideoWndNXG::CAGVideoWndNXG()
	: m_nUID(0)
	, m_crBackColor(RGB(0x58, 0x58, 0x58))
{
	
}

CAGVideoWndNXG::~CAGVideoWndNXG()
{
	m_imgBackGround.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CAGVideoWndNXG, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_NCACTIVATE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
END_MESSAGE_MAP()


void CAGVideoWndNXG::OnNcPaint()
{    // Call the base class implementation
	CWnd::OnNcPaint();

	// Check if the window is currently selected
	if (false)
	{
		// Get the device context for the window's non-client area
		CWindowDC dc(this);

		// Create a red pen with a thickness of 3 pixels
		CPen pen(PS_SOLID, 3, RGB(255, 0, 0));

		// Select the pen into the device context
		CPen* pOldPen = dc.SelectObject(&pen);

		// Get the window's non-client rectangle
		CRect rect;
		GetWindowRect(&rect);
		rect.OffsetRect(-rect.left, -rect.top);

		// Draw the border
		dc.Rectangle(rect);

		// Restore the old pen
		dc.SelectObject(pOldPen);
	}

}

void CAGVideoWndNXG::OnDestroy()
{
	CWnd::OnDestroy();
	m_drawing_wnd.DestroyWindow();
}

void CAGVideoWndNXG::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	if ( m_drawing_wnd.GetSafeHwnd() != NULL )
	{
		// Resize the child dialog to match the parent window size
		CRect rect;
		GetClientRect(&rect);
		this->ClientToScreen(&rect);
		m_drawing_wnd.MoveWindow(rect);
		//m_drawing_wnd.ShowWindow(bShow);

	}
	
}

// CAGVideoWnd message handle
BOOL CAGVideoWndNXG::OnEraseBkgnd(CDC* pDC)
{
	// TODO:   add message handle code and /or call default values here
	CRect		rcClient;
	CPoint		ptDraw;
	//IMAGEINFO	imgInfo;

	GetClientRect(&rcClient);

	pDC->FillSolidRect(&rcClient, m_crBackColor);
	return TRUE;
}

void CAGVideoWndNXG::SetUID(UINT nUID)
{
	m_nUID = nUID;
	//m_wndInfo.ShowWindow(SW_SHOW);
}

UINT CAGVideoWndNXG::GetUID()
{
	return m_nUID;
}

BOOL CAGVideoWndNXG::IsWndFree()
{
	return m_nUID == 0 ? TRUE : FALSE;
}

BOOL CAGVideoWndNXG::SetBackImage(UINT nID, UINT nWidth, UINT nHeight, COLORREF crMask)
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


void CAGVideoWndNXG::SetVideoStatsInfo(UINT nWidth, UINT nHeight, UINT nFps, UINT nBitrate, UINT lossRate, UINT delay)
{
	m_wndInfo.SetVideoResolution(nWidth, nHeight);
	m_wndInfo.SetFrameRateInfo(nFps);
	m_wndInfo.SetVideoBitrate(nBitrate);
	m_wndInfo.SetVideoLossRate(lossRate);
	m_wndInfo.SetVideoDelay(delay);
}


void CAGVideoWndNXG::SetAudioStatsInfo(UINT nBitrate, UINT lossRate, UINT delay)
{
	m_wndInfo.SetAudioBitrate(nBitrate);
	m_wndInfo.SetAudioLossRate(lossRate);
	m_wndInfo.SetAudioDelay(delay);
}

void CAGVideoWndNXG::SetFaceColor(COLORREF crBackColor)
{
	m_crBackColor = crBackColor;

	Invalidate(TRUE);
}


void CAGVideoWndNXG::SetVideoSource(VideoSource *vs)
{
	m_videoSource = vs;
}
VideoSource* CAGVideoWndNXG::GetVideoSource()
{
	return m_videoSource;
}

void CAGVideoWndNXG::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  add message handle code and /or call default values here

	::SendMessage(GetParent()->GetSafeHwnd(), WM_LBUTTON_DOWN_WND, (WPARAM)point.x, (LPARAM)point.y);
	CWnd::OnLButtonDown(nFlags, point);
	setActive(TRUE);
	

	if (m_videoSource->canDragAndDrop())
	{
		m_bDragging = TRUE;
		m_ptDragStart = point;

		SetCapture();
		SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}




	
}






void CAGVideoWndNXG::OnMouseMove(UINT nFlags, CPoint point)
{
	// The user is moving the mouse on the window
	if (m_bDragging && m_videoSource->canDragAndDrop())
	{
		// Calculate the new position of the window
		
		
		CPoint ptOffset = point - m_ptDragStart;
		CRect rectWnd;
		GetWindowRect(&rectWnd);
		GetParent()->ScreenToClient(&rectWnd);

		rectWnd.OffsetRect(ptOffset);
		CRect rectParent;
		GetParent()->GetClientRect(&rectParent);

		if (rectWnd.left < rectParent.left)
		{
			rectWnd.OffsetRect(rectParent.left - rectWnd.left, 0);
		}
		if (rectWnd.right > rectParent.right)
		{
			rectWnd.OffsetRect(rectParent.right - rectWnd.right, 0);
		}
		if (rectWnd.top < rectParent.top)
		{
			rectWnd.OffsetRect(0, rectParent.top - rectWnd.top);
		}
		if (rectWnd.bottom > rectParent.bottom)
		{
			rectWnd.OffsetRect(0, rectParent.bottom - rectWnd.bottom);
		}

		SetWindowPos(NULL, rectWnd.left, rectWnd.top, rectWnd.Width(), rectWnd.Height(), SWP_NOZORDER | SWP_NOACTIVATE);

		//GetParent()->Invalidate();
	


	}
}


void CAGVideoWndNXG::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  add message handle code and /or call default values here

	::SendMessage(GetParent()->GetSafeHwnd(), WM_LBUTTON_UP_WND, (WPARAM)point.x, (LPARAM)point.y);
	CWnd::OnLButtonDown(nFlags, point);
	setActive(FALSE);
	if (m_bDragging && m_videoSource->canDragAndDrop())
	{
		// Move the window to the new position
		ReleaseCapture();
		m_bDragging = FALSE;
		
		CRect rectWnd;
		GetWindowRect(&rectWnd);
		GetParent()->ScreenToClient(&rectWnd);

		m_videoSource->drop_to_main(rectWnd);
		

	}


	
}

void CAGVideoWndNXG::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  add message handle code and /or call default values here
	::SendMessage(GetParent()->GetSafeHwnd(), WM_SHOWMODECHANGED, (WPARAM)this, (LPARAM)m_nUID);

	CWnd::OnRButtonDown(nFlags, point);
}


int CAGVideoWndNXG::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  add you own creation code here
	m_wndInfo.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, WND_INFO_WIDTH, WND_INFO_HEIGHT), this, IDC_STATIC);
	m_wndInfo.ShowTips(FALSE);
	m_drawing_wnd.setParent(this);
	m_drawing_wnd.Create(CAnnotationCwnd::IDD,this);



	// FOR TEST
	m_drawing_wnd.setDrawingStatus(NO_DRAWING);
	return 0;
}


void CAGVideoWndNXG::ShowStatsInfo(BOOL bShow, BOOL bIsRemote)
{
	m_wndInfo.ShowTips(bShow, bIsRemote);
	if (!bIsRemote) {
		m_wndInfo.MoveWindow(0, 0, WND_INFO_WIDTH, WND_INFO_HEIGHT - 25);
	}
	
}


void CAGVideoWndNXG::Reset()
{
	m_wndInfo.Reset();
	Invalidate(TRUE);
}

void CAGVideoWndNXG::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (m_drawing_wnd.GetSafeHwnd() != NULL)
	{
		//m_drawing_wnd.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		// Resize the child dialog to match the parent window size
		CRect rect;
		GetClientRect(&rect);
		m_drawing_wnd.MoveWindow(rect);
		this->ClientToScreen(&rect);
		::MoveWindow(m_drawing_wnd.GetSafeHwnd(), rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

	}
}

void CAGVideoWndNXG::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  add message handle code and /or call default values here
	::SendMessage(GetParent()->GetSafeHwnd(), WM_SHOWMODECHANGED, (WPARAM)this, (LPARAM)m_nUID);

	CWnd::OnLButtonDblClk(nFlags, point);
}


BOOL CAGVideoWndNXG::OnNcActivate(BOOL bActive)
{
	setActive(bActive);
	return TRUE;
}


void CAGVideoWndNXG::setActive(BOOL active)
{
	bActive = active;
	if (bActive)
	{
		// highlight the window
		ModifyStyleEx(0, WS_EX_WINDOWEDGE);
	}
	else
	{
		// remove the highlight
		ModifyStyleEx(WS_EX_WINDOWEDGE, 0);
	}
}

CAnnotationCwnd * CAGVideoWndNXG::getAnnotationWnd()
{
	return &this->m_drawing_wnd;
}

void CapturedSourceVideoWnd::SetLocalVideoSource(CapturedVideoSource * localvideosource)
{
	m_local_video_source = localvideosource;
}

IMPLEMENT_DYNAMIC(CVideoPlayerWnd, CWnd)

BEGIN_MESSAGE_MAP (CVideoPlayerWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_VIDEO_PLAYER_NOTIFCATION, on_video_player_notification)

END_MESSAGE_MAP()

void CVideoPlayerWnd::OnSize(UINT id, int w, int h)
{
	CWnd::OnSize(id, w, h);
}


void CVideoPlayerWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDD_PLAY_TIMER)
	{
		next_Frame();
	}
	if (nIDEvent == IDD_RECORDING_5S)
	{
		
		KillTimer(IDD_RECORDING_5S);
		if (m_videoSourcceMgr->get_interactive_state() == VIDEO_RECORDING_STATE)
		{
			this->stop_Recording();
			m_videoSourcceMgr->set_interactive_state(VIDEO_RECORD_REVIEW_STATE);
			m_current_record_frame_number = load_Review_Record(RECORDING_FILE_NAME);
			current_display_frame_index = 0;
			::PostMessage(this->GetSafeHwnd(), WM_VIDEO_PLAYER_NOTIFCATION, WM_PARAM_RECORD_LOADED, NULL);
			Invalidate();
		}
		
	}
}
void CVideoPlayerWnd::setVideoSourceMgr(VideoSourceMgr *mgr)
{
	m_videoSourcceMgr = mgr;

}

int CVideoPlayerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	BOOL result = CWnd::OnCreate(lpCreateStruct);
	return result;
}

void CVideoPlayerWnd::display_freeze_frame(FreezeFrame *freezeframe)
{
	m_current_freeze_frame = freezeframe;
	Invalidate();

}
	void CVideoPlayerWnd::OnPaint() {
		CWnd::OnPaint();
		if (m_videoSourcceMgr->get_interactive_state() == VIDEO_RECORD_REVIEW_STATE)
		{
			m_current_capture_main_source->getFrameRecorder()->display_frame(current_display_frame_index, this);
		}
		if (m_videoSourcceMgr->get_interactive_state() == FREEZE_FRAME_REVIEW_STATE && m_current_freeze_frame!=nullptr)
		{

			m_current_capture_main_source->getFrameRecorder()->display_frame(m_current_freeze_frame, this);
		}
		
		
	}

	 void CVideoPlayerWnd::start_Recording(string record_file)
	 {
		 if ( m_current_capture_main_source != nullptr)
		 {
			 FileHeader header;
			 header.frame_width = m_current_capture_main_source->videoInfoHeader.bmiHeader.biWidth;
			 header.frame_heigth = m_current_capture_main_source->videoInfoHeader.bmiHeader.biHeight;
			 m_current_capture_main_source->getFrameRecorder()->start_recording(header, record_file);
			 SetTimer(IDD_RECORDING_5S, 5000,NULL);
		 }
	 }
	 void CVideoPlayerWnd::stop_Recording()
	 {
		 if (m_current_capture_main_source != nullptr)
		 {
			 m_current_capture_main_source->getFrameRecorder()->stop_recording();
		 }
	 }
	 int  CVideoPlayerWnd::load_Review_Record(string record_file)
	 {
		 int frames_in_file = 0;
		 if (m_current_capture_main_source != nullptr)
		 {
			 frames_in_file = m_current_capture_main_source->getFrameRecorder()->load_file(record_file);
			 current_display_video_total_frame_number = frames_in_file;
		 }
		 return frames_in_file;
	 }
	 void CVideoPlayerWnd::start_Playing_Record(int frame_index)
	 {
			 if (m_isPaused) {
				
				 int average_fs_ms = m_current_capture_main_source->videoInfoHeader.AvgTimePerFrame / 10000;
				 // start playing from index
				 if (frame_index <= current_display_video_total_frame_number - 1 && frame_index >= 0)
				 {
					 current_display_frame_index = frame_index;
					 SetTimer(IDD_PLAY_TIMER, average_fs_ms, NULL);
					 m_isPlaying = true;
					 m_isPaused = false;
				 }	 
			 }
	 }

	 void CVideoPlayerWnd::pause_Playing_Record()
	 {
		 if (m_isPlaying) {
			 // Pause the playback
			 m_isPlaying = false;
			 m_isPaused = true;
			 KillTimer(IDD_PLAY_TIMER);

		 }
	 }


	 void CVideoPlayerWnd::next_Frame()
	 {
		
		 if (current_display_frame_index < current_display_video_total_frame_number - 1)
		 {
			 current_display_frame_index++;
			 ::PostMessage(this->GetSafeHwnd(), WM_VIDEO_PLAYER_NOTIFCATION, WM_PARAM_REVIEW_FRAME_CHANGED, NULL);
			 Invalidate();
		 }
		 else
		 {
			 pause_Playing_Record();
			 ::PostMessage(this->GetSafeHwnd(), WM_VIDEO_PLAYER_NOTIFCATION, WM_PARAM_REVIEW_FRAME_TO_END, NULL);
		 }
			 
		 

	 }
	 void CVideoPlayerWnd::prev_Frame()
	 {
		
		 if (current_display_frame_index > 0)
		 {
			 current_display_frame_index--;
			 ::PostMessage(this->GetSafeHwnd(), WM_VIDEO_PLAYER_NOTIFCATION, WM_PARAM_REVIEW_FRAME_CHANGED, NULL);
			 Invalidate();
		 }
	 }
	 void CVideoPlayerWnd::goto_Frame(int frame_index)
	 {
		 
		 if (frame_index <= current_display_video_total_frame_number - 1 && frame_index >= 0)
		 {
			 current_display_frame_index = frame_index;
			 ::PostMessage(this->GetSafeHwnd(), WM_VIDEO_PLAYER_NOTIFCATION, WM_PARAM_REVIEW_FRAME_CHANGED, NULL);
			 Invalidate();
		 }
	 }


	 void CVideoPlayerWnd::freeze_one_frame()
	 {
		 if (m_current_capture_main_source != nullptr)
		 {
			 m_current_capture_main_source->getFrameRecorder()->freeze_frame();
		 }
	 }

	 void CVideoPlayerWnd::on_frame_freezed(FreezeFrame *freezeframe)
	 {
		 //PostMessage(WM_ON_FREEZE_FRAME, WM_ON_FREEZE_FRAME, NULL);

		 if (m_videoSourcceMgr->get_interactive_state() == FREEZING_FRAME_STATE)
		 {
			 m_videoSourcceMgr->set_interactive_state(FREEZE_FRAME_REVIEW_STATE);
		
			 m_current_freeze_frame = freezeframe;
			 display_freeze_frame(freezeframe);
			 
			 ::PostMessage(this->GetSafeHwnd(), WM_VIDEO_PLAYER_NOTIFCATION, WM_PARAM_FREEZE_FRAME_TAKEN_AND_LOAED, NULL);
		 }
		 
		
	 }

	LRESULT CVideoPlayerWnd::on_video_player_notification(WPARAM wParam, LPARAM lParam)
	 {
		 switch (wParam)
		 {
		 case WM_PARAM_FREEZE_FRAME_TAKEN_AND_LOAED:
		 {
			 
			 for each (auto observer in m_observers)
			 {
				
				 observer->on_frame_freezed(*m_current_freeze_frame);

			 }
			 break;
		 }
		 case WM_PARAM_RECORD_LOADED:
		 {

			 for each (auto observer in m_observers)
			 {
				 
				 observer->on_video_record_loaded(this->get_current_record_frames());

			 }
			 break;
		 }
		 case WM_PARAM_REVIEW_FRAME_CHANGED:
		 {

			 for each (auto observer in m_observers)
			 {

				 observer->on_frame_changed(this->current_display_frame_index);

			 }
			 break;
		 }
		 case WM_PARAM_REVIEW_FRAME_TO_END:
		 {

			 for each (auto observer in m_observers)
			 {

				 observer->on_video_play_to_end(this->current_display_frame_index);

			 }
			 break;
		 }

		 default:
			 break;
		 }

		 return 0;
	 }

	IMPLEMENT_DYNAMIC(CapturedSourceVideoWnd, CAGVideoWndNXG)
	BEGIN_MESSAGE_MAP(CapturedSourceVideoWnd, CAGVideoWndNXG)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_SHOWWINDOW()
		ON_WM_MOVE()
	END_MESSAGE_MAP()
	int CapturedSourceVideoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CAGVideoWndNXG::OnCreate(lpCreateStruct) == -1)
			return -1;

	
		
		return 0;
	}
	void CapturedSourceVideoWnd::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		CAGVideoWndNXG::OnShowWindow(bShow, nStatus);
		/*
		if ( m_drawing_wnd.GetSafeHwnd() != NULL)
		{
			// Resize the child dialog to match the parent window size
			CRect rect;
			GetClientRect(&rect);
			//this->ClientToScreen(&rect);
			m_drawing_wnd.MoveWindow(rect);
			m_drawing_wnd.ShowWindow(bShow);

		}
		*/
	}
	void CapturedSourceVideoWnd::OnMove(int x, int y)
	{
		CAGVideoWndNXG::OnMove( x,  y);
		/*
		if (m_drawing_wnd.GetSafeHwnd() != NULL)
		{
			// Resize the child dialog to match the parent window size
			CRect rect;
			GetClientRect(&rect);
			//this->ClientToScreen(&rect);
			m_drawing_wnd.MoveWindow(rect);
			//::MoveWindow(m_drawing_wnd.GetSafeHwnd(), rect.left,rect.top,rect.Width(),rect.Height(),FALSE);
			//m_drawing_wnd->MoveWindow(rect);

		}
		*/
	}

	void CapturedSourceVideoWnd::OnSize(UINT nType, int cx, int cy)
	{
		CAGVideoWndNXG::OnSize(nType, cx, cy);
		/*
		if (m_drawing_wnd.GetSafeHwnd() != NULL)
		{
			// Resize the child dialog to match the parent window size
			CRect rect;
			GetClientRect(&rect);
			m_drawing_wnd.MoveWindow(rect);
			//this->ClientToScreen(&rect);
			//::MoveWindow(m_drawing_wnd.GetSafeHwnd(), rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			
		}*/
	}

