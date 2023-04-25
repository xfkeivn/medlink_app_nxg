#include "stdafx.h"
#include "clients_manager.h"
#include <direct.h>
#include "base64.h"

extern bool CompareWithTitleAse(Individual* client1, Individual* client2);
ClientsManager* ClientsManager::instance_ = NULL;

ClientsManager* ClientsManager::getInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new ClientsManager();
	}
	return instance_;
}

string getProjectPath()
{
	char buffer[64];
	_getcwd(buffer, 64);
	string path = buffer;
	return path;
}

std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

vector<Individual*> ClientsManager::getClients(string rsp)
{
	rapidjson::Document doc;
	wstring wrsp = StringUtil::utf8string2wstring(rsp);
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		string log = "Receive host clients infos:";
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Clients") && doc["Clients"].IsArray())
				{
					const rapidjson::Value& array = doc["Clients"];
					size_t len = array.Size();
					for (size_t i = 0; i < len; i++)
					{
						const rapidjson::Value& object = array[i];
						if (object.IsObject())
						{
							log = log + "client_" + to_string(i) + "{";
							string name = "";
							wstring wname = L"";
							if (object.HasMember("display_name"))
							{
								if (object["display_name"].IsString())
								{
									string s = object["display_name"].GetString();
									wname = StringUtil::utf8string2wstring(s);
									name = StringUtil::WStringToString(wname);
									log = log + "display_name=" + name;
								}
								else
								{
									logError("The value of display_name is not string type.");
								}
							}
							string telephone = "";
							if (object.HasMember("telephone"))
							{
								if (object["telephone"].IsString())
								{
									telephone = object["telephone"].GetString();
									log = log + " telephone=" + telephone;
								}
								else
								{
									logError("The value of telephone is not string type.");
								}
							}
							string user_id = "";
							if (object.HasMember("id"))
							{
								if (object["id"].IsInt())
								{
									user_id = to_string(object["id"].GetInt());
								}
								else
								{
									logError("The value of is is not int type.");
								}
							}
							log = log + " id=" + user_id;
							Individual* individual = new Individual(wname, user_id, telephone);
							if (object.HasMember("icon_round"))
							{
								if (object["icon_round"].IsString())
								{
									string dir = getAbsoluteDir();
									string imagepath = dir + "res\\" + user_id + ".png";
									log = log + " icon_path=" + imagepath;
									string imagecontent = object["icon_round"].GetString();
									WritePhotoFile(s2ws(imagepath), imagecontent);
									individual->setImagePath(imagepath.c_str());
								}
								else
								{
									logError("The value of icon_round is not string type.");
								}
							}
							log = log + "} ";
							individualMap.insert(map<string, Individual*>::value_type(user_id, individual));
							all_clients.push_back(individual);
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
				}
			}

		}
		//if (doc.HasMember("Groups") && doc["Groups"].IsArray())
		//{
		//	const rapidjson::Value& array = doc["Groups"];
		//	size_t len = array.Size();
		//	for (size_t i = 0; i < len; i++)
		//	{
		//		const rapidjson::Value& object = array[i];
		//		if (object.IsObject())
		//		{
		//			string name(object["name"].GetString());
		//			Group* group = new Group(name);
		//			if (object.HasMember("Individuals") && object["Individuals"].IsArray())
		//			{
		//				const rapidjson::Value& individual_array = object["Individuals"];
		//				for (size_t j = 0; j < individual_array.Size(); j++)
		//				{
		//					const rapidjson::Value& individual_object = individual_array[j];
		//					if (individual_object.IsObject())
		//					{
		//						wstring name = StringUtil::utf8string2wstring(individual_object["name"].GetString());
		//						string telephone(individual_object["telephone"].GetString());
		//						string user_id(individual_object["user_id"].GetString());
		//						Individual* individual = new Individual(name, user_id, telephone);
		//						/*if (status == "Online")
		//						{
		//							individual->setStatus(ClientStatus::ONLINE);
		//						}
		//						else if (status == "Offline")
		//						{
		//							individual->setStatus(ClientStatus::OFFLINE);
		//						}
		//						else if (status == "Busy")
		//						{
		//							individual->setStatus(ClientStatus::BUSY);
		//						}	*/							
		//						individualMap.insert(map<string, Individual*>::value_type(user_id, individual));
		//						group->add(individual);
		//					}
		//				}
		//			}
		//			all_clients.push_back(group);
		//		}
		//	}
		//}
	}
	else
	{
		logError("Error in getting host clients response:" + rsp);
		::MessageBox(NULL, L"Error in getting host clients！", L"error", MB_OK);
	}
	sort(all_clients.begin(), all_clients.end(), CompareWithTitleAse);
	return all_clients;
}
// 比较函数，比较字段是名称,升序
bool CompareWithTitleAse(Individual* client1, Individual* client2)
{
	wstring name1 = client1->getWName();
	wstring name2 = client2->getWName();
	if (_tcscmp(name1.c_str(), name2.c_str()) < 0) 
		return true;
	else 
		return false;
}

map<string, Individual*> ClientsManager::getIndividualMap()
{
	return this->individualMap;
}

vector<Individual*> ClientsManager::getInvitedClients()
{
	return this->invited_clients;
}

void ClientsManager::addInvitedClient(Individual* client)
{
	invited_clients.push_back(client);
}

void ClientsManager::clearInvitedClients()
{
	invited_clients.clear();
}

map<string, Individual*> ClientsManager::getInvitationMap()
{
	return this->invitationMap;
}

void ClientsManager::updateInvitedMap(string user_id, Individual* client)
{
	invitationMap.insert(map<string, Individual*>::value_type(user_id, client));
}

bool ClientsManager::isClientInvited(Individual* client)
{
	vector<Individual*>::iterator iter = std::find(invited_clients.begin(), invited_clients.end(),client);
	if (iter != invited_clients.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ClientsManager::removeClient(Individual* client)
{
	vector<Individual*>::iterator iter = std::find(invited_clients.begin(), invited_clients.end(), client);
	if (iter != invited_clients.end())
	{
		iter = invited_clients.erase(iter);
	}
}

vector<Individual*> ClientsManager::getParticipants(UINT *uids, UINT count)
{
	CAgoraObject* agoraObject = CAgoraObject::GetAgoraObject();
	if (all_participants.size() > 0)
	{
		all_participants.clear();
	}
	if (participantsMap.size() > 0)
	{
		participantsMap.clear();
	}
	for (int i = 0; i < count; i++)
	{
		string str_uid = to_string(uids[i]);
		if (!agoraObject->GetSelfHost())
		{
			if (uids[i] >= 1000)
			{
				logInfo("Host id is " + str_uid);
				agoraObject->setHostId(str_uid);
			}
		}
		Individual* participant = NULL;
		if (individualMap.size() > 0)
		{
			map<string, Individual*>::iterator iter1 = individualMap.find(str_uid);			
			if (iter1 != individualMap.end())
			{
				//participant is in the clients list
				Individual* individual = iter1->second;
				participant = new Individual(individual->getWName(), individual->getUserId(), individual->getTelephone());
				participant->setStatus(USER_STATUS_JOIN_MEETING);
				participant->setImagePath(individual->getImagePath().c_str());
			}
			else
			{
				//client
				participant = new Individual(L"", str_uid, "");
				participant->setStatus(USER_STATUS_JOIN_MEETING);
			}
		}
		else
		{
			//client
			participant = new Individual(L"", str_uid, "");
			participant->setStatus(USER_STATUS_JOIN_MEETING);
		}
		all_participants.push_back(participant);
		participantsMap.insert(map<string, Individual*>::value_type(str_uid, participant));
	}
	return this->all_participants;
}

vector<Individual*> ClientsManager::getAllParticipants()
{
	return this->all_participants;
}

void ClientsManager::addParticipant(Individual* individual)
{
	all_participants.push_back(individual);
	participantsMap.insert(map<string, Individual*>::value_type(individual->getUserId(), individual));
}

void ClientsManager::removeParticipant(Individual* individual)
{
	vector<Individual*>::iterator iter = std::find(all_participants.begin(), all_participants.end(), individual);
	if (iter != all_participants.end())
	{
		iter = all_participants.erase(iter);
	}
	map<string, Individual*>::iterator iter1 = participantsMap.find(individual->getUserId());
	if (iter1 != participantsMap.end())
	{
		iter1 = participantsMap.erase(iter1);
	}
}

Individual* ClientsManager::updateParticipant(string rsp)
{
	Individual* participant = NULL;
	rapidjson::Document doc;
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		string log = "Get participant info:";
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Client") && doc["Client"].IsObject())
				{
					log = log + "this is a client, ";
					const rapidjson::Value& client = doc["Client"];
					string display_name = client["display_name"].GetString();
					wstring wname = StringUtil::utf8string2wstring(display_name);
					string name = StringUtil::WStringToString(wname);
					log = log + "name=" + name;
					string telephone = "";
					//if (client.HasMember("telephone"))
					//{
					//	telephone = client["telephone"].GetString();
					//}
					string user_id(to_string(client["id"].GetInt()));
					log = log + " id=" + user_id;
					string imagepath = "";
					if (client.HasMember("icon_round"))
					{
						if (client["icon_round"].IsString())
						{
							string dir = getAbsoluteDir();
							imagepath = dir + "res\\" + user_id + ".png";
							log = log + " icon_path=" + imagepath;
							string imagecontent = client["icon_round"].GetString();
							WritePhotoFile(s2ws(imagepath), imagecontent);
						}
						else {
							logError("The icon value is not string type.");
						}
					}
					map<string, Individual*>::iterator iter1 = participantsMap.find(user_id);
					logInfo(log);
					if (iter1 != participantsMap.end())
					{
						participant = iter1->second;
						participant->setImagePath(imagepath.c_str());
						participant->setWName(wname);
						participant->setTelephone(telephone.c_str());
					}
					else
					{
						logError("This client is not in the participant map.");
					}
				}
				if (doc.HasMember("Host") && doc["Host"].IsObject())
				{
					log = log + "this is a host, ";
					const rapidjson::Value& host = doc["Host"];
					string name = "";
					wstring host_display_name = L"";
					if (host.HasMember("hospital") && host["hospital"].IsObject())
					{
						const rapidjson::Value& hospital = host["hospital"];
						if (hospital.HasMember("chinese_name") && hospital["chinese_name"].IsString())
						{
							string hospital_name = hospital["chinese_name"].GetString();
							wstring wname = StringUtil::utf8string2wstring(hospital_name);
							host_display_name = wname;
							name = StringUtil::WStringToString(wname);
							log = log + "hospital=" + name;
						}
					}
					if (host.HasMember("department") && host["department"].IsObject())
					{
						const rapidjson::Value& department = host["department"];
						if (department.HasMember("chinese_name") && department["chinese_name"].IsString())
						{
							string department_name = department["chinese_name"].GetString();
							wstring wname = StringUtil::utf8string2wstring(department_name);
							host_display_name = host_display_name + L"_" + wname;
							name = name + "_" + StringUtil::WStringToString(wname);
							log = log + " department=" + StringUtil::WStringToString(wname);
						}
					}
					string additional = "";
					if (host.HasMember("equipment") && host["equipment"].IsObject())
					{
						const rapidjson::Value& equipment = host["equipment"];
						if (equipment.HasMember("name") && equipment["name"].IsString())
						{
							additional = equipment["name"].GetString();
							log = log + " equipment=" + additional;
						}
					}
					string user_id(to_string(host["id"].GetInt()));
					log = log + " id=" + user_id;
					string imagepath = "";
					if (host.HasMember("icon_round"))
					{
						if (host["icon_round"].IsString())
						{
							string dir = getAbsoluteDir();
							imagepath = dir + "res\\" + user_id + ".png";
							log = log + " icon_path=" + imagepath;
							string imagecontent = host["icon_round"].GetString();
							WritePhotoFile(s2ws(imagepath), imagecontent);
						}
						else
						{
							logError("The icon value is not string type.");
						}
					}
					logInfo(log);
					map<string, Individual*>::iterator iter1 = participantsMap.find(user_id);

					if (iter1 != participantsMap.end())
					{
						participant = iter1->second;
						participant->setImagePath(imagepath.c_str());
						participant->setWName(host_display_name);
						participant->setAdditional(additional.c_str());
					}
					else
					{
						logError("Host is not in the participant map.");
					}
				}
			}
			else
			{
				if (doc.HasMember("Error") && doc["Error"].IsString())
				{
					string error = doc["Error"].GetString();
					logError(error);
				}
			}
		}
	}
	return participant;
}

vector<Individual*> ClientsManager::getAllClients()
{
	return this->all_clients;
}