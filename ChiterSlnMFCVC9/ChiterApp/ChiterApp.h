// ChiterApp.h : main header file for the ChiterApp application
//
#pragma once

#include "Library/AuxiliaryTypedefs.h"
#include "Library/Command.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <string>

// CChiterAppApp:
// See ChiterApp.cpp for the implementation of this class
//

//forward declaration
class  CChiterAppApp;
typedef void (CChiterAppApp::*CChiterAppAppMethod) ( void );


class CChiterAppApp : public CWinApp
{
public:
	CChiterAppApp();

  void openFile(const std::string& i_name);
  void openProcess(const std::string& i_name);

  // Overrides
private:
  CFrameWnd* prepareFrame();

  CommandsContainer d_menuCommands;

  TMenu* d_menu;
  TMenu& getMainMenu();

  void appendMenuItem(TMenu& menu,
    CommandsContainer& commandsContainer,
    CChiterAppAppMethod method,
    const std::string& i_caption);

  void OnProcessNew();
  void initializeMainMenu(); 
public:
	virtual BOOL InitInstance();

  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
// Implementation
	afx_msg void OnAppAbout();
  afx_msg void OnFileNew();
  afx_msg void OnProcessView();
  
	DECLARE_MESSAGE_MAP()
};

extern CChiterAppApp theApp;