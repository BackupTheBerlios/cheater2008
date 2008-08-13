//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ProcessView.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TProcessView *)
{
        new TProcessView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TProcessView::TProcessView(TComponent* Owner)
        : TTreeView(Owner)
{
}
//---------------------------------------------------------------------------
namespace Processview
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TProcessView)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------

