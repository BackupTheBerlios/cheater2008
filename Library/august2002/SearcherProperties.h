//---------------------------------------------------------------------------
#ifndef SearcherPropertiesH
#define SearcherPropertiesH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>

#include <boost/shared_ptr.hpp>
#include "inputgroupbox.h"
#include "ansi_stream.h"
#include "PointersViewBox.h"
//---------------------------------------------------------------------------
class PACKAGE TSearcherProperties : public TGroupBox
{
private:
//        TInputGroupBox* Find;
//        TInputGroupBox* Replace;
        TButton* SetFind;
        TButton* SetReplace;
        boost::shared_ptr<TSearcher> FSearcher;
        TButton* ReloadProperties;
        TPointersViewBox* Pointers;
        TInputGroupBox* FFind;
        TInputGroupBox* FReplace;
        TInputGroupBox* PageSize;
        TButton* SetPageSize;
        void __fastcall SetFindClick(TObject*);
        void __fastcall SetReplaceClick(TObject*);
        void __fastcall ReloadClick(TObject*);
        void __fastcall SetSearcher(boost::shared_ptr<TSearcher> value);
        boost::shared_ptr<TSearcher>  __fastcall GetSearcher(void);
        void __fastcall SetOnSelectPointer(TPointerSelectEvent value);
        void __fastcall SetPageSizeClick(TObject*);
protected:
public:
        __fastcall TSearcherProperties(TComponent* Owner);

__published:
public:
int __fastcall Search(bool IsNewSearch, boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress);
int __fastcall SlowSearch(boost::shared_ptr<std::iostream>,AfterReadNotify DoProgress);
        void __fastcall Reload(void);
        void __fastcall ReplaceAll(void);
        __fastcall ~TSearcherProperties(void);
        __property boost::shared_ptr<TSearcher> Searcher  = { read=GetSearcher, write=SetSearcher };
        __property TPointerSelectEvent OnSelectPointer  = { write=SetOnSelectPointer };
        __property TInputGroupBox* Find  = { read=FFind };
        __property TInputGroupBox* Replace  = { read=FReplace };
};
//---------------------------------------------------------------------------
#endif
