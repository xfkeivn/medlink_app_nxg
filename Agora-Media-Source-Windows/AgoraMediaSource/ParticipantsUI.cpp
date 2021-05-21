#include "stdafx.h"
#include "MessageProtocal.h"
#include "ParticipantsUI.h"

LPCTSTR string2LPCTSTR(string str)
{
	LPCTSTR  pDest = NULL;
	int length = sizeof(TCHAR)*(strlen(str.c_str()) + 1);
	LPTSTR tcBuffer = new TCHAR[length];
	memset(tcBuffer, 0, length);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), strlen(str.c_str()), tcBuffer, length);
	pDest = (LPCTSTR)tcBuffer;
	return pDest;
}

ParticipantsUI::ParticipantsUI(HWND hwnd)
{
	this->hwndParent = hwnd;
	m_clientsManager = ClientsManager::getInstance();
	m_bRemotingControling = false;
	m_twoWinsQueueForWinMode2 = new TwoWinsQueue();
	m_twoWinsQueueForWinMode4 = new TwoWinsQueue();
	m_twoWinsQueueForWinMode6 = new TwoWinsQueue();
	m_threeWinsQueueForWinMode7 = new ThreeWinsQueue();
}

UINT ParticipantsUI::GetSkinRes()
{
	//if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	//{
		return IDXML_HOST_PARTICIPANTS_UI;
	//}
	//else
	//{
	//	return IDXML_CLIENT_PARTICIPANTS_UI;
	//}
}

UINT ParticipantsUI::GetParticipantItemRes()
{
	//if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	//{
		return IDXML_HOST_PARTICIPANT_ITEM;
	//}
	//else
	//{
	//	return IDXML_CLIENT_PARTICIPANT_ITEM;
	//}
}
void ParticipantsUI::InitWindow()
{
	RECT rc = { 0 };
	if (!::GetClientRect(m_hWnd, &rc)) return;

	rc.right = rc.left + 600;// width;
	rc.bottom = rc.top + 600;// height;
	if (!::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd))) return;
	int y = 32;
	//if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	//{
		int bar_height = getResolutionHeight(MAX_BAR_HEIGHT);
		y = bar_height;
	//}
	::SetWindowPos(m_hWnd, NULL, 0, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);
	m_participantslist = static_cast<CListUI*>(m_PaintManager.FindControl(_T("participants")));
	m_participantslist->SetDelayedDestroy(false);
	CLabelUI* title_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title")));
	m_pip_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("pip_btn")));
	m_pbyp_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("pbyp_btn")));
	m_pup_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("pup_btn")));
	m_hd1_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hd1_btn")));
	m_hd2_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hd2_btn")));
	m_pip_control_container = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("pip_control")));
	m_video_2input_ctrl = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("video_2input_ctrl")));
	m_video_4input_ctrl = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("video_4input_ctrl")));
	m_winmode1_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("winmode1_btn")));
	m_winmode2_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("winmode2_btn")));
	m_winmode4_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("winmode4_btn")));
	m_winmode6_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("winmode6_btn")));
	m_winmode7_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("winmode7_btn")));
	m_winmode8_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("winmode8_btn")));
	m_hdm1_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hdmi1")));
	m_hdm2_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hdmi2")));
	m_hdm3_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hdmi3")));
	m_hdm4_btn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hdmi4")));

	//if (CAgoraObject::GetAgoraObject()->GetSelfHost())
	//{
		int size = getResolutionFontSize(MAX_TITLE_SIZE);
		title_label->SetFont(size);
		CScrollBarUI* pListVerticalScrollBar = m_participantslist->GetVerticalScrollBar();
		int scrollbar_width = getResolutionWidth(MAX_CLIENTS_SCROLLBAR_WIDTH);
		//pListVerticalScrollBar->SetAttribute(_T("width"), _T("48"));
		pListVerticalScrollBar->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(scrollbar_width)).c_str());
	//}
		pictureDivider = CAGConfig::GetInstance()->GetPictureDivider();
		if (pictureDivider == 2)
		{
			m_video_2input_ctrl->SetVisible(true);
		}
		else if (pictureDivider == 4)
		{
			m_video_4input_ctrl->SetVisible(true);
		}
		
}

void ParticipantsUI::Notify(TNotifyUI& msg)
{
	//logInfo("msg.sType=" + StringUtil::StringFromLPCTSTR(msg.sType.GetData()) + ",msg.pSender=" + StringUtil::StringFromLPCTSTR(msg.pSender->GetName().GetData()));
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == L"remote_ctrl_btn")
		{
			BOOL isRCEnable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
			CString equipment = readRegKey(EQUIPMENT_NAME, APP_REG_DIR);
			if (isRCEnable && equipment.MakeUpper() != _T("SPYGLASS"))
			{
				if (CAGConfig::GetInstance()->GetDeviceCom().IsEmpty())
				{
					::MessageBox(NULL, L"Device COM doesn't set properly!", L"Error", MB_OK);
					return;
				}
			}
			m_bRemotingControling = !m_bRemotingControling;
			CButtonUI* remote_control_btn = static_cast<CButtonUI *>(msg.pSender);
			map<CButtonUI*, Individual*>::iterator it = remoteMap.find(remote_control_btn);
			if (it != remoteMap.end())
			{
				Individual* participant = it->second;
				if(CAgoraObject::GetAgoraObject()->GetSelfHost())
				{ 
					if(m_bRemotingControling)
					{ 
						logInfo("Grant user " + participant->getUserId() + " to have remote control.");
						CommandManager::GetInstance()->GrantRemoteControl(atoi(participant->getUserId().c_str()));
						if (pictureDivider == 2)
						{
							m_video_2input_ctrl->SetVisible(false);
						}						
					}
					else
					{
						logInfo("Take back remote control form user " + participant->getUserId());
						CommandManager::GetInstance()->TakeBackRemoteControl(atoi(participant->getUserId().c_str()));
						if (pictureDivider == 2)
						{
							m_video_2input_ctrl->SetVisible(true);
						}						
					}
				}
				else
				{
					logInfo("Client " + participant->getUserId() + " give back remote control.");
					::SendMessage(hwndParent, WM_GIVEBACK_REMOTE_CONTROL, 0, 0);
					if (pictureDivider == 2)
					{
						m_video_2input_ctrl->SetVisible(true);
					}
				}
			}
			 
		}
		else if (msg.pSender->GetName() == L"swap_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send swap cmd to serial port, and broadcast the swap cmd.");
				selectVideoMode(VIDEO_PIP_SWAP, true, true);
			}
			else
			{
				logInfo("Only broadcast swap cmd in the channel.");
				selectVideoMode(VIDEO_PIP_SWAP, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"pic_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send pic cmd to serial port, and broadcast the pic cmd.");
				selectVideoMode(VIDEO_PIP_PIC, true, true);
			}
			else
			{
				logInfo("Only broadcast pic cmd in the channel.");
				selectVideoMode(VIDEO_PIP_PIC, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"rotate_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send rotate cmd to serial port, and broadcast the rotate cmd.");
				selectVideoMode(VIDEO_PIP_ROTATE, true, true);
			}
			else
			{
				logInfo("Only broadcast rotate cmd in the channel.");
				selectVideoMode(VIDEO_PIP_ROTATE, false, true);
			}
		}
	}
	else if (msg.sType == _T("selectchanged"))
	{
		if (msg.pSender->GetName() == L"pip_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send pip cmd to serial port, and broadcast the pip cmd.");
				selectVideoMode(VIDEO_PIP_MODE, true, true);
			}
			else
			{
				logInfo("Only broadcast pip cmd in the channel.");
				selectVideoMode(VIDEO_PIP_MODE, false, true);
			}
			
		}
		else if (msg.pSender->GetName() == L"pbyp_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send pbyp cmd to serial port, and broadcast the pbyp cmd.");
				selectVideoMode(VIDEO_PBYP_MODE, true, true);
			}
			else
			{
				logInfo("Only broadcast pbyp cmd in the channel.");
				selectVideoMode(VIDEO_PBYP_MODE, false, true);
			}
			
		}
		else if (msg.pSender->GetName() == L"pup_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send pup cmd to serial port, and broadcast the pup cmd.");
				selectVideoMode(VIDEO_PUP_MODE, true, true);
			}
			else
			{
				logInfo("Only broadcast pup cmd in the channel.");
				selectVideoMode(VIDEO_PUP_MODE, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"hd1_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send hd1 cmd to serial port, and broadcast the hd1 cmd.");
				selectVideoMode(VIDEO_HD1_MODE, true, true);
			}
			else
			{
				logInfo("Only broadcast hd1 cmd in the channel.");
				selectVideoMode(VIDEO_HD1_MODE, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"hd2_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send hd2 cmd to serial port, and broadcast the hd2 cmd.");
				selectVideoMode(VIDEO_HD2_MODE, true, true);
			}
			else
			{
				logInfo("Only broadcast hd2 cmd in the channel.");
				selectVideoMode(VIDEO_HD2_MODE, false, true);
			}

		}
		else if (msg.pSender->GetName() == L"winmode1_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_WINMODE1 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_WINMODE1, true, true);
			}
			else {
				logInfo("Only broadcast VIDEO_WINMODE1 cmd in the channel.");
				selectVideoMode(VIDEO_WINMODE1, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"winmode2_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_WINMODE2 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_WINMODE2, true, true);
			}
			else 
			{
				logInfo("Only broadcast VIDEO_WINMODE2 cmd in the channel.");
				selectVideoMode(VIDEO_WINMODE2, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"winmode4_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_WINMODE4 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_WINMODE4, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_WINMODE4 cmd in the channel.");
				selectVideoMode(VIDEO_WINMODE4, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"winmode6_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_WINMODE6 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_WINMODE6, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_WINMODE6 cmd in the channel.");
				selectVideoMode(VIDEO_WINMODE6, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"winmode7_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_WINMODE7 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_WINMODE7, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_WINMODE7 cmd in the channel.");
				selectVideoMode(VIDEO_WINMODE7, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"winmode8_btn")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_WINMODE8 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_WINMODE8, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_WINMODE8 cmd in the channel.");
				selectVideoMode(VIDEO_WINMODE8, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"hdmi1")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_HDMI1 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_HDMI1, true, true);
			}
			else 
			{
				logInfo("Only broadcast VIDEO_HDMI1 cmd in the channel.");
				selectVideoMode(VIDEO_HDMI1, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"hdmi2")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_HDMI2 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_HDMI2, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_HDMI2 cmd in the channel.");
				selectVideoMode(VIDEO_HDMI2, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"hdmi3")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_HDMI3 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_HDMI3, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_HDMI3 cmd in the channel.");
				selectVideoMode(VIDEO_HDMI3, false, true);
			}
		}
		else if (msg.pSender->GetName() == L"hdmi4")
		{
			if (CAgoraObject::GetAgoraObject()->GetSelfHost())
			{
				logInfo("Host send VIDEO_HDMI4 cmd to serial port, and broadcast it.");
				selectVideoMode(VIDEO_HDMI4, true, true);
			}
			else
			{
				logInfo("Only broadcast VIDEO_HDMI4 cmd in the channel.");
				selectVideoMode(VIDEO_HDMI4, false, true);
			}
		}
	}
}

LRESULT ParticipantsUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);

}

void ParticipantsUI::onUserListUpdate(UINT *uids, UINT count)
{
	m_participantslist->RemoveAll();
	remoteMap.clear();
	m_model_ui_map.clear();
	vector<Individual*> participants = m_clientsManager->getParticipants(uids, count);
	for (int i = 0; i < count; i++)
	{
		CListContainerElementUI* pListItem;
		CDialogBuilder builder;
		STRINGorID individual_xml(GetParticipantItemRes()); 
		pListItem = static_cast<CListContainerElementUI *>(builder.Create(individual_xml, _T("xml"), 0, &m_PaintManager));
		CLabelUI *pNameLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("nickname")));
		CLabelUI *pAdditionalLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("additional")));
		CButtonUI *pImageBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("image_btn")));
		CButtonUI *pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
		if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			pRemoteBtn->SetVisible(false);
		}
		else
		{
			string hostuid = to_string(CAgoraObject::GetAgoraObject()->GetSelfUID());
			BOOL isRCEnable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
			CString equipment = readRegKey(EQUIPMENT_NAME, APP_REG_DIR);
			if (hostuid == participants[i]->getUserId() || !isRCEnable || equipment.MakeUpper() == _T("SPYGLASS"))
			{
				pRemoteBtn->SetVisible(false);
			}
		}

		//else
		//{
			CControlUI* item_left = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_left")));
			CControlUI* item_space1 = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_space1")));
			CControlUI* item_space2 = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_space2")));
			CVerticalLayoutUI* item_image_container = static_cast<CVerticalLayoutUI*>(pListItem->FindSubControl(_T("item_image")));
			CVerticalLayoutUI* item_remote_container = static_cast<CVerticalLayoutUI*>(pListItem->FindSubControl(_T("item_remote")));

			int listitem_height = getResolutionHeight(MAX_PARTICIPANTS_LISTITEM_HEIGHT);
			pListItem->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(listitem_height)).c_str());

			int space = getResolutionWidth(MAX_ITEM_SPACE);
			item_left->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());
			item_space1->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());
			item_space2->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());

			int image_container_size = getResolutionSize(MAX_PARTICIPANTS_IMAGE_SIZE);
			item_image_container->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(image_container_size)).c_str());
			int remote_container_size = getResolutionSize(MAX_PARTICIPANTS_REMOTE_SIZE);
			item_remote_container->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(remote_container_size)).c_str());
			pImageBtn->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(image_container_size)).c_str());
			pImageBtn->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(image_container_size)).c_str());
			pRemoteBtn->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(remote_container_size)).c_str());
			pRemoteBtn->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(remote_container_size)).c_str());

			int size = getResolutionFontSize(MEDIUM_TITLE_SIZE);
			pNameLab->SetFont(size);
			pAdditionalLab->SetFont(size);


		//}
		pNameLab->SetText(participants[i]->getWName().c_str());
		int id = atoi(participants[i]->getUserId().c_str());
		if (id >= 1000)
		{
			pAdditionalLab->SetText(string2LPCTSTR(participants[i]->getAdditional()));
		}
		else
		{
			pAdditionalLab->SetText(string2LPCTSTR(participants[i]->getTelephone()));
		}

		if (participants[i]->getImagePath() != "")
		{
			string s = "file='" + participants[i]->getImagePath() + "'";
			pImageBtn->SetBkImage(string2LPCTSTR(s));
		}	
		m_participantslist->Add(pListItem);
		m_model_ui_map.insert(map<Individual*, CListContainerElementUI*>::value_type(participants[i], pListItem));
		remoteMap.insert(map<CButtonUI*, Individual*>::value_type(pRemoteBtn, participants[i]));

		if (participants[i]->getWName() == L"")
		{
			onRequestParticipantInfo(participants[i]->getUserId());
		}

	}
}

Individual* ParticipantsUI::onUserJoined(UINT uid)
{
	vector<Individual*> participants = m_clientsManager->getAllParticipants();
	string user_id = to_string(uid);
	CAgoraObject* agoraObject = CAgoraObject::GetAgoraObject();
	if (!agoraObject->GetSelfHost())
	{
		if (uid >= 1000)
		{
			logInfo("Host id is " + user_id);
			agoraObject->setHostId(user_id);
		}
	}
	for (int i = 0; i < participants.size(); i++)
	{		
		if (participants[i]->getUserId() == user_id)
		{
			if (participants[i]->getStatus() == USER_STAUTS_RECONNECTING)
			{
				map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(participants[i]);
				if (iter != m_model_ui_map.end())
				{
					CListContainerElementUI* pListItem = iter->second;
					pListItem->SetEnabled(true);
					CButtonUI* pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
					if (pRemoteBtn != NULL)
					{
						pRemoteBtn->SetEnabled(true);
					}
					CLabelUI *pNameLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("nickname")));
					pNameLab->SetText(participants[i]->getWName().c_str());
					participants[i]->setStatus(USER_STATUS_RECONNECTED_SUCCESSFULLY);
				}
			}
			return participants[i];
		}
	}
	Individual* new_user = NULL;
	map<string, Individual*> individualMap = m_clientsManager->getIndividualMap();
	if (individualMap.size() > 0)
	{
		map<string, Individual*>::iterator iter1 = individualMap.find(user_id);
		if (iter1 != individualMap.end())
		{
			//participant is in the clients list
			Individual* individual = iter1->second;
			new_user = new Individual(individual->getWName(), individual->getUserId(), individual->getTelephone());
			new_user->setImagePath(individual->getImagePath().c_str());
		}
	}
	else
	{
		new_user = new Individual(L"", user_id, "");
	}
	new_user->setStatus(USER_STATUS_JOIN_MEETING);
	m_clientsManager->addParticipant(new_user);

	CListContainerElementUI* pListItem;
	CDialogBuilder builder;
	STRINGorID individual_xml(GetParticipantItemRes());
	pListItem = static_cast<CListContainerElementUI *>(builder.Create(individual_xml, _T("xml"), 0, &m_PaintManager));
	CLabelUI *pNameLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("nickname")));
	CLabelUI *pAdditionalLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("additional")));
	CButtonUI *pImageBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("image_btn")));
	CButtonUI *pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
	if (!CAgoraObject::GetAgoraObject()->GetSelfHost())
	{
		pRemoteBtn->SetVisible(false);
	}
	else
	{
		BOOL isRCEnable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
		CString equipment = readRegKey(EQUIPMENT_NAME, APP_REG_DIR);
		if (CAgoraObject::GetAgoraObject()->GetSelfUID() == uid || !isRCEnable || equipment.MakeUpper() == _T("SPYGLASS"))
		{
			pRemoteBtn->SetVisible(false);
		}
	}
	//else
	//{
		if (m_bRemotingControling)
		{
			string s = "file='res\\remote_normal_disable.png'";
			pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
			pRemoteBtn->SetEnabled(false);
		}
		CControlUI* item_left = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_left")));
		CControlUI* item_space1 = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_space1")));
		CControlUI* item_space2 = static_cast<CControlUI*>(pListItem->FindSubControl(_T("item_space2")));
		CVerticalLayoutUI* item_image_container = static_cast<CVerticalLayoutUI*>(pListItem->FindSubControl(_T("item_image")));
		CVerticalLayoutUI* item_remote_container = static_cast<CVerticalLayoutUI*>(pListItem->FindSubControl(_T("item_remote")));

		int listitem_height = getResolutionHeight(MAX_PARTICIPANTS_LISTITEM_HEIGHT);
		pListItem->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(listitem_height)).c_str());

		int space = getResolutionWidth(MAX_ITEM_SPACE);
		item_left->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());
		item_space1->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());
		item_space2->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(space)).c_str());

		int image_container_size = getResolutionSize(MAX_PARTICIPANTS_IMAGE_SIZE);
		item_image_container->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(image_container_size)).c_str());
		int remote_container_size = getResolutionSize(MAX_PARTICIPANTS_REMOTE_SIZE);
		item_remote_container->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(remote_container_size)).c_str());

		pImageBtn->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(image_container_size)).c_str());
		pImageBtn->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(image_container_size)).c_str());
		pRemoteBtn->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(remote_container_size)).c_str());
		pRemoteBtn->SetAttribute(_T("height"), StringUtil::StringToWstring(to_string(remote_container_size)).c_str());

		int size = getResolutionFontSize(MEDIUM_TITLE_SIZE);
		pNameLab->SetFont(size);
		pAdditionalLab->SetFont(size);


	//}
	pNameLab->SetText(new_user->getWName().c_str());
	if (uid >= 1000)
	{
		pAdditionalLab->SetText(string2LPCTSTR(new_user->getAdditional()));
	}
	else
	{
		pAdditionalLab->SetText(string2LPCTSTR(new_user->getTelephone()));
	}

	if (new_user->getImagePath() != "")
	{
		string s = "file='" + new_user->getImagePath() + "'";
		pImageBtn->SetBkImage(string2LPCTSTR(s));
	}
	m_participantslist->Add(pListItem);
	m_model_ui_map.insert(map<Individual*, CListContainerElementUI*>::value_type(new_user, pListItem));
	remoteMap.insert(map<CButtonUI*, Individual*>::value_type(pRemoteBtn, new_user));
	if (new_user->getWName() == L"")
	{
		onRequestParticipantInfo(new_user->getUserId());
	}
	return new_user;
}

wstring ParticipantsUI::onUserLeave(UINT uid)
{
	vector<Individual*> participants = m_clientsManager->getAllParticipants();
	string user_id = to_string(uid);
	for (int i = 0; i < participants.size(); i++)
	{
		if (participants[i]->getUserId() == user_id)
		{
			m_clientsManager->removeParticipant(participants[i]);
			map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(participants[i]);
			if (iter != m_model_ui_map.end())
			{
				CListContainerElementUI* listitem = iter->second;
				m_participantslist->Remove(listitem);
				m_model_ui_map.erase(iter);
			}
			return participants[i]->getWName();
		}
	}
	return L"";
}

void ParticipantsUI::onRequestParticipantInfo(string uid)
{
	//CString ip = CAGConfig::GetInstance()->GetWebServerIP();
	//CString port = CAGConfig::GetInstance()->GetWebServerPort();
	CString ip = readRegKey(WEBSERVERIP, APP_REG_DIR);
	CString port = readRegKey(WEBSERVERPORT, APP_REG_DIR);
	string ip_str = CT2A(ip.GetBuffer());
	if (port.GetLength() > 0)
	{
		CString c_ip_str = ip + ":" + port;
		ip_str = CT2A(c_ip_str);
	}
	string url = "";

	int id = atoi(uid.c_str());
	if (id >= 1000)
	{
		url = "http://" + ip_str + "/api-meeting/getHost/" + uid;
	}
	else
	{
		url = "http://" + ip_str + "/api-meeting/getClient/" + uid;
	}
	HttpClient::SendReq(url, NULL, handleParticipantInfo, this);
}

void ParticipantsUI::handleParticipantInfo(string rsp, void * pParam)
{
	ParticipantsUI *participantsUI = (ParticipantsUI *)pParam;
	participantsUI->updateParticipant(rsp);
}

void ParticipantsUI::updateParticipant(string rsp)
{
	Individual* participant = m_clientsManager->updateParticipant(rsp);
	if (participant != NULL)
	{
		map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(participant);
		if (iter != m_model_ui_map.end())
		{
			CListContainerElementUI* pListItem = iter->second;
			CLabelUI *pNameLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("nickname")));
			CLabelUI *pAdditionalLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("additional")));
			CButtonUI *pImageBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("image_btn")));
			pNameLab->SetText(participant->getWName().c_str());
			int id = atoi(participant->getUserId().c_str());
			if (id >= 1000)
			{
				pAdditionalLab->SetText(string2LPCTSTR(participant->getAdditional()));
			}
			else
			{
				pAdditionalLab->SetText(string2LPCTSTR(participant->getTelephone()));
			}
			if (participant->getImagePath() != "")
			{
				string s = "file='" + participant->getImagePath() + "'";
				pImageBtn->SetBkImage(string2LPCTSTR(s));
			}
			::SendMessage(hwndParent, WM_SHOW_PARTICIPANT, stoi(participant->getUserId()), 0);
		}
	}
}

void ParticipantsUI::updateUIForRemoteControl(UINT control_user, bool control)
{
	if (pictureDivider == 2)
	{
		m_video_2input_ctrl->SetVisible(!control);
	}
	m_bRemotingControling = control;
	vector<Individual*> all_participants = m_clientsManager->getAllParticipants();	
	for (int i = 0; i < all_participants.size(); i++)
	{
		map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(all_participants[i]);
		CButtonUI *pRemoteBtn = NULL;
		if (iter != m_model_ui_map.end())
		{
			CListContainerElementUI* pListItem = iter->second;
			pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
		}
		if(pRemoteBtn != NULL)
		{ 
			if(control)
			{ 
				string control_user_id = to_string(control_user);
				if (all_participants[i]->getUserId() == control_user_id)
				{
					pRemoteBtn->SetVisible(true);
					if (CAgoraObject::GetAgoraObject()->GetSelfHost())
					{
						string s = "file='res\\self_in_control.png'";
						pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
						pRemoteBtn->SetEnabled(true);
					}
					else
					{
						if (CAgoraObject::GetAgoraObject()->GetSelfUID() == control_user)
						{
							string s = "file='res\\self_in_control.png'";
							pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
							pRemoteBtn->SetEnabled(true);
						}
						else
						{
							string s = "file='res\\other_in_control.png'";
							pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
							pRemoteBtn->SetEnabled(false);
						}
						
					}
					
					
				}
				else
				{
					if(CAgoraObject::GetAgoraObject()->GetSelfHost())
					{ 
						string s = "file='res\\remote_normal_disable.png'";
						pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
						pRemoteBtn->SetEnabled(false);
					}
					else
					{
						pRemoteBtn->SetVisible(false);
					}
				}
			}
			else
			{
				if (CAgoraObject::GetAgoraObject()->GetSelfHost())
				{
					string s = "file='res\\remote_normal.png'";
					pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
					pRemoteBtn->SetEnabled(true);
				}
				else
				{
					pRemoteBtn->SetVisible(false);
				}
			}
		}
	}
}

void ParticipantsUI::setRemoteControlling(bool isRemoteControlling)
{
	this->m_bRemotingControling = isRemoteControlling;
	if (pictureDivider == 2)
	{
		m_video_2input_ctrl->SetVisible(!isRemoteControlling);
	}
}

void ParticipantsUI::updateUIForAnnotation(bool startAnnotation)
{
	vector<Individual*> all_participants = m_clientsManager->getAllParticipants();
	for (int i = 0; i < all_participants.size(); i++)
	{
		map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(all_participants[i]);
		CButtonUI *pRemoteBtn = NULL;
		if (iter != m_model_ui_map.end())
		{
			CListContainerElementUI* pListItem = iter->second;
			pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
		}
		if (pRemoteBtn != NULL)
		{
			if (startAnnotation)
			{
				string s = "file='res\\remote_normal_disable.png'";
				pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
				pRemoteBtn->SetEnabled(false);
			}
			else
			{
				string s = "file='res\\remote_normal.png'";
				pRemoteBtn->SetNormalImage(string2LPCTSTR(s));
				pRemoteBtn->SetEnabled(true);
			}
		}
	}
}

wstring ParticipantsUI::updateUIForDropOffline(UINT uid)
{
	vector<Individual*> all_participants = m_clientsManager->getAllParticipants();
	for (int i = 0; i < all_participants.size(); i++)
	{
		map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(all_participants[i]);
		CButtonUI *pRemoteBtn = NULL;
		if (iter != m_model_ui_map.end())
		{
			CListContainerElementUI* pListItem = iter->second;
			pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
			CLabelUI *pNameLab = static_cast<CLabelUI *>(pListItem->FindSubControl(_T("nickname")));
			string control_user_id = to_string(uid);
			if (all_participants[i]->getUserId() == control_user_id)
			{
				pListItem->SetEnabled(false);
				if (pRemoteBtn != NULL)
				{
					pRemoteBtn->SetEnabled(false);
				}
				wstring name = all_participants[i]->getWName();
				string reconnecting = "(Reconnecting)";
				name = name + StringUtil::StringToWstring(reconnecting);
				pNameLab->SetText(name.c_str());
				all_participants[i]->setStatus(USER_STAUTS_RECONNECTING);
				return all_participants[i]->getWName();
			}
		}				
	}
	return L"";
}

Individual* ParticipantsUI::getParticipant(UINT uid)
{
	vector<Individual*> participants = m_clientsManager->getAllParticipants();
	string user_id = to_string(uid);
	for (int i = 0; i < participants.size(); i++)
	{
		if (participants[i]->getUserId() == user_id)
		{
			return participants[i];
		}
	}
	return NULL;
}

void ParticipantsUI::selectVideoMode(MSG_EVENT_TYPE type, bool trigger, bool broadcast)
{
	int win;
	switch (type)
	{
	case VIDEO_HD1_MODE:
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			enableRemoteControl(true);
		}
		selectedVideoMode = type;
		m_hd1_btn->SetNormalImage(string2LPCTSTR("file='res\\hd1_push.png'"));
		m_hd2_btn->SetNormalImage(string2LPCTSTR("file='res\\hd2_normal.png'"));
		m_pip_btn->SetNormalImage(string2LPCTSTR("file='res\\pip_normal.png'"));
		m_pbyp_btn->SetNormalImage(string2LPCTSTR("file='res\\pbyp_normal.png'"));
		m_pup_btn->SetNormalImage(string2LPCTSTR("file='res\\pup_normal.png'"));
		m_pip_control_container->SetVisible(false);
		if (trigger)
		{
			logInfo("Execute hd1 operation.");
			VideoScreenControl::GetInstance()->sendHD1Op();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoHD1ModeCMD();
		}
		break;
	case VIDEO_HD2_MODE:
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			enableRemoteControl(false);
		}
		selectedVideoMode = type;
		m_hd1_btn->SetNormalImage(string2LPCTSTR("file='res\\hd1_normal.png'"));
		m_hd2_btn->SetNormalImage(string2LPCTSTR("file='res\\hd2_push.png'"));
		m_pip_btn->SetNormalImage(string2LPCTSTR("file='res\\pip_normal.png'"));
		m_pbyp_btn->SetNormalImage(string2LPCTSTR("file='res\\pbyp_normal.png'"));
		m_pup_btn->SetNormalImage(string2LPCTSTR("file='res\\pup_normal.png'"));
		m_pip_control_container->SetVisible(false);
		if (trigger)
		{
			logInfo("Execute hd2 operation.");
			VideoScreenControl::GetInstance()->sendHD2Op();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoHD2ModeCMD();
		}
		break;
	case VIDEO_PIP_MODE:
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			enableRemoteControl(false);
		}
		selectedVideoMode = type;
		m_hd1_btn->SetNormalImage(string2LPCTSTR("file='res\\hd1_normal.png'"));
		m_hd2_btn->SetNormalImage(string2LPCTSTR("file='res\\hd2_normal.png'"));
		m_pip_btn->SetNormalImage(string2LPCTSTR("file='res\\pip_push.png'"));
		m_pbyp_btn->SetNormalImage(string2LPCTSTR("file='res\\pbyp_normal.png'"));
		m_pup_btn->SetNormalImage(string2LPCTSTR("file='res\\pup_normal.png'"));
		m_pip_control_container->SetVisible(true);
		if (trigger)
		{
			logInfo("Execute Pip operation.");
			VideoScreenControl::GetInstance()->sendPipModeOp();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoPIPModeCMD();
		}
		break;
	case VIDEO_PBYP_MODE:
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			enableRemoteControl(false);
		}
		selectedVideoMode = type;
		m_hd1_btn->SetNormalImage(string2LPCTSTR("file='res\\hd1_normal.png'"));
		m_hd2_btn->SetNormalImage(string2LPCTSTR("file='res\\hd2_normal.png'"));
		m_pip_btn->SetNormalImage(string2LPCTSTR("file='res\\pip_normal.png'"));
		m_pbyp_btn->SetNormalImage(string2LPCTSTR("file='res\\pbyp_push.png'"));
		m_pup_btn->SetNormalImage(string2LPCTSTR("file='res\\pup_normal.png'"));
		m_pip_control_container->SetVisible(false);
		if (trigger)
		{
			logInfo("Execute Pbyp operation.");
			VideoScreenControl::GetInstance()->sendPbypModeOp();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoPBYPModeCMD();
		}
		break;
	case VIDEO_PUP_MODE:
		if (CAgoraObject::GetAgoraObject()->GetSelfHost())
		{
			enableRemoteControl(false);
		}
		selectedVideoMode = type;
		m_hd1_btn->SetNormalImage(string2LPCTSTR("file='res\\hd1_normal.png'"));
		m_hd2_btn->SetNormalImage(string2LPCTSTR("file='res\\hd2_normal.png'"));
		m_pip_btn->SetNormalImage(string2LPCTSTR("file='res\\pip_normal.png'"));
		m_pbyp_btn->SetNormalImage(string2LPCTSTR("file='res\\pbyp_normal.png'"));
		m_pup_btn->SetNormalImage(string2LPCTSTR("file='res\\pup_push.png'"));
		m_pip_control_container->SetVisible(false);
		if (trigger)
		{
			logInfo("Execute pup operation.");
			VideoScreenControl::GetInstance()->sendPupModeOp();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoPUPModeCMD();
		}
		break;
	case VIDEO_PIP_SWAP:
		if (trigger)
		{
			logInfo("Execute swap operation.");
			VideoScreenControl::GetInstance()->sendSwapPortOp();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoPIPSwapModeCMD();
		}
		break;
	case VIDEO_PIP_PIC:
		if (trigger)
		{
			logInfo("Execute Pic operation.");
			VideoScreenControl::GetInstance()->sendPicSizeOp();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoPIPPicModeCMD();
		}
		break;
	case VIDEO_PIP_ROTATE:
		if (trigger)
		{
			logInfo("Execute Rotate operation.");
			VideoScreenControl::GetInstance()->sendRotateOp();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoPIPRotateModeCMD();
		}
		break;
	case VIDEO_WINMODE1:
		selectedWinMode = type;
		m_winmode1_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode1_push.png'"));
		m_winmode2_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode2_normal.png'"));
		m_winmode4_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode4_normal.png'"));
		m_winmode6_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode6_normal.png'"));
		m_winmode7_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode7_normal.png'"));
		m_winmode8_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode8_normal.png'"));
		activeHDMIRadios();
		if (trigger)
		{
			logInfo("Execute Winmode1 operation.");
			Sleep(200);
			VideoScreenControl::GetInstance()->sendWinMode1Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI11Op();			
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoWinMode1CMD();
		}
		break;
	case VIDEO_WINMODE2:
		selectedWinMode = type;
		m_winmode1_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode1_normal.png'"));
		m_winmode2_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode2_push.png'"));
		m_winmode4_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode4_normal.png'"));
		m_winmode6_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode6_normal.png'"));
		m_winmode7_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode7_normal.png'"));
		m_winmode8_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode8_normal.png'"));
		activeHDMIRadios();
		if (trigger)
		{
			logInfo("Execute Winmode2 operation.");
			Sleep(200);
			VideoScreenControl::GetInstance()->sendWinMode2Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI11Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI22Op();
			m_twoWinsQueueForWinMode2->reset();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoWinMode2CMD();
		}
		break;
	case VIDEO_WINMODE4:
		selectedWinMode = type;
		m_winmode1_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode1_normal.png'"));
		m_winmode2_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode2_normal.png'"));
		m_winmode4_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode4_push.png'"));
		m_winmode6_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode6_normal.png'"));
		m_winmode7_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode7_normal.png'"));
		m_winmode8_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode8_normal.png'"));
		activeHDMIRadios();
		if (trigger)
		{
			logInfo("Execute Winmode4 operation.");
			Sleep(200);
			VideoScreenControl::GetInstance()->sendWinMode4Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI11Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI22Op();
			m_twoWinsQueueForWinMode4->reset();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoWinMode4CMD();
		}
		break;
	case VIDEO_WINMODE6:
		selectedWinMode = type;
		m_winmode1_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode1_normal.png'"));
		m_winmode2_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode2_normal.png'"));
		m_winmode4_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode4_normal.png'"));
		m_winmode6_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode6_push.png'"));
		m_winmode7_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode7_normal.png'"));
		m_winmode8_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode8_normal.png'"));
		activeHDMIRadios();
		if (trigger)
		{
			logInfo("Execute Winmode6 operation.");
			Sleep(200);
			VideoScreenControl::GetInstance()->sendWinMode6Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI11Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI22Op();
			m_twoWinsQueueForWinMode6->reset();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoWinMode6CMD();
		}
		break;
	case VIDEO_WINMODE7:
		selectedWinMode = type;
		m_winmode1_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode1_normal.png'"));
		m_winmode2_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode2_normal.png'"));
		m_winmode4_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode4_normal.png'"));
		m_winmode6_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode6_normal.png'"));
		m_winmode7_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode7_push.png'"));
		m_winmode8_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode8_normal.png'"));
		activeHDMIRadios();
		if (trigger)
		{
			logInfo("Execute Winmode7 operation.");
			Sleep(200);
			VideoScreenControl::GetInstance()->sendWinMode7Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI11Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI22Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI33Op();
			m_threeWinsQueueForWinMode7->reset();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoWinMode7CMD();
		}
		break;
	case VIDEO_WINMODE8:
		selectedWinMode = type;
		m_winmode1_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode1_normal.png'"));
		m_winmode2_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode2_normal.png'"));
		m_winmode4_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode4_normal.png'"));
		m_winmode6_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode6_normal.png'"));
		m_winmode7_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode7_normal.png'"));
		m_winmode8_btn->SetNormalImage(string2LPCTSTR("file='res\\winmode8_push.png'"));
		m_hdm1_btn->SetNormalImage(string2LPCTSTR("file='res\\disable_radio.png' dest='0,0,38,39'"));
		m_hdm2_btn->SetNormalImage(string2LPCTSTR("file='res\\disable_radio.png' dest='0,0,38,39'"));
		m_hdm3_btn->SetNormalImage(string2LPCTSTR("file='res\\disable_radio.png' dest='0,0,38,39'"));
		m_hdm4_btn->SetNormalImage(string2LPCTSTR("file='res\\disable_radio.png' dest='0,0,38,39'"));
		m_hdm1_btn->SetEnabled(false);
		m_hdm2_btn->SetEnabled(false);
		m_hdm3_btn->SetEnabled(false);
		m_hdm4_btn->SetEnabled(false);
		if (trigger)
		{
			logInfo("Execute Winmode8 operation.");
			Sleep(200);
			VideoScreenControl::GetInstance()->sendWinMode8Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI11Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI22Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI33Op();
			Sleep(200);
			VideoScreenControl::GetInstance()->sendHDMI44Op();
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoWinMode8CMD();
		}
		break;
	case VIDEO_HDMI1:
		selectedHDMIMode = type;
		m_hdm1_btn->SetNormalImage(string2LPCTSTR("file='res\\selected_radio.png' dest='0,0,38,39'")); 
		m_hdm2_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm3_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm4_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		if (trigger)
		{
			switch (selectedWinMode)
			{
				case VIDEO_WINMODE1:
					logInfo("Execute HDMI11 operation.");
					VideoScreenControl::GetInstance()->sendHDMI11Op();
					break;
				case VIDEO_WINMODE2:
					win = m_twoWinsQueueForWinMode2->getWin();
					if (win == 1)
					{
						logInfo("Execute HDMI11 operation.");
						VideoScreenControl::GetInstance()->sendHDMI11Op();					
					}
					else if (win == 2)
					{
						logInfo("Execute HDMI12 operation.");
						VideoScreenControl::GetInstance()->sendHDMI12Op();
					}
					break;
				case VIDEO_WINMODE4:
					win = m_twoWinsQueueForWinMode4->getWin();
					if (win == 1)
					{
						logInfo("Execute HDMI11 operation.");
						VideoScreenControl::GetInstance()->sendHDMI11Op();
					}
					else if (win == 2)
					{
						logInfo("Execute HDMI12 operation.");
						VideoScreenControl::GetInstance()->sendHDMI12Op();
					}
					break;
				case VIDEO_WINMODE6:
					win = m_twoWinsQueueForWinMode6->getWin();
					if (win == 1)
					{
						logInfo("Execute HDMI11 operation.");
						VideoScreenControl::GetInstance()->sendHDMI11Op();
					}
					else if (win == 2)
					{
						logInfo("Execute HDMI12 operation.");
						VideoScreenControl::GetInstance()->sendHDMI12Op();
					}
					break;
				case VIDEO_WINMODE7:
					win = m_threeWinsQueueForWinMode7->getWin();
					if (win == 1)
					{
						logInfo("Execute HDMI11 operation.");
						VideoScreenControl::GetInstance()->sendHDMI11Op();
					}
					else if (win == 2)
					{
						logInfo("Execute HDMI12 operation.");
						VideoScreenControl::GetInstance()->sendHDMI12Op();
					}
					else if (win == 3)
					{
						logInfo("Execute HDMI13 operation.");
						VideoScreenControl::GetInstance()->sendHDMI13Op();
					}
					break;
			}			
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoHDMI1CMD();
		}
		break;
	case VIDEO_HDMI2:
		selectedHDMIMode = type;
		m_hdm1_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm2_btn->SetNormalImage(string2LPCTSTR("file='res\\selected_radio.png' dest='0,0,38,39'"));
		m_hdm3_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm4_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		if (trigger)
		{
			switch (selectedWinMode)
			{
			case VIDEO_WINMODE1:
				logInfo("Execute HDMI21 operation.");
				VideoScreenControl::GetInstance()->sendHDMI21Op();
				break;
			case VIDEO_WINMODE2:
				win = m_twoWinsQueueForWinMode2->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI21 operation.");
					VideoScreenControl::GetInstance()->sendHDMI21Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI22 operation.");
					VideoScreenControl::GetInstance()->sendHDMI22Op();
				}
				break;
			case VIDEO_WINMODE4:
				win = m_twoWinsQueueForWinMode4->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI21 operation.");
					VideoScreenControl::GetInstance()->sendHDMI21Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI22 operation.");
					VideoScreenControl::GetInstance()->sendHDMI22Op();
				}
				break;
			case VIDEO_WINMODE6:
				win = m_twoWinsQueueForWinMode6->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI21 operation.");
					VideoScreenControl::GetInstance()->sendHDMI21Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI22 operation.");
					VideoScreenControl::GetInstance()->sendHDMI22Op();
				}
				break;
			case VIDEO_WINMODE7:
				win = m_threeWinsQueueForWinMode7->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI21 operation.");
					VideoScreenControl::GetInstance()->sendHDMI21Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI22 operation.");
					VideoScreenControl::GetInstance()->sendHDMI22Op();
				}
				else if (win == 3)
				{
					logInfo("Execute HDMI23 operation.");
					VideoScreenControl::GetInstance()->sendHDMI23Op();
				}
				break;
			}
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoHDMI2CMD();
		}
		break;
	case VIDEO_HDMI3:
		selectedHDMIMode = type;
		m_hdm1_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm2_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm3_btn->SetNormalImage(string2LPCTSTR("file='res\\selected_radio.png' dest='0,0,38,39'"));
		m_hdm4_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		if (trigger)
		{
			switch (selectedWinMode)
			{
			case VIDEO_WINMODE1:
				logInfo("Execute HDMI31 operation.");
				VideoScreenControl::GetInstance()->sendHDMI31Op();
				break;
			case VIDEO_WINMODE2:
				win = m_twoWinsQueueForWinMode2->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI31 operation.");
					VideoScreenControl::GetInstance()->sendHDMI31Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI32 operation.");
					VideoScreenControl::GetInstance()->sendHDMI32Op();
				}
				break;
			case VIDEO_WINMODE4:
				win = m_twoWinsQueueForWinMode4->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI31 operation.");
					VideoScreenControl::GetInstance()->sendHDMI31Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI32 operation.");
					VideoScreenControl::GetInstance()->sendHDMI32Op();
				}
				break;
			case VIDEO_WINMODE6:
				win = m_twoWinsQueueForWinMode6->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI31 operation.");
					VideoScreenControl::GetInstance()->sendHDMI31Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI32 operation.");
					VideoScreenControl::GetInstance()->sendHDMI32Op();
				}
				break;
			case VIDEO_WINMODE7:
				win = m_threeWinsQueueForWinMode7->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI31 operation.");
					VideoScreenControl::GetInstance()->sendHDMI31Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI32 operation.");
					VideoScreenControl::GetInstance()->sendHDMI32Op();
				}
				else if (win == 3)
				{
					logInfo("Execute HDMI33 operation.");
					VideoScreenControl::GetInstance()->sendHDMI33Op();
				}
				break;
			}
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoHDMI3CMD();
		}
		break;
	case VIDEO_HDMI4:
		selectedHDMIMode = type;
		m_hdm1_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm2_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm3_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
		m_hdm4_btn->SetNormalImage(string2LPCTSTR("file='res\\selected_radio.png' dest='0,0,38,39'"));
		if (trigger)
		{
			switch (selectedWinMode)
			{
			case VIDEO_WINMODE1:
				logInfo("Execute HDMI41 operation.");
				VideoScreenControl::GetInstance()->sendHDMI41Op();
				break;
			case VIDEO_WINMODE2:
				win = m_twoWinsQueueForWinMode2->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI41 operation.");
					VideoScreenControl::GetInstance()->sendHDMI41Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI42 operation.");
					VideoScreenControl::GetInstance()->sendHDMI42Op();
				}
				break;
			case VIDEO_WINMODE4:
				win = m_twoWinsQueueForWinMode4->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI41 operation.");
					VideoScreenControl::GetInstance()->sendHDMI41Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI42 operation.");
					VideoScreenControl::GetInstance()->sendHDMI42Op();
				}
				break;
			case VIDEO_WINMODE6:
				win = m_twoWinsQueueForWinMode6->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI41 operation.");
					VideoScreenControl::GetInstance()->sendHDMI41Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI42 operation.");
					VideoScreenControl::GetInstance()->sendHDMI42Op();
				}
				break;
			case VIDEO_WINMODE7:
				win = m_threeWinsQueueForWinMode7->getWin();
				if (win == 1)
				{
					logInfo("Execute HDMI41 operation.");
					VideoScreenControl::GetInstance()->sendHDMI41Op();
				}
				else if (win == 2)
				{
					logInfo("Execute HDMI42 operation.");
					VideoScreenControl::GetInstance()->sendHDMI42Op();
				}
				else if (win == 3)
				{
					logInfo("Execute HDMI43 operation.");
					VideoScreenControl::GetInstance()->sendHDMI43Op();
				}
				break;
			}
		}
		if (broadcast)
		{
			CommandManager::GetInstance()->sendVideoHDMI4CMD();
		}
		break;
	}
}

void ParticipantsUI::activeHDMIRadios()
{
	m_hdm1_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
	m_hdm2_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
	m_hdm3_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
	m_hdm4_btn->SetNormalImage(string2LPCTSTR("file='res\\non_selected_radio.png' dest='0,0,38,39'"));
	m_hdm1_btn->SetEnabled(true);
	m_hdm2_btn->SetEnabled(true);
	m_hdm3_btn->SetEnabled(true);
	m_hdm4_btn->SetEnabled(true);
	m_hdm1_btn->Selected(false);
	m_hdm2_btn->Selected(false);
	m_hdm3_btn->Selected(false);
	m_hdm4_btn->Selected(false);
}
void ParticipantsUI::resetSelectedVideoMode()
{
	int pictureDivider = CAGConfig::GetInstance()->GetPictureDivider();
	if (pictureDivider == 2)
	{
		switch (selectedVideoMode)
		{
		case VIDEO_HD1_MODE:
			m_hd1_btn->Selected(false);
			break;
		case VIDEO_HD2_MODE:
			m_hd2_btn->Selected(false);
			break;
		case VIDEO_PIP_MODE:
			m_pip_btn->Selected(false);
			break;
		case VIDEO_PUP_MODE:
			m_pup_btn->Selected(false);
			break;
		case VIDEO_PBYP_MODE:
			m_pbyp_btn->Selected(false);
			break;
		}
	}
	else if (pictureDivider == 4)
	{
		switch (selectedWinMode)
		{
		case VIDEO_WINMODE1:
			m_winmode1_btn->Selected(false);
			break;
		case VIDEO_WINMODE2:
			m_winmode2_btn->Selected(false);
			break;
		case VIDEO_WINMODE4:
			m_winmode4_btn->Selected(false);
			break;
		case VIDEO_WINMODE6:
			m_winmode6_btn->Selected(false);
			break;
		case VIDEO_WINMODE7:
			m_winmode7_btn->Selected(false);
			break;
		case VIDEO_WINMODE8:
			m_winmode8_btn->Selected(false);
			break;
		}

		switch (selectedHDMIMode)
		{
		case VIDEO_HDMI1:
			m_hdm1_btn->Selected(false);
			break;
		case VIDEO_HDMI2:
			m_hdm2_btn->Selected(false);
			break;
		case VIDEO_HDMI3:
			m_hdm3_btn->Selected(false);
			break;
		case VIDEO_HDMI4:
			m_hdm4_btn->Selected(false);
			break;
		}
	}
}

void ParticipantsUI::enableRemoteControl(bool enable)
{
	BOOL isRCEnable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
	if (isRCEnable)
	{
		vector<Individual*> all_participants = m_clientsManager->getAllParticipants();
		for (int i = 0; i < all_participants.size(); i++)
		{
			map<Individual*, CListContainerElementUI*>::iterator iter = m_model_ui_map.find(all_participants[i]);
			CButtonUI *pRemoteBtn = NULL;
			if (iter != m_model_ui_map.end())
			{
				CListContainerElementUI* pListItem = iter->second;
				pRemoteBtn = static_cast<CButtonUI *>(pListItem->FindSubControl(_T("remote_ctrl_btn")));
			}
			if (pRemoteBtn != NULL)
			{
				//pRemoteBtn->SetEnabled(enable);
				pRemoteBtn->SetVisible(enable);
				string hostuid = to_string(CAgoraObject::GetAgoraObject()->GetSelfUID());
				if (hostuid == all_participants[i]->getUserId())
				{
					pRemoteBtn->SetVisible(false);
				}
			}
		}
	}
	
}

