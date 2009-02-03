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
#ifndef SplitterCtrl_SplitterCtrl_h
#define SplitterCtrl_SplitterCtrl_h

#include "SplitterCtrl/SplitterCtrlFwd.h"
#include "SplitterCtrl/SplitterCtrlUtil.h"

namespace SplitterCtrl_NS
{

  class SPLITTERCTRL_EXPORT SplitterCtrl : virtual public SplitterCtrlBase, 
    public ISplitterCtrlRecalc, public SplitterCtrlDraw
  {
  public:
    SplitterCtrl();

  private:
    DECLARE_DYNCREATE(SplitterCtrl)

  protected:	// ISplitterCtrlRecalc.
    virtual int GetSplitterWidth(ISplitterCtrlRecalc *base) const;
    virtual int GetSplitterHeight(ISplitterCtrlRecalc *base) const;
    virtual int GetBorderWidth(ISplitterCtrlRecalc *base) const;

  protected:	// SplitterCtrlDraw.
    virtual void DrawSplitter(CDC *pDC, bool horz, int idx, CRect const *pRect);
    virtual void DrawDragRect(CDC *pDC, bool horz, bool firstTime, CRect const *pRectOld, CRect const *pRectNew);
    virtual void DrawBorder(CDC *pDC, CRect const *pRect);
    virtual void DrawEnd(CDC *pDC);

  protected:
    virtual COLORREF GetSplitterColor();
    virtual COLORREF GetInnerBorderColor();
    virtual COLORREF GetOuterBorderColor();
  };


  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  class SPLITTERCTRL_EXPORT SplitterCtrlAggregate : 
    public SplitterCtrl, public SplitterCtrlCustom1, public SplitterCtrlCustom2
  {	
  };

} // namespace SplitterCtrl_NS



#endif //! SplitterCtrl_SplitterCtrl_h
