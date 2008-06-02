//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChitThread.h"
#include "Searcher.h"
#pragma package(smart_init)
TChitThread* GlobalChit=NULL;
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit2::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TChitThread::TChitThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
if(GlobalChit!=NULL) throw Exception("One chiter is enough");
GlobalChit=this;
sleep=10000;
FSearchers=new TList();
}
//---------------------------------------------------------------------------
void __fastcall TChitThread::Execute()
{
while(!Terminated)
 {
   Synchronize(Work);
  Sleep(sleep);
 }

}
//---------------------------------------------------------------------------
void __fastcall TChitThread::Work(void)
{
    for(int i=0;i<Searchers->Count;i++)
     {
      ((TSearcher*)Searchers->Items[i])->ReplaceAll();
     }
}

__fastcall TChitThread::~TChitThread()
{
      for(int i=0;i<Searchers->Count;i++)
       delete (TSearcher*) Searchers->Items[i];
      delete Searchers;
      GlobalChit=NULL;      
}
//--------------------------------------------------
TChitThread* TChitThread::GetGlobal(void)
 {
  if(GlobalChit==NULL) GlobalChit=new TChitThread(true);
  return GlobalChit;
 }
