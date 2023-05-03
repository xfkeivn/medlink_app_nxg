#include "stdafx.h"
#include "InitializationUI.h"
#include <cstring>
#include "AGConfig.h"
#include "ConfigCenter.h"
#include "BackendComm.h"


void InitializationUI::handlerHospitalInfos()
{
	
	int i;
	CListLabelElementUI* item;
	vector<Hospital> &hospitals = m_hospitalMgr.getHospitals();
	for (i = 0; i < hospitals.size(); i++)
	{
		item = new CListLabelElementUI();
		item->SetText(hospitals[i].getCNName().c_str());
		m_hospital_combo->Add(item);
	}
	vector<Department> &departments = m_hospitalMgr.getDepartments();
	for (i = 0; i < departments.size(); i++)
	{
		item = new CListLabelElementUI();
		item->SetText(departments[i].getCNName().c_str());
		m_department_combo->Add(item);
	}
	vector<Equipment> equipments = m_hospitalMgr.getEquipments();
	for (i = 0; i < equipments.size(); i++)
	{
		item = new CListLabelElementUI();
		CString text = CString(equipments[i].getName().c_str());
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
	if (RegConfig::Instance()->getHospitalId().empty())//(CAGConfig::GetInstance()->GetHospitalId().IsEmpty())
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
	
	m_waitingMessage->SetText(L"Waiting Login result...");
	string error_string, userid, channelname;
	bool result = BackendCommImpl::Instance()->requestMeetingAccount(userid,channelname,error_string);
	if (result)
	{
		logToRTMService(userid, channelname);
	}
	else
	{
		showInitializationError(utf82cs( error_string));
	}
	//handleMeetingAccount(response);
	
}

void InitializationUI::onRequestHospitalInfos()
{
	m_waitingMessage->SetText(L"Loading Infos...");
	
	bool result = BackendCommImpl::Instance()->requestHostInitialInfos(m_hospitalMgr);
	if (result)
	{
		handlerHospitalInfos();
	}
	else
	{
		showInitializationError(L"There is error during get the hospiatal information");
	}
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
			vector<Hospital> &hospitals = m_hospitalMgr.getHospitals();
			vector<Department> &departments = m_hospitalMgr.getDepartments();
			vector<Equipment> &equipments = m_hospitalMgr.getEquipments();
			string hospital_id="";
			string department_id = "";
			string equipment_id = "";
			for (int i = 0; i < hospitals.size(); i++)
			{
				if (hospitals[i].getCNName() == lhospital_str)
				{
					hospital_id = to_string(hospitals[i].getId());
					//CString hospitalCStr = CString(hospital_id.c_str());
					RegConfig::Instance()->setHospitalId(hospital_id);
					//string hospname = StringUtil::wstring2utf8string(lhospital_str);
					RegConfig::Instance()->setHospitalName(lhospital_str);
					
					break;
				}
			}
			for (int i = 0; i < departments.size(); i++)
			{
				if (departments[i].getCNName() == ldepartment_str)
				{
					department_id = to_string(departments[i].getId());
					CString departmentCStr = CString(department_id.c_str());
					RegConfig::Instance()->setDepartmentId(department_id);
					//string departname = StringUtil::wstring2utf8string(ldepartment_str);
					RegConfig::Instance()->setDepartmentName(ldepartment_str);
					break;
				}
			}
			for (int i = 0; i < equipments.size(); i++)
			{
				if (equipments[i].getName() == equipment_str)
				{
					equipment_id = to_string(equipments[i].getId());

					CString equipmentCStr = CString(equipment_id.c_str());
					RegConfig::Instance()->setEquipmentId(equipment_id);
					
					RegConfig::Instance()->setEquipmentTypeName(StringUtil::utf8string2wstring(equipment_str));
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
