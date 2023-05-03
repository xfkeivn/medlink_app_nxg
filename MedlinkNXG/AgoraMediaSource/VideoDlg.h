#pragma once
#include "AGButton.h"
#include "AGVideoWnd.h"
#include "DeviceDlg.h"
#include "AGScreenCaptureDlg.h"
#include "AGDesktopCaptureDlg.h"
#include "ReconnectingUI.h"
#include "ChatDlg.h"
#include "MessageProtocal.h"
#include "AGConfig.h"
// CVideoDlg 对话框
#include "AgoraRTMInstance.h"
#include "ParticipantWnd.h"
#include "ParticipantsUI.h"
#include <cmath>
#include <queue>
#include <mmsystem.h>
#include "CVideoSourceManagerDlg.h"
#include "VideoSourceMgr.h"
using namespace std;
typedef enum
{
	MEETING_NORMAL,
	SELF_REMOTE_CONTROL,
	OTHER_REMOTE_CONTROL,
	SELF_ANNOTATION,
	OTHER_ANNOTATION
}MEETING_STATUS;

class HIDControl;
class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoDlg();

// 对话框数据
	enum { IDD = IDD_VIDEO_DIALOG };




	enum { 
		SCREEN_VIDEO1 = 0,	// 单屏
		SCREEN_VIDEO4,		// 4分屏
		SCREEN_VIDEOMULTI,	// 1大4小
//		SCREEN_WHITEBOARD	// 白板
	};

//	enum {
//		WHITEBOARD_HOST = 0,
//		WHITEBOARD_GUEST
//	};


	CWnd* GetLocalVideoWnd();
	
	void SetLocalVideoWndResoluation(int,int);

	void RebindVideoWnd();

	void ShowControlButton(BOOL bShow = TRUE);
	MEETING_STATUS GetMeetingStatus() { return mMeetingStatus; }
	void setAgoraMediaSourceDlgHWND(HWND hwnd);
	void handleLeftOne(bool dropped, UINT uid);
	UserStatus getParticipantStatus(UINT uid);
	void startVideoManager(bool isHost);
	void stopVideoManager();

	void onShowInfoControlView(bool show = true);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnMove(int x, int y);
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnShowModeChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowBig(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnWindowShareStart(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnDesktopShareStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEscapeKey(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnclose();
	afx_msg void OnBnClickedBtnrest();
	afx_msg void OnBnClickedBtnremote();
	afx_msg void OnBnClickedBtnanno();
	afx_msg void OnBnClickedRecording();
	afx_msg void OnBnClickedBtnEraser();
    //afx_msg void OnBnClickedBtnShowParticipant();
    //afx_msg void OnBnClickedBtnmode();
	afx_msg void OnBnClickedBtnaudio();
	//afx_msg void OnBnClickedBtnScreenCapture();
	//afx_msg void OnBnCliekedBtnWhiteBoard();
	//afx_msg void OnBnCliekedBtnCloseWhiteBoard();
    //afx_msg void OnBnClickedBtnsetup();
	
    //afx_msg void OnBnClickedBtntip();
    afx_msg void OnBnClickedBtnsetup();
    //afx_msg void OnBnClickedBtnfilter();

	afx_msg void OnBnClickedBtnfullscr();

	//afx_msg void OnBnClickedScreenshare();
	//afx_msg void OnBnClickedWindowshare();

	//afx_msg void OnBnClickedHostMode();
	//afx_msg void OnBnClickedGuestMode();
	// 用于处理引擎的回调消息
	afx_msg LRESULT OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDUserJoined(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDUserOffline(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDConnectionLost(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoteVideoStats(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLocalVideoStats(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnStartRecordingService(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStopRecordingService(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnApiCallExecuted(WPARAM wParam, LPARAM lParam);
	
    //afx_msg LRESULT OnStreamMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnEIDAudioQuality(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnEIDAudioVolumeIndication(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRTMUserEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInvitationEvents(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnMemberStatusEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetworkQuality(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCleanScreenBtnClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnJoinRTMChannelSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnJoinChannelAudioMuted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientGiveBackRemoteControl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowParticipant(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnectionStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReconnectionUIClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientDroped(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL NeedShowSizeBox(CPoint point);
	
	//void CreateScreenShareMenu();
    //void CreateMoreMenu();

	void DrawHead(CDC *pDC);

	void InitCtrls();

	void ShowButtonsNormal();

	void AdjustButtonsNormal(int cx, int cy);


	void onStartAnnotation();
	void onStopAnnotation();
	void ScaleBitmap(CBitmap *pBitmap, CBitmap &BitmapNew, int nWidth, int nHeight);
	void onShowReconnectingUI(bool self_reconnect, bool modal, int timer);
	void onCloseReconnectingUI();
	static DWORD WINAPI threadTiming(LPVOID lpParamter);
	void removeThread(UINT user_id);
	void handleHostLeftMeetingResponse(string rsp);
	
	private:
		void updateVideoArea();
		void updateInfoControlViewArea();

	private:


	
	int             pre_x = 0;
	int             pre_y = 0;
	MEETING_STATUS  mMeetingStatus = MEETING_NORMAL;
	int            m_other_anno_id;
	int            m_other_remote_id;
	CBrush			m_brHead;
	CFont		    m_ftTitle;
	CAGButton		m_btnMin;
	CAGButton		m_btnRst;
	CAGButton		m_btnClose;


	CAGButton       m_btnRemoteControl;
	CAGButton       m_btnAnnotation;
	CAGButton       m_btnEraser;
   // CAGButton       m_btnParticipaent;
	//CAGButton		m_btnMode;
	CAGButton		m_btnAudio;
	CAGButton		m_btnEndCall;
	//CAGButton		m_btnScrCap;
    CAGButton       m_btnSetup;

	// NXG Control Button

	CAGButton       m_btnRecording;
	CAGButton       m_btnBackToNoInteractive;
	CAGButton       m_btnRecordPlay;
	CAGButton       m_btnRecordFreeze;
	CAGButton       m_btnRecordNextFrame;
	CAGButton       m_btnRecordPrevFrame;
	CAGButton       m_btnRecordPause;


	//show the layout 1,2,4
	CAGButton       m_btnShowLayoutCtrlDlg;
	//show the camera control
	CAGButton       m_btnShowCameraCtrlDlg;


	BOOL m_showInfoAndControlView = TRUE;




	BOOL            m_bRemotingControling = FALSE;
	CAGButton		m_btnShow;

	BOOL			m_bLastStat;
	UINT			m_nScreenMode;
	UINT			m_nBigShowedUID;
	
	CAGVideoWnd		*m_lpBigShowed;

	//CDeviceDlg		m_dlgDevice;
    //CChatDlg        m_dlgChat;
	//CDrawLayerDlg   m_DrawLayerDlg;
	ReconnectingUI*  m_reconnectingUI;
	//BOOL			m_bShowParticipantWnd = false;
	//ParticipantWnd  m_ParticipantWnd;
	ParticipantsUI*  m_ParticipantsUI;
	//CAGScreenCaptureDlg		m_dlgScreenCapture;
	//CAGDesktopCaptureDlg	m_dlgDesktopCapture;

	CRect			m_rcVideoArea;


    CBitmap         m_bitMenuInfo;
    CBitmap         m_bitMenuDevice;
    CBitmap         m_bitMenuFilter;

	CImageList	    m_imgNetQuality;
	int             m_networkQuality;
	long            m_joinChannel_time;
	VideoSourceMgr *pWndmgr = NULL;
	CVideoSourceManagerDlg m_dlgVideoMgrContainer;


private:	// data	

	typedef struct _AGVIDEO_WNDINFO
	{
		UINT	nUID;
		int		nIndex;

		UINT	nWidth;
		UINT	nHeight;
		int		nBitrate;
		int		nFramerate;
		int		nCodec;

	} AGVIDEO_WNDINFO, *PAGVIDEO_WNDINFO, *LPAGVIDEO_WNDINFO;

	CList<AGVIDEO_WNDINFO>	m_listWndInfo;
	map<UINT, HANDLE> m_thread_map;
	BOOL			m_bRecording;
	BOOL			m_bFullScreen;
    BOOL            m_bFilter;
    BOOL            m_bShowInfo;
	HWND            m_AgoraMedaiSourceDlg_hwnd;
	int             m_client_meeting_id;
	



};
