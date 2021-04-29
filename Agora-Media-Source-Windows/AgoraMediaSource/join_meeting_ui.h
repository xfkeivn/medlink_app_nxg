#pragma once
#include "resource.h"
#include "client_common.h"
#include "string_util.h"

class ClientJoinMeetingUIMgr;

class ClientJoinMeetingUI
{
public:
	ClientJoinMeetingUI();
	virtual ~ClientJoinMeetingUI();
	void InitWindow(CPaintManagerUI& ui_mgr, ClientJoinMeetingUIMgr* main_frame_);
	void UninitWindow();
	void Show();
	void Hide();
	void Notify(TNotifyUI& msg);
	void DoJoinMeetingBtnClick();
	void DoSignoutBtnClick();
	void acceptIncomingCall(const std::string &meeting_id);
	static void handleHttpReportStatusRes(string rsp, void* pParam);
	void showNormalButton();
private:
	void HideWaitingPanel();
	void ShowWaitingPanel();
	void ShowErrorMsg(const wchar_t* str);
protected:
 	CVerticalLayoutUI* m_JoinMeetingPage;
 	CRichEditUI*	   m_editMeetingNumber;
 	CButtonUI*	       m_btnJoin;
	CCheckBoxUI*       m_checkBoxAudio;
	CVerticalLayoutUI* m_joinWaitingPanel;
	CGifAnimUI*		   m_gifJoinWaiting;
	CLabelUI*          m_errLabelUI;

	ClientJoinMeetingUIMgr*  m_joinMeetingUIMgr;
	bool m_bJoinIn;
};
class ClientJoinMeetingUIMgr : 
	public CWindowWnd, 
	public INotifyUI
{
public:
	ClientJoinMeetingUIMgr(HWND hWND);
	virtual ~ClientJoinMeetingUIMgr();
	

public:
	virtual LPCTSTR		GetWindowClassName() const   {   return _T("zSDKDemoUI");  }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS ; }
	virtual UINT		GetSkinRes()				 {	 return IDXML_MLINK_JOIN_MEETING_UI; }
	UILIB_RESOURCETYPE GetResourceType() const{	return UILIB_RESOURCE; }
	HWND GetParentHWND();

	virtual void		InitWindow();
	virtual void		OnFinalMessage(HWND) {}

	virtual void		Notify( TNotifyUI& msg );
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void acceptIncomingCall(const std::string &meeting_id);
	void show();
protected:
	CPaintManagerUI m_PaintManager;
	ClientJoinMeetingUI m_JoinMeetingUI;	
	HWND                m_parentHWND;
};