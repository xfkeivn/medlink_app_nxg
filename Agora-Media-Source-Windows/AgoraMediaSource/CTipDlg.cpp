// CTipDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "CTipDlg.h"
#include "afxdialogex.h"
#include "resource.h"

int CTipDlg::TipDlgCount = 0;

CRITICAL_SECTION gSection;

CRITICAL_SECTION * CTipDlg::section = NULL;

// CTipDlg 对话框


IMPLEMENT_DYNAMIC(CTipDlg, CDialogEx)

CTipDlg::CTipDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIP, pParent)
{
	
	if (section == NULL)
	{
		section = &gSection;
		InitializeCriticalSection(section);
	}
		
	CTipDlg::Incre();
	Pos = TipDlgCount;
	TipDlgType = INFO;
	
}

void CTipDlg::Incre()
{
	EnterCriticalSection(section);
	TipDlgCount++;
	LeaveCriticalSection(section);
}
void CTipDlg::Decre()
{
	EnterCriticalSection(section);
	TipDlgCount--;
	LeaveCriticalSection(section);
}

CTipDlg::~CTipDlg()
{
}

void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIP_TITLE, m_title);
	DDX_Control(pDX, IDC_TIP_MESSAGE, m_message);
}

BOOL CTipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int fontsize = getResolutionFontSize(MEDIUM_TITLE_SIZE);
	fontTitle.CreateFont(fontsize, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	fontContext.CreateFont(fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_title.SetFont(&fontTitle);
	m_message.SetFont(&fontContext);
	
	CRect screen_rc;
	int width = getResolutionWidth(MAX_TIP_DIALOG_WIDTH);
	int height = getResolutionHeight(MAX_TIP_DIALOG_HEIGHT);
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&screen_rc, 0);
	long left = screen_rc.right - width - 10;
	long top = screen_rc.top;
	if (Pos == 1)
	{
		top = screen_rc.bottom - height -30;
	}
	else if (Pos > 1)
	{
		top = screen_rc.bottom - height -30 - (Pos - 1) * height;
	}
	MoveWindow(left, top, width, height, 1);
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, left, top, width, height, NULL);

	int tip_icon_size = getResolutionSize(MAX_TIP_ICON_SIZE);
	int tip_left_space = getResolutionWidth(MAX_TIP_LEFT_SPACE);
	int tip_gap = getResolutionWidth(MAX_TIP_GAP);
	int x = tip_icon_size + tip_left_space * 2;
	m_title.MoveWindow(x, tip_left_space, 100, getResolutionWidth(30));
	m_message.MoveWindow(x, getResolutionWidth(30) + tip_gap + tip_left_space, width - x, height - getResolutionWidth(30) - tip_gap - tip_left_space);

	return TRUE;  
}

void CTipDlg::OnTimer(UINT nIDEvent)
{

	KillTimer(1);
	EndDialog(0);
	CTipDlg::Decre();

}
BOOL CTipDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, RGB(13, 42, 76));
	pDC->FillPath();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmpBackground;	
	if (TipDlgType == INFO)
	{
		bmpBackground.LoadBitmap(IDB_TIPLG_INFO);
	}
	else 
	{
		bmpBackground.LoadBitmap(IDB_TIPLG_WARNING);
	}
	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld = dcMem.SelectObject(&bmpBackground);

	int tip_icon_size = getResolutionSize(MAX_TIP_ICON_SIZE);
	int tip_left_space = getResolutionWidth(MAX_TIP_LEFT_SPACE);
	pDC->StretchBlt(tip_left_space, (rect.Height()- tip_icon_size)/2, tip_icon_size, tip_icon_size, &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	dcMem.SelectObject(pbmpOld);
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CTipDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRgn rgn;
	CRect rc;
	GetWindowRect(&rc);
	rc -= rc.TopLeft();
	rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 15, 15); //根据窗口矩形创建一个圆角矩形最后两个是形成圆角的大小
	SetWindowRgn(rgn, TRUE);

}


HBRUSH CTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CreateSolidBrush(RGB(13, 42, 76));

	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_TIP_TITLE:
			pDC->SetTextColor(RGB(255, 200, 100));
			pDC->SetBkColor(RGB(13, 42, 76));
			pDC->SelectObject(&fontTitle);
			break;
		case IDC_TIP_MESSAGE:
			pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
			pDC->SetBkColor(RGB(13, 42, 76));
			pDC->SelectObject(&fontContext);
			break;
	}

	return hbr;
}

BEGIN_MESSAGE_MAP(CTipDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTipDlg 消息处理程序



void DisplayTips(void* param)
{
	PTipParam p = (PTipParam)param;
	CRect rc;
	CRect rc_pop;
	CTipDlg tipDlg;
	TIPDLGTYPE dlgtype = p->dlgtype;
	tipDlg.Create(IDD_DIALOG_TIP);
	LPCTSTR lpString = NULL;
	tipDlg.TipDlgType = dlgtype;
	if (dlgtype == INFO)
	{	
		lpString = LANG_STR("IDS_TIP_INFO");
	}
	else
	{
		lpString = LANG_STR("IDS_TIP_WARNING");
	}

	((CStatic*)tipDlg.GetDlgItem(IDC_TIP_TITLE))->SetWindowTextW(lpString);
	((CStatic*)tipDlg.GetDlgItem(IDC_TIP_MESSAGE))->SetWindowTextW(p->message);
	tipDlg.ShowWindow(SW_HIDE);
	tipDlg.SetTimer(1, 3000, NULL);
	
	free(p);
	
	::AnimateWindow(tipDlg.GetSafeHwnd(), 800, AW_SLIDE | AW_VER_NEGATIVE);

	MSG msg;                             
	while (GetMessage(&msg, NULL, 0, 0)) 
	{                                     
		TranslateMessage(&msg);          
		DispatchMessage(&msg);
	}

}

void DisplayTip(CString messagestr)
{
	PTipParam parampter = new TipParam();
	parampter->dlgtype = INFO;
	memcpy(parampter->message, messagestr.GetBuffer(), messagestr.GetLength() * sizeof(TCHAR));
	_beginthread(DisplayTips, 0, parampter);
}
void DisplayTipWarning(CString messagestr)
{
	PTipParam parampter = new TipParam();
	parampter->dlgtype = WARNING;
	memcpy(parampter->message, messagestr.GetBuffer(), messagestr.GetLength() * sizeof(TCHAR));
	_beginthread(DisplayTips, 0, parampter);
}