#pragma once

#include "CurlHttpClient.h"
#include "AGConfig.h"
#include "clients_manager.h"
#include "Resource.h"
#include "../duilib/includes/UIlib.h"
#include "VideoScreenControl.h"
using namespace DuiLib;

class TwoWinsQueue
{
	//Win1 1
	//Win2 2
public:
	TwoWinsQueue()
	{
		m_pointer = 0;
		m_pQueue = new int[2];
		for (int i = 0; i < 2; i++)
		{
			m_pQueue[i] = i + 1;
		}
	};

	virtual ~TwoWinsQueue() {
		delete[] m_pQueue;
	};

	int getWin()
	{
		if (m_pointer == 2)
		{
			m_pointer = 0;
		}
		int operation = m_pQueue[m_pointer];
		m_pointer++;
		return operation;
	}

	void reset()
	{
		m_pointer = 0;
	}
private:
	int* m_pQueue;
	int m_pointer;
};

class ThreeWinsQueue
{
	//Win1 1
	//Win2 2
	//Win3 3
public:
	ThreeWinsQueue()
	{
		m_pointer = 0;
		m_pQueue = new int[3];
		for (int i = 0; i < 3; i++)
		{
			m_pQueue[i] = i + 1;
		}
	};

	virtual ~ThreeWinsQueue() {
		delete[] m_pQueue;
	};

	int getWin()
	{
		if (m_pointer == 3)
		{
			m_pointer = 0;
		}
		int operation = m_pQueue[m_pointer];
		m_pointer++;
		return operation;
	}

	void reset()
	{
		m_pointer = 0;
	}
private:
	int* m_pQueue;
	int m_pointer;
};
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
	void resetSelectedVideoMode();
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

	COptionUI* m_hdm1_btn;
	COptionUI* m_hdm2_btn;
	COptionUI* m_hdm3_btn;
	COptionUI* m_hdm4_btn;

	map<Individual*, CListContainerElementUI*>              m_model_ui_map;
	map<CButtonUI*, Individual*>      remoteMap;
	ClientsManager* m_clientsManager;
	bool m_bRemotingControling;
	MSG_EVENT_TYPE selectedVideoMode;
	MSG_EVENT_TYPE selectedWinMode;
	MSG_EVENT_TYPE selectedHDMIMode;

	int pictureDivider;
	TwoWinsQueue* m_twoWinsQueueForWinMode2;
	TwoWinsQueue* m_twoWinsQueueForWinMode4;
	TwoWinsQueue* m_twoWinsQueueForWinMode6;
	ThreeWinsQueue* m_threeWinsQueueForWinMode7;

private:
	void onRequestParticipantInfo(string uid);
	void updateParticipant(string resp);
	void enableRemoteControl(bool enable);
	void activeHDMIRadios();
	void handleParticipantInfo(string rsp);

};

