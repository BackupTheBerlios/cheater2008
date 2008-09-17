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
        std::vector< boost::shared_ptr<TSearcher> > FSearchers;
        void __fastcall SetSearchers(std::vector< boost::shared_ptr<TSearcher> > value);
        TScrollBar* Scroll;
  void __fastcall ScrollEvent(System::TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);
        void __fastcall DeleteUnitMenuItemClick(TObject*);
        void __fastcall Reload(int);
        void __fastcall ReloadMenuItemClick(TObject*);
protected:
public:


        __fastcall TChitThreadProperties(TComponent* Owner);
        __property std::vector< boost::shared_ptr<TSearcher> > Searchers  = { read=FSearchers, write=SetSearchers };
__published:
};
//---------------------------------------------------------------------------
#endif

