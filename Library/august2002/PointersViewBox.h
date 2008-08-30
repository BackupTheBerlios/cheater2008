//---------------------------------------------------------------------------

#ifndef PointersViewBoxH
#define PointersViewBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include <boost/shared_ptr.hpp>
#include "Ansi_Stream.h"
extern TList* PoitersDB;

typedef void __fastcall (__closure *TPointerSelectEvent)(int);
//---------------------------------------------------------------------------
class PACKAGE TPointersViewBox : public TGroupBox
{
__published:	// IDE-managed Components
private:
        TScrollBar *ScrollBar;
        TListBox *PointersBox;
        TPopupMenu* PopupMenu;


        TMenuItem *CopyMenuItem;
        TMenuItem *PasteMenuItem;
        TMenuItem *CopytoMenuItem;
        TMenuItem *PasteFromMenuItem;
        TMenuItem *DeleteMenuItem;
        TMenuItem *ClearClipBoardMenuItem;
        TMenuItem *AddPointerToListMenuItem;
        TMenuItem *DeletePointerMenuItem;
        void __fastcall ScrollBarScroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
        void __fastcall PointersBoxDblClick(TObject *Sender);
        boost::shared_ptr<TList> FList;
        void __fastcall Init(void);
        void __fastcall PopupMenuOnPopup(TObject *Sender);
        void __fastcall WritePointersToFile(TObject*);
        void __fastcall LoadPointersFromFile(TObject*);


        void __fastcall DeleteSubMenuClick(TObject *Sender);
        void __fastcall CopytoMenuClick(TObject *Sender);
        void __fastcall PasteFromMenuClick(TObject *Sender);

        void __fastcall CopyMenuItemClick(TObject*);
        void __fastcall PasteMenuItemClick(TObject*);
        void __fastcall ClearClipBoardMenuItemClick(TObject*);
        void __fastcall AddPointerToListMenuItemClick(TObject *Sender);
        void __fastcall DeletePointerMenuItemClick(TObject *Sender);

        void __fastcall DeleteSelectedPointers(void);

        void __fastcall KeyDown(TObject *Sender,WORD &Key, TShiftState Shift);
        void __fastcall Reload(int);	// User declarations
        void __fastcall Update();        
public:		// User declarations
        void __fastcall SetList(boost::shared_ptr<TList> value);
         TPointerSelectEvent NotifyDblClick;
        __fastcall TPointersViewBox(TComponent* Owner);
//        __property TList* List  = { read=FList, write=SetList };
};
//---------------------------------------------------------------------------
#endif

