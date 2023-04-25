#include "stdafx.h"
#include "InitializationUI.h"
#include <cstring>
#include "AGConfig.h"

void InitializationUI::handleMeetingAccount(string rsp)
{
	rapidjson::Document doc;
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		string log = "Receive host meeting account response:";
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Host") && doc["Host"].IsObject())
				{
					const rapidjson::Value& host = doc["Host"];
					if (host.HasMember("is_enable") && host["is_enable"].IsBool())
					{
						bool is_enable = host["is_enable"].GetBool();
						if (!is_enable)
						{
							logInfo(log + "is_enable=false, Current device is not available");
							showInitializationError(L"Current device is not available, please contact the administrator.");
							return;
						}
					}
					string userid = "";
					string channelname = "";
					if (host.HasMember("id") && host["id"].IsInt())
					{
						userid = to_string(host["id"].GetInt());
						log = log + " id=" + userid;
					}
					if (host.HasMember("channel") && host["channel"].IsObject())
					{
						const rapidjson::Value& channel = host["channel"];
						if (channel.HasMember("channel_id") && channel["channel_id"].IsString())
						{
							channelname = channel["channel_id"].GetString();
							log = log + " channel_id=" + channelname;
						}
					}
					if (host.HasMember("hospital") && host["hospital"].IsObject())
					{
						const rapidjson::Value& hospital = host["hospital"];
						int hospital_id = hospital["id"].GetInt();
						string str_id = to_string(hospital_id);
						log = log + " hospital_id=" + str_id;
						CString cs_id(str_id.c_str());
						//CAGConfig::GetInstance()->SetHospitalId(cs_id);
						writeRegKey(HOSPITAL_ID, cs_id, APP_REG_DIR);
						if (hospital.HasMember("chinese_name") && hospital["chinese_name"].IsString())
						{
							wstring cn_name = StringUtil::utf8string2wstring(hospital["chinese_name"].GetString());
							log = log + " hospital_name=" + StringUtil::WStringToString(cn_name);
							writeRegKey(HOSPITAL_NAME, cn_name.c_str(), APP_REG_DIR);
							//CAGConfig::GetInstance()->SetHospitalName(cn_name.c_str());
						}
					}
					if (host.HasMember("department") && host["department"].IsObject())
					{
						const rapidjson::Value& department = host["department"];
						int department_id = department["id"].GetInt();
						string str_id = to_string(department_id);
						log = log + " department_id=" + str_id;
						CString cs_id(str_id.c_str());
						//CAGConfig::GetInstance()->SetDepartmentId(cs_id);
						writeRegKey(DEPARTMENT_ID, cs_id, APP_REG_DIR);
						if (department.HasMember("chinese_name") && department["chinese_name"].IsString())
						{
							wstring cn_name = StringUtil::utf8string2wstring(department["chinese_name"].GetString());
							log = log + " department_name=" + StringUtil::WStringToString(cn_name);
							//CAGConfig::GetInstance()->SetDepartmentName(cn_name.c_str());
							writeRegKey(DEPARTMENT_NAME, cn_name.c_str(), APP_REG_DIR);
						}
					}
					if (host.HasMember("equipment") && host["equipment"].IsObject())
					{
						const rapidjson::Value& equipment = host["equipment"];
						int equipment_id = equipment["id"].GetInt();
						string str_id = to_string(equipment_id);
						log = log + " equipment_id=" + str_id;
						CString cs_id(str_id.c_str());
						//CAGConfig::GetInstance()->SetEquipmentId(cs_id);
						writeRegKey(EQUIPMENT_ID, cs_id, APP_REG_DIR);
						if (equipment.HasMember("name") && equipment["name"].IsString())
						{
							string name = equipment["name"].GetString();
							log = log + " equipment_name=" + name;
							CString cs_name(StringUtil::StringToWstring(name).c_str());
							//CAGConfig::GetInstance()->SetEquipmentName(cs_name);
							writeRegKey(EQUIPMENT_NAME, cs_name, APP_REG_DIR);
							if (cs_name.MakeUpper() == L"ILAB" || cs_name.MakeUpper() == L"RHYTHMIA")
							{
								//CAGConfig::GetInstance()->setRelativeMousePos(TRUE);
								writeRegKey(RELATIVE_MOUSE_POS, L"True", APP_REG_DIR);
							}
							else
							{
								//"Polaris"
								writeRegKey(RELATIVE_MOUSE_POS, L"False", APP_REG_DIR);
							}
						}
					}
					logInfo(log);
					logToRTMService(userid, channelname);
				}
			}
			else
			{
				if (doc.HasMember("Error") && doc["Error"].IsString())
				{
					string error = doc["Error"].GetString();
					logError(error);
					showInitializationError(StringUtil::StringToWstring(error).c_str());
				}
			}
		}
	}
	else
	{
		logError("Error in parsing host meeting account response:" + rsp);
		showInitializationError(L"Can't get host messages from webserver.");
	}

}

void InitializationUI::onReceiveHospitalInfos(string rsp)
{
	rapidjson::Document doc;
	handlerHospitalInfos(rsp);

}
void InitializationUI::handlerHospitalInfos(string rsp)
{
	m_hospitalMgr->handleHospitalsInfo(rsp);
	int i;
	CListLabelElementUI* item;
	vector<Hospital*> hospitals = m_hospitalMgr->getHospitals();
	for (i = 0; i < hospitals.size(); i++)
	{
		item = new CListLabelElementUI();
		item->SetText(hospitals[i]->getCNName().c_str());
		m_hospital_combo->Add(item);
	}
	vector<Department*> departments = m_hospitalMgr->getDepartments();
	for (i = 0; i < departments.size(); i++)
	{
		item = new CListLabelElementUI();
		item->SetText(departments[i]->getCNName().c_str());
		m_department_combo->Add(item);
	}
	vector<Equipment*> equipments = m_hospitalMgr->getEquipments();
	for (i = 0; i < equipments.size(); i++)
	{
		item = new CListLabelElementUI();
		CString text = CString(equipments[i]->getName().c_str());
		item->SetText(text);
		m_equipment_combo->Add(item);
	}
	ShowWaiting(false);
	m_waiting_panel->SetVisible(false);
	m_login_panel->SetVisible(true);
}

InitializationUI::InitializationUI(HWND hwnd)
{
	hwndParent = hwnd;
	m_gifWaiting = NULL;
	m_waitingMessage = NULL;

	m_waiting_panel = NULL;
	m_login_panel = NULL;
	m_hospital_combo = NULL;
	m_equipment_combo = NULL;
	m_department_combo = NULL;
	m_init_btn = NULL;
	m_hospitalMgr = new HospitalMgr();
}

InitializationUI::~InitializationUI()
{
	m_gifWaiting = NULL;
	m_waitingMessage = NULL;
}

void InitializationUI::logToRTMService(string user_id, string channel_name)
{
	PHOST_LOG_TO_RTM_SERVICE lpData = new HOST_LOG_TO_RTM_SERVICE;
	//const char* channel = channel_name.c_str();
	//int nChannelLen = strlen(channel) + 1;
	//lpData->channel = new char[nChannelLen];
	//strcpy_s(lpData->channel, nChannelLen, channel);
	lpData->uid = atoi(user_id.c_str());
	lpData->channel = channel_name;
	
	if (hwndParent != NULL)
		::PostMessage(hwndParent, WM_HOSTLOGTORTMSERVICE, (WPARAM)lpData, 0);
}

void InitializationUI::InitWindow()
{
	
	RECT rc = { 0 };
	if (!::GetClientRect(m_hWnd, &rc)) return;
	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	rc.right = rc.left + rcWorkArea.Width();// width;
	rc.bottom = rc.top + rcWorkArea.Height()-64;// height;
	if (!::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd))) return;
	::SetWindowPos(m_hWnd, NULL, 0, 64, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitAllControls();
}

void InitializationUI::InitAllControls()
{
	m_waiting_panel = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("waiting_panel")));
	int w = m_waiting_panel->GetWidth();
	m_login_panel = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("login_panel")));
	m_hospital_combo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("hospital")));
	m_equipment_combo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("equipment")));
	m_department_combo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("department")));
	m_init_btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_init")));
	
	m_gifWaiting = static_cast<CGifAnimUI*>(m_PaintManager.FindControl(_T("gif_waiting")));
	if (!CAGConfig::GetInstance()->GetVersion().IsEmpty())
	{
		CLabelUI* version_label = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("version_label")));
		int size = getResolutionFontSize(MAX_VERSION_SIZE);
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
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	m_waitingMessage = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("waiting_message")));
	m_waitingMessage->SetAttribute(_T("width"), StringUtil::StringToWstring(to_string(rect.right - rect.left)).c_str());
	m_PaintManager.GetDefaultAttributeList(_T("VScrollBar"));
	ShowWaiting(true);
	if (!checkNetworkStatus())
	{
		logWarn("Network Connection Error");
		ShowWaiting(false);
		showInitializationError(L"Network Connection Error");
	}
	if (readRegKey(HOSPITAL_ID, APP_REG_DIR).IsEmpty())//(CAGConfig::GetInstance()->GetHospitalId().IsEmpty())
	{
		logInfo("No registered hospital info, request hospital infos from webserver.");
		onRequestHospitalInfos();
	}
	else
	{
		onRequestMeetingAccount();
	}

}

bool InitializationUI::checkNetworkStatus()
{
	DWORD   flag;
	bool    con = InternetGetConnectedState(&flag, 0);
	return con;
}


void InitializationUI::onRequestMeetingAccount()
{
	logInfo("Hospital information registered, request host account from webserver.");
	string uuid = UUIDGenerator::getInstance()->getUUID();
	CString ip = readRegKey(WEBSERVERIP, APP_REG_DIR);
	CString port = readRegKey(WEBSERVERPORT, APP_REG_DIR);
	CString hospital = readRegKey(HOSPITAL_ID, APP_REG_DIR);//CAGConfig::GetInstance()->GetHospitalId();
	CString equipment = readRegKey(EQUIPMENT_ID, APP_REG_DIR);// CAGConfig::GetInstance()->GetEquipmentId();
	CString department = readRegKey(DEPARTMENT_ID, APP_REG_DIR); //CAGConfig::GetInstance()->GetDepartmentId();
	string ip_str = CT2A(ip.GetBuffer());
	if (port.GetLength() > 0)
	{
		CString c_ip_str = ip + ":" + port;
		ip_str = CT2A(c_ip_str);
	}	
	string hospital_str = CT2A(hospital.GetBuffer());
	string equip_str = CT2A(equipment.GetBuffer());
	string department_str = CT2A(department.GetBuffer());
	string url = "http://" + ip_str + "/api-meeting/RequestMeetingAccount/UUID/" + uuid + "?hospital=" + hospital_str + "&equipment=" + equip_str + "&department=" + department_str;
	//string url = "http://" + ip_str + "/api-meeting/RequestMeetingAccount/UUID/cb253764-ef3c-4c2c-954b-52c5a53a294f";
	logInfo("Request url: " + url);
	m_waitingMessage->SetText(L"Waiting Login result...");
	string response = CurlHttpClient::SendGetReq(url.c_str());
	handleMeetingAccount(response);
	
}

void InitializationUI::onRequestHospitalInfos()
{
	CString ip = readRegKey(WEBSERVERIP, APP_REG_DIR); //CAGConfig::GetInstance()->GetWebServerIP();
	CString port = readRegKey(WEBSERVERPORT, APP_REG_DIR);//CAGConfig::GetInstance()->GetWebServerPort();
	string ip_str = CT2A(ip.GetBuffer());
	if (port.GetLength() > 0)
	{
		CString c_ip_str = ip + ":" + port;
		ip_str = CT2A(c_ip_str);
	}
	string url = "http://" + ip_str + "/api-meeting/getHostInitialInfo";
	logInfo("Request url: " + url);
	m_waitingMessage->SetText(L"Loading Infos...");
	string response = CurlHttpClient::SendGetReq(url.c_str());
	onReceiveHospitalInfos(response);
}


void InitializationUI::ShowWaiting(bool bWaiting)
{
	bWaiting ? m_gifWaiting->PlayGif() : m_gifWaiting->StopGif();
}



LRESULT InitializationUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	//TCHAR szLog[MAX_PATH] = { 0 };
	//wsprintf(szLog, _T("uMsg=%d\n"), uMsg);
	//OutputDebugString(szLog);
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
	else if (uMsg == WM_TIMER)
	{
		int n = 1;
	}
	else if (uMsg == WM_CLOSE)
	{
		OnClose(uMsg, wParam, lParam, bHandled);
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

LRESULT InitializationUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}

LRESULT InitializationUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_gifWaiting = NULL;
	m_waitingMessage = NULL;
	return 0;
}
void InitializationUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_init_btn)
		{
			ShowWaiting(true);
			m_waiting_panel->SetVisible(true);
			m_login_panel->SetVisible(false);
			if (!m_hospital_combo->GetShowText())
			{
				::MessageBox(NULL, L"请选择医院！", L"error", MB_OK);
				return;
			}
			if (!m_equipment_combo->GetShowText())
			{
				::MessageBox(NULL, L"请选择设备！", L"error", MB_OK);
				return;
			}
			if (!m_department_combo->GetShowText())
			{
				::MessageBox(NULL, L"请选择科室！", L"error", MB_OK);
				return;
			}
			LPCTSTR lhospital_str = m_hospital_combo->GetText();
			LPCTSTR ldepartment_str = m_department_combo->GetText();
			LPCTSTR lequipment_str = m_equipment_combo->GetText();
			string equipment_str = StringUtil::WStringToString(lequipment_str);
			vector<Hospital*> hospitals = m_hospitalMgr->getHospitals();
			vector<Department*> departments = m_hospitalMgr->getDepartments();
			vector<Equipment*> equipments = m_hospitalMgr->getEquipments();
			string hospital_id="";
			string department_id = "";
			string equipment_id = "";
			for (int i = 0; i < hospitals.size(); i++)
			{
				if (hospitals[i]->getCNName() == lhospital_str)
				{
					hospital_id = to_string(hospitals[i]->getId());
					CString hospitalCStr = CString(hospital_id.c_str());
					writeRegKey(HOSPITAL_ID, hospitalCStr, APP_REG_DIR);
					writeRegKey(HOSPITAL_NAME, lhospital_str, APP_REG_DIR);
					//CAGConfig::GetInstance()->SetHospitalId(hospitalCStr);
					//CAGConfig::GetInstance()->SetHospitalName(lhospital_str);
					break;
				}
			}
			for (int i = 0; i < departments.size(); i++)
			{
				if (departments[i]->getCNName() == ldepartment_str)
				{
					department_id = to_string(departments[i]->getId());
					CString departmentCStr = CString(department_id.c_str());
					writeRegKey(DEPARTMENT_ID, departmentCStr, APP_REG_DIR);
					writeRegKey(DEPARTMENT_NAME, ldepartment_str, APP_REG_DIR);
					//CAGConfig::GetInstance()->SetDepartmentId(departmentCStr);
					//CAGConfig::GetInstance()->SetDepartmentName(ldepartment_str);
					break;
				}
			}
			for (int i = 0; i < equipments.size(); i++)
			{
				if (equipments[i]->getName() == equipment_str)
				{
					equipment_id = to_string(equipments[i]->getId());
					CString equipmentCStr = CString(equipment_id.c_str());
					writeRegKey(EQUIPMENT_ID, equipmentCStr, APP_REG_DIR);
					writeRegKey(EQUIPMENT_NAME, CString(equipments[i]->getName().c_str()), APP_REG_DIR);
					//CAGConfig::GetInstance()->SetEquipmentId(equipmentCStr);
					//CAGConfig::GetInstance()->SetEquipmentName(CString(equipments[i]->getName().c_str()));
					break;
				}
			}
			onRequestMeetingAccount();
		}
	}
}

wstring InitializationUI::stringToWstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}

void InitializationUI::showInitializationError(const wchar_t* error)
{
	ShowWaiting(false);
	m_waitingMessage->SetText(error);
	m_waitingMessage->SetTextColor(0xFFB300);
}
