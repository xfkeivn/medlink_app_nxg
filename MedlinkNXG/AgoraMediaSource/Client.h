#pragma once

#include "stdafx.h"
#include "client_common.h"
#include "init_auth_ui.h"
#include "join_meeting_ui.h"
#include "incoming_call_ui.h"
#include "MissedCallUi.h"
#include <mmsystem.h>

class Client
{
public:
	Client(HWND parentHWND, CAgoraObject* agObject);
	~Client();
	void onClientShowJoinMeetingUI();
	void onSwitchToJoinMeetingUI();
	void onClientQuitApp();
	void onClientShowLoginUI(SwitchToLoginUIType loginType);
	void onClientShowIncomingCallUI(IRemoteCallInvitation* invitation);
	void onClientAcceptIncomingCall(const char* meeting_id);
	void onHostCancelInvitation();
	void onClientShowMissedCallUI(wstring caller);
public:
	ClientLoginUIMgr*			m_client_login_ui_mgr;
	ClientJoinMeetingUIMgr*		m_client_join_meeting_ui_mgr;
	ClientIncomingCallUIMgr*	m_client_incoming_call_ui_mgr;
	MissedCallUI*               m_missed_call_ui;
	USER_INFO_STRUCT*			m_client_user_param;
	MEETING_INFO_STRUCT*		m_client_meeting_param;
	HWND                        m_parentHWND;
	CAgoraObject*				m_agObject;
};

