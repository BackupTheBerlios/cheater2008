// ChiterAppView.cpp : implementation of the CChiterAppView class
//

#include "stdafx.h"
#include "ChiterApp.h"

#include "ChiterAppDoc.h"
#include "ChiterAppView.h"

#include <boost/shared_ptr.hpp>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChiterAppView

IMPLEMENT_DYNCREATE(CChiterAppView, CView)

BEGIN_MESSAGE_MAP(CChiterAppView, CView)
	// Standard printing commands
  ON_WM_CREATE()
  ON_WM_SIZE()
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

    CRect streamEditrRect(0,0,0,0);
    streamEditrRect.right = streamEditrRect.left + 500;
    streamEditrRect.top = 10;
    streamEditrRect.bottom =800;
    d_streamEdit.Create(this);
    d_streamEdit.SetWindowPos ( 0, streamEditrRect.left, streamEditrRect.top, streamEditrRect.Width()+400, streamEditrRect.Height(), SWP_NOZORDER  );
    d_streamEdit.ShowWindow( SW_SHOW );

    return 0;

}

void CChiterAppView::OnSize(UINT nType,int cx,int cy)
{
  CView::OnSize(nType, cx, cy);
  // Resize d_streamEdit to fill the whole view.
  d_streamEdit.MoveWindow (0, 0, cx, cy);
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

void CChiterAppView::setStream(boost::shared_ptr<std::iostream> i_stream)
{
  d_streamEdit.setStream(i_stream);
}

#endif //_DEBUG


// CChiterAppView message handlers
