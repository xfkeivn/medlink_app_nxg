#pragma once

#include "Resource.h"
#include "client_common.h"
#include "string_util.h"
#include "MessageProtocal.h"
#include <mmsystem.h>
class ClientIncomingCallUIMgr;

class ClientIncomingCallUI
{
public:
	ClientIncomingCallUI();
	virtual ~ClientIncomingCallUI();
	void InitWindow(CPaintManagerUI& ui_mgr, ClientIncomingCallUIMgr* main_frame_);
	void UninitWindow();
	void Notify(TNotifyUI& msg);
	void DoJoinMeetingBtnClick();
	void DoRejectBtnClick();
	void updateTimingLabel(int i);
	bool isAnswered();
	ClientIncomingCallUIMgr* GetParentFrame();

protected:
	CHorizontalLayoutUI*	m_incoming_call_panel;
	CLabelUI*				m_callingLabelUI;
	CLabelUI*				m_timingLabelUI;
 	CButtonUI*				m_btnReject;
	//CButtonUI*				m_btnAccept;
	CGifAnimUI*				m_gitAccept;
	HANDLE                  m_hThread;
	bool                    answered;
	ClientIncomingCallUIMgr*		m_incomingCallUIMgr;
};

class ClientIncomingCallUIMgr : 
	public CWindowWnd, 
	public INotifyUI
{
public:
	ClientIncomingCallUIMgr(HWND hwnd, IRemoteCallInvitation* invitation);
	virtual ~ClientIncomingCallUIMgr();

public:
	virtual LPCTSTR		GetWindowClassName() const   {   return _T("incomingCallUI");  }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS ; };
	virtual UINT		GetSkinRes()				 {	 return IDXML_MLINK_INCOMING_CALL_UI; }
	UILIB_RESOURCETYPE GetResourceType() const{	return UILIB_RESOURCE; }
	HWND GetParentHWND();

	virtual void		InitWindow();
	virtual void		OnFinalMessage(HWND) {}

	virtual void		Notify( TNotifyUI& msg );
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);


	IRemoteCallInvitation* ClientIncomingCallUIMgr::GetMeetingInvitation();

protected:
	CPaintManagerUI m_PaintManager;
	ClientIncomingCallUI m_incommingCallUI;	

private:
	IRemoteCallInvitation* m_invitation;
	HWND               m_parentHWND;

};