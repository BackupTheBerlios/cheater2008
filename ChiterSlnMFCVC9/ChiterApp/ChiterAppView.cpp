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

    d_valueBox.Create ( this );
    d_valueBox.SetWindowPos (NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    d_valueBox.ShowWindow( SW_SHOW );
    CRect valueRect;
    d_valueBox.GetClientRect( valueRect );
    d_valueBox.MapWindowPoints(this,valueRect);
    valueRect.MoveToY(valueRect.bottom);
    d_countEdit.Create ( this );
    d_countEdit.SetWindowPos ( 0, valueRect.left, valueRect.top, 100, 50, SWP_NOZORDER | SWP_NOSIZE);
    d_countEdit.ShowWindow( SW_SHOW );

    d_ponterViewBox.Create( this );
    CRect ponterViewBoxRect;
    d_countEdit.GetClientRect(ponterViewBoxRect);
    d_countEdit.MapWindowPoints(this,ponterViewBoxRect);
    ponterViewBoxRect.MoveToY(ponterViewBoxRect.bottom + 4);
    ponterViewBoxRect.right = ponterViewBoxRect.left + 400;
    ponterViewBoxRect.bottom = ponterViewBoxRect.top + 400;
    d_ponterViewBox.SetWindowPos ( 0, ponterViewBoxRect.left, ponterViewBoxRect.top, ponterViewBoxRect.Width(), ponterViewBoxRect.Height(), SWP_NOZORDER );
    d_ponterViewBox.ShowWindow( SW_SHOW );

    CRect searcherRect;
    d_ponterViewBox.GetClientRect( searcherRect );
    d_ponterViewBox.MapWindowPoints(this,searcherRect);
    searcherRect.MoveToX( searcherRect.right );
    searcherRect.right = searcherRect.left + 700;
    searcherRect.top = 10;
    searcherRect.bottom =900;
    d_searcherProperties.Create(this);
    d_searcherProperties.SetWindowPos ( 0, searcherRect.left, searcherRect.top, searcherRect.Width(), searcherRect.Height(), SWP_NOZORDER  );
    d_searcherProperties.ShowWindow( SW_SHOW );

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
