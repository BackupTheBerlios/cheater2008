// ChiterApp.h : main header file for the ChiterApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <string>

// CChiterAppApp:
// See ChiterApp.cpp for the implementation of this class
//

class CChiterAppApp : public CWinApp
{
public:
	CChiterAppApp();

  void openFile(const std::string& i_name);
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
  afx_msg void OnFileNew();
  afx_msg void OnProcessView();
  
	DECLARE_MESSAGE_MAP()
};

extern CChiterAppApp theApp;