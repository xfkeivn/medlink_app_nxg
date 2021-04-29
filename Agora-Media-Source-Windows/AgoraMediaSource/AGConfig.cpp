#include "stdafx.h"
#include "AGConfig.h"

CAGConfig* CAGConfig::m_agConfigInstance = nullptr;

CAGConfig::CAGConfig()
{
	::GetModuleFileName(NULL, m_szConfigFile.GetBuffer(MAX_PATH), MAX_PATH);
	m_szConfigFile.ReleaseBuffer();
	
	 int pos = m_szConfigFile.Find(APP_NAME);
	 CString newstr = m_szConfigFile.Left(pos);
	 newstr += _T("MedLinkConfig.ini");
	 m_szConfigFile = newstr;
	if (::GetFileAttributes(m_szConfigFile) == INVALID_FILE_ATTRIBUTES) {
		HANDLE hFile = ::CreateFile(m_szConfigFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		::CloseHandle(hFile);
	}
}

CAGConfig* CAGConfig::GetInstance()
{
	if (m_agConfigInstance == nullptr)
		m_agConfigInstance = new CAGConfig();

	return m_agConfigInstance;
}

CAGConfig::~CAGConfig()
{
}

int CAGConfig::GetCodec()
{
#ifdef ENABLE_CODEC
	CString strCodec;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("CodecIndex"), _T("1"), strCodec.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	strCodec.ReleaseBuffer();

	return _ttoi(strCodec);
#else
	return 0;
#endif
}

BOOL CAGConfig::SetCodec(int nCodec)
{
#ifdef ENABLE_CODEC
	CString strCodec;

	strCodec.Format(_T("%d"), nCodec);

	return ::WritePrivateProfileString(_T("MedLinkConfig"), _T("CodecIndex"), strCodec, m_szConfigFile);
#else
	return TRUE;
#endif
}
void CAGConfig::SetVideoScreenCom(CString com)
{
	::WritePrivateProfileString(_T("MedLinkConfig"), _T("VideoScreenCom"), com.GetBuffer(0), m_szConfigFile);
}

CString CAGConfig::GetVideoScreenCom()
{
	CString comport;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("VideoScreenCom"), _T("COM1"), comport.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	comport.ReleaseBuffer();

	return comport;
}

void CAGConfig::SetDeviceCom(CString com)
{
	::WritePrivateProfileString(_T("MedLinkConfig"), _T("DeviceCom"), com.GetBuffer(0), m_szConfigFile);
}

CString CAGConfig::GetDeviceCom()
{
	CString comport;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("DeviceCom"), _T("COM1"), comport.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	comport.ReleaseBuffer();

	return comport;
}
int CAGConfig::GetBaudrate()
{
	CString baudrate;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("HIDBaudrate"), _T("9600"), baudrate.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	baudrate.ReleaseBuffer();

	return _ttoi(baudrate);
}

int CAGConfig::GetSolution()
{
    CString strResolution;

    ::GetPrivateProfileString(_T("MedLinkConfig"), _T("SolutionIndex"), _T("1"), strResolution.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

    strResolution.ReleaseBuffer();

    return _ttoi(strResolution);
}

BOOL CAGConfig::SetSolution(int nResolution)
{
    CString strResolution;

    strResolution.Format(_T("%d"), nResolution);

    return ::WritePrivateProfileString(_T("MedLinkConfig"), _T("SolutionIndex"), strResolution, m_szConfigFile);
}

BOOL CAGConfig::EnableAutoSave(BOOL bEnable)
{
	CString strSave;

	if (bEnable)
		strSave = _T("1");
	else
		strSave = _T("0");

	return ::WritePrivateProfileString(_T("MedLinkConfig"), _T("SaveSetting"), strSave, m_szConfigFile);
}

BOOL CAGConfig::IsAutoSaveEnabled()
{
	CString strSaveSetting;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("SaveSetting"), _T("0"), strSaveSetting.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	strSaveSetting.ReleaseBuffer();

	return (_ttoi(strSaveSetting) == 1) ? TRUE : FALSE;
}



CString CAGConfig::GetRememberMe()
{
	CString rememberMe;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("RememberMe"), _T(""), rememberMe.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	rememberMe.ReleaseBuffer();

	return rememberMe;
}

void CAGConfig::SetRememberMe(CString isRememberMe)
{
	::WritePrivateProfileString(_T("MedLinkConfig"), _T("RememberMe"), isRememberMe.GetBuffer(0), m_szConfigFile);
}


double CAGConfig::GetRate()
{
	CString rate_cstr;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("Rate"), _T(""), rate_cstr.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	rate_cstr.ReleaseBuffer();
	string rate_str = CT2A(rate_cstr.GetBuffer());
	double rate = 1;
	rate = atof(rate_str.c_str());
	return rate;
}

CString CAGConfig::GetVersion()
{
	CString version_str;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("Version"), _T(""), version_str.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	version_str.ReleaseBuffer();

	return version_str;
}

BOOL CAGConfig::isRemoteMouseShow()
{
	CString strIsRemoteMouseShow;
	BOOL isRemoteMouseShow;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("RemoteMouseShow"), _T(""), strIsRemoteMouseShow.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);
	strIsRemoteMouseShow.ReleaseBuffer();
	if (strIsRemoteMouseShow.Compare(_T("True")) == 0)
	{
		isRemoteMouseShow = TRUE;
	}
	else
	{
		isRemoteMouseShow = FALSE;
	}
	return isRemoteMouseShow;
}


int CAGConfig::GetTargetWinWidth()
{
	CString cstrTargetWinWidth;
	int targetWinWidth;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("TargetWinWidth"), _T(""), cstrTargetWinWidth.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);
	cstrTargetWinWidth.ReleaseBuffer();
	string strTargetWinWidth = CT2A(cstrTargetWinWidth.GetBuffer());
	int width = stoi(strTargetWinWidth);
	return width;
}

int CAGConfig::GetTargetWinHeight()
{
	CString cstrTargetWinHeight;
	int targetWinHeight;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("TargetWinHeight"), _T(""), cstrTargetWinHeight.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);	
	cstrTargetWinHeight.ReleaseBuffer();
	string strTargetWinHeight = CT2A(cstrTargetWinHeight.GetBuffer());
	int height = stoi(strTargetWinHeight);
	return height;
}

int CAGConfig::GetPictureDivider()
{
	CString cstrPictureDivider;
	int pictureDivider;
	::GetPrivateProfileString(_T("MedLinkConfig"), _T("PictureDivider"), _T("0"), cstrPictureDivider.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);
	cstrPictureDivider.ReleaseBuffer();
	string strPictureDivider = CT2A(cstrPictureDivider.GetBuffer());
	pictureDivider = stoi(strPictureDivider);
	return pictureDivider;
}
vector<CString> CAGConfig::GetCOMDeviceName()
{
	CString comdevice_name;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("COMDeviceName"), _T(""), comdevice_name.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);
	comdevice_name.ReleaseBuffer();
	vector<CString> comdevice_array;
	int nPos_left = comdevice_name.Find(L"{");
	int nPos_right = comdevice_name.Find(L"}");
	CString device = L"";
	while (nPos_left >= 0 && nPos_right >= 0)
	{
		device = comdevice_name.Mid(nPos_left + 1, nPos_right - nPos_left - 1);
		if (!device.IsEmpty())
		{
			comdevice_array.push_back(device);
		}
		comdevice_name = comdevice_name.Right(comdevice_name.GetLength() - nPos_right - 1);
		nPos_left = comdevice_name.Find(L"{");
		nPos_right = comdevice_name.Find(L"}");
	}
	return comdevice_array;
}

vector<CString> CAGConfig::GetExcludedCameraNames()
{
	CString excludedCameraNamestr;

	::GetPrivateProfileString(_T("MedLinkConfig"), _T("ExcludedCameraNames"), _T(""), excludedCameraNamestr.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);
	excludedCameraNamestr.ReleaseBuffer();
	vector<CString> excludedCameraNames;
	int nPos_left = excludedCameraNamestr.Find(L"{");
	int nPos_right = excludedCameraNamestr.Find(L"}");
	CString camera = L"";
	while (nPos_left >= 0 && nPos_right >= 0)
	{
		camera = excludedCameraNamestr.Mid(nPos_left + 1, nPos_right - nPos_left - 1);
		if (!camera.IsEmpty())
		{
			excludedCameraNames.push_back(camera);
		}
		excludedCameraNamestr = excludedCameraNamestr.Right(excludedCameraNamestr.GetLength() - nPos_right - 1);
		nPos_left = excludedCameraNamestr.Find(L"{");
		nPos_right = excludedCameraNamestr.Find(L"}");
	}
	return excludedCameraNames;
}