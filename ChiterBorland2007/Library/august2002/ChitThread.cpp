//---------------------------------------------------------------------------
#include <vcl.h>
#include <sstream>
#include <stdexcept>
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
if(GlobalChit!=NULL)
{
std::stringstream msg;
msg << "Only one chiter at any time can exist" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNC__ << std::endl;
throw std::runtime_error( msg.str() );
}
GlobalChit=this;
sleep=10000;
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
    for(int i=0;i<Searchers.size();i++)
     {
      Searchers[i]->ReplaceAll();
     }
}

__fastcall TChitThread::~TChitThread()
{
      GlobalChit=NULL;
}

//--------------------------------------------------
TChitThread* TChitThread::GetGlobal(void)
 {
  if(GlobalChit==NULL) GlobalChit=new TChitThread(true);
  return GlobalChit;
 }

