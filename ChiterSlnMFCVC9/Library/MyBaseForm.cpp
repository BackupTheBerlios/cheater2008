// MyBaseForm.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "MyBaseForm.h"


// CMyBaseForm dialog

IMPLEMENT_DYNAMIC(CMyBaseForm, CDialog)

CMyBaseForm::CMyBaseForm(CWnd* pParent /*=NULL*/)
	: CDialog(CMyBaseForm::IDD, pParent)
{

}

CMyBaseForm::~CMyBaseForm()
{
}

void CMyBaseForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyBaseForm, CDialog)
END_MESSAGE_MAP()


// CMyBaseForm message handlers
