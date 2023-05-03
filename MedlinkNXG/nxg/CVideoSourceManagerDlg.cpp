// CVideoSourceManagerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CVideoSourceManagerDlg.h"
#include "afxdialogex.h"
#include "VideoSourceMgr.h"

// CVideoSourceManagerDlg 对话框


#define IDC_BUTTON_NO_INTERACTIVE_VIEW 2000
#define IDC_BUTTON_INTERACTIVE_RECORD 2001
#define IDC_BUTTON_INTERACTIVE_FREEZEFRAME 2002
#define IDC_BUTTON_SINGLE 2003
#define IDC_BUTTON_4_VIEW 2004
#define IDC_BUTTON_DOUBLE 2005
#define IDC_BUTTON_INTERACTIVE_RECORD_REVIEW_PLAY 2006
#define INTERACTIVE_RECORD_REVIEW_NEXT 2007
#define INTERACTIVE_RECORD_REVIEW_PREV 2008
#define IDC_SLIDER_FRAME_SEL 2009
#define IDC_SHOW_SUB_VIEW 2010
#define IDC_BUTTON_INTERACTIVE_RECORD_REVIEW_STOP 2011


IMPLEMENT_DYNAMIC(CVideoSourceManagerDlg, CDialogEx)

CVideoSourceManagerDlg::CVideoSourceManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEO_MGR_CONTAINER, pParent)
{
	SetBackgroundColor(RGB(0x26, 0x26, 0x26));
}

CVideoSourceManagerDlg::~CVideoSourceManagerDlg()
{
}

void CVideoSourceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoSourceManagerDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	//ON_WM_SHOWWINDOW()
	// NXG Control
	ON_BN_CLICKED(IDC_BUTTON_NO_INTERACTIVE_VIEW, &CVideoSourceManagerDlg::onBackToNoInteractive)
	ON_BN_CLICKED(IDC_BUTTON_INTERACTIVE_RECORD, &CVideoSourceManagerDlg::onStartRecording)
	ON_BN_CLICKED(IDC_BUTTON_INTERACTIVE_FREEZEFRAME, &CVideoSourceManagerDlg::onFreezeFrame)
	ON_BN_CLICKED(IDC_BUTTON_4_VIEW, &CVideoSourceManagerDlg::on4view)
	ON_BN_CLICKED(IDC_BUTTON_SINGLE, &CVideoSourceManagerDlg::on1view)
	ON_BN_CLICKED(IDC_BUTTON_DOUBLE, &CVideoSourceManagerDlg::on2view)
	ON_BN_CLICKED(IDC_BUTTON_INTERACTIVE_RECORD_REVIEW_PLAY, &CVideoSourceManagerDlg::onPlayRecord)
	ON_BN_CLICKED(INTERACTIVE_RECORD_REVIEW_NEXT, &CVideoSourceManagerDlg::onNextFrame)
	ON_BN_CLICKED(INTERACTIVE_RECORD_REVIEW_PREV, &CVideoSourceManagerDlg::onPrevFrame)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FRAME_SEL, &CVideoSourceManagerDlg::onSliderFrameSel)
	ON_BN_CLICKED(IDC_SHOW_SUB_VIEW, &CVideoSourceManagerDlg::onShowSubView)
	ON_BN_CLICKED(IDC_BUTTON_INTERACTIVE_RECORD_REVIEW_STOP, &CVideoSourceManagerDlg::onStopRecordPlay)
END_MESSAGE_MAP()


void CVideoSourceManagerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == false)
		pWndmgr->hideAllWindows();
}


void CVideoSourceManagerDlg::setVideoSourceMgr(VideoSourceMgr *mgr)
{
	pWndmgr = mgr;
}

BOOL CVideoSourceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	return TRUE;
}





void CVideoSourceManagerDlg::OnClose()
{

	delete pWndmgr;
	CDialogEx::OnClose();
}

void CVideoSourceManagerDlg::OnSize(UINT id, int w, int h)
{
	CDialogEx::OnSize(id, w, h);
	if (pWndmgr != nullptr)
	{
		pWndmgr->onParentWndSize();
	}
	Invalidate();
}

void CVideoSourceManagerDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	//dc.FillSolidRect(&rect, RGB(0x26, 0x26, 0x26));
	if (pWndmgr != nullptr)
	{
		pWndmgr->draw_main_rect(dc);
	}
		
	CDialogEx::OnPaint();
}


void CVideoSourceManagerDlg::onBackToNoInteractive()
{

}
void CVideoSourceManagerDlg::on1view()
{

}
void CVideoSourceManagerDlg::on2view()
{

}
void CVideoSourceManagerDlg::on4view()
{

}
void CVideoSourceManagerDlg::onStartRecording()
{

}
void CVideoSourceManagerDlg::onFreezeFrame()
{

}
void CVideoSourceManagerDlg::onPlayRecord()
{

}
void CVideoSourceManagerDlg::onStopRecordPlay()
{

}
void CVideoSourceManagerDlg::onNextFrame()
{

}
void CVideoSourceManagerDlg::onPrevFrame()
{

}
void CVideoSourceManagerDlg::onSliderFrameSel(NMHDR *pNMHDR, LRESULT *pResult)
{

}
void CVideoSourceManagerDlg::onShowSubView()
{}

void CVideoSourceManagerDlg::on_frame_freezed(FreezeFrame freezeframe)
{

}
void CVideoSourceManagerDlg::on_video_record_loaded(int frame_number)
{

}
void CVideoSourceManagerDlg::on_frame_changed(int frame_index)
{

}
void CVideoSourceManagerDlg::on_video_play_to_end(int frame_index)
{

}
