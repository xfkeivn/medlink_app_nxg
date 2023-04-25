#pragma once

#include  "MessageProtocal.h"
// CDrawLayerDlg 对话框
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

typedef vector<CPoint> Points;

typedef struct{
	Points points;
	int sickness;
	COLORREF color;
}PolyLine;


typedef vector<PolyLine> PolyLines;

class CVideoDlg;
class CDrawLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDrawLayerDlg)

public:
	CDrawLayerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDrawLayerDlg();

// 对话框数据

	enum { IDD = IDD_DIALOG_DRAW_LAYER };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnPaint();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	//afx_msg void OnOperationClose();

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
	void ClearScreen(CDC *pDC);

public:


	

	void KeepNoTransparent(CDC *pDC);

	void processRemoteAnnotationEvent(RTMMessagEvent &messageEvent);
	void processSelfAnnotationEvent();
	void CleanScreen();
	void onCleanScreenBtnClicked();
	
	void GetVisualWindow(int winWidth, int winHeight, int *visualWidth, int *visualHeight, int *widpadding, int *heightpadding);
	bool IsInVisualWindow(int posx, int posy);
	bool CalAnnotationPos(int sourcePosx, int sourcePosy, int sourceWidth, int sourceHeight, LONG *posx, LONG *posy);
public:

	
	PolyLines historyLines;

	PolyLine  currentLine;

	CPoint m_ptPrev;//鼠标移动时点（之前的点）

	CPoint m_ptCurrent;//鼠标移动时点（当前的点）

	BOOL m_blMouseDown;//鼠标左键处于按下的状态标志

	COLORREF m_clrLineColor;//线颜色

	COLORREF m_clrFontColor;//字体颜色

	int m_nLineWidth;//线宽

	BOOL m_blMouseTrack;//是否处于鼠标轨迹状态

	BOOL m_blWndFloating;

	int visualWidth, visualHeight, widthPadding, heightPadding;


};
