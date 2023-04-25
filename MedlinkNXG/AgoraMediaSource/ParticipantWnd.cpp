#include "stdafx.h"
#include "ParticipantWnd.h"
#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include "UIListCommonDefine.hpp"
#include "commonFun.h"
const TCHAR* const kFriendButtonControlName = _T("friendbtn");
const TCHAR* const kFriendsListControlName = _T("friends");
const TCHAR* const kFriendsListTitle = _T("listtitle");
ParticipantWnd::ParticipantWnd()
	: bk_image_index_(0)
{}

ParticipantWnd::~ParticipantWnd()
{
	//PostQuitMessage(0);
}

LPCTSTR ParticipantWnd::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CControlUI* ParticipantWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendsUI(m_PaintManager);
	}
	if (_tcsicmp(pstrClass, _T("Label")) == 0)
	{
		return new CLabelUI();
	}
	return NULL;
}

void ParticipantWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this;
}

CDuiString ParticipantWnd::GetSkinFile()
{
	return _T("ParticipaentUI.xml");
}

CDuiString ParticipantWnd::GetSkinFolder()
{
	return  _T(".\\skin\\");
}

LRESULT ParticipantWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ParticipantWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT ParticipantWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void ParticipantWnd::OnTimer(TNotifyUI& msg)
{
}

void ParticipantWnd::OnExit(TNotifyUI& msg)
{
	Close();
}

void ParticipantWnd::InitWindow()
{}

void ParticipantWnd::onUserJoined(UINT uid)
{
	// if the user is already in, skip
	vector<FriendListItemInfo>::iterator end = friends_.end();
	vector<FriendListItemInfo>::iterator itor = friends_.begin();
	while (itor != end)
	{
		FriendListItemInfo &info = *itor;
		if (str2int(cs2s(info.id.GetData())) == uid)
		{
			return;
		}
		itor++;
	}


	FriendListItemInfo item;
	item.id = s2cs(int2str(uid));
	item.folder = false;

	// Get the log from DB,this should be done before the event to avoid long time of this event processing
	item.logo = _T("man.png");
	item.nick_name = _T("张华兵");
	item.description = _T("13818713295");
	friends_.push_back(item);

	UpdateFriendsList();
}
void ParticipantWnd::onUserLeave(UINT uid)
{
	vector<FriendListItemInfo>::iterator end = friends_.end();
	vector<FriendListItemInfo>::iterator itor = friends_.begin();
	while (itor != end)
	{
		FriendListItemInfo &info= *itor;
		if (str2int(cs2s(info.id.GetData())) == uid)
		{
			friends_.erase(itor);
			break;
		}
		itor++;
	}

	UpdateFriendsList();

}
void ParticipantWnd::onUserListUpdate(UINT *uids, UINT count)
{
	if (!friends_.empty())
		friends_.clear();
	for (int i = 0; i < count; i++)
	{
		FriendListItemInfo item;
		item.id = s2cs(int2str(uids[i]));
		item.folder = false;

		// Get the log from DB,this should be done before the event to avoid long time of this event processing
		item.logo = _T("man.png");
		item.nick_name = _T("张华兵");
		item.description = _T("13818713295");
		friends_.push_back(item);
	}

	UpdateFriendsList();

}

void ParticipantWnd::UpdateFriendsList()
{
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(_T("friends")));
	CLabelUI* label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("listtitle")));
	if (pFriendsList != NULL)
	{

		if (pFriendsList->GetCount() > 0)
			pFriendsList->RemoveAll();
		Node* root_parent = NULL;

		vector<FriendListItemInfo>::iterator end = friends_.end();
		vector<FriendListItemInfo>::iterator itor = friends_.begin();
		while (itor != end)
		{
			FriendListItemInfo &info = *itor;
			pFriendsList->AddNode(info, root_parent);
			itor++;
		}


	}
}


void ParticipantWnd::OnPrepare(TNotifyUI& msg)
{

	UpdateFriendsList();

}

void ParticipantWnd::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
		/*
		if (_tcsicmp(msg.pSender->GetName(), kSignatureControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* signature_tip = m_PaintManager.FindControl(kSignatureTipsControlName);
			if (signature_tip != NULL)
			{
				CRichEditUI* signature = static_cast<CRichEditUI*>(msg.pSender);
				if (signature != NULL)
					signature_tip->SetText(signature->GetText());
				signature_tip->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* search_tip = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditTipControlName));
			if (search_tip != NULL)
			{
				CRichEditUI* search_edit = static_cast<CRichEditUI*>(msg.pSender);
				if (search_edit != NULL)
					search_tip->SetText(search_edit->GetText());
				search_tip->SetVisible(true);
			}
		}
		*/
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		/*
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if (pControl) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if (pControl) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kHideLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = m_PaintManager.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = m_PaintManager.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = m_PaintManager.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(false);
				left_main_pannel->SetVisible(false);
				show_left_main_pannel->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kShowLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = m_PaintManager.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = m_PaintManager.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = m_PaintManager.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(true);
				left_main_pannel->SetVisible(true);
				show_left_main_pannel->SetVisible(false);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSignatureTipsControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* signature = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSignatureControlName));
			if (signature != NULL)
			{
				signature->SetText(msg.pSender->GetText());
				signature->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditTipControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* search_edit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditControlName));
			if (search_edit != NULL)
			{
				search_edit->SetText(msg.pSender->GetText());
				search_edit->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeBkSkinControlName) == 0)
		{
			CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
			if (background != NULL)
			{
				TCHAR szBuf[MAX_PATH] = { 0 };
				++bk_image_index_;
				if (kBackgroundSkinImageCount < bk_image_index_)
					bk_image_index_ = 0;

#if defined(UNDER_WINCE)
				_stprintf(szBuf, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#else
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#endif
				background->SetBkImage(szBuf);

				SkinChangedParam param;
				CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
				if (background != NULL)
				{
					param.bkcolor = background->GetBkColor();
					if (_tcslen(background->GetBkImage()) > 0)
					{
#if defined(UNDER_WINCE)
						_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
						_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
					}

					param.bgimage = szBuf;
				}
				skin_changed_observer_.Broadcast(param);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeColorSkinControlName) == 0)
		{
			CDuiRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);
			rcWindow.top = rcWindow.top + msg.pSender->GetPos().bottom;
			new ColorSkinWindow(this, rcWindow);
		}
		*/
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{



		UpdateFriendsList();
	}

	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		
				CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(kFriendsListControlName));
				if ((pFriendsList != NULL) && pFriendsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (!pFriendsList->CanExpand(node) )
						{
							FriendListItemInfo friend_info;

							for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); ++citer)
							{
								if (_tcsicmp(citer->id, node->data().value) == 0)
								{
									friend_info = *citer;
									break;
								}
							}
							TCHAR szBuf[MAX_PATH] = _T("bg1.png");
							
						}
					
					}


				}
			}
		
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{

	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(kFriendsListControlName));
		if ((pFriendsList != NULL) && pFriendsList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();

				if (pFriendsList->CanExpand(node))
				{
					pFriendsList->SetChildVisible(node, !node->data().child_visible_);
				}
			}
		}
	}
			
			

		
		
	}


LRESULT ParticipantWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

UILIB_RESOURCETYPE ParticipantWnd::GetResourceType() const
{
	return UILIB_FILE;
}

LPCTSTR ParticipantWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(101);
}
