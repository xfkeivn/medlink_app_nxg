#include "stdafx.h"
#include "HostDUIMgr.h"

bool checkNetworkStatus()
{
	DWORD   flag;
	bool    con = InternetGetConnectedState(&flag, 0);
	return con;
}

DUIMgr::DUIMgr(HWND hwnd)
{
	m_hwndParent = hwnd;
	meetingId = -1;
	initialization_ui = NULL;
}

DUIMgr::~DUIMgr()
{

}

void DUIMgr::onShowInitializationUI()
{
	logInfo("Show InitializationUI...");
	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	::CoInitialize(NULL);
	initialization_ui = new InitializationUI(m_hwndParent);
	initialization_ui->Create(m_hwndParent, _T("MedLink"), UI_WNDSTYLE_DIALOG & (~(WS_BORDER | WS_CAPTION)), 0, 0, 0, 600, 800);
	initialization_ui->SetIcon(IDI_ICON_LOGO);
	//initialization_ui->Create(m_hwndParent, _T("Med-Link"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	initialization_ui->ShowWindow(true);
}

void DUIMgr::onShowLoggedInUI()
{
	initialization_ui->ShowWindow(false);
	loggedIn_ui = new LoggedInUI(m_hwndParent, string(strChannelName));
	loggedIn_ui->Create(m_hwndParent, _T("MedLink"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	loggedIn_ui->ShowWindow(true);
	updateCOMPortandDisableCamera();
}

void DUIMgr::onSwitchToLoggedInUI()
{
	loggedIn_ui->switchToLoggedInUI();
}

void DUIMgr::onShowError(const wchar_t* error_message)
{
	initialization_ui->showInitializationError(error_message);
}

void  DUIMgr::setChannelName(const char* channel)
{
	this->strChannelName = channel;
}

const char* DUIMgr::getChannelName()
{
	return this->strChannelName;
}
void  DUIMgr::setUserId(int id)
{
	this->userId = id;
}

int DUIMgr::getUserId()
{
	return this->userId;
}

void DUIMgr::setMeetingID(int id)
{
	this->meetingId = id;
}

int DUIMgr::getMeetingID()
{
	return this->meetingId;
}

void DUIMgr::updateClientsStatus(string user_id, UserStatus status,string client_channel)
{
	loggedIn_ui->updateClientsStatus(user_id, status, client_channel);
}

void DUIMgr::onInvitationRefusedByClient(string client_id)
{
	loggedIn_ui->onInvitationRefuseOrTimeoutByClient(client_id, true);
}

void DUIMgr::onInvitationTimeoutByClient(string client_id)
{
	loggedIn_ui->onInvitationRefuseOrTimeoutByClient(client_id, false);
}


void DUIMgr::onInvitationEnd()
{
	loggedIn_ui->onInvitationEnd();
}

void DUIMgr::setMeetingStatus(bool inMeeting)
{
	loggedIn_ui->setMeetingStatus(inMeeting);
}

bool DUIMgr::isInMeeting()
{
	return loggedIn_ui->isInMeeting();
}

wstring DUIMgr::getDisplayName(string user_id)
{
	return loggedIn_ui->getDisplayName(user_id);
}
void DUIMgr::updateCOMPort()
{
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		logWarn("获取系统设备列表失败");
		return;
	}
	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	deviceInfoData.DevInst = 0;
	deviceInfoData.Reserved = 0;
	DWORD regDataType;
	LPTSTR buffer = new TCHAR[200];
	DWORD buffersize = 200;
	LPTSTR bufferPortNum = new TCHAR[200];
	DWORD buffersizePortNum = 200;
	CString ComPort = L"";
	vector<CString> comdevice_names = CAGConfig::GetInstance()->GetCOMDeviceName();
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++)
	{
		buffersize = 200;
		SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_CLASS, &regDataType, (PBYTE)buffer, buffersize, &buffersize);
		if (CString(buffer).MakeUpper() == L"PORTS")
		{
			buffersizePortNum = 200;
			SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, &regDataType, (PBYTE)bufferPortNum, buffersizePortNum, &buffersizePortNum);
			CString comPortInfo = CString(bufferPortNum);
			logInfo("COMPortName:" + cs2s(comPortInfo));
			int comIndex = comPortInfo.MakeUpper().Find(_T("(COM")) + 1;
			for (int j = 0; j < comdevice_names.size(); j++)
			{
				logInfo("com_device_name[" + to_string(j) + "]:" + cs2s(comdevice_names[j]));
				int deviceIndex = comPortInfo.MakeUpper().Find(comdevice_names[j].MakeUpper());
				if (comIndex > -1 && deviceIndex > -1)
				{
					int right = comPortInfo.Find(_T(")"));
					ComPort = CString(bufferPortNum).Mid(comIndex, right - comIndex);
					if (VideoScreenControl::GetInstance()->testComPort(cs2s(ComPort), CAGConfig::GetInstance()->GetPictureDivider()))
					{
						logInfo("Video Screen COM:" + cs2s(ComPort));
						CAGConfig::GetInstance()->SetVideoScreenCom(ComPort);
					}
					else
					{
						logInfo("Device COM:" + cs2s(ComPort));
						CAGConfig::GetInstance()->SetDeviceCom(ComPort);
					}
				}
			}
		}//end if ports
	}// end for	
	if (buffer != nullptr)
	{
		delete[] buffer;
	}
	if (bufferPortNum != nullptr)
	{
		delete[] bufferPortNum;
	}
	// 释放设备  
	SetupDiDestroyDeviceInfoList(hDevInfo);
}

void DUIMgr::updateCOMPortandDisableCamera()
{
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		logWarn("获取系统设备列表失败");
		return;
	}
	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	deviceInfoData.DevInst = 0;
	deviceInfoData.Reserved = 0;


	DWORD regDataType;
	LPTSTR buffer = new TCHAR[200];
	DWORD buffersize = 200;

	LPTSTR bufferPortNum = new TCHAR[200];
	DWORD buffersizePortNum = 200;
	LPTSTR bufferCameraNum = new TCHAR[200];
	DWORD buffersizeCameraNum = 200;

	CString ComPort = L"";
	vector<CString> comdevice_names = CAGConfig::GetInstance()->GetCOMDeviceName();
	vector<CString> excludedCameraNames = CAGConfig::GetInstance()->GetExcludedCameraNames();
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++)
	{
		buffersize = 200;
		SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_CLASS, &regDataType, (PBYTE)buffer, buffersize, &buffersize);
		//logInfo("Device name:" + StringUtil::StringFromLPCTSTR(buffer));
		if (CString(buffer).MakeUpper() == L"PORTS")
		{
			buffersizePortNum = 200;
			SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, &regDataType, (PBYTE)bufferPortNum, buffersizePortNum, &buffersizePortNum);
			CString comPortInfo = CString(bufferPortNum);
			logInfo("COM Name:" + cs2s(comPortInfo));
			int comIndex = comPortInfo.MakeUpper().Find(_T("(COM")) + 1;
			for (int j = 0; j < comdevice_names.size(); j++)
			{
				logInfo("com_device_name[" + to_string(j) + "]:" + cs2s(comdevice_names[j]));
				int deviceIndex = comPortInfo.MakeUpper().Find(comdevice_names[j].MakeUpper());
				if (comIndex > -1 && deviceIndex > -1)
				{
					int right = comPortInfo.Find(_T(")"));
					ComPort = CString(bufferPortNum).Mid(comIndex, right - comIndex);
					if (VideoScreenControl::GetInstance()->testComPort(cs2s(ComPort), CAGConfig::GetInstance()->GetPictureDivider()))
					{
						logInfo("Video Screen COM:" + cs2s(ComPort));
						CAGConfig::GetInstance()->SetVideoScreenCom(ComPort);
					}
					else
					{
						logInfo("Device COM:" + cs2s(ComPort));
						CAGConfig::GetInstance()->SetDeviceCom(ComPort);
					}
				}
			}
			
			
		}//end if ports
		else if (CString(buffer).MakeUpper() == L"CAMERA")
		{
			LPTSTR bufferCameraNum = new TCHAR[200];
			DWORD buffersizeCameraNum = 200;
			SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, &regDataType, (PBYTE)bufferCameraNum, buffersizeCameraNum, &buffersizeCameraNum);
			CString camerName = CString(bufferCameraNum);
			bool contains = false;
			for (int j = 0; j < excludedCameraNames.size(); j++)
			{
				if (camerName.Compare(excludedCameraNames[j]) == 0)
				{
					contains = true;
					logInfo("The camera name is " + cs2s(camerName) + ", cannot be disabled.");
					break;
				}
			}
			if (contains)
			{
				continue;
			}
			SP_PROPCHANGE_PARAMS propChange;
			propChange.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
			propChange.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
			propChange.Scope = DICS_FLAG_GLOBAL;
			propChange.StateChange = DICS_DISABLE;
			propChange.HwProfile = 0;

			if (SetupDiSetClassInstallParams(hDevInfo, &deviceInfoData, (SP_CLASSINSTALL_HEADER*)&propChange, sizeof(propChange)))
			{
				if (!SetupDiChangeState(hDevInfo, &deviceInfoData))
				{
					logError("Change Device ERR! " + cs2s(camerName) + " is disabled unsuccessfully.");
					loggedIn_ui->setCameraDisabled(false);
				}
				else
				{
					logInfo(cs2s(camerName) + " is disabled successfully.");
					loggedIn_ui->setCameraDisabled(true);
				}
			}
			else
			{
				logError("SetupDiSetClassInstallParams ERR! " + cs2s(camerName) + " is disabled unsuccessfully.");
				loggedIn_ui->setCameraDisabled(false);
			}
		}
	}// end for	
	if (buffer != nullptr)
	{
		delete[] buffer;
	}
	if (bufferPortNum != nullptr)
	{
		delete[] bufferPortNum;
	}
	if (bufferCameraNum != nullptr)
	{
		delete[] bufferCameraNum;
	}
	// 释放设备  
	SetupDiDestroyDeviceInfoList(hDevInfo);	
	if (CAGConfig::GetInstance()->GetDeviceCom().IsEmpty())
	{
		BOOL isRCEnable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
		if (isRCEnable)
		{
			logWarn("Can not get COM Port Number, please make sure the USB cable is connected between host and the Medical device!");
			//::MessageBox(NULL, L"Can not get COM Port Number, please make sure the USB cable is connected between host and the Medical device!", L"Warn", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
		}	
	}
	if (!loggedIn_ui->isCameraDisabled())
	{
		logError("Local Camera is not disabled!");
		//::MessageBox(NULL, L"Local Camera is enable, please disable the local camera manually!", L"Warn", MB_OK| MB_SYSTEMMODAL| MB_ICONWARNING);
	}
}