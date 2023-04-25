#pragma once
#include <string>
#include <vector>
#include "atlstr.h"
#include <locale>
#include <codecvt>
#include "client_common.h"
using namespace std;

class StringUtil
{
public:
	static wstring utf8string2wstring(const std::string& str);
	static string wstring2utf8string(const std::wstring& str);
	static string utf8_to_ansi(string strUTF8);
	static string WStringToString(const wstring &wstr);
	static wstring StringToWstring(string &str);
	static vector<std::wstring> Splitw(const wstring& str, const wstring& delim);
	static vector<std::string> Split(const string& str, const string& delim);
	static void StringReplace(string &s1, const string &s2, const string &s3);
	static void WstringReplace(wstring &s1, const wstring &s2, const wstring &s3);
	static string StringTrim(const char* str);
	static wstring CStringToWString(CString &cstr);
	static string getClientStatusName(UserStatus status);
	static string StringFromLPCTSTR(LPCTSTR str);
};

