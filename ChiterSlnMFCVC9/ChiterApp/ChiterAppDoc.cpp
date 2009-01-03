// ChiterAppDoc.cpp : implementation of the CChiterAppDoc class
//

#include "stdafx.h"
#include "ChiterApp.h"

#include "ChiterAppDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChiterAppDoc

IMPLEMENT_DYNCREATE(CChiterAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CChiterAppDoc, CDocument)
END_MESSAGE_MAP()


// CChiterAppDoc construction/destruction

CChiterAppDoc::CChiterAppDoc()
{
	// TODO: add one-time construction code here

}

CChiterAppDoc::~CChiterAppDoc()
{
}

BOOL CChiterAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CChiterAppDoc serialization

void CChiterAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CChiterAppDoc diagnostics

#ifdef _DEBUG
void CChiterAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChiterAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CChiterAppDoc commands
