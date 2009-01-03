//---------------------------------------------------------------------------
#include "stdafx.h"
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
 TTypeComboBox::TTypeComboBox(TComponent* Owner)
        : TComboBox(Owner)
{
 Style=csDropDownList;
}
//---------------------------------------------------------------------------
namespace Typecombobox
{
        void  PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TTypeComboBox)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------

