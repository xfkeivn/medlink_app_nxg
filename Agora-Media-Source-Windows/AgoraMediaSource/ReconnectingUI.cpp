#include "stdafx.h"
#include "ReconnectingUI.h"

ReconnectingUI::ReconnectingUI(HWND hwnd, bool self_reconnect, int timer)
{
	this->hwndParent = hwnd;

	this->m_self_reconnect = self_reconnect;
	this->m_timer = timer;
}

void ReconnectingUI::InitWindow()
{
	RECT rc = { 0 };
	if (!::GetClientRect(m_hWnd, &rc)) return;
	rc.right = rc.left + 500;
	rc.bottom = rc.top + 350;
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//´°¿ÚÖÃ¶¥
	//m_exit_btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_exit")));
	//m_exit_btn->SetVisible(false);
	//m_time_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("timer")));
	m_message_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("message")));
	//m_message_label->SetText(m_init_message.c_str());
	m_message_label->SetTextStyle(DT_WORDBREAK);
	CString str;
	if (m_self_reconnect)
	{
		str.Format(L"You are offline now, please recover your connection, or meeting will end in %d seconds...", m_timer);
	}
	else
	{
		str.Format(L"Catheter Lab is offline now, please hold for host back online, or meeting will end in %d seconds...", m_timer);
	}
	m_message_label->SetText(str);
}

void ReconnectingUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{

		if (msg.pSender == m_exit_btn)
		{
			//PostQuitMessage(0);
			::SendMessage(this->hwndParent, WM_NETWORK_ERROR, 0, 0);
		}
	}
}

void ReconnectingUI::OnFinalMessage(HWND hwnd)
{
	__super::OnFinalMessage(hwnd);
	delete this;
}
LRESULT ReconnectingUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		SetTimer(GetHWND(), 1, 1000, NULL);
		return lRes;
	}
	else if (uMsg == WM_TIMER)
	{
		m_timer--;
		if (m_timer > 0)
		{
			CString str;
			if (m_self_reconnect)
			{
				str.Format(L"You are offline now, please recover your connection, or meeting will end in %d seconds...", m_timer);
			}
			else
			{
				str.Format(L"Catheter Lab is offline now, please hold for host back online, or meeting will end in %d seconds...", m_timer);
			}
			//wstring wstr = to_wstring(m_timer) + L"s";
			m_message_label->SetText(str);
			
		}
		else
		{
			//m_message_label->SetText(m_end_message.c_str());
			//m_exit_btn->SetVisible(true);
			//m_time_label->SetVisible(false);
			KillTimer(GetHWND(), 1);
			::PostMessage(this->hwndParent, WM_NETWORK_ERROR, 0, 0);
			Close();
		}
	}
	else if (uMsg == WM_NCACTIVATE)
	{
		if (!::IsIconic(m_hWnd))
		{
			return (wParam == 0) ? TRUE : FALSE;
		}
	}
	else if (uMsg == WM_NCCALCSIZE)
	{
		return 0;
	}
	else if (uMsg == WM_NCPAINT)
	{
		return 0;
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

LRESULT ReconnectingUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostMessage(hwndParent, WM_CLOSE, 0, 0);
	//DestroyWindow(GetHWND());
	return 0;
}

LRESULT ReconnectingUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostMessage(hwndParent, WM_CLOSE, 0, 0);
	return 0;
}

void ReconnectingUI::updateTimeLabel(int i)
{
	//m_time_label->SetText(to_wstring(i).c_str());
}