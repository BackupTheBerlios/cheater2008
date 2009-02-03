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
#include "SplitterCtrlUtil.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// VirtualWindowClient.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
namespace SplitterCtrl_NS
{
  class VirtualWindowClient : public CDC
  {public:
  VirtualWindowClient(CWnd *pWnd)
  {	_ASSERTE(pWnd!=NULL && ::IsWindow(pWnd->m_hWnd)!=0);
  pWnd->GetClientRect(&rect);
  success = Init(pWnd);
  }
  VirtualWindowClient(CWnd *pWnd, CRect const *rc)
  {	_ASSERTE(pWnd!=NULL && ::IsWindow(pWnd->m_hWnd)!=0);
  _ASSERTE(rc!=NULL);
  rect = *rc;
  success = Init(pWnd);
  }
  ~VirtualWindowClient()
  {	_ASSERTE(::IsWindow(pwnd->m_hWnd)!=0);
  // 
  if(success==true)
    pdc->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(), this,rect.left,rect.top, SRCCOPY);
  pwnd->EndPaint(&ps);
  }

  public:
    bool IsSuccess() const
    {	return success;
    }

  private:
    bool Init(CWnd *pWnd)
    {	pwnd = pWnd;
    pdc = pwnd->BeginPaint(&ps);
    if(CreateCompatibleDC(pdc)==0 || bitmap.CreateCompatibleBitmap(pdc,rect.Width(),rect.Height())==0)
      return false;
    SelectObject(&bitmap);
    SetBkMode(TRANSPARENT);
    return true;
    }

  private:
    CWnd *pwnd;
    CRect rect;
    PAINTSTRUCT ps;
    CDC *pdc;
    CBitmap bitmap;
    bool success;
  };
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // SplitterCtrlBase.
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  IMPLEMENT_DYNCREATE(SplitterCtrlBase,CWnd)
    /////////////////////////////////////////////////////////////////////////////
    BEGIN_MESSAGE_MAP(SplitterCtrlBase, CWnd)
      ON_WM_NCCALCSIZE()
      ON_WM_NCPAINT()
      ON_WM_PAINT()
      ON_WM_NCHITTEST()
      ON_WM_SIZE()
      ON_WM_SETCURSOR()
      ON_WM_CAPTURECHANGED()
      ON_WM_NCLBUTTONDOWN()
      ON_WM_MOUSEMOVE()
      ON_WM_LBUTTONUP()
      ON_WM_RBUTTONDOWN()
      ON_WM_MBUTTONDOWN()
      ON_WM_LBUTTONDBLCLK()
      ON_WM_NCLBUTTONDBLCLK()
    END_MESSAGE_MAP()
    /////////////////////////////////////////////////////////////////////////////
    // 
    SplitterCtrlBase::SplitterCtrlBase()
  {	m_hCurHorz = m_hCurVert = m_hCurHorzVert = NULL;
  m_bDragHorz = m_bDragVert = m_bDragHorzVert = false;
  m_SnapMode = SPLITTERCTRL_SNAP_LEFT_TOP;
  m_ResizeMode = SPLITTERCTRL_RESIZE_STATIC;
  m_DraggingMode = SPLITTERCTRL_DRAGGING_STATIC;
  m_iMinWidth = m_iMinHeight = 0;
  m_bShowBorder = true;
  // 
  m_pDrawManager = NULL;
  m_pRecalcManager = &m_Service;
  // 
  m_iTotalWidth = m_iTotalHeight = 0;
  }
  // 
  SplitterCtrlBase::~SplitterCtrlBase()
  {	if(m_hCurHorz!=NULL) ::DestroyCursor(m_hCurHorz);
  if(m_hCurVert!=NULL) ::DestroyCursor(m_hCurVert);
  if(m_hCurHorzVert!=NULL) ::DestroyCursor(m_hCurHorzVert);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
#define DEFAULT_WIDTH	100
#define DEFAULT_HEIGHT	100
  /////////////////////////////////////////////////////////////////////////////
  // 
  BOOL SplitterCtrlBase::Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* /*pContext*/)
  {	
    return Create(pParentWnd,dwStyle,rect,nID)==true ? 1 : 0;
  }
  // 
  bool SplitterCtrlBase::Create(CWnd *pParentWnd, DWORD dwStyle, RECT const &rect, UINT uID)
  {	
    CString classname = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(NULL,IDC_ARROW),NULL,NULL);
    if(CWnd::Create(classname,_T(""),dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,rect,pParentWnd,uID)==0) return false;
    // 
    SetCursors(::LoadCursor(NULL,IDC_SIZENS),::LoadCursor(NULL,IDC_SIZEWE),::LoadCursor(NULL,IDC_SIZEALL));
    SetWindowPos(0, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS);
    // 
    return true;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetCursors(UINT uIdHorz, UINT uIdVert, UINT uIdHorzVert)
  {	if(m_hCurHorz!=NULL) ::DestroyCursor(m_hCurHorz);
  if(m_hCurVert!=NULL) ::DestroyCursor(m_hCurVert);
  if(m_hCurHorzVert!=NULL) ::DestroyCursor(m_hCurHorzVert);
  // 
  if(uIdHorz!=0) m_hCurHorz = ::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(uIdHorz));
  if(uIdVert!=0) m_hCurVert = ::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(uIdVert));
  if(uIdHorzVert!=0) m_hCurHorzVert = ::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(uIdHorzVert));
  }
  // 
  void SplitterCtrlBase::SetCursors(HCURSOR hHorz, HCURSOR hVert, HCURSOR hHorzVert)
  {	if(m_hCurHorz!=NULL) ::DestroyCursor(m_hCurHorz);
  if(m_hCurVert!=NULL) ::DestroyCursor(m_hCurVert);
  if(m_hCurHorzVert!=NULL) ::DestroyCursor(m_hCurHorzVert);
  // 
  m_hCurHorz = hHorz;
  m_hCurVert = hVert;
  m_hCurHorzVert = hHorzVert;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  SplitterCtrlBase::CELL *SplitterCtrlBase::GetCell(int row, int col)
  {	return m_matrix.GetCell(row,col);
  }
  // 
  SplitterCtrlBase::CELL const *SplitterCtrlBase::GetCell(int row, int col) const
  {	return m_matrix.GetCell(row,col);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  bool SplitterCtrlBase::AddRow()
  {	if(m_matrix.AddRow()==false) return false;
  // 
  int row = GetCountRow();
  int col = GetCountColumn();
  CELL *p1, *p2;
  // 
  if(row==1)	// it is first addition of row.
    for(int c=0; c<col; ++c)
    {	p1 = GetCell(0,c);
  p1->real.width = DEFAULT_WIDTH;
  // 
  if(c==0)
    p1->real.factorWidth = 1.0;
  else
  {	p2 = GetCell(0,c-1);
  p1->real.factorWidth = p2->real.factorWidth/2.0;
  p2->real.factorWidth -= p1->real.factorWidth;
  }
  // 
  p1->real.height = DEFAULT_HEIGHT;
  p1->real.factorHeight = 1.0;
  p1->hWnd = NULL;
  p1->activeRow = p1->activeCol = true;
  }
  else
    for(int c=0; c<col; ++c)
    {	p1 = GetCell(row-1,c);	// just added item.
  p2 = GetCell(row-2,c);	// item before it.
  p1->real.width = p2->real.width;
  p1->real.factorWidth = p2->real.factorWidth;
  p1->real.height = DEFAULT_HEIGHT;
  p1->real.factorHeight = p2->real.factorHeight/2.0;
  p2->real.factorHeight -= p1->real.factorHeight;
  p1->hWnd = NULL;
  p1->activeRow = p1->activeCol = true;
  }
  // 
  return true;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  bool SplitterCtrlBase::AddColumn()
  {	if(m_matrix.AddColumn()==false) return false;
  // 
  int row = GetCountRow();
  int col = GetCountColumn();
  CELL *p1, *p2;
  // 
  if(col==1)	// it is first addition of column.
    for(int r=0; r<row; ++r)
    {	p1 = GetCell(r,0);
  p1->real.width = DEFAULT_WIDTH;
  p1->real.factorWidth = 1.0;
  // 
  if(r==0)
    p1->real.factorHeight = 1.0;
  else
  {	p2 = GetCell(r-1,0);
  p1->real.factorHeight = p2->real.factorHeight/2.0;
  p2->real.factorHeight -= p1->real.factorHeight;
  }
  p1->real.height = DEFAULT_HEIGHT;
  p1->hWnd = NULL;
  p1->activeRow = p1->activeCol = true;
  }
  else
    for(int r=0; r<row; ++r)
    {	p1 = GetCell(r,col-1);	// just added item.
  p2 = GetCell(r,col-2);	// item before it.
  p1->real.width = DEFAULT_WIDTH;
  p1->real.factorWidth = p2->real.factorWidth/2.0;
  p2->real.factorWidth -= p1->real.factorWidth;
  p1->real.height = p2->real.height;
  p1->real.factorHeight = p2->real.factorHeight;
  p1->hWnd = NULL;
  p1->activeRow = p1->activeCol = true;
  }
  // 
  return true;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  bool SplitterCtrlBase::InsertRow(int r)
  {	if(m_matrix.InsertRow(r)==false) return false;
  // 
  int col = GetCountColumn();
  CELL *p1, *p2, *p3;
  // 
  for(int c=0; c<col; ++c)
  {	p1 = GetCell(r,c);
  p2 = GetCell(r+1,c);
  p1->real.width = p2->real.width;
  p1->real.factorWidth = p2->real.factorWidth;
  // 
  if(r==0)
  {	p1->real.factorHeight = p2->real.factorHeight/2.0;
  p2->real.factorHeight -= p1->real.factorHeight;
  }
  else
  {	p3 = GetCell(r-1,c);
  double totalfactor = p3->real.factorHeight + p2->real.factorHeight;
  p3->real.factorHeight *= 2.0/3.0;
  p2->real.factorHeight *= 2.0/3.0;
  p1->real.factorHeight = totalfactor - p3->real.factorHeight - p2->real.factorHeight;
  }
  p1->real.height = DEFAULT_HEIGHT;
  p1->hWnd = NULL;
  p1->activeRow = p1->activeCol = true;
  }
  // 
  return true;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  bool SplitterCtrlBase::InsertColumn(int c)
  {	if(m_matrix.InsertColumn(c)==false) return false;
  // 
  int row = GetCountRow();
  CELL *p1, *p2, *p3;
  // 
  for(int r=0; r<row; ++r)
  {	p1 = GetCell(r,c);
  p2 = GetCell(r,c+1);
  p1->real.width = DEFAULT_WIDTH;
  // 
  if(c==0)
  {	p1->real.factorWidth = p2->real.factorWidth/2.0;
  p2->real.factorWidth -= p1->real.factorWidth;
  }
  else
  {	p3 = GetCell(r,c-1);
  double totalfactor = p3->real.factorWidth + p2->real.factorWidth;
  p3->real.factorWidth *= 2.0/3.0;
  p2->real.factorWidth *= 2.0/3.0;
  p1->real.factorWidth = totalfactor - p3->real.factorWidth - p2->real.factorWidth;
  }
  // 
  p1->real.height = p2->real.height;
  p1->real.factorHeight = p2->real.factorHeight;
  p1->hWnd = NULL;
  p1->activeRow = p1->activeCol = true;
  }
  // 
  return true;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::DeleteRow(int row)
  {	m_matrix.DeleteRow(row);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::DeleteColumn(int col)
  {	m_matrix.DeleteColumn(col);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::DeleteAll()
  {	m_matrix.DeleteAll();
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::OnPaint()
  {	
    if(m_pDrawManager==NULL)
    {	
      CPaintDC dc(this);
      return;
    }
    // 
    VirtualWindowClient virtwnd(this);
    if(virtwnd.IsSuccess()==false)
    {	
      CPaintDC dc(this);
      return;
    }
    // 
    m_pDrawManager->DrawBegin(&virtwnd);
    // 
    int row = GetCountRow();
    int col = GetCountColumn();
    if(row>0 && col>0)
    {	
      CRect rc;
      for(int r=0; r<row-1; ++r)
      {	
        GetSplitterRect(true,r,&rc);
        m_pDrawManager->DrawSplitter(&virtwnd,true,r,&rc);
      }
      for(int c=0; c<col-1; ++c)
      {	
        GetSplitterRect(false,c,&rc);
        m_pDrawManager->DrawSplitter(&virtwnd,false,c,&rc);
      }
    }
    // 
    m_pDrawManager->DrawEnd(&virtwnd);
  }
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrlBase::OnNcPaint()
  {	CWnd::OnNcPaint();
  // 
  if(m_bShowBorder==true && m_pDrawManager!=NULL)
  {	CWindowDC dc(this);
  CRect rc;
  CWnd::GetWindowRect(&rc);
  rc.OffsetRect(-rc.left,-rc.top);
  m_pDrawManager->DrawBorder(&dc,&rc);
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::Update()
  {	Recalc();
  Invalidate(FALSE);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrlBase::SetDrawManager(SplitterCtrlDraw *p)
  {	m_pDrawManager = p;
  }
  // 
  SplitterCtrlDraw *SplitterCtrlBase::GetDrawManager()
  {	return m_pDrawManager;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetRecalcManager(ISplitterCtrlRecalc *p)
  {	m_pRecalcManager = (p!=NULL ? p : &m_Service);
  }
  // 
  ISplitterCtrlRecalc *SplitterCtrlBase::GetRecalcManager()
  {	return m_pRecalcManager;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrlBase::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
  {	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
  // 
  int iBorderWidth = GetBorderWidth();
  if(m_bShowBorder==true) ::InflateRect(&lpncsp->rgrc[0],-iBorderWidth,-iBorderWidth);
  }
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrlBase::OnSize(UINT nType, int cx, int cy)
  {	CWnd::OnSize(nType, cx, cy);
  Recalc();
  Invalidate(FALSE);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //
  int SplitterCtrlBase::GetBorderWidth() const
  {	return m_pRecalcManager->GetBorderWidth((ISplitterCtrlRecalc *)&m_Service);
  }
  // 
  int SplitterCtrlBase::GetSplitterWidth() const
  {	return m_pRecalcManager->GetSplitterWidth((ISplitterCtrlRecalc *)&m_Service);
  }
  // 
  int SplitterCtrlBase::GetSplitterHeight() const
  {	return m_pRecalcManager->GetSplitterHeight((ISplitterCtrlRecalc *)&m_Service);
  }
  /////////////////////////////////////////////////////////////////////////////
  //
  int SplitterCtrlBase::Service::GetBorderWidth(ISplitterCtrlRecalc * /*base*/) const
  {	return 1;
  }
  // 
  int SplitterCtrlBase::Service::GetSplitterWidth(ISplitterCtrlRecalc * /*base*/) const
  {	return 4;
  }
  // 
  int SplitterCtrlBase::Service::GetSplitterHeight(ISplitterCtrlRecalc * /*base*/) const
  {	return 4;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::Recalc()
  {	int row = GetCountRow();
  int col = GetCountColumn();
  // 
  if(row>0 && col>0)
  {	CRect rc;
  GetClientRect(&rc);
  // 
  switch(m_ResizeMode)
  {	case SPLITTERCTRL_RESIZE_STATIC:
  RecalcStatic(&rc,row,col);
  break;
  case SPLITTERCTRL_RESIZE_STATICFULL:
    RecalcStaticFull(&rc,row,col);
    break;
  case SPLITTERCTRL_RESIZE_DYNAMIC:
    RecalcDynamic(&rc,row,col);
    break;
  }
  }
  }
  // 
  void SplitterCtrlBase::RecalcStatic(CRect *rc, int iCountRow, int iCountColumn)
  {	int iSplitterWidth = GetSplitterWidth();
  int iSplitterHeight = GetSplitterHeight();
  // 
  m_iTotalWidth = max(rc->Width()-(iCountColumn-1)*iSplitterWidth, iCountColumn*m_iMinWidth);
  m_iTotalHeight = max(rc->Height()-(iCountRow-1)*iSplitterHeight, iCountRow*m_iMinHeight);
  // 
  int r,c, pos, tail, cx,cy;
  CELL *p;
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	pos = rc->left;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	p = GetCell(0,c);
  tail = max(0, rc->right-pos - (iCountColumn-1-c)*(iSplitterWidth+m_iMinWidth));
  // 
  if(tail<p->real.width || c==iCountColumn-1)
    cx = tail;
  else
    cx = p->real.width;
  cx = max(cx,m_iMinWidth);
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,c);
  p->real.rc.left = pos;
  p->real.rc.right = pos+cx;
  p->real.factorWidth = (double)cx/(double)m_iTotalWidth;
  }
  pos += (cx+iSplitterWidth);
  }
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	pos = rc->right;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	p = GetCell(0,c);
  tail = max(0, pos-rc->left - (iCountColumn-1-c)*(iSplitterWidth+m_iMinWidth));
  // 
  if(tail<p->real.width || c==iCountColumn-1)
    cx = tail;
  else
    cx = p->real.width;
  cx = max(cx,m_iMinWidth);
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,c);
  p->real.rc.left = pos-cx;
  p->real.rc.right = pos;
  p->real.factorWidth = (double)cx/(double)m_iTotalWidth;
  }
  pos -= (cx+iSplitterWidth);
  }
  }
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	pos = rc->top;
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,0);
  tail = max(0, rc->bottom-pos - (iCountRow-1-r)*(iSplitterHeight+m_iMinHeight));
  // 
  if(tail<p->real.height || r==iCountRow-1)
    cy = tail;
  else
    cy = p->real.height;
  cy = max(cy,m_iMinHeight);
  // 
  for(c=0; c<iCountColumn; ++c)
  {	
    p = GetCell(r,c);
    p->real.rc.top = pos;
    p->real.rc.bottom = pos+cy;
    p->real.factorHeight = (double)cy/(double)m_iTotalHeight;
    // 
    if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
  }
  pos += (cy+iSplitterHeight);
  }
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	
    pos = rc->bottom;
    // 
    for(r=0; r<iCountRow; ++r)
    {	
      p = GetCell(r,0);
      tail = max(0, pos-rc->top - (iCountRow-1-r)*(iSplitterHeight+m_iMinHeight));
      // 
      if(tail<p->real.height || r==iCountRow-1)
        cy = tail;
      else
        cy = p->real.height;
      cy = max(cy,m_iMinHeight);
      // 
      for(c=0; c<iCountColumn; ++c)
      {	
        p = GetCell(r,c);
        p->real.rc.top = pos-cy;
        p->real.rc.bottom = pos;
        p->real.factorHeight = (double)cy/(double)m_iTotalHeight;
        // 
        if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
      }
      pos -= (cy+iSplitterHeight);
    }
  }
  }
  // 
  void SplitterCtrlBase::RecalcStaticFull(CRect *rc, int iCountRow, int iCountColumn)
  {	int iSplitterWidth = GetSplitterWidth();
  int iSplitterHeight = GetSplitterHeight();
  // 
  int r,c, pos, cx,cy;
  CELL *p;
  // 
  // 
  m_iTotalWidth=0, m_iTotalHeight=0;
  // 		
  for(c=0; c<iCountColumn; ++c)
    if(c!=iCountColumn-1)
      m_iTotalWidth += GetCell(0,c)->real.width;
    else	// last column.
      m_iTotalWidth += max(m_iMinWidth,rc->Width()-(iCountColumn-1)*iSplitterWidth-m_iTotalWidth);
  // 
  for(r=0; r<iCountRow; ++r)
    if(r!=iCountRow-1)
      m_iTotalHeight += GetCell(r,0)->real.height;
    else	// last row.
      m_iTotalHeight += max(m_iMinHeight,rc->Height()-(iCountRow-1)*iSplitterHeight-m_iTotalHeight);
  // 
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	pos = rc->left;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	if(c!=iCountColumn-1)
  cx = GetCell(0,c)->real.width;
  else	// last column.
    cx = rc->right-pos;
  // 
  cx = max(cx,m_iMinWidth);
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,c);
  p->real.rc.left = pos;
  p->real.rc.right = pos+cx;
  p->real.factorWidth = (double)cx/m_iTotalWidth;
  }
  pos += (cx+iSplitterWidth);
  }
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	pos = rc->right;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	if(c!=iCountColumn-1)
  cx = GetCell(0,c)->real.width;
  else	// last column.
    cx = pos-rc->left;
  // 
  cx = max(cx,m_iMinWidth);
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,c);
  p->real.rc.left = pos-cx;
  p->real.rc.right = pos;
  p->real.factorWidth = (double)cx/m_iTotalWidth;
  }
  pos -= (cx+iSplitterWidth);
  }
  }
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	pos = rc->top;
  // 
  for(r=0; r<iCountRow; ++r)
  {	if(r!=iCountRow-1)
  cy = GetCell(r,0)->real.height;
  else	// last row.
    cy = rc->bottom-pos;
  // 
  cy = max(cy,m_iMinHeight);
  // 
  for(c=0; c<iCountColumn; ++c)
  {	p = GetCell(r,c);
  p->real.rc.top = pos;
  p->real.rc.bottom = pos+cy;
  p->real.factorHeight = (double)cy/m_iTotalHeight;
  // 
  if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
  }
  pos += (cy+iSplitterHeight);
  }
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	pos = rc->bottom;
  // 
  for(r=0; r<iCountRow; ++r)
  {	if(r!=iCountRow-1)
  cy = GetCell(r,0)->real.height;
  else	// last row.
    cy = pos-rc->top;
  // 
  cy = max(cy,m_iMinHeight);
  // 
  for(c=0; c<iCountColumn; ++c)
  {	p = GetCell(r,c);
  p->real.rc.top = pos-cy;
  p->real.rc.bottom = pos;
  p->real.factorHeight = (double)cy/m_iTotalHeight;
  // 
  if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
  }
  pos -= (cy+iSplitterHeight);
  }
  }
  }
  // 
  void SplitterCtrlBase::RecalcDynamic(CRect *rc, int iCountRow, int iCountColumn)
  {	int iSplitterWidth = GetSplitterWidth();
  int iSplitterHeight = GetSplitterHeight();
  // 
  m_iTotalWidth = max(rc->Width()-(iCountColumn-1)*iSplitterWidth, iCountColumn*m_iMinWidth);
  m_iTotalHeight = max(rc->Height()-(iCountRow-1)*iSplitterHeight, iCountRow*m_iMinHeight);
  // 
  int iCorrectedTail, iTotalOver;
  int r,c, pos, cx,cy;
  CELL *p;
  // 
  iCorrectedTail = iTotalOver = 0;
  pos = rc->left;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	if(c!=iCountColumn-1)
  cx = (int)((double)m_iTotalWidth * GetCell(0,c)->real.factorWidth + 0.5);
  else	// last column.
    cx = rc->right-pos;
  // 
  if(cx<=m_iMinWidth)
    iCorrectedTail += m_iMinWidth-cx;
  else
    iTotalOver += cx-m_iMinWidth;
  pos += (cx+iSplitterWidth);
  }
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	pos = rc->left;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	if(c!=iCountColumn-1)
  cx = (int)((double)m_iTotalWidth * GetCell(0,c)->real.factorWidth + 0.5);
  else	// last column.
    cx = rc->right-pos;
  // 
  if(c!=iCountColumn-1)	// except last column.
    if(cx>m_iMinWidth)
      cx -= (int)((float)iCorrectedTail * ((float)(cx-m_iMinWidth)/(float)iTotalOver) + 0.5f);
  cx = max(cx,m_iMinWidth);
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,c);
  p->real.rc.left = pos;
  p->real.rc.right = pos+cx;
  p->real.width = cx;
  }
  pos += (cx+iSplitterWidth);
  }
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	pos = rc->right;
  // 
  for(c=0; c<iCountColumn; ++c)
  {	if(c!=iCountColumn-1)
  cx = (int)((double)m_iTotalWidth * GetCell(0,c)->real.factorWidth + 0.5);
  else	// last column.
    cx = pos-rc->left;
  // 
  if(c!=iCountColumn-1)	// except last column.
    if(cx>m_iMinWidth)
      cx -= (int)((float)iCorrectedTail * ((float)(cx-m_iMinWidth)/(float)iTotalOver) + 0.5f);
  cx = max(cx,m_iMinWidth);
  // 
  for(r=0; r<iCountRow; ++r)
  {	p = GetCell(r,c);
  p->real.rc.left = pos-cx;
  p->real.rc.right = pos;
  p->real.width = cx;
  }
  pos -= (cx+iSplitterWidth);
  }
  }
  // 
  // 
  iCorrectedTail = iTotalOver = 0;
  pos = rc->top;
  // 
  for(r=0; r<iCountRow; ++r)
  {	if(r!=iCountRow-1)
  cy = (int)((double)m_iTotalHeight * GetCell(r,0)->real.factorHeight + 0.5);
  else	// last row.
    cy = rc->bottom-pos;
  // 
  if(cy<=m_iMinHeight)
    iCorrectedTail += m_iMinHeight-cy;
  else
    iTotalOver += cy-m_iMinHeight;
  pos += (cy+iSplitterHeight);
  }
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	pos = rc->top;
  // 
  for(r=0; r<iCountRow; ++r)
  {	if(r!=iCountRow-1)
  cy = (int)((double)m_iTotalHeight * GetCell(r,0)->real.factorHeight + 0.5);
  else	// last row.
    cy = rc->bottom-pos;
  // 
  if(r!=iCountRow-1)	// except last row.
    if(cy>m_iMinHeight)
      cy -= (int)((float)iCorrectedTail * ((float)(cy-m_iMinHeight)/(float)iTotalOver) + 0.5f);
  cy = max(cy,m_iMinHeight);
  // 
  for(c=0; c<iCountColumn; ++c)
  {	p = GetCell(r,c);
  p->real.rc.top = pos;
  p->real.rc.bottom = pos+cy;
  p->real.height = cy;
  // 
  if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
  }
  pos += (cy+iSplitterHeight);
  }
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	pos = rc->bottom;
  // 
  for(r=0; r<iCountRow; ++r)
  {	if(r!=iCountRow-1)
  cy = (int)((double)m_iTotalHeight * GetCell(r,0)->real.factorHeight + 0.5);
  else	// last row.
    cy = pos-rc->top;
  // 
  if(r!=iCountRow-1)	// except last row.
    if(cy>m_iMinHeight)
      cy -= (int)((float)iCorrectedTail * ((float)(cy-m_iMinHeight)/(float)iTotalOver) + 0.5f);
  cy = max(cy,m_iMinHeight);
  // 
  for(c=0; c<iCountColumn; ++c)
  {	p = GetCell(r,c);
  p->real.rc.top = pos-cy;
  p->real.rc.bottom = pos;
  p->real.height = cy;
  // 
  if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
  }
  pos -= (cy+iSplitterHeight);
  }
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::MoveChangedWindow(HWND hWnd, CRect const *rc)
  {	CRect rcwin;
  ::GetClientRect(hWnd,&rcwin);
  ::MapWindowPoints(hWnd,m_hWnd,(POINT *)&rcwin,2);
  if(rcwin!=*rc)
    ::MoveWindow(hWnd,rc->left,rc->top,rc->Width(),rc->Height(),TRUE);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
#define HTSPLITTER_HORZ			0x0000fff0
#define HTSPLITTER_VERT			0x0000fff1
#define HTSPLITTER_VERTHORZ	0x0000fff2
  /////////////////////////////////////////////////////////////////////////////
  // 
#if _MSC_VER <= 1310	// Microsoft Visual C++ .NET 2003 and below.
  UINT SplitterCtrlBase::OnNcHitTest(CPoint point)
#else
  LRESULT SplitterCtrlBase::OnNcHitTest(CPoint point)
#endif
  {
    CPoint pt(point);
    ScreenToClient(&pt);
    // 
    int col, row;
    HitTest(pt,&row,&col);
    // 
    if(row!=-1 && IsActiveRowSplitter(row)==false) row = -1;
    if(col!=-1 && IsActiveColumnSplitter(col)==false) col = -1;
    // 
    if(col==-1 && row!=-1) return HTSPLITTER_HORZ;
    else if(col!=-1 && row==-1) return HTSPLITTER_VERT;
    else if(col!=-1 && row!=-1) return HTSPLITTER_VERTHORZ;
    // 
    return CWnd::OnNcHitTest(point);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  BOOL SplitterCtrlBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
  {	if((m_bDragHorz==true || (nHitTest & 0x0000ffff)==HTSPLITTER_HORZ) && m_hCurHorz!=NULL)
  {	SetCursor(m_hCurHorz);
  return TRUE;
  }
  else if((m_bDragVert==true || (nHitTest & 0x0000ffff)==HTSPLITTER_VERT) && m_hCurVert!=NULL)
  {	SetCursor(m_hCurVert);
  return TRUE;
  }
  else if((m_bDragHorzVert==true || (nHitTest & 0x0000ffff)==HTSPLITTER_VERTHORZ) && m_hCurHorzVert!=NULL)
  {	SetCursor(m_hCurHorzVert);
  return TRUE;
  }
  // 
  return CWnd::OnSetCursor(pWnd, nHitTest, message);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::OnNcLButtonDown(UINT nHitTest, CPoint point)
  {	CPoint pt(point);
  ScreenToClient(&pt);
  HitTest(pt,&m_iDragRow,&m_iDragCol);
  // 
  if(m_iDragRow!=-1 && IsActiveRowSplitter(m_iDragRow)==false) m_iDragRow = -1;
  if(m_iDragCol!=-1 && IsActiveColumnSplitter(m_iDragCol)==false) m_iDragCol = -1;
  // 
  if(m_iDragCol==-1 && m_iDragRow!=-1) m_bDragHorz = true;
  else if(m_iDragCol!=-1 && m_iDragRow==-1) m_bDragVert = true;
  else if(m_iDragCol!=-1 && m_iDragRow!=-1) m_bDragHorzVert = true;
  // 
  if(m_bDragHorz==true || m_bDragVert==true || m_bDragHorzVert==true)
  {	m_ptStartDrag = pt;
  // 
  for(int r=0; r<GetCountRow(); ++r)
    for(int c=0; c<GetCountColumn(); ++c)
    {	CELL *p = GetCell(r,c);
  p->store = p->real;
  }
  SetCapture();
  }
  // 
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC)
  {	CRect rcdrag;
  if(m_bDragHorz==true || m_bDragHorzVert==true)
  {	GetSplitterRect(true,m_iDragRow,&rcdrag);
  DrawDragRectHorz(&rcdrag);
  }
  if(m_bDragVert==true || m_bDragHorzVert==true) 
  {	GetSplitterRect(false,m_iDragCol,&rcdrag);
  DrawDragRectVert(&rcdrag);
  }
  }
  // 
  CWnd::OnNcLButtonDown(nHitTest, point);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
  {	CPoint pt(point);
  ScreenToClient(&pt);
  HitTest(pt,&m_iDragRow,&m_iDragCol);
  // 
  if(m_iDragRow!=-1 && IsActiveRowSplitter(m_iDragRow)==false) m_iDragRow = -1;
  if(m_iDragCol!=-1 && IsActiveColumnSplitter(m_iDragCol)==false) m_iDragCol = -1;
  // 
  if((m_iDragCol==-1 && m_iDragRow!=-1) || (m_iDragCol!=-1 && m_iDragRow!=-1))
    SetHalfAlignHeight(m_iDragRow);
  if((m_iDragCol!=-1 && m_iDragRow==-1) || (m_iDragCol!=-1 && m_iDragRow!=-1))
    SetHalfAlignWidth(m_iDragCol);
  // 
  CWnd::OnNcLButtonDblClk(nHitTest, point);
  }
  // 
  void SplitterCtrlBase::OnLButtonUp(UINT nFlags, CPoint point)
  {	Cancel(false);
  CWnd::OnLButtonUp(nFlags, point);
  }
  // 
  void SplitterCtrlBase::OnMButtonDown(UINT nFlags, CPoint point)
  {	Cancel(true);
  CWnd::OnMButtonDown(nFlags, point);
  }
  // 
  void SplitterCtrlBase::OnRButtonDown(UINT nFlags, CPoint point)
  {	Cancel(true);
  CWnd::OnRButtonDown(nFlags, point);
  }
  // 
  void SplitterCtrlBase::OnCaptureChanged(CWnd *pWnd)
  {	if(pWnd!=this) Cancel(true);
  CWnd::OnCaptureChanged(pWnd);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::OnMouseMove(UINT nFlags, CPoint point)
  {	int iShiftX = point.x - m_ptStartDrag.x;
  int iShiftY = point.y - m_ptStartDrag.y;
  CRect rcdrag;
  // 
  if(m_bDragHorz==true || m_bDragHorzVert==true)
  {	SetDraggingHeight(m_iDragRow,iShiftY,&rcdrag);
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC) DrawDragRectHorz(&rcdrag);
  }
  if(m_bDragVert==true || m_bDragHorzVert==true) 
  {	SetDraggingWidth(m_iDragCol,iShiftX,&rcdrag);
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC) DrawDragRectVert(&rcdrag);
  }
  // 
  CWnd::OnMouseMove(nFlags, point);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  BOOL SplitterCtrlBase::PreTranslateMessage(MSG* pMsg)
  {	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
  if(m_bDragHorz==true || m_bDragVert==true || m_bDragHorzVert==true)
    Cancel(true);
  // 
  return CWnd::PreTranslateMessage(pMsg);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::Cancel(bool reset)
  {	if(m_bDragHorz==false && m_bDragVert==false && m_bDragHorzVert==false) return;
  // 
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC)
    if(m_bDragHorz==true || m_bDragHorzVert==true) DrawDragRectHorz(NULL);
  if(m_bDragVert==true || m_bDragHorzVert==true) DrawDragRectVert(NULL);
  // 
  if((m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC && reset==false) ||
    (m_DraggingMode==SPLITTERCTRL_DRAGGING_DYNAMIC && reset==true))
  {
    for(int r=0; r<GetCountRow(); ++r)
      for(int c=0; c<GetCountColumn(); ++c)
      {	CELL *p = GetCell(r,c);
    p->real = p->store;
    if(p->hWnd!=NULL) MoveChangedWindow(p->hWnd,&p->real.rc);
    }
  }
  // 
  m_bDragHorz = m_bDragVert = m_bDragHorzVert = false;
  if(GetCapture()==this) ::ReleaseCapture();
  Invalidate(FALSE);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetDraggingWidth(int col, int offset, CRect *rcdrag/*out*/)
  {	CELL *p1 = GetCell(0,col);
  CELL *p2 = GetCell(0,col+1);
  // 
  CELL::STATE *state1 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p1->real : &p1->store);
  CELL::STATE *state2 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p2->real : &p2->store);
  // 
  int totalcx = state1->rc.Width() + state2->rc.Width();
  double totalfactor = state1->factorWidth + state2->factorWidth;
  int cx1,cx2;
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
    cx1 = state1->rc.Width() + offset;
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
    cx1 = state1->rc.Width() - offset;
  // 
  if(cx1<m_iMinWidth) cx1 = m_iMinWidth;
  cx2 = totalcx - cx1;
  if(cx2<m_iMinWidth)
  {	cx2 = m_iMinWidth;
  cx1 = totalcx - cx2;
  }
  // 
  int row = GetCountRow();
  // 
  for(int r=0; r<row; ++r)
  {	p1 = GetCell(r,col);
  p2 = GetCell(r,col+1);
  // 
  state1 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p1->store : &p1->real);
  state2 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p2->store : &p2->real);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	state1->rc.right = state1->rc.left + cx1;
  state2->rc.left = state2->rc.right - cx2;
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	state1->rc.left = state1->rc.right - cx1;
  state2->rc.right = state2->rc.left + cx2;
  }
  state1->width = cx1;
  state2->width = cx2;
  state1->factorWidth = totalfactor * ((double)cx1/(double)totalcx);
  state2->factorWidth = totalfactor - state1->factorWidth;
  if(state2->factorWidth<0.0) state2->factorWidth = 0.0;
  // 
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_DYNAMIC)
  {	if(p1->hWnd!=NULL) ::MoveWindow(p1->hWnd,state1->rc.left,state1->rc.top,state1->rc.Width(),state1->rc.Height(),TRUE);
  if(p2->hWnd!=NULL) ::MoveWindow(p2->hWnd,state2->rc.left,state2->rc.top,state2->rc.Width(),state2->rc.Height(),TRUE);
  }
  }
  // 
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC)
  {	GetClientRect(rcdrag);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	rcdrag->left = GetCell(0,col)->store.rc.right;
  rcdrag->right = GetCell(0,col+1)->store.rc.left;
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	rcdrag->left = GetCell(0,col+1)->store.rc.right;
  rcdrag->right = GetCell(0,col)->store.rc.left;
  }
  }
  else
  {	CRect rc;
  GetClientRect(&rc);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	rc.left = state1->rc.left;
  rc.right = state2->rc.right;
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	rc.left = state2->rc.left;
  rc.right = state1->rc.right;
  }
  // 
  InvalidateRect(&rc,FALSE);
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetDraggingHeight(int row, int offset, CRect *rcdrag/*out*/)
  {	CELL *p1 = GetCell(row,0);
  CELL *p2 = GetCell(row+1,0);
  // 
  CELL::STATE *state1 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p1->real : &p1->store);
  CELL::STATE *state2 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p2->real : &p2->store);
  // 
  int totalcy = state1->rc.Height() + state2->rc.Height();
  double totalfactor = state1->factorHeight + state2->factorHeight;
  int cy1, cy2;
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
    cy1 = state1->rc.Height() + offset;
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
    cy1 = state1->rc.Height() - offset;
  if(cy1<m_iMinHeight) cy1 = m_iMinHeight;
  cy2 = totalcy - cy1;
  if(cy2<m_iMinHeight)
  {	cy2 = m_iMinHeight;
  cy1 = totalcy - cy2;
  }
  // 
  int col = GetCountColumn();
  // 
  for(int c=0; c<col; ++c)
  {	p1 = GetCell(row,c);
  p2 = GetCell(row+1,c);
  // 
  state1 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p1->store : &p1->real);
  state2 = (m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC ? &p2->store : &p2->real);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	state1->rc.bottom = state1->rc.top + cy1;
  state2->rc.top = state2->rc.bottom - cy2;
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	state1->rc.top = state1->rc.bottom - cy1;
  state2->rc.bottom = state2->rc.top + cy2;
  }
  state1->height = cy1;
  state2->height = cy2;
  state1->factorHeight = totalfactor * ((double)cy1/(double)totalcy);
  state2->factorHeight = totalfactor - state1->factorHeight;
  if(state2->factorHeight<0.0) state2->factorHeight = 0.0;
  // 
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_DYNAMIC)
  {	if(p1->hWnd!=NULL) ::MoveWindow(p1->hWnd,state1->rc.left,state1->rc.top,state1->rc.Width(),state1->rc.Height(),TRUE);
  if(p2->hWnd!=NULL) ::MoveWindow(p2->hWnd,state2->rc.left,state2->rc.top,state2->rc.Width(),state2->rc.Height(),TRUE);
  }
  }
  // 
  if(m_DraggingMode==SPLITTERCTRL_DRAGGING_STATIC)
  {	GetClientRect(rcdrag);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	rcdrag->top = GetCell(row,0)->store.rc.bottom;
  rcdrag->bottom = GetCell(row+1,0)->store.rc.top;
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	rcdrag->top = GetCell(row+1,0)->store.rc.bottom;
  rcdrag->bottom = GetCell(row,0)->store.rc.top;
  }
  }
  else
  {	CRect rc;
  GetClientRect(&rc);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	rc.top = state1->rc.top;
  rc.bottom = state2->rc.bottom;
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	rc.top = state2->rc.top;
  rc.bottom = state1->rc.bottom;
  }
  // 
  InvalidateRect(&rc,FALSE);
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetHalfAlignWidth(int col)
  {	CELL *p1 = GetCell(0,col);
  CELL *p2 = GetCell(0,col+1);
  // 
  int totalcx = p1->real.rc.Width() + p2->real.rc.Width();
  double totalfactor = p1->real.factorWidth + p2->real.factorWidth;
  int cx1 = totalcx/2;
  int cx2 = totalcx - cx1;
  if(cx1<m_iMinWidth)
  {	cx1 = m_iMinWidth;
  cx2 = totalcx - cx1;
  }
  else if(cx2<m_iMinWidth)
  {	cx2 = m_iMinWidth;
  cx1 = totalcx - cx2;
  }
  // 
  int row = GetCountRow();
  // 
  for(int r=0; r<row; ++r)
  {	p1 = GetCell(r,col);
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
    p1->real.rc.right = p1->real.rc.left + cx1;
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
    p1->real.rc.left = p1->real.rc.right - cx1;
  p1->real.width = cx1;
  p1->real.factorWidth = totalfactor * ((double)cx1/(double)totalcx);
  // 
  p2 = GetCell(r,col+1);
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
    p2->real.rc.left = p2->real.rc.right - cx2;
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
    p2->real.rc.right = p2->real.rc.left + cx2;
  p2->real.width = cx2;
  p2->real.factorWidth = totalfactor - p1->real.factorWidth;
  if(p2->real.factorWidth<0.0) p2->real.factorWidth = 0.0;
  // 
  if(p1->hWnd!=NULL) ::MoveWindow(p1->hWnd,p1->real.rc.left,p1->real.rc.top,p1->real.rc.Width(),p1->real.rc.Height(),TRUE);
  if(p2->hWnd!=NULL) ::MoveWindow(p2->hWnd,p2->real.rc.left,p2->real.rc.top,p2->real.rc.Width(),p2->real.rc.Height(),TRUE);
  }
  // 
  CRect rc;
  GetClientRect(&rc);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	rc.left = p1->real.rc.left;
  rc.right = p2->real.rc.right;
  }
  else	// SPLITTERCTRL_SNAP_RIGHT_TOP or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	rc.left = p2->real.rc.left;
  rc.right = p1->real.rc.right;
  }
  // 
  InvalidateRect(&rc,FALSE);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetHalfAlignHeight(int row)
  {	CELL *p1 = GetCell(row,0);
  CELL *p2 = GetCell(row+1,0);
  // 
  int totalcy = p1->real.rc.Height() + p2->real.rc.Height();
  double totalfactor = p1->real.factorHeight + p2->real.factorHeight;
  int cy1 = totalcy/2;
  int cy2 = totalcy - cy1;
  if(cy1<m_iMinHeight)
  {	cy1 = m_iMinHeight;
  cy2 = totalcy - cy1;
  }
  else if(cy2<m_iMinHeight)
  {	cy2 = m_iMinHeight;
  cy1 = totalcy - cy2;
  }
  // 
  int col = GetCountColumn();
  // 
  for(int c=0; c<col; ++c)
  {	p1 = GetCell(row,c);
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
    p1->real.rc.bottom = p1->real.rc.top + cy1;
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
    p1->real.rc.top = p1->real.rc.bottom - cy1;
  p1->real.height = cy1;
  p1->real.factorHeight = totalfactor * ((double)cy1/(double)totalcy);
  // 
  p2 = GetCell(row+1,c);
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
    p2->real.rc.top = p2->real.rc.bottom - cy2;
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
    p2->real.rc.bottom = p2->real.rc.top + cy2;
  p2->real.height = cy2;
  p2->real.factorHeight = totalfactor - p1->real.factorHeight;
  if(p2->real.factorHeight<0.0) p2->real.factorHeight = 0.0;
  // 
  if(p1->hWnd!=NULL) ::MoveWindow(p1->hWnd,p1->real.rc.left,p1->real.rc.top,p1->real.rc.Width(),p1->real.rc.Height(),TRUE);
  if(p2->hWnd!=NULL) ::MoveWindow(p2->hWnd,p2->real.rc.left,p2->real.rc.top,p2->real.rc.Width(),p2->real.rc.Height(),TRUE);
  }
  // 
  CRect rc;
  GetClientRect(&rc);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	rc.top = p1->real.rc.top;
  rc.bottom = p2->real.rc.bottom;
  }
  else	// SPLITTERCTRL_SNAP_LEFT_BOTTOM or SPLITTERCTRL_SNAP_RIGHT_BOTTOM.
  {	rc.top = p2->real.rc.top;
  rc.bottom = p1->real.rc.bottom;
  }
  // 
  InvalidateRect(&rc,FALSE);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::HitTest(CPoint point, int *prow/*out*/, int *pcol/*out*/)
  {	int row = GetCountRow();
  int col = GetCountColumn();
  // 
  if(row>0 && col>0)
  {	if(pcol!=NULL)
  {	*pcol=-1;
  // 
  for(int c=0; c<col; ++c)
  {	CELL *p = GetCell(0,c);
  // 
  if((point.x < p->real.rc.left && (m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)) ||
    (point.x > p->real.rc.right && (m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_BOTTOM)))
  {
    *pcol = c-1;
    break;
  }
  if(point.x>=p->real.rc.left && point.x<=p->real.rc.right) break;
  }
  }
  // 
  if(prow!=NULL)
  {	*prow=-1;
  // 
  for(int r=0; r<row; ++r)
  {	CELL *p = GetCell(r,0);
  // 
  if((point.y < p->real.rc.top && (m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)) ||
    (point.y > p->real.rc.bottom && (m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_BOTTOM)))
  {
    *prow = r-1;
    break;
  }
  if(point.y>=p->real.rc.top && point.y<=p->real.rc.bottom) break;
  }
  }
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::GetWindowRect(int row, int col, RECT *rect/*out*/) const
  {	_ASSERTE(row>=0 && row<GetCountRow());
  _ASSERTE(col>=0 && col<GetCountColumn());
  // 
  ::CopyRect(rect,&GetCell(row,col)->real.rc);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::GetSplitterRect(bool horz, int idx, RECT *rect/*out*/) const
  {	_ASSERTE(idx>=0 && (horz==true && idx<GetCountRow()-1) || (horz==false && idx<GetCountColumn()-1));
  _ASSERTE(GetCountColumn()>0 && GetCountRow()>0);
  // 
  GetClientRect(rect);
  CRect rc;
  // 
  if(horz==true)
  {	GetWindowRect(idx,0,&rc);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_RIGHT_TOP)
  {	rect->top = rc.bottom;
  rect->bottom = rect->top + GetSplitterHeight();
  }
  else
  {	rect->bottom = rc.top;
  rect->top = rect->bottom - GetSplitterHeight();
  }
  }
  else
  {	GetWindowRect(0,idx,&rc);
  // 
  if(m_SnapMode==SPLITTERCTRL_SNAP_LEFT_TOP || m_SnapMode==SPLITTERCTRL_SNAP_LEFT_BOTTOM)
  {	rect->left = rc.right;
  rect->right = rect->left + GetSplitterWidth();
  }
  else
  {	rect->right = rc.left;
  rect->left = rect->right - GetSplitterWidth();
  }
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::DrawDragRectVert(CRect const *pRect)
  {	static CRect pRectOld(0,0,0,0);
  DrawDragRectManage(&pRectOld,pRect,false);
  }
  // 
  void SplitterCtrlBase::DrawDragRectHorz(CRect const *pRect)
  {	static CRect pRectOld(0,0,0,0);
  DrawDragRectManage(&pRectOld,pRect,true);
  }
  // 
  void SplitterCtrlBase::DrawDragRectManage(CRect *pRectOld, CRect const *pRectNew, bool horz)
  {	CRect rcNew(0,0,0,0);
  if(pRectNew!=NULL)
  {	CWnd::GetWindowRect(&rcNew);
  ScreenToClient(&rcNew);
  rcNew.SetRect(pRectNew->left-rcNew.left,pRectNew->top-rcNew.top,pRectNew->right-rcNew.left,pRectNew->bottom-rcNew.top);
  }
  // 
  if(m_pDrawManager!=NULL)
  {	CDC *pDC = GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
  if(pDC!=NULL)
  {	m_pDrawManager->DrawDragRect(pDC,horz,pRectOld->IsRectNull()==TRUE,pRectOld,&rcNew);
  ReleaseDC(pDC);
  }
  }
  // 
  *pRectOld = (pRectNew!=NULL ? rcNew : CRect(0,0,0,0));
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetWindow(int row, int col, HWND hWnd)
  {	_ASSERTE(row>=0 && row<GetCountRow());
  _ASSERTE(col>=0 && col<GetCountColumn());
  _ASSERTE(hWnd==NULL || (::IsWindow(hWnd)!=0 && ::GetParent(hWnd)==m_hWnd));
  // 
  GetCell(row,col)->hWnd = hWnd;
  if(hWnd!=NULL)		// for correct mutual repaint.
    ::SetWindowLong(hWnd,GWL_STYLE,::GetWindowLong(hWnd,GWL_STYLE) | WS_CLIPSIBLINGS);	// add WS_CLIPSIBLINGS style.
  }
  // 
  HWND SplitterCtrlBase::GetWindow(int row, int col) const
  {	_ASSERTE(row>=0 && row<GetCountRow());
  _ASSERTE(col>=0 && col<GetCountColumn());
  // 
  return GetCell(row,col)->hWnd;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // Only for SPLITTERCTRL_RESIZE_STATIC, SPLITTERCTRL_RESIZE_STATICFULL.
  // 
  void SplitterCtrlBase::SetWidthForStatic(int col, int width)
  {	_ASSERTE(m_ResizeMode!=SPLITTERCTRL_RESIZE_DYNAMIC);
  // 
  _ASSERTE(col>=0 && col<GetCountColumn());
  _ASSERTE(GetCountRow()>0 && GetCountColumn()>0);
  _ASSERTE(width>=0);
  // 
  width = max(width,m_iMinWidth);
  // 
  int row = GetCountRow();
  // 
  for(int r=0; r<row; ++r)
    GetCell(r,col)->real.width = width;
  }
  /////////////////////////////////////////////////////////////////////////////
  // Only for SPLITTERCTRL_RESIZE_STATIC, SPLITTERCTRL_RESIZE_STATICFULL.
  // 
  void SplitterCtrlBase::SetHeightForStatic(int row, int height)
  {	_ASSERTE(m_ResizeMode!=SPLITTERCTRL_RESIZE_DYNAMIC);
  // 
  _ASSERTE(row>=0 && row<GetCountRow());
  _ASSERTE(GetCountRow()>0 && GetCountColumn()>0);
  _ASSERTE(height>=0);
  // 
  height = max(height,m_iMinHeight);
  // 
  int col = GetCountColumn();
  // 
  for(int c=0; c<col; ++c)
    GetCell(row,c)->real.height = height;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // For SPLITTERCTRL_RESIZE_DYNAMIC.
  // 
  void SplitterCtrlBase::SetWidthForDynamic(int col, float percent)
  {	_ASSERTE(m_ResizeMode==SPLITTERCTRL_RESIZE_DYNAMIC);
  // 
  _ASSERTE(col>=0 && col<GetCountColumn());
  _ASSERTE(percent>0.1f && percent<99.9f);
  // 
  int rowCount = GetCountRow();
  int colCount = GetCountColumn();
  _ASSERTE(rowCount>0 && colCount>0);
  // 
  int r,c;
  int totalWidth = m_iTotalWidth - GetCell(0,col)->real.width;	// without width of col.
  double factor;
  // 
  for(c=0; c<colCount; ++c)
  {	if(c!=col) factor = (1.0-(double)percent/100.0) * (double)GetCell(0,c)->real.width/(double)totalWidth;
  else factor = (double)percent/100.0;
  // 
  for(r=0; r<rowCount; ++r)
    GetCell(r,c)->real.factorWidth = factor;
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // For SPLITTERCTRL_RESIZE_DYNAMIC.
  // 
  void SplitterCtrlBase::SetHeightForDynamic(int row, float percent)
  {	_ASSERTE(m_ResizeMode==SPLITTERCTRL_RESIZE_DYNAMIC);
  // 
  _ASSERTE(row>=0 && row<GetCountRow());
  _ASSERTE(percent>0.1f && percent<99.9f);
  // 
  int rowCount = GetCountRow();
  int colCount = GetCountColumn();
  _ASSERTE(rowCount>0 && colCount>0);
  // 
  int r,c;
  int totalHeight = m_iTotalHeight - GetCell(row,0)->real.height;	// without height of row.
  double factor;
  // 
  for(r=0; r<rowCount; ++r)
  {	if(r!=row) factor = (1.0-(double)percent/100.0) * (double)GetCell(r,0)->real.height/(double)totalHeight;
  else factor = (double)percent/100.0;
  // 
  for(c=0; c<colCount; ++c)
    GetCell(r,c)->real.factorHeight = factor;
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // Set width for every column (count items in 'percent'==GetCountColumn()).
  // 
  void SplitterCtrlBase::SetWidthsForDynamic(int *percent/*in*/)
  {	_ASSERTE(m_ResizeMode==SPLITTERCTRL_RESIZE_DYNAMIC);
  // 
  int row = GetCountRow();
  int col = GetCountColumn();
  _ASSERTE(row>0 && col>0);
  // 
  __try
  {	int count = 0;
  for(int c=0; c<col; ++c)
    count += percent[c];
  _ASSERTE(count==100);
  if(count!=100) return;
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {	_ASSERTE(!"count items in 'percent' != GetCountColumn()");
  return;
  }
  // 
  int width, tail=0;
  // 
  for(int c=0; c<col; ++c)
  {	double factor = (double)percent[c]/100.0;
  // 
  if(c!=col-1)
  {	width = (int)((double)m_iTotalWidth*factor + 0.5);
  tail += width;
  }
  else
    width = m_iTotalWidth - tail;
  // 
  for(int r=0; r<row; ++r)
  {	GetCell(r,c)->real.width = width;
  GetCell(r,c)->real.factorWidth = factor;
  }
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // Set height for every row (count items in 'percent'==GetCountRow()).
  // 
  void SplitterCtrlBase::SetHeightsForDynamic(int *percent/*in*/)
  {	_ASSERTE(m_ResizeMode==SPLITTERCTRL_RESIZE_DYNAMIC);
  // 
  int row = GetCountRow();
  int col = GetCountColumn();
  _ASSERTE(row>0 && col>0);
  // 
  __try
  {	int count = 0;
  for(int r=0; r<row; ++r)
    count += percent[r];
  _ASSERTE(count==100);
  if(count!=100) return;
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {	_ASSERTE(!"count items in 'percent' != GetCountRow()");
  return;
  }
  // 
  int height, tail=0;
  // 
  for(int r=0; r<row; ++r)
  {	double factor = (double)percent[r]/100.0;
  // 
  if(r!=row-1)
  {	height = (int)((double)m_iTotalHeight*factor + 0.5);
  tail += height;
  }
  else
    height = m_iTotalHeight - tail;
  // 
  for(int c=0; c<col; ++c)
  {	GetCell(r,c)->real.height = height;
  GetCell(r,c)->real.factorHeight = factor;
  }
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetEqualWidthColumns()
  {	int row = GetCountRow();
  int col = GetCountColumn();
  _ASSERTE(row>0 && col>0);
  // 
  int r,c, width, tail=0;
  double factor = 1.0/(double)col;
  // 
  for(c=0; c<col; ++c)
  {	
    if(c!=col-1)
    {	width = (int)((double)m_iTotalWidth * factor + 0.5);
    tail += width;
    }
    else	// last column.
      width = m_iTotalWidth-tail;
    // 
    for(r=0; r<row; ++r)
    {	GetCell(r,c)->real.width = width;
    GetCell(r,c)->real.factorWidth = factor;
    }
  }
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetEqualHeightRows()
  {	
    int row = GetCountRow();
    int col = GetCountColumn();
    _ASSERTE(row>0 && col>0);
    // 
    int r,c, height, tail=0;
    double factor = 1.0/(double)row;
    // 
    for(r=0; r<row; ++r)
    {	
      if(r!=row-1)
      {	
        height = (int)((double)m_iTotalHeight * factor + 0.5);
        tail += height;
      }
      else	// last row.
        height = m_iTotalHeight-tail;
      // 
      for(c=0; c<col; ++c)
      {	
        GetCell(r,c)->real.height = height;
        GetCell(r,c)->real.factorHeight = factor;
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetMinWidth(int width)
  {	_ASSERTE(width>=0);
  m_iMinWidth = width;
  }
  // 
  int SplitterCtrlBase::GetMinWidth() const
  {	
    return m_iMinWidth;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetMinHeight(int height)
  {	
    _ASSERTE(height>=0);
    m_iMinHeight = height;
  }
  // 
  int SplitterCtrlBase::GetMinHeight() const
  {	
    return m_iMinHeight;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  int SplitterCtrlBase::GetCountRow() const
  {	
    return m_matrix.GetCountRow();
  }
  // 
  int SplitterCtrlBase::GetCountColumn() const
  {
    return m_matrix.GetCountColumn();
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetSnapMode(SPLITTERCTRL_SNAP mode)
  {
    m_SnapMode = mode;
  }
  // 
  SPLITTERCTRL_SNAP SplitterCtrlBase::GetSnapMode() const
  {
    return m_SnapMode;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetResizeMode(SPLITTERCTRL_RESIZE mode)
  {	
    m_ResizeMode = mode;
  }
  // 
  SPLITTERCTRL_RESIZE SplitterCtrlBase::GetResizeMode() const
  {	return m_ResizeMode;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SetDraggingMode(SPLITTERCTRL_DRAGGING mode)
  {	
    m_DraggingMode = mode;
  }
  // 
  SPLITTERCTRL_DRAGGING SplitterCtrlBase::GetDraggingMode() const
  {	return m_DraggingMode;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::IsDraggingState(bool *bDragHorz/*out*/, bool *bDragVert/*out*/, bool *bDragHorzVert/*out*/) const
  {	
    if(bDragHorz!=NULL) *bDragHorz = m_bDragHorz;
    if(bDragVert!=NULL) *bDragVert = m_bDragVert;
    if(bDragHorzVert!=NULL) *bDragHorzVert = m_bDragHorzVert;
  }
  // 
  void SplitterCtrlBase::CancelDragging()
  {	Cancel(true);
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::ActiveRowSplitter(int row, bool active)
  {	
    _ASSERTE(row>=0 && row<GetCountRow()-1);
    _ASSERTE(GetCountColumn()>0);
    // 
    GetCell(row,0)->activeRow = active;
  }
  // 
  bool SplitterCtrlBase::IsActiveRowSplitter(int row) const
  {	
    _ASSERTE(row>=0 && row<GetCountRow()-1);
    _ASSERTE(GetCountColumn()>0);
    // 
    return GetCell(row,0)->activeRow;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::ActiveColumnSplitter(int col, bool active)
  {	
    _ASSERTE(GetCountRow()>0);
    _ASSERTE(col>=0 && col<GetCountColumn()-1);
    // 
    GetCell(0,col)->activeCol = active;
  }
  // 
  bool SplitterCtrlBase::IsActiveColumnSplitter(int col) const
  {	
    _ASSERTE(GetCountRow()>0);
    _ASSERTE(col>=0 && col<GetCountColumn()-1);
    // 
    return GetCell(0,col)->activeCol;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::ShowBorder(bool show)
  {	
    m_bShowBorder = show;
  }
  // 
  bool SplitterCtrlBase::IsShowBorder() const
  {	return m_bShowBorder;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // Save state in registry.
  // 
  bool SplitterCtrlBase::SaveState(CWinApp *app, CString section, CString entry)
  {	_ASSERTE(app!=NULL);
  // 
  try
  {	
    CMemFile file;
    CArchive ar(&file,CArchive::store);
    SaveState(&ar);
    ar.Flush();
    // 
    UINT uDataSize = (UINT)file.GetLength();
    BYTE *pData = file.Detach();
    // 
    if(app->WriteProfileBinary(section,entry,pData,uDataSize)==0)
    {	
      free(pData);
      return false;
    }
    free(pData);
  }
  catch(CMemoryException* pEx)
  {	
    pEx->Delete();
    return false;
  }
  catch(CArchiveException* pEx)
  {	
    pEx->Delete();
    return false;
  }
  catch(...)
  {	return false;
  }
  // 
  return true;
  }
  /////////////////////////////////////////////////////////////////////////////
  // Load state from registry.
  // 
  bool SplitterCtrlBase::LoadState(CWinApp *app, CString section, CString entry)
  {	_ASSERTE(app!=NULL);
  //
  bool res = false;
  BYTE *pData = NULL;
  UINT uDataSize;
  // 
  try
  {	
    if(app->GetProfileBinary(section,entry,&pData,&uDataSize)!=0)
    {	
      CMemFile file(pData,uDataSize);
      CArchive ar(&file,CArchive::load);
      LoadState(&ar);
      res = true;
    }
  }
  catch(CMemoryException* pEx)
  {	pEx->Delete();
  }
  catch(CArchiveException* pEx)
  {	pEx->Delete();
  }
  catch(...)
  {
  }
  if(pData!=NULL) delete [] pData;
  // 
  return res;
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlBase::SaveState(CArchive *ar)
  {	
    int row = GetCountRow();
    int col = GetCountColumn();
    // 
    *ar << row;
    *ar << col;
    // 
    if(row>0 && col>0)
    {	
      for(int c=0; c<col; ++c)
      {	
        *ar << GetCell(0,c)->real.width;
        *ar << GetCell(0,c)->real.factorWidth;
      }
      // 
      for(int r=0; r<row; ++r)
      {	
        *ar << GetCell(r,0)->real.height;
        *ar << GetCell(r,0)->real.factorHeight;
      }
    }
  }
  // 
  void SplitterCtrlBase::LoadState(CArchive *ar)
  {	
    int row, col;
    // 
    *ar >> row;
    *ar >> col;
    // 
    _ASSERTE(row==GetCountRow() && col==GetCountColumn());
    // 
    if(row>0 && col>0)
    {	
      int width,height;
      double factorWidth, factorHeight;
      // 
      for(int c=0; c<col; ++c)
      {	
        *ar >> width;
        *ar >> factorWidth;
        // 
        for(int r=0; r<row; ++r)
        {	
          GetCell(r,c)->real.width = width;
          GetCell(r,c)->real.factorWidth = factorWidth;
        }
      }
      // 
      for(int r=0; r<row; ++r)
      {	
        *ar >> height;
        *ar >> factorHeight;
        // 
        for(int c=0; c<col; ++c)
        {	
          GetCell(r,c)->real.height = height;
          GetCell(r,c)->real.factorHeight = factorHeight;
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  void DrawGradient(CDC *pDC, CRect const *rc, bool horz, COLORREF clrTop, COLORREF clrBottom)
  {	GRADIENT_RECT gRect = {0,1};
  TRIVERTEX vert[2] = 
  {	{rc->left,rc->top,(COLOR16)(GetRValue(clrTop)<<8),(COLOR16)(GetGValue(clrTop)<<8),(COLOR16)(GetBValue(clrTop)<<8),0},
  {rc->right,rc->bottom,(COLOR16)(GetRValue(clrBottom)<<8),(COLOR16)(GetGValue(clrBottom)<<8),(COLOR16)(GetBValue(clrBottom)<<8),0}
  };
  ::GradientFill(pDC->m_hDC,vert,2,&gRect,1,(horz==true ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V));
  }
  // 
  void SplitterCtrlCustom1::DrawSplitter(CDC *pDC, bool horz, int /*idx*/, CRect const *pRect)
  {	DrawGradient(pDC,pRect,horz==false,RGB(245,245,245),RGB(160,165,170));
  }
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrlCustom1::DrawDragRect(CDC *pDC, bool /*horz*/, bool firstTime, CRect const *pRectOld, CRect const *pRectNew)
  {	pDC->DrawDragRect(pRectNew,CSize(pRectNew->Width(),pRectNew->Height()),
  (firstTime==true ? NULL : pRectOld),CSize(pRectOld->Width(),pRectOld->Height()));
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlCustom1::DrawBorder(CDC *pDC, CRect const *pRect)
  {	pDC->Draw3dRect(pRect,RGB(77,115,61),RGB(77,115,61));
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // SplitterCtrlCustom2.
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  int SplitterCtrlCustom2::GetBorderWidth(ISplitterCtrlRecalc * /*base*/) const
  {	return 2;
  }
  // 
  int SplitterCtrlCustom2::GetSplitterWidth(ISplitterCtrlRecalc *base) const
  {	return base->GetSplitterWidth(NULL);
  }
  // 
  int SplitterCtrlCustom2::GetSplitterHeight(ISplitterCtrlRecalc *base) const
  {	return base->GetSplitterHeight(NULL);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlCustom2::DrawSplitter(CDC *pDC, bool /*horz*/, int /*idx*/, CRect const *pRect)
  {	pDC->DrawEdge((RECT *)pRect,EDGE_RAISED,BF_RECT);
  }
  /////////////////////////////////////////////////////////////////////////////
  //
  void SplitterCtrlCustom2::DrawDragRect(CDC *pDC, bool horz, bool firstTime, CRect const *pRectOld, CRect const *pRectNew)
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
    CBrush brush(HS_DIAGCROSS,RGB(0,0,0));
    pDC->DrawDragRect(&rcNew,CSize(rcNew.Width(),rcNew.Height()),
      (firstTime==true ? NULL : &rcOld),CSize(rcOld.Width(),rcOld.Height()),
      &brush,&brush);
  }
  /////////////////////////////////////////////////////////////////////////////
  // 
  void SplitterCtrlCustom2::DrawBorder(CDC *pDC, CRect const *pRect)
  {	
    pDC->DrawEdge((RECT *)pRect,EDGE_SUNKEN,BF_RECT);
  }

} // namespace SplitterCtrl_NS
