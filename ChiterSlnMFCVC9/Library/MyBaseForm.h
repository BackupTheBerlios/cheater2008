#pragma once

#include "stdafx.h"
#include "resource.h"
#include "august2002Fwd.h"

// CMyBaseForm dialog
//typedef CFrameWnd CBase ;
typedef CDialog CBase ;
class AUGUST2002_EXPORT CMyBaseForm : public CBase
{
	DECLARE_DYNAMIC(CMyBaseForm)

public:
  CMyBaseForm(CWnd* pParent);
	CMyBaseForm(CWnd* pParent,UINT i_IDD);   // standard constructor
	virtual ~CMyBaseForm();

// Dialog Data
	enum { IDD = IDD_MYBASEFORM };
  void Create( CWnd* pParentWnd );

protected:
  HGLOBAL initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName );
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  UINT d_IDD;
  HMODULE d_hModule;
  HGLOBAL d_hDialogInit; 

	DECLARE_MESSAGE_MAP()
};
