//---------------------------------------------------------------------------

#ifndef ChitThreadH
#define ChitThreadH
//---------------------------------------------------------------------------
#include "searcher.h"
#include <vector>
#include <boost/shared_ptr.hpp>
 
//---------------------------------------------------------------------------
class TChitThread : public CWinThread
{
private:
        std::vector< boost::shared_ptr<TSearcher> > FSearchers;
protected:
        //void  Execute();
		virtual int Run();

public:
        int sleep;
 static  TChitThread* GetGlobal(void);
   TChitThread(bool CreateSuspended);
        void  Work(void);
         ~TChitThread();
        std::vector< boost::shared_ptr<TSearcher> > getSearchers() { return FSearchers; };

};
extern TChitThread* GlobalChit;
//---------------------------------------------------------------------------
#endif

