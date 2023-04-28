
// AgoraVideoCallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "AgoraMediaSourceDlg.h"
#include "CTipDlg.h"
#include "BackendComm.h"
#include "ConfigCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CAgoraVideoCallDlg dialog


CAgoraMediaSourceDlg::CAgoraMediaSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAgoraMediaSourceDlg::IDD, pParent)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nVideoSolution = 0;

	m_lpAgoraObject = NULL;
	m_lpRtcEngine = NULL;

	m_nLastmileQuality = 0;
	m_duimgr = NULL; 
	m_client = NULL;
	//m_websocket = new websocket_endpoint();
	m_client_user_struct = new USER_INFO_STRUCT;
	m_client_meeting_struct = new MEETING_INFO_STRUCT;
	appID = L"";
	socketID = -1;
}

void CAgoraMediaSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNMIN, m_btnMin);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnClose);

	DDX_Control(pDX, IDC_LINKAGORA, m_linkAgora);
}

BEGIN_MESSAGE_MAP(CAgoraMediaSourceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOMMAND()

	
	ON_MESSAGE(WM_JOINCHANNEL, &CAgoraMediaSourceDlg::OnJoinChannel)
	ON_MESSAGE(WM_LEAVECHANNEL, &CAgoraMediaSourceDlg::OnLeaveChannel)
	ON_MESSAGE(WM_MSGID(EID_AUDIO_VOLUME_INDICATION), &CAgoraMediaSourceDlg::OnEIDAudioVolumeIndication)
	//ON_MESSAGE(WM_CLIENT_LOGINSUCCESS, &CAgoraMediaSourceDlg::onClientShowJoinMeetingUI)
	ON_MESSAGE(WM_CLIENT_QUITAPP, &CAgoraMediaSourceDlg::onClientQuitApp)
	ON_MESSAGE(WM_CLIENT_JOINMEETING, &CAgoraMediaSourceDlg::onClientShowInMeetingUI)
	//ON_MESSAGE(WM_CLIENT_INCOMINGCALL, &CAgoraMediaSourceDlg::onClientShowIncomingCallUI)
	ON_MESSAGE(WM_CLIENT_INIT, &CAgoraMediaSourceDlg::onClientShowLoginUI)
	ON_MESSAGE(WM_CLIENT_LOGOUT, &CAgoraMediaSourceDlg::onClientShowLoginUI)
	ON_MESSAGE(WM_CLIENT_REJECTCALL, &CAgoraMediaSourceDlg::onClientRejectIncomingCall)
	ON_MESSAGE(WM_CLIENT_ACCEPTCALL, &CAgoraMediaSourceDlg::onClientAcceptIncomingCall)

	ON_BN_CLICKED(IDC_BTNMIN, &CAgoraMediaSourceDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE, &CAgoraMediaSourceDlg::OnBnClickedBtnclose)

	ON_MESSAGE(WM_MSGID(EID_LASTMILE_QUALITY), &CAgoraMediaSourceDlg::OnLastmileQuality)
	ON_MESSAGE(WM_MSGID(EID_CONNECTION_LOST), &CAgoraMediaSourceDlg::OnConnectionLost)
	ON_MESSAGE(WM_CONNECTION_STATUS_CHANGED, &CAgoraMediaSourceDlg::OnConnectionStatusChanged)
	ON_MESSAGE(WM_RTM_USER_MSG_EVENT, &CAgoraMediaSourceDlg::OnRTMUserEvent)
	ON_MESSAGE(WM_InviteAcceptedByPeer, &CAgoraMediaSourceDlg::OnInvitationEvents)
	ON_MESSAGE(WM_InviteRefusedByPeer, &CAgoraMediaSourceDlg::OnInvitationEvents)
	ON_MESSAGE(WM_InviteReceived, &CAgoraMediaSourceDlg::onClientShowIncomingCallUI)
	ON_MESSAGE(WM_InviteEndByPeer, &CAgoraMediaSourceDlg::OnInvitationEvents)

	ON_MESSAGE(WM_HOSTLOGTORTMSERVICE, &CAgoraMediaSourceDlg::OnHostLogToRTMService)
	ON_MESSAGE(WM_CLIENTLOGTORTMSERVICE, &CAgoraMediaSourceDlg::OnClientLogToRTMService)
	ON_MESSAGE(WM_LoginFailed, &CAgoraMediaSourceDlg::OnLoginFailed)
	ON_MESSAGE(WM_LoginSuccess, &CAgoraMediaSourceDlg::OnLoginSuccess)
	ON_MESSAGE(WM_CLINETONLINESTATUSCHANGE, &CAgoraMediaSourceDlg::OnClientOnlineStatusChange)
	ON_MESSAGE(WM_JOINCHANNEL_SUCCESS, &CAgoraMediaSourceDlg::OnJoinChannelSucces)
	ON_MESSAGE(WM_REJOINCHANNEL_SUCCESS, &CAgoraMediaSourceDlg::OnReJoinChannelSucces)
	ON_MESSAGE(WM_LEAVECHANNEL_SUCCESS, &CAgoraMediaSourceDlg::OnLeaveChannelSuccess)
	ON_MESSAGE(WM_CLIENT_JOIN_CHANNEL, &CAgoraMediaSourceDlg::OnClientJoinChannelSuccess)
	ON_MESSAGE(WM_PARTICIPANT_LEAVE, &CAgoraMediaSourceDlg::OnParticipantLeave)
	//ON_MESSAGE(WM_NETWORK_ERROR, &CAgoraMediaSourceDlg::OnClientNetworkError)
	ON_MESSAGE(WM_INVITATION_EXPIRE, &CAgoraMediaSourceDlg::OnInvitationExpire)
END_MESSAGE_MAP()


void CAgoraMediaSourceDlg::OnMove(int x, int y)
{
	if (m_client && m_client->m_client_login_ui_mgr)
	{
		HWND clientloginuihwn = m_client->m_client_login_ui_mgr->GetHWND();
		if (::IsWindow(clientloginuihwn) && ::IsWindowVisible(clientloginuihwn))
		{
			RECT rect;
			GetClientRect(&rect);
			ClientToScreen(&rect);
			rect.top = rect.top + 32;
			::MoveWindow(clientloginuihwn, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
		}
	}
	if (m_client && m_client->m_client_join_meeting_ui_mgr)
	{
		HWND joinmeetinguihwn = m_client->m_client_join_meeting_ui_mgr->GetHWND();
		if (::IsWindow(joinmeetinguihwn) && ::IsWindowVisible(joinmeetinguihwn))
		{
			RECT rect;
			GetClientRect(&rect);
			ClientToScreen(&rect);
			rect.top = rect.top + 32;
			::MoveWindow(joinmeetinguihwn, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
		}
	}
	if (m_duimgr && m_duimgr->initialization_ui)
	{
		HWND initialize_ui_hwn = m_duimgr->initialization_ui->GetHWND();
		if (::IsWindow(initialize_ui_hwn) && ::IsWindowVisible(initialize_ui_hwn))
		{
			RECT rect;
			GetClientRect(&rect);
			ClientToScreen(&rect);
			//rect.top = rect.top + 64;
			rect.top = rect.top + getResolutionHeight(MAX_BAR_HEIGHT);
			::MoveWindow(initialize_ui_hwn, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
		}
	}
}

void CAgoraMediaSourceDlg::OnShowWindow(BOOL, UINT)
{
	if (m_client && m_client->m_client_login_ui_mgr)
	{
		HWND clientloginuihwn = m_client->m_client_login_ui_mgr->GetHWND();
		if (::IsWindow(clientloginuihwn) && ::IsWindowVisible(clientloginuihwn))
		{
			RECT rect;
			GetClientRect(&rect);
			ClientToScreen(&rect);
			rect.top = rect.top + 32;
			::MoveWindow(clientloginuihwn, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
		}
	}
	if (m_duimgr && m_duimgr->initialization_ui)
	{
		HWND initialize_ui_hwn = m_duimgr->initialization_ui->GetHWND();
		if (::IsWindow(initialize_ui_hwn) && ::IsWindowVisible(initialize_ui_hwn))
		{
			RECT rect;
			GetClientRect(&rect);
			ClientToScreen(&rect);
			//rect.top = rect.top + 64;
			rect.top = rect.top + getResolutionHeight(MAX_BAR_HEIGHT);
			::MoveWindow(initialize_ui_hwn, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
		}
	}
	//if (m_client && m_client->m_client_join_meeting_ui_mgr)
	//{
	//	HWND joinmeetinguihwn = m_client->m_client_join_meeting_ui_mgr->GetHWND();
	//	if (::IsWindow(joinmeetinguihwn) && ::IsWindowVisible(joinmeetinguihwn))
	//	{
	//		RECT rect;
	//		GetClientRect(&rect);
	//		ClientToScreen(&rect);
	//		rect.top = rect.top + 32;
	//		::MoveWindow(joinmeetinguihwn, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
	//	}
	//}

}

LRESULT CAgoraMediaSourceDlg::OnRTMUserEvent(WPARAM wParam, LPARAM lParam)
{
	RTMMessagEvent *event = (RTMMessagEvent*)wParam;
	if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		if (event->msgtype == UPDATE_STATUS)
		{			
			UINT clientUID = event->userId;
			logInfo("Receive UPDATE_STATUS cmd from client " + to_string(clientUID));
			UserStatus clientStatus = event->client_status;
			string clientChannel = event->client_channel;
			this->m_duimgr->updateClientsStatus(to_string(clientUID), clientStatus,clientChannel);
		}
	}
	else
	{
		UINT hostUID = event->userId;
		if (event->msgtype == QUERY_STATUS_CMD)
		{
			logInfo("Receive QUERY_STATUS_CMD from host " + to_string(hostUID));
			string channel = CT2A(m_lpAgoraObject->GetChanelName().GetBuffer());
			CommandManager::GetInstance()->sendUpdateClientStatus(hostUID, m_client_user_struct->current_status, channel);
			m_hostset.insert(hostUID);
		}
		else if (event->msgtype == INVITATION_EXPIRE)
		{
			logInfo("Receive INVITATION_EXPIRE event from host " + to_string(hostUID));
			PlaySound(NULL, NULL, SND_FILENAME);
			m_client->onSwitchToJoinMeetingUI();
		}
		else if (event->msgtype == CANCEL_INVITATION_CMD)
		{
			logInfo("Receive CANCEL_INVITATION_CMD event from host " + to_string(hostUID));
			PlaySound(NULL, NULL, SND_FILENAME);
			m_client->onSwitchToJoinMeetingUI();
		}
	}
	return 0;
}
LRESULT CAgoraMediaSourceDlg::OnInvitationEvents(WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_InviteAcceptedByPeer)
	{ 
		OnJoinChannel(wParam, lParam);
		m_duimgr->onInvitationEnd();
		m_lpAgoraObject->onLocalInvitationAccept((ILocalCallInvitation *)lParam);
		wstring user_name = m_duimgr->getDisplayName(((ILocalCallInvitation *)lParam)->getCalleeId());
		wstring message = L"User " + user_name + L" accept the meeting invitation.";
		DisplayTip(message.c_str());
		logInfo(StringUtil::WStringToString(message));
	}
	else if (wParam == WM_InviteRefusedByPeer)
	{
		ILocalCallInvitation *localInvitation = (ILocalCallInvitation*)lParam;
		string clientId = localInvitation->getCalleeId();
		m_duimgr->onInvitationRefusedByClient(clientId);
		m_lpAgoraObject->onLocalInvitationRefused((ILocalCallInvitation *)lParam);
		wstring user_name = m_duimgr->getDisplayName(((ILocalCallInvitation *)lParam)->getCalleeId());
		string s = "User " + StringUtil::WStringToString(user_name) + " refuse the meeting invitation.";
		logInfo(s);

	}
	else if (wParam == WM_InviteEndByPeer)
	{
		//client get cancel Invitation command from host
		logInfo("Host cancel the meeting call.");
		PlaySound(NULL, NULL, SND_FILENAME);
		m_client->onSwitchToJoinMeetingUI();		
	}
	return 0;
}
void CAgoraMediaSourceDlg::PublishClientStatus()
{
	//set<UINT>::iterator it;
	//for (it = m_hostset.begin(); it != m_hostset.end(); it++)
	//{
	//	UINT hostUID = *it;
	//	CommandManager::GetInstance()->sendUpdateClientStatus(hostUID, m_client_user_struct->current_status);
	//}
	string log = "Client status is " + StringUtil::getClientStatusName(m_client_user_struct->current_status) + ", publish to host:";
	
	for (int i = 0; i < m_client_user_struct->hostsize; i++)
	{
		string channel = CT2A(m_lpAgoraObject->GetChanelName().GetBuffer());
		log = log + " " + to_string(m_client_user_struct->hostids[i]);
		CommandManager::GetInstance()->sendUpdateClientStatus(m_client_user_struct->hostids[i], m_client_user_struct->current_status, channel);
	}
	logInfo(log);
}

// CAgoraVideoCallDlg message handlers
BOOL CAgoraMediaSourceDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_ESCAPE:
		case VK_RETURN:
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CAgoraMediaSourceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
 //   m_ftLink.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
 //   m_ftDes.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
 //   m_ftPhone.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	
	string app_id;
	bool result = BackendCommImpl::Instance()->requestAppID(app_id);
	if (result == TRUE)
	{
		InitDialog( StringUtil::StringToWstring( app_id).c_str());
	}
	//ModifyStyleEx(WS_EX_CLIENTEDGE, NULL, SWP_DRAWFRAME);	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CAgoraMediaSourceDlg::OnHostLogToRTMService(WPARAM wParam, LPARAM lParam)
{
	PHOST_LOG_TO_RTM_SERVICE lpData = (PHOST_LOG_TO_RTM_SERVICE)wParam;
	m_duimgr->setChannelName(lpData->channel.c_str());
	m_duimgr->setUserId(lpData->uid);
	logInfo("Host login to RTM service, hostid = " + to_string(lpData->uid) + ", channnel = " + lpData->channel);
	m_lpAgoraObject->SetSelfUID(lpData->uid); //Rtmservice login
	m_lpAgoraObject->RTMLogin();
	return 0;
}
LRESULT CAgoraMediaSourceDlg::OnClientLogToRTMService(WPARAM wParam, LPARAM lParam)
{
	m_client_user_struct = (USER_INFO_STRUCT*)wParam;
	m_lpAgoraObject->SetSelfUID(str2int(m_client_user_struct->uid));
	wstring displayname = m_client_user_struct->display_name;
	logInfo("Client login to RTM service, client name is " + StringUtil::WStringToString(displayname) + ", id is " + m_client_user_struct->uid);
	m_lpAgoraObject->RTMLogin();
	return 0;
}


LRESULT CAgoraMediaSourceDlg::OnLoginFailed(WPARAM wParam, LPARAM lParam)
{
	int errorCode = (int)lParam;
	logInfo("Failed to login RTMService, error code is " + errorCode);
	if (m_lpAgoraObject->GetSelfHost())
	{		
		m_duimgr->onShowError(L"Failed to Login to RTMService.");
	}
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	//CPaintManagerUI::SetInstance(AfxGetInstanceHandle());                    // 指定duilib的实例
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());    // 指定duilib资源的路径，这里指定为和exe同目录
	//::CoInitialize(NULL);  // remember to release ::CoUninitialize();
	if (m_lpAgoraObject->GetSelfHost())
	{
		logInfo("Host successfully login to RTMService. Show LoggedIn UI...");
		m_duimgr->onShowLoggedInUI();
	}
	else
	{
		//m_lpAgoraObject->MuteLocalVideo();
		logInfo("Client successfully login to RTMService. Show JoinMeeting UI...");
		m_client_user_struct->current_status = USER_STATUS_LOGIN;
		PublishClientStatus();
		onClientShowJoinMeetingUI();
	}
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnClientOnlineStatusChange(WPARAM wParam, LPARAM lParam)
{
	PClientOnlineStatusChange data = (PClientOnlineStatusChange)wParam;
	UserStatus status;
	if (data->isOnline)
	{
		status = UserStatus::USER_STATUS_LOGIN;
	}
	else
	{
		status = UserStatus::USER_STATUS_LOGOUT;
	}
	m_duimgr->updateClientsStatus(data->user_id, status,"");
	return 0;
}

void CAgoraMediaSourceDlg::terminateHeartBeatThread()
{
	if (heartbeatThread != NULL)   //线程正在运行
	{
		TerminateThread(heartbeatThread, 0);
		if (heartbeatThread != NULL)
		{
			CloseHandle(heartbeatThread);   //关闭线程句柄，可能线程已结束，但句柄还没释放
			heartbeatThread = NULL;
		}
		logInfo("Terminate the heartbeat thread.");
	}
}

DWORD WINAPI CAgoraMediaSourceDlg::threadTiming(LPVOID lpParamter)
{
	if (!lpParamter)
		return 0;

	CAgoraMediaSourceDlg* instance = (CAgoraMediaSourceDlg*)lpParamter;

	while (instance->getCurrentSocketStatus() != "Open") {
		Sleep(500);
	}
	logInfo("Start to send heart beat.");
	while (instance->getCurrentSocketStatus() == "Open")
	{
		instance->sent_heart_beat();
		Sleep(60000);
	}
	logInfo("Socket is closed, exit the heart beat thread.");
	return 0;
}
string CAgoraMediaSourceDlg::getCurrentSocketStatus()
{
	return endpoint.get_current_status();
}
void CAgoraMediaSourceDlg::sent_heart_beat()
{
	endpoint.send(socketID, to_string(m_duimgr->getMeetingID()));
}
LRESULT CAgoraMediaSourceDlg::OnJoinChannelSucces(WPARAM wParam, LPARAM lParam)
{
	
	if (m_lpAgoraObject->GetSelfHost())
	{ 
		//m_duimgr->updateCOMPort();	
		if (CAGConfig::GetInstance()->GetPictureDivider() == 2 || CAGConfig::GetInstance()->GetPictureDivider() == 4)
		{
			logInfo("Init video screen com.");
			VideoScreenControl::GetInstance()->Init(cs2s(CAGConfig::GetInstance()->GetVideoScreenCom()));
		}
		string equipment = RegConfig::Instance()->getEquipmentTypeName();
		//CString equipment = readRegKey(EQUIPMENT_NAME, APP_REG_DIR);
		bool isRCEnable = RegConfig::Instance()->getRCEnable();
		//BOOL isRCEnable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
		if (CString(equipment.c_str()).MakeUpper() != _T("SPYGLASS") && isRCEnable)
		{
			logInfo("Init device com.");
			HIDControl::GetInstance()->Init(cs2s(CAGConfig::GetInstance()->GetDeviceCom()), CAGConfig::GetInstance()->GetBaudrate());
		}	
		m_duimgr->setMeetingStatus(true);
		string host_id = to_string(m_duimgr->getUserId());
		int meeting_id = BackendCommImpl::Instance()->reportStartMeeting(host_id);
		if (meeting_id != -1)
		{
			string websocket_uri = RegConfig::Instance()->getWebSocketURI();
			setMeetingID(meeting_id);
			endpoint.connect(websocket_uri);
			terminateHeartBeatThread();
			heartbeatThread = CreateThread(NULL, 0, threadTiming, (void*)this, 0, NULL);
		}	
	}
	else
	{
		m_client_user_struct->current_status = USER_STATUS_JOIN_MEETING;
		PublishClientStatus();
		//m_lpAgoraObject->MuteLocalVideo(true);
		PJoinChannelAudioMuted pdata = new JoinChannelAudioMuted;
		pdata->audiomuted = m_client_meeting_struct->audiomuted;
		::PostMessage(m_dlgVideo.GetSafeHwnd(),WM_JOINCHANNEL_AUDIO, (WPARAM)pdata, 0);
		string channel_id = m_client_meeting_struct->meeting_channel;
		int meeting_id = BackendCommImpl::Instance()->reportClientJoinMeeting1(m_client_user_struct->uid,channel_id);
	}
	
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnReJoinChannelSucces(WPARAM wParam, LPARAM lParam)
{
	if (m_lpAgoraObject->GetSelfHost())
	{
		logInfo("Reconnect socket connection for rejoining channel.");
		//socketID = m_websocket->connect(websocket_uri, to_string(m_duimgr->getMeetingID()));
		socketID = endpoint.connect(websocket_uri);
		terminateHeartBeatThread();
		heartbeatThread = CreateThread(NULL, 0, threadTiming, (void*)this, 0, NULL);
	}	
	return 0;
}
LRESULT CAgoraMediaSourceDlg::OnClientJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_JOINED data = (LPAGE_USER_JOINED)wParam;
	string client_id = to_string(data->uid);
	string meeting_id = to_string(m_duimgr->getMeetingID());
	string uuid = UUIDGenerator::getInstance()->getUUID();
	string host_id = to_string(m_duimgr->getUserId());
	string channel_id = m_duimgr->getChannelName();
	BackendCommImpl::Instance()->reportClientJoinMeeting2(client_id, meeting_id, host_id, channel_id);
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnParticipantLeave(WPARAM wParam, LPARAM lParam)
{

	PParticipant_Left pdata = (PParticipant_Left)wParam;
	string client_id = to_string(pdata->uid);
	string meeting_id = to_string(m_duimgr->getMeetingID());
	string channel_id = m_duimgr->getChannelName();
	BackendCommImpl::Instance()->reportClientExitMeeting(client_id, meeting_id, channel_id);
	return 0;
}


LRESULT CAgoraMediaSourceDlg::OnLeaveChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	logInfo("=====================OnLeaveChannelSuccess====================");
	m_lpAgoraObject->setInChannel(false);
	if (m_lpAgoraObject->GetSelfHost())
	{
		string meeting_id = to_string(m_duimgr->getMeetingID());
		BackendCommImpl::Instance()->reportEndMeeting(meeting_id);
	}
	else
	{
		string meeting_id = to_string(m_duimgr->getMeetingID());
		string client_id = m_client_user_struct->uid;
		string channel_id = m_client_meeting_struct->meeting_channel;
		
		BackendCommImpl::Instance()->reportClientExitMeeting(client_id, meeting_id,channel_id);
	}
	
	return 0;
}




void CAgoraMediaSourceDlg::InitDialog(CString strAppid)
{
	if (strAppid.IsEmpty())
	{
		MessageBox(_T("No APP_ID Info from web server, quit app!"), _T("information"), MB_OK | MB_ICONINFORMATION);
		::PostQuitMessage(0);
	}
	appID = strAppid;
	BOOL isHost = RegConfig::Instance()->getIsHost();
	m_lpAgoraObject = CAgoraObject::GetAgoraObject(strAppid);
	m_lpRtcEngine = CAgoraObject::GetEngine(); 

	m_lpAgoraObject->EnableVideo(TRUE);
	m_lpAgoraObject->SetLogFilePath(NULL);
	m_lpAgoraObject->EnableNetworkTest(TRUE);
	m_lpAgoraObject->SetSelfHost(isHost);
	m_lpAgoraObject->SetMsgHandlerWnd(GetSafeHwnd());
	if (!isHost)
	{
		m_lpAgoraObject->MuteLocalVideo(true);
		m_ftTitle.CreateFont(21, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	}
	else
	{
		//m_ftTitle.CreateFont(40, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
		int fontsize = getResolutionFontSize(MAX_TITLE_SIZE);
		m_ftTitle.CreateFont(fontsize, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	}
	//SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
	InitCtrls();
	InitChildDialog();

	if (isHost)
	{
		m_duimgr = new DUIMgr(*this);
		m_duimgr->onShowInitializationUI();
	}
	else
	{
		InitClientDialog();
	}
	// Com port

}

void CAgoraMediaSourceDlg::InitClientDialog()
{
	m_client = new Client(*this, m_lpAgoraObject);
	logInfo("Show login UI...");
	m_client->onClientShowLoginUI(SwitchToLoginUIType_INITIAL);
}
void CAgoraMediaSourceDlg::InitCtrls()
{
	CRect ClientRect;
	CBitmap	bmpNetQuality;

	if (m_lpAgoraObject->GetSelfHost())
	{
		//MoveWindow(0, 0, 818, 607, 1);
		CRect rcWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
		MoveWindow(&rcWorkArea);
	}
	else
	{
		MoveWindow(0, 0, 524, 376, 1);
	}

	GetClientRect(&ClientRect);

	bmpNetQuality.LoadBitmap(IDB_NETWORK_QUALITY);

	m_imgNetQuality.Create(30, 30, ILC_COLOR24 | ILC_MASK, 6, 1);
	m_imgNetQuality.Add(&bmpNetQuality, RGB(15, 43, 75));
	if (m_lpAgoraObject->GetSelfHost())
	{
		int width = getResolutionWidth(FUN_BUTTON_WIDTH);
		int height = getResolutionHeight(FUN_BUTTON_HEIGHT);
		m_btnMin.MoveWindow(ClientRect.Width() - width * 2, 0, width, height, TRUE);
		m_btnClose.MoveWindow(ClientRect.Width() - width, 0, width, height, TRUE);
		//m_btnMin.MoveWindow(ClientRect.Width() - 160, 0, 80, 64, TRUE);
		//m_btnClose.MoveWindow(ClientRect.Width() - 80, 0, 80, 64, TRUE);
	}
	else
	{
		m_btnMin.MoveWindow(ClientRect.Width() - 102, 1, 50, 30, TRUE);
		m_btnClose.MoveWindow(ClientRect.Width() - 51, 1, 50, 30, TRUE);
	}

	//m_linkAgora.MoveWindow(ClientRect.Width() / 2 - 110, ClientRect.Height()-55, 80, 20, TRUE);
	
	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0, 0xFF));
	m_btnMin.SetBackColor(RGB(15, 43, 75), RGB(255, 200, 100), RGB(15, 43, 75), RGB(15, 43, 75));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0, 160, 239));
	m_btnClose.SetBackColor(RGB(15, 43, 75), RGB(255, 200, 100), RGB(15, 43, 75), RGB(15, 43, 75));
	//m_linkAgora.SetFont(&m_ftLink);
	//m_linkAgora.SetURL(_T("http://www.agora.io"));
	//m_linkAgora.SetWindowText(LANG_STR("IDS_LOGO_AGORAWEB"));
	CMFCButton::EnableWindowsTheming(FALSE);
}

void CAgoraMediaSourceDlg::InitChildDialog()
{
	CString str;

	//m_dlgEnterChannel.Create(CEnterChannelDlg::IDD, this);
	m_dlgSetup.Create(CSetupDlg::IDD, this);
	m_dlgVideo.Create(CVideoDlg::IDD, this);
	m_dlgVideo.setAgoraMediaSourceDlgHWND(GetSafeHwnd());
	//m_dlgEnterChannel.MoveWindow(110, 70, 500, 450, TRUE);
	m_dlgSetup.MoveWindow(110, 70, 500, 450, TRUE);
	m_dlgSetup.ShowWindow(SW_HIDE);

	//m_dlgEnterChannel.ShowWindow(SW_SHOW);
	//m_lpCurDialog = &m_dlgEnterChannel;

	//m_dlgEnterChannel.SetVideoString(m_dlgSetup.GetVideoSolutionDes());

//	str.Format(_T("%d*%d,%dfps,%dKbps"), m_nVideoWidth, m_nVideoHeight, m_nFrameRate, m_nMaxRate);
//	m_dlgEnterChannel.SetVideoString(str);
}

void CAgoraMediaSourceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (m_lpAgoraObject->GetSelfHost())
	{
		if (nID == SC_MOVE || nID == 0xF012)
			return;
	}


	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAgoraMediaSourceDlg::OnPaint()
{
	if (appID.IsEmpty())
	{
		CDialogEx::OnCancel();
		logError("Failed to start MedLink for not getting APP_ID from webserver!");
		::MessageBox(NULL, L"Failed to start MedLink for not getting APP_ID from webserver!", L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawClient(&dc);
		//		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAgoraMediaSourceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CAgoraMediaSourceDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CAgoraMediaSourceDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;
	CFont* defFont = lpDC->SelectObject(&m_ftTitle);
	

	GetClientRect(&rcClient);

	if (m_lpAgoraObject->GetSelfHost())
	{
		int bar_height = getResolutionHeight(MAX_BAR_HEIGHT);
		lpDC->FillSolidRect(0, 0, rcClient.Width(), bar_height, RGB(15, 43, 75));
		lpDC->SetBkColor(RGB(15, 43, 75));
		lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		lpString = LANG_STR("IDS_TITLE");
		TEXTMETRIC tm;
		lpDC->GetTextMetrics(&tm);
		lpDC->TextOut(10, (bar_height - tm.tmHeight) / 2, lpString, _tcslen(lpString));
	}
	else
	{
		lpDC->FillSolidRect(0, 0, rcClient.Width(), 32, RGB(15, 43, 75));
		lpDC->SetBkColor(RGB(15, 43, 75));
		lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		lpString = LANG_STR("IDS_TITLE");
		lpDC->TextOut(5, 6, lpString, _tcslen(lpString));
	}
	
	
	//m_imgNetQuality.Draw(lpDC, m_nLastmileQuality, CPoint(0, 1), ILD_NORMAL);

	lpDC->SelectObject(&m_ftDes);
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = LANG_STR("IDS_CONTACT");
	lpDC->TextOut(rcClient.Width() / 2 - 50, rcClient.Height() - 55, lpString, _tcslen(lpString));

	lpDC->SelectObject(&m_ftPhone);
	lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
	lpString = LANG_STR("IDS_PHONENUMBER");
	lpDC->TextOut(rcClient.Width() / 2 + 45, rcClient.Height() - 55, lpString, _tcslen(lpString));

	lpDC->SelectObject(&m_ftDes);
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = AVC_VER;// _T("v1.3");

	rcText.SetRect(0, rcClient.Height() - 30, rcClient.Width(), rcClient.Height() - 5);
	lpDC->DrawText(lpString, _tcslen(lpString), &rcText, DT_CENTER | DT_SINGLELINE);
	lpDC->SelectObject(defFont);
}

void CAgoraMediaSourceDlg::OnBnClickedBtnmin()
{
	// TODO:  在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void CAgoraMediaSourceDlg::OnBnClickedBtnclose()
{
	// TODO:  在此添加控件通知处理程序代码
	ExitApp();
	CDialogEx::OnCancel();
}


LRESULT CAgoraMediaSourceDlg::OnJoinChannel(WPARAM wParam, LPARAM lParam)
{
	this->ShowWindow(SW_HIDE);
	IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	CString strChannelName;
	CString userId;
	if (!lpAgoraObject->GetSelfHost())
	{
		strChannelName = (m_client_meeting_struct->meeting_channel).c_str();
		userId = (m_client_user_struct->uid).c_str();
		m_dlgVideo.MoveWindow(0, 0, 960, 720, 1);
		m_dlgVideo.ShowWindow(SW_SHOW);
		m_dlgVideo.CenterWindow();
	}
	else
	{
		strChannelName = CString(m_duimgr->getChannelName());
		userId.Format(_T("%d"), m_duimgr->getUserId());
		CRect rcWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
		m_dlgVideo.MoveWindow(&rcWorkArea);
	}

	m_dlgVideo.MoveWindow(0, 0, 960, 720, 1);
	m_dlgVideo.ShowWindow(SW_SHOW);
	m_dlgVideo.CenterWindow();

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_dlgVideo.GetLocalVideoWnd();
	vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;
	
	if (lpAgoraObject->GetSelfHost())
	{
		vc.mirrorMode = VIDEO_MIRROR_MODE_DISABLED;

	}
	CameraCapturerConfiguration config;
	config.preference = CAPTURER_OUTPUT_PREFERENCE_MANUAL;
	config.captureWidth = 1280;
	config.captureHeight = 1024;
	lpRtcEngine->setCameraCapturerConfiguration(config);
	lpAgoraObject->EnableVideo(TRUE);
	//// 创建一个 EncryptionConfig 实例
	//EncryptionConfig encryptionConfig;
	//// 设置加密模式为国密 SM4 加密模式
	//encryptionConfig.encryptionMode = SM4_128_ECB;
	//// 设置加密密钥
	//encryptionConfig.encryptionKey = "6666666666666666";
	//// 启用内置加密
	//lpRtcEngine->enableEncryption(true, encryptionConfig);
	if (lpAgoraObject->GetSelfHost()) {
		m_nVideoSolution = m_dlgSetup.GetVideoSolution();
		double rate = CAGConfig::GetInstance()->GetRate();
		VideoEncoderConfiguration config;
		config.dimensions.width = 1280 * rate; 
		config.dimensions.height = 1024 * rate;
		config.frameRate = FRAME_RATE_FPS_30;
		config.minFrameRate = FRAME_RATE_FPS_24;
		config.minBitrate = 2500;
		config.bitrate = 5000;
		//config.degradationPreference = 
		lpRtcEngine->setVideoEncoderConfiguration(config);
		m_dlgVideo.SetLocalVideoWndResoluation(config.dimensions.width, config.dimensions.height);
		lpRtcEngine->setupLocalVideo(vc);
		
		lpRtcEngine->startPreview();	
	}
	//lpAgoraObject->EnableVideo(TRUE);
	m_dlgVideo.SetWindowText(strChannelName);
	//lpRtcEngine->setParameters("{\"che.video.capture_width\":1280,\"che.video.capture_height\":1024}");
	lpAgoraObject->JoinChannel(strChannelName, _ttoi(userId));
	lpAgoraObject->SetMsgHandlerWnd(m_dlgVideo.GetSafeHwnd());

	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnLeaveChannel(WPARAM wParam, LPARAM lParam)
{
	logInfo("==============OnLeaveChannel===============");
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	lpAgoraObject->LeaveCahnnel();
	//m_dlgEnterChannel.CleanEncryptionSecret();

	//m_dlgEnterChannel.ExtCaptureControl(FALSE);
	CAgoraObject::GetAgoraObject()->SetMsgHandlerWnd(GetSafeHwnd());

	m_dlgVideo.ShowWindow(SW_HIDE);
	//this->ShowWindow(SW_SHOW);
	ShowWindow(SW_SHOWNORMAL);
	
	m_lpAgoraObject->setInChannel(false);
	if (!m_lpAgoraObject->GetSelfHost())
	{
		m_client_user_struct->current_status = USER_STATUS_ENDMEETING;
		PublishClientStatus();
		m_client->onSwitchToJoinMeetingUI();
		m_client_user_struct->current_status = USER_STATUS_LOGIN;
		string client_id = m_client_user_struct->uid;
		string channel_id = m_client_meeting_struct->meeting_channel;
		BackendCommImpl::Instance()->reportActiveLogClientExitMeeting(client_id, channel_id);
		
	}
	else
	{
		HIDControl::GetInstance()->Close();
		VideoScreenControl::GetInstance()->Close();
		m_duimgr->onSwitchToLoggedInUI();
		m_duimgr->setMeetingStatus(false);
		vector<Individual*> participants = ClientsManager::getInstance()->getAllParticipants();
		string meeting_id = to_string(m_duimgr->getMeetingID());
		BackendCommImpl::Instance()->reportEndMeeting(meeting_id);
		string channel_id = m_duimgr->getChannelName();
		for (int i = 0; i < participants.size(); i++)
		{
			int id = atoi(participants[i]->getUserId().c_str());
			if (id < 1000)
			{
				string client_id = participants[i]->getUserId();
				BackendCommImpl::Instance()->reportClientExitMeeting(client_id, meeting_id, channel_id);
				
			}
		}
		logInfo("Meeting is end, close current socket connection.");
		//m_websocket->close(socketID, websocketpp::close::status::normal, "Meeting is End.");
		endpoint.close(socketID, websocketpp::close::status::normal, "Meeting is End.");
		socketID = -1;
		terminateHeartBeatThread();
	}
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnLastmileQuality(WPARAM wParam, LPARAM lParam)
{
	//LPAGE_LASTMILE_QUALITY lpData = (LPAGE_LASTMILE_QUALITY)wParam;
	int quality = wParam;
	if (m_nLastmileQuality != quality) {
		m_nLastmileQuality = quality;
		InvalidateRect(CRect(16, 40, 48, 72), TRUE);
	}
	m_lpAgoraObject->EnableNetworkTest(FALSE);
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnConnectionLost(WPARAM wParam, LPARAM lParam)
{
	logInfo("Lose connection with the server for more than 10 seconds, OnConnectionLost callback is received.");
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnConnectionStatusChanged(WPARAM wParam, LPARAM lParam)
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
	return 0;
}

LRESULT CAgoraMediaSourceDlg::OnEIDAudioVolumeIndication(WPARAM wParam, LPARAM lParam)
{
	//::PostMessage(m_dlgEnterChannel.GetDeviceHwnd(), WM_MSGID(EID_AUDIO_VOLUME_INDICATION), wParam, 0);

	return 0;
}
LRESULT CAgoraMediaSourceDlg::onClientShowLoginUI(WPARAM wParam, LPARAM lParam)
{
	m_client->onClientShowLoginUI(SwitchToLoginUIType_LOGOUT);
	m_client_user_struct->current_status = USER_STATUS_LOGOUT;
	PublishClientStatus();
	return 0;
}
LRESULT CAgoraMediaSourceDlg::onClientShowInMeetingUI(WPARAM wParam, LPARAM lParam)
{
	if (wParam != NULL)
	{
		MEETING_INFO_STRUCT* tmp_meeting_struct = (MEETING_INFO_STRUCT*)wParam;
		m_client_meeting_struct->meeting_channel = tmp_meeting_struct->meeting_channel;
		m_client_meeting_struct->audiomuted = tmp_meeting_struct->audiomuted;
	}
	::PostMessage(*this, WM_JOINCHANNEL, wParam, lParam);
	return 0;
}
void CAgoraMediaSourceDlg::onClientShowJoinMeetingUI()
{
	CRect ClientRect;
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(ScreenX-425, 10, 420, 770, 1);
	GetClientRect(&ClientRect);
	m_btnMin.MoveWindow(ClientRect.Width() - 102, 1, 50, 30, TRUE);
	m_btnClose.MoveWindow(ClientRect.Width() - 51, 1, 50, 30, TRUE);
	m_client->onClientShowJoinMeetingUI();

}
LRESULT CAgoraMediaSourceDlg::onClientAcceptIncomingCall(WPARAM wParam, LPARAM lParam)
{
	PlaySound(NULL, NULL, SND_FILENAME);
	IRemoteCallInvitation* invitation = (IRemoteCallInvitation*)lParam;
	string from = invitation->getCallerId();
	logInfo("Stop playing ring, accept invitation call from " + from);
	m_lpAgoraObject->AcceptInvitation(invitation);
	m_client->onClientAcceptIncomingCall(invitation->getChannelId());
	m_client_meeting_struct->meeting_from = invitation->getCallerId();
	m_client_meeting_struct->meeting_channel = invitation->getChannelId();
	return 0;
}
LRESULT CAgoraMediaSourceDlg::onClientShowIncomingCallUI(WPARAM wParam, LPARAM lParam)
{
	//check if the client have already in the channel
	IRemoteCallInvitation* invitation = (IRemoteCallInvitation*)lParam;
	int state = invitation->getState();
	logInfo("Receive RemoteCallInvitation from host, and its state is:" + to_string(state));
	CString remoteChannel = CString(invitation->getChannelId());
	if (m_lpAgoraObject->GetChanelName() == remoteChannel && remoteChannel != L"")
	{
		// accept the call directly
		logInfo("Client has already in the channel, accept the call directly, not showing incoming call UI.");
		m_lpAgoraObject->AcceptInvitation(invitation);
	}
	else
	{
		// show incoming call ui
		logInfo("Showing incoming call UI.");
		m_client->onClientShowIncomingCallUI(invitation);
	}
	return 0;
}
LRESULT CAgoraMediaSourceDlg::onClientRejectIncomingCall(WPARAM wParam, LPARAM lParam)
{
	PlaySound(NULL, NULL, SND_FILENAME);
	IRemoteCallInvitation* invitation = (IRemoteCallInvitation*)lParam;
	string from = invitation->getCallerId();
	logInfo("Stop playing ring, reject invitation call from " + from);
	m_lpAgoraObject->RejectInvitation(invitation);
	//m_client_user_struct->current_status = USER_STATUS_REJECT_MEETING;
	//PublishClientStatus();
	m_client_user_struct->current_status = USER_STATUS_LOGIN;
	return 0;
}
LRESULT CAgoraMediaSourceDlg::onClientQuitApp(WPARAM wParam, LPARAM lParam)
{
	m_client_user_struct->current_status = USER_STATUS_LOGOUT;
	PublishClientStatus();
	m_client->onClientQuitApp();
	::PostQuitMessage(0);
	return 0;
}

void CAgoraMediaSourceDlg::ExitApp()
{
	if (!m_lpAgoraObject->GetSelfHost())
	{
		m_client_user_struct->current_status = USER_STATUS_LOGOUT;
		PublishClientStatus();
		Sleep(100);
	}
}

void CAgoraMediaSourceDlg::setMeetingID(int id)
{
	CommandManager::GetInstance()->broadcastMeetingID(id);
	m_duimgr->setMeetingID(id);
}

//LRESULT CAgoraMediaSourceDlg::OnClientNetworkError(WPARAM wParam, LPARAM lParam)
//{
//	OnBnClickedBtnclose();
//	return 0;
//}

LRESULT CAgoraMediaSourceDlg::OnInvitationExpire(WPARAM wParam, LPARAM lParam)
{
	if (m_lpAgoraObject->GetSelfHost())
	{
		//m_duimgr->onInvitationEnd();		
		ILocalCallInvitation* invitation = (ILocalCallInvitation*)wParam;
		string user_id = invitation->getCalleeId();
		m_duimgr->onInvitationTimeoutByClient(user_id);
		logInfo("The invitation to client " + user_id + " expired.");
		CommandManager::GetInstance()->sendInvitatonExpireEvent(user_id);
	}
	else
	{
		IRemoteCallInvitation* invitation = (IRemoteCallInvitation*)wParam;
		string host_id = invitation->getCallerId();
		const char* from = invitation->getContent();
		wstring caller = StringUtil::utf8string2wstring(from);
		//wstring message = L"A missed call from \"" + caller + L".\"";
		logInfo("The invitation from host " + host_id + " expired, end this call.");
		PlaySound(NULL, NULL, SND_FILENAME);
		m_client->onSwitchToJoinMeetingUI();
		m_client->onClientShowMissedCallUI(caller);
	}
	return 0;
}

CAgoraMediaSourceDlg::~CAgoraMediaSourceDlg()
{
	//delete this->m_client_user_struct;
	//delete this->m_client_meeting_struct;
}