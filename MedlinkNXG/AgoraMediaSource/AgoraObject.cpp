#include "StdAfx.h"
#include "AgoraObject.h"
#include "AGResourceVisitor.h"
#include "AgoraRTMInstance.h"
#include <stdio.h>
#include "AGRTMEventHandler.h"
#include "string_util.h"
CAgoraObject *CAgoraObject::m_lpAgoraObject = NULL;
IRtcEngine *CAgoraObject::m_lpAgoraEngine = NULL;
CAgoraRTMInstance *CAgoraObject::m_lpAgoraRTMInstance = NULL;
CRTMCallBack* CAgoraObject::m_lpRTMCallback = NULL;
CAGEngineEventHandler CAgoraObject::m_EngineEventHandler;
CString   CAgoraObject::m_strVendorKey;

CAgoraObject::CAgoraObject(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
	, m_bScreenCapture(FALSE)
	, m_bEchoTest(FALSE)
	, m_nSelfUID(0)
{
	m_strChannelName.Empty();
	m_bLocalVideoMuted = FALSE;
	
	m_lpRTMCallback = new CRTMCallBack(NULL);
	
}

CAgoraObject::~CAgoraObject(void)
{
}

CString CAgoraObject::GetSDKVersion()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	CString strEngineVer;

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszEngineVer, -1, strEngineVer.GetBuffer(256), 256);
	strEngineVer.ReleaseBuffer();
#endif

	return strEngineVer;
}

IRtcEngine *CAgoraObject::GetEngine()
{
	if (m_lpAgoraEngine == NULL)
	{
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();
		RtcEngineContext ctx;

		ctx.eventHandler = &m_EngineEventHandler;

#ifdef UNICODE
		char szVendorKey[128];

		::WideCharToMultiByte(CP_ACP, 0, m_strVendorKey, -1, szVendorKey, 128, NULL, NULL);

		ctx.appId = szVendorKey;
#else
		ctx.appId = lpVendorKey;
#endif

		m_lpAgoraEngine->initialize(ctx);		
		if (!m_lpAgoraObject->GetSelfHost())
		{
			m_lpAgoraObject->MuteLocalVideo(true);
		}
	}
		
	return m_lpAgoraEngine;
}

CAgoraObject *CAgoraObject::GetAgoraObject(LPCTSTR lpVendorKey)
{
	if (m_lpAgoraObject == NULL)
	{
		m_lpAgoraObject = new CAgoraObject();
		
		
	}
		
	//if(m_lpAgoraEngine == NULL)
	//	m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	// 如果VendorKey为空则直接返回对象
	if (lpVendorKey == NULL)
		return m_lpAgoraObject;

//	RtcEngineContext ctx;
//
//	ctx.eventHandler = &m_EngineEventHandler;
//
//#ifdef UNICODE
//	char szVendorKey[128];
//
//	::WideCharToMultiByte(CP_ACP, 0, lpVendorKey, -1, szVendorKey, 128, NULL, NULL);
//	 
//	ctx.appId = szVendorKey;
//#else
//	ctx.appId = lpVendorKey;
//#endif
//
//	m_lpAgoraEngine->initialize(ctx);
	if (lpVendorKey != NULL)
		m_strVendorKey = lpVendorKey;
	char szVendorKey[128];
	::WideCharToMultiByte(CP_ACP, 0, lpVendorKey, -1, szVendorKey, 128, NULL, NULL);
	m_lpAgoraRTMInstance = CAgoraRTMInstance::getSignalInstance(szVendorKey, m_lpRTMCallback);
	

	return m_lpAgoraObject;
}

void CAgoraObject::CloseAgoraObject()
{
	if(m_lpAgoraEngine != NULL)
		m_lpAgoraEngine->release();

	if(m_lpAgoraObject != NULL)
		delete m_lpAgoraObject;

	m_lpAgoraEngine = NULL;
	m_lpAgoraObject = NULL;
}

void CAgoraObject::SetSelfUID(UINT nUID) {
	m_nSelfUID = nUID;
	m_strAccountName = s2cs(int2str(nUID));

	//if (!m_bHost)
	//{
	//	m_strAccountName = s2cs(int2str(nUID));
	//}
	//else
	//{
	//	m_strAccountName = HOST_ACCOUNT;
	//	m_nSelfUID = HOST_UID;
	//}

};

void CAgoraObject::SetMsgHandlerWnd(HWND hWnd)
{
	m_EngineEventHandler.SetMsgReceiver(hWnd);
	m_lpRTMCallback->setCallBackWnd(hWnd);
}

HWND CAgoraObject::GetMsgHandlerWnd()
{
	return m_EngineEventHandler.GetMsgReceiver();
}


void CAgoraObject::SetNetworkTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_ENNETTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_ENNETTEST);
}

BOOL CAgoraObject::GetNetworkTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ENNETTEST) != 0;
}

void CAgoraObject::SetEchoTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_ECHOTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_ECHOTEST);
}

BOOL CAgoraObject::GetEchoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ECHOTEST) != 0;
}

void CAgoraObject::SetSpeakerphoneTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_SPKPHTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_SPKPHTEST);
}

BOOL CAgoraObject::GetSpeakerphoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_SPKPHTEST) != 0;
}

void CAgoraObject::SetMicrophoneTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_MICPHTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_MICPHTEST);
}

BOOL CAgoraObject::GetMicrophoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_MICPHTEST) != 0;
}


void CAgoraObject::SetVideoTestFlag(BOOL bEnable)
{
	if (bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_VIDEOTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_VIDEOTEST);
}

BOOL CAgoraObject::GetVideoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_VIDEOTEST) != 0;
}

BOOL CAgoraObject::SetLogFilePath(LPCTSTR lpLogPath)
{
	ASSERT(m_lpAgoraEngine != NULL);

	CHAR szLogPathA[MAX_PATH];
	CHAR szLogPathTrans[MAX_PATH];

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (::GetFileAttributes(lpLogPath) == INVALID_FILE_ATTRIBUTES) {
		::GetModuleFileNameA(NULL, szLogPathA, MAX_PATH);
		LPSTR lpLastSlash = strrchr(szLogPathA, '\\')+1;
		strcpy_s(lpLastSlash, 64, "AgoraSDK.log");
	}
	else {
#ifdef UNICODE
		::WideCharToMultiByte(CP_UTF8, 0, lpLogPath, -1, szLogPathA, MAX_PATH, NULL, NULL);
#else
		::MultiByteToWideChar(CP_UTF8, 0, lpLogPath, -1, (WCHAR *)szLogPathA, MAX_PATH, NULL, NULL);
#endif
	}

	CAGResourceVisitor::TransWinPathA(szLogPathA, szLogPathTrans, MAX_PATH);

	ret = rep.setLogFile(szLogPathTrans);

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::JoinChannel(LPCTSTR lpChannelName, UINT nUID)
{
	int nRet = 0;

#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	nRet = m_lpAgoraEngine->joinChannel(NULL, szChannelName, NULL, nUID);
#else
	nRet = m_lpAgoraEngine->joinChannel(NULL, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;
	m_lpAgoraRTMInstance->JoinChannel(cs2s(m_strChannelName));	
	logInfo("Join channel " + cs2s(m_strChannelName));
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::LeaveCahnnel()
{
	if (!m_bHost)
	{
		m_strChannelName = L"";
		m_hostUID = "";
	}
	m_lpAgoraEngine->stopPreview();
	int nRet = m_lpAgoraEngine->leaveChannel();
	m_lpAgoraRTMInstance->LeaveChannel();
	m_lpAgoraEngine->release();
	m_lpAgoraEngine = NULL;

	return nRet == 0 ? TRUE : FALSE;
}

CString CAgoraObject::GetChanelName()
{
	return m_strChannelName;
}

CString CAgoraObject::GetCallID()
{
	agora::util::AString uid;
	CString strUID;

	m_lpAgoraEngine->getCallId(uid);

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, 0, uid->c_str(), -1, strUID.GetBuffer(128), 128);
	strUID.ReleaseBuffer();
#else
	strUID = uid->c_str();
#endif

	return strUID;
}

BOOL CAgoraObject::EnableVideo(BOOL bEnable)
{
	int nRet = 0;

	if (bEnable)
		nRet = m_lpAgoraEngine->enableVideo();
	else
		nRet = m_lpAgoraEngine->disableVideo();

	if (nRet == 0)
		m_bVideoEnable = bEnable;

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsVideoEnabled()
{
	return m_bVideoEnable;
}

BOOL CAgoraObject::EnableScreenCapture(HWND hWnd, int nCapFPS, LPCRECT lpCapRect, BOOL bEnable, int nBitrate)
{
	ASSERT(m_lpAgoraEngine != NULL);

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	agora::rtc::Rect rcCap;

	if (bEnable) {
		if (lpCapRect == NULL)
			ret = m_lpAgoraEngine->startScreenCapture(hWnd, nCapFPS, NULL, nBitrate);
		else {
			rcCap.left = lpCapRect->left;
			rcCap.right = lpCapRect->right;
			rcCap.top = lpCapRect->top;
			rcCap.bottom = lpCapRect->bottom;

			ret = m_lpAgoraEngine->startScreenCapture(hWnd, nCapFPS, &rcCap, nBitrate);
		}
	}
	else
		ret = m_lpAgoraEngine->stopScreenCapture();

	if (ret == 0)
		m_bScreenCapture = bEnable;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsScreenCaptureEnabled()
{
	return m_bScreenCapture;
}

BOOL CAgoraObject::MuteLocalAudio(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalAudioStream((bool)bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}


void CAgoraObject::MuteLocalVideo(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	rep.enableLocalVideo(!bMuted);//开关本地视频采集
	int ret = rep.muteLocalVideoStream((bool)bMuted);//停止/恢复发送本地视频流
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;
	//m_lpAgoraEngine->enableLocalVideo(!bMuted);
	//m_lpAgoraEngine->muteLocalVideoStream(bMuted);

}

BOOL CAgoraObject::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}


BOOL CAgoraObject::EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath)
{
	int ret = 0;

	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (bEnable) {
#ifdef UNICODE
		CHAR szFilePath[MAX_PATH];
		::WideCharToMultiByte(CP_ACP, 0, lpFilePath, -1, szFilePath, MAX_PATH, NULL, NULL);
		ret = rep.startAudioRecording(szFilePath, AUDIO_RECORDING_QUALITY_HIGH);
#else
		ret = rep.startAudioRecording(lpFilePath);
#endif
	}
	else
		ret = rep.stopAudioRecording();

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableNetworkTest(BOOL bEnable)
{
	int ret = 0;

	
	if (bEnable)
		ret = m_lpAgoraEngine->enableLastmileTest();
	else
		ret = m_lpAgoraEngine->disableLastmileTest();
	

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableEchoTest(BOOL bEnable)
{
	int ret = 0;

	if (bEnable)
		ret = m_lpAgoraEngine->startEchoTest();
	else
		ret = m_lpAgoraEngine->stopEchoTest();

	if (ret == 0)
		m_bEchoTest = bEnable;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetAudioProfileEx(int nSampleRate, int nChannels, int nSamplesPerCall)
{
	RtcEngineParameters rep(*m_lpAgoraEngine);

	int nRet = rep.setRecordingAudioFrameParameters(nSampleRate, nChannels, RAW_AUDIO_FRAME_OP_MODE_READ_WRITE, nSamplesPerCall);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableExtendAudioCapture(BOOL bEnable, IAudioFrameObserver* lpAudioFrameObserver)
{
	agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
	mediaEngine.queryInterface(m_lpAgoraEngine, agora::AGORA_IID_MEDIA_ENGINE);

	int nRet = 0;

	if (mediaEngine.get() == NULL)
		return FALSE;

	if (bEnable && lpAudioFrameObserver != NULL)
		nRet = mediaEngine->registerAudioFrameObserver(lpAudioFrameObserver);
	else
		nRet = mediaEngine->registerAudioFrameObserver(NULL);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableExtendVideoCapture(BOOL bEnable, IVideoFrameObserver* lpVideoFrameObserver)
{
	agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
	mediaEngine.queryInterface(m_lpAgoraEngine, agora::AGORA_IID_MEDIA_ENGINE);

	int nRet = 0;
	AParameter apm(*m_lpAgoraEngine);

	if (mediaEngine.get() == NULL)
		return FALSE;

	if (bEnable && lpVideoFrameObserver != NULL) {
		apm->setParameters("{\"che.video.local.camera_index\":1024}");
		nRet = mediaEngine->registerVideoFrameObserver(lpVideoFrameObserver);
	}
	else {
		nRet = mediaEngine->registerVideoFrameObserver(NULL);
		apm->setParameters("{\"che.video.local.camera_index\":0}");
	}
    isExtenalCaptureVideo = bEnable;
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn)
{
	int nRet = 0;

	if (bPreviewOn) {
		VideoCanvas vc;

		vc.uid = 0;
		vc.view = hVideoWnd;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN;

		m_lpAgoraEngine->setupLocalVideo(vc);
		nRet = m_lpAgoraEngine->startPreview();
	}
	else
		nRet = m_lpAgoraEngine->stopPreview();

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetLogFilter(agora::LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath)
{
	int nRet = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	nRet = rep.setLogFilter(logFilterType);

#ifdef UNICODE
	CHAR szFilePath[MAX_PATH];
	::WideCharToMultiByte(CP_ACP, 0, lpLogPath, -1, szFilePath, MAX_PATH, NULL, NULL);
	nRet |= rep.setLogFile(szFilePath);
#else
	nRet |= rep.setLogFile(lpLogPath);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetEncryptionSecret(LPCTSTR lpKey, int nEncryptType)
{
	CHAR szUTF8[MAX_PATH];

#ifdef UNICODE
	::WideCharToMultiByte(CP_UTF8, 0, lpKey, -1, szUTF8, MAX_PATH, NULL, NULL);
#else
	WCHAR szAnsi[MAX_PATH];
	::MultiByteToWideChar(CP_ACP, 0, lpKey, -1, szAnsi, MAX_PATH);
	::WideCharToMultiByte(CP_UTF8, 0, szAnsi, -1, szUTF8, MAX_PATH, NULL, NULL);
#endif
    switch (nEncryptType)
    {
    case 0:
        m_lpAgoraEngine->setEncryptionMode("aes-128-xts");
    	break;
    case 1:
        m_lpAgoraEngine->setEncryptionMode("aes-256-xts");
        break;
    default:
        m_lpAgoraEngine->setEncryptionMode("aes-128-xts");
        break;
    }
	int nRet = m_lpAgoraEngine->setEncryptionSecret(szUTF8);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableLocalRender(BOOL bEnable)
{
    int nRet = 0;

/*    if (bEnable)
        nRet = m_lpAgoraEngine->setParameters("{\"che.video.local.render\":true}");
    else
        nRet = m_lpAgoraEngine->setParameters("{\"che.video.local.render\":false}");
*/
    return nRet == 0 ? TRUE : FALSE;
}

void CAgoraObject::subscribePeersOnlineStatus(const char* peerIds[], int peerCount)
{
	CAgoraObject::m_lpAgoraRTMInstance->subscribePeersOnlineStatus(peerIds, peerCount);
}


void  CAgoraObject::SendMessageToPeer(UINT uid, std::string cstr)
{

	CAgoraObject::m_lpAgoraRTMInstance->sendInstantMsg(int2str(uid), cstr, false);
}

void CAgoraObject::SendCancelRemoteInvitationCMD(string user_id)
{
	string message = "CANCEL_REMOTE_INVITATION_CMD";
	CAgoraObject::m_lpAgoraRTMInstance->sendInstantMsg(user_id, message, true);
}

void CAgoraObject::SendMessageToHost(std::string cstr)
{
	if (m_hostUID.length() == 0)
	{
		logWarn("host id is empty, can not send message to host");
		return;
	}
	CAgoraObject::m_lpAgoraRTMInstance->sendInstantMsg(m_hostUID, cstr, false);

}


void CAgoraObject::SendMessageToChannel(std::string cstr)
{

	CAgoraObject::m_lpAgoraRTMInstance->sendChannelMsg(cs2s(m_strChannelName), cstr, false);

}

void CAgoraObject::RTMLogin()
{
	m_lpAgoraRTMInstance->Login(cs2s(GetRTMAccountName()), cs2s(GetVendorKey()));
}

void CAgoraObject::Invite(UINT uid, string channel)
{
	CString message = readRegKey(HOSPITAL_NAME, APP_REG_DIR) + readRegKey(DEPARTMENT_NAME, APP_REG_DIR) + L"," + readRegKey(EQUIPMENT_NAME, APP_REG_DIR);
	std::string callFrom = StringUtil::wstring2utf8string(message.GetBuffer());
	m_lpAgoraRTMInstance->SendInvitationPeer(uid, channel, callFrom);
}

void CAgoraObject::CancelInvitations()
{
	m_lpAgoraRTMInstance->CancelInvitations();
}

void CAgoraObject::CancelInvitation(string user_id)
{
	m_lpAgoraRTMInstance->CancelInvitation(user_id);
}

void CAgoraObject::RejectInvitation(IRemoteCallInvitation *invitation)
{
	m_lpAgoraRTMInstance->RefuseInvitation(invitation);
}

void CAgoraObject::AcceptInvitation(IRemoteCallInvitation *invitation)
{
	m_hostUID = invitation->getCallerId();
	m_lpAgoraRTMInstance->AcceptInvitation(invitation);
}

void CAgoraObject::onLocalInvitationAccept(ILocalCallInvitation *localInvitation)
{
	m_lpAgoraRTMInstance->onLocalInvitationAccept(localInvitation);
}

void CAgoraObject::onLocalInvitationRefused(ILocalCallInvitation *localInvitation)
{
	m_lpAgoraRTMInstance->onLocalInvitationRefused(localInvitation);
}

BOOL CAgoraObject::EnableWhiteboardVer(BOOL bEnable)
{
	// HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION
	HKEY hKey = NULL;

	LSTATUS lStatus = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION"), 0, REG_OPTION_NON_VOLATILE
		, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

	if (lStatus != ERROR_SUCCESS)
		return FALSE;

	DWORD dwIEVer = 11001;

	if (bEnable)
		lStatus = ::RegSetValueEx(hKey, _T("AgoraVideoCall.exe"), 0, REG_DWORD, (const BYTE*)&dwIEVer, sizeof(DWORD));
	else
		lStatus = ::RegDeleteKeyValue(hKey, NULL, _T("AgoraVideoCall.exe"));

	::RegCloseKey(hKey);

	return lStatus == ERROR_SUCCESS ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableWhiteboardFeq(BOOL bEnable)
{
	// HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION
	HKEY hKey = NULL;
	
	LSTATUS lStatus = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_MANAGE_SCRIPT_CIRCULAR_REFS"), 0, REG_OPTION_NON_VOLATILE
		, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

	if (lStatus != ERROR_SUCCESS)
		return FALSE;

	DWORD dwValue = 1;

	if (bEnable)
		lStatus = ::RegSetValueEx(hKey, _T("AgoraVideoCall.exe"), 0, REG_DWORD, (const BYTE*)&dwValue, sizeof(DWORD));
	else
		lStatus = ::RegDeleteKeyValue(hKey, NULL, _T("AgoraVideoCall.exe"));

	::RegCloseKey(hKey);

	return lStatus == ERROR_SUCCESS ? TRUE : FALSE;
}

void CAgoraObject::GetMembers()
{
	CAgoraObject::m_lpAgoraRTMInstance->ChannelQueryNumResult(cs2s(m_strChannelName));

}

void CAgoraObject::setHostId(string hostuid)
{
	logInfo("current host id is " + m_hostUID + ", set new host id " + hostuid);
	this->m_hostUID = hostuid;
}

void CAgoraObject::setInChannel(bool bInChannel)
{
	this->m_binChannel = bInChannel;
}

BOOL CAgoraObject::isInChannel()
{
	return this->m_binChannel;
}