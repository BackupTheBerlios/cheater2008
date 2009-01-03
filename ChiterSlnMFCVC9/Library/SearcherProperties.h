//---------------------------------------------------------------------------
#ifndef SearcherPropertiesH
#define SearcherPropertiesH
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "inputgroupbox.h"
#include "Standart/ansi_stream.h"
#include "PointersViewBox.h"

#include "august2002Fwd.h"
//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TSearcherProperties : public CDialog
{
private:
//        TInputGroupBox* Find;
//        TInputGroupBox* Replace;
        CButton* SetFind;
        CButton* SetReplace;
        boost::shared_ptr<TSearcher> FSearcher;
        CButton* ReloadProperties;
        TPointersViewBox* Pointers;
        InputGroupBox* FFind;
        InputGroupBox* FReplace;
        InputGroupBox* PageSize;
        CButton* SetPageSize;
        void  SetFindClick(TObject*);
        void  SetReplaceClick(TObject*);
        void  ReloadClick(TObject*);
public:
        void  setSearcher(boost::shared_ptr<TSearcher> value);
        boost::shared_ptr<TSearcher>   getSearcher(void);
        void  setOnSelectPointer(TPointerSelectEvent value);
        void  setPageSizeClick(TObject*);
protected:
public:
         TSearcherProperties(CWnd* pParent = NULL);

public:
int  Search(bool IsNewSearch, boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress);
int  SlowSearch(boost::shared_ptr<std::iostream>,AfterReadNotify DoProgress);
        void  Reload(void);
        void  ReplaceAll(void);
        virtual ~TSearcherProperties(void);

        InputGroupBox* getFind()  {return FFind; }
        InputGroupBox* getReplace()  { return FReplace; }
};
//---------------------------------------------------------------------------
#endif
