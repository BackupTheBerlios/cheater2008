//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "EditMemory"
#pragma link "StreamEdit"
#pragma resource "*.dfm"
//TEditMemoryForm *EditMemoryForm;
//---------------------------------------------------------------------------
__fastcall TEditMemoryForm::TEditMemoryForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditMemoryForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
Action=caFree;
}
//---------------------------------------------------------------------------



