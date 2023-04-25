#include "stdafx.h"
#include "string_util.h"

string StringUtil::utf8_to_ansi(string strUTF8) {
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strUTF8 = szBuffer;
	delete[]szBuffer;
	delete[]wszBuffer;
	return strUTF8;
}

wstring StringUtil::utf8string2wstring(const std::string& str)
{
	static std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.from_bytes(str);
}

string StringUtil::wstring2utf8string(const std::wstring& str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.to_bytes(str);
}

string StringUtil::WStringToString(const wstring &wstr)
{
	string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	if (len <= 0)return result;
	char* buffer = new char[len + 1];
	if (buffer == NULL)return result;
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

wstring StringUtil::StringToWstring(string &str)
{
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	if (len < 0)return result;
	wchar_t* buffer = new wchar_t[len + 1];
	if (buffer == NULL)return result;
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

void StringUtil::StringReplace(string &s1, const string &s2, const string &s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
}

void StringUtil::WstringReplace(wstring &s1, const wstring &s2, const wstring &s3)
{
	wstring::size_type pos = 0;
	wstring::size_type a = s2.size();
	wstring::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != wstring::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
}

vector<wstring> StringUtil::Splitw(const wstring& str, const wstring& delim)
{
	vector<wstring> splited_str_vector;

	size_t last = 0;
	size_t index = str.find_first_of(delim, last);
	while (index != wstring::npos)
	{
		splited_str_vector.push_back(str.substr(last, index - last));
		last = index + 1;
		index = str.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		splited_str_vector.push_back(str.substr(last, index - last));
	}
	return splited_str_vector;
}

vector<string> StringUtil::Split(const string& str, const string& delim)
{
	vector<string> splited_str_vector;

	size_t last = 0;
	size_t index = str.find_first_of(delim, last);
	while (index != string::npos)
	{
		splited_str_vector.push_back(str.substr(last, index - last));
		last = index + 1;
		index = str.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		splited_str_vector.push_back(str.substr(last, index - last));
	}
	return splited_str_vector;
}

string StringUtil::StringTrim(const char* str)
{
	std::string s = str;

	if (s.compare("") == 0)
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

wstring StringUtil::CStringToWString(CString &cstr)
{
	wstring wstr = cstr.GetString();
	return wstr;
}

string StringUtil::StringFromLPCTSTR(LPCTSTR str) {
	#ifdef _UNICODE
		int size_str = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, NULL, NULL);

		char* point_new_array = new char[size_str];

		WideCharToMultiByte(CP_UTF8, 0, str, -1, point_new_array, size_str, NULL, NULL);

		std::string return_string(point_new_array);
		delete[] point_new_array;
		point_new_array = NULL;
		return return_string;
	#else
		return std::string(str);
	#endif
}

string StringUtil::getClientStatusName(UserStatus status)
{
	switch (status)
	{
	case USER_STATUS_UNKNOWN:
		return "USER_STATUS_UNKNOWN";
		break;
	case USER_STATUS_LOGIN:
		return "USER_STATUS_LOGIN";
		break;
	case USER_STATUS_LOGOUT:
		return "USER_STATUS_LOGOUT";
		break;
	case USER_STATUS_CLOSE:
		return "USER_STATUS_CLOSE";
		break;
	case USER_STATUS_JOIN_MEETING:
		return "USER_STATUS_JOIN_MEETING";
		break;
	case USER_STATUS_ENDMEETING:
		return "USER_STATUS_ENDMEETING";
		break;
	case USER_STATUS_REJECT_MEETING:
		return "USER_STATUS_REJECT_MEETING";
		break;
	case USER_STATUS_TIMEOUT:
		return "USER_STATUS_TIMEOUT";
		break;
	case USER_STATUS_JOIN_MEETING_FAILED:
		return "USER_STATUS_JOIN_MEETING_FAILED";
		break;
	case USER_STATUS_INVITING_PENDING:
		return "USER_STATUS_INVITING_PENDING";
		break;
	case USER_STATUS_JOIN_MEETING_WAITING:
		return "USER_STATUS_JOIN_MEETING_WAITING";
		break;
	default:
		return "Error";
		break;
	}
}