#pragma once
#include "afxcoll.h"
#include "resource.h"
// CVideoSourceManagerDlg 对话框
#include "VideoSourceMgr.h"
class CVideoSourceManagerDlg : public CDialogEx, public IVideoPlayerObserver
{
	DECLARE_DYNAMIC(CVideoSourceManagerDlg)

public:
	CVideoSourceManagerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVideoSourceManagerDlg();
	enum { IDD = IDD_VIDEO_MGR_CONTAINER };
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEO_MGR_CONTAINER };
#endif
	void setVideoSourceMgr(VideoSourceMgr *pWndmgr);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	VideoSourceMgr *pWndmgr = NULL;
	afx_msg void OnPaint();
	//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnSize(UINT id, int w, int h);

	// For NXG Window and Source Mgr 
private:
	int frame_num = 0;
	bool show_sub_view = TRUE;
	bool start_recording = FALSE;
	
public:
	afx_msg void onBackToNoInteractive();
	afx_msg void on1view();
	afx_msg void on2view();
	afx_msg void on4view();
	afx_msg void onStartRecording();
	afx_msg void onFreezeFrame();
	afx_msg void onPlayRecord();
	afx_msg void onStopRecordPlay();
	afx_msg void onNextFrame();
	afx_msg void onPrevFrame();
	afx_msg void onSliderFrameSel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void onShowSubView();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
	virtual void on_frame_freezed(FreezeFrame freezeframe);
	virtual void on_video_record_loaded(int frame_number);
	virtual void on_frame_changed(int frame_index);
	virtual void on_video_play_to_end(int frame_index);

	DECLARE_MESSAGE_MAP()
};
