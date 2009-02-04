//---------------------------------------------------------------------------
#ifndef PointersViewBoxH
#define PointersViewBoxH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <vector>
#include "Standart/Ansi_Stream.h"
#include "AuxiliaryTypedefs.h"
#include "Resource.h"
#include "MyBaseForm.h"
#include "Command.h"
#include "august2002Fwd.h"
extern std::vector< boost::shared_ptr<std::vector<PointerType> > > PoitersDB;

typedef boost::function<void (int)> TPointerSelectEvent;
typedef boost::shared_ptr<TPointerSelectEvent> TPointerSelectEventPtr;

enum SubMenus
{
  TPointersViewBox_DELETE_SUBMENU_ITEM,
  TPointersViewBox_COPTY_SUBMENU_ITEM,
  TPointersViewBox_PASTEFROM_SUBMENU_ITEM, 
  TPointersViewBox_ADDPOINTERTOLIST_SUBMENU_ITEM
};


//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TPointersViewBox : public CMyBaseForm
{
    	DECLARE_DYNAMIC(TPointersViewBox)
private:
        enum {LISTBOXFILED=1000,SCROLLBARFILED};
        CScrollBar *d_scrollBar;
        CListBox *d_pointersBox;
        TPopupMenu PopupMenu;

        Container d_menuCommands;
        //enum { IDD = IDD_MEMORYINFOEDIT };
        CMenuItem CopytoMenuItem;
        CMenuItem PasteFromMenuItem;
        CMenuItem DeleteMenuItem;
        CMenuItem AddPointerToLisCMenuItem;
        CMenuItem DeletePointerMenuItem;
        
        void  PointersBoxDblClick(UINT nFlags, CPoint point);
        boost::shared_ptr< std::vector<PointerType> > FList;
        void  updateSize();
        void  create();
        void  PopupMenuOnPopup();
        void  WritePointersToFile();
        void  LoadPointersFromFile();

        void  CopyMenuItemClick();
        void  PasteMenuItemClick();
        void  ClearClipBoardMenuItemClick();
        void  DeletePointerMenuItemClick();

        void  DeleteSubMenuClick(unsigned int idx);
        void  CopytoMenuClick(unsigned int idx);
        void  PasteFromMenuClick(unsigned int idx);
        void  AddPointerToLisCMenuItemClick(unsigned int idx);

        void  DeleteSelectedPointers(void);

///////////////////////////////////////////////////////////

        void  Reload(unsigned int);	// User declarations
        void  Update();        
public:		// User declarations
        void  SetList(boost::shared_ptr< std::vector<PointerType> > value);
         TPointerSelectEventPtr NotifyDblClick;
         TPointersViewBox(CWnd* pParent = NULL);
//        __property TList* List  = { read=FList, write=SetList };
protected:

  //Windows events
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //void  KeyDown(TObject *Sender,WORD &Key, TShiftState Shift);

  afx_msg void OnInitMenu(CMenu* pMenu);
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu,UINT nIndex,BOOL bSysMenu);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);

  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
    	DECLARE_MESSAGE_MAP()

};
//---------------------------------------------------------------------------
#endif

