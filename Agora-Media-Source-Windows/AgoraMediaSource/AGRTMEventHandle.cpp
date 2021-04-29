#include "stdafx.h"
#include "commonFun.h"
#include "AGRTMEventHandler.h"
#include "AgoraRTMInstance.h"
#include "MessageProtocal.h"



RTMMessagEvent G_RTM_USER_EVENT;
RTMMemberStatusEvent G_RTM_MEMBER_STATUS_EVENT;

CRTMCallBack::CRTMCallBack(HWND wnd) :m_MsgWnd(wnd), mRTMInstance(NULL)
{
}

CRTMCallBack::~CRTMCallBack()
{
	m_MsgWnd = nullptr;
}

void CRTMCallBack::setCallBackWnd(HWND wnd)
{
	m_MsgWnd = wnd;
}



// Inherited from |agora::rtm::IRtmServiceEventHandler|
void CRTMCallBack::onLoginSuccess() {
	//PAG_SIGNAL_LOGINSUCCESS lpData = new AG_SIGNAL_LOGINSUCCESS;
	//lpData->fd = 0;
	//lpData->uid = 0;  
	//mRTMInstance->JoinChannel(cs2s(CAgoraObject::GetAgoraObject()->GetChanelName()));
	//postMsg(WM_LoginSuccess, WPARAM(lpData));
	postMsg(WM_LoginSuccess, 0, 0);
}

void CRTMCallBack::onLoginFailure(LOGIN_ERR_CODE errorCode) {
	PAG_SIGNAL_LOGINFAILED lpData = new AG_SIGNAL_LOGINFAILED;
	lpData->ecode = errorCode;

	postMsg(WM_LoginFailed, WPARAM(lpData), LPARAM(errorCode));
}

void CRTMCallBack::onLogout(LOGOUT_ERR_CODE errorCode) {
	PAG_SIGNAL_LOGOUT lpData = new AG_SIGNAL_LOGOUT;
	lpData->ecode = errorCode;

	postMsg(WM_LogOut, WPARAM(lpData), LPARAM(errorCode));
}

void CRTMCallBack::onConnectionStateChanged(CONNECTION_STATE state, CONNECTION_CHANGE_REASON reason) {
}

void CRTMCallBack::onSendMessageResult(long long messageId, PEER_MESSAGE_ERR_CODE errorCode) {
	PAG_SIGNAL_MESSAGESENDSUCCESS  lpData = new AG_SIGNAL_MESSAGESENDSUCCESS;
	lpData->messageID = utf82gbk(std::to_string(messageId));
	if (errorCode == PEER_MESSAGE_ERR_OK)
		::PostMessage(m_MsgWnd, WM_MessageSendSuccess, WPARAM(lpData), errorCode);
	else
		::PostMessage(m_MsgWnd, WM_MessageSendError, WPARAM(lpData), errorCode);
}



// Inherited from |agora::rtm::IChannelEventHandler|
void CRTMCallBack::onJoinSuccess() {
	PAG_SIGNAL_CHANNELJOIN lpData = new AG_SIGNAL_CHANNELJOIN;
	lpData->channelID = utf82gbk(m_channeId.c_str());

	postMsg(WM_ChannelJoined, WPARAM(lpData));
}

void CRTMCallBack::onJoinFailure(JOIN_CHANNEL_ERR errorCode) {
	PAG_SIGNAL_CHANNELJOINFAILED lpData = new AG_SIGNAL_CHANNELJOINFAILED;
	lpData->channelID = utf82gbk(m_channeId);
	lpData->ecode = errorCode;

	postMsg(WM_ChannelJoinedFailed, WPARAM(lpData));
}

void CRTMCallBack::onLeave(LEAVE_CHANNEL_ERR errorCode) {
	PAG_SIGNAL_CHANNELLEAVE lpData = new AG_SIGNAL_CHANNELLEAVE;
	lpData->channelID = utf82gbk(m_channeId);
	lpData->ecode = errorCode;

	postMsg(WM_ChannelLeaved, WPARAM(lpData));
}


void CRTMCallBack::onMessageReceivedFromPeer(const char *peerId, const IMessage *message) {
	PAG_SIGNAL_MESSAGEINSTANCERECEIVE lpData = new AG_SIGNAL_MESSAGEINSTANCERECEIVE;
	lpData->msg = utf82gbk(message->getText());
	G_RTM_USER_EVENT.fromRTMMessage(str2int(peerId), (lpData->msg));
	postMsg(WM_RTM_USER_MSG_EVENT, WPARAM(&G_RTM_USER_EVENT));
}



void CRTMCallBack::onMessageReceived(const char *userId, const IMessage *message) {
	PAG_SIGNAL_MESSAGECHANNELRECEIVE lpData = new AG_SIGNAL_MESSAGECHANNELRECEIVE;
	lpData->msg = utf82gbk(message->getText());
	G_RTM_USER_EVENT.fromRTMMessage(str2int(userId), (lpData->msg));
	postMsg(WM_RTM_USER_MSG_EVENT, WPARAM(&G_RTM_USER_EVENT));
}

void CRTMCallBack::onSendMessageResult(long long messageId, CHANNEL_MESSAGE_ERR_CODE state) {
}

void CRTMCallBack::onMemberJoined(IChannelMember *member) {

	G_RTM_MEMBER_STATUS_EVENT.StatusEvent = USER_JOIN_CHANNEL;
	G_RTM_MEMBER_STATUS_EVENT.uid = str2int(member->getUserId());
	postMsg(WM_ChannelUserJoined, (WPARAM)&G_RTM_MEMBER_STATUS_EVENT, 0);
}

void CRTMCallBack::onMemberLeft(IChannelMember *member) {
	G_RTM_MEMBER_STATUS_EVENT.StatusEvent = USER_LEAVE_CHANNEL;
	G_RTM_MEMBER_STATUS_EVENT.uid = str2int(member->getUserId());
	postMsg(WM_ChannelUserLeaved, (WPARAM)&G_RTM_MEMBER_STATUS_EVENT, 0);

}

void CRTMCallBack::onGetMembers(IChannelMember **members, int userCount, GET_MEMBERS_ERR errorCode) {

	G_RTM_MEMBER_STATUS_EVENT.StatusEvent = USER_LIST;
	G_RTM_MEMBER_STATUS_EVENT.memberList.count = userCount;
	for (int i = 0; i < min(userCount, MAX_MEMBERS_IN_CHANNEL); i++)
	{
		IChannelMember *member = members[i];
		G_RTM_MEMBER_STATUS_EVENT.memberList.memberlist[i] = str2int(member->getUserId());

	}
	postMsg(WM_ChannelUserList, (WPARAM)&G_RTM_MEMBER_STATUS_EVENT, 0);
}

void CRTMCallBack::postMsg(UINT msg, WPARAM wParam /*= NULL*/, LPARAM lParam /*= NULL*/)
{
	//if (m_MsgWnd != NULL)
	//	::PostMessage(m_MsgWnd, msg, wParam, lParam);
	if (m_MsgWnd != NULL)
	{
		::SendMessage(m_MsgWnd, msg, wParam, lParam);
	}
	else
	{
		logError("m_MsgWnd is null, can not send message to dialog.");
	}
}


void CRTMCallBack::onFileMediaUploadResult(long long requestId, IFileMessage* fileMessage, UPLOAD_MEDIA_ERR_CODE code)
{

}

void CRTMCallBack::onImageMediaUploadResult(long long requestId, IImageMessage* imageMessage, UPLOAD_MEDIA_ERR_CODE code)
{
	if (imageMessage) {
		PImageMediaUploadResult imageMsg = new ImageMediaUploadResult;
		if (imageWidth > 0 && imageHeight > 0) {
			//imageMessage->setWidth(imageWidth);
			//imageMessage->setWidth(imageHeight);
		}
		if (thumbWidth > 0 && thumbHeight > 0) {
			imageMessage->setThumbnailWidth(thumbWidth);
			imageMessage->setThumbnailHeight(thumbHeight);
		}

		imageMsg->err = code;
		imageMsg->requestId = requestId;
		imageMsg->imageMessage = imageMessage;
		postMsg(WM_ImageMessageUploadResult, (WPARAM)imageMsg, code);
	}
	else {
		postMsg(WM_ImageMessageUploadResult, 0, code);
	}
}

void CRTMCallBack::onMediaDownloadToFileResult(long long requestId, DOWNLOAD_MEDIA_ERR_CODE code)
{
	postMsg(WM_ImageMessageDownloadResult, (WPARAM)requestId, (LPARAM)code);
}

void CRTMCallBack::onMediaUploadingProgress(long long requestId, const MediaOperationProgress &progress)
{
	PMediaProgress uploadProgress = new MediaProgress;
	uploadProgress->requestId = requestId;
	uploadProgress->currentSize = progress.currentSize;
	uploadProgress->totalSize = progress.totalSize;
	::PostMessage(m_MsgWnd, WM_MediaUploadingProgress, (WPARAM)uploadProgress, 0);

}

void CRTMCallBack::onMediaDownloadingProgress(long long requestId, const MediaOperationProgress &progress)
{
	PMediaProgress downloadProgress = new MediaProgress;
	downloadProgress->requestId = requestId;
	downloadProgress->currentSize = progress.currentSize;
	downloadProgress->totalSize = progress.totalSize;
	postMsg(WM_MediaDownloadingProgress, (WPARAM)downloadProgress, 0);
}

void CRTMCallBack::onImageMessageReceivedFromPeer(const char *peerId, const IImageMessage* message)
{
	if (message) {
		PAG_IMAGE_MESSAGE imageMsg = new AG_IMAGE_MESSAGE;
		imageMsg->peerId = peerId;

		imageMsg->messageId = message->getMessageId();
		imageMsg->mediaId = message->getMediaId();
		imageMsg->width = message->getWidth();
		imageMsg->size = message->getSize();

		imageMsg->thumbnailWidth = message->getThumbnailWidth();
		imageMsg->thumbnailHeight = message->getThumbnailHeight();
		imageMsg->thumbmailSize = message->getThumbnailLength();

		if (message->getThumbnailLength() > 0 && message->getThumbnailData()) {

			imageMsg->thumbFile = imageMessagepath + "thumbnail" + cs2s(utf82cs(message->getFileName()));
			FILE* fp = nullptr;
			fopen_s(&fp, imageMsg->thumbFile.c_str(), "wb");
			if (fp) {
				fwrite(message->getThumbnailData(), 1, message->getThumbnailLength(), fp);
				fclose(fp);
			}

		}
		imageMsg->filePath = imageMessagepath + message->getFileName();
		imageMsg->fileName = message->getFileName();
		postMsg(WM_ImageMessageRecvFromPeer, (WPARAM)imageMsg, 0);
	}
}

void CRTMCallBack::onImageMessageReceived(const char *userId, const IImageMessage* message)
{
	if (message) {
		PAG_IMAGE_MESSAGE imageMsg = new AG_IMAGE_MESSAGE;
		imageMsg->peerId = userId;

		imageMsg->messageId = message->getMessageId();
		imageMsg->mediaId = message->getMediaId();
		imageMsg->width = message->getWidth();
		imageMsg->size = message->getSize();

		imageMsg->thumbnailWidth = message->getThumbnailWidth();
		imageMsg->thumbnailHeight = message->getThumbnailHeight();
		imageMsg->thumbmailSize = message->getThumbnailLength();

		if (message->getThumbnailLength() > 0 && message->getThumbnailData()) {
			imageMsg->thumbFile = imageMessagepath + "thumbnail" + message->getFileName();
			FILE* fp = nullptr;
			fopen_s(&fp, imageMsg->thumbFile.c_str(), "wb");
			if (fp) {
				fwrite(message->getThumbnailData(), 1, message->getThumbnailLength(), fp);
				fclose(fp);
			}

		}

		imageMsg->filePath = imageMessagepath + message->getFileName();
		imageMsg->fileName = message->getFileName();
		postMsg(WM_ImageMessageRecvChannel, (WPARAM)imageMsg, 0);
	}
}

void CRTMCallBack::onPeersOnlineStatusChanged(const PeerOnlineStatus peersStatus[], int peerCount)
{
	for (int i = 0; i < peerCount; i++)
	{
		PClientOnlineStatusChange pdata = new ClientOnlineStatusChange;
		pdata->user_id = peersStatus[i].peerId;
		pdata->isOnline = peersStatus[i].isOnline;
		postMsg(WM_CLINETONLINESTATUSCHANGE, (WPARAM)pdata, 0);
	}
}

void CRTMCallBack::onFileMessageReceivedFromPeer(const char *peerId, const IFileMessage* message)
{

}

void CRTMCallBack::onMediaCancelResult(long long requestId, CANCEL_MEDIA_ERR_CODE code)
{

}

void CRTMCallBack::SetImageInfo(int w, int h, int tw, int th)
{
	imageWidth = w;
	imageHeight = h;
	thumbWidth = tw;
	thumbHeight = th;
}



void CRTMCallBack::onLocalInvitationReceivedByPeer(ILocalCallInvitation *localInvitation)
{

}
void CRTMCallBack::onLocalInvitationCanceled(ILocalCallInvitation *localInvitation)
{

}
void CRTMCallBack::onLocalInvitationFailure(ILocalCallInvitation *localInvitation, LOCAL_INVITATION_ERR_CODE errorCode)
{
	logInfo("============onLocalInvitationFailure:================");
	if (errorCode == LOCAL_INVITATION_ERR_PEER_OFFLINE)
	{
		logWarn("The callee is still offline 30 seconds since the call invitation is sent");
	}
	else if (errorCode == LOCAL_INVITATION_ERR_PEER_NO_RESPONSE)
	{
		logWarn("The callee is online but has not ACKed to the call invitation 30 seconds since it is sent");
	}
	else if (errorCode == LOCAL_INVITATION_ERR_INVITATION_EXPIRE)
	{
		logWarn("The call invitation expires 60 seconds since it is sent.");
		postMsg(WM_INVITATION_EXPIRE, (WPARAM)localInvitation, (LPARAM)errorCode);
	}
	else if (errorCode == LOCAL_INVITATION_ERR_NOT_LOGGEDIN)
	{
		logWarn("The caller is not logged in.");
	}
}

void CRTMCallBack::onLocalInvitationAccepted(ILocalCallInvitation *localInvitation, const char *response)
{
	postMsg(WM_InviteAcceptedByPeer, (WPARAM)WM_InviteAcceptedByPeer, (LPARAM)localInvitation);
}
void CRTMCallBack::onLocalInvitationRefused(ILocalCallInvitation *localInvitation, const char *response)
{
	postMsg(WM_InviteRefusedByPeer, (WPARAM)WM_InviteRefusedByPeer, (LPARAM)localInvitation);
}

void CRTMCallBack::onRemoteInvitationRefused(IRemoteCallInvitation *remoteInvitation)
{


}
void CRTMCallBack::onRemoteInvitationAccepted(IRemoteCallInvitation *remoteInvitation)
{



}
void CRTMCallBack::onRemoteInvitationReceived(IRemoteCallInvitation *remoteInvitation)
{
	//client receive an invitation from host
	postMsg(WM_InviteReceived, (WPARAM)WM_InviteReceived, (LPARAM)remoteInvitation);

}
void CRTMCallBack::onRemoteInvitationFailure(IRemoteCallInvitation *remoteInvitation, REMOTE_INVITATION_ERR_CODE errorCode)
{
	logInfo("============onRemoteInvitationFailure:================");
	if (errorCode == REMOTE_INVITATION_ERR_PEER_OFFLINE)
	{
		logWarn("The call invitation received by the callee fails: the callee is not online.");
	}
	else if (errorCode == REMOTE_INVITATION_ERR_ACCEPT_FAILURE)
	{
		logWarn("The call invitation received by callee fails: The callee does not ACK within a set time after the callee accepts the call invitation. This is usually a result of a network interruption.");
	}
	else if (errorCode == REMOTE_INVITATION_ERR_INVITATION_EXPIRE)
	{
		logWarn("The call invitation expires 60 seconds since it is sent, if the callee ACKs to the call invitation but neither the caller or callee takes any further action (cancel, accpet, or decline it).");
		postMsg(WM_INVITATION_EXPIRE, (WPARAM)remoteInvitation, (LPARAM)errorCode);
	}

}
void CRTMCallBack::onRemoteInvitationCanceled(IRemoteCallInvitation *remoteInvitation)
{

	postMsg(WM_InviteEndByPeer, (WPARAM)WM_InviteEndByPeer, (LPARAM)remoteInvitation);


}