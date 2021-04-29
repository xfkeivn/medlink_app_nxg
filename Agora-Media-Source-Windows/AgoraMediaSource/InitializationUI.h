#pragma once

#include "httpclient.h"
#include "Resource.h"
#include <WinInet.h>
#include "stringbuffer.h"
#include "writer.h"
#include "document.h"
#include "uuid.h"
#include "AGConfig.h"
#include "HospitalsInfo.h"
#include "string_util.h"
#include "../duilib/includes/UIlib.h"
using namespace DuiLib;
#pragma comment(lib, "Wininet.lib")


class InitializationUI :
	public CWindowWnd,
	public INotifyUI
{
public:
	InitializationUI(HWND hwnd);
	virtual ~InitializationUI();


public:
	virtual LPCTSTR		GetWindowClassName() const { return _T("InitializationUI"); }
	virtual UINT		GetSkinRes() { return IDXML_INITIALIZATION_UI; }
	virtual void		InitWindow();
	virtual void		Notify(TNotifyUI& msg);
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


public:
	void ShowWaiting(bool bWaiting);
	void showInitializationError(const wchar_t* error);

protected:
	CPaintManagerUI      m_PaintManager;

private:
	CContainerUI*	     m_waiting_panel;
	CContainerUI*        m_login_panel;
	CComboUI*            m_hospital_combo;
	CComboUI*            m_equipment_combo;
	CComboUI*            m_department_combo;
	//CRichEditUI*         m_edit_hospital;
	//CRichEditUI*         m_edit_equipment;
	//CRichEditUI*         m_edit_telephone;
	CButtonUI*           m_init_btn;

	CGifAnimUI*			 m_gifWaiting;
	CLabelUI*            m_waitingMessage;
	
	string               webserver_url;
	HWND hwndParent;
	HospitalMgr*         m_hospitalMgr;
private:
	bool checkNetworkStatus();
	void onRequestMeetingAccount();
	void onRequestHospitalInfos();
	void handlerHospitalInfos(string rsp);
	void logToRTMService(string user_id, string channel_name);
	wstring stringToWstring(const std::string& str);
	void InitAllControls();

public:
	static void handleMeetingAccount(string rsp, void * pParam);
	static void onReceiveHospitalInfos(string rsp, void* pParam);

};

