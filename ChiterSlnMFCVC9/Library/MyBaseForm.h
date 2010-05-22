#pragma once

#include "stdafx.h"
#include "resource.h"
#include "august2002Fwd.h"

// CMyBaseForm dialog
//#define CBase_CFrameWnd
#define CBase_CDialog
#ifdef CBase_CFrameWnd
typedef CFrameWnd CBase ;
#else
typedef CDialog CBase ;
#endif

//typedef CDialog CBase ;
class AUGUST2002_EXPORT CMyBaseForm : public CBase
{
  DECLARE_DYNAMIC(CMyBaseForm)

  DECLARE_MESSAGE_MAP()
public:

  CMyBaseForm(CWnd* pParent);
  /*
  CMyBaseForm(CWnd* pParent,UINT i_IDD);   // standard constructor
  */
  virtual ~CMyBaseForm();

  // Dialog Data
  enum { IDD = IDD_MYBASEFORM };
  void Create( CWnd* pParentWnd );

protected:
#ifdef CBase_CDialog
  HGLOBAL initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName );
#endif
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  //! @brief c++/windows exceptions guard wrapper
  //! Wrapper over PlugUI_NS::CPlugDlg::OnCmdMsg to forbid c++/windows exceptions propagation beyound MFC execption handlers
  //! Does not throw, neither c++ no windows exception
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
    AFX_CMDHANDLERINFO* pHandlerInfo);

private:
  BOOL OnCmdMsgNoCppException( UINT nID, int nCode, void* pExtra,
    AFX_CMDHANDLERINFO* pHandlerInfo);

  UINT d_IDD;
  HMODULE d_hModule;
  HGLOBAL d_hDialogInit; 

};
