//==========================================================
// Author: Borodenko Oleg
// 28/10/2008 <oktamail@gmail.com>
//==========================================================
// 
// http://www.codeproject.com/KB/splitter/SplitterCtrl.aspx
// The Code Project Open License (CPOL) 1.02
// Modified 2009-02-02 Sergey Geyn
/////////////////////////////////////////////////////////////////////////////// 
// 

#include "stdafx.h"
#include "SplitterCtrl.h"

namespace SplitterCtrl_NS
{
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // SplitterCtrl.
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  IMPLEMENT_DYNCREATE(SplitterCtrl,SplitterCtrlBase)
    /////////////////////////////////////////////////////////////////////////////
    // 
    SplitterCtrl::SplitterCtrl()
  {	
    SetRecalcManager(this);
    SetDrawManager(this);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  int SplitterCtrl::GetBorderWidth(ISplitterCtrlRecalc *base) const
  {	
    return base->GetBorderWidth(NULL);
  }
  // 
  int SplitterCtrl::GetSplitterWidth(ISplitterCtrlRecalc *base) const
  {	
    return base->GetSplitterWidth(NULL) + 2/*inner borders*/;
  }
  // 
  int SplitterCtrl::GetSplitterHeight(ISplitterCtrlRecalc *base) const
  {	
    return base->GetSplitterHeight(NULL) + 2/*inner borders*/;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  COLORREF SplitterCtrl::GetSplitterColor()
  {	
    return ::GetSysColor(COLOR_WINDOW);
  }
  // 
  COLORREF SplitterCtrl::GetInnerBorderColor()
  {	
    return ::GetSysColor(COLOR_BTNSHADOW);
  }
  // 
  COLORREF SplitterCtrl::GetOuterBorderColor()
  {	
    return ::GetSysColor(COLOR_BTNSHADOW);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrl::DrawSplitter(CDC *pDC, bool /*horz*/, int /*idx*/, CRect const *pRect)
  {	
    pDC->FillSolidRect(pRect,GetSplitterColor());
  }
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrl::DrawDragRect(CDC *pDC, bool horz, bool firstTime, CRect const *pRectOld, CRect const *pRectNew)
  {	
    CRect rcOld=*pRectOld, rcNew=*pRectNew;
    if(horz==true)
    {	
      rcOld.DeflateRect(0,1);
      rcNew.DeflateRect(0,1);
    }
    else
    {	
      rcOld.DeflateRect(1,0);
      rcNew.DeflateRect(1,0);
    }
    pDC->DrawDragRect(&rcNew,CSize(rcNew.Width(),rcNew.Height()),
      (firstTime==true ? NULL : &rcOld),CSize(rcOld.Width(),rcOld.Height()));
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrl::DrawBorder(CDC *pDC, CRect const *pRect)
  {	COLORREF clr = GetOuterBorderColor();
  pDC->Draw3dRect(pRect,clr,clr);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrl::DrawEnd(CDC *pDC)
  {	
    CRect rc;
    SPLITTERCTRL_SNAP snap = GetSnapMode();
    int row = GetCountRow();
    int col = GetCountColumn();
    // 
    CPen penBorder(PS_SOLID,1,GetInnerBorderColor());
    CPen* pOldPen = pDC->SelectObject(&penBorder);
    // 
    for(int r=0; r<row; ++r)
      for(int c=0; c<col; ++c)
      {	GetWindowRect(r,c,&rc);
    rc.left--;
    rc.top--;
    // top.
    if((r>0 && (snap==SPLITTERCTRL_SNAP_LEFT_TOP || snap==SPLITTERCTRL_SNAP_RIGHT_TOP)) ||
      (r<row-1 && (snap==SPLITTERCTRL_SNAP_LEFT_BOTTOM || snap==SPLITTERCTRL_SNAP_RIGHT_BOTTOM)))
    {
      pDC->MoveTo(rc.left, rc.top);
      pDC->LineTo(rc.right, rc.top);
    }
    // bottom.
    if((r<row-1 && (snap==SPLITTERCTRL_SNAP_LEFT_TOP || snap==SPLITTERCTRL_SNAP_RIGHT_TOP)) ||
      (r>0 && (snap==SPLITTERCTRL_SNAP_LEFT_BOTTOM || snap==SPLITTERCTRL_SNAP_RIGHT_BOTTOM)))
    {
      pDC->MoveTo(rc.left, rc.bottom);
      pDC->LineTo(rc.right, rc.bottom);
    }
    // left.
    if((c>0 && (snap==SPLITTERCTRL_SNAP_LEFT_TOP || snap==SPLITTERCTRL_SNAP_LEFT_BOTTOM)) ||
      (c<col-1 && (snap==SPLITTERCTRL_SNAP_RIGHT_TOP || snap==SPLITTERCTRL_SNAP_RIGHT_BOTTOM)))
    {
      pDC->MoveTo(rc.left, rc.top);
      pDC->LineTo(rc.left, rc.bottom);
    }
    // right.
    if((c<col-1 && (snap==SPLITTERCTRL_SNAP_LEFT_TOP || snap==SPLITTERCTRL_SNAP_LEFT_BOTTOM)) ||
      (c>0 && (snap==SPLITTERCTRL_SNAP_RIGHT_TOP || snap==SPLITTERCTRL_SNAP_RIGHT_BOTTOM)))
    {
      pDC->MoveTo(rc.right, rc.top);
      pDC->LineTo(rc.right, rc.bottom+1);
    }
    }
    // 
    pDC->SelectObject(pOldPen);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

} // namespace SplitterCtrl_NS
