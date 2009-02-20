// ChiterAppView.h : interface of the CChiterAppView class
//


#pragma once
#include "Library/InputGroupBox.h"
#include "Library/CountEdit.h"
#include "Library/PointersViewBox.h"
#include "Library/SearcherProperties.h"
#include "Library/StreamEdit.h"

#include <boost/shared_ptr.hpp>
#include <iostream>

class CChiterAppView : public CView
{
protected: // create from serialization only
	CChiterAppView();
	DECLARE_DYNCREATE(CChiterAppView)
	
  TStreamEdit d_streamEdit;

// Attributes
public:
	CChiterAppDoc* GetDocument() const;
  void setStream(boost::shared_ptr<std::iostream>);
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnSize( UINT, int, int );
// Implementation
public:
	virtual ~CChiterAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ChiterAppView.cpp
inline CChiterAppDoc* CChiterAppView::GetDocument() const
   { return reinterpret_cast<CChiterAppDoc*>(m_pDocument); }
#endif

