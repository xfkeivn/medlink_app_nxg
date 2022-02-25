#include "stdafx.h"
#include "client_common.h"
#include "MessageProtocal.h"
CommandManager* CommandManager::commandMgr = NULL;

RTMMessagEvent parseMessage(std::string &message);

BOOL RTMMessagEvent::IsMouseEvent()
{
	return  msgtype == MOUSE_MOVE || msgtype == MOUSE_LEFT_DOWN || msgtype == MOUSE_LEFT_UP || msgtype == MOUSE_RIGHT_DOWN || msgtype == MOUSE_RIGHT_UP;
}
BOOL RTMMessagEvent::IsKeyEvent()
{
	return  msgtype == KEY_DOWN || msgtype == KEY_UP;
}

void RTMMessagEvent::fromRTMMessage(UINT uid, string &message)
{
	*this = parseMessage(message);
	userId = uid;
}



string packetMouseEvent(MSG_EVENT_TYPE mousetype, int screen_w, int screen_h, int x, int y, int mouseStatus)
{
	string cmd = "";
	char temp[100];

	if (mousetype == MOUSE_LEFT_DOWN)
	{
		sprintf(temp, "MLD,%d,%d,%d,%d,%d", screen_w, screen_h, x, y, mouseStatus);

	}
	else if (mousetype == MOUSE_LEFT_UP)
	{
		sprintf(temp, "MLU,%d,%d,%d,%d,%d", screen_w, screen_h, x, y, mouseStatus);

	}
	else if (mousetype == MOUSE_MOVE)
	{
		sprintf(temp, "MV,%d,%d,%d,%d,%d", screen_w, screen_h, x, y, mouseStatus);

	}
	else if (mousetype == MOUSE_RIGHT_UP)
	{
		sprintf(temp, "MRU,%d,%d,%d,%d,%d", screen_w, screen_h, x, y, mouseStatus);

	}
	else if (mousetype == MOUSE_RIGHT_DOWN)
	{
		sprintf(temp, "MRD,%d,%d,%d,%d,%d", screen_w, screen_h, x, y, mouseStatus);

	}
	cmd = temp;
	//logInfo(cmd);
	return cmd;

}


string packetKeyEvent(MSG_EVENT_TYPE keytype, int screen_w, int screen_h, int keyCode, int modifier)
{
	string cmd = "";
	char temp[100];

	if (keytype == KEY_UP)
	{
		sprintf(temp, "KU,%d,%d,%d,%d", screen_w, screen_h, keyCode, modifier);

	}
	else if (keytype == KEY_DOWN)
	{
		sprintf(temp, "KD,%d,%d,%d,%d", screen_w, screen_h, keyCode, modifier);

	}
	cmd = temp;
	return cmd;
}

string packQueryClientStatusCMDEvent()
{
	string cmd = "QUERY_CLIENT_STATUS_CMD";
	return cmd;
}
string packInvitationExpireEvent()
{
	string cmd = "LOCAL_INVITATION_EXPIRE";
	return cmd;
}

string packUpdateClientStatusEvent(UserStatus clientStatus)
{
	string cmd = "";
	char temp[100];
	switch (clientStatus)
	{
	case USER_STATUS_LOGIN:
	{
		cmd = "UPDATE_CLIENT_STATUS,LOGIN";
		break;
	}
	case USER_STATUS_LOGOUT:
	{
		cmd = "UPDATE_CLIENT_STATUS,LOGOUT";
		break;
	}
	case USER_STATUS_JOIN_MEETING:
	{
		cmd = "UPDATE_CLIENT_STATUS,IN_MEETING";
		break;
	}
	case USER_STATUS_ENDMEETING:
	{
		cmd = "UPDATE_CLIENT_STATUS,END_MEETING";
		break;
	}
	case USER_STATUS_REJECT_MEETING:
	{
		cmd = "UPDATE_CLIENT_STATUS,REJECT_MEETING";
		break;
	}
	case USER_STATUS_TIMEOUT:
	{
		cmd = "UPDATE_CLIENT_STATUS,TIME_OUT";
		break;
	}
	default:
		cmd = "UPDATE_CLIENT_STATUS,UNKNOWN";
	}
	return cmd;
}

string packBroadCastMeetingIDEvent(int id)
{
	string cmd = "BROADCAST_MEETING_ID," + to_string(id);
	return cmd;
}
RTMMessagEvent parseMessage(std::string &message)
{
	RTMMessagEvent msgevent;
	

	if (message.find(MOUSE_MOVE_CMD) == 0 || message.find(MOUSE_LEFT_DOWN_CMD) == 0 || message.find(MOUSE_LEFT_UP_CMD) == 0 || message.find(MOUSE_RIGHT_UP_CMD) == 0|| message.find(MOUSE_RIGHT_DOWN_CMD)== 0)
	{
		vector<string> vs;
		split(message, vs,',');
		
		if (vs.size() != 6)
		{
			msgevent.msgtype = INVALID;
			return msgevent;
		}
		else
		{
			int w = atoi(vs.at(1).c_str());
			int h = atoi(vs.at(2).c_str());
			int x = atoi(vs.at(3).c_str());
			int y = atoi(vs.at(4).c_str());
			int ms = atoi(vs.at(5).c_str());
			msgevent.source_window_width = w;
			msgevent.source_window_height = h;
			msgevent.mouse_x = x;
			msgevent.mouse_y = y;
			msgevent.mouseButtonStatus = ms;
		}
		if (message.find(MOUSE_MOVE_CMD) == 0)
			msgevent.msgtype = MOUSE_MOVE;
		else if (message.find(MOUSE_LEFT_DOWN_CMD) == 0)
			msgevent.msgtype = MOUSE_LEFT_DOWN;
		else if (message.find(MOUSE_LEFT_UP_CMD) == 0)
			msgevent.msgtype = MOUSE_LEFT_UP;
		else if (message.find(MOUSE_RIGHT_DOWN_CMD) == 0)
			msgevent.msgtype = MOUSE_RIGHT_DOWN;
		else if (message.find(MOUSE_RIGHT_UP_CMD) == 0)
			msgevent.msgtype = MOUSE_MIDDLE_UP;
		else if (message.find(MOUSE_MIDDLE_DOWN_CMD) == 0)
			msgevent.msgtype = MOUSE_MIDDLE_DOWN;
		else if (message.find(MOUSE_MIDDLE_UP_CMD) == 0)
			msgevent.msgtype = MOUSE_RIGHT_UP;

		return msgevent;
	}
	else if (message.find(KEY_DOWN_CMD) == 0 || message.find(KEY_UP_CMD) == 0)
	{

		vector<string> vs;
		split(message, vs, ',');
		if (vs.size() != 5)
		{
			msgevent.msgtype = INVALID;
			return msgevent;
		}
		else
		{
			int w = atoi(vs.at(1).c_str());
			int h = atoi(vs.at(2).c_str());
			int x = atoi(vs.at(3).c_str());
			int y = atoi(vs.at(4).c_str());
			msgevent.source_window_width = w;
			msgevent.source_window_height = h;
			msgevent.keyCode = x;
			msgevent.modifierKey = y;
		}
		if (message.find(KEY_DOWN_CMD) == 0)
			msgevent.msgtype = KEY_DOWN;
		else if (message.find(KEY_UP_CMD) == 0)
			msgevent.msgtype = KEY_UP;

		return msgevent;
	}
	else if (message.find(ANNOTATE_START_CMD) == 0)
	{

		msgevent.msgtype = ANNOTATE_START;
		return msgevent;
	}
	else if (message.find(ANNOTATE_END_CMD) == 0)
	{

		msgevent.msgtype = ANNOTATE_END;
		return msgevent;
	}
	else if (message.find(ANNOTATE_CLEAR_CMD) == 0)
	{

		msgevent.msgtype = ANNOTATE_CLEAR;
		return msgevent;
	}
	else if (message.find(ANNOTATE_REQUEST_CMD) == 0)
	{

		msgevent.msgtype = ANNOTATE_REQUEST;
		return msgevent;
	}
	else if (message.find(ANNOTATE_GRANT_CMD) == 0)
	{

		msgevent.msgtype = ANNOTATE_GRANT;
		return msgevent;
	}
	else if (message.find(ANNOTATE_REFUSE_CMD) == 0)
	{

		msgevent.msgtype = ANNOTATE_REFUSE;
		return msgevent;
	}

	else if (message.find(REMOTE_GRANT_CMD) == 0)
	{

		msgevent.msgtype = REMOTE_CONTROL_GRANT;
		return msgevent;
	}

	else if (message.find(REMOTE_TAKEBACK_CMD) == 0)
	{

		msgevent.msgtype = REMOTE_CONTROL_TAKEBACK;
		return msgevent;
	}
	else if (message.find(REMOTE_GIVEBACK_CMD) == 0)
	{

	msgevent.msgtype = REMOTE_CONTROL_GIVEBACK;
		return msgevent;
	}
	else if (message.find(REMOTE_CONTROL_START_CMD) == 0)
	{
		msgevent.msgtype = REMOTE_CONTROL_START;
		return msgevent;
	}
	else if (message.find(REMOTE_CONTROL_END_CMD) == 0)
	{
		msgevent.msgtype = REMOTE_CONTROL_END;
		return msgevent;
	}
	else if (message.find(VIDEO_HD1_MODE_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_HD1_MODE;
		return msgevent;
	}
	else if (message.find(VIDEO_HD2_MODE_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_HD2_MODE;
		return msgevent;
	}
	else if (message.find(VIDEO_PIP_MODE_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_PIP_MODE;
		return msgevent;
	}
	else if (message.find(VIDEO_PBYP_MODE_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_PBYP_MODE;
		return msgevent;
	}
	else if (message.find(VIDEO_PUP_MODE_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_PUP_MODE;
		return msgevent;
	}
	else if (message.find(VIDEO_PIP_SWAP_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_PIP_SWAP;
		return msgevent;
	}
	else if (message.find(VIDEO_PIP_PIC_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_PIP_PIC;
		return msgevent;
	}
	else if (message.find(VIDEO_PIP_ROTATE_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_PIP_ROTATE;
		return msgevent;
	}
	else if (message.find(VIDEO_WINMODE1_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_WINMODE1;
		return msgevent;
	}
	//else if (message.find(VIDEO_HDMI1_WIN1_CMD) == 0)
	//{
	//	msgevent.msgtype = VIDEO_HDMI1_WIN1;
	//	return msgevent;
	//}
	//else if (message.find(VIDEO_HDMI2_WIN1_CMD) == 0)
	//{
	//	msgevent.msgtype = VIDEO_HDMI2_WIN1;
	//	return msgevent;
	//}
	//else if (message.find(VIDEO_HDMI3_WIN1_CMD) == 0)
	//{
	//	msgevent.msgtype = VIDEO_HDMI3_WIN1;
	//	return msgevent;
	//}
	//else if (message.find(VIDEO_HDMI4_WIN1_CMD) == 0)
	//{
	//	msgevent.msgtype = VIDEO_HDMI4_WIN1;
	//	return msgevent;
	//}
	else if (message.find(VIDEO_WINMODE2_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_WINMODE2;
		return msgevent;
	}
	else if (message.find(VIDEO_WINMODE4_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_WINMODE4;
		return msgevent;
	}
	else if (message.find(VIDEO_WINMODE6_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_WINMODE6;
		return msgevent;
	}
	else if (message.find(VIDEO_WINMODE7_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_WINMODE7;
		return msgevent;
	}
	else if (message.find(VIDEO_WINMODE8_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_WINMODE8;
		return msgevent;
	}
	else if (message.find(VIDEO_HDMI1_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_HDMI1;
		return msgevent;
	}
	else if (message.find(VIDEO_HDMI2_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_HDMI2;
		return msgevent;
	}
	else if (message.find(VIDEO_HDMI3_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_HDMI3;
		return msgevent;
	}
	else if (message.find(VIDEO_HDMI4_CMD) == 0)
	{
		msgevent.msgtype = VIDEO_HDMI4;
		return msgevent;
	}
	else if (message.find(QUERY_CLIENT_STATUS_CMD) == 0)
	{
		msgevent.msgtype = QUERY_STATUS_CMD;
		return msgevent;
	}
	else if (message.find(UPDATE_CLIENT_STATUS) == 0)
	{
		msgevent.msgtype = UPDATE_STATUS;
		vector<string> vs;
		split(message, vs, ',');
		//if (vs.size() != 2)
		//{
		//	msgevent.msgtype = INVALID;
		//	return msgevent;
		//}
		//else
		//{
			if (vs[1].compare("LOGIN") == 0)
			{
				msgevent.client_status = USER_STATUS_LOGIN;
			}
			else if (vs[1].compare("LOGOUT") == 0)
			{
				msgevent.client_status = USER_STATUS_LOGOUT;
			}
			else if (vs[1].compare("IN_MEETING") == 0)
			{
				msgevent.client_status = USER_STATUS_JOIN_MEETING;
				msgevent.client_channel = vs[2];
			}
			else if (vs[1].compare("END_MEETING") == 0)
			{
				msgevent.client_status = USER_STATUS_ENDMEETING;
			}
			else if (vs[1].compare("REJECT_MEETING") == 0)
			{
				msgevent.client_status = USER_STATUS_REJECT_MEETING;
			}
			else if (vs[1].compare("TIME_OUT") == 0)
			{
				msgevent.client_status = USER_STATUS_TIMEOUT;
			}
			else
			{
				msgevent.client_status = USER_STATUS_UNKNOWN;
			}
		//}
	return msgevent;
	}
	else if (message.find(LOCAL_INVITATION_EXPIRE) == 0)
	{
		msgevent.msgtype = INVITATION_EXPIRE;
		return msgevent;
	}
	else if (message.find(CANCEL_REMOTE_INVITATION_CMD) == 0)
	{
		msgevent.msgtype = CANCEL_INVITATION_CMD;
		return msgevent;
	}
	else if (message.find(BROADCAST_MEETING_ID_CMD) == 0)
	{
		msgevent.msgtype = BROADCAST_MEETING_ID;
		vector<string> vs;
		split(message, vs, ',');
		msgevent.meeting_id = atoi(vs.at(1).c_str());
		return msgevent;
    }
	else
	{
		msgevent.msgtype = INVALID;
		return msgevent;
	}
}


void CommandManager::GrantRemoteControl(UINT user)
{
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(user,REMOTE_GRANT_CMD);

}
void CommandManager::TakeBackRemoteControl(UINT user)
{
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(user, REMOTE_TAKEBACK_CMD);

}

void CommandManager::GiveBackRemoteControl()
{
	CAgoraObject::GetAgoraObject()->SendMessageToHost(REMOTE_GIVEBACK_CMD);

}


void CommandManager::RequestAnnotation() {

	CAgoraObject::GetAgoraObject()->SendMessageToHost(ANNOTATE_REQUEST_CMD);
}
void CommandManager::GrantAnnotation(UINT uid) {
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(uid,ANNOTATE_GRANT_CMD);
}
void CommandManager::RefuseAnnotationRequest(UINT uid) {
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(uid, ANNOTATE_REFUSE_CMD);
}

void CommandManager::AnnotationStart()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(ANNOTATE_START_CMD);
}
void CommandManager::AnnotationEnd()
{

	CAgoraObject::GetAgoraObject()->SendMessageToChannel(ANNOTATE_END_CMD);
}


void CommandManager::AnnotationClear()
{

	CAgoraObject::GetAgoraObject()->SendMessageToChannel(ANNOTATE_CLEAR_CMD);
}

void CommandManager::RemoteControlStart()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(REMOTE_CONTROL_START_CMD);
}

void CommandManager::RemoteControlEnd()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(REMOTE_CONTROL_END_CMD);
}

void CommandManager::sendVideoHD1ModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HD1_MODE_CMD);
}

void CommandManager::sendVideoHD2ModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HD2_MODE_CMD);
}

void CommandManager::sendVideoPIPModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_PIP_MODE_CMD);
}

void CommandManager::sendVideoPBYPModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_PBYP_MODE_CMD);
}

void CommandManager::sendVideoPUPModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_PUP_MODE_CMD);
}

void CommandManager::sendVideoPIPSwapModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_PIP_SWAP_CMD);
}

void CommandManager::sendVideoPIPPicModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_PIP_PIC_CMD);
}

void CommandManager::sendVideoPIPRotateModeCMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_PIP_ROTATE_CMD);
}
void CommandManager::sendVideoWinMode1CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_WINMODE1_CMD);
}
void CommandManager::sendVideoWinMode2CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_WINMODE2_CMD);
}
void CommandManager::sendVideoWinMode4CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_WINMODE4_CMD);
}
void CommandManager::sendVideoWinMode6CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_WINMODE6_CMD);
}
void CommandManager::sendVideoWinMode7CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_WINMODE7_CMD);
}
void CommandManager::sendVideoWinMode8CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_WINMODE8_CMD);
}
//void CommandManager::sendVideoHDMI11CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI1_WIN1_CMD);
//}
//void CommandManager::sendVideoHDMI21CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI2_WIN1_CMD);
//}
//void CommandManager::sendVideoHDMI31CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI3_WIN1_CMD);
//}
//void CommandManager::sendVideoHDMI41CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI4_WIN1_CMD);
//}
//void CommandManager::sendVideoHDMI12CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI1_WIN2_CMD);
//}
//void CommandManager::sendVideoHDMI22CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI2_WIN2_CMD);
//}
//void CommandManager::sendVideoHDMI32CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI3_WIN2_CMD);
//}
//void CommandManager::sendVideoHDMI42CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI4_WIN2_CMD);
//}
//void CommandManager::sendVideoHDMI13CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI1_WIN3_CMD);
//}
//void CommandManager::sendVideoHDMI23CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI2_WIN3_CMD);
//}
//void CommandManager::sendVideoHDMI33CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI3_WIN3_CMD);
//}
//void CommandManager::sendVideoHDMI43CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI4_WIN3_CMD);
//}
//void CommandManager::sendVideoHDMI14CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI1_WIN4_CMD);
//}
//void CommandManager::sendVideoHDMI24CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI2_WIN4_CMD);
//}
//void CommandManager::sendVideoHDMI34CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI3_WIN4_CMD);
//}
//void CommandManager::sendVideoHDMI44CMD()
//{
//	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI4_WIN4_CMD);
//}
void CommandManager::sendVideoHDMI1CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI1_CMD);
}
void CommandManager::sendVideoHDMI2CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI2_CMD);
}
void CommandManager::sendVideoHDMI3CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI3_CMD);
}
void CommandManager::sendVideoHDMI4CMD()
{
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(VIDEO_HDMI4_CMD);
}
void CommandManager::BroadcastAnnotationMouseMoveEvent(int w, int h, int x, int y) {
	//logInfo("BroadcastAnnotationMouseMoveEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",x=" + to_string(x) + ",y=" + to_string(y));
	string messagestr = packetMouseEvent(MOUSE_MOVE, w, h, x, y, 0);
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(messagestr);
}
void CommandManager::BroadcastAnnotationMouseLDownEvent(int w, int h, int x, int y) {
	//logInfo("BroadcastAnnotationMouseLDownEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",x=" + to_string(x) + ",y=" + to_string(y));
	string messagestr = packetMouseEvent(MOUSE_LEFT_DOWN, w, h, x, y, 0);
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(messagestr);
}
void CommandManager::BroadcastAnnotationMouseLUpEvent(int w, int h, int x, int y) {
	//logInfo("BroadcastAnnotationMouseLUpEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",x=" + to_string(x) + ",y=" + to_string(y));
	string messagestr = packetMouseEvent(MOUSE_LEFT_UP, w, h, x, y, 0);
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(messagestr);
}
void CommandManager::BroadcastAnnotationMouseRUpEvent(int w, int h, int x, int y) {
	//logInfo("BroadcastAnnotationMouseRUpEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",x=" + to_string(x) + ",y=" + to_string(y));
	string messagestr = packetMouseEvent(MOUSE_RIGHT_UP, w, h, x, y, 0);
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(messagestr);
}
void CommandManager::BroadcastAnnotationMouseRDownEvent(int w, int h, int x, int y) {
	//logInfo("BroadcastAnnotationMouseRDownEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",x=" + to_string(x) + ",y=" + to_string(y));
	string messagestr = packetMouseEvent(MOUSE_RIGHT_DOWN, w, h, x, y, 0);
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(messagestr);
}

void CommandManager::broadcastMeetingID(int meeting_id)
{
	logInfo("Broadcast the meeting id is " + to_string(meeting_id) + " in channel.");
	CAgoraObject::GetAgoraObject()->SendMessageToChannel(packBroadCastMeetingIDEvent(meeting_id));
}




void CommandManager::sendRemoteControlMouseMoveEvent(int w, int h, int x, int y, int mousestatus) {

	//logInfo("===============sendRemoteControlMouseMoveEvent====================");
	string messagestr = packetMouseEvent(MOUSE_MOVE, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
	

}
void CommandManager::sendRemoteControlMouseLDownEvent(int w, int h, int x, int y, int mousestatus) {
	//logInfo("===============sendRemoteControlMouseLDownEvent====================");
	string messagestr = packetMouseEvent(MOUSE_LEFT_DOWN, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
	
}
void CommandManager::sendRemoteControlMouseLUpEvent(int w, int h, int x, int y, int mousestatus) {
	//logInfo("===============sendRemoteControlMouseLUpEvent====================");
	string messagestr = packetMouseEvent(MOUSE_LEFT_UP, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
	
}

void CommandManager::sendRemoteControlMouseMDownEvent(int w, int h, int x, int y, int mousestatus) {
	//logInfo("===============sendRemoteControlMouseMDownEvent====================");
	string messagestr = packetMouseEvent(MOUSE_MIDDLE_DOWN, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
	
}
void CommandManager::sendRemoteControlMouseMUpEvent(int w, int h, int x, int y, int mousestatus) {
	//logInfo("===============sendRemoteControlMouseMUpEvent====================");
	string messagestr = packetMouseEvent(MOUSE_MIDDLE_UP, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
	
}


void CommandManager::sendRemoteControlMouseRDownEvent(int w, int h, int x, int y, int mousestatus) {
	//logInfo("===============sendRemoteControlMouseRDownEvent====================");
	string messagestr = packetMouseEvent(MOUSE_RIGHT_DOWN, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
}
void CommandManager::sendRemoteControlMouseRUpEvent(int w, int h, int x, int y, int mousestatus) {
	//logInfo("===============sendRemoteControlMouseRUpEvent====================");
	string messagestr = packetMouseEvent(MOUSE_RIGHT_UP, w, h, x, y, mousestatus);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
}

void CommandManager::sendRemoteControlKeyUpEvent(int w, int h, int keycode, int modifier){
	//logInfo("sendRemoteControlKeyUpEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",keycode=" + to_string(keycode) + ",modifier=" + to_string(modifier));
	string messagestr = packetKeyEvent(KEY_UP, w, h, keycode, modifier);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
}
void CommandManager::sendRemoteControlKeyDownEvent(int w, int h, int keycode, int modifier) {
	//logInfo("sendRemoteControlKeyDownEvent:messageEvent.source_window_width=" + to_string(w)
	//	+ ",messageEvent.source_window_height=" + to_string(h) + ",keycode=" + to_string(keycode) + ",modifier=" + to_string(modifier));
	string messagestr = packetKeyEvent(KEY_DOWN, w, h, keycode, modifier);
	CAgoraObject::GetAgoraObject()->SendMessageToHost(messagestr);
}

void CommandManager::sendQueryClientStatus(UINT UID) {
	string messagestr = packQueryClientStatusCMDEvent();
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(UID, messagestr);
}
void CommandManager::sendUpdateClientStatus(UINT UID, UserStatus clientStatus, string channel) {
	string messagestr = packUpdateClientStatusEvent(clientStatus);
	if (channel != "")
	{
		messagestr = messagestr + "," + channel;
	}
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(UID, messagestr);
}

void CommandManager::subscribePeersOnlineStatus(const char* peerIds[], int peerCount)
{
	CAgoraObject::GetAgoraObject()->subscribePeersOnlineStatus(peerIds, peerCount);
}

void CommandManager::Invite(UINT uid,string channel)
{
	CAgoraObject::GetAgoraObject()->Invite(uid, channel);
}

void CommandManager::getRTMChannelMembers()
{
	CAgoraObject::GetAgoraObject()->GetMembers();
}

void CommandManager::cancelInvitaions()
{
	CAgoraObject::GetAgoraObject()->CancelInvitations();
}

void CommandManager::cancelInvitation(string user_id)
{
	CAgoraObject::GetAgoraObject()->CancelInvitation(user_id);
}

void CommandManager::sendInvitatonExpireEvent(string user_id)
{
	string messagestr = packInvitationExpireEvent();
	CAgoraObject::GetAgoraObject()->SendMessageToPeer(stoi(user_id), messagestr);
}
