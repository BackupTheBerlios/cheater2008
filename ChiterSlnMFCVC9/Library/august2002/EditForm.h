//---------------------------------------------------------------------------
#ifndef EditFormH
#define EditFormH
//---------------------------------------------------------------------------
#include "StreamEdit.h"
#include "august2002Fwd.h"
//---------------------------------------------------------------------------
class  _export TStreamEdit;
class  TEditMemoryForm : public TForm
{
__published:	// IDE-managed Components
        TStreamEdit *StreamEdit;
        void  FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
         TEditMemoryForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TEditMemoryForm *EditMemoryForm;
//---------------------------------------------------------------------------
#endif

