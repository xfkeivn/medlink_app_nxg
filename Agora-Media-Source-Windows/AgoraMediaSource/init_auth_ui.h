#pragma once

#include "resource.h"
#include "client_common.h"
#include "string_util.h"
#include "httpclient.h"
#include "AGConfig.h"
#include "AGMessage.h"


/////////////////////////
class ClientLoginUIMgr;
class ClientLoginUI
{
public:
	ClientLoginUI();
	virtual ~ClientLoginUI();
	void InitWindow(CPaintManagerUI& ui_mgr, ClientLoginUIMgr* init_mgr);
	void UninitWindow();
	void Notify(TNotifyUI& msg);
	void LogOut();
	void DoLoginBtnClick();
	void SwitchToLoginPage();
	void ShowLoginErrorMessage(const wchar_t* error_message);
	void CheckNetwork();
	void UpdateConfig(bool loginResult);
	ClientLoginUIMgr* GetInitAuthLoginUIMgr();
	static void handleHttpLoginRes(string rsp, void* pParam);

	virtual void onLogout() {};
	virtual void onZoomIdentityExpired() {};
	virtual void onZoomAuthIdentityExpired() {};

protected:
	CVerticalLayoutUI*     m_loginPage;
	CRichEditUI*           m_editName;
	CRichEditUI*           m_editPassword;
	CCheckBoxUI*		   m_chkRememberMe;
	CButtonUI*		       m_btnLogin;
	CLabelUI*			   m_lableError;
	CHorizontalLayoutUI*   m_loginWaitingPanel;

	ClientLoginUIMgr*	   m_initMgr;
	bool                   m_bCheckNetwork;
	bool				   m_bConnectRedisServer;
	bool				   m_bSDKInit;
	bool				   m_bSDKAuth;
	bool                   m_rememberMe;	

private:
	void SDKInit();
};

///////////////////////////
class ClientLoginUIMgr :
	public CWindowWnd,
	public INotifyUI
{
public:
	ClientLoginUIMgr(HWND hWND, CAgoraObject* agObject);
	virtual ~ClientLoginUIMgr();

public:
	virtual LPCTSTR		GetWindowClassName() const { return _T("MedLinkClientUI"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	virtual UINT		GetSkinRes() { return IDXML_MLINK_LOGIN_UI; }
	UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; }

	virtual void		InitWindow();
	virtual void		OnFinalMessage(HWND) {}

	virtual void		Notify(TNotifyUI& msg);
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	HWND GetParentHWND();
	void SetCurrentPage(CVerticalLayoutUI* current_) { m_currentPage = current_; }

	void SwitchToWaitingPage(bool showWaitingPage, const wchar_t* waiting_message, bool show);
	void ShowErrorMessage(const wchar_t* error_message);
	void LogOut();
	void DoRetryBtnClick();
	void RTMLogin(string uid);

protected:
	CVerticalLayoutUI* m_waitingPage;
	CLabelUI*		   m_lableWaiting;
	CButtonUI*		   m_btnRetry;
	CGifAnimUI*		   m_gifWaiting;
	CPaintManagerUI    m_PaintManager;
	ClientLoginUI	   m_loginUI;
	CAgoraObject*      m_agObject;
	CVerticalLayoutUI* m_currentPage;
	HWND               m_parentHWND;
};