#pragma once
using namespace std;
#include <string>
#include <vector>
#include "client_common.h"
#include "AGConfig.h"
// COMMAND OR COMMANDER SENT ON THE RTM
#define ERROR_CMD "ERR"
//The clinet will send ANNO_REQUEST to requset the annotation from host
//The host will grant the annotation right to the first requester by ANNO_GRANT
//If the host had give out the Annotation rights, the other request will be refused by ANNO_REFUSE
//The client or host will send out ANNO_START to tell that ANNO_START and all the clients and host will popup the draw layer for ready. The Annotation
//Icon will be grey out for those client and host that is aceepting annotation signals
//The Icon of annotating client will show active annotating icon
//After the active annotating icon is clicked, it will end annotating by sending the ANNO_END to all the clients.
#define ANNOTATE_REQUEST_CMD "ANNO_REQUEST"
#define ANNOTATE_GRANT_CMD "ANNO_GRANT"
#define ANNOTATE_REFUSE_CMD "ANNO_REFUSE"
#define ANNOTATE_CLEAR_CMD "ANNO_CLEAR_ALL"
#define ANNOTATE_START_CMD "ANNO_START"
#define ANNOTATE_END_CMD "ANNO_END"
//Host will grant remote by sending GRANT_REMOTE to a user
//The user will Immediately go into the remote control status and all its events on the screen will be sent to host
//The host can takeback remote anytime by sending TACKBACK_REMOTE to the client
//The client can also terminate the remote control by sending GIVEBACK_REMOTE.
#define REMOTE_GRANT_CMD "GRANT_REMOTE"
#define REMOTE_TAKEBACK_CMD "TAKEBACK_REMOTE"
#define REMOTE_GIVEBACK_CMD "GIVEBACK_REMOTE"
#define REMOTE_CONTROL_START_CMD "REMOTE_CONTROL_START"
#define REMOTE_CONTROL_END_CMD "REMOTE_CONTROL_END"
#define BROADCAST_MEETING_ID_CMD "BROADCAST_MEETING_ID"
#define VIDEO_HD1_MODE_CMD "HD1_MODE"
#define VIDEO_HD2_MODE_CMD "HD2_MODE"
#define VIDEO_PIP_MODE_CMD "PIP_MODE"
#define VIDEO_PBYP_MODE_CMD "PBYP_MODE"
#define VIDEO_PUP_MODE_CMD "PUP_MODE"
#define VIDEO_PIP_SWAP_CMD "PIP_SWAP"
#define VIDEO_PIP_PIC_CMD "PIP_PIC"
#define VIDEO_PIP_ROTATE_CMD "PIP_ROTATE"
#define VIDEO_WINMODE1_CMD "WINMODE1"
#define VIDEO_WINMODE2_CMD "WINMODE2"
#define VIDEO_WINMODE4_CMD "WINMODE4"
#define VIDEO_WINMODE6_CMD "WINMODE6"
#define VIDEO_WINMODE7_CMD "WINMODE7"
#define VIDEO_WINMODE8_CMD "WINMODE8"
#define VIDEO_HDMI1_CMD "HDMI1"
#define VIDEO_HDMI2_CMD "HDMI2"
#define VIDEO_HDMI3_CMD "HDMI3"
#define VIDEO_HDMI4_CMD "HDMI4"
//#define VIDEO_HDMI1_WIN1_CMD "HDMI11"
//#define VIDEO_HDMI2_WIN1_CMD "HDMI21"
//#define VIDEO_HDMI3_WIN1_CMD "HDMI31"
//#define VIDEO_HDMI4_WIN1_CMD "HDMI41"
//#define VIDEO_HDMI1_WIN2_CMD "HDMI12"
//#define VIDEO_HDMI2_WIN2_CMD "HDMI22"
//#define VIDEO_HDMI3_WIN2_CMD "HDMI32"
//#define VIDEO_HDMI4_WIN2_CMD "HDMI42"
//#define VIDEO_HDMI1_WIN3_CMD "HDMI13"
//#define VIDEO_HDMI2_WIN3_CMD "HDMI23"
//#define VIDEO_HDMI3_WIN3_CMD "HDMI33"
//#define VIDEO_HDMI4_WIN3_CMD "HDMI43"
//#define VIDEO_HDMI1_WIN4_CMD "HDMI14"
//#define VIDEO_HDMI2_WIN4_CMD "HDMI24"
//#define VIDEO_HDMI3_WIN4_CMD "HDMI34"
//#define VIDEO_HDMI4_WIN4_CMD "HDMI44"
#define MOUSE_MOVE_CMD "MV"
#define MOUSE_LEFT_DOWN_CMD "MLD"
#define MOUSE_LEFT_UP_CMD "MLU"
#define MOUSE_RIGHT_DOWN_CMD "MRD"
#define MOUSE_RIGHT_UP_CMD "MRU"
#define MOUSE_MIDDLE_DOWN_CMD "MMD"
#define MOUSE_MIDDLE_UP_CMD "MMU"
#define KEY_DOWN_CMD "KD"
#define KEY_UP_CMD "KU"
#define QUERY_CLIENT_STATUS_CMD "QUERY_CLIENT_STATUS_CMD"
#define UPDATE_CLIENT_STATUS "UPDATE_CLIENT_STATUS"
#define LOCAL_INVITATION_EXPIRE "LOCAL_INVITATION_EXPIRE"
#define CANCEL_REMOTE_INVITATION_CMD "CANCEL_REMOTE_INVITATION_CMD"
// EVENT TYPE
typedef enum {
	INVALID,
	ANNOTATE_REQUEST,
	ANNOTATE_GRANT,
	ANNOTATE_REFUSE,
	ANNOTATE_START,
	ANNOTATE_END,
	ANNOTATE_CLEAR,
	REMOTE_CONTROL_GRANT,
	REMOTE_CONTROL_TAKEBACK,
	REMOTE_CONTROL_GIVEBACK,
	REMOTE_CONTROL_START,
	REMOTE_CONTROL_END,
	VIDEO_HD1_MODE,
	VIDEO_HD2_MODE,
	VIDEO_PIP_MODE,
	VIDEO_PBYP_MODE,
	VIDEO_PUP_MODE,
	VIDEO_PIP_SWAP,
	VIDEO_PIP_PIC,
	VIDEO_PIP_ROTATE,
	VIDEO_WINMODE1,
	VIDEO_WINMODE2,
	VIDEO_WINMODE4,
	VIDEO_WINMODE6,
	VIDEO_WINMODE7,
	VIDEO_WINMODE8,
	VIDEO_HDMI1,
	VIDEO_HDMI2,
	VIDEO_HDMI3,
	VIDEO_HDMI4,
	/*VIDEO_HDMI1_WIN1,
	VIDEO_HDMI2_WIN1,
	VIDEO_HDMI3_WIN1,
	VIDEO_HDMI4_WIN1,
	VIDEO_HDMI1_WIN2,
	VIDEO_HDMI2_WIN2,
	VIDEO_HDMI3_WIN2,
	VIDEO_HDMI4_WIN2,
	VIDEO_HDMI1_WIN3,
	VIDEO_HDMI2_WIN3,
	VIDEO_HDMI3_WIN3,
	VIDEO_HDMI4_WIN3,
	VIDEO_HDMI1_WIN4,
	VIDEO_HDMI2_WIN4,
	VIDEO_HDMI3_WIN4,
	VIDEO_HDMI4_WIN4,*/
	MOUSE_MOVE,
	MOUSE_LEFT_DOWN,
	MOUSE_LEFT_UP,
	MOUSE_RIGHT_DOWN,
	MOUSE_RIGHT_UP,
	MOUSE_MIDDLE_DOWN,
	MOUSE_MIDDLE_UP,
	MOUSE_DOUBLE_CLICK,
	KEY_DOWN,
	KEY_UP,
	KEY_RELEASEALL,
	QUERY_STATUS_CMD,
	UPDATE_STATUS,
	INVITATION_EXPIRE,
	CANCEL_INVITATION_CMD,
	BROADCAST_MEETING_ID
}
MSG_EVENT_TYPE;

// FOR WINDOW MESSAGE PARAM
class RTMMessagEvent
{
public:
	UINT userId = 0;
	MSG_EVENT_TYPE msgtype = INVALID;
	int source_window_width = 0;
	int source_window_height = 0;
	int mouse_x = 0;
	int mouse_y = 0;
	int keyCode = 0;

	/// <summary>
	///0 = no button press => 000
	///1 = right           => 001
	///2 = middle          => 010
	///3 = right | middle  => 011
	///4 = left            => 100
	///5 = left  | Right   => 101
	///6 = left  | middle  => 110
	///7 = all             => 111
	/// </summary>
	int mouseButtonStatus = 0;

	/// <summary>
	/// 0 = No  modifier key	=> 0000
	/// 1 = Win					=> 0001
	///	2 = Alt                 => 0010
	///	3 = Alt|Win             => 0011
	///	4 = Shift			    => 0100
	///	5 = Shift|Win           => 0101
	///	6 = Shift|Alt           => 0110
	///	7 = Shift|Alt|Win       => 0111
	///	8 = Ctrl				=> 1000
	///	9 = Ctrl|Win			=> 1001
	///	10 = Ctrl|Alt			=> 1010
	/// 11 = Ctrl|Alt|Win		=> 1011
	/// 12 = Ctrl|Shift  		=> 1100
	/// 13 = Ctrl|Shift|Win		=> 1101
	/// 14 = Ctrl|Shift|Alt     => 1110
	/// 15 = All				=> 1111
	/// </summary>
	int modifierKey = 0;
	//bool isRelativePosition = CAGConfig::GetInstance()->IsRelativeMousePos();

	BOOL IsMouseEvent();
	BOOL IsKeyEvent();
	UserStatus client_status;
	std::string client_channel;
	int meeting_id;
	void fromRTMMessage(UINT uid, string& message);
	RTMMessagEvent(MSG_EVENT_TYPE mt, int sww, int swh, int x, int y, int s)
	{
		this->msgtype = mt;
		this->source_window_width = sww;
		this->source_window_height = swh;
		this->mouse_x = x;
		this->mouse_y = y;
		this->mouseButtonStatus = s;
	}
	RTMMessagEvent(MSG_EVENT_TYPE mt, int sww, int swh, int kc, int mk)
	{
		this->msgtype = mt;
		this->source_window_width = sww;
		this->source_window_height = swh;
		this->keyCode = kc;
		this->modifierKey = mk;
	}
	RTMMessagEvent()
	{}
};

//All RTM Commands is defined here to easy management
class CommandManager
{
private:
	static CommandManager* commandMgr;
public:
	static CommandManager* GetInstance()
	{
		if (commandMgr == NULL)
		{
			commandMgr = new CommandManager();
		}
		return commandMgr;
	}
public:
	void GrantRemoteControl(UINT user);
	void TakeBackRemoteControl(UINT user);
	void GiveBackRemoteControl();
	void RequestAnnotation();
	void GrantAnnotation(UINT uid);
	void RefuseAnnotationRequest(UINT uid);
	void AnnotationStart();
	void AnnotationEnd();
	void AnnotationClear();
	void RemoteControlStart();
	void RemoteControlEnd();
	void sendVideoHD1ModeCMD();
	void sendVideoHD2ModeCMD();
	void sendVideoPIPModeCMD();
	void sendVideoPUPModeCMD();
	void sendVideoPBYPModeCMD();
	void sendVideoPIPSwapModeCMD();
	void sendVideoPIPPicModeCMD();
	void sendVideoPIPRotateModeCMD();
	void sendVideoWinMode1CMD();
	void sendVideoWinMode2CMD();
	void sendVideoWinMode4CMD();
	void sendVideoWinMode6CMD();
	void sendVideoWinMode7CMD();
	void sendVideoWinMode8CMD();
	void sendVideoHDMI11CMD();
	void sendVideoHDMI21CMD();
	void sendVideoHDMI31CMD();
	void sendVideoHDMI41CMD();
	void sendVideoHDMI12CMD();
	void sendVideoHDMI22CMD();
	void sendVideoHDMI32CMD();
	void sendVideoHDMI42CMD();
	void sendVideoHDMI13CMD();
	void sendVideoHDMI23CMD();
	void sendVideoHDMI33CMD();
	void sendVideoHDMI43CMD();
	void sendVideoHDMI14CMD();
	void sendVideoHDMI24CMD();
	void sendVideoHDMI34CMD();
	void sendVideoHDMI44CMD();
	void sendVideoHDMI1CMD();
	void sendVideoHDMI2CMD();
	void sendVideoHDMI3CMD();
	void sendVideoHDMI4CMD();
	void BroadcastAnnotationMouseMoveEvent(int w, int h, int x, int y);
	void BroadcastAnnotationMouseLDownEvent(int w, int h, int x, int y);
	void BroadcastAnnotationMouseLUpEvent(int w, int h, int x, int y);
	void BroadcastAnnotationMouseRUpEvent(int w, int h, int x, int y);
	void BroadcastAnnotationMouseRDownEvent(int w, int h, int x, int y);

	void sendRemoteControlMouseMoveEvent(int w, int h, int x, int y, int mousestatus);
	void sendRemoteControlMouseLDownEvent(int w, int h, int x, int y, int mousestatus);
	void sendRemoteControlMouseLUpEvent(int w, int h, int x, int y, int mousestatus);
	void sendRemoteControlMouseMDownEvent(int w, int h, int x, int y, int mousestatus);
	void sendRemoteControlMouseMUpEvent(int w, int h, int x, int y, int mousestatus);
	void sendRemoteControlMouseRDownEvent(int w, int h, int x, int y, int mousestatus);
	void sendRemoteControlMouseRUpEvent(int w, int h, int x, int y, int mousestatus);

	void sendRemoteControlKeyUpEvent(int w, int h, int keycode, int modifer);
	void sendRemoteControlKeyDownEvent(int w, int h, int keycode, int modifer);
	void sendQueryClientStatus(UINT UID);
	void sendUpdateClientStatus(UINT UID, UserStatus clientStatus, string channel);
	void subscribePeersOnlineStatus(const char* peerIds[], int peerCount);
	void Invite(UINT uid, string channel);
	void getRTMChannelMembers();
	void cancelInvitaions();
	void cancelInvitation(string user_id);
	void sendInvitatonExpireEvent(string user_id);
	void broadcastMeetingID(int meeting_id);
};
