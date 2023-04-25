// CReconnectingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "CReconnectingDlg.h"



// CReconnectingDlg dialog

IMPLEMENT_DYNAMIC(CReconnectingDlg, CDialogEx)

CReconnectingDlg::CReconnectingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RECONNECTING, pParent)
{

}

CReconnectingDlg::~CReconnectingDlg()
{
}

void CReconnectingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECONNECTING_MESSAGE, m_message);
	DDX_Control(pDX, IDC_RECONNECTING_TIME, m_time);
	DDX_Control(pDX, IDC_BTNEXIT, m_exitBtn);
}


BEGIN_MESSAGE_MAP(CReconnectingDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNEXIT, &CReconnectingDlg::OnExitBtnClicked)
END_MESSAGE_MAP()


// CReconnectingDlg message handlers
BOOL CReconnectingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	//MoveWindow(0, 0, 500, 300, 1);
	CRect rc;
	GetClientRect(&rc);
	m_message.SetFont(&m_font);
	m_time.SetFont(&m_font);
	m_exitBtn.SetFont(&m_font);

	m_message.MoveWindow(rc.Width() / 2 - 300, (rc.Height() - 75)/2, 300, 25, TRUE);
	m_message.SetWindowTextW(L"Host is dropped offline, waiting for its reconnecting:");
	m_time.MoveWindow(rc.Width() / 2 - 50, (rc.Height() - 75) / 2 + 25, 50, 25, TRUE);
	m_exitBtn.MoveWindow(rc.Width() / 2 - 50, (rc.Height() - 75) / 2 + 50, 50, 25, TRUE);
	GetDlgItem(IDC_BTNEXIT)->ShowWindow(SW_HIDE);
	return true;
}

void CReconnectingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect	rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(0, 0, rcClient.Width(), 32, RGB(38, 38, 38));
	dc.SetBkColor(RGB(38, 38, 38));
	
	CBrush m_edgeBrush;
	m_edgeBrush.CreateSolidBrush(RGB(100, 100, 100));
	CBrush* pOldBrush = (CBrush*)dc.SelectObject(&m_edgeBrush);

	dc.DrawEdge(rcClient, EDGE_BUMP, BF_ADJUST | BF_BOTTOMLEFT | BF_TOPRIGHT);
	dc.SelectObject(pOldBrush);
}


HBRUSH CReconnectingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_RECONNECTING_MESSAGE || pWnd->GetDlgCtrlID() == IDC_RECONNECTING_TIME)
	{
		pDC->SetTextColor(RGB(158, 158, 158));
	}
	if (pWnd->GetDlgCtrlID() == IDC_BTNEXIT)
	{
		pDC->SetTextColor(RGB(158, 158, 158));
		pDC->SetBkColor(RGB(109, 109, 109));
	}
	return hbr;
}

void CReconnectingDlg::OnExitBtnClicked()
{

}
