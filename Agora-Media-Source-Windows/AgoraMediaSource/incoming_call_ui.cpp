#include "stdafx.h"
#include "incoming_call_ui.h"

//DWORD WINAPI threadTiming(LPVOID lpParamter);

ClientIncomingCallUI::ClientIncomingCallUI()
{
	m_incoming_call_panel = NULL;
	m_callingLabelUI = NULL;
	m_timingLabelUI = NULL;
	m_btnReject = NULL;
	m_gitAccept = NULL;
	m_incomingCallUIMgr = NULL;
}

ClientIncomingCallUI::~ClientIncomingCallUI()
{
	m_incoming_call_panel = NULL;
	m_callingLabelUI = NULL;
	m_timingLabelUI = NULL;
	m_btnReject = NULL;
	m_gitAccept = NULL;

	m_incomingCallUIMgr = NULL;
	//if (m_hThread)
	//{
	//	TerminateThread(m_hThread, 0);
	//}
}

void ClientIncomingCallUI::InitWindow(CPaintManagerUI& ui_mgr, ClientIncomingCallUIMgr* main_frame_)
{
	m_callingLabelUI = static_cast<CLabelUI*>(ui_mgr.FindControl(_T("calling_label")));
	m_timingLabelUI = static_cast<CLabelUI*>(ui_mgr.FindControl(_T("timing_label")));
	m_btnReject = static_cast<CButtonUI*>(ui_mgr.FindControl(_T("btn_reject")));
	m_gitAccept = static_cast<CGifAnimUI*>(ui_mgr.FindControl(_T("gif_accept")));
	m_incomingCallUIMgr = main_frame_;
	answered = false;
	const char* content = m_incomingCallUIMgr->GetMeetingInvitation()->getContent();
	logInfo("Host invitation content is:" + string(content));
	vector<string> vs;
	split(content, vs, ',');
	string fromStr = content;
	if (vs.size() == 2)
	{
		fromStr = vs.at(0);
		CAgoraObject::GetAgoraObject()->SetHostEquipment(vs.at(1));
	}
	wstring wstr = StringUtil::utf8string2wstring(fromStr);
	wstring from = L"Calling from " + wstr + L"...";

#ifdef UNICODE
	LPCWSTR result = from.c_str();
#else
	LPCWSTR result = s.c_str();
#endif>
	m_callingLabelUI->SetText(result);

	//m_hThread = CreateThread(NULL, 0, threadTiming, (void*)this, 0, NULL);
}

void ClientIncomingCallUI::updateTimingLabel(int i)
{
	std::string str = "(" + to_string(i) + " s)";

#ifdef UNICODE
	std::wstring stemp = StringUtil::StringToWstring(str); // Temporary buffer is required
	LPCWSTR result = stemp.c_str();
#else
	LPCWSTR result = s.c_str();
#endif>
	m_timingLabelUI->SetText(result);
}

bool ClientIncomingCallUI::isAnswered()
{
	return answered;
}

void ClientIncomingCallUI::UninitWindow()
{}

//DWORD WINAPI threadTiming(LPVOID lpParamter)
//{
//	if (!lpParamter)
//		return 0;
//
//	ClientIncomingCallUI* m_IncomingCallUI = (ClientIncomingCallUI*)lpParamter;
//	for (int i = 55; i >= 0; i--)
//	{
//		m_IncomingCallUI->updateTimingLabel(i);
//		Sleep(1000);
//	}
//
//	if (m_IncomingCallUI != NULL && !m_IncomingCallUI->isAnswered())
//	{
//		UserStatus userStatus = USER_STATUS_TIMEOUT;
//		::PostMessage(m_IncomingCallUI->GetParentFrame()->GetHWND(), WM_CLOSE, 0, (LPARAM)userStatus);
//	}
//	return 0;
//}

ClientIncomingCallUIMgr* ClientIncomingCallUI::GetParentFrame()
{
	return m_incomingCallUIMgr;
}

void ClientIncomingCallUI::DoJoinMeetingBtnClick()
{
	m_incomingCallUIMgr->ShowWindow(false);	
	answered = true;
	::PostMessage(m_incomingCallUIMgr->GetParentHWND(), WM_CLIENT_ACCEPTCALL, 0, (LPARAM)(m_incomingCallUIMgr->GetMeetingInvitation()));
}

void ClientIncomingCallUI::DoRejectBtnClick()
{
	m_incomingCallUIMgr->ShowWindow(false);
	answered = true;
	::PostMessage(m_incomingCallUIMgr->GetParentHWND(), WM_CLIENT_REJECTCALL, 0, (LPARAM)(m_incomingCallUIMgr->GetMeetingInvitation()));
}

void ClientIncomingCallUI::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("click"))
	{
		if (msg.pSender == m_btnReject)
		{
			DoRejectBtnClick();
		}
	}
	else if(msg.sType == _T("setfocus"))
	{
		if (msg.pSender == m_gitAccept)
		{
			DoJoinMeetingBtnClick();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///CIncomingCallUIMgr
ClientIncomingCallUIMgr::ClientIncomingCallUIMgr(HWND hwnd, IRemoteCallInvitation* invitation)
{
	m_parentHWND = hwnd;
	m_invitation = invitation;
}

IRemoteCallInvitation* ClientIncomingCallUIMgr::GetMeetingInvitation()
{
	return m_invitation;
}

ClientIncomingCallUIMgr::~ClientIncomingCallUIMgr()
{	
}

void ClientIncomingCallUIMgr::InitWindow()
{
	m_incommingCallUI.InitWindow(m_PaintManager, this);
	RECT rc = { 0 };
	if( !::GetClientRect(m_hWnd, &rc) ) return;
	rc.right = rc.left + 380;
	rc.bottom = rc.top + 280;
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) return;
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);

	//::SetWindowPos(m_hWnd, NULL, (ScreenX - (rc.right - rc.left)) / 2,
	//	(ScreenY - (rc.bottom - rc.top)) / 2, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//´°¿ÚÖÃ¶¥
}

void ClientIncomingCallUIMgr::Notify( TNotifyUI& msg )
{
	m_incommingCallUI.Notify(msg);
}

LRESULT ClientIncomingCallUIMgr::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	if( uMsg == WM_CREATE ) 
	{
		m_PaintManager.Init(m_hWnd);

		CDialogBuilder builder;
		STRINGorID xml(GetSkinRes());
		CControlUI* pRoot = builder.Create(xml, _T("xml"), 0, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		InitWindow(); 

		return lRes;
	}
	else if (uMsg == WM_CLOSE)
	{
		OnClose(uMsg, wParam, lParam, bHandled);		
	}
	else if (uMsg == WM_DESTROY)
	{
		OnDestroy(uMsg, wParam, lParam, bHandled);		
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ClientIncomingCallUIMgr::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (lParam == NULL)
	{
		::PostMessage(m_parentHWND, WM_CLIENT_REJECTCALL, 0, (LPARAM)(m_invitation));
	}
	else
	{
		PlaySound(NULL, NULL, SND_FILENAME);
		UserStatus userStatus = (UserStatus)lParam;
		CommandManager::GetInstance()->sendUpdateClientStatus(atoi(m_invitation->getCallerId()), userStatus, "");
	}
	this->ShowWindow(false);
	return 0;
}

LRESULT ClientIncomingCallUIMgr::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_incommingCallUI.UninitWindow();
	return 0;
}

HWND ClientIncomingCallUIMgr::GetParentHWND()
{
	return m_parentHWND;
}