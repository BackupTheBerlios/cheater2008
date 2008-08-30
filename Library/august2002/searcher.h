//---------------------------------------------------------------------------
#ifndef searcherH
#define searcherH
#include <vector>
typedef unsigned int PointerType;
#include "ansi_stream.h"
#include <boost/shared_ptr.hpp>
//typedef TStream* TStrteamPtr;

typedef void __fastcall (__closure *AfterReadNotify)(int);
//---------------------------------------------------------------------------
union _export VoidInt
 {
  int i;
  unsigned long ul;
  void* v;
 };
class  _export TSearcher
{
private:
        int FPageSize;
        boost::shared_ptr<std::vector<PointerType> > FPointers;
        boost::shared_ptr<TStream> Fstream;
        TMemoryStream* FFind;
        TMemoryStream* FReplace;
        void __fastcall SetPageSize(int value);
        void __fastcall Setstream(boost::shared_ptr<TStream> value);
        void __fastcall Setsearcher(TSearcher* value);
        void Init(void);

public:
        bool StartSearch(void);
        __fastcall TSearcher(boost::shared_ptr<TStream>);
        virtual __fastcall ~TSearcher();
        bool __fastcall ContinueSearch(void);

        AfterReadNotify NotifyEvent;
        bool __fastcall SlowSearch(void);
        bool __fastcall StartSearchDilim(void);
        void __fastcall ReplaceAll(void);
        void __fastcall Assign(TSearcher*);
        __fastcall TSearcher(TSearcher*);
        __property int PageSize  = { read=FPageSize,write=SetPageSize };

        __property boost::shared_ptr<std::vector<PointerType> > Pointers  = { read=FPointers };
        __property boost::shared_ptr<TStream> stream  = { read=Fstream, write=Setstream };
        __property TMemoryStream* Find  = { read=FFind};
        __property TMemoryStream* Replace  = { read=FReplace};
};
#endif


