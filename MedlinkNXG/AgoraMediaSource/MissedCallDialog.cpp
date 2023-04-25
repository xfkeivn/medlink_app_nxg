// MissedCallDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AgoraMediaSource.h"
#include "MissedCallDialog.h"


// MissedCallDialog dialog

IMPLEMENT_DYNAMIC(MissedCallDialog, CDialogEx)

MissedCallDialog::MissedCallDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MISSEDCALL, pParent)
{

}

MissedCallDialog::~MissedCallDialog()
{
}

void MissedCallDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MISSED_CALL_CONFIRMBTN, m_confirmBtn);
	DDX_Control(pDX, IDC_MISSED_CALL_MESSAGE, m_message);
}


BEGIN_MESSAGE_MAP(MissedCallDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MISSED_CALL_CONFIRMBTN, &MissedCallDialog::OnBnClickedMissedCallConfirmbtn)
END_MESSAGE_MAP()


// MissedCallDialog message handlers

BOOL MissedCallDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_fonttitle.CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fontmessage.CreateFont(22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	CRect rc;
	GetClientRect(&rc);
	m_message.SetFont(&m_fontmessage);
	m_message.MoveWindow((rc.Width() - 300)/2, rc.Height() - 40, 300, 25, TRUE);
	m_confirmBtn.SetFont(&m_fontmessage);
	m_confirmBtn.MoveWindow(rc.Width() - 60, rc.Height() - 35, 50, 25, TRUE);
	m_confirmBtn.SetWindowText(L"OK");
	return true;
}
HBRUSH MissedCallDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CreateSolidBrush(RGB(13, 42, 76));

	switch (pWnd->GetDlgCtrlID())
	{

	case IDC_TIP_MESSAGE:
		pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		pDC->SetBkColor(RGB(13, 42, 76));
		pDC->SelectObject(&m_fontmessage);
		break;
	}

	return hbr;
}
void MissedCallDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
		
	CFont* defFont = dc.SelectObject(&m_fonttitle);
	CRect	rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(0, 0, rcClient.Width(), 32, RGB(38, 38, 38));
	dc.SetBkColor(RGB(38, 38, 38));
	dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	LPCTSTR lpString = L"Missed Call";
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	dc.TextOut(10, (32 - tm.tmHeight) / 2, lpString, _tcslen(lpString));

	CBrush m_edgeBrush;
	m_edgeBrush.CreateSolidBrush(RGB(100, 100, 100));
	CBrush* pOldBrush = (CBrush*)dc.SelectObject(&m_edgeBrush);

	dc.DrawEdge(rcClient, EDGE_BUMP, BF_ADJUST | BF_BOTTOMLEFT | BF_TOPRIGHT);
	dc.SelectObject(pOldBrush);
}


void MissedCallDialog::OnBnClickedMissedCallConfirmbtn()
{
	CDialog::OnOK();
}
