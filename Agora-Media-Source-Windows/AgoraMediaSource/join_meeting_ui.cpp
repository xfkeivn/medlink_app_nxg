#include "stdafx.h"
#include "join_meeting_ui.h"
#include "../rapidjson/document.h"

std::wstring gErrorMessage;
ClientJoinMeetingUI::ClientJoinMeetingUI()
{
	m_JoinMeetingPage = NULL;

	m_joinMeetingUIMgr = NULL;
	m_editMeetingNumber = NULL;
	m_btnJoin = NULL;
	m_bJoinIn = false;
	m_checkBoxAudio = NULL;
	m_joinWaitingPanel = NULL;
	m_gifJoinWaiting = NULL;
	m_errLabelUI = NULL;
}
ClientJoinMeetingUI::~ClientJoinMeetingUI()
{
	m_JoinMeetingPage = NULL;
	m_joinMeetingUIMgr = NULL;
	m_editMeetingNumber = NULL;
	m_btnJoin = NULL;
	m_bJoinIn = false;
	m_checkBoxAudio = NULL;
	m_joinWaitingPanel = NULL;
	m_gifJoinWaiting = NULL;
	m_errLabelUI = NULL;
}
void ClientJoinMeetingUI::InitWindow(CPaintManagerUI& ui_mgr, ClientJoinMeetingUIMgr* main_frame_)
{
	m_JoinMeetingPage = static_cast<CVerticalLayoutUI* >(ui_mgr.FindControl(_T("panel_join_meeting")));
	m_editMeetingNumber = static_cast<CRichEditUI* >(ui_mgr.FindControl(_T("edit_meeting_number")));
	m_editMeetingNumber->SetText(L"");	
	m_btnJoin = static_cast<CButtonUI* >(ui_mgr.FindControl(_T("btn_join_meeeting")));
	m_checkBoxAudio = static_cast<CCheckBoxUI*>(ui_mgr.FindControl(_T("chk_audio")));
	m_joinWaitingPanel = static_cast<CVerticalLayoutUI*>(ui_mgr.FindControl(_T("join_waiting_panel")));
	m_gifJoinWaiting = static_cast<CGifAnimUI*>(ui_mgr.FindControl(_T("gif_join_waiting")));
	m_gifJoinWaiting = static_cast<CGifAnimUI*>(ui_mgr.FindControl(_T("gif_join_waiting")));
	m_errLabelUI = static_cast<CLabelUI*>(ui_mgr.FindControl(_T("join_error_label")));
	if (!CAGConfig::GetInstance()->GetVersion().IsEmpty())
	{
		CLabelUI* version_label = static_cast<CLabelUI*>(ui_mgr.FindControl(_T("version_label")));
		CString version = L"Version:" + CAGConfig::GetInstance()->GetVersion();
		version_label->SetText(version);
	}
	else
	{
		logWarn("The version is empty.");
	}
	HideWaitingPanel();
	m_joinMeetingUIMgr = main_frame_;
}
void ClientJoinMeetingUI::UninitWindow()
{
}
void ClientJoinMeetingUI::Show()
{
	if(m_JoinMeetingPage)
	{
		m_JoinMeetingPage->SetVisible(true);
	}
}
void ClientJoinMeetingUI::Hide()
{
	if(m_JoinMeetingPage)
	{
		m_JoinMeetingPage->SetVisible(false);
	}
}
void ClientJoinMeetingUI::HideWaitingPanel()
{
	m_joinWaitingPanel->SetVisible(false);
}
void ClientJoinMeetingUI::ShowWaitingPanel()
{
	m_btnJoin->SetEnabled(false);
	m_errLabelUI->SetVisible(false);
	m_gifJoinWaiting->SetVisible(true);
	m_joinWaitingPanel->SetVisible(true);
}

void ClientJoinMeetingUI::ShowErrorMsg(const wchar_t* errStr)
{
	m_btnJoin->SetEnabled(true);
	m_gifJoinWaiting->SetVisible(false);
	m_errLabelUI->SetText(errStr);
	m_errLabelUI->SetVisible(true);
	m_joinWaitingPanel->SetVisible(true);
}

void ClientJoinMeetingUI::showNormalButton()
{
	m_btnJoin->SetEnabled(true);
	m_gifJoinWaiting->SetVisible(false);
	m_joinWaitingPanel->SetVisible(false);
}

void ClientJoinMeetingUI::acceptIncomingCall(const std::string &meeting_id)
{
	std::string meeting_id_str = meeting_id;
	std::wstring incoming_call_meeting_id = StringUtil::StringToWstring(meeting_id_str);
	m_editMeetingNumber->SetText(incoming_call_meeting_id.c_str());

	DoJoinMeetingBtnClick();
}

void ClientJoinMeetingUI::DoJoinMeetingBtnClick()
{
	gErrorMessage = L"";
	ShowWaitingPanel();

	if(NULL == m_editMeetingNumber)
		return;
	std::wstring wstrMeetingNumber = m_editMeetingNumber->GetText().GetData();

	if (wstrMeetingNumber.length() <= 0)
	{
		ShowErrorMsg(L"Meeting Number can not be empy!");
		return;
	}

	m_joinMeetingUIMgr->ShowWindow(false);

	MEETING_INFO_STRUCT* tmp_meeting_struct = new MEETING_INFO_STRUCT;
	tmp_meeting_struct->meeting_channel = StringUtil::WStringToString(wstrMeetingNumber);
	tmp_meeting_struct->audiomuted = m_checkBoxAudio->GetCheck();
	::PostMessage(m_joinMeetingUIMgr->GetParentHWND(), WM_CLIENT_JOINMEETING, (WPARAM)tmp_meeting_struct, 0);

}

void ClientJoinMeetingUI::DoSignoutBtnClick()
{
	::PostMessage(m_joinMeetingUIMgr->GetParentHWND(), WM_CLIENT_LOGOUT, 0, 0);
}

void ClientJoinMeetingUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_btnJoin)
		{
			DoJoinMeetingBtnClick();
		}
		//else if (msg.pSender == m_btnSignout)
		//{
		//	DoSignoutBtnClick();
		//}
	}
}

//void CJoinMeetingUI::onMeetingStatusChanged(ZOOM_SDK_NAMESPACE::MeetingStatus status, int iResult)
//{
//	CHAR szlog[MAX_PATH] = { 0 };
//
// 	switch (status)
// 	{
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_IDLE:///<No meeting is running.
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_FAILED:///<Failed to connect the meeting server.
//			{
//				gErrorMessage = L"Join meeting failed!";
//
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_IDLE or MEETING_STATUS_FAILED! \n");
//				OutputDebugStringA(szlog);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_CONNECTING:///<Connect to the meeting server status.
//			{
//				if (m_joinMeetingUIMgr)
//				{
//					m_joinMeetingUIMgr->ShowWindow(false);
//				}
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_CONNECTING! \n");
//				OutputDebugStringA(szlog);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_IN_WAITING_ROOM:///<Participants who join the meeting before the start are in the waiting room.
//			{
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_IN_WAITING_ROOM! \n");
//				OutputDebugStringA(szlog);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_WAITINGFORHOST:///<Waiting for the host to start the meeting.
//			{
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_WAITINGFORHOST! \n");
//				OutputDebugStringA(szlog);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING:///<Meeting is ready, in meeting status.
//			{
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_INMEETING! \n");
//				OutputDebugStringA(szlog);
//				string url1 = "http://127.0.0.1:7999/Client/" + m_joinMeetingUIMgr->GetAppEvent()->getUserInfoParam()->getLoginName();
//				//string url1 = "http://10.226.102.156:7999/Client/" + m_joinMeetingUIMgr->GetAppEvent()->getUserInfoParam()->getLoginName();
//				HttpClient::SendReq(url1, "ReportJoinMeeting", handleHttpReportStatusRes, this);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_DISCONNECTING:///<Disconnect the meeting server, leave meeting status.
//			{
//				gErrorMessage = L"Meeting is disconnected!";
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_DISCONNECTING! \n");
//				OutputDebugStringA(szlog);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_RECONNECTING:///<Reconnecting meeting server status.
//			{
//				sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_RECONNECTING! \n");
//				OutputDebugStringA(szlog);
//			}
//			break;
//
//		case ZOOM_SDK_NAMESPACE::MEETING_STATUS_ENDED:///<Meeting ends.
//			{
//				if (m_joinMeetingUIMgr)
//				{
//					m_joinMeetingUIMgr->ShowWindow(true);
//					m_joinMeetingUIMgr->GetAppEvent()->publish(ZOOM_SDK_NAMESPACE::USER_STATUS_ENDMEETING, false);
//					m_bJoinIn = false;
//					ShowErrorMsg(gErrorMessage.c_str());
//				}
//				string url1 = "http://127.0.0.1:7999/Client/" + m_joinMeetingUIMgr->GetAppEvent()->getUserInfoParam()->getLoginName();
//				//string url1 = "http://10.226.102.156:7999/Client/" + m_joinMeetingUIMgr->GetAppEvent()->getUserInfoParam()->getLoginName();
//				HttpClient::SendReq(url1, "ReportExitMeeting", handleHttpReportStatusRes, this);
//			}
//			sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_ENDED! \n");
//			OutputDebugStringA(szlog);
//			break;
//
// 		default:
//			/* Default is for the following status, do nothing.
//
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_UNKNOW:///<Unknown status.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_LOCKED:///<Meeting is locked to prevent the further participants to join the meeting.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_UNLOCKED:///<Meeting is open and participants can join the meeting.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_WEBINAR_PROMOTE:///<Upgrade the attendees to panelist in webinar.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_WEBINAR_DEPROMOTE:///<Downgrade the attendees from the panelist.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_JOIN_BREAKOUT_ROOM:///<Join the breakout room.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_LEAVE_BREAKOUT_ROOM:///<Leave the breakout room.
//				case ZOOM_SDK_NAMESPACE::MEETING_STATUS_WAITING_EXTERNAL_SESSION_KEY:///<Waiting for the additional secret key.
//			*/
//			sprintf_s(szlog, "MeetingStatusChanged: MEETING_STATUS_IDLE or MEETING_STATUS_FAILED! \n");
//			OutputDebugStringA(szlog);
// 			break;
// 	}
//}

//ReportJoinMeeting/ReportExitMeeting:
//{"Result":"True/False", "Error":"0"}]}
void ClientJoinMeetingUI::handleHttpReportStatusRes(string rsp, void* pParam)
{
	rapidjson::Document doc;
	CHAR szlog[MAX_PATH] = { 0 };

	if (!doc.Parse(rsp.data()).HasParseError())
	{
		if (doc.HasMember("Result") && doc["Result"].IsString())
		{
			std::string result = doc["Result"].GetString();
			result = StringUtil::StringTrim(result.c_str());
			if (result.compare("True") == 0)
			{
				sprintf_s(szlog, "Report meeting status through http success! \n");
				OutputDebugStringA(szlog);
			}
			else //result is False
			{
				if (doc.HasMember("Error") && doc["Error"].IsString())
				{
					sprintf_s(szlog, "Report meeting status through http failed! Error=%s !\n", doc["Error"].GetString());
					OutputDebugStringA(szlog);
				}
				else
				{
					sprintf_s(szlog, "Report meeting status through http failed! Error is unknown!\n", doc["Error"].GetString());
					OutputDebugStringA(szlog);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///CJoinMeetingUIMgr
ClientJoinMeetingUIMgr::ClientJoinMeetingUIMgr(HWND hWND)
{
	m_parentHWND = hWND;
}
ClientJoinMeetingUIMgr::~ClientJoinMeetingUIMgr()
{
	m_parentHWND = NULL;
}

void ClientJoinMeetingUIMgr::acceptIncomingCall(const std::string &meeting_id)
{
	m_JoinMeetingUI.acceptIncomingCall(meeting_id);
}

void ClientJoinMeetingUIMgr::show()
{
	ShowWindow(true);
	m_JoinMeetingUI.showNormalButton();
}

void ClientJoinMeetingUIMgr::InitWindow()
{
	m_JoinMeetingUI.InitWindow(m_PaintManager,this);
	RECT rc = { 0 };
	if( !::GetClientRect(m_hWnd, &rc) ) return;
	rc.right = rc.left + 420;
	rc.bottom = rc.top + 770; //770-24=746
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) return;
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);
	
	//::SetWindowPos(m_hWnd, NULL, (ScreenX - (rc.right - rc.left)) / 2, 
	//	(ScreenY - (rc.bottom - rc.top)) / 2, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);
	::SetWindowPos(m_hWnd, NULL, (ScreenX - (rc.right - rc.left)-5),
		42, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);
}
void ClientJoinMeetingUIMgr::Notify( TNotifyUI& msg )
{
	m_JoinMeetingUI.Notify(msg);
}
LRESULT ClientJoinMeetingUIMgr::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	if( uMsg == WM_CREATE ) 
	{
		m_PaintManager.Init(m_hWnd);

		CDialogBuilder builder;
		STRINGorID xml(GetSkinRes());
		CControlUI* pRoot = builder.Create(xml, _T("xml"), 0, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		InitWindow(); 
		return lRes;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
			m_JoinMeetingUI.DoJoinMeetingBtnClick();
			break;
		default:
			break;
		}

	}
	else if (uMsg == WM_CLOSE)
	{
		OnClose(uMsg, wParam, lParam, bHandled);		
	}
	else if (uMsg == WM_DESTROY)
	{
		OnDestroy(uMsg, wParam, lParam, bHandled);
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ClientJoinMeetingUIMgr::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	::PostMessage(m_parentHWND, WM_CLIENT_QUITAPP, 0, 0);
	return 0;
}
LRESULT ClientJoinMeetingUIMgr::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_JoinMeetingUI.UninitWindow();																								  
	return 0;
}

HWND ClientJoinMeetingUIMgr::GetParentHWND()
{
	return m_parentHWND;
}