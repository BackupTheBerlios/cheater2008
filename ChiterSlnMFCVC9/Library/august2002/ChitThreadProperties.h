//---------------------------------------------------------------------------
#ifndef ChitThreadPropertiesH
#define ChitThreadPropertiesH
//---------------------------------------------------------------------------
#include "searcherProperties.h"
#include "august2002Fwd.h"
//---------------------------------------------------------------------------
class AUGUST2002_EXPORT TChitThreadProperties : public TGroupBox
{
private:
        TSearcherProperties* SearcherProperties;
        std::vector< boost::shared_ptr<TSearcher> > FSearchers;
        void  SetSearchers(std::vector< boost::shared_ptr<TSearcher> > value);
        TScrollBar* Scroll;
  void  ScrollEvent(System::TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);
        void  DeleteUnitMenuItemClick(TObject*);
        void  Reload(int);
        void  ReloadMenuItemClick(TObject*);
protected:
public:


         TChitThreadProperties(TComponent* Owner);
		 std::vector< boost::shared_ptr<TSearcher> > getSearchers  {return FSearchers;}
		void setSearchers(std::vector< boost::shared_ptr<TSearcher> >& val) {SetSearchers(val);}

};
//---------------------------------------------------------------------------
#endif

