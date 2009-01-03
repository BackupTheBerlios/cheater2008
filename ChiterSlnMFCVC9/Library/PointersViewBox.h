//---------------------------------------------------------------------------



#ifndef PointersViewBoxH
#define PointersViewBoxH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <vector>
#include "Standart/Ansi_Stream.h"
#include "AuxiliaryTypedefs.h"
#include "Resource.h"
#include "august2002Fwd.h"
extern std::vector< boost::shared_ptr<std::vector<PointerType> > > PoitersDB;

typedef void  ( *TPointerSelectEvent)(int);


//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TPointersViewBox : public CDialog
{
    	DECLARE_DYNAMIC(TPointersViewBox)
private:
        CScrollBar *ScrollBar;
        CListBox *PointersBox;
        TPopupMenu* PopupMenu;

        //enum { IDD = IDD_MEMORYINFOEDIT };
        CMenuItem *CopyMenuItem;
        CMenuItem *PasteMenuItem;
        CMenuItem *CopytoMenuItem;
        CMenuItem *PasteFromMenuItem;
        CMenuItem *DeleteMenuItem;
        CMenuItem *ClearClipBoardMenuItem;
        CMenuItem *AddPointerToLisCMenuItem;
        CMenuItem *DeletePointerMenuItem;
        void  ScrollBarScroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
        void  PointersBoxDblClick(TObject *Sender);
        boost::shared_ptr< std::vector<PointerType> > FList;
        void  Init(void);
        void  PopupMenuOnPopup(TObject *Sender);
        void  WritePointersToFile(TObject*);
        void  LoadPointersFromFile(TObject*);


        void  DeleteSubMenuClick(TObject *Sender);
        void  CopytoMenuClick(TObject *Sender);
        void  PasteFromMenuClick(TObject *Sender);

        void  CopyMenuItemClick(TObject*);
        void  PasteMenuItemClick(TObject*);
        void  ClearClipBoardMenuItemClick(TObject*);
        void  AddPointerToLisCMenuItemClick(TObject *Sender);
        void  DeletePointerMenuItemClick(TObject *Sender);

        void  DeleteSelectedPointers(void);

        afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        //void  KeyDown(TObject *Sender,WORD &Key, TShiftState Shift);

        void  Reload(int);	// User declarations
        void  Update();        
public:		// User declarations
        void  SetList(boost::shared_ptr< std::vector<PointerType> > value);
         TPointerSelectEvent NotifyDblClick;
         TPointersViewBox(CWnd* pParent = NULL);
//        __property TList* List  = { read=FList, write=SetList };
protected:
    	DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------
#endif

