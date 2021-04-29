#pragma once
#include "resource.h"

// MissedCallDialog dialog

class MissedCallDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MissedCallDialog)

public:
	MissedCallDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MissedCallDialog();

// Dialog Data

	enum { IDD = IDD_DIALOG_MISSEDCALL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedMissedCallConfirmbtn();
	DECLARE_MESSAGE_MAP()


private:
	CFont m_fonttitle;
	CFont m_fontmessage;
	CButton m_confirmBtn;
	CStatic m_message;

};
