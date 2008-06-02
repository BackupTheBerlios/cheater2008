//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TypeComboBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TTypeComboBox *)
{
        new TTypeComboBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTypeComboBox::TTypeComboBox(TComponent* Owner)
        : TComboBox(Owner)
{
 Style=csDropDownList;
}
//---------------------------------------------------------------------------
namespace Typecombobox
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TTypeComboBox)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------
 