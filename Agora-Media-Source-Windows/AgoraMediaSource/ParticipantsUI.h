#pragma once

#include "httpclient.h"
#include "AGConfig.h"
#include "clients_manager.h"
#include "Resource.h"
#include "../duilib/includes/UIlib.h"
#include "VideoScreenControl.h"
using namespace DuiLib;


class ParticipantsUI :
	public CWindowWnd,
	public INotifyUI
{
public:
	ParticipantsUI(HWND hwnd);
	virtual ~ParticipantsUI() {};

	void onUserListUpdate(UINT *uids, UINT count);
	Individual* onUserJoined(UINT uid);
	wstring onUserLeave(UINT uid);
	void updateUIForRemoteControl(UINT control_user, bool control);
	void updateUIForAnnotation(bool startAnnotation);
	void setRemoteControlling(bool isRemoteControlling);
	wstring updateUIForDropOffline(UINT uid);
	Individual* getParticipant(UINT uid);
	void selectVideoMode(MSG_EVENT_TYPE type, bool trigger, bool broadcast);
	void restoreSelectedVideoMode();
public:
	virtual LPCTSTR		GetWindowClassName() const { return _T("ParticipantsUI"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	virtual UINT		GetSkinRes();
	virtual UINT        GetParticipantItemRes();

	virtual void		InitWindow();
	virtual void		Notify(TNotifyUI& msg);
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);



protected:
	CPaintManagerUI  m_PaintManager;


private:
	HWND hwndParent;
	CListUI*   m_participantslist;
	COptionUI* m_pip_btn;
	COptionUI* m_pbyp_btn;
	COptionUI* m_pup_btn;
	COptionUI* m_hd1_btn;
	COptionUI* m_hd2_btn;
	CHorizontalLayoutUI* m_pip_control_container;
	CVerticalLayoutUI* m_video_2input_ctrl;
	CVerticalLayoutUI* m_video_4input_ctrl;
	COptionUI* m_winmode1_btn;
	COptionUI* m_winmode2_btn;
	COptionUI* m_winmode4_btn;
	COptionUI* m_winmode6_btn;
	COptionUI* m_winmode7_btn;
	COptionUI* m_winmode8_btn;
	CHorizontalLayoutUI* m_winmode1_prop;
	COptionUI* m_winmode1_hdm1_btn;
	COptionUI* m_winmode1_hdm2_btn;
	COptionUI* m_winmode1_hdm3_btn;
	COptionUI* m_winmode1_hdm4_btn;
	CVerticalLayoutUI* m_winmode2_prop;
	CVerticalLayoutUI* m_winmode4_prop;
	CVerticalLayoutUI* m_winmode6_prop;
	CVerticalLayoutUI* m_winmode7_prop;

	map<Individual*, CListContainerElementUI*>              m_model_ui_map;
	map<CButtonUI*, Individual*>      remoteMap;
	ClientsManager* m_clientsManager;
	bool m_bRemotingControling;
	MSG_EVENT_TYPE selectedVideoMode;
	MSG_EVENT_TYPE selectedWinMode;
	MSG_EVENT_TYPE selectedWin1HDIMode;
private:
	void onRequestParticipantInfo(string uid);
	void updateParticipant(string resp);
	void enableRemoteControl(bool enable);
public:
	static void handleParticipantInfo(string rsp, void * pParam);

};