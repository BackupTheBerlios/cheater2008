//---------------------------------------------------------------------------
#ifndef searcherH
#define searcherH
#include <vector>
typedef unsigned int PointerType;
#include "Standart/ansi_stream.h"
#include "august2002Fwd.h"
#include <sstream>
#include <boost/shared_ptr.hpp>
//typedef TStream* TStrteamPtr;

typedef void  ( *AfterReadNotify)(int);
//---------------------------------------------------------------------------
union AUGUST2002_EXPORT VoidInt
 {
  int i;
  unsigned long ul;
  void* v;
 };
class  AUGUST2002_EXPORT TSearcher
{
private:
        int FPageSize;
        boost::shared_ptr<std::vector<PointerType> > FPointers;
        boost::shared_ptr<std::iostream> Fstream;
        std::stringstream FFind;
        std::stringstream FReplace;
        void  SetPageSize(int value);
        void  Setstream(boost::shared_ptr<std::iostream> value);
        void  Setsearcher(TSearcher* value);
        void Init(void);

public:
        bool StartSearch(void);
         TSearcher(boost::shared_ptr<std::iostream>);
        virtual  ~TSearcher();
        bool  ContinueSearch(void);

        AfterReadNotify NotifyEvent;
        bool  SlowSearch(void);
        bool  StartSearchDilim(void);
        void  ReplaceAll(void);
        void  Assign(boost::shared_ptr<TSearcher>);
         TSearcher(boost::shared_ptr<TSearcher>);
		 int getPageSize() const {return FPageSize;}
		 void setPageSize(int val) {SetPageSize(val);}

        boost::shared_ptr<std::vector<PointerType> > getPointers() { return FPointers; }
		boost::shared_ptr<std::iostream> getStream()  { return Fstream;}
		void setStream(boost::shared_ptr<std::iostream>& val) {Setstream(val); };
        std::stringstream& getFind();
        std::stringstream& getReplace();
};
#endif


