#ifndef __SINGLEWRAP_H__
#define __SINGLEWRAP_H__

#include "../SDK/include/IAgoraRtmService.h"
#include "../SDK/include/IAgoraRtmCallManager.h"
#include "../SDK/include/IAgoraService.h"
using namespace agora::rtm;
#define MAX_MEMBERS_IN_CHANNEL 20
/*typedef struct _ImageMsgInfo {
    IImageMessage* imageMsg;
    std::string fileName;
    std::string userAccount;
    int expectedThumbWidth;
    int expectedThumbHeight;
    CString fullPath;
    CString imageFormat;
}ImageMsgInfo;*/

typedef struct _LastImageInfo {
    std::string mediaId;
    AG_IMAGE_MESSAGE imageMessage;
}LastImageInfo, *PLastImageInfo, *LPLastImageInfo;

typedef enum
{
	USER_JOIN_CHANNEL,
	USER_LEAVE_CHANNEL,
	USER_LIST
}MemberStatusEvent;

typedef struct
{
	UINT memberlist[MAX_MEMBERS_IN_CHANNEL];
	UINT count;
}MemberList;

typedef struct
{
	MemberStatusEvent StatusEvent;
	union
	{
		UINT uid;
		MemberList memberList;
	};
}RTMMemberStatusEvent;



class CAgoraRTMInstance;
class CRTMCallBack :public agora::rtm::IRtmServiceEventHandler,
	public agora::rtm::IChannelEventHandler,public agora::rtm::IRtmCallEventHandler {
public:
  CRTMCallBack(HWND wnd);
  ~CRTMCallBack();

  void setCallBackWnd(HWND wnd);
  void setRTMInstance(CAgoraRTMInstance *instance) { mRTMInstance = instance; }


  void setJoinChannel(const std::string & channelId) {
    m_channeId = channelId;
  }
  void SetImageInfo(int w, int h, int tw, int th);
  void SetImagePath(std::string imagepath) { imageMessagepath = imagepath; }
protected:
  // Inherited from |agora::rtm::IRtmServiceEventHandler|
  void onLoginSuccess() override;

  void onLoginFailure(LOGIN_ERR_CODE errorCode) override;

  void onLogout(LOGOUT_ERR_CODE errorCode) override;

  void onConnectionStateChanged(CONNECTION_STATE state, CONNECTION_CHANGE_REASON reason) override;

  void onSendMessageResult(long long messageId, PEER_MESSAGE_ERR_CODE errorCode) override;

  void onMessageReceivedFromPeer(const char *peerId, const IMessage *message) override;

  // Inherited from |agora::rtm::IChannelEventHandler|
  void onJoinSuccess() override;

  void onJoinFailure(JOIN_CHANNEL_ERR errorCode) override;

  void onLeave(LEAVE_CHANNEL_ERR errorCode) override;

  void onMessageReceived(const char *userId, const IMessage *message) override;

  void onSendMessageResult(long long messageId, CHANNEL_MESSAGE_ERR_CODE state) override;

  void onMemberJoined(IChannelMember *member) override;

  void onMemberLeft(IChannelMember *member) override;

  void onGetMembers(IChannelMember **members, int userCount, GET_MEMBERS_ERR errorCode) override;
 // void onMediaUploadResult(long long requestId, IMessage* mediaMessage, UPLOAD_MEDIA_ERR_CODE code) override;
  void onFileMediaUploadResult(long long requestId, IFileMessage* fileMessage, UPLOAD_MEDIA_ERR_CODE code) override;

  void onImageMediaUploadResult(long long requestId, IImageMessage* imageMessage, UPLOAD_MEDIA_ERR_CODE code) override;
  void onMediaDownloadToFileResult(long long requestId, DOWNLOAD_MEDIA_ERR_CODE code) override;
  void onMediaUploadingProgress(long long requestId, const MediaOperationProgress &progress) override;
  void onMediaDownloadingProgress(long long requestId, const MediaOperationProgress &progress) override;
  void onImageMessageReceivedFromPeer(const char *peerId, const IImageMessage* message) override;
  void onFileMessageReceivedFromPeer(const char *peerId, const IFileMessage* message) override;
  void onMediaCancelResult(long long requestId, CANCEL_MEDIA_ERR_CODE code)override;
  void onImageMessageReceived(const char *userId, const IImageMessage* message)override;
  void onPeersOnlineStatusChanged(const PeerOnlineStatus peersStatus[], int peerCount)override;




  virtual void onLocalInvitationReceivedByPeer(ILocalCallInvitation *localInvitation);
  virtual void onLocalInvitationCanceled(ILocalCallInvitation *localInvitation);
  virtual void onLocalInvitationFailure(ILocalCallInvitation *localInvitation, LOCAL_INVITATION_ERR_CODE errorCode);
  virtual void onLocalInvitationAccepted(ILocalCallInvitation *localInvitation, const char *response);
  virtual void onLocalInvitationRefused(ILocalCallInvitation *localInvitation, const char *response);



  virtual void onRemoteInvitationRefused(IRemoteCallInvitation *remoteInvitation);
  virtual void onRemoteInvitationAccepted(IRemoteCallInvitation *remoteInvitation);
  virtual void onRemoteInvitationReceived(IRemoteCallInvitation *remoteInvitation);
  virtual void onRemoteInvitationFailure(IRemoteCallInvitation *remoteInvitation, REMOTE_INVITATION_ERR_CODE errorCode);
  virtual void onRemoteInvitationCanceled(IRemoteCallInvitation *remoteInvitation);

  


private:
  inline void postMsg(UINT msg, WPARAM wParam = NULL, LPARAM lParam = NULL);


private:
  HWND m_MsgWnd;
  CAgoraRTMInstance *mRTMInstance;
  std::string m_channeId;
  std::string imageMessagepath;
  int imageWidth;
  int imageHeight;
  int thumbWidth;
  int thumbHeight;
};


#endif