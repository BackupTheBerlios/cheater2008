//---------------------------------------------------------------------------
#ifndef LIBRARY_ProcessChoiceFrame_h
#define LIBRARY_ProcessChoiceFrame_h
//---------------------------------------------------------------------------
#include "Library/august2002Fwd.h"
#include "Library/AuxiliaryTypedefs.h"
#include "Library/WindowProcedure.h"
#include "SplitterCtrl/SplitterCtrl.h"
#include "Library/MyBaseForm.h"

//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TProcessChoice1 :  public CMyBaseForm
{
    DECLARE_DYNAMIC(TProcessChoice1)
	// IDE-managed Components
        CTreeCtrl ProcessTree;
        CTreeCtrl AdditionalInfo;
        SplitterCtrl_NS::SplitterCtrl Splitter1;
        void  ProcessTreeEnter(TObject *Sender);
        void  ProcessTreeDblClick(TObject *Sender);
        void initialize();
public:		// User declarations
        TProcessChoice1(CWnd* pParent = NULL);
        void __fastcall GetProcesses(void);
protected:	// User declarations
  virtual BOOL OnInitDialog();
  DECLARE_MESSAGE_MAP()
};

#endif

