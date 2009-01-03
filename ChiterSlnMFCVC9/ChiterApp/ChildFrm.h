// ChildFrm.h : interface of the CChildFrame class
//


#pragma once
#include "Library/InputGroupBox.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
private:

public:
	CChildFrame();

// Attributes
public:

// Operations
public:
// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL Create(LPCTSTR lpszClassName,
        LPCTSTR lpszWindowName,
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        const RECT& rect = rectDefault,
        CMDIFrameWnd* pParentWnd = NULL,
        CCreateContext* pContext = NULL);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
