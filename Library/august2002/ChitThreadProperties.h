//---------------------------------------------------------------------------

#ifndef ChitThreadPropertiesH
#define ChitThreadPropertiesH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include "searcherProperties.h"
//---------------------------------------------------------------------------
class PACKAGE TChitThreadProperties : public TGroupBox
{
private:
        TSearcherProperties* SearcherProperties;
        TList* FSearchers;
        void __fastcall SetSearchers(TList* value);
        TScrollBar* Scroll;
  void __fastcall ScrollEvent(System::TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);
        void __fastcall DeleteUnitMenuItemClick(TObject*);
        void __fastcall Reload(int);
        void __fastcall ReloadMenuItemClick(TObject*);
protected:
public:


        __fastcall TChitThreadProperties(TComponent* Owner);
        __property TList* Searchers  = { read=FSearchers, write=SetSearchers };
__published:
};
//---------------------------------------------------------------------------
#endif
