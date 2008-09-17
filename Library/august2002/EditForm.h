//---------------------------------------------------------------------------
#ifndef EditFormH
#define EditFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "StreamEdit.h"
//---------------------------------------------------------------------------
class  _export TStreamEdit;
class  TEditMemoryForm : public TForm
{
__published:	// IDE-managed Components
        TStreamEdit *StreamEdit;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TEditMemoryForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TEditMemoryForm *EditMemoryForm;
//---------------------------------------------------------------------------
#endif

