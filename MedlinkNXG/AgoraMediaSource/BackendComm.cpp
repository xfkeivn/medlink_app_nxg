#include "rapidjson/document.h"
#include "BackendComm.h"
#include "ConfigCenter.h"
#include "CurlHttpClient.h"
#include "log_util.h"
#include "uuid.h"
#include "HospitalsInfo.h"
#include "clients_manager.h"
#include "client_common.h"
#include "utility_socket.h"

//#include "websocketpp/client.hpp"
//#include "websocketpp/config/asio_no_tls_client.hpp"
//#include <iostream>
//#include <string>
//#include "log_util.h"
//#include "websocketpp/common/thread.hpp"
//#include "websocketpp/common/memory.hpp"

shared_ptr< BackendComm> BackendCommImpl::backend_common_impl = nullptr;
shared_ptr< BackendComm> BackendCommImpl::Instance() {
	if (backend_common_impl == nullptr)
	{
		backend_common_impl = shared_ptr<BackendComm>(new BackendCommImpl());
	}
	return backend_common_impl;
}
BackendCommImpl::~BackendCommImpl()
{
	delete endpoint;
}
BackendCommImpl::BackendCommImpl()
{
	endpoint = new websocket_endpoint();
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
bool BackendCommImpl::requestMeetingAccount(string &user_id, string &channel_name,string &errorsting)
{
	string uuid = UUIDGenerator::getInstance()->getUUID();
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string hospital = StringUtil::wstring2utf8string(RegConfig::Instance()->getHospitalName());//CAGConfig::GetInstance()->GetHospitalId();
	string equipment = StringUtil::wstring2utf8string(RegConfig::Instance()->getEquipmentTypeName());// CAGConfig::GetInstance()->GetEquipmentId();
	string department = StringUtil::wstring2utf8string(RegConfig::Instance()->getDepartmentName());//CAGConfig::GetInstance()->GetDepartmentId();
	string  c_ip_str = ip + ":" + port;

	string url = "http://" + c_ip_str + "/api-meeting/RequestMeetingAccount/UUID/" +  uuid + "?hospital=" + hospital + "&equipment=" + equipment + "&department=" + department;
	//string url = "http://" + ip_str + "/api-meeting/RequestMeetingAccount/UUID/cb253764-ef3c-4c2c-954b-52c5a53a294f";
	
	string response = CurlHttpClient::SendGetReq(url.c_str());
	return handleMeetingAccount(response,user_id,channel_name, errorsting);
	
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
bool BackendCommImpl::requestHostClients(ClientsManager *mgr, vector<Individual*>& clients)
{
	logInfo("Host request clients infos from webserver.");
	string uuid = UUIDGenerator::getInstance()->getUUID();
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;

	string url = "http://" + ip_str + "/api-meeting/RequestHostClients/ServiceClients/" + uuid;
	logInfo("Request url:" + url);
	//string url = "http://" + ip_str +"/api-meeting/RequestHostClients/ServiceClients/cb253764-ef3c-4c2c-954b-52c5a53a294f";
	string response = CurlHttpClient::SendGetReq(url.c_str());

	clients = mgr->getClients(response);
	return true;
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
bool BackendCommImpl::handleMeetingAccount(string rsp, string &userid, string &channelname, string& error_string)
{
	rapidjson::Document doc;
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		string log = "Receive host meeting account response:";
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Host") && doc["Host"].IsObject())
				{
					const rapidjson::Value& host = doc["Host"];
					if (host.HasMember("is_enable") && host["is_enable"].IsBool())
					{
						bool is_enable = host["is_enable"].GetBool();
						if (!is_enable)
						{
							logInfo(log + "is_enable=false, Current device is not available");
							error_string = "Current device is not available, please contact the administrator.";
							return false;
						}
					}
					
					
					if (host.HasMember("id") && host["id"].IsInt())
					{
						userid = to_string(host["id"].GetInt());
						log = log + " id=" + userid;
					}
					if (host.HasMember("channel") && host["channel"].IsObject())
					{
						const rapidjson::Value& channel = host["channel"];
						if (channel.HasMember("channel_id") && channel["channel_id"].IsString())
						{
							channelname = channel["channel_id"].GetString();
							log = log + " channel_id=" + channelname;
						}
					}
					if (host.HasMember("hospital") && host["hospital"].IsObject())
					{
						const rapidjson::Value& hospital = host["hospital"];
						int hospital_id = hospital["id"].GetInt();
						string str_id = to_string(hospital_id);
						log = log + " hospital_id=" + str_id;
	
						//CAGConfig::GetInstance()->SetHospitalId(cs_id);
						RegConfig::Instance()->setHospitalId(str_id);
						if (hospital.HasMember("chinese_name") && hospital["chinese_name"].IsString())
						{
							string cn_name = hospital["chinese_name"].GetString();
							log = log + " hospital_name=" + cn_name;
							RegConfig::Instance()->setHospitalName(StringUtil::utf8string2wstring(cn_name));
						
							//CAGConfig::GetInstance()->SetHospitalName(cn_name.c_str());
						}
					}
					if (host.HasMember("department") && host["department"].IsObject())
					{
						const rapidjson::Value& department = host["department"];
						int department_id = department["id"].GetInt();
						string str_id = to_string(department_id);
						log = log + " department_id=" + str_id;
						RegConfig::Instance()->setDepartmentId(str_id);
						if (department.HasMember("chinese_name") && department["chinese_name"].IsString())
						{
							string cn_name =(department["chinese_name"].GetString());
							log = log + " department_name=" + (cn_name);
							
							RegConfig::Instance()->setDepartmentName(StringUtil::utf8string2wstring(cn_name));
						
						}
					}
					if (host.HasMember("equipment") && host["equipment"].IsObject())
					{
						const rapidjson::Value& equipment = host["equipment"];
						int equipment_id = equipment["id"].GetInt();
						string str_id = to_string(equipment_id);
						log = log + " equipment_id=" + str_id;
						RegConfig::Instance()->setEquipmentId(str_id);
						if (equipment.HasMember("name") && equipment["name"].IsString())
						{
							string name = equipment["name"].GetString();
							log = log + " equipment_name=" + name;
							RegConfig::Instance()->setEquipmentTypeName(StringUtil::utf8string2wstring(name));
							CString cs_name = name.c_str();
							if (cs_name.MakeUpper() == L"ILAB" || cs_name.MakeUpper() == L"RHYTHMIA")
							{
								RegConfig::Instance()->setIsRelativeMouse(true);
							}
							else
							{
								//"Polaris"
								RegConfig::Instance()->setIsRelativeMouse(false);
							}
						}
					}
					logInfo(log);
					
				}
			}
			else
			{
				if (doc.HasMember("Error") && doc["Error"].IsString())
				{
					string error = doc["Error"].GetString();
					logError(error);
					error_string = error;
				}
			}
		}
	}
	else
	{
		logError("Error in parsing host meeting account response:" + rsp);
		error_string = "Can't get host messages from webserver.";
	}
	return true;

}

bool BackendCommImpl::requestHostInitialInfos(HospitalMgr &hospitalMgr)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;
	
	string url = "http://" + ip_str + "/api-meeting/getHostInitialInfo";
	logInfo("Request url: " + url);
	
	string response = CurlHttpClient::SendGetReq(url.c_str());
	return handleHospitalsInfo(hospitalMgr, response);
}

bool BackendCommImpl::handleHospitalsInfo (HospitalMgr &hospitalMgr,string &rsp)
{
	hospitalMgr.clear();
	rapidjson::Document doc;
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		logInfo("Receive hospital infos from webserver:" + rsp);
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Info") && doc["Info"].IsObject())
				{
					const rapidjson::Value& info = doc["Info"];
					if (info.HasMember("Hospitals") && info["Hospitals"].IsArray())
					{
						const rapidjson::Value& array = info["Hospitals"];
						size_t len = array.Size();
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& hospital = array[i];
							if (hospital.IsObject())
							{
								string en_name = "";
								wstring cn_name = L"";
								if (hospital.HasMember("english_name") && hospital["english_name"].IsString())
								{
									en_name = hospital["english_name"].GetString();
								}
								if (hospital.HasMember("chinese_name") && hospital["chinese_name"].IsString())
								{
									cn_name = StringUtil::utf8string2wstring(hospital["chinese_name"].GetString());
								}
								Hospital p_hospital(hospital["id"].GetInt(), cn_name, en_name);
								
								hospitalMgr.getHospitals().push_back(p_hospital);
							}
						}
					}
					if (info.HasMember("Departments") && info["Departments"].IsArray())
					{
						const rapidjson::Value& array = info["Departments"];
						size_t len = array.Size();
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& department = array[i];
							if (department.IsObject())
							{
								string en_name = "";
								wstring cn_name = L"";
								if (department.HasMember("english_name") && department["english_name"].IsString())
								{
									en_name = department["english_name"].GetString();
								}
								if (department.HasMember("chinese_name") && department["chinese_name"].IsString())
								{
									cn_name = StringUtil::utf8string2wstring(department["chinese_name"].GetString());
								}
								Department p_department(department["id"].GetInt(), cn_name, en_name);
								hospitalMgr.getDepartments().push_back(p_department);
							}
						}
					}
					if (info.HasMember("Equipments") && info["Equipments"].IsArray())
					{
						const rapidjson::Value& array = info["Equipments"];
						size_t len = array.Size();
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& equipment = array[i];
							if (equipment.IsObject())
							{
								string name = "";
								if (equipment.HasMember("name") && equipment["name"].IsString())
								{
									name = equipment["name"].GetString();
								}
								Equipment p_equipment(equipment["id"].GetInt(), name);
								hospitalMgr.getEquipments().push_back(p_equipment);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		logError("Error in parsing hospital infos from webserver");
		return false;
	}
	return true;
}

bool BackendCommImpl::clientAuth(string username, string password, USER_INFO_STRUCT &userInfoStruct,string &error_string)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;
	
	string url = "http://" + ip_str + "/api-meeting/RequestLogin/ClientLogin?user=" + username + "&pwd=" + password;
	string rsp = CurlHttpClient::SendGetReq(url.c_str());
	//logInfo("Request url:" + url);
	printf("handleHttpLoginRes: %s", rsp);
	rapidjson::Document doc;
	logInfo("Receive handleHttpLoginRes:" + rsp);
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			string log = "Client login response:";
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Client") && doc["Client"].IsObject())
				{
					const rapidjson::Value& client = doc["Client"];
					if (client.HasMember("id") && client["id"].IsInt())
					{
						userInfoStruct.uid = to_string(client["id"].GetInt());
						log = log + "id=" + userInfoStruct.uid;
					}
					if (client.HasMember("display_name") && client["display_name"].IsString())
					{
						wstring wname = StringUtil::utf8string2wstring(client["display_name"].GetString());
						userInfoStruct.display_name = wname;
						log = log + " display_name=" + StringUtil::WStringToString(wname);
					}
					if (client.HasMember("host_list") && client["host_list"].IsArray())
					{
						const rapidjson::Value& array = client["host_list"];
						size_t len = array.Size();
						log = log + " host_ids{";
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& object = array[i];
							if (object.IsObject())
							{
								userInfoStruct.hostids[i] = object["id"].GetInt();
								log = log + " " + to_string(userInfoStruct.hostids[i]);
							}
						}
						log = log + "}";
						userInfoStruct.hostsize = len;
					}
					logInfo(log);
				}
				
				
				return true;
			}
			else //result is False
			{
				CHAR szlog[MAX_PATH] = { 0 };
				if (doc.HasMember("Error") && doc["Error"].IsString())
				{
					std::string err = doc["Error"].GetString();
					logInfo("handleHttpLoginRes Failed. Error:" + err);
					//sprintf_s(szlog, "handleHttpLoginRes Failed: Error=%s \n", err);
					//OutputDebugStringA(szlog);
				}
				else
				{
					logInfo("handleHttpLoginRes Failed: Error is unknown!");
					sprintf_s(szlog, "handleHttpLoginRes Failed: Error is unknown!\n");
					OutputDebugStringA(szlog);
				}

				//UpdateConfig(true);
				error_string = "Login failed! Please check the name and password.";
				
			}
		}
		else
		{
			logError("Error in parsing client login response:" + rsp);
			
			
		}
	}
	return false;
}

bool BackendCommImpl::requestParticientInfo(string uid, ClientsManager *mgr,Individual *participant)
{
	string ip = RegConfig::Instance()->getWebServerIP();
	string port = RegConfig::Instance()->getWebServerPort();
	string ip_str = ip + ":" + port;
	
	string url = "";

	int id = atoi(uid.c_str());
	if (id >= 1000)
	{
		url = "http://" + ip_str + "/api-meeting/getHost/" + uid;
	}
	else
	{
		url = "http://" + ip_str + "/api-meeting/getClient/" + uid;
	}
	string response = CurlHttpClient::SendGetReq(url.c_str());

	participant = mgr->updateParticipant(response);
	return true;
}

bool BackendCommImpl::websocketConnect()
{
	string websocket_uri = RegConfig::Instance()->getWebSocketURI();
	m_websocket_id = endpoint->connect(websocket_uri);
	
	
	return true;
}
string BackendCommImpl::websocketGetStatus()
{
	return endpoint->get_current_status();
}
bool BackendCommImpl::websocketClose()
{
	endpoint->close(m_websocket_id, websocketpp::close::status::normal, "Meeting is End.");
	return true;
}
bool BackendCommImpl::websocketSendHeartBeat(string meeting_id)
{
	endpoint->send(m_websocket_id, meeting_id);
	return true;
}
