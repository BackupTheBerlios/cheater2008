// ChiterAppView.cpp : implementation of the CChiterAppView class
//

#include "stdafx.h"
#include "ChiterApp.h"

#include "ChiterAppDoc.h"
#include "ChiterAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChiterAppView

IMPLEMENT_DYNCREATE(CChiterAppView, CView)

BEGIN_MESSAGE_MAP(CChiterAppView, CView)
	// Standard printing commands
    ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CChiterAppView construction/destruction

CChiterAppView::CChiterAppView()
{
	// TODO: add construction code here

}

CChiterAppView::~CChiterAppView()
{
}

BOOL CChiterAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

int CChiterAppView::OnCreate(LPCREATESTRUCT lpcs)
{
    if(CView::OnCreate( lpcs) == -1 )
        return -1;

    d_valueBox.Create (this);
    d_valueBox.SetWindowPos (NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    d_valueBox.ShowWindow(SW_SHOW);
    CRect valueRect;
    d_valueBox.GetClientRect(valueRect);
    d_countEdit.Create (this);
    d_countEdit.SetWindowPos (&d_valueBox, valueRect.left, valueRect.bottom, 100, 50, SWP_NOZORDER | SWP_NOSIZE);
    d_countEdit.ShowWindow(SW_SHOW);

    

    return 0;

}



// CChiterAppView drawing

void CChiterAppView::OnDraw(CDC* /*pDC*/)
{
	CChiterAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CChiterAppView printing

BOOL CChiterAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CChiterAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CChiterAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CChiterAppView diagnostics

#ifdef _DEBUG
void CChiterAppView::AssertValid() const
{
	CView::AssertValid();
}

void CChiterAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChiterAppDoc* CChiterAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChiterAppDoc)));
	return (CChiterAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CChiterAppView message handlers
