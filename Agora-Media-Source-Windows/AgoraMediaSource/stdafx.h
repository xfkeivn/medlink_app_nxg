
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include "LanguageSet.h"
#include "AGMessage.h"

#include "../SDK/include/IAgoraRtcEngine.h"
#include "../SDK/include/IAgoraMediaEngine.h"
#include "../SDK/include/IAgoraRtmService.h"
#include "AgoraObject.h"
#include "AGEventDef.h"
#include <afxdisp.h>
#include <afxdisp.h>
#include <afxcontrolbars.h>
#include <iostream>
#include <string>
#include "log_util.h"
#include "../SDK/include/IAgoraRtmService.h"
#include <GdiPlus.h> 
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
#pragma comment(lib,"../SDK/lib/agora_rtm_sdk.lib")
#pragma comment(lib, "../SDK/lib/agora_rtc_sdk.lib")
using namespace agora::rtm;

#pragma comment(lib,"../log4cpp/lib/log4cppD.lib")
#pragma comment(lib,"../log4cpp/lib/log4cppLIB.lib")

#define REMOTE_CONTROL_TARGET_SIZE_WIDTH 1280
#define REMOTE_CONTROL_TARGET_SIZE_HEIGHT 1024
#define REMTOE_CONTROL_TARGET_WIDTH_HEIGHT_RATIO (1.25)
#define MAXPATHLEN 1024
#define MOUSE_MOVE_EVENT_DELAY_INTERVAL 20
#define MAX_WIDTH 2736
#define MAX_HEIGHT 1824
#define MAX_BAR_HEIGHT 64
#define MAX_BOTTOM_HEIGHT 120
#define FUN_BUTTON_WIDTH 80
#define FUN_BUTTON_HEIGHT 64
#define MAX_TITLE_SIZE 40
#define MEDIUM_TITLE_SIZE 32
#define TEXT_LEFT_PADDING 15
#define TEXT_TOP_PADDING 15
#define TEXT_BOTTOM_PADDING 15
#define MAX_BSC_IMAGE_TOP 30
#define MAX_BSC_IMAGE_RIGHT 30
#define MAX_BSC_IMAGE_WIDTH 1172
#define MAX_BSC_IMAGE_HEIGHT 260
#define MAX_START_BUTTON_WIDTH 920
#define MAX_START_BUTTON_HEIGHT 168
#define MAX_CLIENT_LISTITEM_HEIGHT 220
#define MAX_PARTICIPANTS_LISTITEM_HEIGHT 180
#define MAX_ITEM_SPACE 15
#define MAX_CHECKBOX_SIZE 48
#define MAX_STATUS_BUTTON_SIZE 160
#define MAX_PARTICIPANTS_IMAGE_SIZE 120
#define MAX_PARTICIPANTS_REMOTE_SIZE 48
#define MAX_PARTICIPANTS_CONTAINER_SIZE 96
#define MAX_STATUS_SIZE 48
#define MAX_CLIENTS_SCROLLBAR_WIDTH 48
#define MAX_NETWORKIMAGE_SIZE 60
#define MAX_VIDEO_BUTTON_SIZE 96
#define MAX_TIP_ICON_SIZE 138
#define MAX_TIP_LEFT_SPACE 10
#define MAX_TIP_GAP 25
#define MAX_TIP_DIALOG_WIDTH 648
#define MAX_TIP_DIALOG_HEIGHT 240
#define MAX_VERSION_SIZE 34
#define VERSION_PADDING_RIGHT 21
#define VERSION_PADDING_BOTTOM 25
//all host acc
//#define HOST_ACCOUNT  _T("1")

//#define HOST_UID 1
#define CLIENT_UID_BASE 100
#define DB_CLIENT_ID_TO_UID(CLIENT_ID) (CLIENT_ID + CLIENT_UID_BASE)
#define APP_NAME _T("MedLink.exe")
#define APP_REG_DIR _T("SOFTWARE\\MedLink\\Properties")
#define IS_HOST_APP _T("IsHost")
#define HOSPITAL_ID _T("Hospital_id")
#define HOSPITAL_NAME _T("Hospital_name")
#define DEPARTMENT_ID _T("Department_id")
#define DEPARTMENT_NAME _T("Department_name")
#define EQUIPMENT_ID _T("Equipment_id")
#define EQUIPMENT_NAME _T("Equipment_name")
#define RELATIVE_MOUSE_POS _T("RelativeMousePos")
#define RCENABLE _T("RCEnable")
#define WEBSERVERIP _T("WebServerIP")
#define WEBSERVERPORT _T("WebServerPort")
#define WEBSOCKETPORT _T("WebSocketPort")
#define USEREMAIL _T("UserEmail")
#define USERPASSWORD _T("UserPassword")
namespace SingleDesc
{
	enum SingleMsg
	{
		WM_Default = WM_USER,
		WM_Reconnecting,
		WM_Reconnected,
		WM_LoginSuccess,
		WM_LogOut,
		WM_LoginFailed,
		WM_ChannelJoined,
		WM_ChannelJoinedFailed,
		WM_ChannelLeaved,
		WM_ChannelUserJoined,
		WM_ChannelUserLeaved,
		WM_ChannelUserList,
		WM_ChannelQueryUserNumResult,
		WM_ChannelQueryUserIsIn,
		WM_ChannelAttrUpdated,
		WM_InviteReceived,
		WM_InviteReceivedByPeer,
		WM_InviteAcceptedByPeer,
		WM_InviteRefusedByPeer,
		WM_InviteFailed,
		WM_InviteEndByPeer,
		WM_InviteEndByMyself,
		WM_InviteMsg,
		WM_MessageSendError,
		WM_MessageSendProgress,
		WM_MessageSendSuccess,
		WM_MessageAppReceived,
		WM_MessageInstantReceive,
		WM_MessageChannelReceive,
		WM_Log,
		WM_InvokeRet,
		WM_Msg,
		WM_UserAttrResult,
		WM_UserAttrAllResult,
		WM_Error,
		WM_QueryUserStatusResult,
		WM_Dbg,
		WM_BCCall_result,
		WM_ImageMessageUploadResult,
		WM_RTM_USER_MSG_EVENT,
		WM_HOSTLOGTORTMSERVICE,
		WM_CLIENTLOGTORTMSERVICE,
		WM_CLINETONLINESTATUSCHANGE,
		WM_JOINCHANNEL_SUCCESS,
		WM_REJOINCHANNEL_SUCCESS,
		WM_CLIENT_JOIN_CHANNEL,
		WM_NETWORK_QUALITY,
		WM_CLEANSCREEN_CMD,
		WM_JOINCHANNEL_AUDIO,
		WM_LEAVECHANNEL_SUCCESS,
		WM_GIVEBACK_REMOTE_CONTROL,
		WM_PARTICIPANT_LEAVE,
		WM_RECONNECTION_UI_CLOSE,
		WM_CONNECTION_STATUS_CHANGED,
		WM_VK_ESCAPE,
		WM_INVITATION_EXPIRE,
		WM_SHOW_PARTICIPANT,
		WM_CLIENT_DROPED,
		WM_CLIENT_GET_MEETINGID,
		WM_ImageMessageRecvFromPeer = 255,
		WM_ImageMessageRecvChannel,
		WM_ImageMessageDownloadResult,
		WM_MediaUploadingProgress = 270,
		WM_MediaDownloadingProgress,
		WM_UNKWOWN = WM_USER + 0xfff
	};

	enum eTagMsgtype
	{
		eType_Instance,
		eType_Channel,
	};

	typedef struct InPutParam
	{
		eTagMsgtype type;
		std::string strParam;
	}AG_INPUTPARAM, *PAG_INPUTPARAM, *LPAG_INPUTPARAM;

#define 	WM_InviteReceived_ChannelName  (WM_BCCall_result + 1)
#define WM_UpdateInputParam (WM_BCCall_result + 2)

	typedef struct AGTagStructReconnecting
	{
		UINT32 nretry;
	}AG_SIGNAL_RECONNECTING, *PAG_SIGNAL_RECONNECTING, *LPAG_SIGNAL_RECONECTING;

	typedef struct AGTagReconnected
	{
		int fd;
	}AG_SIGNAL_RECONNECTED, *PAG_SIGNAL_RECONNECTED, *LPAG_SIGNAL_RECONNECTED;

	typedef struct AGTagLoginSuccess
	{
		UINT32 uid;
		int fd;
	}AG_SIGNAL_LOGINSUCCESS, *PAG_SIGNAL_LOGINSUCCESS, *LPAG_SIGNAL_LOGINSUCCESS;

	typedef struct AGTagLogout
	{
		int ecode;
	}AG_SIGNAL_LOGOUT, *PAG_SIGNAL_LOGOUT, *LPAG_SIGNAL_LOGOUT;

	typedef struct AGTagLoginFailed
	{
		int ecode;
	}AG_SIGNAL_LOGINFAILED, *PAG_SIGNAL_LOGINFAILED, *LPAG_SIGNAL_LOGINFAILED;

	typedef struct AGTagChannelJoin
	{
		std::string channelID;
	}AG_SIGNAL_CHANNELJOIN, *PAG_SIGNAL_CHANNELJOIN, *LPAG_SIGNAL_CHANNELJOIN;

	typedef struct AGTagChannelJoinFailed
	{
		std::string channelID;
		int ecode;
	}AG_SIGNAL_CHANNELJOINFAILED, *PAG_SIGNAL_CHANNELJOINFAILED, *LPAG_SIGNAL_CHANNELJOINFAILED;

	typedef struct AGTagChannelLeave
	{
		std::string channelID;
		int ecode;
	}AG_SIGNAL_CHANNELLEAVE, *PAG_SIGNAL_CHANNELLEAVE, *LPAG_SIGNAL_CHANNELLEAVE;

	typedef struct  AGTagChannelUserJoin
	{
		std::string account;
		UINT32 uid;
	}AG_SIGNAL_CHANNELUSERJOIN, *PAG_SIGNAL_CHANNELUSERJOIN, *LPAG_SIGNAL_CHANNELUSERJOIN;

	typedef struct AGTagChannelUserLeave
	{
		std::string account;
		UINT32 uid;
	}AG_SIGNAL_CHANNELUSERLEAVE, *PAG_SIGNAL_CHANNELUSERLEAVE, *LPAG_SIGNAL_CHANNELUSERLEAVE;

	typedef struct AGTagChannelUserList
	{
		int nUserNum;
		char** ppAccount;
		std::string curChannel;
		UINT32 uids;
	}AG_SIGNAL_CHANNELUSERLIST, *PAG_SIGNAL_CHANNELUSERLIST, *LPAG_SIGNAL_CHANNELUSERLIST;

	typedef struct AGTagChannelQueryUserNumResult
	{
		std::string channelID;
		int ecode;
		int num;
	}AG_SIGNAL_CHANNELQUERYUSERNUMRESULT, *PAG_SIGNAL_CHANNELQUERYUSERNUMRESULT, *LPAG_SIGNAL_CHANNELQUERYUSERNUMRESULT;

	typedef struct AGTagChannelQueryUserIsln
	{
		std::string channelID;
		std::string account;
		int isIn;
	}AG_SIGNAL_CHANNELQUERYUSERISIN, *PAG_SIGNAL_CHANNELQUERYUSERISIN, *LPAG_SIGNAL_CHANNELQUERYUSERISIN;

	typedef struct AGTagChannelAttrUpdated
	{
		std::string channelD;
		std::string name;
		std::string value;
		std::string type;
	}AG_SIGNAL_CHANNELATTRUPDATED, *PAG_SIGNAL_CHANNELATTRUPDATED, *LPAG_SIGNAL_CHANNELATTRUPDATED;

	typedef struct AGTagInviteReceived
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
		std::string extra;
	}AG_SIGNAL_INVITERECEIVED, *PAG_SIGNAL_INVITERECEIVED, *LPAG_SIGNAL_INVITERECEIVED;

	typedef struct AGTagInviteReceivedByPeer
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
	}AG_SIGNAL_INVITERECEIVEDBYPEER, *PAG_SIGNAL_INVITERECEIVEDBYPEER, *LPAG_SIGNAL_INVITERECEIVEDBYPEER;

	typedef struct AGTagInviteAcceptedByPeer
	{
		std::string channelID;
		std::string account;
		int uid;
		std::string extra;
	}AG_SIGNAL_INVITEACCEPTEDBYPEER, *PAG_SIGNAL_INVITEACCEPTEDBYPEER, *LPAG_SIGNAL_INVITEACCEPTEDBYPEER;

	typedef struct AGTagInviteRefusedByPeer
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
		std::string extra;
	}AG_SIGNAL_INVITEREFUSEDBYPEER, *PAG_SIGNAL_INVITEREFUSEDBYPEER, *LPAG_SIGNAL_INVITEREFUSEDBYPEER;

	typedef struct AGTagInviteFailed
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
		int ecode;
		std::string extra;
	}AG_SIGNAL_INVITEFAILED, *PAG_SIGNAL_INVITEFAILED, *LPAG_SIGNAL_INVITEFAILED;

	typedef struct AGTagInviteEndByPeer
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
		std::string extra;
	}AG_SIGNAL_INVITEENDBYPEER, *PAG_SIGNAL_INVITEENDBYPEER, *LPAG_SIGNAL_INVITEENDBYPEER;

	typedef struct AGTagInviteEndByMyself
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
	}AG_SIGNAL_INVITEENDBYMYSELF, *PAG_SIGNAL_INVITEENDBYMYSELF, *LPAG_SIGNAL_INVITEENDBYMYSELF;

	typedef struct AGTagInviteMsg
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
		std::string msgType;
		std::string msgData;
		std::string extra;
	}AG_SIGNAL_INVIDEMSG, *PAG_SIGNAL_INVIDEMSG, *LPAG_SIGNAL_INVIDEMSG;

	typedef struct AGTagMessageSendError
	{
		std::string message;
		int ecode;
	}AG_SIGNAL_MESSAGESENDERROR, *PAG_SIGNAL_MESSAGESENDERROR, *LPAG_SIGNAL_MESSAGESENDERROR;

	typedef struct AGTagMessageSendProgress
	{
		std::string account;
		std::string messageID;
		std::string type;
		std::string info;
	}AG_SIGNAL_MESSAGESENDPROGRESS, *PAG_SIGNAL_MESSAGESENDPROGRESS, *LPAG_SIGNAL_MESSAGESENDPROGRESS;

	typedef struct AGTagMessageSendSuccess
	{
		std::string messageID;
		std::string mediaId;
	}AG_SIGNAL_MESSAGESENDSUCCESS, *PAG_SIGNAL_MESSAGESENDSUCCESS, *LPAG_SIGNAL_MESSAGESENDSUCCESS;

	typedef struct AGTagMessageInstantReceive
	{
		std::string account;
		UINT32 uid;
		std::string msg;
	}AG_SIGNAL_MESSAGEINSTANCERECEIVE, *PAG_SIGNAL_MESSAGEINSTANCERECEIVE, *LPAG_SIGNAL_MESSAGEINSTANCERECEIVE;

	typedef struct AGTagMessageChannelReceive
	{
		std::string channelID;
		std::string account;
		UINT32 uid;
		std::string msg;
	}AG_SIGNAL_MESSAGECHANNELRECEIVE, *PAG_SIGNAL_MESSAGECHANNELRECEIVE, *LPAG_SIGNAL_MESSAGECHANNELRECEIVE;

	typedef struct AGTagLog
	{
		std::string txt;
	}AG_SIGNAL_LOG, *PAG_SIGNAL_LOG, *LPAG_SIGNAL_LOG;

	typedef struct AGTagError
	{
		std::string name;
		int ecode;
		std::string desc;
	}AG_SIGNAL_ERROR, *PAG_SIGNAL_ERROR, *LPAG_SIGNAL_ERROR;

	typedef struct AgTagUploadImageResult {
		std::string mediaId;
		std::string messageId;
	}AG_UPLOAD_IMAGE_RESULT, *PAG_UPLOAD_IMAGE_RESULT, *LPAG_UPLOAD_IMAGE_RESULT;

	typedef struct AGTagQueryUserStatusResult
	{
		std::string name;
		std::string status;
	}AG_SIGNAL_QUERYUSERSTATUSRESULT, *PAG_SIGNAL_QUERYUSERSTATUSRESULT, *LPAG_SIGNAL_QUERYUSERSTATUSRESULT;

	typedef struct AgTagImageMessage {
		std::string filePath;
		std::string fileName;
		std::string thumbFile;
		std::string peerId;
		std::string mediaId;
		std::string messageId;
		int width;
		int height;
		int thumbnailWidth;
		int thumbnailHeight;
		int size;
		long long thumbmailSize;
	}AG_IMAGE_MESSAGE, *PAG_IMAGE_MESSAGE, *LPAG_IMAGE_MESSAGE;

	typedef struct _UploadMediaProgress {
		long long requestId;
		long long totalSize;
		long long currentSize;
	}MediaProgress, *PMediaProgress, *LPMediaProgress;

	typedef struct _tagImageMediaUploadResult {
		long long requestId;
		IImageMessage* imageMessage;
		UPLOAD_MEDIA_ERR_CODE err;
	}ImageMediaUploadResult, *PImageMediaUploadResult, *LPImageMediaUploadResult;

	typedef struct _clientOnlineStatusChange {
		string user_id;
		bool isOnline;
	}ClientOnlineStatusChange, *PClientOnlineStatusChange, *LPClientOnlineStatusChange;

	typedef struct _HOST_LOG_TO_RTM_SERVICE
	{
		string channel;
		int uid;
	}HOST_LOG_TO_RTM_SERVICE, *PHOST_LOG_TO_RTM_SERVICE, *LPHOST_LOG_TO_RTM_SERVICE;

	typedef struct _joinchannel_audio_muted {
		bool audiomuted;
	}JoinChannelAudioMuted, *PJoinChannelAudioMuted, *LPJoinChannelAudioMuted;

	typedef struct _participant_left {
		int uid;
	}Participant_Left, *PParticipant_Left, *LPParticipant_Left;

	typedef struct _connection_status_changed {
		CONNECTION_STATE_TYPE state;
		CONNECTION_CHANGED_REASON_TYPE reason;
	}ConnectionStatusChanged, *PConnectionStatusChanged, *LPConnectionStatusChanged;

}


using namespace SingleDesc;

#include "FileIO.h"
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
extern CConfigSignal gConfigSignal;
extern CFileIO gLogSignal;



#include <ObjBase.h>
#include "../duilib/includes/UIlib.h"
#include <afxcontrolbars.h>
using namespace DuiLib;
//#pragma comment(lib, "../duilib/lib/DuiLib_2017.lib")

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "../duilib/lib/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "../duilib/lib/DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "../duilib/lib/DuiLib_u.lib")
#   else
#       pragma comment(lib, "../duilib/lib/DuiLib.lib")
#   endif
#endif










#define HAVE_JPEG
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


