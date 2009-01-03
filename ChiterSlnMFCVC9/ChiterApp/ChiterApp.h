// ChiterApp.h : main header file for the ChiterApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CChiterAppApp:
// See ChiterApp.cpp for the implementation of this class
//

class CChiterAppApp : public CWinApp
{
public:
	CChiterAppApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChiterAppApp theApp;