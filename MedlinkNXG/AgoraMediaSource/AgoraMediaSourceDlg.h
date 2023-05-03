
// AgoraVideoCallDlg.h : header file
//

#pragma once

#include "AGHyperlink.h"
#include "SetupDlg.h"
#include "VideoDlg.h"
#include "AGConfig.h"
#include "Client.h"
#include "HostDUIMgr.h"
#include "VideoScreenControl.h"
#include "CurlHttpClient.h"
#include <set>

class websocket_endpoint;
class CAgoraMediaSourceDlg : public CDialogEx
{
	// Construction
public:
	CAgoraMediaSourceDlg(CWnd* pParent = NULL);	// standard constructor
	~CAgoraMediaSourceDlg();
// Dialog Data
	enum { IDD = IDD_OPENVIDEOCALL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HICON m_hIcon;

	// Generated message map functions
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnclose();
	afx_msg void OnMove(int, int);
	afx_msg void OnShowWindow(BOOL, UINT);


	afx_msg LRESULT OnJoinChannel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLeaveChannel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnectionLost(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnectionStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLastmileQuality(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDAudioVolumeIndication(WPARAM wParam, LPARAM lParam);

	
	afx_msg LRESULT onClientQuitApp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onClientShowInMeetingUI(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onClientShowLoginUI(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onClientShowIncomingCallUI(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onClientRejectIncomingCall(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onClientAcceptIncomingCall(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRTMUserEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInvitationEvents(WPARAM wParam, LPARAM lParam);
	

	afx_msg LRESULT OnHostLogToRTMService(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientLogToRTMService(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoginFailed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientOnlineStatusChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnJoinChannelSucces(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReJoinChannelSucces(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLeaveChannelSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientJoinChannelSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnParticipantLeave(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnClientNetworkError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInvitationExpire(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHostDirectStartMeeting(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void InitDialog(CString strAppid);
	void DrawClient(CDC *lpDC);
	void InitChildDialog();
	void InitClientDialog();
	void PublishClientStatus();
	void onClientShowJoinMeetingUI();
	
	
	string getCurrentSocketStatus();
	void sent_heart_beat();

private:
	CAGButton		m_btnMin;
	CAGButton		m_btnClose;

	CAGHyperLink	m_linkAgora;

	CDialogEx			*m_lpCurDialog;

	CSetupDlg			m_dlgSetup;

	CFont		m_ftTitle;
	CFont		m_ftLink;
	CFont       m_ftDes;
	CFont		m_ftPhone;
	CImageList	m_imgNetQuality;
	HANDLE heartbeatThread;

private:
	CVideoDlg		m_dlgVideo;
	CAgoraObject	*m_lpAgoraObject;
	IRtcEngine		*m_lpRtcEngine;

	int     m_nCodecType;
	int     m_nVideoSolution;

	int     m_nLastmileQuality;
	CString appID;
	string  websocket_uri;
	int     socketID;

	Client*                     m_client;
	set<UINT>                   m_hostset;
	USER_INFO_STRUCT*			m_client_user_struct;
	MEETING_INFO_STRUCT*		m_client_meeting_struct;
	DUIMgr*                     m_duimgr;

public:
	void ExitApp();
	void setMeetingID(int id);
};
