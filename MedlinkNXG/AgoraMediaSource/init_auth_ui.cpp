#include "stdafx.h"
#include "init_auth_ui.h"
#include "../rapidjson/document.h"
#pragma comment(lib,"Wininet.lib")
USER_INFO_STRUCT userInfoStruct;

ClientLoginUI::ClientLoginUI()
{
	m_loginPage = NULL;
	m_editName = NULL;
	m_editPassword = NULL;
	m_chkRememberMe = NULL;
	m_btnLogin = NULL;

	m_bCheckNetwork = false;
	m_bConnectRedisServer = false;
	m_bSDKInit = false;
	m_bSDKAuth = false;
}
ClientLoginUI::~ClientLoginUI()
{
	UninitWindow();
}
void ClientLoginUI::InitWindow(CPaintManagerUI& ui_mgr, ClientLoginUIMgr* init_mgr)
{
	m_initMgr = init_mgr;

	m_loginPage = static_cast<CVerticalLayoutUI*>(ui_mgr.FindControl(_T("login_panel")));

	m_lableError = static_cast<CLabelUI*>(ui_mgr.FindControl(_T("error_label")));
	m_lableError->SetVisible(false);

	m_editName = static_cast<CRichEditUI*>(ui_mgr.FindControl(_T("edit_user_email")));
	m_editPassword = static_cast<CRichEditUI*>(ui_mgr.FindControl(_T("edit_user_password")));

	m_chkRememberMe = static_cast<CCheckBoxUI*>(ui_mgr.FindControl(_T("chk_remember_me")));

	m_btnLogin = static_cast<CButtonUI*>(ui_mgr.FindControl(_T("btn_login")));
	
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
	m_loginWaitingPanel = static_cast<CHorizontalLayoutUI*>(ui_mgr.FindControl(_T("login_waiting_panel")));
	m_loginWaitingPanel->SetVisible(false);

	m_loginPage->SetVisible(false);
}

void ClientLoginUI::UninitWindow()
{
	m_loginPage = NULL;
	m_editName = NULL;
	m_editPassword = NULL;
	m_chkRememberMe = NULL;
	m_btnLogin = NULL;
	m_loginWaitingPanel = NULL;
}

void ClientLoginUI::CheckNetwork()
{
	//m_initMgr->SwitchToWaitingPage(true, L"Checking network...", true);

	//DWORD   flag;

	////bool    con = InternetGetConnectedState(&flag, 0);
	//bool con = true;
	//if (!con)
	//{
	//	m_initMgr->SwitchToWaitingPage(true, L"Network connection failed!", false);
	//}
	//else
	//{
	//	m_bCheckNetwork = true;
	//	m_initMgr->SwitchToWaitingPage(true, L"Initializing SDK...", true);
	//	SDKInit();
	//}
}

void ClientLoginUI::SDKInit()
{
	//CString strAppid("9fd9ea1c21d04fa7ab50f04ca4c6c991");
	//if (_tcslen(APP_ID) == 0) {
	//	CString appid = m_agConfig.GetAppid();
	//	if (appid.IsEmpty()) {
	//		m_initMgr->SwitchToWaitingPage(true, L"Please confirm your APP_ID! ", false);			
	//		m_agConfig.SetAppid(strAppid);			
	//		ShellExecute(NULL, _T("open"), m_agConfig.GetFilePath(), NULL, NULL, SW_SHOW);

	//		MessageBox(NULL, _T("Please confirm your APP_ID."), _T("information"), MB_OK | MB_ICONINFORMATION);
	//		::PostQuitMessage(0);
	//	}
	//	else
	//	{
	//		strAppid = m_agConfig.GetAppid();
	//		SwitchToLoginPage();
	//	}
	//}
	//else
	//{
	//	strAppid = APP_ID;
	//	SwitchToLoginPage();
	//}
	//SwitchToLoginPage();
	//m_initMgr->GetAppEvent()->setAppId(strAppid);
}

void ClientLoginUI::SwitchToLoginPage()
{
	m_initMgr->SwitchToWaitingPage(false, L"", false);

	CString isRememberMe = CAGConfig::GetInstance()->GetRememberMe();
	if (isRememberMe.IsEmpty() || isRememberMe.Compare(L"False") == 0) 
	{
		m_chkRememberMe->SetCheck(false);
		m_editName->SetText(L"");
		m_editPassword->SetText(L"");
	}
	else
	{
		m_chkRememberMe->SetCheck(true);
		//CString email = CAGConfig::GetInstance()->GetEmail();
		CString email = readRegKey(USEREMAIL, APP_REG_DIR);
		if (!email.IsEmpty())
		{
			m_editName->SetText(email.GetString());
			//CString password = CAGConfig::GetInstance()->GetPassword();
			CString password = readRegKey(USERPASSWORD, APP_REG_DIR);
			if (!password.IsEmpty())
			{
				m_editPassword->SetText(password.GetString());
			}
		}
	}

	//////////////////////TempChange//////////////////
	//m_editName->SetText(L"Yongcheng.Fang@bsci.com");
	//m_editPassword->SetText(L"Boston$123");
	//////////////////////////////////////////////////

	m_loginPage->SetVisible(true);
}

void ClientLoginUI::UpdateConfig(bool loginResult)
{
	bool isRememberMe = m_chkRememberMe->GetCheck();
	if (isRememberMe)
	{
		CAGConfig::GetInstance()->SetRememberMe(_T("True"));
		wstring email = m_editName->GetText().GetData();
		CString emailCstr(email.c_str());
		writeRegKey(USEREMAIL, emailCstr, APP_REG_DIR);
		//CAGConfig::GetInstance()->SetEmail(emailCstr);
		if (loginResult)
		{
			wstring password = m_editPassword->GetText().GetData();
			CString passwordCstr(password.c_str());
			//CAGConfig::GetInstance()->SetPassword(passwordCstr);
			writeRegKey(USERPASSWORD, passwordCstr, APP_REG_DIR);
		}
		else
		{
			//CAGConfig::GetInstance()->SetPassword(_T(""));
			writeRegKey(USERPASSWORD, _T(""), APP_REG_DIR);
		}
	}
	else
	{
		CAGConfig::GetInstance()->SetRememberMe(_T("False"));
		//CAGConfig::GetInstance()->SetEmail(_T(""));
		//CAGConfig::GetInstance()->SetPassword(_T(""));
		writeRegKey(USEREMAIL, _T(""), APP_REG_DIR);
		writeRegKey(USERPASSWORD, _T(""), APP_REG_DIR);
	}
}

void ClientLoginUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_btnLogin)
		{
			DoLoginBtnClick();
		}
	}
}

void ClientLoginUI::DoLoginBtnClick()
{
	m_lableError->SetVisible(false);
	m_editName->SetEnabled(false);
	m_editPassword->SetEnabled(false);
	m_btnLogin->SetEnabled(false);
	m_loginWaitingPanel->SetVisible(true);

	std::wstring m_strEmail = m_editName->GetText().GetData();
	std::string userName = StringUtil::WStringToString(m_strEmail);
	std::wstring strPwd = m_editPassword->GetText().GetData();
	std::string userPwd = StringUtil::WStringToString(strPwd);
	m_rememberMe = m_chkRememberMe->GetCheck();

	if (m_strEmail.size() == 0 || strPwd.size() == 0)
	{
		ShowLoginErrorMessage(_T("Error: Email or Password can not be empty!"));
		return;
	}

	ZeroMemory(&userInfoStruct, sizeof(userInfoStruct));
	userInfoStruct.login_name = userName.c_str();
	userInfoStruct.login_pwd = userName.c_str();

	//logInfo("Input username:" + userName + ",password:" + userPwd + ", login to webserver...");
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
	//string url = "http://106.14.97.27/api-meeting/RequestLogin/ClientLogin?user=" + userName + "&pwd=" + userPwd;
	string url = "http://" + ip_str +"/api-meeting/RequestLogin/ClientLogin?user=" + userName + "&pwd=" + userPwd;
	string response = CurlHttpClient::SendGetReq(url.c_str());
	//logInfo("Request url:" + url);
	handleHttpLoginRes(response);
}



void ClientLoginUI::ShowLoginErrorMessage(const wchar_t * error_message)
{
	m_lableError->SetText(error_message);
	m_lableError->SetVisible(true);
	m_btnLogin->SetEnabled(true);
	m_editName->SetEnabled(true);
	m_editPassword->SetEnabled(true);
	m_loginWaitingPanel->SetVisible(false);
}

void ClientLoginUI::LogOut()
{
	m_lableError->SetText(L"");
	m_lableError->SetVisible(false);
	m_btnLogin->SetEnabled(true);
	m_editName->SetEnabled(true);

	if (!m_rememberMe)
	{
		m_editPassword->SetText(L"");
	}
	m_editPassword->SetEnabled(true);
	m_loginWaitingPanel->SetVisible(false);
}

ClientLoginUIMgr* ClientLoginUI::GetInitAuthLoginUIMgr()
{
	return m_initMgr;
}

//RequestLogin:
//{"Result":"True/False", "Error":"0", Clients:[{"DisplayName":"Mr. Fang"}]}
void ClientLoginUI::handleHttpLoginRes(string rsp)
{
	printf("handleHttpLoginRes: %s", rsp);
	rapidjson::Document doc;
	logInfo("Receive handleHttpLoginRes:" + rsp);
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			string log = "Client login response:";
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Client") && doc["Client"].IsObject())
				{
					const rapidjson::Value& client = doc["Client"];
					if (client.HasMember("id") && client["id"].IsInt())
					{
						userInfoStruct.uid = to_string(client["id"].GetInt());
						log = log + "id=" + userInfoStruct.uid;
					}
					if (client.HasMember("display_name") && client["display_name"].IsString())
					{
						wstring wname = StringUtil::utf8string2wstring(client["display_name"].GetString());
						userInfoStruct.display_name = wname;
						log = log + " display_name=" + StringUtil::WStringToString(wname);
					}
					if (client.HasMember("host_list") && client["host_list"].IsArray())
					{
						const rapidjson::Value& array = client["host_list"];
						size_t len = array.Size();
						log = log + " host_ids{";
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& object = array[i];
							if (object.IsObject())
							{
								userInfoStruct.hostids[i] = object["id"].GetInt();
								log = log +" " + to_string(userInfoStruct.hostids[i]);
							}
						}
						log = log + "}";
						userInfoStruct.hostsize = len;
					}
					logInfo(log);
				}
				UpdateConfig(true);
				::PostMessage(GetInitAuthLoginUIMgr()->GetParentHWND(), WM_CLIENTLOGTORTMSERVICE, (WPARAM)&userInfoStruct, 0);

			}
			else //result is False
			{
				CHAR szlog[MAX_PATH] = { 0 };
				if (doc.HasMember("Error") && doc["Error"].IsString())
				{
					std::string err = doc["Error"].GetString();
					logInfo("handleHttpLoginRes Failed. Error:" + err);
					//sprintf_s(szlog, "handleHttpLoginRes Failed: Error=%s \n", err);
					//OutputDebugStringA(szlog);
				}
				else
				{
					logInfo("handleHttpLoginRes Failed: Error is unknown!");
					sprintf_s(szlog, "handleHttpLoginRes Failed: Error is unknown!\n");
					OutputDebugStringA(szlog);
				}

				UpdateConfig(true);
				ShowLoginErrorMessage(L"Login failed! Please check the name and password.");
			}
		}
		else
		{
			logError("Error in parsing client login response:" + rsp);
		}
	}
}


/////////////////////////////////////////
ClientLoginUIMgr::ClientLoginUIMgr(HWND hWND, CAgoraObject* agObject)
{
	m_waitingPage = NULL;
	m_lableWaiting = NULL;
	m_gifWaiting = NULL;
	m_btnRetry = NULL;
	m_parentHWND = hWND;
	m_agObject = agObject;
}

ClientLoginUIMgr::~ClientLoginUIMgr()
{
	m_lableWaiting = NULL;
	m_gifWaiting = NULL;
	m_waitingPage = NULL;
	m_btnRetry = NULL;
}

void ClientLoginUIMgr::InitWindow()

{
	RECT rc = { 0 };
	if (!::GetClientRect(m_hWnd, &rc)) return;
	//rc.right = rc.left + 524;
	//rc.bottom = rc.top + 352; //376-24=352
	//if (!::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd))) return;
	//int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	//int ScreenY = GetSystemMetrics(SM_CYSCREEN);

	rc.right = rc.left + 524;// width;
	rc.bottom = rc.top + 352;// height;
	if (!::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd))) return;
	::SetWindowPos(m_hWnd, NULL, 0, 32, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);

	m_waitingPage = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("waiting_panel")));
	m_lableWaiting = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("waiting_label")));
	m_gifWaiting = static_cast<CGifAnimUI*>(m_PaintManager.FindControl(_T("gif_waiting")));
	m_btnRetry = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_retry")));

	m_loginUI.InitWindow(m_PaintManager, this);
	m_loginUI.SwitchToLoginPage();
	

}

void ClientLoginUIMgr::Notify(TNotifyUI& msg)
{
	m_loginUI.Notify(msg);

	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_btnRetry)
		{
			DoRetryBtnClick();
		}
	}
}

void ClientLoginUIMgr::DoRetryBtnClick()
{
	//RTMLogin();
}

void ClientLoginUIMgr::RTMLogin(string uid)
{
	m_agObject->SetSelfUID(str2int(uid));
	m_agObject->RTMLogin();
}

LRESULT ClientLoginUIMgr::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	if (uMsg == WM_CREATE)
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
			m_loginUI.DoLoginBtnClick();
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
	//else if (uMsg == WM_LoginSuccess)
	//{
	//	m_loginUI.SwitchToLoginPage();
	//}
	//else if (uMsg == WM_LoginFailed)
	//{
	//	SwitchToWaitingPage(true, L"Network connection failed!", false);
	//}

	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ClientLoginUIMgr::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	::PostMessage(m_parentHWND, WM_CLIENT_QUITAPP, 0, 0);
	return 0;
}

LRESULT ClientLoginUIMgr::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_loginUI.UninitWindow();
	return 0;
}

void ClientLoginUIMgr::SwitchToWaitingPage(bool showWaitingPage, const wchar_t* waiting_message, bool show_waiting_gif)
{
	if (showWaitingPage)
	{
		m_waitingPage->SetVisible(true);

		m_lableWaiting->SetText(waiting_message);

		if (show_waiting_gif)
		{
			m_gifWaiting->PlayGif();
			m_gifWaiting->SetVisible(true);
			m_btnRetry->SetVisible(false);
		}
		else
		{
			m_gifWaiting->StopGif();
			m_gifWaiting->SetVisible(false);
			m_btnRetry->SetVisible(true);
		}
	}
	else
	{
		m_waitingPage->SetVisible(false);
	}
}

void ClientLoginUIMgr::ShowErrorMessage(const wchar_t * error_message)
{
	m_loginUI.ShowLoginErrorMessage(error_message);
}

void ClientLoginUIMgr::LogOut()
{
	m_loginUI.LogOut();
}

HWND ClientLoginUIMgr::GetParentHWND()
{
	return m_parentHWND;
}
