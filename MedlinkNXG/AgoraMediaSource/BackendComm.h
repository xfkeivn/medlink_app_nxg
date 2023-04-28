#pragma once
using namespace std;
#include <string>
#include <vector>
#include "Individual.h"
class BackendComm
{
public:
	virtual int reportStartMeeting(string host_id) = 0;
	virtual bool requestAppID(string &appid) = 0;
	virtual bool reportEndMeeting(string meetingId) = 0;
	virtual bool reportClientJoinMeeting1(string user_id, string meeting_id) = 0;
	virtual bool reportClientJoinMeeting2(string client_id, string meeting_id, string host_id, string channel_id)=0;
	virtual bool reportActiveLogClientExitMeeting(string user_id, string meeting_id) = 0;
	virtual bool requestMeetingAccount(string uuid, string hospital, string department, string equipment) = 0;
	virtual bool reportClientExitMeeting(string client_id,string meeting_id, string channel_id) = 0;
	virtual bool requestHostInitialInfo() = 0;
	virtual bool requestServiceClients(string uuid,vector<std::shared_ptr<Individual>> &clients) = 0;
};
class BackendCommImpl :public BackendComm
{
private:
	static shared_ptr< BackendComm> backend_common_impl;
	BackendCommImpl();
public:
	static shared_ptr< BackendComm> Instance();
	virtual bool requestAppID(string &appid);
	virtual int reportStartMeeting(string host_id);
	virtual bool reportEndMeeting(string meetingId);
	virtual bool reportClientJoinMeeting1(string user_id, string meeting_id);
	virtual bool reportClientJoinMeeting2(string client_id, string meeting_id, string host_id, string channel_id);
	virtual bool requestMeetingAccount(string uuid, string hospital, string department, string equipment);
	virtual bool reportActiveLogClientExitMeeting(string client_id, string channel_id);
	virtual bool reportClientExitMeeting(string client_id, string meeting_id, string channel_id);
	virtual bool requestHostInitialInfo();
	virtual bool requestServiceClients(string uuid, vector<std::shared_ptr<Individual>> &clients);

};
