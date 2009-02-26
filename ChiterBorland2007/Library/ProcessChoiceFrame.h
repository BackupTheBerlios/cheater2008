//---------------------------------------------------------------------------
#ifndef ProcessChoiceFrameH
#define ProcessChoiceFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <psapi.h>
#include <AnsiLib.h>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TProcessChoice1 : public TFrame
{
__published:	// IDE-managed Components
        TTreeView *ProcessTree;
        TTreeView *AdditionalInfo;
        TSplitter *Splitter1;
        void __fastcall ProcessTreeEnter(TObject *Sender);
        void __fastcall ProcessTreeDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TProcessChoice1(TComponent* Owner);
        void __fastcall GetProcesses(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TProcessChoice1 *ProcessChoice1;
//---------------------------------------------------------------------------
#endif

