//---------------------------------------------------------------------------

#ifndef InputDlgH
#define InputDlgH
//---------------------------------------------------------------------------
#include "InputGroupBox.h"
//---------------------------------------------------------------------------
class TInputForm : public TForm

{


        TInputGroupBox *InputBox;

        TButton *Ok;

        TButton *Cancel;

private:	// User declarations

public:		// User declarations

         TInputForm(TComponent* Owner);

};

//---------------------------------------------------------------------------

extern PACKAGE TInputForm *InputForm;

//---------------------------------------------------------------------------

#endif

