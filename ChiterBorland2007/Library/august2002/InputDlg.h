//---------------------------------------------------------------------------

#ifndef InputDlgH
#define InputDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "InputGroupBox.h"
//---------------------------------------------------------------------------
class TInputForm : public TForm
{
__published:	// IDE-managed Components
        TInputGroupBox *InputBox;
        TButton *Ok;
        TButton *Cancel;
private:	// User declarations
public:		// User declarations
        __fastcall TInputForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInputForm *InputForm;
//---------------------------------------------------------------------------
#endif
