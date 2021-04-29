#pragma once

#include "httpclient.h"
#include "clients_manager.h"
#include "uuid.h"
#include "Resource.h"
#include "AGMessage.h"
#include "MessageProtocal.h"
#include "AGConfig.h"
#include "../duilib/includes/UIlib.h"
#include "client_common.h"
#include "AGConfig.h"
#include "string_util.h"
#include "HIDControl.h"
using namespace DuiLib;


class LoggedInUI : 
	public CWindowWnd,
	public INotifyUI
{
public:
	LoggedInUI(HWND hwnd, string channel);
	virtual ~LoggedInUI();
	void ShowErrorMessage(const wchar_t* error_message);
	
	void updateClientsStatus(string user_id, UserStatus status, string client_channel);
	void switchToLoggedInUI();
	void onInvitationRefuseOrTimeoutByClient(string client_id, bool refuse);
	void onInvitationEnd();
	void setMeetingStatus(bool inMeeting);
	bool isInMeeting();
	bool isInCalling();
	void setCameraDisabled(bool disable) {
		this->bCameraDisabled = disable;
	}
	bool isCameraDisabled() {
		return this->bCameraDisabled;
	}
	wstring getDisplayName(string user_id);
public:
	virtual LPCTSTR		GetWindowClassName() const { return _T("LoggedInUI"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	virtual UINT		GetSkinRes() { return IDXML_LOGGEDIN_UI; }
	virtual UINT        GetIndividualRes() { return IDXML_INDIVIDUAL_ITEM; }
	virtual UINT        GetGroupRes() { return IDXML_GROUP_ITEM; }
	virtual UINT        GetGroupIndividualRes() { return IDXML_GROUP_INDIVIDUAL_ITEM; }
	virtual void		InitWindow();
	virtual void		Notify(TNotifyUI& msg);
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


public:
	//LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void DoStartMeetingBtnClick();
	static DWORD WINAPI threadTiming(LPVOID lpParamter);
	void removeThread(string user_id);
	//void teminalAllThreads();
protected:
	CPaintManagerUI                                      m_PaintManager;
	map<CButtonUI*, vector< CListContainerElementUI*>>   groupMap;
	map<CCheckBoxUI*, Individual*>                       checkMap;
	map<Individual*, CListContainerElementUI*>           m_model_ui_map;
	map<string, HANDLE>                                  m_thread_map;
private:
	CButtonUI* m_btnStartMeeting;
	CListUI*   m_clientslist;
	bool m_bInMeeting;
	ClientsManager* clients_manager;
	bool m_inCalling;

	HWND hwndParent;
	string channel;
	int icon_size;
	int status_size;
	//HANDLE m_hThread;
	bool bCameraDisabled = true;
private:
	LPCTSTR string2LPCTSTR(string str);
	void  onRequestHostClients();
	void onClientsUpdate(string rsp);
	void clearSelectedClients();
	static void handleHostClients(string rsp, void* pParam);
	string getStatusDestString();
};