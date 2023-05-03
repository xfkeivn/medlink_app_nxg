#pragma once
using namespace std;
#include <string>
#include <vector>
#include "Individual.h"
#include "client_common.h"
class HospitalMgr;
class ClientsManager;
class Individual;
class websocket_endpoint;
class BackendComm
{
public:
	virtual int  reportStartMeeting(string host_id) = 0;
	virtual bool requestAppID(string &appid) = 0;
	virtual bool reportEndMeeting(string meetingId) = 0;
	virtual bool requestHostClients(ClientsManager *mgr, vector<Individual*>& clients) = 0;
	virtual bool reportClientJoinMeeting1(string user_id, string meeting_id) = 0;
	virtual bool reportClientJoinMeeting2(string client_id, string meeting_id, string host_id, string channel_id)=0;
	virtual bool reportActiveLogClientExitMeeting(string user_id, string meeting_id) = 0;
	virtual bool requestMeetingAccount(string &user_id, string &channel_name, string &errorstring) = 0;
	virtual bool reportClientExitMeeting(string client_id,string meeting_id, string channel_id) = 0;
	virtual bool requestHostInitialInfos(HospitalMgr &hospitalMgr)=0;
	virtual bool requestServiceClients(string uuid,vector<std::shared_ptr<Individual>> &clients) = 0;
	virtual bool clientAuth(string username, string password, USER_INFO_STRUCT &userInfoStruct,string &error_string) = 0;
	virtual bool requestParticientInfo(string uid, ClientsManager *mgr, Individual*) = 0;

	virtual bool websocketConnect() = 0;
	virtual string websocketGetStatus() = 0;
	virtual bool websocketClose() = 0;
	virtual bool websocketSendHeartBeat(string meeting_id) = 0;
};

class BackendCommImpl :public BackendComm
{
private:
	static shared_ptr< BackendComm> backend_common_impl;
	websocket_endpoint *endpoint;
	BackendCommImpl();
	int m_websocket_id = -1;
	
public:
	static shared_ptr< BackendComm> Instance();
	virtual bool requestAppID(string &appid);
	virtual int  reportStartMeeting(string host_id);
	virtual bool reportEndMeeting(string meetingId);
	virtual bool requestHostClients(ClientsManager *mgr, vector<Individual*>& clients);
	virtual bool reportClientJoinMeeting1(string user_id, string meeting_id);
	virtual bool reportClientJoinMeeting2(string client_id, string meeting_id, string host_id, string channel_id);
	virtual bool requestMeetingAccount(string &user_id,string &channel_name,string &errorstring);
	virtual bool reportActiveLogClientExitMeeting(string client_id, string channel_id);
	virtual bool reportClientExitMeeting(string client_id, string meeting_id, string channel_id);
	virtual bool requestServiceClients(string uuid, vector<std::shared_ptr<Individual>> &clients);
	virtual bool requestHostInitialInfos(HospitalMgr &hospitalMgr);
	virtual bool clientAuth(string username, string password, USER_INFO_STRUCT &userInfoStruct,string &errorstring);
	virtual bool requestParticientInfo(string uid, ClientsManager *mgr, Individual* indvidual);

	virtual bool websocketConnect();
	virtual string websocketGetStatus();
	virtual bool websocketClose();
	virtual bool websocketSendHeartBeat(string meeting_id);
	virtual ~BackendCommImpl();

private:
	bool handleMeetingAccount(string rsp, string &user_id, string &channel_name, string& error_string);
	bool handleHospitalsInfo(HospitalMgr &hospitalMgr, string &rsp);

};
