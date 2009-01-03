#pragma once

#include "stdafx.h"
#include "resource.h"
#include "august2002Fwd.h"

// CMyBaseForm dialog

class AUGUST2002_EXPORT CMyBaseForm : public CDialog
{
	DECLARE_DYNAMIC(CMyBaseForm)

public:
	CMyBaseForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyBaseForm();

// Dialog Data
	enum { IDD = IDD_MYBASEFORM };

protected:
  HGLOBAL initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName );
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


  HMODULE d_hModule;
  HGLOBAL d_hDialogInit; 

	DECLARE_MESSAGE_MAP()
};
