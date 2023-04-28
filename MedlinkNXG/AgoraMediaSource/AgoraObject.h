#pragma once

#include "stdafx.h"
#include "../SDK/include/IAgoraRtcEngine.h"
#include "../SDK/include/IAgoraMediaEngine.h"
#include "../SDK/include/IAgoraRtmCallManager.h"
#include "AGEngineEventHandler.h"
#include "commonFun.h"
//#include "ExtendVideoFrameObserver.h"
#include "AGConfig.h"
#define AVC_VER _T("V1.13.0, Build234, 09/11/2017, SDK1.13.0")

using namespace agora::media;
using namespace agora::rtc;

// 引擎标记位
#define AG_ENGFLAG_ENNETTEST	0x00000001
#define AG_ENGFLAG_ECHOTEST		0x00000002
#define AG_ENGFLAG_SPKPHTEST	0x00000004
#define AG_ENGFLAG_MICPHTEST	0x00000008
#define AG_ENGFLAG_VIDEOTEST	0x00000010


#define AG_CODEC_E264	0x00000000
#define AG_CODEC_EVP	0x00000001
#define AG_CODEC_VP8	0x00000002

#define APP_ID _T("")
class CAgoraRTMInstance;
class CRTMCallBack;
class CAgoraObject
{
public:
	~CAgoraObject(void);

	void SetNetworkTestFlag(BOOL bEnable);
	BOOL GetNetworkTestFlag();

	void SetEchoTestFlag(BOOL bEnable);
	BOOL GetEchoTestFlag();

	void SetSpeakerphoneTestFlag(BOOL bEnable);
	BOOL GetSpeakerphoneTestFlag();

	void SetMicrophoneTestFlag(BOOL bEnable);
	BOOL GetMicrophoneTestFlag();

	void SetVideoTestFlag(BOOL bEnable);
	BOOL GetVideoTestFlag();

	void SetMsgHandlerWnd(HWND hWnd);
	HWND GetMsgHandlerWnd();

	BOOL SetLogFilePath(LPCTSTR lpLogPath = NULL);

	BOOL JoinChannel(LPCTSTR lpChannelName, UINT nUID = 0);
	BOOL LeaveCahnnel();
	CString GetChanelName();
	CString GetCallID();
	CString GetVendorKey() { return m_strVendorKey; };

	void SetSelfUID(UINT nUID);
	UINT GetSelfUID() { return m_nSelfUID; };

	void setHostId(string hostuid);
	CString GetRTMAccountName()
	{
		return m_strAccountName;
	}

	void SetSelfHost(bool isHost) { m_bHost = isHost; };
	BOOL GetSelfHost() { return m_bHost; }
	void SetHostEquipment(string equipment) { m_hostEquipment = equipment; }
	string GetHostEquipment() { return m_hostEquipment; }

	BOOL EnableVideo(BOOL bEnable = TRUE);
	BOOL IsVideoEnabled();

	BOOL EnableScreenCapture(HWND hWnd, int nCapFPS = 15, LPCRECT lpCapRect = NULL, BOOL bEnable = TRUE, int nBitrate = 0);
	BOOL IsScreenCaptureEnabled();

	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();

	void MuteLocalVideo(BOOL bMuted);
	BOOL IsLocalVideoMuted();

	BOOL EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath);

	BOOL EnableNetworkTest(BOOL bEnable);

	BOOL EnableEchoTest(BOOL bEnable = TRUE);
	BOOL IsEchoTesting() { return m_bEchoTest; };

	BOOL SetAudioProfileEx(int nSampleRate, int nChannels, int nSamplesPerCall);

	BOOL EnableExtendAudioCapture(BOOL bEnable, IAudioFrameObserver* lpAudioFrameObserver);
	BOOL EnableExtendVideoCapture(BOOL bEnable, IVideoFrameObserver* lpVideoFrameObserver);
    BOOL IsExternalCaptureVideo() { return isExtenalCaptureVideo; }
	BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn = TRUE);

	BOOL SetLogFilter(agora::LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath);

    BOOL SetEncryptionSecret(LPCTSTR lpKey, int nEncryptType = 0);

    BOOL EnableLocalRender(BOOL bEnable);

	BOOL isInChannel();
	void setInChannel(bool bInChannel);
	void SendMessageToHost(std::string cstr);
	void SendMessageToChannel(std::string cstr);
	void SendMessageToPeer(UINT uid,std::string cstr);
	void subscribePeersOnlineStatus(const char* peerIds[], int peerCount);

	void RTMLogin();
	void Invite(UINT uid, string channel);
	void CancelInvitations();	
	void CancelInvitation(string user_id);
	void SendCancelRemoteInvitationCMD(string user_id);
	void RejectInvitation(agora::rtm::IRemoteCallInvitation *invitation);
	void AcceptInvitation(agora::rtm::IRemoteCallInvitation *invitation);
	void onLocalInvitationAccept(agora::rtm::ILocalCallInvitation *localInvitation);
	void onLocalInvitationRefused(agora::rtm::ILocalCallInvitation *localInvitation);
	void GetMembers();	

	static IRtcEngine *GetEngine();
	static CString GetSDKVersion();

	static BOOL EnableWhiteboardVer(BOOL bEnable);
	static BOOL EnableWhiteboardFeq(BOOL bEnable);

    

protected:
	CAgoraObject(void);

private:
	DWORD	m_dwEngineFlag;
	static  CAgoraObject	*m_lpAgoraObject;
	static	IRtcEngine	    *m_lpAgoraEngine;
	static	CString			m_strVendorKey;

	static CAgoraRTMInstance* m_lpAgoraRTMInstance;
	static CRTMCallBack* m_lpRTMCallback;
	UINT		m_nSelfUID;
	CString		m_strChannelName;
	BOOL		m_bVideoEnable;

	BOOL		m_bHost;
	string      m_hostUID;
	string      m_hostEquipment;

	CString     m_strAccountName;

	BOOL		m_bLocalAudioMuted;
	BOOL		m_bLocalVideoMuted;
	BOOL		m_bEchoTest;

	BOOL		m_bScreenCapture;
    BOOL        isExtenalCaptureVideo;
	BOOL        m_binChannel;
//	int			m_nCodecType;

public:
	static CAgoraObject *GetAgoraObject(LPCTSTR lpVendorKey = NULL);
	static void CloseAgoraObject();

	static CAGEngineEventHandler m_EngineEventHandler;
};
