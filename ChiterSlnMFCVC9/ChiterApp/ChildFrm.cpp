// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "ChiterApp.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName,
                    LPCTSTR lpszWindowName,
                    DWORD dwStyle/* = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW*/,
                    const RECT& rect /*= rectDefault*/,
                    CMDIFrameWnd* pParentWnd/* = NULL*/,
                    CCreateContext* pContext/* = NULL*/)
{
    CMDIChildWnd::Create(lpszClassName,
        lpszWindowName,
        dwStyle,
        rect,
        pParentWnd,
        pContext);
    /*
    d_valueBox.Create (this);
    d_valueBox.SetWindowPos (NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    d_valueBox.ShowWindow(SW_SHOW);
    */
    //d_valueBox.DoModal();
    return TRUE;
}

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_LBUTTONDOWN:
        /*{
            // Check if it is to edit box
            CRect cRect;
            d_valueBox.GetWindowRect(cRect);
            if(cRect.PtInRect(pMsg->pt))
                d_valueBox.SetFocus();
            else
                SetFocus();
        }*/
        break;
    case WM_LBUTTONDBLCLK:
        {
            // Check if it is to edit box
           /* CRect cRect;
            d_valueBox.GetWindowRect(cRect);
            if(cRect.PtInRect(pMsg->pt))
                d_valueBox.SetFocus();
            else
                SetFocus();*/
        }
        break;
    }

    
    return CMDIChildWnd::PreTranslateMessage(pMsg);
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers
