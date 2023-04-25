#pragma once

#include "../SDK/include/IAgoraRtmCallManager.h"
#include "atlstr.h"
using namespace agora::rtm;
enum UserStatus
{
	USER_STATUS_UNKNOWN,
	USER_STATUS_LOGIN,
	USER_STATUS_LOGOUT,
	USER_STATUS_CLOSE,
	USER_STATUS_JOIN_MEETING,
	USER_STATUS_JOIN_MEETING_WAITING,
	USER_STATUS_ENDMEETING,
	USER_STATUS_REJECT_MEETING,
	USER_STATUS_TIMEOUT,
	USER_STATUS_JOIN_MEETING_FAILED,
	USER_STATUS_INVITING_PENDING,
	USER_STAUTS_RECONNECTING,
	USER_STATUS_RECONNECTED_SUCCESSFULLY
};

typedef struct 
{
	std::string login_name;
	std::string login_pwd;
	std::wstring display_name;
	std::string uid;
	UserStatus current_status;
	int hostids[100];
	int hostsize;
}USER_INFO_STRUCT;

typedef struct 
{
	std::string meeting_channel;
	std::string meeting_from;
	bool audiomuted;
}MEETING_INFO_STRUCT;

enum SwitchToLoginUIType
{
	SwitchToLoginUIType_INITIAL,
	SwitchToLoginUIType_LOGOUT,
};
