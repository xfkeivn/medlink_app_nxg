#pragma once
#include <string>
#include <functional>
#include "mongoose.h"
using namespace std;
using ReqCallback = function<void(string, void*)>;

class HttpClient
{
public:
	HttpClient() {}
	~HttpClient() {}

	static void SendReq(const string &url, const char* req_param,ReqCallback req_callback, void* pParam);
	static void OnHttpEvent(mg_connection *connection, int event_type, void *event_data);
	static int s_exit_flag;
	static ReqCallback s_req_callback;
	static void* p;
};
