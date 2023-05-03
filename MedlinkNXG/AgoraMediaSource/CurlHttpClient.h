#pragma once
#include "../ThirdParty/libcurl/include/curl.h"
#include <sstream>
#pragma comment(lib,"../ThirdParty/libcurl/lib/libcurl.lib")
using namespace std;

class CurlHttpClient
{
public:
	CurlHttpClient() {}
	~CurlHttpClient() {}
	static string SendGetReq(const char* url);
	static string SendPostReq(const char* url, const char* param);
};

