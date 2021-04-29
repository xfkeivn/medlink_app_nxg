
// OpenVideoCall.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAgoraVideoCallApp:
// See AgoraVideoCall.cpp for the implementation of this class
//

class CAgoraMediaSourceApp : public CWinApp
{
public:
	CAgoraMediaSourceApp();
    ~CAgoraMediaSourceApp();
// Overrides
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();
// Implementation
	COLORREF m_clrWndTransparencyColorKey;

	COLORREF m_clrWndNoTransparencyColorKey;
	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hMutex;
};

extern CAgoraMediaSourceApp theApp;