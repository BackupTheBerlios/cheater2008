//---------------------------------------------------------------------------
#ifndef SearcherPropertiesH
#define SearcherPropertiesH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "inputgroupbox.h"
#include "Standart/ansi_stream.h"
#include "PointersViewBox.h"
#include "searcher.h"
#include "MyBaseForm.h"
#include "Command.h"
#include "august2002Fwd.h"


//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TSearcherProperties : public CMyBaseForm
{
  DECLARE_DYNAMIC(TSearcherProperties)

private:
  Container d_controlCommands;
  //        TInputGroupBox* Find;
  //        TInputGroupBox* Replace;
  CButton* SetFind;
  CButton* SetReplace;
  boost::shared_ptr<TSearcher> FSearcher;
  CButton* ReloadProperties;
  TPointersViewBox* Pointers;
  InputGroupBox* FFind;
  InputGroupBox* FReplace;
  InputGroupBox* PageSize;
  CButton* SetPageSize;
  void  SetFindClick();
  void  SetReplaceClick();
  void  ReloadClick();
  void  setPageSizeClick();
  void initialize();
public:
  void  setSearcher(boost::shared_ptr<TSearcher> value);
  boost::shared_ptr<TSearcher>   getSearcher(void);
  void  setOnSelectPointer(TPointerSelectEvent value);

  TSearcherProperties(CWnd* pParent = NULL);


  int  Search(bool IsNewSearch, boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress);
  int  SlowSearch(boost::shared_ptr<std::iostream>,AfterReadNotify DoProgress);
  void  Reload(void);
  void  ReplaceAll(void);
  virtual ~TSearcherProperties(void);

  InputGroupBox* getFind()  {return FFind; }
  InputGroupBox* getReplace()  { return FReplace; }


protected:

  //Windows events
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //void  KeyDown(TObject *Sender,WORD &Key, TShiftState Shift);

  afx_msg void OnInitMenu(CMenu* pMenu);
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);



  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual BOOL OnInitDialog();


  DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------
#endif
