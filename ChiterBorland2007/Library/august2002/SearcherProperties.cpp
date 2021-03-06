//---------------------------------------------------------------------------
#include <vcl.h>
#include <sstream>
#include <stdexcept>
#pragma hdrstop
#include "SearcherProperties.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TSearcherProperties *)
{
        new TSearcherProperties(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSearcherProperties::TSearcherProperties(TComponent* Owner)
        : TGroupBox(Owner)
{
 FFind=new TInputGroupBox(this);
 FFind->Top=13;Find->Left=5;
 FFind->Caption="String to Find";
this->InsertControl(Find);
 FReplace=new TInputGroupBox(this);
 FReplace->Top=Find->Top+Find->Height+4;Replace->Left=5;
 FReplace->Caption="String to Replace";
this->InsertControl(Replace);
 SetFind=new TButton(this);
 SetFind->Caption="Set Find";
 SetFind->Top=Replace->Top+Replace->Height+4;SetFind->Left=10;
 SetFind->OnClick=SetFindClick;
this->InsertControl(SetFind);
 SetReplace=new TButton(this);
 SetReplace->Caption="Set Replace";
 SetReplace->Top=SetFind->Top+SetFind->Height+4;SetReplace->Left=10;
 SetReplace->OnClick=SetReplaceClick;
this->InsertControl(SetReplace);
 ReloadProperties=new TButton(this);
 ReloadProperties->Caption="Reload Properties";
 ReloadProperties->Top=Replace->Top+Replace->Height+4;ReloadProperties->Left=SetReplace->Left+SetReplace->Width+5;
 ReloadProperties->OnClick=ReloadClick;
this->InsertControl(ReloadProperties);
 PageSize=new TInputGroupBox(this);
 PageSize->Caption="Page Size";
 PageSize->Top=SetReplace->Top+SetReplace->Height+4;PageSize->Left=5;
this->InsertControl(PageSize);
 SetPageSize=new TButton(this);
 SetPageSize->Caption="Set Page Size";
 SetPageSize->OnClick=SetPageSizeClick;
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
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TSearcherProperties)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------


void __fastcall TSearcherProperties::SetSearcher(boost::shared_ptr<TSearcher> value)
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

boost::shared_ptr<TSearcher> __fastcall TSearcherProperties::GetSearcher(void)
{

   if(!FSearcher) FSearcher=boost::shared_ptr<TSearcher>(new TSearcher(boost::shared_ptr<std::iostream>((std::iostream*)0)));
   return FSearcher;

}

void __fastcall TSearcherProperties::Reload(void)
{
if((Searcher!=NULL)&&(Showing))
 { Find->InitBox(ConvertStreamToHexAnsi(Searcher->getFind()),HEX_STRING);
   Replace->InitBox(ConvertStreamToHexAnsi(Searcher->getReplace()),HEX_STRING);
   Pointers->SetList(Searcher->Pointers);
   PageSize->InitBox(IntToString(Searcher->PageSize),DEC_NUM);
 }
}

void __fastcall TSearcherProperties::SetFindClick(TObject*)
{
  if(Searcher)
  {
   Searcher->getFind().str("");
   Find->WriteDataToStream(Searcher->getFind());
  }
}

void __fastcall TSearcherProperties::SetReplaceClick(TObject*)
{
if(Searcher)
 {
  Searcher->getReplace().str("");
  Replace->WriteDataToStream(Searcher->getReplace());
 }
}

void __fastcall TSearcherProperties::ReloadClick(TObject*)
{
 if(Searcher)
  {
   Pointers->SetList(Searcher->Pointers);
   Reload();
  }
}

__fastcall TSearcherProperties::~TSearcherProperties(void)
{

}
void __fastcall TSearcherProperties::SetOnSelectPointer(TPointerSelectEvent value)
{
  Pointers->NotifyDblClick=value;
}
//----------------------------------------------------------------------------
void __fastcall TSearcherProperties::SetPageSizeClick(TObject*)
{
 if(Searcher)
  Searcher->PageSize=PageSize->GetPointer();
}

//----------------------------------------------------------------------------
int __fastcall TSearcherProperties::Search(bool IsNewSearch, boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress)
{
if(! Searcher)
{
std::stringstream msg;
msg << "Searcher is null" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNC__ << std::endl;
throw std::runtime_error( msg.str() );
}
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
unsigned long len;
bool SearchResult;
VoidInt vi;
Searcher->stream=stream;
Searcher->NotifyEvent=DoProgress;
//ProgressBar->Max=stream->Size;
//ProgressBar->Min=0;ProgressBar->Position=0;

if(!Find->IsEmpty())
 {
  Searcher->getFind().str("");
  Find->WriteDataToStream(Searcher->getFind());
  if(IsNewSearch)
   {
//    ProgressBar->Max=Searcher->stream->Size;
    SearchResult=Searcher->StartSearch();
   }
  else
   {
//    ProgressBar->Max=Searcher->Pointers->Count;
    SearchResult=Searcher->ContinueSearch();
   }
if(SearchResult)
 {
  if(Searcher->Pointers->size()<200)
   {
     for(int i=0;i<Searcher->Pointers->size();i++)
      {
       vi.ul=(*(Searcher->Pointers))[i];
//       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
      }
   }
 }
 }
EndSearch=TDateTime::CurrentDateTime();

Reload();
return Searcher->Pointers->size();
}
//----------------------------------------------------------------------------
int __fastcall TSearcherProperties::SlowSearch(boost::shared_ptr<std::iostream> stream,AfterReadNotify DoProgress)
{
if(!Searcher)
{
std::stringstream msg;
msg << "Searcher is null" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNC__ << std::endl;
throw std::runtime_error( msg.str() );
}
TDateTime StartSearch=TDateTime::CurrentDateTime();
TDateTime EndSearch;
unsigned long len;
bool SearchResult;
VoidInt vi;
Searcher->stream=stream;

Searcher->NotifyEvent=DoProgress;
//ProgressBar->Max=stream->Size;
//ProgressBar->Min=0;ProgressBar->Position=0;

if(!Find->IsEmpty())
 {
  Searcher->getFind().str("");
  Find->WriteDataToStream(Searcher->getFind());
//    ProgressBar->Max=Searcher->stream->Size;
    SearchResult=Searcher->SlowSearch();

if(SearchResult)
 {
  if(Searcher->Pointers->size()<200)
   {
     for(int i=0;i<Searcher->Pointers->size();i++)
      {
       vi.ul=(*(Searcher->Pointers))[i];
//       GotoInputGroupBox->Strings->Add(ulongToHexAnsi(vi.ul));
      }
   }
 }
 }
EndSearch=TDateTime::CurrentDateTime();

Reload();
return Searcher->Pointers->size();
}

//----------------------------------------------------------------------------
void __fastcall TSearcherProperties::ReplaceAll(void)
{
if(!Replace->IsEmpty())
 {
   Searcher->getReplace().str("");
   Replace->WriteDataToStream(Searcher->getReplace());
   Searcher->ReplaceAll();
 }
}

