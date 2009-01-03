//---------------------------------------------------------------------------
#include "stdafx.h"

#pragma hdrstop
#include <sstream>
#include <stdexcept>

#include "ChitThread.h"
#include "Searcher.h"

TChitThread* GlobalChit=NULL;
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void  Unit2::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------


 TChitThread::TChitThread(bool CreateSuspended)
{
if(GlobalChit!=NULL)
{
std::stringstream msg;
msg << "Only one chiter at any time can exist" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
throw std::runtime_error( msg.str() );
}
GlobalChit=this;
sleep=10000;
}

//---------------------------------------------------------------------------
int  TChitThread::Run() //Execute
{
while(!Terminated)
 {
   Work();
   Sleep(sleep);
 }
	return 0;
}

//---------------------------------------------------------------------------
void  TChitThread::Work(void)
{
    for(int i=0;i<FSearchers.size();i++)
     {
      FSearchers[i]->ReplaceAll();
     }
}

 TChitThread::~TChitThread()
{
      GlobalChit=NULL;
}

//--------------------------------------------------
TChitThread* TChitThread::GetGlobal(void)
 {
  if(GlobalChit==NULL) GlobalChit=new TChitThread(true);
  return GlobalChit;
 }

