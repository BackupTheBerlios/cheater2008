//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include <sstream>
#include <stdexcept>

#include "SearcherProperties.h"
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

//---------------------------------------------------------------------------
 TSearcherProperties::TSearcherProperties(CWnd* pParent /*= NULL*/)
{
 FFind=new InputGroupBox(this);
 FFind->Top=13;Find->Left=5;
 FFind->Caption="String to Find";
this->InsertControl(Find);
 FReplace=new InputGroupBox(this);
 FReplace->Top=Find->Top+Find->Height+4;Replace->Left=5;
 FReplace->Caption="String to Replace";
this->InsertControl(Replace);
 SetFind=new CButton(this);
 SetFind->Caption="Set Find";
 SetFind->Top=Replace->Top+Replace->Height+4;SetFind->Left=10;
 SetFind->OnClick=SetFindClick;
this->InsertControl(SetFind);
 SetReplace=new CButton(this);
 SetReplace->Caption="Set Replace";
 SetReplace->Top=SetFind->Top+SetFind->Height+4;SetReplace->Left=10;
 SetReplace->OnClick=SetReplaceClick;
this->InsertControl(SetReplace);
 ReloadProperties=new CButton(this);
 ReloadProperties->Caption="Reload Properties";
 ReloadProperties->Top=Replace->Top+Replace->Height+4;ReloadProperties->Left=SetReplace->Left+SetReplace->Width+5;
 ReloadProperties->OnClick=ReloadClick;
this->InsertControl(ReloadProperties);
 PageSize=new InputGroupBox(this);
 PageSize->Caption="Page Size";
 PageSize->Top=SetReplace->Top+SetReplace->Height+4;PageSize->Left=5;
this->InsertControl(PageSize);
 SetPageSize=new CButton(this);
 SetPageSize->Caption="Set Page Size";
 SetPageSize->OnClick=setPageSizeClick;
 SetPageSize->Top=SetReplace->Top;SetPageSize->Left=SetReplace->Left+SetReplace->Width+4;
this->InsertControl(SetPageSize);
 ReloadProperties->Width=100;
//this->Constraints->MaxWidth=Find->Width+15;
 Pointers=new TPointersViewBox(this);
 Pointers->Align=alBottom;
this->InsertControl(Pointers);
 TSplitter* split=new TSplitter(this);
 split->Beveled=true;
 split->Align=alBottom;
this->InsertControl(split);

}
//---------------------------------------------------------------------------
namespace Searcherproperties
{
        void  PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TSearcherProperties)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------


void  TSearcherProperties::setSearcher(boost::shared_ptr<TSearcher> value)
{
// throw Exception("Sergey14");
    FSearcher=value;
  if(Searcher)
   Pointers->SetList(Searcher->Pointers);
  else
   Pointers->SetList(boost::shared_ptr< std::vector<PointerType> >((std::vector<PointerType>*)0));
  if(Showing)  Reload();
// throw Exception("Sergey15");
}

boost::shared_ptr<TSearcher>  TSearcherProperties::getSearcher(void)
{

   if(!FSearcher) FSearcher=boost::shared_ptr<TSearcher>(new TSearcher(boost::shared_ptr<std::iostream>((std::iostream*)0)));
   return FSearcher;

}

void  TSearcherProperties::Reload(void)
{
if((Searcher!=NULL)&&(Showing))
 { Find->InitBox(ConvertStreamToHexAnsi(Searcher->getFind()),HEX_STRING);
   Replace->InitBox(ConvertStreamToHexAnsi(Searcher->getReplace()),HEX_STRING);
   Pointers->SetList(Searcher->Pointers);
   PageSize->InitBox(IntToString(Searcher->PageSize),DEC_NUM);
 }
}

void  TSearcherProperties::SetFindClick(TObject*)
{
  if(Searcher)
  {
   Searcher->getFind().str("");
   Find->WriteDataToStream(Searcher->getFind());
  }
}

void  TSearcherProperties::SetReplaceClick(TObject*)
{
if(Searcher)
 {
  Searcher->getReplace().str("");
  Replace->WriteDataToStream(Searcher->getReplace());
 }
}

void  TSearcherProperties::ReloadClick(TObject*)
{
 if(Searcher)
  {
   Pointers->SetList(Searcher->Pointers);
   Reload();
  }
}

 TSearcherProperties::~TSearcherProperties(void)
{

}
void  TSearcherProperties::SetOnSelectPointer(TPointerSelectEvent value)
{
  Pointers->NotifyDblClick=value;
}
//----------------------------------------------------------------------------
void  TSearcherProperties::setPageSizeClick(TObject*)
{
 if(getSearcher())
  getSearcher()->PageSize=PageSize->GetPointer();
}

//----------------------------------------------------------------------------
int  TSearcherProperties::Search(bool IsNewSearch, boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress)
{
if(! getSearcher())
{
std::stringstream msg;
msg << "Searcher is null" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
throw std::runtime_error( msg.str() );
}
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
unsigned long len;
bool SearchResult;
VoidInt vi;
getSearcher()->stream=stream;
getSearcher()->NotifyEvent=DoProgress;
//ProgressBar->Max=stream->Size;
//ProgressBar->Min=0;ProgressBar->Position=0;

if(!getFind()->IsEmpty())
 {
  getSearcher()->getFind().str("");
  getFind()->WriteDataToStream(Searcher->getFind());
  if(IsNewSearch)
   {
//    ProgressBar->Max=Searcher->stream->Size;
    SearchResult=getSearcher()->StartSearch();
   }
  else
   {
//    ProgressBar->Max=Searcher->Pointers->Count;
    SearchResult=getSearcher()->ContinueSearch();
   }
if(SearchResult)
 {
  if(getSearcher()->Pointers->size()<200)
   {
     for(int i=0;i<getSearcher()->Pointers->size();i++)
      {
       vi.ul=(*(getSearcher()->Pointers))[i];
//       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
      }
   }
 }
 }
EndSearch=TDateTime::CurrentDateTime();

Reload();
return getSearcher()->Pointers->size();
}
//----------------------------------------------------------------------------
int  TSearcherProperties::SlowSearch(boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress)
{
if(!getSearcher())
{
std::stringstream msg;
msg << "Searcher is null" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
throw std::runtime_error( msg.str() );
}
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
unsigned long len;
bool SearchResult;
VoidInt vi;
getSearcher()->setStream(stream);

getSearcher()->setNotifyEvent(DoProgress);
//ProgressBar->Max=stream->Size;
//ProgressBar->Min=0;ProgressBar->Position=0;

if(!Find->IsEmpty())
 {
  getSearcher()->getFind().str("");
  Find->WriteDataToStream(Searcher->getFind());
//    ProgressBar->Max=Searcher->stream->Size;
    SearchResult=getSearcher()->SlowSearch();

if(SearchResult)
 {
  if(getSearcher()->Pointers->size()<200)
   {
     for(int i=0;i<getSearcher()->Pointers->size();i++)
      {
       vi.ul=(*(getSearcher()->Pointers))[i];
//       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
      }
   }
 }
 }
EndSearch=TDateTime::CurrentDateTime();

Reload();
return getSearcher()->Pointers->size();
}

//----------------------------------------------------------------------------
void  TSearcherProperties::ReplaceAll(void)
{
if(!getReplace()->IsEmpty())
 {
   getSearcher()->getReplace().str("");
   Replace->WriteDataToStream(getSearcher()->getReplace());
   getSearcher()->ReplaceAll();
 }
}

