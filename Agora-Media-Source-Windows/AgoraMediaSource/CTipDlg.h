#pragma once


// CTipDlg 对话框

typedef enum
{
	INFO,
	WARNING
}TIPDLGTYPE;

typedef struct
{
	TIPDLGTYPE dlgtype;
	TCHAR message[512];
}*PTipParam,TipParam;

class CTipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTipDlg)

public:
	CTipDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTipDlg();
	afx_msg void OnTimer(UINT nIDEvent);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	static CRITICAL_SECTION *section;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//void ChangeSize(UINT nID, int cx, int cy);
	CFont fontTitle;
	CFont fontContext;
	CRect m_rect;
	CStatic m_title;
	CStatic m_message;
public:
	static void Incre();
	static void Decre();
public:
	static int TipDlgCount;
	int Pos;
	TIPDLGTYPE TipDlgType;
	
};
void DisplayTip(CString message);
void DisplayTipWarning(CString message);