#pragma once
#include "../duilib/includes/UIlib.h"
#include "Resource.h"
#include <string>
#include <map>
using namespace std;

class MissedCallUI :
	public CWindowWnd,
	public INotifyUI
{
public:
	MissedCallUI(HWND hwnd, wstring caller);
	virtual ~MissedCallUI() {};

public:
	virtual LPCTSTR		GetWindowClassName() const { return _T("MissedCallUI"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	virtual UINT		GetSkinRes() { return IDXML_MISSEDCALL_UI; } 
	virtual void		InitWindow();
	virtual void		Notify(TNotifyUI& msg);
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


public:
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void addMissedCaller(wstring caller);
protected:
	CPaintManagerUI   m_PaintManager;
	CVerticalLayoutUI*     m_message_panel;
private:
	HWND hwndParent;
	wstring m_message;
	map<wstring, int> callersMap;
	CLabelUI* createMessageLabel(wstring message);
};

