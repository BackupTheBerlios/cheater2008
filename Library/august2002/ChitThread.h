//---------------------------------------------------------------------------

#ifndef ChitThreadH
#define ChitThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TChitThread : public TThread
{
private:
        TList* FSearchers;
protected:
        void __fastcall Execute();

public:
        int sleep;
 static  TChitThread* GetGlobal(void);
  __fastcall TChitThread(bool CreateSuspended);
        void __fastcall Work(void);
        __fastcall ~TChitThread();
        __property TList* Searchers  = { read=FSearchers };

};
extern TChitThread* GlobalChit;
//---------------------------------------------------------------------------
#endif
