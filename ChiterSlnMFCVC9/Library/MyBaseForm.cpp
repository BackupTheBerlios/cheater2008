// MyBaseForm.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "MyBaseForm.h"

extern HINSTANCE hInstance;
// CMyBaseForm dialog

IMPLEMENT_DYNAMIC(CMyBaseForm, CBase)

BEGIN_MESSAGE_MAP(CMyBaseForm, CBase)
END_MESSAGE_MAP()


/*CMyBaseForm::CMyBaseForm(CWnd* pParent )
	: CDialog(CMyBaseForm::IDD, pParent)
{

}
*/
CMyBaseForm::~CMyBaseForm()
{
}

void CMyBaseForm::DoDataExchange(CDataExchange* pDX)
{
	CBase::DoDataExchange(pDX);
}

CMyBaseForm::CMyBaseForm(CWnd* pParent):
d_hDialogInit(0),
d_hModule(0)
{
  d_IDD = IDD;
  VERIFY( InitModalIndirect( initDialog(hInstance,MAKEINTRESOURCE(d_IDD)), pParent ) );

}


CMyBaseForm::CMyBaseForm(CWnd* pParent,UINT i_IDD):
d_hDialogInit(0),
d_hModule(0)
{
  if(i_IDD)
    d_IDD = i_IDD;
  VERIFY( InitModalIndirect( initDialog(hInstance,MAKEINTRESOURCE(d_IDD)), pParent ) );

}

void CMyBaseForm::Create( CWnd* pParentWnd )
{
  CreateIndirect( initDialog(hInstance,MAKEINTRESOURCE(d_IDD)), pParentWnd );

  //CDialog::Create( MAKEINTRESOURCE(CMemoryInfoEdit::IDD), wnd );
}

HGLOBAL CMyBaseForm::initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName )
{
  d_hModule = hinst;


  d_hDialogInit = NULL;
  HRSRC hDlgInit = ::FindResource( hinst, lpszTemplateName, RT_DLGINIT);
  if( hDlgInit )
  {
    // load it
    d_hDialogInit = LoadResource( hinst, hDlgInit );
    if( d_hDialogInit == NULL )
    {
      TRACE0("Warning: Load of RT_DLGINIT failed during dialog init.\n");
    }
    else
    {
      // lock it
      m_lpDialogInit = LockResource(d_hDialogInit);
      ASSERT(m_lpDialogInit != NULL);
    }
  }

  // load dialog
  HRSRC hrsrc = FindResource( hinst, lpszTemplateName, RT_DIALOG );
  if( hrsrc != 0 )
    return LoadResource( hinst, hrsrc );
  else
    return 0;
}




// CMyBaseForm message handlers
