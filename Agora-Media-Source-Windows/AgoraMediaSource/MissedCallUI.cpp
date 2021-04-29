#include "stdafx.h"
#include "MissedCallUI.h"

MissedCallUI::MissedCallUI(HWND hwnd, wstring caller)
{
	this->hwndParent = hwnd;
	callersMap.insert(map<wstring, int>::value_type(caller, 1));
	wstring message = L"Missed call from \"" + caller + L"\".";
	this->m_message = message;
}

void MissedCallUI::InitWindow()
{
	RECT rc = { 0 };
	if (!::GetClientRect(m_hWnd, &rc)) return;

	if (!::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd))) return;

	//::SetWindowPos(m_hWnd, NULL, left, top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//´°¿ÚÖÃ¶¥
	//m_message_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("message")));
	//m_message_label->SetTextStyle(DT_WORDBREAK);
	//m_message_label->SetText(m_message.c_str());
	m_message_panel = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("message_panel")));
	CLabelUI* label = createMessageLabel(m_message);
	m_message_panel->Add(label);
}

void MissedCallUI::Notify(TNotifyUI& msg)
{

}


LRESULT MissedCallUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	if (uMsg == WM_CREATE)
	{
		m_PaintManager.Init(m_hWnd);
		CDialogBuilder       builder;
		STRINGorID xml(GetSkinRes());
		CControlUI* pRoot = builder.Create(xml, _T("xml"), 0, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		InitWindow();
		return lRes;
	}
	else if (uMsg == WM_DESTROY)
	{
		OnDestroy(uMsg, wParam, lParam, bHandled);
	}
	else if (uMsg == WM_CLOSE)
	{
		OnClose(uMsg, wParam, lParam, bHandled);
	}
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MissedCallUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostMessage(hwndParent, WM_CLOSE, 0, 0);
	//DestroyWindow(GetHWND());
	callersMap.clear();
	return 0;
}

LRESULT MissedCallUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostMessage(hwndParent, WM_CLOSE, 0, 0);
	return 0;
}

void MissedCallUI::addMissedCaller(wstring caller)
{
	m_message_panel->RemoveAll();
	map<wstring, int>::iterator it = callersMap.begin();
	wstring message = L"";
	bool newMissedCaller = true;
	while (it != callersMap.end())
	{
		wstring key = it->first;
		int value = it->second;
		if (key == caller)
		{
			value++;
			callersMap[caller] = value;
			newMissedCaller = false;
		}
		if (value > 1)
		{
			message = L"Missed Call from \"" + key + L"\"(" + to_wstring(value) + L").";
		}
		else
		{
			message = L"Missed Call from \"" + key + L"\".";
		}
		m_message_panel->Add(createMessageLabel(message));
		it++;
	}
	if (newMissedCaller)
	{
		message = L"Missed Call from \"" + caller + L"\".";
		m_message_panel->Add(createMessageLabel(message));
	}
}

CLabelUI* MissedCallUI::createMessageLabel(wstring message)
{
	CLabelUI* label = new CLabelUI(); 
	label->SetFont(1);
	label->SetAttribute(_T("textcolor"), _T("#FFFFFFFF"));
	label->SetAttribute(_T("align"), _T("center"));
	label->SetTextStyle(DT_WORDBREAK);
	label->SetText(message.c_str());
	return label;
}
