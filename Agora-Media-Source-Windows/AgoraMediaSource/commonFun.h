#ifndef __COMMONFUN_H__
#define __COMMONFUN_H__
using namespace std;
#include <string>
#include <time.h>
#include <sys/timeb.h>
#include <WinInet.h>
#include "string_util.h"
#include "log_util.h"
std::string getAbsoluteDir();
std::string getFilePath();
std::string getCurRunningExeName();
std::string getFileAbsolutePath(const std::string &file);
std::string getPirorDir(const std::string &file);
std::string getRootDir(const std::string &file);

std::string int2str(int nNum);
std::string float2str(float fValue);
std::string gbk2utf8(const char *gbk);
std::string utf82gbk(const char *utf8);
std::string gbk2utf8(const std::string &gbk);
std::string utf82gbk(const std::string &utf8);
int str2int(const std::string &str);
float str2float(const std::string &str);
CString s2cs(const std::string &str);
std::string cs2s(const CString &str);
std::string cs2Utf8(const CString & str);
CString utf82cs(std::string str);
bool isContailSpace(CString str);
void initlog();
void uninitLog();
int getResolutionHeight(int height);
int getResolutionWidth(int width);
int getResolutionSize(int size);
int getResolutionFontSize(int size);
long long getTimeStamp();
int r(double a);
bool isNetworkConnected();
void split(const string& s, vector<string>& tokens, const char& delim = ' ');
typedef enum eTagLogType
{
	LogType_NULL = -1,
	LogType_SDK_Error,
	LogType_API_Operator,
	LogType_CallBack,
	LogType_SDK_LOG,

	LogType_Unknown = 0xff,
}AG_SIGNAL_LOGTYPE;

void LOG_MSG(const std::string &msgInfo,AG_SIGNAL_LOGTYPE eLogType =  LogType_SDK_Error);
void writeRegKey(CString key, CString value, CString dir);
CString readRegKey(CString key, CString dir);
BOOL CString2BOOL(CString str);
#endif