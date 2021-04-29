#pragma once

#include "httpclient.h"
#include "InitializationUI.h"
#include "VideoScreenControl.h"
#include "LoggedInUI.h"
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#include <SetupAPI.h>
#pragma comment(lib, "setupapi.lib")

class DUIMgr
{
public:
	DUIMgr(HWND hwnd);
	~DUIMgr();

public:
	void onShowLoggedInUI();
	void onSwitchToLoggedInUI();
	void onShowInitializationUI();
	void onShowError(const wchar_t* error_message);
	void setChannelName(const char* channel);
	const char* getChannelName();
	void setUserId(int id);
	int getUserId();
	void setMeetingID(int id);
	int getMeetingID();
	void updateCOMPort();
	void updateClientsStatus(string user_id, UserStatus status,string client_channel);
	void onInvitationRefusedByClient(string client_id);
	void onInvitationTimeoutByClient(string client_id);
	void onInvitationEnd();
	void setMeetingStatus(bool inMeeting);
	bool isInMeeting();
	wstring getDisplayName(string user_id);
	InitializationUI* initialization_ui;
private:
	void updateCOMPortandDisableCamera();
	
	HWND m_hwndParent;
	
	LoggedInUI* loggedIn_ui;
	const char* strChannelName;
	int userId;
	const char* hostpital;
	int meetingId;
};