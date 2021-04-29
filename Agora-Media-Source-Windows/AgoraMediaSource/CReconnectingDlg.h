#pragma once
#include "resource.h"

// CReconnectingDlg dialog

class CReconnectingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReconnectingDlg)

public:
	CReconnectingDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CReconnectingDlg();
	

// Dialog Data

	enum { IDD = IDD_DIALOG_RECONNECTING };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnExitBtnClicked();
	DECLARE_MESSAGE_MAP()


private:
	CFont m_font;
	CButton m_exitBtn;
	CStatic m_message;
	CStatic m_time;
	
	
};
