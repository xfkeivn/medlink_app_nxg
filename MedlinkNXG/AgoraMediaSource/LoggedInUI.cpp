#include "stdafx.h"
#include "LoggedInUI.h"
#include "CTipDlg.h"
#include "BackendComm.h"
extern bool compareWithStatusAndName(CListContainerElementUI * item1, CListContainerElementUI * item2);

typedef struct _CLIENT_PARAM {
	LoggedInUI *loggedInUI;
	string user_id;
}CLIENT_PARAM, *PCLIENT_PARAM;

void LoggedInUI::handleHostClients(vector<Individual*> &clients)
{
	m_clientslist->RemoveAll();
	m_model_ui_map.clear();
	m_listElements.clear();
	for (int i = 0; i < clients.size(); i++)
	{
		Individual* individual = dynamic_cast<Individual*>(clients[i]);
		CommandManager::GetInstance()->sendQueryClientStatus(atoi(individual->getUserId().c_str()));
		CDialogBuilder builder;
		CListContainerElementUI* pListItem;

		STRINGorID individual_xml(GetIndividualRes());
		pListItem = static_cast<CListContainerElementUI *>(builder.Create(individual_xml, _T("xml"), 0, &m_PaintManager));
		CControlUI* item_left = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_left")));
		CControlUI* item_space1 = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_space1")));
		CControlUI* item_space2 = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_space2")));
		CVerticalLayoutUI* item_check_container = static_cast<CVerticalLayoutUI*>(pListItem->FindSubControl(_T("item_check_container")));
		CVerticalLayoutUI* item_status_container = static_cast<CVerticalLayoutUI*>(pListItem->FindSubControl(_T("item_status_container")));
		CLabelUI *pNameLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("name_label")));
		CLabelUI *pTelLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("telephone_label")));
		CButtonUI *pStatus = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("status_btn")));
		CCheckBoxUI *checkbox = static_cast<CCheckBoxUI *>(pListItem->FindSubControl(_T("checkbox")));

		pListItem->SetName(string2LPCTSTR(individual->getUserId()));
		int listitem_height = getResolutionHeight(MAX_CLIENT_LISTITEM_HEIGHT);
		pListItem->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(listitem_height)).c_str());

		int space = getResolutionWidth(MAX_ITEM_SPACE);
		item_left->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());
		item_space1->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());
		item_space2->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());

		int check_size = getResolutionSize(MAX_CHECKBOX_SIZE);
		item_check_container->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(check_size)).c_str());
		checkbox->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(check_size)).c_str());
		checkbox->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(check_size)).c_str());

		icon_size = getResolutionSize(MAX_STATUS_BUTTON_SIZE);
		status_size = getResolutionSize(MAX_STATUS_SIZE);
		item_status_container->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(icon_size)).c_str());
		pStatus->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(icon_size)).c_str());
		pStatus->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(icon_size)).c_str());


		checkMap.insert(map<CCheckBoxUI*, Individual*>::value_type(checkbox, individual));
		pNameLab->SetText(individual->getWName().c_str());
		pTelLab->SetText(string2LPCTSTR(individual->getTelephone()));
		int size = getResolutionFontSize(MAX_TITLE_SIZE);
		pNameLab->SetFont(size);
		pTelLab->SetFont(size);
		if (individual->getImagePath() != "")
		{
			string s = "file='" + individual->getImagePath() + "'";
			pStatus->SetBkImage(string2LPCTSTR(s));
		}
		m_clientslist->Add(pListItem);
		m_listElements.push_back(pListItem);
		m_model_ui_map.insert(map<Individual*, CListContainerElementUI*>::value_type(clients[i], pListItem));
	}
	if (m_model_ui_map.size() > 0)
	{

		const char* peerIds[100];
		map<Individual*, CListContainerElementUI*>::iterator iter;
		iter = m_model_ui_map.begin();
		int i = 0;
		while (iter != m_model_ui_map.end())
		{
			if (!iter->first->isGroup())
			{
				Individual* individual = dynamic_cast<Individual*>(iter->first);

				peerIds[i] = individual->getUserId().c_str();
				i++;
			}
			iter++;
		}
		CommandManager::GetInstance()->subscribePeersOnlineStatus(peerIds, i);
	}
}

LoggedInUI::LoggedInUI(HWND hwnd, string channnel)
{
	this->channel = channnel;
	this->hwndParent = hwnd;
	this->m_bInMeeting = false;
	m_inCalling = false;
	m_btnStartMeeting = NULL;
	m_clientslist == NULL;
	clients_manager = ClientsManager::getInstance();
}

LoggedInUI::~LoggedInUI()
{
	m_btnStartMeeting = NULL;
	m_clientslist == NULL;
}

void LoggedInUI::InitWindow()
{
	RECT rc = { 0 };
	if (!::GetClientRect(m_hWnd, &rc)) return;

	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	int bar_height = getResolutionHeight(MAX_BAR_HEIGHT);
	rc.right = rc.left + rcWorkArea.Width();// width;
	rc.bottom = rc.top + rcWorkArea.Height() - bar_height;// 64;// height;
	if (!::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd))) return;
	
	::SetWindowPos(m_hWnd, NULL, 0, bar_height, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);

	CVerticalLayoutUI* left = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("left")));
	left->SetFixedWidth(rcWorkArea.Width() * 3 / 10);
	CLabelUI* contacts_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("contacts_label")));
	int size = getResolutionFontSize(MAX_TITLE_SIZE);
	contacts_label->SetFont(size);
	int left_padding = getResolutionWidth(TEXT_LEFT_PADDING);
	int top_padding = getResolutionHeight(TEXT_TOP_PADDING);
	int bottom_padding = top_padding;
	string attr = to_string(left_padding) + "," + to_string(top_padding) + "," + to_string(bottom_padding) +",0";
	contacts_label->SetAttribute(_T("textpadding"), StringUtil::StringToWstring(attr).c_str());
	CControlUI* bsc_image_top = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("bsc_image_top")));
	int image_top = getResolutionHeight(MAX_BSC_IMAGE_TOP);
	bsc_image_top->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(image_top)).c_str());
	CControlUI* bsc_image_right = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("bsc_image_right")));
	int image_right = getResolutionWidth(MAX_BSC_IMAGE_RIGHT);
	bsc_image_right->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(image_right)).c_str());
	CControlUI* bsc_image = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("bsc_image")));
	int image_width = getResolutionWidth(MAX_BSC_IMAGE_WIDTH);
	int image_height = getResolutionHeight(MAX_BSC_IMAGE_HEIGHT);
	bsc_image->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(image_width)).c_str());
	bsc_image->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(image_height)).c_str());

	m_btnStartMeeting = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_startMeeting")));
	int button_width = getResolutionWidth(MAX_START_BUTTON_WIDTH);	
	int button_height = getResolutionHeight(MAX_START_BUTTON_HEIGHT);
	m_btnStartMeeting->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(button_width)).c_str());
	m_btnStartMeeting->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(button_height)).c_str());

	m_btnDirectStartMeeting = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_directStartMeeting")));
	m_btnDirectStartMeeting->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(button_width)).c_str());
	m_btnDirectStartMeeting->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(button_height)).c_str());



	if (!CAGConfig::GetInstance()->GetVersion().IsEmpty())
	{
		CLabelUI* version_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("version_label")));
		size = getResolutionFontSize(MAX_VERSION_SIZE);
		version_label->SetFont(size);
		CString version = L"Version:" + CAGConfig::GetInstance()->GetVersion();
		version_label->SetText(version);
		int padding_right = getResolutionWidth(VERSION_PADDING_RIGHT);
		int padding_bottom = getResolutionHeight(VERSION_PADDING_BOTTOM);
		RECT padding = { 0, 0, padding_right, padding_bottom };
		version_label->SetPadding(padding);
	}
	else
	{
		logWarn("The version is empty.");
	}
	m_clientslist = static_cast<CListUI*>(m_PaintManager.FindControl(_T("clients_list")));
	CScrollBarUI* pListVerticalScrollBar = m_clientslist->GetVerticalScrollBar();
	int scrollbar_width = getResolutionWidth(MAX_CLIENTS_SCROLLBAR_WIDTH);
	//pListVerticalScrollBar->SetAttribute(_T("width"), _T("48"));
	pListVerticalScrollBar->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(scrollbar_width)).c_str());
	m_btnStartMeeting->SetEnabled(false);
	m_btnDirectStartMeeting->SetEnabled(true);
	onRequestHostClients();
}

LRESULT LoggedInUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}



LRESULT LoggedInUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnStartMeeting = NULL;
	::PostMessage(hwndParent, WM_CLOSE, 0, 0);
	return 0;
}
void LoggedInUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_btnDirectStartMeeting)
		{
			::PostMessage(hwndParent, WM_HOST_DIRECT_START_MEETING, NULL, NULL);
		}

		if (msg.pSender == m_btnStartMeeting)
		{
			m_inCalling = !m_inCalling;
			if (m_inCalling)
			{
				logInfo("Click start meeting button to call a meeting");
				m_btnStartMeeting->SetNormalImage(_T("file = 'res\\calling_normal.png'"));
				m_btnStartMeeting->SetHotImage(_T("file = 'res\\calling_hot.png'"));
				m_btnStartMeeting->SetPushedImage(_T("file = 'res\\calling_push.png'"));
				vector<Individual*> invitedClients = clients_manager->getInvitedClients();
				if (invitedClients.size() > 0)
				{
					logInfo("Invite selected clients to start meeting:");
					for (int k = 0; k < invitedClients.size(); k++)
					{
						Individual* individual = static_cast<Individual*>(invitedClients[k]);
						logInfo("Send invitation to " + individual->getUserId());
						CommandManager::GetInstance()->Invite(atoi(individual->getUserId().c_str()), this->channel);
						individual->setStatus(UserStatus::USER_STATUS_INVITING_PENDING);
					}
				}
				else
				{
					logInfo("No selected clients, invite an available client to start meeting");
					map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.begin();
					while (iter != m_model_ui_map.end())
					{
						Individual* individual = static_cast<Individual*>(iter->first);
						if (individual->getStatus() == UserStatus::USER_STATUS_LOGIN || individual->getStatus() == UserStatus::USER_STATUS_INVITING_PENDING || individual->getStatus() == USER_STATUS_JOIN_MEETING_WAITING)
						{
							logInfo("Send invitation to " + individual->getUserId() + ", which status is " + StringUtil::getClientStatusName(individual->getStatus()));
							CommandManager::GetInstance()->Invite(atoi(individual->getUserId().c_str()), this->channel);
							if (individual->getStatus() != USER_STATUS_JOIN_MEETING_WAITING)
							{
								individual->setStatus(UserStatus::USER_STATUS_INVITING_PENDING);
							}
							break;
						}
						iter++;
					}
				}
			}
			else
			{
				logInfo("Click button to cancel the call");
				m_btnStartMeeting->SetNormalImage(_T("file = 'res\\start_meeting_normal.png'"));
				m_btnStartMeeting->SetHotImage(_T("file = 'res\\start_meeting_hot.png'"));
				m_btnStartMeeting->SetPushedImage(_T("file = 'res\\start_meeting_push.png'"));
				vector<Individual*> all_clients = clients_manager->getAllClients();
				for (int i = 0; i < all_clients.size(); i++)
				{
					Individual* individual = static_cast<Individual*>(all_clients[i]);
					if (individual->getStatus() == USER_STATUS_INVITING_PENDING)
					{
						individual->setStatus(USER_STATUS_LOGIN);
					}
				}
				CommandManager::GetInstance()->cancelInvitaions();
				m_thread_map.clear();
			}


		}
		else if (msg.pSender->GetName() == L"group_btn")
		{
			CButtonUI* group_btn = static_cast<CButtonUI *>(msg.pSender);
			map<CButtonUI*, vector< CListContainerElementUI*>>::iterator iter = groupMap.find(group_btn);
			if (iter != groupMap.end())
			{
				vector< CListContainerElementUI*> list = iter->second;
				for (int j = 0; j < list.size(); j++)
				{
					if (list[j]->IsVisible())
					{
						list[j]->SetVisible(false);
						group_btn->SetBkImage(_T("file = 'res\\expand.png'"));
					}
					else
					{
						list[j]->SetVisible(true);
						group_btn->SetBkImage(_T("file = 'res\\collapse.png'"));
					}
				}
			}
		}
		else if (CCheckBoxUI * checkbox = dynamic_cast<CCheckBoxUI *>(msg.pSender))
		{
			//logInfo("Click checkbox...");
			map<CCheckBoxUI*, Individual*>::iterator it = checkMap.find(checkbox);
			if (it != checkMap.end())
			{
				Individual* client = it->second;
				if (!checkbox->GetCheck() && !clients_manager->isClientInvited(client))
				{
					logInfo("Checkbox is checked, client is " + client->getUserId());
					clients_manager->addInvitedClient(client);
				}
				else if (checkbox->GetCheck() && clients_manager->isClientInvited(client))
				{
					logInfo("Checkbox is unchecked, client is " + client->getUserId());
					clients_manager->removeClient(client);
				}
			}
			
		}

	}
	else if (msg.sType == L"itemclick")
	{
		//logInfo("Click ListItem...");
		CListContainerElementUI* pListItem = static_cast<CListContainerElementUI *>(msg.pSender);
		string user_id = StringUtil::WStringToString(pListItem->GetName().GetData());
		CCheckBoxUI *checkbox = static_cast<CCheckBoxUI *>(pListItem->FindSubControl(_T("checkbox")));
		if (checkbox->IsEnabled())
		{
			if (checkbox->GetCheck())
			{
				logInfo("Checkbox is unchecked...");
				checkbox->SetCheck(false);
			}
			else
			{
				logInfo("Checkbox is checked...");
				checkbox->SetCheck(true);
			}
			map<CCheckBoxUI*, Individual*>::iterator it = checkMap.find(checkbox);
			if (it != checkMap.end())
			{
				Individual* client = it->second;
				if (checkbox->GetCheck() && !clients_manager->isClientInvited(client))
				{
					logInfo("The checked client is " + client->getUserId());
					clients_manager->addInvitedClient(client);
				}
				else if (!checkbox->GetCheck() && clients_manager->isClientInvited(client))
				{
					logInfo("The unchecked client is " + client->getUserId());
					clients_manager->removeClient(client);
				}
			}
		}
	}
}

void LoggedInUI::DoStartMeetingBtnClick()
{
}

void LoggedInUI::ShowErrorMessage(const wchar_t* error_message)
{
	if (error_message)
		::MessageBox(NULL, error_message, L"error", MB_OK);
}

void LoggedInUI::switchToLoggedInUI()
{
	//m_btnStartMeeting->SetEnabled(true);
	clearSelectedClients();
}

void LoggedInUI::clearSelectedClients()
{
	map<CCheckBoxUI*, Individual*>::iterator it = checkMap.begin();
	while (it != checkMap.end())
	{
		CCheckBoxUI* checkbox = it->first;
		if (checkbox->GetCheck())
		{
			checkbox->SetCheck(false);
		}
		it++;
	}
	clients_manager->clearInvitedClients();
}

LPCTSTR LoggedInUI::string2LPCTSTR(string str)
{
	LPCTSTR  pDest = NULL;
	int length = sizeof(TCHAR)*(strlen(str.c_str()) + 1);
	LPTSTR tcBuffer = new TCHAR[length];
	memset(tcBuffer, 0, length);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), strlen(str.c_str()), tcBuffer, length);
	pDest = (LPCTSTR)tcBuffer;
	return pDest;
}

void LoggedInUI::onRequestHostClients()
{
	vector<Individual*> clients;
	BackendCommImpl::Instance()->requestHostClients(clients_manager, clients);
	handleHostClients(clients);
}
string LoggedInUI::getStatusDestString()
{
	int left = icon_size - status_size;
	int top = icon_size - status_size;
	int right = icon_size;
	int bottom = icon_size;
	return "dest='" + to_string(left) + "," + to_string(top) + "," + to_string(right) + "," + to_string(bottom) + "'";
}
void LoggedInUI::updateClientsStatus(string user_id, UserStatus status, string client_channel)
{
	logInfo("===========LoggedInUI::updateClientsStatus===========");
	map<string, Individual*> individualMap = clients_manager->getIndividualMap();
	map<string, Individual*>::iterator iter1 = individualMap.find(user_id);
	if (iter1 != individualMap.end())
	{
		Individual* individual = iter1->second;
		logInfo("Client " + individual->getUserId() + " report its status is " + StringUtil::getClientStatusName(status) + ", and its status in host is " + StringUtil::getClientStatusName(individual->getStatus()));
		map<Individual*, CListContainerElementUI*>::iterator iter2 = m_model_ui_map.find(individual);
		if (iter2 != m_model_ui_map.end())
		{
			CListContainerElementUI* listitem_ui = iter2->second;
			CButtonUI *statusBtn = static_cast<CButtonUI *>(listitem_ui->FindSubControl(_T("status_btn")));
			CCheckBoxUI *checkbox = static_cast<CCheckBoxUI *>(listitem_ui->FindSubControl(_T("checkbox")));
			if (status == USER_STATUS_LOGIN)
			{
				logInfo("New Status: USER_STATUS_LOGIN");
				if (individual->getStatus() == USER_STATUS_JOIN_MEETING)
				{
					logInfo("Old Stauts: USER_STATUS_JOIN_MEETING");
					if (channel == client_channel)
					{
						logInfo("Client is in the same channel with host.");
						string attribut = "file='res\\online.png' " + getStatusDestString();
						statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
						//statusBtn->SetNormalImage(_T("file='res\\online.png' dest='112,112,160,160'"));
						if (checkbox != NULL)
						{
							checkbox->SetEnabled(true);
						}
						return;
					}
					individual->setStatus(UserStatus::USER_STATUS_JOIN_MEETING);
					string attribut = "file='res\\busy.png' " + getStatusDestString();
					statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
					//statusBtn->SetNormalImage(_T("file='res\\busy.png' dest='112,112,160,160'"));
					if (checkbox != NULL)
					{
						checkbox->SetEnabled(false);
					}
				}
				else if (individual->getStatus() == USER_STATUS_INVITING_PENDING)
				{
					map<string, HANDLE>::iterator iter = m_thread_map.find(individual->getUserId());
					if (iter != m_thread_map.end())
					{
						logInfo("Client " + individual->getUserId() + " has been recovered from offline to online");
						HANDLE thread = iter->second;
						if (thread != NULL)   //线程正在运行
						{
							TerminateThread(thread, 0);
							if (thread != NULL)
							{
								CloseHandle(thread);   //关闭线程句柄，可能线程已结束，但句柄还没释放
								thread = NULL;
							}
							logInfo("Terminate the thread for " + iter->first);
						}
						m_thread_map.erase(iter);
					}
				}
				else
				{ 
					individual->setStatus(UserStatus::USER_STATUS_LOGIN);
					string attribut = "file='res\\online.png' " + getStatusDestString();
					statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
					//statusBtn->SetNormalImage(_T("file='res\\online.png' dest='112,112,160,160'"));
					if (checkbox != NULL)
					{
						checkbox->SetEnabled(true);
					}
					
				}
			}
			else if (status == USER_STATUS_LOGOUT)
			{
				logInfo("New Status: USER_STATUS_LOGOUT");
				if (individual->getStatus() == UserStatus::USER_STATUS_INVITING_PENDING && m_inCalling)
				{
					//checkbox->SetEnabled(false);
					map<string, HANDLE>::iterator iter = m_thread_map.find(individual->getUserId());
					if (iter == m_thread_map.end())
					{
						logInfo("Calling " + individual->getUserId() + " to start meeting, but now this client is offline, waiting 20s to see if he can recover.");
						PCLIENT_PARAM param = new CLIENT_PARAM;
						param->loggedInUI = this;
						param->user_id = individual->getUserId();
						HANDLE hThread = CreateThread(NULL, 0, threadTiming, param, 0, NULL);
						m_thread_map.insert(map<string, HANDLE>::value_type(individual->getUserId(), hThread));
					}
				}
				else
				{
					checkbox->SetEnabled(false);
					checkbox->SetCheck(false);
					individual->setStatus(UserStatus::USER_STATUS_LOGOUT);
					string attribut = "file='res\\offline.png' " + getStatusDestString();
					statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
				}

				////statusBtn->SetNormalImage(_T("file='res\\offline.png' dest='112,112,160,160'"));
				//if (checkbox != NULL)
				//{
				//	checkbox->SetEnabled(false);
				//	checkbox->SetCheck(false);
				//}
			}
			else if (status == USER_STATUS_JOIN_MEETING)
			{
				logInfo("New Status: USER_STATUS_JOIN_MEETING");
				individual->setStatus(UserStatus::USER_STATUS_JOIN_MEETING);
				if (channel == client_channel)
				{
					logInfo("Client is in the same channel with host.");
					string attribut = "file='res\\online.png' " + getStatusDestString();
					statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
					//statusBtn->SetNormalImage(_T("file='res\\online.png' dest='112,112,160,160'"));
					if (checkbox != NULL)
					{
						checkbox->SetEnabled(true);
					}
					if (!m_bInMeeting)
					{
						logInfo("Host has not started the meeting yet.");
						individual->setStatus(UserStatus::USER_STATUS_JOIN_MEETING_WAITING);
					}
					return;
				}
				string attribut = "file='res\\busy.png' " + getStatusDestString();
				statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
				//statusBtn->SetNormalImage(_T("file='res\\busy.png' dest='112,112,160,160'"));
				if (checkbox != NULL)
				{
					checkbox->SetEnabled(false);
					checkbox->SetCheck(false);
				}
			}
			else if (status == USER_STATUS_ENDMEETING)
			{
				individual->setStatus(UserStatus::USER_STATUS_LOGIN);
				string attribut = "file='res\\online.png' " + getStatusDestString();
				statusBtn->SetNormalImage(StringUtil::StringToWstring(attribut).c_str());
				//statusBtn->SetNormalImage(_T("file='res\\online.png' dest='112,112,160,160'"));
				if (checkbox != NULL)
				{
					checkbox->SetEnabled(true);
				}
			}
			else if (status == USER_STATUS_TIMEOUT)
			{
				logInfo("New Status: USER_STATUS_TIMEOUT");
				if (individual->getStatus() == USER_STATUS_INVITING_PENDING)
				{
					individual->setStatus(USER_STATUS_TIMEOUT);
					onInvitationRefuseOrTimeoutByClient(individual->getUserId(), false);
				}
			}
		}
	}
	doSort();
	map<CCheckBoxUI*, Individual*>::iterator it = checkMap.begin();
	while (it != checkMap.end())
	{
		CCheckBoxUI* checkbox = it->first;
		if (checkbox->IsEnabled())
		{
			m_btnStartMeeting->SetEnabled(true);
			return;
		}
		it++;
	}
	m_btnStartMeeting->SetEnabled(false);
	
	//m_btnStartMeeting->SetNormalImage(_T("file = 'res\\start_meeting_normal.png'"));
	//m_btnStartMeeting->SetHotImage(_T("file = 'res\\start_meeting_hot.png'"));
	//m_btnStartMeeting->SetPushedImage(_T("file = 'res\\start_meeting_push.png'"));
	//m_inCalling = false;
}

void LoggedInUI::removeThread(string user_id)
{
	map<string, HANDLE>::iterator iter = m_thread_map.find(user_id);
	if (iter != m_thread_map.end())
	{
		m_thread_map.erase(iter);
	}
}

//void LoggedInUI::teminalAllThreads()
//{
//	map<string, HANDLE>::iterator iter;
//	for (iter = m_thread_map.begin(); iter != m_thread_map.end(); iter++)
//	{
//		HANDLE thread = iter->second;
//		//DWORD dw = WaitForSingleObject(thread, 0);
//
//		if (thread != NULL)   //线程正在运行
//		{
//			TerminateThread(thread, 0);  
//			if (thread != NULL)
//			{
//				CloseHandle(thread);   //关闭线程句柄，可能线程已结束，但句柄还没释放
//				thread = NULL;
//			}
//			logInfo("Terminate the thread for " + iter->first);
//		}
//	}
//	m_thread_map.clear();
//}

void LoggedInUI::onInvitationRefuseOrTimeoutByClient(string client_id, bool refuse)
{
	CString tip = L"";
	if (refuse)
	{
		tip = L" refused the invitation.";
	}
	else
	{
		tip = L" was time out on the invitation";
	}
	//if (m_bInMeeting)
	//{
		map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.begin();
		while (iter != m_model_ui_map.end())
		{
			Individual* individual = static_cast<Individual*>(iter->first);
			if (individual->getUserId() == client_id)
			{
				wstring wname = individual->getWName();
				CString name = CString(wname.c_str());
				CString message = name + tip;
				DisplayTip(message);
				logInfo(cs2s(message));
				break;
			}
			iter++;
		}
	//}
	//else
	//{
		vector<Individual*> invitedClients = clients_manager->getInvitedClients();
		if (invitedClients.size() > 0)
		{
			int reject = 0;
			for (int k = 0; k < invitedClients.size(); k++)
			{

				Individual* individual = static_cast<Individual*>(invitedClients[k]);
				if (individual->getUserId() == client_id)
				{
					if (refuse)
						individual->setStatus(USER_STATUS_REJECT_MEETING);
					else
						individual->setStatus(USER_STATUS_TIMEOUT);
				}
				if (individual->getStatus() == USER_STATUS_REJECT_MEETING || individual->getStatus() == USER_STATUS_TIMEOUT)
				{
					reject++;
				}
			}
			if (reject == invitedClients.size())
			{
				DisplayTipWarning(_T("No one accept the meeting."));
				logInfo("Select " + to_string(invitedClients.size()) + " clients, no one accept the meeting.");
				m_btnStartMeeting->SetNormalImage(_T("file = 'res\\start_meeting_normal.png'"));
				m_btnStartMeeting->SetHotImage(_T("file = 'res\\start_meeting_hot.png'"));
				m_btnStartMeeting->SetPushedImage(_T("file = 'res\\start_meeting_push.png'"));
				m_inCalling = false;
				clearSelectedClients();
				m_thread_map.clear();
				//Re_get clients status
				map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.begin();
				logInfo("Send command to query all clients status.");
				while (iter != m_model_ui_map.end())
				{
					Individual* individual = static_cast<Individual*>(iter->first);
					CommandManager::GetInstance()->sendQueryClientStatus(atoi(individual->getUserId().c_str()));
					iter++;
				}
			}
		}
		else
		{
			logInfo("There are no invited clients, invite an available client to join meeting.");
			map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.begin();
			bool invite = false;
			while (iter != m_model_ui_map.end())
			{
				Individual* individual = static_cast<Individual*>(iter->first);				
				if (individual->getUserId() == client_id)
				{
					if (refuse)
						individual->setStatus(USER_STATUS_REJECT_MEETING);
					else
						individual->setStatus(USER_STATUS_TIMEOUT);
				}
				if (individual->getStatus() == UserStatus::USER_STATUS_LOGIN)
				{
					invite = true;
					logInfo("Invite " + individual->getUserId() + " to join meeting.");
					CommandManager::GetInstance()->Invite(atoi(individual->getUserId().c_str()), this->channel);
					individual->setStatus(UserStatus::USER_STATUS_INVITING_PENDING);
				}
				iter++;
			}
			if (!invite)
			{
				m_btnStartMeeting->SetNormalImage(_T("file = 'res\\start_meeting_normal.png'"));
				m_btnStartMeeting->SetHotImage(_T("file = 'res\\start_meeting_hot.png'"));
				m_btnStartMeeting->SetPushedImage(_T("file = 'res\\start_meeting_push.png'"));
				DisplayTipWarning(_T("No one accept the meeting."));
				logInfo("No available one accept the meeting.");
				m_inCalling = false;
				clearSelectedClients();
				m_thread_map.clear();
				//Re_get clients status
				map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.begin();
				while (iter != m_model_ui_map.end())
				{
					Individual* individual = static_cast<Individual*>(iter->first);
					CommandManager::GetInstance()->sendQueryClientStatus(atoi(individual->getUserId().c_str()));
					iter++;
				}
			}
		}
	//}
}

void LoggedInUI::onInvitationEnd()
{
	m_btnStartMeeting->SetNormalImage(_T("file = 'res\\start_meeting_normal.png'"));
	m_btnStartMeeting->SetHotImage(_T("file = 'res\\start_meeting_hot.png'"));
	m_btnStartMeeting->SetPushedImage(_T("file = 'res\\start_meeting_push.png'"));
	m_inCalling = false;
	//Re_get clients status
	map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.begin();
	while (iter != m_model_ui_map.end())
	{
		Individual* individual = static_cast<Individual*>(iter->first);
		CommandManager::GetInstance()->sendQueryClientStatus(atoi(individual->getUserId().c_str()));
		iter++;
	}
}


void LoggedInUI::setMeetingStatus(bool inMeeting)
{
	this->m_bInMeeting = inMeeting;
	if (m_bInMeeting)
	{
		m_inCalling = false;
	}
	m_btnStartMeeting->SetEnabled(true);
	m_btnStartMeeting->SetNormalImage(_T("file = 'res\\start_meeting_normal.png'"));
	m_btnStartMeeting->SetHotImage(_T("file = 'res\\start_meeting_hot.png'"));
	m_btnStartMeeting->SetPushedImage(_T("file = 'res\\start_meeting_push.png'"));
}

bool LoggedInUI::isInMeeting()
{
	return m_bInMeeting;
}

bool LoggedInUI::isInCalling()
{
	return m_inCalling;
}

wstring LoggedInUI::getDisplayName(string user_id)
{
	map<string, Individual*> individualMap = clients_manager->getIndividualMap();
	map<string, Individual*>::iterator iter1 = individualMap.find(user_id);
	wstring displayName = L"";
	if (iter1 != individualMap.end())
	{
		Individual* individual = iter1->second;
		displayName = individual->getWName();
	}
	return displayName;
}

DWORD WINAPI LoggedInUI::threadTiming(LPVOID lpParamter)
{
	if (!lpParamter)
		return 0;

	PCLIENT_PARAM param = (PCLIENT_PARAM)lpParamter;
	for (int i = 20; i >= 0; i--)
	{
		Sleep(1000);
	}

	if (param->loggedInUI != NULL && param->loggedInUI->isInCalling())
	{
		CommandManager::GetInstance()->cancelInvitation(param->user_id);
		param->loggedInUI->removeThread(param->user_id);
		param->loggedInUI->onInvitationRefuseOrTimeoutByClient(param->user_id, false);
	}
	return 0;
}

void LoggedInUI::doSort()
{
	sort(m_listElements.begin(), m_listElements.end(), compareWithStatusAndName);
	vector<CListContainerElementUI *>::iterator listElementIter;
	int index = 0;
	for (listElementIter = m_listElements.begin(); listElementIter != m_listElements.end(); listElementIter++) {
		CListContainerElementUI *pListElement = *listElementIter;
		m_clientslist->SetItemIndex(pListElement, index);
		index++;
	}
}

bool compareWithStatusAndName(CListContainerElementUI * item1, CListContainerElementUI * item2)
{
	CLabelUI *pLabelName1 = static_cast<CLabelUI *>(item1->FindSubControl(_T("name_label")));
	CCheckBoxUI *pCheckbox1 = static_cast<CCheckBoxUI *>(item1->FindSubControl(_T("checkbox")));
	CLabelUI *pLabelName2 = static_cast<CLabelUI *>(item2->FindSubControl(_T("name_label")));
	CCheckBoxUI *pCheckbox2 = static_cast<CCheckBoxUI *>(item2->FindSubControl(_T("checkbox")));
	if (pCheckbox1->IsEnabled() && !pCheckbox2->IsEnabled())
	{
		return true;
	}
	else if (!pCheckbox1->IsEnabled() && pCheckbox2->IsEnabled())
	{
		return false;
	}
	else
	{
		LPCTSTR strName1 = pLabelName1->GetText().GetData();
		LPCTSTR strName2 = pLabelName2->GetText().GetData();


		if (_tcscmp(strName1, strName2) < 0) 
			return true;
		else 
			return false;
	}

	
}