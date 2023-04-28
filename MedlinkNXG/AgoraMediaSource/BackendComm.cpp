#include "rapidjson/document.h"
#include "BackendComm.h"
#include "ConfigCenter.h"
#include "CurlHttpClient.h"
#include "log_util.h"
#include "uuid.h"
shared_ptr< BackendComm> BackendCommImpl::backend_common_impl = nullptr;
shared_ptr< BackendComm> BackendCommImpl::Instance() {
	if (backend_common_impl == nullptr)
	{
		backend_common_impl = shared_ptr<BackendComm>(new BackendCommImpl());
	}
	return backend_common_impl;
}

BackendCommImpl::BackendCommImpl()
{

}
bool BackendCommImpl::requestAppID(string &appid)
{
	//
	appid = "b158865ce08b43f788e909aceda9fb7f";
	return true;


	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	
	string url = "http://" + ip +":"+ port + "/api-meeting/RequestAppID";
	string response = CurlHttpClient::SendGetReq(url.c_str());
	logInfo("Receive request AppId response:" + response);
	if (response.length() > 0)
	{
		string s = "\"";
		string::size_type index = response.find(s);
		while (index != string::npos)
		{
			response.replace(index, 1, "");
			index = response.find(s);
		}
		appid = response;
		return true;
	}
	return false;


}

int BackendCommImpl::reportStartMeeting(string host_id)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string socket_port = RegConfig::Instance()->getWebSocketPort();
	string websocket_uri = "ws://" + ip + ":"+socket_port+ "/ws/keepMeeting";
	string url = "";
	url = "http://" + ip + ":" + port + "/api-meeting/ReportStartMeeting";
	string uuid = UUIDGenerator::getInstance()->getUUID();
	string req_param = "uuid=" + uuid + "&host_id=" + host_id;
	string rsp = CurlHttpClient::SendPostReq(url.c_str(), req_param.c_str());
	rapidjson::Document doc;
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		logInfo("Start meeting, receive meeting infos from webserver:" + rsp);
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Meeting") && doc["Meeting"].IsObject())
				{
					const rapidjson::Value& meeting = doc["Meeting"];
					int id = meeting["id"].GetInt();
					logInfo("meeting id is " + to_string(id));
					return id;
				}
			}
		}
	}
	return -1;
}

bool BackendCommImpl::reportEndMeeting(string meeting_id)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;	
	string url = "http://" + ip_str + "/api-meeting/ReportEndMeeting/MeetingID/" + meeting_id;
	string response = CurlHttpClient::SendGetReq(url.c_str());
	rapidjson::Document doc;
	if (!doc.Parse(response.data()).HasParseError())
	{
		logInfo("ReportEndMeetingResponse from webserver:" + response);
	}

	return true;
}
bool BackendCommImpl::reportClientJoinMeeting2(string client_id,string meeting_id, string host_id,string channel_id){
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string uuid = UUIDGenerator::getInstance()->getUUID();
	string url = "http://" + ip+":"+port + "/api-meeting/ReportJoinMeeting/ClientJoinMeeting";
	string req_param = "";
	if (meeting_id.length() > 0)
	{
		req_param = "client_id=" + client_id + "&meeting_id=" + meeting_id + "&host_id=" + host_id + "&uuid=" + uuid + "&is_init=False&channel_id=" + channel_id;
	}
	else
	{
		req_param = "client_id=" + client_id + "&host_id=" + host_id + "&uuid=" + uuid + "&is_init=True&channel_id=" + channel_id;
	}
	string response = CurlHttpClient::SendPostReq(url.c_str(), req_param.c_str());
	rapidjson::Document doc;
	if (!doc.Parse(response.data()).HasParseError())
	{
		logInfo("ClientJoinMeetingResponse from webserver:" + response);
	}
	return TRUE;
}
bool BackendCommImpl::reportClientJoinMeeting1(string user_id, string channel_id)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;
	string url = "http://" + ip_str + "/api-meeting/ClientActiveLog/ClientJoinMeeting";
	string req_param = "client_id=" + user_id + "&channel_id=" + channel_id;
	string response = CurlHttpClient::SendPostReq(url.c_str(), req_param.c_str());
	rapidjson::Document doc;
	if (!doc.Parse(response.data()).HasParseError())
	{
		logInfo("ClientJoinMeetingResponse from webserver:" + response);
	}
	return true;
}
bool BackendCommImpl::requestMeetingAccount(string uuid, string hospital, string department, string equipment)
{
	return true;
}
bool BackendCommImpl::requestHostInitialInfo()
{
	return true;
}

bool BackendCommImpl::requestServiceClients(string uuid, vector<std::shared_ptr<Individual>> &clients)
{
	return true;
}

bool BackendCommImpl::reportClientExitMeeting(string client_id, string meeting_id, string channel_id)
{

	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;
	
	string url = "http://" + ip_str + "/api-meeting/ReportExitMeeting/ClientExitMeeting";
	string req_param = "client_id=" + client_id + "&meeting_id=" + meeting_id + "&channel_id=" + channel_id;
	string response = CurlHttpClient::SendPostReq(url.c_str(), req_param.c_str());
	rapidjson::Document doc;
	if (!doc.Parse(response.data()).HasParseError())
	{
		logInfo("ClientExitMeetingResponse from webserver:" + response);
	}
	return TRUE;

}
bool  BackendCommImpl::reportActiveLogClientExitMeeting(string client_id, string channel_id)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;

	string url = "http://" + ip_str + "/api-meeting/ClientActiveLog/ClientExitMeeting";
	string req_param = "client_id=" + client_id + "&channel_id=" + channel_id;
	string response = CurlHttpClient::SendPostReq(url.c_str(), req_param.c_str());
	rapidjson::Document doc;
	if (!doc.Parse(response.data()).HasParseError())
	{
		logInfo("ClientExitMeetingResponse from webserver:" + response);
	}
	return TRUE;
}