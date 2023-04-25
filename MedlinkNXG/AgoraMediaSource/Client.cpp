
#include "Client.h"

Client::Client(HWND parentHWND, CAgoraObject* agObject)
{
	m_parentHWND = parentHWND;
	m_agObject = agObject;
	m_client_login_ui_mgr = NULL;
	m_client_join_meeting_ui_mgr = NULL;
	m_client_incoming_call_ui_mgr = NULL;
	m_missed_call_ui = NULL;
}

Client::~Client()
{
	m_client_login_ui_mgr = NULL;
	m_client_join_meeting_ui_mgr = NULL;
	m_client_incoming_call_ui_mgr = NULL;
	m_missed_call_ui = NULL;
}

void Client::onSwitchToJoinMeetingUI()
{
	if (m_client_incoming_call_ui_mgr)
	{
		HWND incoming_hwnd = m_client_incoming_call_ui_mgr->GetHWND();
		if (::IsWindowVisible(incoming_hwnd))
		{
			m_client_incoming_call_ui_mgr->ShowWindow(false);
		}
	}
	if (m_client_join_meeting_ui_mgr)
	{
		m_client_join_meeting_ui_mgr->show();
	}
}
void Client::onClientShowMissedCallUI(wstring caller)
{
	//if (m_missed_call_ui)
	//{
	//	::PostMessage(m_missed_call_ui->GetHWND(), WM_CLOSE, 0, 0);
	//	m_missed_call_ui = NULL;
	//}
	//m_missed_call_ui = new MissedCallUI(m_parentHWND, caller);
	//CRect screen_rc;
	//SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&screen_rc, 0);
	//long left = screen_rc.right - 350;
	//long top = screen_rc.bottom - 200;
	//m_missed_call_ui->Create(m_parentHWND, _T("Missed Calls"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, left, top, 350, 200);
	//m_missed_call_ui->SetIcon(IDI_ICON_LOGO);
	//m_missed_call_ui->ShowWindow(true);

	if (m_missed_call_ui && ::IsWindowVisible(m_missed_call_ui->GetHWND()))
	{
		m_missed_call_ui->addMissedCaller(caller);
	}
	else
	{
		m_missed_call_ui = new MissedCallUI(m_parentHWND, caller);
		CRect screen_rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&screen_rc, 0);
		long left = screen_rc.right - 350;
		long top = screen_rc.bottom - 200;
		m_missed_call_ui->Create(m_parentHWND, _T("Missed Call"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, left, top, 350, 200);
		m_missed_call_ui->SetIcon(IDI_ICON_LOGO);
		m_missed_call_ui->ShowWindow(true);
	}

}
void Client::onClientShowJoinMeetingUI()
{
	if (m_client_login_ui_mgr)
	{
		//Close login screen
		m_client_login_ui_mgr->ShowWindow(false);
	}

	if (m_client_join_meeting_ui_mgr)
	{
		::PostMessage(m_client_join_meeting_ui_mgr->GetHWND(), WM_DESTROY, 0, 0);
		m_client_join_meeting_ui_mgr = NULL;
	}

	m_client_join_meeting_ui_mgr = new ClientJoinMeetingUIMgr(m_parentHWND);
	if (NULL == m_client_join_meeting_ui_mgr)
	{
		return;
	}
	m_client_join_meeting_ui_mgr->Create(m_parentHWND, _T("Med-Link Client"), UI_WNDSTYLE_DIALOG & (~(WS_BORDER | WS_CAPTION)), 0, 0, 0, 600, 800);
	//m_client_join_meeting_ui_mgr->Create(m_parentHWND, _T("Med-Link Client"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES, 0, 0, 0, 0);
	m_client_join_meeting_ui_mgr->SetIcon(IDI_ICON_LOGO);
	//m_client_join_meeting_ui_mgr->ShowModal();
	m_client_join_meeting_ui_mgr->ShowWindow(true);
}

void Client::onClientQuitApp()
{
	if (m_client_login_ui_mgr)
	{
		::PostMessage(m_client_login_ui_mgr->GetHWND(), WM_DESTROY, 0, 0);
	}
	if (m_client_join_meeting_ui_mgr)
	{
		::PostMessage(m_client_join_meeting_ui_mgr->GetHWND(), WM_DESTROY, 0, 0);
	}
	::PostQuitMessage(0);
}

void Client::onClientShowLoginUI(SwitchToLoginUIType loginType)
{
	switch (loginType)
	{
	case SwitchToLoginUIType_INITIAL:
	{
		CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
		CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
		::CoInitialize(NULL);

		m_client_login_ui_mgr = new ClientLoginUIMgr(m_parentHWND, m_agObject);
		m_client_login_ui_mgr->Create(m_parentHWND, _T("Med-Link Client"), UI_WNDSTYLE_DIALOG & (~(WS_BORDER | WS_CAPTION)),0,0,0,600,800);
		m_client_login_ui_mgr->SetIcon(IDI_ICON_LOGO);		
		//m_client_login_ui_mgr->Create(m_parentHWND, _T("Med-Link Client"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES, 0, 0, 0, 0);
		//m_client_login_ui_mgr->ShowModal();
		m_client_login_ui_mgr->ShowWindow(true);
		break;
	}
	case SwitchToLoginUIType_LOGOUT:
	{
		CRect rc(0, 0, 0, 0);
		HWND hwndUI(NULL);
		if (m_client_login_ui_mgr)
		{
			hwndUI = m_client_login_ui_mgr->GetHWND();
			if (NULL != hwndUI)
				::GetWindowRect(hwndUI, &rc);
			m_client_login_ui_mgr->ShowWindow(true);
		}

		if (m_client_join_meeting_ui_mgr)
		{
			m_client_join_meeting_ui_mgr->ShowWindow(false);
		}

		if (m_client_login_ui_mgr)
		{
			m_client_login_ui_mgr->LogOut();
		}
		break;
	}
	default:
		break;
	}
}

void Client::onClientShowIncomingCallUI(IRemoteCallInvitation* invitation)
{
	if (m_client_incoming_call_ui_mgr)
	{
		DestroyWindow(m_client_incoming_call_ui_mgr->GetHWND());
		m_client_incoming_call_ui_mgr = NULL;
	}

	m_client_incoming_call_ui_mgr = new ClientIncomingCallUIMgr(m_parentHWND, invitation);
	if (NULL == m_client_incoming_call_ui_mgr)
	{
		return;
	}
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);
	int x = (ScreenX - 380) / 2;
	int y = (ScreenY - 319) / 2;
	m_client_incoming_call_ui_mgr->Create(m_parentHWND, _T("MedLink"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE,x,y,0,0);
	m_client_incoming_call_ui_mgr->SetIcon(IDI_ICON_LOGO);
	m_client_incoming_call_ui_mgr->ShowWindow(true);

	string path = getAbsoluteDir();
	path = path + "rings\\Alarm01.wav";
	wstring MusicPath = StringUtil::StringToWstring(path);
	//CString MusicPath = _T("C:\\Windows\\media\\Alarm01.wav");
	logInfo("Play ring " + path);
	PlaySound(MusicPath.c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void Client::onClientAcceptIncomingCall(const char* meeting_id)
{
	if (m_client_join_meeting_ui_mgr)
	{
		m_client_join_meeting_ui_mgr->acceptIncomingCall(meeting_id);
	}
}

void Client::onHostCancelInvitation()
{
	if (m_client_incoming_call_ui_mgr)
	{
		m_client_incoming_call_ui_mgr->ShowWindow(false);
	}
}
