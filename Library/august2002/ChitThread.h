//---------------------------------------------------------------------------
#ifndef ChitThreadH
#define ChitThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "searcher.h"
#include <vector>
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
class TChitThread : public TThread
{
private:
        std::vector< boost::shared_ptr<TSearcher> > FSearchers;
protected:
        void __fastcall Execute();
public:
        int sleep;
 static  TChitThread* GetGlobal(void);
  __fastcall TChitThread(bool CreateSuspended);
        void __fastcall Work(void);
        __fastcall ~TChitThread();
        __property std::vector< boost::shared_ptr<TSearcher> > Searchers  = { read=FSearchers };
};
extern TChitThread* GlobalChit;
//---------------------------------------------------------------------------
#endif
