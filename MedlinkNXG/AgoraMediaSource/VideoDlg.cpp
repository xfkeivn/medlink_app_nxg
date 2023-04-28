// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "AGEventDef.h"
#include "video_preprocessing_plugin.h"
#include "MessageProtocal.h"
#include "AGRTMEventHandler.h"
#include "HIDControl.h"
#include <mmsystem.h>
#include "CTipDlg.h"
#pragma comment(lib, "WINMM.LIB")
#include "VideoSourceMgr.h"
extern RTMMemberStatusEvent G_RTM_MEMBER_STATUS_EVENT;


//#define PARTICIPANT_WINDOW_WIDTH 250

#define BUTTON_SPACE 100 

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)
typedef struct _DROPOFFLINE_PARAM {
	CVideoDlg *videoDlg;
	UINT user_id;
}DROPOFFLINE_PARAM, *PDROPOFFLINE_PARAM;

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{
	m_brHead.CreateSolidBrush(RGB(15, 43, 75));
	SetBackgroundColor(RGB(0x26, 0x26, 0x26));
	m_bLastStat = FALSE;
	m_nScreenMode = SCREEN_VIDEO1;
	m_bFullScreen = FALSE;
	m_lpBigShowed = NULL;
    m_bFilter = FALSE;
    m_bShowInfo = FALSE;
	m_networkQuality = 0;
	m_ParticipantsUI = NULL;
	m_other_remote_id = -1;
	m_other_anno_id = -1;
	m_reconnectingUI = NULL;
}

CVideoDlg::~CVideoDlg()
{
	m_brHead.DeleteObject();
	m_reconnectingUI = NULL;
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTNMIN_VIDEO, m_btnMin);

	DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
	
	DDX_Control(pDX, IDC_BTNCLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
	ON_WM_NCCALCSIZE()
	ON_MESSAGE(WM_SHOWMODECHANGED, &CVideoDlg::OnShowModeChanged)
	ON_MESSAGE(WM_SHOWBIG, &CVideoDlg::OnShowBig)

	//ON_MESSAGE(WM_WINDOWSHARE, &CVideoDlg::OnWindowShareStart)
	//ON_MESSAGE(WM_DESKTOPSHARE, &CVideoDlg::OnDesktopShareStart)
	ON_MESSAGE(WM_MSGID(EID_AUDIO_VOLUME_INDICATION), &CVideoDlg::OnEIDAudioVolumeIndication)

	ON_MESSAGE(WM_RTM_USER_MSG_EVENT, &CVideoDlg::OnRTMUserEvent)
	ON_MESSAGE(WM_InviteAcceptedByPeer, &CVideoDlg::OnInvitationEvents)
	ON_MESSAGE(WM_InviteRefusedByPeer, &CVideoDlg::OnInvitationEvents)
	ON_MESSAGE(WM_InviteReceived, &CVideoDlg::OnInvitationEvents)
	ON_MESSAGE(WM_InviteEndByPeer, &CVideoDlg::OnInvitationEvents)

	ON_MESSAGE(WM_ChannelUserJoined, &CVideoDlg::OnMemberStatusEvent)
	ON_MESSAGE(WM_ChannelUserLeaved, &CVideoDlg::OnMemberStatusEvent)
	ON_MESSAGE(WM_ChannelUserList, &CVideoDlg::OnMemberStatusEvent)
	ON_MESSAGE(WM_CONNECTION_STATUS_CHANGED, &CVideoDlg::OnConnectionStatusChanged)
	

	ON_MESSAGE(WM_MSGID(EID_JOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_REJOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDReJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), &CVideoDlg::OnEIDFirstLocalFrame)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CVideoDlg::OnEIDFirstRemoteFrameDecoded)
	ON_MESSAGE(WM_MSGID(EID_USER_OFFLINE), &CVideoDlg::OnEIDUserOffline)
	
	ON_MESSAGE(WM_MSGID(EID_REMOTE_VIDEO_STAT), &CVideoDlg::OnRemoteVideoStats)
	ON_MESSAGE(WM_MSGID(EID_LOCAL_VIDEO_STAT), &CVideoDlg::OnLocalVideoStats)

	ON_MESSAGE(WM_MSGID(EID_START_RCDSRV), &CVideoDlg::OnStartRecordingService)
	ON_MESSAGE(WM_MSGID(EID_STOP_RCDSRV), &CVideoDlg::OnStopRecordingService)

    //ON_MESSAGE(WM_MSGID(EID_STREAM_MESSAGE), &CVideoDlg::OnStreamMessage)
    ON_MESSAGE(WM_MSGID(EID_USER_JOINED), &CVideoDlg::OnEIDUserJoined)
    ON_MESSAGE(WM_MSGID(EID_AUDIO_QUALITY), &CVideoDlg::OnEIDAudioQuality)
	ON_MESSAGE(WM_NETWORK_QUALITY, &CVideoDlg::OnNetworkQuality)
    ON_MESSAGE(WM_CLEANSCREEN_CMD, &CVideoDlg::OnCleanScreenBtnClicked)
	ON_MESSAGE(WM_ChannelJoined, &CVideoDlg::OnJoinRTMChannelSuccess)
	ON_MESSAGE(WM_JOINCHANNEL_AUDIO, &CVideoDlg::OnJoinChannelAudioMuted)
	ON_MESSAGE(WM_GIVEBACK_REMOTE_CONTROL, &CVideoDlg::OnClientGiveBackRemoteControl)
	ON_MESSAGE(WM_SHOW_PARTICIPANT, &CVideoDlg::OnShowParticipant)
	ON_MESSAGE(WM_VK_ESCAPE, &CVideoDlg::OnEscapeKey)
	ON_MESSAGE(WM_RECONNECTION_UI_CLOSE, &CVideoDlg::OnReconnectionUIClose)
	ON_MESSAGE(WM_CLIENT_DROPED, &CVideoDlg::OnClientDroped)

	ON_BN_CLICKED(IDC_BTNMIN_VIDEO, &CVideoDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE_VIDEO, &CVideoDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNRSTO_VIDEO, &CVideoDlg::OnBnClickedBtnrest)
	ON_BN_CLICKED(IDC_BTNENDCALL_VIDEO, &CVideoDlg::OnBnClickedBtnclose)

    //ON_BN_CLICKED(IDC_BTNMPARTS_VIDEO, &CVideoDlg::OnBnClickedBtnShowParticipant)
	ON_BN_CLICKED(IDC_BTNANNO_VIDEO, &CVideoDlg::OnBnClickedBtnanno)
	ON_BN_CLICKED(IDC_BTNREMOTE_VIDEO, &CVideoDlg::OnBnClickedBtnremote)
	ON_BN_CLICKED(IDC_BTNERASER, &CVideoDlg::OnBnClickedBtnEraser)
	//ON_BN_CLICKED(IDC_BTNSCRCAP_VIDEO, &CVideoDlg::OnBnClickedBtnScreenCapture)
    ON_BN_CLICKED(IDC_BTNSETUP_VIDEO, &CVideoDlg::OnBnClickedBtnsetup)
	//ON_BN_CLICKED(IDC_BTNMODE_VIDEO, &CVideoDlg::OnBnClickedBtnmode)
	ON_BN_CLICKED(IDC_BTNAUDIO_VIDEO, &CVideoDlg::OnBnClickedBtnaudio)
	ON_BN_CLICKED(IDC_BTNRECORDING_VIDEO, &CVideoDlg::OnBnClickedRecording)

    //ON_BN_CLICKED(ID_IDR_VIDEOINFO, &CVideoDlg::OnBnClickedBtntip)
   // ON_BN_CLICKED(ID_IDR_DEVICE, &CVideoDlg::OnBnClickedBtnsetup)
    //ON_BN_CLICKED(ID_IDR_FILTER, &CVideoDlg::OnBnClickedBtnfilter)


	ON_BN_CLICKED(IDC_BTNSCR_VIDEO, &CVideoDlg::OnBnClickedBtnfullscr)
	
	//ON_BN_CLICKED(ID_SCRSHARE_DESKTOPSHARE, &CVideoDlg::OnBnClickedScreenshare)
	//ON_BN_CLICKED(ID_SCRSHARE_WINDOWSHARE, &CVideoDlg::OnBnClickedWindowshare)

	ON_WM_SHOWWINDOW()
    ON_WM_MOVE()
END_MESSAGE_MAP()


LRESULT CVideoDlg::OnMemberStatusEvent(WPARAM wParam, LPARAM lParam)
{
	
	if (G_RTM_MEMBER_STATUS_EVENT.StatusEvent == USER_LIST)
	{
		logInfo("==============OnMemberStatusEvent.USER_LIST=============");
		m_ParticipantsUI->onUserListUpdate(G_RTM_MEMBER_STATUS_EVENT.memberList.memberlist, G_RTM_MEMBER_STATUS_EVENT.memberList.count);
	}
	else if (G_RTM_MEMBER_STATUS_EVENT.StatusEvent == USER_LEAVE_CHANNEL)
	{
		logInfo("==============OnMemberStatusEvent.USER_LEAVE_CHANNEL:" + to_string(G_RTM_MEMBER_STATUS_EVENT.uid) + " left the channel.=============");
		//if(G_RTM_MEMBER_STATUS_EVENT.uid >= 1000)
		//{
		//	// host id > 1000
		//	logInfo("Host left the Meeting");
		//	OnBnClickedBtnclose();
		//	DisplayTip(_T("Host left the Meeting"));
		//}
		//else
		//{
		//	wstring left_name = m_ParticipantsUI->onUserLeave(G_RTM_MEMBER_STATUS_EVENT.uid);
		//	if (left_name != L"")
		//	{
		//		PParticipant_Left pdata = new _participant_left;
		//		pdata->uid = G_RTM_MEMBER_STATUS_EVENT.uid;
		//		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		//		{
		//			::SendMessage(m_AgoraMedaiSourceDlg_hwnd, WM_PARTICIPANT_LEAVE, (WPARAM)pdata, lParam);
		//		}
		//		if (G_RTM_MEMBER_STATUS_EVENT.uid == m_other_anno_id)
		//		{
		//			mMeetingStatus = MEETING_NORMAL;
		//			m_other_anno_id = -1;
		//			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		//			m_btnAnnotation.EnableWindow(true);
		//			string message = "User ";
		//			wstring wmessage = StringUtil::StringToWstring(message) + left_name;
		//			message = " left the Meeting, and clean annotation.";
		//			wmessage = wmessage + StringUtil::StringToWstring(message);
		//			DisplayTip(wmessage.c_str());
		//			logInfo(StringUtil::WStringToString(wmessage));
		//			onStopAnnotation();
		//			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		//			{
		//				m_ParticipantsUI->updateUIForRemoteControl(0, false);
		//			}
		//		}
		//		else if (G_RTM_MEMBER_STATUS_EVENT.uid == m_other_remote_id)
		//		{
		//			mMeetingStatus = MEETING_NORMAL;
		//			m_other_remote_id = -1;
		//			m_btnRemoteControl.ShowWindow(SW_HIDE);
		//			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		//			m_btnAnnotation.EnableWindow(true);
		//			string message = "User ";
		//			wstring wmessage = StringUtil::StringToWstring(message) + left_name;
		//			message = " left the Meeting, and stop remote control.";
		//			wmessage = wmessage + StringUtil::StringToWstring(message);
		//			DisplayTip(wmessage.c_str());
		//			logInfo(StringUtil::WStringToString(wmessage));
		//			m_ParticipantsUI->setRemoteControlling(false);
		//			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		//			{
		//				m_ParticipantsUI->updateUIForRemoteControl(0, false);
		//			}
		//		}
		//		else
		//		{
		//			string message = "User ";
		//			wstring wmessage = StringUtil::StringToWstring(message) + left_name;
		//			message = " left the Meeting";
		//			wmessage = wmessage + StringUtil::StringToWstring(message);
		//			DisplayTip(wmessage.c_str());
		//			logInfo(StringUtil::WStringToString(wmessage));
		//		}
		//		vector<Individual*> participants = ClientsManager::getInstance()->getAllParticipants();
		//		if (participants.size() == 1 && atoi(participants[0]->getUserId().c_str()) >= 1000)
		//		{
		//			logInfo("All clients left, so end current meeting.");
		//			OnBnClickedBtnclose();
		//			DisplayTip(_T("All clients left, so end current meeting."));
		//		}
		//	}
		//	else
		//	{
		//		logInfo("The left one is not in the participant list.");
		//	}
		//	
		//}
	}
	else if (G_RTM_MEMBER_STATUS_EVENT.StatusEvent == USER_JOIN_CHANNEL)
	{
		logInfo("==============OnMemberStatusEvent.USER_JOIN_CHANNEL:" + to_string(G_RTM_MEMBER_STATUS_EVENT.uid) + " joined the channel.=============");

		////m_ParticipantWnd.onUserJoined(G_RTM_MEMBER_STATUS_EVENT.uid);
		//Individual* individual = m_ParticipantsUI->onUserJoined(G_RTM_MEMBER_STATUS_EVENT.uid);
		//if (individual->getStatus() == USER_STATUS_RECONNECTED_SUCCESSFULLY)
		//{
		//	individual->setStatus(USER_STATUS_JOIN_MEETING);
		//	wstring message = individual->getWName() + L" reconnected successfully.";
		//	logInfo(StringUtil::WStringToString(message));
		//	DisplayTip(message.c_str());
		//	if (G_RTM_MEMBER_STATUS_EVENT.uid >= 1000)
		//	{
		//		onCloseReconnectingUI();
		//	}
		//	else
		//	{
		//		map<UINT, HANDLE>::iterator iter = m_thread_map.find(G_RTM_MEMBER_STATUS_EVENT.uid);
		//		if (iter != m_thread_map.end())
		//		{
		//			logInfo("Client " + individual->getUserId() + " has been recovered from offline to online");
		//			HANDLE thread = iter->second;
		//			if (thread != NULL)   //线程正在运行
		//			{
		//				TerminateThread(thread, 0);
		//				if (thread != NULL)
		//				{
		//					CloseHandle(thread);   //关闭线程句柄，可能线程已结束，但句柄还没释放
		//					thread = NULL;
		//				}
		//				logInfo("Terminate the thread for " + iter->first);
		//			}
		//			m_thread_map.erase(iter);
		//		}
		//	}
		//}
		//else
		//{
		//	if (individual->getWName() != L"")
		//	{
		//		wstring message = individual->getWName() + L" joined the Meeting.";
		//		logInfo(StringUtil::WStringToString(message));
		//		DisplayTip(message.c_str());
		//	}
		//	
		//	if (this->mMeetingStatus == SELF_REMOTE_CONTROL)
		//	{
		//		CAgoraObject::GetAgoraObject()->SendMessageToPeer(G_RTM_MEMBER_STATUS_EVENT.uid, REMOTE_CONTROL_START_CMD);
		//	}
		//	else if (this->mMeetingStatus == SELF_ANNOTATION)
		//	{
		//		CAgoraObject::GetAgoraObject()->SendMessageToPeer(G_RTM_MEMBER_STATUS_EVENT.uid, ANNOTATE_START_CMD);
		//	}
		//}

	}

	return 0;
}


void CVideoDlg::OnBnClickedRecording()
{

}

void CVideoDlg::OnNcCalcSize(BOOL wParam, NCCALCSIZE_PARAMS* lParam)
{
	CDialogEx::OnNcCalcSize(wParam, lParam);
}

// CVideoDlg 消息处理程序

void CVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	//if (m_btnMin.GetSafeHwnd() != NULL)
	//	m_btnMin.MoveWindow(cx - 153, 1, 50, 30, TRUE);
	//if (m_btnRst.GetSafeHwnd() != NULL)
	//	m_btnRst.MoveWindow(cx - 102, 1, 50, 30, TRUE);
	//if (m_btnClose.GetSafeHwnd() != NULL)
	//	m_btnClose.MoveWindow(cx - 51, 1, 50, 30, TRUE);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int width = getResolutionWidth(FUN_BUTTON_WIDTH);
		int height = getResolutionHeight(FUN_BUTTON_HEIGHT);
		if (m_btnMin.GetSafeHwnd() != NULL)
			m_btnMin.MoveWindow(cx - width*2, 0, width, height, TRUE);
		if (m_btnRst.GetSafeHwnd() != NULL)
			m_btnRst.MoveWindow(cx, 0, width, height, TRUE);
		if (m_btnClose.GetSafeHwnd() != NULL)
			m_btnClose.MoveWindow(cx - width, 0, width, height, TRUE);
	}
	else
	{
		if (m_btnMin.GetSafeHwnd() != NULL)
			m_btnMin.MoveWindow(cx - 153, 1, 50, 30, TRUE);
		if (m_btnRst.GetSafeHwnd() != NULL)
			m_btnRst.MoveWindow(cx - 102, 1, 50, 30, TRUE);
		if (m_btnClose.GetSafeHwnd() != NULL)
			m_btnClose.MoveWindow(cx - 51, 1, 50, 30, TRUE);
	}


	GetClientRect(&m_rcVideoArea);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int top = getResolutionHeight(MAX_BAR_HEIGHT);
		int bottom = getResolutionHeight(MAX_BOTTOM_HEIGHT);
		m_rcVideoArea.top += top;
		m_rcVideoArea.bottom -= bottom;
		if (m_ParticipantsUI && ::IsWindow(m_ParticipantsUI->GetHWND()) && ::IsWindowVisible(m_ParticipantsUI->GetHWND()))
			m_rcVideoArea.right -= cx / 5;
	}
	else
	{
		m_rcVideoArea.top += 32;
		m_rcVideoArea.bottom -= 72;
		if (m_ParticipantsUI && ::IsWindow(m_ParticipantsUI->GetHWND()) && ::IsWindowVisible(m_ParticipantsUI->GetHWND()))
			m_rcVideoArea.right -= cx / 5;// PARTICIPANT_WINDOW_WIDTH;
	}



	// 2人， 右上角子画面区域
	m_rcChildVideoArea.top = m_rcVideoArea.top + 10;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 14;
	m_rcChildVideoArea.left = m_rcChildVideoArea.right - 192;

	//if (::IsWindow(m_ParticipantWnd.GetHWND()) && ::IsWindowVisible(m_ParticipantWnd.GetHWND())) {
	//	
	//	RECT participateRect;

	//	GetClientRect(&participateRect);

	//	ClientToScreen(&participateRect);
	//	participateRect.top += 24;
	//	participateRect.bottom -= 72;
	//	participateRect.left = participateRect.right- PARTICIPANT_WINDOW_WIDTH;
	//	::MoveWindow(m_ParticipantWnd.GetHWND(), participateRect.left, participateRect.top, participateRect.right - participateRect.left, participateRect.bottom - participateRect.top, TRUE);
	//}

	if (m_ParticipantsUI && ::IsWindow(m_ParticipantsUI->GetHWND()) && ::IsWindowVisible(m_ParticipantsUI->GetHWND())) {

		RECT participateRect;

		GetClientRect(&participateRect);

		ClientToScreen(&participateRect);
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			int top = getResolutionHeight(MAX_BAR_HEIGHT);
			int bottom = getResolutionHeight(MAX_BOTTOM_HEIGHT);
			participateRect.top += top;
			participateRect.bottom -= bottom;
			participateRect.left = participateRect.right - cx / 5;
		}
		else
		{
			participateRect.top += 32;
			participateRect.bottom -= 72;
			participateRect.left = participateRect.right - cx / 5;// PARTICIPANT_WINDOW_WIDTH;
		}

		::MoveWindow(m_ParticipantsUI->GetHWND(), participateRect.left, participateRect.top, participateRect.right - participateRect.left, participateRect.bottom - participateRect.top, TRUE);
	}

	if (::IsWindow(m_DrawLayerDlg.GetSafeHwnd()) && m_DrawLayerDlg.IsWindowVisible()) {
		CRect   rcDrawArea;
		rcDrawArea.CopyRect(&m_rcVideoArea);
		//rcChatArea.top = rcChatArea. - 150;
		ClientToScreen(&rcDrawArea);
		m_DrawLayerDlg.MoveWindow(&rcDrawArea);
	}




	if (m_wndLocal.GetSafeHwnd() == NULL || m_wndVideo[0].GetSafeHwnd() == NULL)
		return;

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		AdjustSizeVideo1(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEO4:
		AdjustSizeVideo4(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEOMULTI:
		AdjustSizeVideoMulti(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	default:
		break;
	}

	Invalidate(TRUE);
}

void CVideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		if (nID == SC_MOVE || nID == 0xF012)
			return;
	}
	if (nID == SC_RESTORE)
	{
		m_ParticipantsUI->ShowWindow(true);
	}
	CDialogEx::OnSysCommand(nID, lParam);

}

BOOL CVideoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!CAgoraObject::GetAgoraObject()->GetSelfHost() && mMeetingStatus == SELF_REMOTE_CONTROL && !CAGConfig::GetInstance()->isRemoteMouseShow())
	{
		CPoint p;
		CRect prect;
		GetClientRect(&prect);
		GetCursorPos(&p);
		ScreenToClient(&p);
		CRect rect;
		GetDlgItem(IDC_BASEWND_VIDEO)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		if (rect.PtInRect(p))
		{
			SetCursor(NULL);
			return true;
		}
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	
}

void CVideoDlg::ShowButtonsNormal()
{
	CRect	rcClient;
	int nShowMode = SW_HIDE;

	if (!m_bFullScreen)
		nShowMode = SW_SHOW;

    //m_btnParticipaent.ShowWindow(nShowMode);
	//m_btnRecording.ShowWindow(nShowMode);

	
	//m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnAudio.ShowWindow(nShowMode);
	//m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnEndCall.ShowWindow(nShowMode);
	//m_btnScrCap.ShowWindow(nShowMode);
	if (m_btnShow.GetSafeHwnd() != NULL)
	{
		m_btnShow.ShowWindow(nShowMode);
	}
	GetClientRect(&rcClient);
	AdjustButtonsNormal(rcClient.Width(), rcClient.Height());
}


void CVideoDlg::AdjustButtonsNormal(int cx, int cy)
{
//	if (m_btnSetup.GetSafeHwnd() != NULL)
//		m_btnSetup.MoveWindow(30, cy - 48, 24, 24, TRUE);

	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int size = getResolutionSize(MAX_VIDEO_BUTTON_SIZE);
		int bottom = getResolutionHeight(MAX_BOTTOM_HEIGHT);
		int offset_y = bottom / 2 + size / 2;
		if (m_btnRemoteControl.GetSafeHwnd() != NULL)
			m_btnRemoteControl.MoveWindow(cx / 2 - size * 8, cy - offset_y, size, size, TRUE);

		if (m_btnAnnotation.GetSafeHwnd() != NULL)
			m_btnAnnotation.MoveWindow(cx / 2 - 13 * size / 2, cy - offset_y, size, size, TRUE);

		if (m_btnEraser.GetSafeHwnd() != NULL)
			m_btnEraser.MoveWindow(cx / 2 - size * 5, cy - offset_y, size, size, TRUE);

		if (m_btnAudio.GetSafeHwnd() != NULL)
			m_btnAudio.MoveWindow(cx / 2 - size * 2, cy - offset_y, size, size, TRUE);

		if (m_btnEndCall.GetSafeHwnd() != NULL)
			m_btnEndCall.MoveWindow(cx / 2 - size / 2, cy - offset_y, size, size, TRUE);

		if (m_btnRecording.GetSafeHwnd() != NULL)
			m_btnRecording.MoveWindow(cx / 2 + size, cy - offset_y, size, size, TRUE);

		//if (m_btnScrCap.GetSafeHwnd() != NULL)
		//	m_btnScrCap.MoveWindow(cx / 2 + 72, cy - 60, 48, 48, TRUE);
		//if (m_btnSetup.GetSafeHwnd() != NULL)
		//	m_btnSetup.MoveWindow(cx / 2 + 11 * size / 2, cy - offset_y, size, size, TRUE);

		//if (m_btnShow.GetSafeHwnd() != NULL)
		//	m_btnShow.MoveWindow(cx - 72, cy - 48, 96, 96, TRUE);
	}
	else
	{
		if (m_btnRemoteControl.GetSafeHwnd() != NULL)
			m_btnRemoteControl.MoveWindow(cx / 2 - 450, cy - 60, 48, 48, TRUE);

		if (m_btnAnnotation.GetSafeHwnd() != NULL)
			m_btnAnnotation.MoveWindow(cx / 2 - 380, cy - 60, 48, 48, TRUE);

		if (m_btnEraser.GetSafeHwnd() != NULL)
			m_btnEraser.MoveWindow(cx / 2 - 310, cy - 60, 48, 48, TRUE);

		if (m_btnRecording.GetSafeHwnd() != NULL)
			m_btnRecording.MoveWindow(cx / 2 - 240, cy - 60, 48, 48, TRUE);

		if (m_btnAudio.GetSafeHwnd() != NULL)
			m_btnAudio.MoveWindow(cx / 2 - 100, cy - 60, 48, 48, TRUE);

		if (m_btnEndCall.GetSafeHwnd() != NULL)
			m_btnEndCall.MoveWindow(cx / 2 - 24, cy - 60, 48, 48, TRUE);

		if (m_btnRecording.GetSafeHwnd() != NULL)
			m_btnRecording.MoveWindow(cx / 2 + 52, cy - 60, 48, 48, TRUE);

		//if (m_btnScrCap.GetSafeHwnd() != NULL)
		//	m_btnScrCap.MoveWindow(cx / 2 + 72, cy - 60, 48, 48, TRUE);
		//if (m_btnSetup.GetSafeHwnd() != NULL)
		//	m_btnSetup.MoveWindow(cx / 2 + 264, cy - 60, 48, 48, TRUE);

		if (m_btnShow.GetSafeHwnd() != NULL)
			m_btnShow.MoveWindow(cx - 72, cy - 48, 48, 48, TRUE);
	}

}




void CVideoDlg::AdjustSizeVideo1(int cx, int cy)
{
	if (m_listWndInfo.GetCount() == 0)
		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
	else {
		m_wndVideo[0].MoveWindow(&m_rcVideoArea, FALSE);
		m_wndLocal.MoveWindow(&m_rcChildVideoArea, FALSE);
	}
}

void CVideoDlg::AdjustSizeVideo4(int cx, int cy)
{
	m_wndVideo[0].MoveWindow(0, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[1].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[2].MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[3].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);

	if (m_listWndInfo.GetCount() == 2)
		m_wndLocal.MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	else
		m_wndLocal.MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
}

void CVideoDlg::AdjustSizeVideoMulti(int cx, int cy)
{
	int nLocalIndex = 0;

	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.top + 16;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, 192, 144, TRUE);
				nLocalIndex++;
			}
		}
		else{
			m_wndLocal.MoveWindow(nXPos, nYPos, 192, 144, TRUE);
			nLocalIndex++;
		}
	}
}


BOOL CVideoDlg::NeedShowSizeBox(CPoint point)
{
	CRect rcClient;

	GetClientRect(&rcClient);

	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 5;
	rcClient.bottom -= 5;
	
	if (rcClient.PtInRect(point) || IsZoomed())
		return FALSE;

	return TRUE;
}



void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	DrawHead(&dc);
}


LRESULT CVideoDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CVideoDlg::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}

void CVideoDlg::OnBnClickedBtnclose()
{
	logInfo("==================VideoDlg:OnBnClickedBtnclose================");
	//PlaySound((LPCTSTR)IDR_WAVE1, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
	if (m_bFullScreen)
	{
		OnBnClickedBtnfullscr();
	}
	m_listWndInfo.RemoveAll();
    //m_dlgChat.ShowWindow(SW_HIDE);
	this->ShowWindow(SW_NORMAL);
	m_DrawLayerDlg.ShowWindow(SW_HIDE);
    m_dlgDevice.ShowWindow(SW_HIDE);	
	//m_ParticipantWnd.ShowWindow(SW_HIDE);
	//m_bShowParticipantWnd = false;
	m_ParticipantsUI->ShowWindow(false);
	m_ParticipantsUI->setRemoteControlling(false);
	m_ParticipantsUI->resetSelectedVideoMode();
	//m_btnParticipaent.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_btnRemoteControl.ShowWindow(SW_HIDE);
	m_btnEraser.ShowWindow(SW_HIDE);
	m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_btnAnnotation.EnableWindow(true);
	mMeetingStatus = MEETING_NORMAL;


	
    // unmute local video
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		CAgoraObject::GetAgoraObject()->MuteLocalVideo(FALSE);
	}	
	else
	{
		if (m_reconnectingUI && ::IsWindowVisible(m_reconnectingUI->GetHWND()))
		{
			m_reconnectingUI->Close();
		}
	}

	map<UINT, HANDLE>::iterator iter = m_thread_map.begin();
	while (iter != m_thread_map.end())
	{
		HANDLE thread = iter->second;
		if (thread != NULL)   //线程正在运行
		{
			TerminateThread(thread, 0);
			if (thread != NULL)
			{
				CloseHandle(thread);   //关闭线程句柄，可能线程已结束，但句柄还没释放
				thread = NULL;
			}
			logInfo("Terminate the thread for " + iter->first);
		}
		m_thread_map.erase(iter);
	}

    // unmute local audio
    CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);
    m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

    CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, 0, NULL, FALSE);
    //m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

    //m_dlgChat.ShowWindow(SW_HIDE);
	m_DrawLayerDlg.CleanScreen();
	//m_DrawLayerDlg.onc
    //m_dlgChat.ClearHistory();
    //m_btnParticipaent.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
    GetParent()->SendMessage(WM_LEAVECHANNEL, 0, 0);

	CDialogEx::OnOK();
}

void CVideoDlg::OnBnClickedBtnrest()
{
	if (IsZoomed()) {
		m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_RESTORE);
	}
	else{
		m_btnRst.SetBackImage(IDB_BTNRESTORE, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_MAXIMIZE);
	}

	CRect	rcClient;

	GetClientRect(&rcClient);
	m_rcVideoArea.CopyRect(&rcClient);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int top = getResolutionHeight(MAX_BAR_HEIGHT);
		m_rcVideoArea.top += top;
	}
	else
	{
		m_rcVideoArea.top += 32;
	}
	
	m_rcVideoArea.bottom -= 72;
	  
	m_rcChildVideoArea.top = m_rcVideoArea.top + 16;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.left = m_rcVideoArea.right - 212;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 20;





	int cx = rcClient.Width();
	int cy = rcClient.Height();


	this->OnSize(0, cx, cy);

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		AdjustSizeVideo1(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEO4:
		AdjustSizeVideo4(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEOMULTI:
		AdjustSizeVideoMulti(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	default:
		break;
	}

	Invalidate(TRUE);
}


void CVideoDlg::OnBnClickedBtnfullscr()
{
	//just for client
	int nShowMode = 0;

	if (m_bFullScreen) {
		m_bFullScreen = FALSE;
		nShowMode = SW_SHOW;		
		ShowWindow(SW_RESTORE);
		OnBnClickedBtnrest();
		GetClientRect(&m_rcVideoArea);
		m_rcVideoArea.top += 32;		
		m_rcVideoArea.bottom -= 72;	
		if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			m_ParticipantsUI->ShowWindow(true);
			RECT rect;
			GetClientRect(&rect);
			this->OnSize(0, rect.right - rect.left, rect.bottom - rect.top);
		}
	}
	else{
		m_bFullScreen = TRUE;
		nShowMode = SW_HIDE;
		//RECT rect;
		//GetClientRect(&rect);
		//rect.left = 0;
		//rect.top = 0;
		if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			m_ParticipantsUI->ShowWindow(false);
		}

		//MoveWindow(&rect);
		ShowWindow(SW_MAXIMIZE);
		
		GetClientRect(&m_rcVideoArea);
	}

	//m_ParticipantWnd.ShowWindow(FALSE);
	//m_ParticipantsUI->ShowWindow(false);
	//m_bShowParticipantWnd = FALSE;

	m_btnMin.ShowWindow(nShowMode);
	m_btnRst.ShowWindow(nShowMode);
	m_btnClose.ShowWindow(nShowMode);
	if (nShowMode == SW_SHOW)
	{
		if (mMeetingStatus == SELF_REMOTE_CONTROL)
		{
			m_btnRemoteControl.ShowWindow(nShowMode);			
		}
		else if (mMeetingStatus == SELF_ANNOTATION)
		{
			m_btnEraser.ShowWindow(nShowMode);
		}
	}
	else
	{
		m_btnRemoteControl.ShowWindow(nShowMode);
		m_btnEraser.ShowWindow(nShowMode);
	}

	m_btnAnnotation.ShowWindow(nShowMode);


	//m_btnRecording.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);

    //m_btnSetup.ShowWindow(nShowMode);
	
	m_btnShow.ShowWindow(nShowMode);

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		ShowVideo1();
		break;
	case SCREEN_VIDEO4:
		ShowVideo4();
		break;
	case SCREEN_VIDEOMULTI:
		ShowMulti();
		break;
	default:
		break;
	}
	if (::IsWindow(m_DrawLayerDlg.GetSafeHwnd()) && m_DrawLayerDlg.IsWindowVisible()) {
		CRect   rcDrawArea;
		rcDrawArea.CopyRect(&m_rcVideoArea);
		//rcChatArea.top = rcChatArea. - 150;
		ClientToScreen(&rcDrawArea);
		m_DrawLayerDlg.MoveWindow(&rcDrawArea);
	}

	Invalidate(TRUE);
}
/*
void CVideoDlg::OnBnClickedScreenshare()
{
	m_dlgDesktopCapture.SaveScreen(NULL);
	m_dlgDesktopCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::OnBnClickedWindowshare()
{
	m_dlgScreenCapture.RefreashWndInfo();
	m_dlgScreenCapture.SaveScreen(NULL);

	m_dlgScreenCapture.ShowWindow(SW_MAXIMIZE);
}
*/
void CVideoDlg::ShowControlButton(BOOL bShow)
{
	int nShowMode = 0;
	if (bShow)
		nShowMode = SW_SHOW;
	else
		nShowMode = SW_HIDE;

	//m_btnScrCap.ShowWindow(nShowMode);
    //m_btnParticipaent.ShowWindow(nShowMode);
	//m_btnRecording.ShowWindow(nShowMode);
    //m_btnSetup.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	if(m_btnShow.GetSafeHwnd() != NULL)
	{ 
		m_btnShow.ShowWindow(nShowMode);
	}
	m_btnEndCall.ShowWindow(nShowMode);
}
/*
void CVideoDlg::OnBnClickedBtntip()
{
	for (int nIndex = 0; nIndex < 4; nIndex++)
        m_wndVideo[nIndex].ShowVideoInfo(!m_bShowInfo);

    m_bShowInfo = !m_bShowInfo;
}

void CVideoDlg::OnBnClickedBtnScreenCapture()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
	
	if (lpAgora->IsScreenCaptureEnabled()) {
		lpAgora->EnableScreenCapture(NULL, 0, NULL, FALSE);
		m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
        CAgoraObject::GetAgoraObject()->EnableLocalRender(TRUE);
	}
	else
		CreateScreenShareMenu();

	Invalidate();
}
*/


/*
void CVideoDlg::CreateScreenShareMenu()
{
	CRect				rcButton;
	CMenu				Menu;
	MENUITEMINFO		mii;

	Menu.LoadMenu(IDR_MENU_SCRSHARE);

	CMenu *pMenuPopup = Menu.GetSubMenu(0);
	
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_SCRSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_SCRSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
	
	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_WNDSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_WNDSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
	m_btnScrCap.GetWindowRect(&rcButton);

	int x = rcButton.left - 60;
	int y = rcButton.top - 50;

	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
	pMenuPopup->DestroyMenu();
}


void CVideoDlg::CreateMoreMenu()
{
    CRect				rcButton;
    CMenu				Menu;
    MENUITEMINFO		mii;
    UINT                nFlag = 0;

    Menu.LoadMenu(IDR_MENU_MORE);

    CMenu *pMenuPopup = Menu.GetSubMenu(0);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREINFO");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREINFO")) + 1;
    pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_VIDEOINFO, MF_BYCOMMAND, &m_bitMenuInfo, &m_bitMenuInfo);
    if (m_bShowInfo)
        nFlag = MF_BYCOMMAND | MF_CHECKED;
    else
        nFlag = MF_BYCOMMAND | MF_UNCHECKED;
    pMenuPopup->CheckMenuItem(ID_IDR_VIDEOINFO, nFlag);

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREDEVICE");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREDEVICE")) + 1;
    pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_DEVICE, MF_BYCOMMAND, &m_bitMenuDevice, &m_bitMenuDevice);

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREFILTER");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREFILTER")) + 1;
    pMenuPopup->SetMenuItemInfo(2, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_FILTER, MF_BYCOMMAND, &m_bitMenuFilter, &m_bitMenuFilter);
    if (m_bFilter)
        nFlag = MF_BYCOMMAND | MF_CHECKED;
    else
        nFlag = MF_BYCOMMAND | MF_UNCHECKED;
    pMenuPopup->CheckMenuItem(ID_IDR_FILTER, nFlag);
    
    
    m_btnMore.GetWindowRect(&rcButton);

    int x = rcButton.left - 60;
    int y = rcButton.top - 75;

    pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
    pMenuPopup->DestroyMenu();
}
*/
void CVideoDlg::OnBnClickedBtnsetup()
{
	m_dlgDevice.ShowWindow(SW_SHOW);
	m_dlgDevice.CenterWindow();
}

/*
void CVideoDlg::OnBnClickedBtnfilter()
{
    IRtcEngine *lpEngine = CAgoraObject::GetEngine();

    if (!m_bFilter){
        load_preprocessing_plugin(lpEngine);
    }
    else{
        unload_preprocessing_plugin(lpEngine);
    }

    m_bFilter = !m_bFilter;
}
*/
void CVideoDlg::OnBnClickedBtnEraser()
{
	CommandManager::GetInstance()->AnnotationClear();
	m_DrawLayerDlg.CleanScreen();
}
void CVideoDlg::OnBnClickedBtnremote()
{
	//m_bRemotingControling = !m_bRemotingControling;
	//
	//	if (!m_bRemotingControling)
	//	{
	//		m_btnRemoteControl.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	//		mMeetingStatus = MEETING_NORMAL;
	//	}

	//	else
	//	{
	//		m_btnRemoteControl.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	//		mMeetingStatus = SELF_REMOTE_CONTROL;
	//	}
	CommandManager::GetInstance()->RemoteControlEnd();
	mMeetingStatus = MEETING_NORMAL;
	m_btnRemoteControl.ShowWindow(SW_HIDE);
	m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_btnAnnotation.EnableWindow(true);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		m_ParticipantsUI->updateUIForRemoteControl(0, false);
	}
	else
	{
		m_ParticipantsUI->updateUIForRemoteControl(CAgoraObject::GetAgoraObject()->GetSelfUID(), false);
	}
	m_ParticipantsUI->setRemoteControlling(false);
	


}


//void CVideoDlg::OnBnClickedBtnShowParticipant()
//{
//
//	//m_ParticipantWnd.ShowWindow(!m_bShowParticipantWnd);
//	m_ParticipantsUI->ShowWindow(!m_bShowParticipantWnd);
//	m_bShowParticipantWnd = !m_bShowParticipantWnd;
//	if (!m_bShowParticipantWnd)
//		m_btnParticipaent.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
//	else
//		m_btnParticipaent.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	//RECT rect;
	//GetClientRect(&rect);
	//this->OnSize(0, rect.right - rect.left, rect.bottom - rect.top);
//
//}

void CVideoDlg::OnBnClickedBtnanno()
{

	if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		if (mMeetingStatus == MEETING_NORMAL)
		{
			CommandManager::GetInstance()->RequestAnnotation();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
			m_btnAnnotation.EnableWindow(false);
		}		
		else if (mMeetingStatus == SELF_ANNOTATION)
		{
			onStopAnnotation();
			CommandManager::GetInstance()->AnnotationEnd();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
			m_btnEraser.ShowWindow(SW_HIDE);
			mMeetingStatus = MEETING_NORMAL;
		}

	}
		
	else
	{
		if (mMeetingStatus == MEETING_NORMAL)
		{
			CommandManager::GetInstance()->AnnotationStart();
			onStartAnnotation();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
			m_btnEraser.ShowWindow(SW_SHOW);
			mMeetingStatus = SELF_ANNOTATION;
			m_ParticipantsUI->updateUIForAnnotation(true);
		}
		else if (mMeetingStatus == SELF_ANNOTATION)
		{
			mMeetingStatus = MEETING_NORMAL;
			CommandManager::GetInstance()->AnnotationEnd();
			onStopAnnotation();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
			m_btnEraser.ShowWindow(SW_HIDE);
			m_ParticipantsUI->updateUIForAnnotation(false);
		}
		
	}
}

/*
void CVideoDlg::OnBnClickedBtnmode()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
    IRtcEngine *lpRtcEngine = CAgoraObject::GetEngine();

	if (!lpAgora->IsLocalVideoMuted()) {
		lpAgora->MuteLocalVideo(TRUE);
        m_btnMode.SetBackImage(IDB_BTNVIDEO_VIDEO);
        
        CAgoraObject::GetAgoraObject()->EnableLocalRender(FALSE);
        m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96);
        m_wndLocal.ShowBackground(TRUE);
	}
	else {
        lpAgora->EnableVideo(TRUE);
		lpAgora->MuteLocalVideo(FALSE);
        CAgoraObject::GetAgoraObject()->EnableLocalRender(TRUE);
        m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO);
        m_wndLocal.ShowBackground(FALSE);
	}
    
    m_wndLocal.Invalidate(TRUE);
    m_wndLocal.UpdateWindow();
    
    Invalidate(TRUE);
    UpdateWindow();
}
*/

void CVideoDlg::OnBnClickedBtnaudio()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();

	if (lpAgora->IsLocalAudioMuted()) {
		lpAgora->MuteLocalAudio(FALSE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	}
	else {
		lpAgora->MuteLocalAudio(TRUE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	}
}

LRESULT CVideoDlg::OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	logInfo("Success to join channel.");
	m_joinChannel_time = GetTickCount();
	if (m_AgoraMedaiSourceDlg_hwnd != NULL)
	{
		::SendMessage(m_AgoraMedaiSourceDlg_hwnd, WM_JOINCHANNEL_SUCCESS, wParam, lParam);
	}
	LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)wParam;

	//m_listWndInfo.RemoveAll();
	CAgoraObject::GetAgoraObject()->SetSelfUID(lpData->uid);
	CAgoraObject::GetAgoraObject()->setInChannel(true);
	this->ShowWindow(SW_MAXIMIZE);
	m_ParticipantsUI->ShowWindow(true);

	RECT rect;
	GetClientRect(&rect);
	this->OnSize(0, rect.right - rect.left, rect.bottom - rect.top);
	m_btnRst.SetBackImage(IDB_BTNRESTORE, RGB(0xFF, 0x00, 0xFF));

	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int divider = CAGConfig::GetInstance()->GetPictureDivider();
		if (divider == 2)
		{
			logInfo("Host select VIDEO_PBYP_MODE, and broadcast it in the channel.");
			m_ParticipantsUI->selectVideoMode(VIDEO_PBYP_MODE, true, true);
		}
		else if (divider == 4)
		{
			logInfo("Host select VIDEO_WINMODE1 and VIDEO_HDMI1_WIN1, broadcast it in the channel.");
			m_ParticipantsUI->selectVideoMode(VIDEO_WINMODE1, true, true);
			Sleep(20);
			m_ParticipantsUI->selectVideoMode(VIDEO_HDMI1, true, true);
		}
	}
	delete[] lpData->channel;
	lpData->channel = NULL;
	delete lpData;
	lpData = NULL;
	return 0;
}

LRESULT CVideoDlg::OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REJOINCHANNEL_SUCCESS lpData = (LPAGE_REJOINCHANNEL_SUCCESS)wParam;
	if (CAgoraObject::GetAgoraObject()->GetSelfHost() && m_AgoraMedaiSourceDlg_hwnd != NULL)
	{
		logInfo("Rebuild the socket connection for rejoining channel.");
		::SendMessage(m_AgoraMedaiSourceDlg_hwnd, WM_REJOINCHANNEL_SUCCESS, wParam, lParam);
	}
	if (m_reconnectingUI && ::IsWindowVisible(m_reconnectingUI->GetHWND()))
	{
		onCloseReconnectingUI();
	}
	
	logInfo("Success to rejoin channel.");
	m_joinChannel_time = GetTickCount();
	DisplayTip(L"Success to rejoin the meeting");

	//m_listWndInfo.RemoveAll();
    delete[] lpData->channel;
    lpData->channel = NULL;
    delete lpData;
    lpData = NULL;
	return 0;
}

LRESULT CVideoDlg::OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = (LPAGE_FIRST_LOCAL_VIDEO_FRAME)wParam;
	m_wndLocal.SetVideoResolution(lpData->width, lpData->height);
	logInfo("===========OnEIDFirstLocalFrame==============");
	if (m_listWndInfo.GetCount() == 0)
		ShowVideo1();

	delete lpData;
    lpData = NULL;
	return 0;
}

LRESULT CVideoDlg::OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;
	BOOL bFound = FALSE;

	
	//For performance purpose, Host will not pull the client stream here and the Client will not pull the other stream except those from host
	if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		POSITION pos = m_listWndInfo.GetHeadPosition();
		while (pos != NULL) {
			AGVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
			if (agvWndInfo.nUID == lpData->uid) {
				bFound = TRUE;
				break;
			}
		}

		if (!bFound) {
			AGVIDEO_WNDINFO agvWndInfo;
			memset(&agvWndInfo, 0, sizeof(AGVIDEO_WNDINFO));
			agvWndInfo.nUID = lpData->uid;
			agvWndInfo.nWidth = lpData->width;
			agvWndInfo.nHeight = lpData->height;

			m_listWndInfo.AddTail(agvWndInfo);
		}

		RebindVideoWnd();
	}


	delete lpData;
    lpData = NULL;
	return 0;
}

LRESULT CVideoDlg::OnEIDUserJoined(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;
	logInfo("===========OnEIDUserJoined:" + to_string(lpData->uid) + " joined the meeting.============");	
	Individual* individual = m_ParticipantsUI->onUserJoined(lpData->uid);
	if (individual->getStatus() == USER_STATUS_RECONNECTED_SUCCESSFULLY)
	{
		individual->setStatus(USER_STATUS_JOIN_MEETING);
		wstring message = individual->getWName() + L" reconnected successfully.";
		logInfo(StringUtil::WStringToString(message));
		DisplayTip(message.c_str());
		if (lpData->uid >= 1000)
		{
			onCloseReconnectingUI();
		}
		else
		{
			map<UINT, HANDLE>::iterator iter = m_thread_map.find(lpData->uid);
			if (iter != m_thread_map.end())
			{
				logInfo("Client " + individual->getUserId() + " has been recovered from offline to online");
				HANDLE thread = iter->second;
				if (thread != NULL)   //线程正在运行
				{
					TerminateThread(thread, 0);
					if (thread != NULL)
					{
						CloseHandle(thread);   //关闭线程句柄，可能线程已结束，但句柄还没释放
						thread = NULL;
					}
					logInfo("Terminate the thread for " + iter->first);
				}
				m_thread_map.erase(iter);
			}
		}
	}
	else
	{
		if (individual->getWName() != L"")
		{
			wstring message = individual->getWName() + L" joined the Meeting.";
			logInfo(StringUtil::WStringToString(message));
			DisplayTip(message.c_str());
		}

		if (this->mMeetingStatus == SELF_REMOTE_CONTROL)
		{
			CAgoraObject::GetAgoraObject()->SendMessageToPeer(G_RTM_MEMBER_STATUS_EVENT.uid, REMOTE_CONTROL_START_CMD);
		}
		else if (this->mMeetingStatus == SELF_ANNOTATION)
		{
			CAgoraObject::GetAgoraObject()->SendMessageToPeer(G_RTM_MEMBER_STATUS_EVENT.uid, ANNOTATE_START_CMD);
		}
	}
	if (CAgoraObject::GetAgoraObject()->GetSelfHost() && m_AgoraMedaiSourceDlg_hwnd != NULL)
	{
		::PostMessage(m_AgoraMedaiSourceDlg_hwnd, WM_CLIENT_JOIN_CHANNEL, wParam, lParam);
	}
    

	delete lpData;
	lpData = NULL;
	return 0;
}

LRESULT CVideoDlg::OnEIDUserOffline(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)wParam;
	if (lpData->reason == USER_OFFLINE_DROPPED)
	{
		logInfo("===================OnEIDUserOffline.USER_OFFLINE_DROPPED=================");
		long current = GetTickCount();
		long interval = current - m_joinChannel_time;
		if (interval < 10000)//ms
		{
			logInfo("The interval time is " + to_string(interval) + ", ignore this USER_OFFLINE_DROPPED callback");
			delete lpData;
			lpData = nullptr;
			return 0;
		}
		else
		{
			logInfo("The interval time is " + to_string(interval) + ", handle this USER_OFFLINE_DROPPED callback");
		}
		if (lpData->uid >= 1000)
		{
			logInfo("Host drop offline, waiting for its reconnection.");
			m_ParticipantsUI->getParticipant(lpData->uid)->setStatus(USER_STAUTS_RECONNECTING);
			onShowReconnectingUI(false, true, 40);
		}
		else
		{
			logInfo(to_string(lpData->uid) + " drop offline, waiting for its reconnection.");
			wstring left = m_ParticipantsUI->updateUIForDropOffline(lpData->uid);
			if (left != L"")
			{
				wstring message = left + L" is reconnecting";
				DisplayTip(message.c_str());
			}
			PDROPOFFLINE_PARAM param = new DROPOFFLINE_PARAM;
			param->videoDlg = this;
			param->user_id = lpData->uid;
			HANDLE hThread = CreateThread(NULL, 0, threadTiming, param, 0, NULL);
			m_thread_map.insert(map<UINT, HANDLE>::value_type(lpData->uid, hThread));
		}

	}
	else if (lpData->reason == USER_OFFLINE_QUIT)
	{
		logInfo("===================OnEIDUserOffline.USER_OFFLINE_QUIT=================");
		handleLeftOne(false, lpData->uid);
	}
	POSITION pos = m_listWndInfo.GetHeadPosition();
	while (pos != NULL){
		if (m_listWndInfo.GetAt(pos).nUID == lpData->uid) {
			m_listWndInfo.RemoveAt(pos);
			RebindVideoWnd();
			break;
		}

		m_listWndInfo.GetNext(pos);
	}

	delete lpData;
    lpData = nullptr;
	return 0;
}

LRESULT CVideoDlg::OnEIDConnectionLost(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CVideoDlg::OnLocalVideoStats(WPARAM wParam, LPARAM lParam)
{
	LPAGE_LOCAL_VIDEO_STAT lpData = (LPAGE_LOCAL_VIDEO_STAT)wParam;
	if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		if (lpData->sentFrameRate != 0 || lpData->sentBitrate != 0)
		{
			logError("=============OnLocalVideoStats:Client is sending local video stream. sentFramerate=" + to_string(lpData->sentFrameRate) + ",sentBitrate=" + to_string(lpData->sentBitrate) + "=============");
		}
	}
	delete lpData;
	lpData = NULL;
	return 0;
}
LRESULT CVideoDlg::OnRemoteVideoStats(WPARAM wParam, LPARAM lParam)
{
	
	LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)wParam;
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		if (lpData->receivedFrameRate != 0 || lpData->receivedBitrate != 0)
		{
			logError("Receive video stream(Framerate = " + to_string(lpData->receivedFrameRate) + ", Bitrate = " + to_string(lpData->receivedBitrate) + ") from client " + to_string(lpData->uid));
		}
	}
	else
	{
		if (lpData->uid < 1000)
		{
			logError("Receive video stream(Framerate = " + to_string(lpData->receivedFrameRate) + ", Bitrate = " + to_string(lpData->receivedBitrate) + ") from client " + to_string(lpData->uid));
		}
	}
	//logInfo("=============OnRemoteVideoStats:Framerate=" + to_string(lpData->receivedFrameRate) + ",Bitrate=" + to_string(lpData->receivedBitrate) + "=============");
	POSITION posNext = m_listWndInfo.GetHeadPosition();

	while (posNext != NULL) {
		AGVIDEO_WNDINFO &rWndInfo = m_listWndInfo.GetNext(posNext);

		if (rWndInfo.nUID == lpData->uid) {
			rWndInfo.nFramerate = lpData->receivedFrameRate;
			rWndInfo.nBitrate = lpData->receivedBitrate;
			rWndInfo.nWidth = lpData->width;
			rWndInfo.nHeight = lpData->height;
			m_wndVideo[rWndInfo.nIndex].SetFrameRateInfo(rWndInfo.nFramerate);
			m_wndVideo[rWndInfo.nIndex].SetBitrateInfo(rWndInfo.nBitrate);
			m_wndVideo[rWndInfo.nIndex].SetVideoResolution(rWndInfo.nWidth, rWndInfo.nHeight);
			break;
		}
	}

	delete lpData;
    lpData = NULL;
	return 0;
}

LRESULT CVideoDlg::OnStartRecordingService(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT CVideoDlg::OnStopRecordingService(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

/*
LRESULT CVideoDlg::OnStreamMessage(WPARAM wParam, LPARAM lParam)
{
    LPAGE_STREAM_MESSAGE lpData = (LPAGE_STREAM_MESSAGE)wParam;
    TCHAR szMessage[256];

    int nUTF8Len = lpData->length;
    
    memset(szMessage, 0, 256 * sizeof(TCHAR));
#ifdef UNICODE
    ::MultiByteToWideChar(CP_UTF8, 0, lpData->data, lpData->length, szMessage, 256);
#else
    _tcscpy_s(szMessage, 256, lpData->data);
#endif

    m_dlgChat.AddChatMessage(lpData->uid, szMessage);

    delete[] lpData->data;
    delete lpData;

    return 0;
}
*/
void CVideoDlg::DrawHead(CDC *pDC)
{
	CRect rcTitle;
	CString strTitle;
	
	if (m_bFullScreen)
		return;

	GetWindowText(strTitle);
	GetClientRect(&rcTitle);
	CBrush *lpDefaultBrush = pDC->SelectObject(&m_brHead);
	CFont* defFont = pDC->SelectObject(&m_ftTitle);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int bar_height = getResolutionHeight(MAX_BAR_HEIGHT);
		rcTitle.bottom = bar_height;
	}
	else
	{
		rcTitle.bottom = 32;	
	}

	pDC->FillRect(&rcTitle, &m_brHead);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->DrawText(strTitle, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->SelectObject(lpDefaultBrush);
	pDC->SelectObject(defFont);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int bar_height = getResolutionHeight(MAX_BAR_HEIGHT);
		int network_image_size = getResolutionSize(MAX_NETWORKIMAGE_SIZE);
		int y = (bar_height - network_image_size) / 2;
		m_imgNetQuality.Draw(pDC, 1, CPoint(0, y), ILD_NORMAL);
	}
	else
	{
		m_imgNetQuality.Draw(pDC, m_networkQuality, CPoint(0, 1), ILD_NORMAL);
	}
}

void CVideoDlg::InitCtrls()
{
	CRect	rcClient;
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		CRect rcWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
		MoveWindow(&rcWorkArea);
	}
	
	GetClientRect(&rcClient);
	
	m_btnRemoteControl.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNREMOTE_VIDEO);
	m_btnAnnotation.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNANNO_VIDEO);
	m_btnEraser.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNERASER);
    //m_btnMode.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMODE_VIDEO);
	m_btnAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNAUDIO_VIDEO);
	m_btnEndCall.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNENDCALL_VIDEO);

	///m_btnScrCap.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCRCAP_VIDEO);
    //m_btnSetup.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSETUP_VIDEO);
	m_btnRecording.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNRECORDING_VIDEO);
	if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		m_btnShow.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCR_VIDEO);
	}

	
	for (int nIndex = 0; nIndex < 4; nIndex++){
		m_wndVideo[nIndex].Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + nIndex);
		m_wndVideo[nIndex].SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
		m_wndVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58));
	}

	m_wndLocal.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
	m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndLocal.SetFaceColor(RGB(0x58, 0x58, 0x58));
	m_wndLocal.SetUID(0);

	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int width = getResolutionWidth(FUN_BUTTON_WIDTH);
		int height = getResolutionHeight(FUN_BUTTON_HEIGHT);

		m_btnMin.MoveWindow(rcClient.Width() - width * 2, 0, width, height, TRUE);
		m_btnRst.MoveWindow(rcClient.Width(), 0, width, height, TRUE);
		m_btnClose.MoveWindow(rcClient.Width() - width, 0, width, height, TRUE);

		int size = getResolutionSize(MAX_VIDEO_BUTTON_SIZE);
		m_btnRemoteControl.MoveWindow(rcClient.Width() / 2 - 180, rcClient.Height() - 84, size, size, TRUE);
		m_btnAnnotation.MoveWindow(rcClient.Width() / 2 - 180, rcClient.Height() - 84, size, size, TRUE);
		m_btnEraser.MoveWindow(rcClient.Width() / 2 - 144, rcClient.Height() - 84, size, size, TRUE);
		//m_btnSetup.MoveWindow(rcClient.Width() / 2 - 24, rcClient.Height() - 84, size, size, TRUE);
		m_btnAudio.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, size, size, TRUE);
		m_btnRecording.MoveWindow(rcClient.Width() - 72, rcClient.Height() - 84, size, size, TRUE);
		m_btnEndCall.MoveWindow(rcClient.Width() - 120, rcClient.Height() - 84, size, size, TRUE);
	}
	else
	{
		m_btnMin.MoveWindow(rcClient.Width() - 153, 1, 50, 30, TRUE);
		m_btnRst.MoveWindow(rcClient.Width() - 102, 1, 50, 30, TRUE);
		m_btnClose.MoveWindow(rcClient.Width() - 51, 1, 0, 30, TRUE);

		m_btnRemoteControl.MoveWindow(rcClient.Width() / 2 - 180, rcClient.Height() - 84, 48, 48, TRUE);
		m_btnAnnotation.MoveWindow(rcClient.Width() / 2 - 180, rcClient.Height() - 84, 48, 48, TRUE);

		m_btnEraser.MoveWindow(rcClient.Width() / 2 - 144, rcClient.Height() - 84, 48, 48, TRUE);
		//m_btnSetup.MoveWindow(rcClient.Width() / 2 - 24, rcClient.Height() - 84, 48, 48, TRUE);
		m_btnAudio.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, 48, 48, TRUE);
		m_btnRecording.MoveWindow(rcClient.Width() - 72, rcClient.Height() - 84, 48, 48, TRUE);
		m_btnEndCall.MoveWindow(rcClient.Width() - 120, rcClient.Height() - 84, 48, 48, TRUE);
	}

	
	if (m_btnShow.GetSafeHwnd() != NULL)
	{
		m_btnShow.MoveWindow(rcClient.Width() - 72, rcClient.Height() - 84, 48, 48, TRUE);
	}
		

    
	m_wndVideo[0].MoveWindow(0, 32, rcClient.Width(), rcClient.Height() - 96, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0x00, 0xFF));
	m_btnMin.SetBackColor(RGB(15, 43, 75), RGB(255, 200, 100), RGB(15, 43, 75), RGB(15, 43, 75));

	m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
	m_btnRst.SetBackColor(RGB(15, 43, 75), RGB(255, 200, 100), RGB(15, 43, 75), RGB(15, 43, 75));
	
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0x00, 0xFF));
	m_btnClose.SetBackColor(RGB(15, 43, 75), RGB(255, 200, 100), RGB(15, 43, 75), RGB(15, 43, 75));
	//m_btnScrCap.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	//m_btnScrCap.EnableFrameEffect(FALSE);
	//m_btnScrCap.SetBackImage(IDB_BTNSCRCAP_VIDEO, RGB(0x26, 0x26, 0x26));

    m_btnEraser.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnEraser.EnableFrameEffect(FALSE);
	m_btnRemoteControl.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnRemoteControl.EnableFrameEffect(FALSE);
	m_btnAnnotation.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnAnnotation.EnableFrameEffect(FALSE);
	m_btnAudio.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnAudio.EnableFrameEffect(FALSE);
	m_btnRecording.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnRecording.EnableFrameEffect(FALSE);
	m_btnEndCall.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnEndCall.EnableFrameEffect(FALSE);
	//m_btnSetup.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	//m_btnSetup.EnableFrameEffect(FALSE);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		m_btnEraser.SetBackImage(IDB_BTNERASER_96, RGB(0x26, 0x26, 0x26));
		m_btnRemoteControl.SetBackImage(IDB_IN_REMOTE_CONTROL_96, RGB(0x26, 0x26, 0x26));
		m_btnAnnotation.SetBackImage(IDB_DRAW_96, RGB(0x26, 0x26, 0x26));
		m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO_96, RGB(0x26, 0x26, 0x26));
		m_btnRecording.SetBackImage(IDB_BTNRECORD_96, RGB(0x26, 0x26, 0x26));
		m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO_96, RGB(0x26, 0x26, 0x26));
		//m_btnSetup.SetBackImage(IDB_BTN_SETTING_96, RGB(0x26, 0x26, 0x26));
	}
	else
	{
		m_btnEraser.SetBackImage(IDB_BTNERASER_48, RGB(0x26, 0x26, 0x26));
		m_btnRemoteControl.SetBackImage(IDB_IN_REMOTE_CONTROL_48, RGB(0x26, 0x26, 0x26));
		m_btnAnnotation.SetBackImage(IDB_DRAW_48, RGB(0x26, 0x26, 0x26));
		m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO_48, RGB(0x26, 0x26, 0x26));
		m_btnRecording.SetBackImage(IDB_BTNRECORD_48, RGB(0x26, 0x26, 0x26));
		m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO_48, RGB(0x26, 0x26, 0x26));
		//m_btnSetup.SetBackImage(IDB_BTN_SETTING_48, RGB(0x26, 0x26, 0x26));
	}

	//m_btnMode.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	//m_btnMode.EnableFrameEffect(FALSE);
	//m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	
	if (m_btnShow.GetSafeHwnd() != NULL)
	{
		m_btnShow.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
		m_btnShow.EnableFrameEffect(FALSE);
		m_btnShow.SetBackImage(IDB_BTNFULLSCR_VIDEO, RGB(0x26, 0x26, 0x26));
	}

	CBitmap	bmpNetQuality;
	
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		BITMAP  bmImageInfo;
		CBitmap scaledImage;
		bmpNetQuality.LoadBitmap(IDB_NETWORK_QUALITY);
		int size = getResolutionSize(MAX_NETWORKIMAGE_SIZE);
		ScaleBitmap(&bmpNetQuality, scaledImage, size * 7, size);
		scaledImage.GetBitmap(&bmImageInfo);
		m_imgNetQuality.DeleteImageList();
		m_imgNetQuality.Create(bmImageInfo.bmWidth / 7, bmImageInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 6, 1);
		m_imgNetQuality.Add(&scaledImage, RGB(15, 43, 75));
		scaledImage.DeleteObject();
	}
	else
	{
		bmpNetQuality.LoadBitmap(IDB_NETWORK_QUALITY_30);
		m_imgNetQuality.DeleteImageList();
		m_imgNetQuality.Create(30, 30, ILC_COLOR24 | ILC_MASK, 6, 1);
		m_imgNetQuality.Add(&bmpNetQuality, RGB(15, 43, 75));
		bmpNetQuality.DeleteObject();
	}
	
	m_btnRemoteControl.ShowWindow(SW_HIDE);
	m_btnEraser.ShowWindow(SW_HIDE);
	m_btnRecording.ShowWindow(SW_HIDE);
}

void CVideoDlg::ScaleBitmap(CBitmap *pBitmap, CBitmap &BitmapNew, int nWidth, int nHeight)
{
	CDC  dcSrc, dcDst;
	int      nWidthOld, nHeightOld;
	int      nWidthNew, nHeightNew;
	BITMAP    pBitMap;

	pBitmap->GetBitmap(&pBitMap);
	nWidthOld = pBitMap.bmWidth;
	nHeightOld = pBitMap.bmHeight;
	nWidthNew = nWidth;
	nHeightNew = nHeight;

	// Create DC

	dcSrc.CreateCompatibleDC((CDC*)NULL);
	dcDst.CreateCompatibleDC((CDC*)NULL);

	// Source Bitmap
	dcSrc.SelectObject(pBitmap);
	// New Bitmap
	BitmapNew.CreateCompatibleBitmap(&dcSrc, nWidthNew, nHeightNew);

	// Scale Bitmap

	dcDst.SelectObject(&BitmapNew);
	//Maps pixels from the source rectangle into blocks of pixels 
	//in the destination rectangle. The average color over 
	//the destination block of pixels approximates the color of the source pixels.
	dcDst.SetStretchBltMode(HALFTONE);
	dcDst.StretchBlt(0, 0, nWidthNew, nHeightNew, &dcSrc, 0, 0, nWidthOld, nHeightOld, SRCCOPY);

	// Free Resource
	dcSrc.DeleteDC();
	dcDst.DeleteDC();
}
void CVideoDlg::ShowVideo1()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
	}

	if (m_listWndInfo.GetCount() == 0) {
		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
		m_wndLocal.SetParent(this);
		m_wndLocal.SetBigShowFlag(TRUE);
		m_wndLocal.ShowWindow(SW_SHOW);
	}
	else {
		m_wndVideo[0].MoveWindow(&m_rcVideoArea, FALSE);
		m_wndVideo[0].ShowWindow(SW_SHOW);
		m_wndVideo[0].SetBigShowFlag(TRUE);
		m_wndVideo[0].SetParent(this);

		//m_wndLocal.MoveWindow(&m_rcChildVideoArea, FALSE);
		//m_wndLocal.SetParent(&m_wndVideo[0]);
		//Not Show the local if there is remote stream, this must be a client application
		//m_wndLocal.ShowWindow(SW_HIDE);
		//m_wndLocal.SetParent(&m_wndVideo[0]);
	}


	m_nScreenMode = SCREEN_VIDEO1;

	ShowButtonsNormal();



}

void CVideoDlg::ShowVideo4()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_SHOW);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
		m_wndVideo[nIndex].SetParent(this);
	}

	m_wndVideo[0].MoveWindow(0, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[1].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[2].MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[3].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);

	CRect	rcLocal;
	int		nRemoteCount = m_listWndInfo.GetCount();
	
	if (nRemoteCount == 2)
		m_wndLocal.MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	else
		m_wndLocal.MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);

	m_wndVideo[nRemoteCount].ShowWindow(SW_HIDE);
	m_wndLocal.SetParent(this);
	m_wndLocal.ShowWindow(SW_SHOW);

	m_nScreenMode = SCREEN_VIDEO4;

	ShowButtonsNormal();
	
}

void CVideoDlg::ShowMulti()
{
	int nLocalIndex = 0;

	m_wndLocal.ShowWindow(TRUE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
	}
	
	if (m_lpBigShowed == NULL)
		m_lpBigShowed = &m_wndVideo[0];

	m_lpBigShowed->ShowWindow(SW_SHOW);
	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);
	m_lpBigShowed->SetParent(this);
	m_lpBigShowed->SetBigShowFlag(TRUE);
	
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.top + 16;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, 192, 144, TRUE);
				m_wndVideo[nIndex].ShowWindow(SW_SHOW);
				m_wndVideo[nIndex].SetParent(m_lpBigShowed);
				nLocalIndex++;
			}
		}
		else{
			m_wndLocal.MoveWindow(nXPos, nYPos, 192, 144, TRUE);
			m_wndLocal.ShowWindow(SW_SHOW);
			m_wndLocal.SetParent(m_lpBigShowed);
			nLocalIndex++;
		}
	}

	m_nScreenMode = SCREEN_VIDEOMULTI;

	ShowButtonsNormal();
}

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	m_dlgDevice.Create(CDeviceDlg::IDD, this);
	m_dlgDevice.EnableDeviceTest(FALSE);

	m_dlgScreenCapture.Create(CAGScreenCaptureDlg::IDD, this);
	m_dlgDesktopCapture.Create(CAGDesktopCaptureDlg::IDD, this);
    //m_dlgChat.Create(CChatDlg::IDD, this);
	m_DrawLayerDlg.Create(CDrawLayerDlg::IDD, this);
	//m_DrawLayerDlg.SetParent(this);
	
	//m_ParticipantWnd.Subclass(this->GetSafeHwnd());
	//m_ParticipantWnd.Create(GetSafeHwnd(), _T("Participant"), UI_WNDSTYLE_DIALOG & (~(WS_CAPTION | WS_BORDER)),0, 0, 0, 600, 800);
	//m_ParticipantWnd.ShowWindow(FALSE);

	m_ParticipantsUI = new ParticipantsUI(GetSafeHwnd());
	m_ParticipantsUI->Create(GetSafeHwnd(), _T("Participants"), UI_WNDSTYLE_DIALOG & (~(WS_BORDER | WS_CAPTION)), 0, 0, 0, 600, 800);
	m_ParticipantsUI->ShowWindow(false);
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		int fontsize = getResolutionFontSize(MAX_TITLE_SIZE);
		m_ftTitle.CreateFont(fontsize, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	}
	else
	{
		m_ftTitle.CreateFont(21, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	}
	InitCtrls();
    
    m_bitMenuInfo.LoadBitmap(IDB_MENU_INFO);
    m_bitMenuDevice.LoadBitmap(IDB_MENU_DEVICE);
    m_bitMenuFilter.LoadBitmap(IDB_MENU_FILTER);




	
	//m_pRTMCallBack->SetImagePath(theApp.recvfilePath);
	//USES_CONVERSION;
	//std::string appId = W2A(m_agConfig.GetAppid());
	
	//std::string appCertificateID = gConfigSignal.getAppCertificatedId();
	//m_pRTMInstance->setAppCertificateId(appCertificateID);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


HWND CVideoDlg::GetRemoteVideoWnd(int nIndex)
{
	if (nIndex < 0 || nIndex > 3)
		return NULL;

	return m_wndVideo[nIndex].GetSafeHwnd();
}


void CVideoDlg::RebindVideoWnd()
{
	if (m_wndVideo[0].GetSafeHwnd() == NULL || m_wndLocal.GetSafeHwnd() == NULL)
		return;

	VideoCanvas canvas;

	canvas.renderMode = RENDER_MODE_FIT;

	POSITION pos = m_listWndInfo.GetHeadPosition();
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		if (pos != NULL) {
			AGVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
			canvas.uid = agvWndInfo.nUID;
			canvas.view = m_wndVideo[nIndex].GetSafeHwnd();
			agvWndInfo.nIndex = nIndex;

			CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
			m_wndVideo[nIndex].SetUID(canvas.uid);
			m_wndVideo[nIndex].SetVideoResolution(agvWndInfo.nWidth, agvWndInfo.nHeight);
			m_wndVideo[nIndex].SetFrameRateInfo(agvWndInfo.nFramerate);
			m_wndVideo[nIndex].SetBitrateInfo(agvWndInfo.nBitrate);
			m_wndVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58)); 
		}
		else
			m_wndVideo[nIndex].SetUID(0);
	}

	if (m_nScreenMode != SCREEN_VIDEOMULTI) {
		if (m_listWndInfo.GetCount() <= 1)
			ShowVideo1();
		else if (m_listWndInfo.GetCount() > 1 && m_listWndInfo.GetCount() < 4)
			ShowVideo4();
		else
			ShowMulti();
	}
	else
		ShowMulti();
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			if (m_bFullScreen)
				OnBnClickedBtnfullscr();
			
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CVideoDlg::OnShowModeChanged(WPARAM wParam, LPARAM lParam)
{
	if (m_nScreenMode == SCREEN_VIDEOMULTI) {
		if (m_listWndInfo.GetCount() <= 1)
			ShowVideo1();
		else if (m_listWndInfo.GetCount() > 1 && m_listWndInfo.GetCount() < 4)
			ShowVideo4();
	}
	else {
		m_lpBigShowed = (CAGVideoWnd *)wParam;
		ShowMulti();
	}
	
	return 0;
}

LRESULT CVideoDlg::OnShowBig(WPARAM wParam, LPARAM lParam)
{
	CRect rcNextBig;
	CRect rcCurBig;

	if (m_nScreenMode != SCREEN_VIDEOMULTI)
		return 0;

	CAGVideoWnd *lpNextBigWnd = (CAGVideoWnd *)wParam;
	if (lpNextBigWnd == m_lpBigShowed)
		return 0;

	ShowMulti();
	m_lpBigShowed = lpNextBigWnd;

	return 0;
}

/*
LRESULT CVideoDlg::OnWindowShareStart(WPARAM wParam, LPARAM lParam)
{
    HWND hShareWnd = (HWND)wParam;
    if (hShareWnd == GetSafeHwnd()) {
        CAgoraObject::GetAgoraObject()->EnableLocalRender(FALSE);
        m_wndLocal.Invalidate(TRUE);
    }

	CAgoraObject::GetAgoraObject()->EnableScreenCapture((HWND)wParam, 15, NULL, TRUE);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}


LRESULT CVideoDlg::OnDesktopShareStart(WPARAM wParam, LPARAM lParam)
{
	LPDESKTOP_SHARE_PARAM	lpDesktopShareParam = (LPDESKTOP_SHARE_PARAM)(wParam);
	CRect rcRegion;

	CAgoraObject::GetAgoraObject()->EnableVideo();

	rcRegion.left = lpDesktopShareParam->nX;
	rcRegion.top = lpDesktopShareParam->nY;
	rcRegion.right = rcRegion.left + lpDesktopShareParam->nWidth;
	rcRegion.bottom = rcRegion.top + lpDesktopShareParam->nHeight;

	CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, lpDesktopShareParam->nFPS, &rcRegion, TRUE);
	//	Sleep(1000);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}
*/
void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码

	if (bShow && GetSafeHwnd() != NULL)
		RebindVideoWnd();
}


void CVideoDlg::OnMove(int x, int y)
{
    CDialogEx::OnMove(x, y);

    // TODO:  在此处添加消息处理程序代码
    CRect rcChatBox;
	/*
    rcChatBox.SetRect(x, y + m_rcVideoArea.Height() - 126, x + m_rcVideoArea.Width(), y + m_rcVideoArea.Height() + 24);

	
    if (::IsWindow(m_dlgChat.GetSafeHwnd()) && m_dlgChat.IsWindowVisible()){
        m_dlgChat.MoveWindow(&rcChatBox);
    }
	*/
	if (::IsWindow(m_DrawLayerDlg.GetSafeHwnd()) && m_DrawLayerDlg.IsWindowVisible()) {
		
		rcChatBox.SetRect(x, y+32, x + m_rcVideoArea.Width(), y + m_rcVideoArea.Height());
		

		m_DrawLayerDlg.MoveWindow(&rcChatBox);
	}
	//if (::IsWindow(m_ParticipantWnd.GetHWND())&&::IsWindowVisible(m_ParticipantWnd.GetHWND()))
	//{
	//	RECT participateRect;
	//	GetClientRect(&participateRect);

	//	ClientToScreen(&participateRect);
	//	participateRect.top += 24;
	//	participateRect.bottom -= 72;
	//	participateRect.left = participateRect.right -  PARTICIPANT_WINDOW_WIDTH;
	//	::MoveWindow(m_ParticipantWnd.GetHWND(), participateRect.left, participateRect.top, participateRect.right - participateRect.left, participateRect.bottom - participateRect.top, TRUE);
	//}
	if (::IsWindowVisible(GetSafeHwnd()))
	{
		CRect	rcclient;
		GetClientRect(&rcclient);
	}
	if (m_ParticipantsUI && ::IsWindow(m_ParticipantsUI->GetHWND()) && ::IsWindowVisible(m_ParticipantsUI->GetHWND()))
	{
		RECT participateRect;
		GetClientRect(&participateRect);

		ClientToScreen(&participateRect);
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			int top = getResolutionHeight(MAX_BAR_HEIGHT);
			int bottom = getResolutionHeight(MAX_BOTTOM_HEIGHT);
			participateRect.top += top;
			participateRect.bottom -= bottom;
			int w = participateRect.right - participateRect.left;
			participateRect.left = participateRect.right - w / 5;
		}
		else
		{
			participateRect.top += 32;
			participateRect.bottom -= 72;
			int w = participateRect.right - participateRect.left;
			participateRect.left = participateRect.right - w / 5;// PARTICIPANT_WINDOW_WIDTH;
		}

		::MoveWindow(m_ParticipantsUI->GetHWND(), participateRect.left, participateRect.top, participateRect.right - participateRect.left, participateRect.bottom - participateRect.top, TRUE);
	}
}

LRESULT CVideoDlg::OnEIDAudioQuality(WPARAM wParam, LPARAM lParam)
{
    LPAGE_AUDIO_QUALITY lpData = new AGE_AUDIO_QUALITY;

    delete lpData;
    lpData = nullptr;
    return 0;
}

LRESULT CVideoDlg::OnEIDAudioVolumeIndication(WPARAM wParam, LPARAM lParam)
{
    ::PostMessage(m_dlgDevice.GetSafeHwnd(), WM_MSGID(EID_AUDIO_VOLUME_INDICATION), wParam, 0);

    return 0;
}

LRESULT CVideoDlg::OnInvitationEvents(WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_InviteReceived)
	{
		if (m_AgoraMedaiSourceDlg_hwnd != NULL)
		{
			::PostMessage(m_AgoraMedaiSourceDlg_hwnd, WM_InviteReceived, wParam, lParam);
		}
	}
	return 0;
 }

void CVideoDlg::onStartAnnotation()
{
	if (::IsWindow(m_DrawLayerDlg.GetSafeHwnd())) {
		CRect   rcDrawArea;
		rcDrawArea.CopyRect(&m_rcVideoArea);
		//rcChatArea.top = rcChatArea. - 150;
		ClientToScreen(&rcDrawArea);
		m_DrawLayerDlg.MoveWindow(&rcDrawArea);
	}
	m_DrawLayerDlg.ShowWindow(SW_SHOW);
}
void CVideoDlg::onStopAnnotation()
{
	m_DrawLayerDlg.CleanScreen();
	m_DrawLayerDlg.ShowWindow(SW_HIDE);
}

LRESULT CVideoDlg::OnRTMUserEvent(WPARAM wParam, LPARAM lParam)
{
	RTMMessagEvent *event = (RTMMessagEvent*)wParam;
	if (event->msgtype == UPDATE_STATUS || event->msgtype == QUERY_STATUS_CMD)
	{
		if (m_AgoraMedaiSourceDlg_hwnd != NULL)
		{
			logInfo("RTM message event on UPDATE_STATUS or QUERY_STATUS_CMD, go to AgoraMedaiSourceDlg to handler this event.");
			::SendMessage(m_AgoraMedaiSourceDlg_hwnd, WM_RTM_USER_MSG_EVENT, wParam, lParam);
		}
	}
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		if (mMeetingStatus == OTHER_ANNOTATION)
		{
			
			if (event->IsMouseEvent())
			{
				
				m_DrawLayerDlg.processRemoteAnnotationEvent(*event);
			
				
			}

		}
		else if (mMeetingStatus == OTHER_REMOTE_CONTROL || mMeetingStatus == MEETING_NORMAL)
		{
			if (event->IsMouseEvent() || event->IsKeyEvent())
			{
				BOOL isRelative = CString2BOOL(readRegKey(RELATIVE_MOUSE_POS, APP_REG_DIR));
				if (mMeetingStatus == OTHER_REMOTE_CONTROL && event->IsMouseEvent() && isRelative)
				{
					int temp_x = event->mouse_x;
					int temp_y = event->mouse_y;					
					logInfo("Before:x=" + to_string(event->mouse_x) + ",y=" + to_string(event->mouse_y) + ",pre_x=" + to_string(pre_x) + ",pre_y=" + to_string(pre_y));
					event->mouse_x = temp_x - pre_x;
					event->mouse_y = temp_y - pre_y;					
					pre_x = temp_x;
					pre_y = temp_y;
					logInfo("After:x=" + to_string(event->mouse_x) + ",y=" + to_string(event->mouse_y) + ",pre_x=" + to_string(pre_x) + ",pre_y=" + to_string(pre_y));
					HIDControl::GetInstance()->SendRelativeEventToHID(*event);					
				}
				else
				{
					HIDControl::GetInstance()->SendEventToHID(*event);
				}				
			}

		}

	    if (event->msgtype == ANNOTATE_REQUEST)
		{
			DisplayTip(_T("Annotation Request Accepted"));
			if (mMeetingStatus == MEETING_NORMAL)
				CommandManager::GetInstance()->GrantAnnotation(event->userId);
			else
				CommandManager::GetInstance()->RefuseAnnotationRequest(event->userId);
			
		}


		else if (event->msgtype == ANNOTATE_START)
		{
			if (mMeetingStatus == MEETING_NORMAL)
			{ 
				DisplayTip(_T("Annotation Starting"));
				mMeetingStatus = OTHER_ANNOTATION;
				m_other_anno_id = event->userId;
				m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
				m_btnAnnotation.EnableWindow(false);
				onStartAnnotation();
				m_ParticipantsUI->updateUIForAnnotation(true);
			}
			
		}
		else if (event->msgtype == ANNOTATE_END)
		{
			//OnBnClickedBtnmessage();
			if (mMeetingStatus == OTHER_ANNOTATION)
			{
				DisplayTip(_T("Annotation Ended"));
				mMeetingStatus = MEETING_NORMAL;
				m_other_anno_id = -1;
				m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
				m_btnAnnotation.EnableWindow(true);
				onStopAnnotation();
				m_ParticipantsUI->updateUIForAnnotation(false);
			}
		}
		else if (event->msgtype == ANNOTATE_CLEAR)
		{
			m_DrawLayerDlg.CleanScreen();
		}
		else if (event->msgtype == REMOTE_CONTROL_START)
		{
			DisplayTip(_T("Remote control Starting"));
			mMeetingStatus = OTHER_REMOTE_CONTROL;
			m_other_remote_id = event->userId;
			m_btnRemoteControl.ShowWindow(SW_SHOW);
			m_btnRemoteControl.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
			m_btnAnnotation.EnableWindow(false);
			m_ParticipantsUI->updateUIForRemoteControl(event->userId, true);
			if (CString2BOOL(readRegKey(RELATIVE_MOUSE_POS, APP_REG_DIR)))
			{
				pre_x = 0;
				pre_y = 0;
				HIDControl::GetInstance()->SendResetMouseEventToHID();
			}			
		}
		else if (event->msgtype == REMOTE_CONTROL_END)
		{
			DisplayTip(_T("Remote control Ended"));
			mMeetingStatus = MEETING_NORMAL;
			m_other_remote_id = -1;
			m_btnRemoteControl.ShowWindow(SW_HIDE);
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
			m_btnAnnotation.EnableWindow(true);
			m_ParticipantsUI->updateUIForRemoteControl(event->userId, false);
			pre_x = 0;
			pre_y = 0;
		}
		else if (event->msgtype == UPDATE_STATUS)
		{
			if (m_AgoraMedaiSourceDlg_hwnd != NULL)
			{
				::PostMessage(m_AgoraMedaiSourceDlg_hwnd, WM_RTM_USER_MSG_EVENT, wParam, lParam);
			}
		}
		else if (event->msgtype == VIDEO_HD1_MODE || event->msgtype == VIDEO_HD2_MODE || event->msgtype == VIDEO_PIP_MODE ||
			event->msgtype == VIDEO_PBYP_MODE || event->msgtype == VIDEO_PUP_MODE || event->msgtype == VIDEO_PIP_SWAP ||
			event->msgtype == VIDEO_PIP_PIC || event->msgtype == VIDEO_PIP_ROTATE || event->msgtype == VIDEO_WINMODE1 ||
			event->msgtype == VIDEO_WINMODE2 || event->msgtype == VIDEO_WINMODE4 || event->msgtype == VIDEO_WINMODE6 ||
			event->msgtype == VIDEO_WINMODE7 || event->msgtype == VIDEO_WINMODE8 || event->msgtype == VIDEO_HDMI1 || 
			event->msgtype == VIDEO_HDMI2 || event->msgtype == VIDEO_HDMI3 || event->msgtype == VIDEO_HDMI4)
		{
			logInfo("Host receive video screen control cmd from client " + to_string(event->userId) + ", send the cmd to serial port.");
			m_ParticipantsUI->selectVideoMode(event->msgtype, true, false);
		}
	}
	else
	{
		if (mMeetingStatus == OTHER_ANNOTATION )
		{

			if (event->IsMouseEvent())
			{
				
					m_DrawLayerDlg.processRemoteAnnotationEvent(*event);
				

			}

		}
		if (mMeetingStatus == OTHER_REMOTE_CONTROL)
		{
			
		}
		
		if (event->msgtype == ANNOTATE_GRANT)
		{
			DisplayTip(_T("Annotation Request Granted"));
			CommandManager::GetInstance()->AnnotationStart();
			mMeetingStatus = SELF_ANNOTATION;
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
			m_btnAnnotation.EnableWindow(true);
			m_btnEraser.ShowWindow(SW_SHOW);
			onStartAnnotation();
			
			
		}
		if (event->msgtype == ANNOTATE_START)
		{
			mMeetingStatus = OTHER_ANNOTATION;
			onStartAnnotation();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
			m_btnAnnotation.EnableWindow(false);
			m_other_anno_id = event->userId;
			DisplayTip(_T("Annotation Starting"));
			
		}
		if (event->msgtype == ANNOTATE_END)
		{
			DisplayTip(_T("Annotation Ended"));
			mMeetingStatus = MEETING_NORMAL;
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
			m_btnAnnotation.EnableWindow(true);
			m_other_anno_id = -1;
			onStopAnnotation();
		}
		if (event->msgtype == QUERY_STATUS_CMD)
		{
			if (m_AgoraMedaiSourceDlg_hwnd != NULL)
			{
				::PostMessage(m_AgoraMedaiSourceDlg_hwnd, WM_RTM_USER_MSG_EVENT, wParam, lParam);
			}
		}
		else if (event->msgtype == ANNOTATE_CLEAR)
		{
			m_DrawLayerDlg.CleanScreen();
		}
		else if (event->msgtype == REMOTE_CONTROL_GRANT)
		{
			mMeetingStatus = SELF_REMOTE_CONTROL;
			m_btnRemoteControl.ShowWindow(SW_SHOW);
			m_btnRemoteControl.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
			CommandManager::GetInstance()->RemoteControlStart();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
			m_btnAnnotation.EnableWindow(false);
			m_ParticipantsUI->updateUIForRemoteControl(CAgoraObject::GetAgoraObject()->GetSelfUID(), true);
		}
		else if (event->msgtype == REMOTE_CONTROL_TAKEBACK)
		{
			DisplayTip(_T("Remote control Ended"));
			mMeetingStatus = MEETING_NORMAL;
			m_btnRemoteControl.ShowWindow(SW_HIDE);
			CommandManager::GetInstance()->RemoteControlEnd();
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
			m_btnAnnotation.EnableWindow(true);
			m_ParticipantsUI->updateUIForRemoteControl(CAgoraObject::GetAgoraObject()->GetSelfUID(), false);
		}
		else if (event->msgtype == REMOTE_CONTROL_START)
		{
			DisplayTip(_T("Remote control Starting"));
			mMeetingStatus = OTHER_REMOTE_CONTROL;
			m_other_remote_id = event->userId;
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
			m_btnAnnotation.EnableWindow(false);
			m_ParticipantsUI->updateUIForRemoteControl(event->userId, true);
		}
		else if (event->msgtype == REMOTE_CONTROL_END)
		{
			DisplayTip(_T("Remote control Ended"));
			mMeetingStatus = MEETING_NORMAL;
			m_other_remote_id = -1;
			m_btnRemoteControl.ShowWindow(SW_HIDE);
			m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
			m_btnAnnotation.EnableWindow(true);
			m_ParticipantsUI->updateUIForRemoteControl(event->userId, false);
		}
		else if (event->msgtype == VIDEO_HD1_MODE || event->msgtype == VIDEO_HD2_MODE || event->msgtype == VIDEO_PIP_MODE ||
			event->msgtype == VIDEO_PBYP_MODE || event->msgtype == VIDEO_PUP_MODE || event->msgtype == VIDEO_PIP_SWAP ||
			event->msgtype == VIDEO_PIP_PIC || event->msgtype == VIDEO_PIP_ROTATE || event->msgtype == VIDEO_WINMODE1 ||
			event->msgtype == VIDEO_WINMODE2 || event->msgtype == VIDEO_WINMODE4 || event->msgtype == VIDEO_WINMODE6 ||
			event->msgtype == VIDEO_WINMODE7 || event->msgtype == VIDEO_WINMODE8 || event->msgtype == VIDEO_HDMI1 ||
			event->msgtype == VIDEO_HDMI2 || event->msgtype == VIDEO_HDMI3 || event->msgtype == VIDEO_HDMI4)
		{
			logInfo("Client receive video screen control cmd from client " + to_string(event->userId) + ", only update button UI for essential.");
			m_ParticipantsUI->selectVideoMode(event->msgtype, false, false);
		}
		else if (event->msgtype == BROADCAST_MEETING_ID)
		{
			this->m_client_meeting_id = event->meeting_id;
		}
	}


	return 0;
 }

 void CVideoDlg::setAgoraMediaSourceDlgHWND(HWND hwnd)
 {
	 this->m_AgoraMedaiSourceDlg_hwnd = hwnd;
 }

 LRESULT CVideoDlg::OnNetworkQuality(WPARAM wParam, LPARAM lParam)
 {
	 LPAGE_NETWORK_QUALITY lpData = (LPAGE_NETWORK_QUALITY)wParam;
	 uid_t uid = lpData->uid;
	 int rx = lpData->rxQuality;
	 int tx = lpData->txQuality;
	 bool changed = false;
	 
	 if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	 {
		 if (m_networkQuality != tx)
		 {
			 m_networkQuality = tx;
			 int bar_height = getResolutionHeight(MAX_BAR_HEIGHT);
			 int network_image_size = getResolutionSize(MAX_NETWORKIMAGE_SIZE);
			 int y = (bar_height - network_image_size) / 2;
			 InvalidateRect(CRect(0, y, network_image_size, network_image_size), TRUE);
			 changed = true;
		 }
	 }
	 else
	 {
		 if (m_networkQuality != rx)
		 {
			 m_networkQuality = rx;
			 InvalidateRect(CRect(0, 1, 30, 30), TRUE);
			 changed = true;
		 }
	 }
	 if (m_networkQuality == QUALITY_UNKNOWN)
	 {
		 logWarn("m_networkQuality == QUALITY_UNKNOWN");
	 }
	 else if (m_networkQuality == QUALITY_VBAD)
	 {
		 logWarn("m_networkQuality == QUALITY_VBAD");
	 }
	 else if (m_networkQuality == QUALITY_DOWN)
	 {
		 logWarn("m_networkQuality == QUALITY_DOWN");
	 }
	 changed = false;
	 delete lpData;
	 return 0;
 }

 LRESULT CVideoDlg::OnCleanScreenBtnClicked(WPARAM wParam, LPARAM lParam)
 {
	 CommandManager::GetInstance()->AnnotationClear();
	 return 0;
 }

 LRESULT CVideoDlg::OnJoinRTMChannelSuccess(WPARAM wParam, LPARAM lParam)
 {
	 CommandManager::GetInstance()->getRTMChannelMembers();
	 return 0;
 }

 LRESULT CVideoDlg::OnJoinChannelAudioMuted(WPARAM wParam, LPARAM lParam)
 {
	 PJoinChannelAudioMuted data = (PJoinChannelAudioMuted)wParam;
	 CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
	 if (data->audiomuted)
	 {
		 lpAgora->MuteLocalAudio(TRUE);
		 m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	 }
	 else
	 {
		 lpAgora->MuteLocalAudio(FALSE);
		 m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	 }
	 return 0;
 }

 LRESULT CVideoDlg::OnClientGiveBackRemoteControl(WPARAM wParam, LPARAM lParam)
 {
	 CommandManager::GetInstance()->RemoteControlEnd();
	 mMeetingStatus = MEETING_NORMAL;
	 m_btnRemoteControl.ShowWindow(SW_HIDE);
	 m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	 m_btnAnnotation.EnableWindow(true);
	 m_ParticipantsUI->updateUIForRemoteControl(CAgoraObject::GetAgoraObject()->GetSelfUID(), false);
	 m_ParticipantsUI->setRemoteControlling(false);
	 return 0;
 }

 LRESULT CVideoDlg::OnEscapeKey(WPARAM wParam, LPARAM lParam)
 {
	 if (m_bFullScreen)
	 {
		 OnBnClickedBtnfullscr();
	 }
	 return 0;
 }

 LRESULT CVideoDlg::OnShowParticipant(WPARAM wParam, LPARAM lParam)
 {
	 UINT uid = wParam;
	 Individual* participant = m_ParticipantsUI->getParticipant(uid);
	 if (participant != NULL && CAgoraObject::GetAgoraObject()->GetSelfUID() != uid && uid < 1000)
	 {
		 wstring message = participant->getWName() + L" joined the meeting.";
		 logInfo(StringUtil::WStringToString(message));
		 DisplayTip(message.c_str());
	 }
	 return 0;
 }

 UserStatus CVideoDlg::getParticipantStatus(UINT uid)
 {
	 return m_ParticipantsUI->getParticipant(uid)->getStatus();
 }

 void CVideoDlg::handleLeftOne(bool dropped, UINT uid)
 {
	 if (uid >= 1000)
	 {
		 // host id > 1000
		 if (dropped)
		 {
			 logInfo("Host dropped from the Meeting");
			 DisplayTip(_T("Host dropped from the Meeting"));
		 }
		 else
		 {
			 logInfo("Host left the Meeting");
			 DisplayTip(_T("Host left the Meeting"));
		 }		 
		 OnBnClickedBtnclose();		 
	 }
	 else
	 {
		 wstring left_name = m_ParticipantsUI->onUserLeave(uid);
		 if (left_name != L"")
		 {
			 PParticipant_Left pdata = new _participant_left;
			 pdata->uid = uid;
			 if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			 {
				 ::SendMessage(m_AgoraMedaiSourceDlg_hwnd, WM_PARTICIPANT_LEAVE, (WPARAM)pdata, 0);
			 }
			 if (uid == m_other_anno_id)
			 {
				 mMeetingStatus = MEETING_NORMAL;
				 m_other_anno_id = -1;
				 m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
				 m_btnAnnotation.EnableWindow(true);
				 wstring wmessage = L"User " + left_name;
				 if (dropped)
				 {
					 wmessage = wmessage + L" dropped from ";
				 }
				 else
				 {
					 wmessage = wmessage + L" left ";
				 }
				 wmessage = wmessage + L"the Meeting, and clean annotation.";
				 DisplayTip(wmessage.c_str());
				 logInfo(StringUtil::WStringToString(wmessage));
				 onStopAnnotation();
				 if (CAgoraObject::GetAgoraObject()->GetSelfHost())
				 {
					 m_ParticipantsUI->updateUIForRemoteControl(0, false);
				 }
			 }
			 else if (uid == m_other_remote_id)
			 {
				 mMeetingStatus = MEETING_NORMAL;
				 m_other_remote_id = -1;
				 m_btnRemoteControl.ShowWindow(SW_HIDE);
				 m_btnAnnotation.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
				 m_btnAnnotation.EnableWindow(true);
				 wstring wmessage = L"User " + left_name;
				 if (dropped)
				 {
					 wmessage = wmessage + L" dropped from ";
				 }
				 else
				 {
					 wmessage = wmessage + L" left ";
				 }
				 wmessage = wmessage + L"the Meeting, and stop remote control.";
				 DisplayTip(wmessage.c_str());
				 logInfo(StringUtil::WStringToString(wmessage));
				 m_ParticipantsUI->setRemoteControlling(false);
				 if (CAgoraObject::GetAgoraObject()->GetSelfHost())
				 {
					 m_ParticipantsUI->updateUIForRemoteControl(0, false);
				 }
			 }
			 else
			 {
				 wstring wmessage = L"User " + left_name;
				 if (dropped)
				 {
					 wmessage = wmessage + L" dropped from ";
				 }
				 else
				 {
					 wmessage = wmessage + L" left ";
				 }
				 wmessage = wmessage + L"the Meeting";
				 DisplayTip(wmessage.c_str());
				 logInfo(StringUtil::WStringToString(wmessage));
			 }
			 vector<Individual*> participants = ClientsManager::getInstance()->getAllParticipants();
			 if (participants.size() == 1 && atoi(participants[0]->getUserId().c_str()) >= 1000)
			 {
				 logInfo("All clients left, so end current meeting.");
				 OnBnClickedBtnclose();
				 DisplayTip(_T("All clients left, so end current meeting."));
			 }
		 }
		 else
		 {
			 logWarn("The left one is not in the participant list.");
		 }

	 }
 }

 void CVideoDlg::onCloseReconnectingUI()
 {
	 if (m_reconnectingUI)
	 {
		 if (::IsWindowVisible(m_reconnectingUI->GetHWND()))
		 {
			 m_reconnectingUI->Close();
		 }
		 m_reconnectingUI = NULL;
	 }
	 if (!IsZoomed())
	 {
		 OnBnClickedBtnrest();
	 }
	 ShowWindow(SW_SHOW);
 }
 void CVideoDlg::onShowReconnectingUI(bool self_reconnect, bool modal, int timer)
 {
	 if (m_reconnectingUI)
	 {
		 if (::IsWindowVisible(m_reconnectingUI->GetHWND()))
		 {
			 m_reconnectingUI->Close();
		 }
		 m_reconnectingUI = NULL;
	 }
	 m_reconnectingUI = new ReconnectingUI(this->GetSafeHwnd(), self_reconnect, timer);
	 int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	 int ScreenY = GetSystemMetrics(SM_CYSCREEN);
	 int x = (ScreenX - 430) / 2;
	 int y = (ScreenY - 180) / 2;
	 m_reconnectingUI->Create(this->GetSafeHwnd(), _T(""), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, x, y, 430, 180);
	 m_reconnectingUI->CenterWindow();	 
	 if (modal)
	 {
		 if (IsIconic())
		 {
			 ShowWindow(SW_RESTORE);
		 }
		 OnBnClickedBtnrest();
		 m_reconnectingUI->ShowModal();		 
	 }
	 else 
	 {
		 m_reconnectingUI->ShowWindow();
	 }
	  
 }
 void CVideoDlg::removeThread(UINT user_id)
 {
	 map<UINT, HANDLE>::iterator iter = m_thread_map.find(user_id);
	 if (iter != m_thread_map.end())
	 {
		 m_thread_map.erase(iter);
	 }
 }

 LRESULT CVideoDlg::OnConnectionStatusChanged(WPARAM wParam, LPARAM lParam)
 {
	 LPConnectionStatusChanged lpData = (LPConnectionStatusChanged)wParam;
	 string log = "";
	 switch (lpData->state)
	 {
	 case CONNECTION_STATE_TYPE::CONNECTION_STATE_CONNECTED:
		 log = log + "Connection State is CONNECTION_STATE_CONNECTED";
		 break;
	 case CONNECTION_STATE_TYPE::CONNECTION_STATE_CONNECTING:
		 log = log + "Connection State is CONNECTION_STATE_CONNECTING";
		 break;
	 case CONNECTION_STATE_TYPE::CONNECTION_STATE_DISCONNECTED:
		 log = log + "Connection State is CONNECTION_STATE_DISCONNECTED";
		 break;
	 case CONNECTION_STATE_TYPE::CONNECTION_STATE_FAILED:
		 log = log + "Connection State is CONNECTION_STATE_FAILED";
		 break;
	 case CONNECTION_STATE_TYPE::CONNECTION_STATE_RECONNECTING:
		 log = log + "Connection State is CONNECTION_STATE_RECONNECTING";
		 break;
	 default:
		 break;
	 }
	 switch (lpData->reason)
	 {
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_BANNED_BY_SERVER:
		 log = log + ", changed reason is CONNECTION_CHANGED_BANNED_BY_SERVER";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_CLIENT_IP_ADDRESS_CHANGED:
		 log = log + ", changed reason is CONNECTION_CHANGED_CLIENT_IP_ADDRESS_CHANGED";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_CONNECTING:
		 log = log + ", changed reason is CONNECTION_CHANGED_CONNECTING";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_INTERRUPTED:
		 log = log + ", changed reason is CONNECTION_CHANGED_INTERRUPTED";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_INVALID_APP_ID:
		 log = log + ", changed reason is CONNECTION_CHANGED_INVALID_APP_ID";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_INVALID_CHANNEL_NAME:
		 log = log + ", changed reason is CONNECTION_CHANGED_INVALID_CHANNEL_NAME";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_INVALID_TOKEN:
		 log = log + ", changed reason is CONNECTION_CHANGED_INVALID_TOKEN";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_JOIN_FAILED:
		 log = log + ", changed reason is CONNECTION_CHANGED_JOIN_FAILED";
		 break;
	 case CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_JOIN_SUCCESS:
		 log = log + ", changed reason is CONNECTION_CHANGED_JOIN_SUCCESS";
		 break;
	 default:
		 break;
	 }
	 logInfo(log);
	 if (lpData->state == CONNECTION_STATE_TYPE::CONNECTION_STATE_RECONNECTING ||
		 lpData->reason == CONNECTION_CHANGED_REASON_TYPE::CONNECTION_CHANGED_INTERRUPTED)
	 {
		// wstring init_message = L"You are offline now, please recover your connection or meeting ends in";
		 if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
		 {			 
			 onShowReconnectingUI(true, false, 60);
		 }
		 else
		 {
			 onShowReconnectingUI(true, true, 60);
		 }		 		 		 
	 }
	 return 0;
 }

 LRESULT CVideoDlg::OnReconnectionUIClose(WPARAM wParam, LPARAM lParam)
 {
	 bool m_self_reconnect = wParam;
	 if (!m_self_reconnect && !CAgoraObject::GetAgoraObject()->GetSelfHost())
	 {
		 CString ip = readRegKey(WEBSERVERIP, APP_REG_DIR);
		 CString port = readRegKey(WEBSERVERPORT, APP_REG_DIR);
		 string ip_str = CT2A(ip.GetBuffer());
		 if (port.GetLength() > 0)
		 {
			 CString c_ip_str = ip + ":" + port;
			 ip_str = CT2A(c_ip_str);
		 }
		 string url = "http://" + ip_str + "/api-meeting/ReportEndMeeting/MeetingID/" + to_string(this->m_client_meeting_id);
		 string response = CurlHttpClient::SendGetReq(url.c_str());
		 handleHostLeftMeetingResponse(response);
	 }
	 OnBnClickedBtnclose();
	 return 0;
 }

 void CVideoDlg::handleHostLeftMeetingResponse(string rsp)
 {
	 rapidjson::Document doc;
	 if (!doc.Parse(rsp.data()).HasParseError())
	 {
		 logInfo("handleHostLeftMeetingResponse from webserver:" + rsp);
	 }
 }

 LRESULT CVideoDlg::OnClientDroped(WPARAM wParam, LPARAM lParam)
 {
	 UINT user_id = (UINT)wParam;
	 handleLeftOne(true, user_id);
	 return 0;
 }

 DWORD WINAPI CVideoDlg::threadTiming(LPVOID lpParamter)
{
	 if (!lpParamter)
		 return 0;

	 PDROPOFFLINE_PARAM param = (PDROPOFFLINE_PARAM)lpParamter;
	 UINT user_id = param->user_id;
	 CVideoDlg* videoDlg = param->videoDlg; 
	 for (int i = 40; i > 0; i--)
	 {
		 Sleep(1000);
	 }
	 if (videoDlg != NULL)
	 {
		 videoDlg->removeThread(user_id);
		 if (videoDlg->getParticipantStatus(user_id) == USER_STAUTS_RECONNECTING)
		 {
			 //videoDlg->handleLeftOne(true, user_id);	
			 ::SendMessage(videoDlg->GetSafeHwnd(), WM_CLIENT_DROPED, (WPARAM)user_id, 0);
		 }		 
	 } 
	return 0;
}
