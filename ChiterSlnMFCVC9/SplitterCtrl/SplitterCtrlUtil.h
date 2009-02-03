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
#ifndef SplitterCtrl_SplitterCtrlUtil_h
#define SplitterCtrl_SplitterCtrlUtil_h
#include <vector>

namespace SplitterCtrl_NS
{


  template<typename T>
  class matrix
  {
  public:
    matrix()
    {	
      m_iRow = m_iCol = 0;
    }

  public:
    bool AddRow()
    {	
      try
      {	m_vtr.push_back( std::vector<T>(m_iCol,T()) );
      }
      catch(std::bad_alloc)
      {	
        return false;
      }
      m_iRow++;
      return true;
    }
    // 
    bool AddColumn()
    {	
      m_i_vtr i=m_vtr.begin();
      try
      {	
        m_i_vtr end = m_vtr.end();
        for(; i!=end; ++i)
          i->push_back(T());
      }
      catch(std::bad_alloc)
      {	
        for(; i!=m_vtr.begin(); )
          (--i)->pop_back();
        return false;
      }
      m_iCol++;
      return true;
    }
    // 
    bool InsertRow(int row)	// insert before.
    {	
      _ASSERTE(row>=0 && row<m_iRow);
      try
      {	m_vtr.insert( m_vtr.begin()+row, std::vector<T>(m_iCol,T()) );
      }
      catch(std::bad_alloc)
      {	
        return false;
      }
      m_iRow++;
      return true;
    }
    // 
    bool InsertColumn(int col)	// insert before.
    {	
      _ASSERTE(col>=0 && col<m_iCol);
      m_i_vtr i=m_vtr.begin();
      try
      {	
        m_i_vtr end = m_vtr.end();
        for(; i!=end; ++i)
          i->insert(i->begin()+col,T());
      }
      catch(std::bad_alloc)
      {	
        for(; i!=m_vtr.begin(); )
        {
          --i;
          i->erase(i->begin()+col);
        }
        return false;
      }
      m_iCol++;
      return true;
    }
    // 
    void DeleteRow(int row)
    {	
      _ASSERTE(row>=0 && row<m_iRow);
      m_vtr.erase( m_vtr.begin()+row );
      m_iRow--;
    }
    // 
    void DeleteColumn(int col)
    {
      _ASSERTE(col>=0 && col<m_iCol);
      m_i_vtr end = m_vtr.end();
      for(m_i_vtr i=m_vtr.begin(); i!=end; ++i)
        i->erase( i->begin()+col );
      m_iCol--;
    }
    // 
    void DeleteAll()
    {	m_vtr.clear();
    m_iRow = m_iCol = 0;
    }
    // 
    T *GetCell(int row, int col)
    {
      _ASSERTE(row>=0 && row<m_iRow);
      _ASSERTE(col>=0 && col<m_iCol);
      return &m_vtr[row][col];
    }
    T const *GetCell(int row, int col) const
    {	
      _ASSERTE(row>=0 && row<m_iRow);
      _ASSERTE(col>=0 && col<m_iCol);
      return &m_vtr[row][col];
    }
    // 
    int GetCountRow() const
    {	
      return m_iRow;
    }
    int GetCountColumn() const
    {	
      return m_iCol;
    }

  private:
    std::vector< std::vector<T> > m_vtr;
    typedef typename std::vector< std::vector<T> >::iterator m_i_vtr;
    int m_iRow, m_iCol;
  };
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 
  enum SPLITTERCTRL_SNAP
  {	SPLITTERCTRL_SNAP_LEFT_TOP,	// default.
  SPLITTERCTRL_SNAP_LEFT_BOTTOM,
  SPLITTERCTRL_SNAP_RIGHT_TOP,
  SPLITTERCTRL_SNAP_RIGHT_BOTTOM
  };
  // 
  enum SPLITTERCTRL_RESIZE
  {	SPLITTERCTRL_RESIZE_STATIC,	// default.
  SPLITTERCTRL_RESIZE_STATICFULL,
  SPLITTERCTRL_RESIZE_DYNAMIC
  };
  // 
  enum SPLITTERCTRL_DRAGGING
  {	SPLITTERCTRL_DRAGGING_STATIC,		// default.
  SPLITTERCTRL_DRAGGING_DYNAMIC
  };
  // 
  struct SplitterCtrlDraw
  {	virtual void DrawBegin(CDC * /*pDC*/) {}
  virtual void DrawSplitter(CDC * /*pDC*/, bool /*horz*/, int /*idx*/, CRect const * /*pRect*/) {}
  virtual void DrawDragRect(CDC * /*pDC*/, bool /*horz*/, bool /*firstTime*/, CRect const * /*pRectOld*/, CRect const * /*pRectNew*/) {}
  virtual void DrawBorder(CDC * /*pDC*/, CRect const * /*pRect*/) {}
  virtual void DrawEnd(CDC * /*pDC*/) {}
  };
  // 
  interface ISplitterCtrlRecalc
  {	virtual int GetSplitterWidth(ISplitterCtrlRecalc *base) const = 0;
  virtual int GetSplitterHeight(ISplitterCtrlRecalc *base) const = 0;
  virtual int GetBorderWidth(ISplitterCtrlRecalc *base) const = 0;
  };
  // 
  class SplitterCtrlBase : public CWnd
  {public:
  SplitterCtrlBase();
  ~SplitterCtrlBase();

  private:
    DECLARE_DYNCREATE(SplitterCtrlBase)

    ///////////////////////////////////////
    // PUBLIC
    ///////////////////////////////////////
  public:
    bool Create(CWnd *pParentWnd, DWORD dwStyle, RECT const &rect, UINT uID);
    void SetCursors(UINT uIdHorz, UINT uIdVert, UINT uIdHorzVert);
    void SetCursors(HCURSOR hHorz, HCURSOR hVert, HCURSOR hHorzVert);
    bool AddRow();
    bool AddColumn();
    bool InsertRow(int row);
    bool InsertColumn(int col);
    void DeleteRow(int row);
    void DeleteColumn(int col);
    void DeleteAll();
    void Update();	// recalculate control.
    // 
    void SetDrawManager(SplitterCtrlDraw *p);
    SplitterCtrlDraw *GetDrawManager();
    void SetRecalcManager(ISplitterCtrlRecalc *p);
    ISplitterCtrlRecalc *GetRecalcManager();
    // 
    void SetWindow(int row, int col, HWND hWnd);
    HWND GetWindow(int row, int col) const;
    // 
    void SetWidthForStatic(int col, int width);		// only for SPLITTERCTRL_RESIZE_STATIC and SPLITTERCTRL_RESIZE_STATICFULL.
    void SetHeightForStatic(int row, int height);	// only for SPLITTERCTRL_RESIZE_STATIC and SPLITTERCTRL_RESIZE_STATICFULL.
    // 
    void SetWidthForDynamic(int col, float percent);	// for SPLITTERCTRL_RESIZE_DYNAMIC (for sure use Update() after each call).
    void SetHeightForDynamic(int row, float percent);	// for SPLITTERCTRL_RESIZE_DYNAMIC (for sure use Update() after each call).
    void SetWidthsForDynamic(int *percent/*in*/);	// set width for every column (count items in 'percent'==GetCountColumn()).
    void SetHeightsForDynamic(int *percent/*in*/);	// set height for every row (count items in 'percent'==GetCountRow()).
    // 
    void SetEqualWidthColumns();
    void SetEqualHeightRows();
    // 
    void SetMinWidth(int width);
    int GetMinWidth() const;
    void SetMinHeight(int height);
    int GetMinHeight() const;
    // 
    int GetCountRow() const;
    int GetCountColumn() const;
    void HitTest(CPoint point, int *row/*out*/, int *col/*out*/);
    void GetWindowRect(int row, int col, RECT *rect/*out*/) const;
    void GetSplitterRect(bool horz, int idx, RECT *rect/*out*/) const;
    // 
    void SetSnapMode(SPLITTERCTRL_SNAP mode);
    SPLITTERCTRL_SNAP GetSnapMode() const;
    void SetResizeMode(SPLITTERCTRL_RESIZE mode);
    SPLITTERCTRL_RESIZE GetResizeMode() const;
    void SetDraggingMode(SPLITTERCTRL_DRAGGING mode);
    SPLITTERCTRL_DRAGGING GetDraggingMode() const;
    // 
    void IsDraggingState(bool *bDragHorz/*out*/, bool *bDragVert/*out*/, bool *bDragHorzVert/*out*/) const;
    void CancelDragging();
    // 
    void ActiveRowSplitter(int row, bool active);
    bool IsActiveRowSplitter(int row) const;
    void ActiveColumnSplitter(int col, bool active);
    bool IsActiveColumnSplitter(int col) const;
    // 
    void ShowBorder(bool show);
    bool IsShowBorder() const;
    // 
    bool LoadState(CWinApp *app, CString section, CString entry);	// load state from registry.
    bool SaveState(CWinApp *app, CString section, CString entry);	// save state in registry.
    void LoadState(CArchive *ar);
    void SaveState(CArchive *ar);

    ///////////////////////////////////////
    // PROTECTED
    ///////////////////////////////////////
  protected:
    struct CELL
    {	HWND hWnd;
    bool activeRow, activeCol;
    // 
    struct STATE
    {	CRect rc;
    int width, height;
    double factorWidth, factorHeight;
    } real, store;
    };

  protected:
    CELL *GetCell(int row, int col);
    CELL const *GetCell(int row, int col) const;

  protected:
    int GetBorderWidth() const;
    int GetSplitterWidth() const;
    int GetSplitterHeight() const;

    ///////////////////////////////////////
    // PRIVATE
    ///////////////////////////////////////
  private:
    SplitterCtrlDraw *m_pDrawManager;
    ISplitterCtrlRecalc *m_pRecalcManager;
    // 
    matrix<CELL> m_matrix;
    SPLITTERCTRL_SNAP m_SnapMode;
    SPLITTERCTRL_RESIZE m_ResizeMode;
    SPLITTERCTRL_DRAGGING m_DraggingMode;
    int m_iMinWidth, m_iMinHeight;
    bool m_bShowBorder;
    HCURSOR m_hCurHorz, m_hCurVert, m_hCurHorzVert;
    // 
    int m_iTotalWidth, m_iTotalHeight;
    // 
    bool m_bDragHorz, m_bDragVert, m_bDragHorzVert;
    int m_iDragCol, m_iDragRow;
    CPoint m_ptStartDrag;

  private:
    struct Service : public ISplitterCtrlRecalc
    {	virtual int GetBorderWidth(ISplitterCtrlRecalc *base) const;
    virtual int GetSplitterWidth(ISplitterCtrlRecalc *base) const;
    virtual int GetSplitterHeight(ISplitterCtrlRecalc *base) const;
    } m_Service;

  private:
    void Recalc();
    void RecalcStatic(CRect *rc, int iCountRow, int iCountColumn);
    void RecalcStaticFull(CRect *rc, int iCountRow, int iCountColumn);
    void RecalcDynamic(CRect *rc, int iCountRow, int iCountColumn);
    void Cancel(bool reset);
    void SetDraggingWidth(int col, int offset, CRect *rcdrag/*out*/);
    void SetDraggingHeight(int row, int offset, CRect *rcdrag/*out*/);
    void DrawDragRectVert(CRect const *pRect);
    void DrawDragRectHorz(CRect const *pRect);
    void DrawDragRectManage(CRect *pRectOld, CRect const *pRectNew, bool horz);
    void SetHalfAlignWidth(int col);
    void SetHalfAlignHeight(int row);
    void MoveChangedWindow(HWND hWnd, CRect const *rc);

    ///////////////////////////////////////
    // PROTECTED
    ///////////////////////////////////////
  protected:
    DECLARE_MESSAGE_MAP()
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnNcPaint();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
#if _MSC_VER <= 1310	// Microsoft Visual C++ .NET 2003 and below.
    afx_msg UINT OnNcHitTest(CPoint point);
#else
    afx_msg LRESULT OnNcHitTest(CPoint point);
#endif
  };
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // 

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////




} // namespace SplitterCtrl_NS



#endif //! SplitterCtrl_SplitterCtrl_h
