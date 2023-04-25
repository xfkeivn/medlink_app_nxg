
#pragma once


//{{NO_DEPENDENCIES}}

// Microsoft Visual C++ 生成的包含文件。

// 供 Temp.rc 使用

//

#define IDI_SIMPLEICON                  100

#define IDR_MENU_CONTEXT                115

#define ID_OPERATION_CLOSE              40010

#define ID_OPERATION_40011              40011

#define ID_OPERATION_CLEAR              40012

#define ID_OPERATION_COLOR              40013

#define ID_COLOR_RED                    40014

#define ID_COLOR_YELLOW                 40015

#define ID_COLOR_GREEN                  40016

#define ID_40017                        40017

#define ID_COLOR_BLACK                  40018

#define ID_COLOR_WHITE                  40019

#define ID_40020                        40020

#define ID_OPERATION_LINEWIDTH          40021

#define ID_LINEWIDTH_1                  40022

#define ID_LINEWIDTH_2                  40023

#define ID_LINEWIDTH_5                  40024

#define ID_LINEWIDTH_10                 40025

#define ID_TEXTOUT                      40026

#define ID_MOUSEMOVE                    40027

#define ID_TEXTCOLOR                    40028

#define ID_TEXTPOINT                    40029

#define ID_TEXTCOLOR_RED                40030

#define ID_TEXTCOLOR_GREEN              40031

#define ID_TEXTCOLOR_BLUE               40032

#define ID_TEXTCOLOR_BLACK              40033

#define ID_TEXTCOLOR_WHITE              40034

#define ID_TEXTPOINT_98                 40035

#define ID_TEXTPOINT_125                40036

#define ID_TEXTPOINT_150                40037

#define ID_TEXTPOINT_250                40038

#define ID_TEXTPOINT_350                40039

#define ID_SCREENSHOTS                  40040

#define ID_MINIMIZED                    40041

#define ID_OPERATION_MINIMIZED          40042

#define ID_OPERATION_NORMAL             40043

#define ID_OPERATION_MAXMIZED           40044





class CMainFrame :

	public CFrameWnd

{

public:

	CMainFrame(CWnd* parent);

	~CMainFrame();



public:

	DECLARE_MESSAGE_MAP()



	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnPaint();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnOperationClose();

	afx_msg void OnOperationClear();

	afx_msg void OnColorRed();

	afx_msg void OnColorYellow();

	afx_msg void OnColorGreen();

	afx_msg void OnColorBlack();

	afx_msg void OnColorWhite();

	afx_msg void OnLinewidth1();

	afx_msg void OnLinewidth2();

	afx_msg void OnLinewidth5();

	afx_msg void OnLinewidth10();

	afx_msg void OnUpdateColorRed(CCmdUI *pCmdUI);

	afx_msg void OnUpdateColorGreen(CCmdUI *pCmdUI);

	afx_msg void OnUpdateColorYellow(CCmdUI *pCmdUI);

	afx_msg void OnUpdateColorBlack(CCmdUI *pCmdUI);

	afx_msg void OnUpdateColorWhite(CCmdUI *pCmdUI);

	afx_msg void OnUpdateLinewidth1(CCmdUI *pCmdUI);

	afx_msg void OnUpdateLinewidth10(CCmdUI *pCmdUI);

	afx_msg void OnUpdateLinewidth2(CCmdUI *pCmdUI);

	afx_msg void OnUpdateLinewidth5(CCmdUI *pCmdUI);

	afx_msg void OnMouseTrack();

	afx_msg void OnUpdateMouseTrack(CCmdUI *pCmdUI);

	afx_msg void OnTextout();

	afx_msg void OnUpdateTextout(CCmdUI *pCmdUI);

	afx_msg void OnTextcolorBlack();

	afx_msg void OnUpdateTextcolorBlack(CCmdUI *pCmdUI);

	afx_msg void OnTextcolorBlue();

	afx_msg void OnUpdateTextcolorBlue(CCmdUI *pCmdUI);

	afx_msg void OnTextcolorGreen();

	afx_msg void OnUpdateTextcolorGreen(CCmdUI *pCmdUI);

	afx_msg void OnTextcolorRed();

	afx_msg void OnUpdateTextcolorRed(CCmdUI *pCmdUI);

	afx_msg void OnTextcolorWhite();

	afx_msg void OnUpdateTextcolorWhite(CCmdUI *pCmdUI);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnTextpoint125();

	afx_msg void OnUpdateTextpoint125(CCmdUI *pCmdUI);

	afx_msg void OnTextpoint350();

	afx_msg void OnUpdateTextpoint350(CCmdUI *pCmdUI);

	afx_msg void OnTextpoint150();

	afx_msg void OnUpdateTextpoint150(CCmdUI *pCmdUI);

	afx_msg void OnTextpoint250();

	afx_msg void OnUpdateTextpoint250(CCmdUI *pCmdUI);

	afx_msg void OnOperationMinimized();

	afx_msg void OnScreenshots();

	afx_msg void OnUpdateScreenshots(CCmdUI *pCmdUI);

	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnOperationMaxmized();

	afx_msg void OnOperationNormal();





public:

	void DrawInputText(CDC *pDC);

	void SetCaretPosition(CDC *pDC);

	void SetCharCXCY(int nFontSize, LPCTSTR strFont = _T("宋体"));

	void ReSetTextAndCaretPos();

	void ClearScreen(CDC *pDC);

	void KeepNoTransparent(CDC *pDC);

	void CreateMyCaret(CPoint point);

	void DestroyMyCaret();



public:

	CPoint m_ptPrev;//鼠标移动时点（之前的点）

	CPoint m_ptCurrent;//鼠标移动时点（当前的点）



	BOOL m_blMouseDown;//鼠标左键处于按下的状态标志



	COLORREF m_clrLineColor;//线颜色

	COLORREF m_clrFontColor;//字体颜色



	int m_nLineWidth;//线宽



	BOOL m_blTextOut;//是否处于文字输入状态

	BOOL m_blMouseTrack;//是否处于鼠标轨迹状态

	BOOL m_blScreenShots;//是否处于截图放大局面的状态



	CFont m_fontMain;//字体

	int m_nFontSize;//字体大小（字号）

	int m_cxChar;//当前字体字号下的字体宽度

	int m_cyChar;//当前字体字号下的字体高度



	CPoint m_ptCaretLineBeginning;//输入光标的位置点

	CString m_strInputText;//输入的文本

	//int m_nTextPos;//文本字符串的索引

	CString m_strFontName;//字体名称



	CRect m_rtScreenShot;

	BOOL m_blWndFloating;



	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);







};


