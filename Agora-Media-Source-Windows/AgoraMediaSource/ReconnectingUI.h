#pragma once
#include "../duilib/includes/UIlib.h"
#include "Resource.h"
class ReconnectingUI :
	public CWindowWnd,
	public INotifyUI
{
public:
	ReconnectingUI(HWND hwnd, bool self_reconnect, int timer);
	virtual ~ReconnectingUI() {};

public:
	virtual LPCTSTR		GetWindowClassName() const { return _T("ReconnectingUI"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	virtual UINT		GetSkinRes() { return IDXML_RECONNECTING_UI; }
	virtual void		InitWindow();
	virtual void		Notify(TNotifyUI& msg);
	virtual void        OnFinalMessage(HWND hwnd);
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


public:
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void updateTimeLabel(int time);
protected:
	CPaintManagerUI   m_PaintManager;
	CButtonUI*           m_exit_btn;
	CLabelUI*            m_time_label;
	CLabelUI*            m_message_label;
private:
	HWND hwndParent;
	wstring m_init_message;
	wstring m_end_message;
	bool m_self_reconnect;
	int m_timer;
};

