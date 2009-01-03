#pragma once

#include "stdafx.h"
#include "resource.h"
// CMyBaseForm dialog

class CMyBaseForm : public CDialog
{
	DECLARE_DYNAMIC(CMyBaseForm)

public:
	CMyBaseForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyBaseForm();

// Dialog Data
	enum { IDD = IDD_MYBASEFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
