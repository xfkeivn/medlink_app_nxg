#include "stdafx.h"
#include "httpclient.h"

int HttpClient::s_exit_flag = 0;
ReqCallback HttpClient::s_req_callback;
void* HttpClient::p;

void HttpClient::OnHttpEvent(mg_connection *connection, int event_type, void *event_data)
{
	http_message *hm = (struct http_message *)event_data;
	int connect_status;

	switch (event_type)
	{
	case MG_EV_CONNECT:
		connect_status = *(int *)event_data;
		if (connect_status != 0)
		{
			printf("Error connecting to server, error code: %d\n", connect_status);
			s_exit_flag = 1;
		}
		break;
	case MG_EV_HTTP_REPLY:
	{
		printf("Got reply:\n%.*s\n", (int)hm->body.len, hm->body.p);
		std::string rsp = std::string(hm->body.p, hm->body.len);
		connection->flags |= MG_F_SEND_AND_CLOSE;		
		s_req_callback(rsp,p);
		s_exit_flag = 1;
	}
	break;
	case MG_EV_CLOSE:
		if (s_exit_flag == 0)
		{
			printf("Server closed connection\n");
			s_exit_flag = 1;
		};
		break;
	default:
		break;
	}
}

void HttpClient::SendReq(const std::string &url, const char* req_param, ReqCallback req_callback, void* pParam)
{
	s_exit_flag = 0;
	s_req_callback = req_callback;
	p = pParam;
	mg_mgr mgr;
	mg_mgr_init(&mgr, NULL);
	if (req_param == NULL)
	{
		auto connection = mg_connect_http(&mgr, OnHttpEvent, url.c_str(), NULL, req_param);
		mg_set_protocol_http_websocket(connection);
	}
	else
	{
		auto connection = mg_connect_http(&mgr, OnHttpEvent, url.c_str(), "Content-Type: application/x-www-form-urlencoded\r\n", req_param);
		mg_set_protocol_http_websocket(connection);
	}

	CHAR szlog[MAX_PATH] = { 0 };
	sprintf_s(szlog, "Send http request:[%s]  %s\n", req_param, url.c_str());
	OutputDebugStringA(szlog);

	// loop
	while (s_exit_flag == 0)
		mg_mgr_poll(&mgr, 500);

	mg_mgr_free(&mgr);
}