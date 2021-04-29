#include "stdafx.h"
#include "commonFun.h"
#include "FileIO.h"
std::string getAbsoluteDir()
{
	TCHAR path[MAXPATHLEN] = { 0 };
	GetModuleFileName(nullptr, path, MAXPATHLEN);

	std::string filePath = CStringA(path).GetBuffer();
	return filePath.substr(0, filePath.rfind("\\") + 1);
}

std::string getFilePath()
{
	TCHAR path[MAXPATHLEN] = { 0 };
	GetModuleFileName(nullptr, path, MAXPATHLEN);
	return CStringA(path).GetBuffer();
}

std::string getCurRunningExeName()
{
	TCHAR path[MAXPATHLEN] = { 0 };
	GetModuleFileName(nullptr, path, MAXPATHLEN);

	std::string filePath = CStringA(path).GetBuffer();
	return filePath.substr(filePath.rfind("\\") + 1, filePath.length() - filePath.rfind("\\"));
}

std::string getFileAbsolutePath(const std::string &file)
{
	HMODULE hModule = GetModuleHandle(CString(file.c_str()));
	TCHAR path[MAXPATHLEN] = { 0 };
	GetModuleFileName(hModule, path, MAXPATHLEN);
	return CStringA(path).GetBuffer();
}

std::string getPirorDir(const std::string &file)
{
	HMODULE hModule = GetModuleHandle(CString(file.c_str()));
	TCHAR path[MAXPATHLEN] = { 0 };
	GetModuleFileName(hModule, path, MAXPATHLEN);
	std::string fullpath = CStringA(path).GetBuffer();
	return fullpath.substr(0, fullpath.rfind("\\") + 1);
}

std::string getRootDir(const std::string &file)
{
	std::string FileDir = getFileAbsolutePath(file);
	return FileDir.substr(0, FileDir.find("\\") + 1);
}

std::string int2str(int nNum)
{
	char str[MAXPATHLEN] = { 0 };
	_itoa_s(nNum, str, 10);
	return str;
}

std::string float2str(float fValue)
{
	char str[MAXPATHLEN] = { 0 };
	sprintf_s(str, "%f", fValue);
	return str;
}

int str2int(const std::string &str)
{
	return atoi(str.c_str());
}

float str2float(const std::string &str)
{
	return (float)atof(str.c_str());
}

CString s2cs(const std::string &str)
{
	return CString(str.c_str());
}

std::string cs2Utf8(const CString & str)
{
    char szFile[MAX_PATH] = { 0 };
    WideCharToMultiByte(CP_UTF8, 0, str, str.GetLength(), szFile, MAX_PATH, 0, 0);
   return szFile;
}

CString utf82cs(std::string str)
{
    TCHAR szFile[MAX_PATH] = { 0 };
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), szFile, MAX_PATH);
    return szFile;
}

std::string cs2s(const CString &str)
{
	CString sTemp(str);
	return CStringA(sTemp.GetBuffer()).GetBuffer();
}

std::string utf82gbk(const char *utf8)
{
	std::string str;

	if (utf8 != NULL)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		std::wstring strW;

		strW.resize(len);

		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (LPWSTR)strW.data(), len);

		len = WideCharToMultiByte(936, 0, strW.data(), len - 1, NULL, 0, NULL, NULL);	

		str.resize(len);

		WideCharToMultiByte(936, 0, strW.data(), -1, (LPSTR)str.data(), len, NULL, NULL);
	}

	return str;
}

std::string gbk2utf8(const char *gbk)
{
	std::string str;

	if (gbk != NULL)
	{
		int len = MultiByteToWideChar(936, 0, gbk, -1, NULL, 0);
		std::wstring strW;

		strW.resize(len);

		MultiByteToWideChar(936, 0, gbk, -1, (LPWSTR)strW.data(), len);

		len = WideCharToMultiByte(CP_UTF8, 0, strW.data(), len - 1, NULL, 0, NULL, NULL);

		str.resize(len);

		WideCharToMultiByte(CP_UTF8, 0, strW.data(), -1, (LPSTR)str.data(), len, NULL, NULL);
	}

	return str;
}

std::string gbk2utf8(const std::string &gbk)
{
	return gbk2utf8(gbk.c_str());
}

std::string utf82gbk(const std::string &utf8)
{
	return utf82gbk(utf8.c_str());
}

bool isContailSpace(CString str)
{
	return str.Find(_T(' ')) > 0;
}

void initlog()
{
	std::string strLogPath = getAbsoluteDir() + "AgoraSignalSDK.log";

	gLogSignal.openLog(strLogPath);
}

void uninitLog()
{
	gLogSignal.close();
}

void LOG_MSG(const std::string &msgInfo, AG_SIGNAL_LOGTYPE eLogType /*= LogType_SDK_Error*/)
{
	char LogDesc[MAXPATHLEN] = { '\0' };
	SYSTEMTIME st;
	GetLocalTime(&st);

	switch (eLogType)
	{
	case AG_SIGNAL_LOGTYPE::LogType_SDK_Error:
	{
		sprintf_s(LogDesc, "[SDK_ERROR] %02d:%02d:%02d:%03d ; %s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, msgInfo.data());
	}
	break;
	case AG_SIGNAL_LOGTYPE::LogType_SDK_LOG:
	{
		sprintf_s(LogDesc, "[SDK_LOG] %02d:%02d:%02d:%03d ; %s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, msgInfo.data());
	}
	break;;
	case AG_SIGNAL_LOGTYPE::LogType_API_Operator:
	{
		sprintf_s(LogDesc, "[SDK_API] %02d:%02d:%02d:%03d ; %s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, msgInfo.data());
	}
	break;
	case AG_SIGNAL_LOGTYPE::LogType_CallBack:
	{
		sprintf_s(LogDesc, "[SDK_CALLBACK] %02d:%02d:%02d:%03d ; %s", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, msgInfo.data());
	}
	break;;
	default:break;
	}

	if (strlen(LogDesc)){
		gLogSignal.write(LogDesc);
	}
}

int getResolutionHeight(int height)
{	
	int y = ::GetSystemMetrics(SM_CYSCREEN);
	return height * y / MAX_HEIGHT;
}

int getResolutionWidth(int width)
{
	int x = ::GetSystemMetrics(SM_CXSCREEN);
	return width * x / MAX_WIDTH;
}

int getResolutionSize(int size)
{
	int width = getResolutionWidth(size);
	int height = getResolutionHeight(size);
	if (width < height)
	{
		return width;
	}
	else
	{
		return height;
	}
}
int getResolutionFontSize(int size)
{
	int x = ::GetSystemMetrics(SM_CXSCREEN);
	int y = ::GetSystemMetrics(SM_CYSCREEN);
	float ratioWidth = x / (float)MAX_WIDTH;
	float rationHeight = x / (float)MAX_HEIGHT;
	float ratio = 0;
	if (ratioWidth < rationHeight)
	{
		ratio = ratioWidth;
	}
	else
	{
		ratio = rationHeight;
	}
	int fontsize = size * ratio;
	return fontsize;
}

long long getTimeStamp()
{
	timeb t;
	ftime(&t);
	return t.time * 1000 + t.millitm;
}

int r(double a)
{
	int b;
	if (a > 0)
	{
		b = (a * 2 + 1) / 2;
	}
	else
	{
		b = (a * 2 - 1) / 2;
	}
	return b;
}

bool isNetworkConnected()
{
	DWORD   flag;
	bool    con = InternetGetConnectedState(&flag, 0);
	return con;
}

void writeRegKey(CString key, CString value, CString dir)
{
	HKEY hKey = nullptr;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		//创建成功,将得到hKey,一个注册表句柄,用于下面操作注册表
		if (ERROR_SUCCESS != RegCreateKey(HKEY_LOCAL_MACHINE, dir, &hKey))
		{
			logError("RegCreateKey failed");
			return;
		}
	}
	LONG lRet = 0;
	//这个函数可以写入更多的Value   
	if (ERROR_SUCCESS != (lRet = RegSetValueEx(hKey, key, 0, REG_SZ, (BYTE*)value.GetBuffer(value.GetLength()), 100)))
	{
		logError("RegSetValueEx \"" + cs2s(key)+"=" + cs2s(value) +"\" failed");
		return;
	}
	//logInfo("writeRegKey successfully");
	RegCloseKey(hKey);
}

CString readRegKey(CString key, CString dir)
{
	HKEY hKey = nullptr;
	DWORD dwTpye = REG_SZ;
	DWORD dwSize = MAX_PATH;
	wchar_t data[MAX_PATH];
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		long ret = RegQueryValueEx(hKey, key, 0, &dwTpye, (LPBYTE)data, &dwSize);
		if (ret == ERROR_SUCCESS)
		{
			return CString(data);
		}
		else
		{
			logError("RegQueryValueEx key:" + cs2s(key) + " failed, ret=" + to_string(ret));
		}
	}
	else
	{
		logError("RegOpenKeyEx failed");
	}
	return L"";
}

BOOL CString2BOOL(CString str)
{	
	if (str.Compare(_T("True")) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}