//---------------------------------------------------------------------------
#include <vcl.h>

#include <sstream>
#include <stdexcept>

#pragma hdrstop

#include "searcher.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

bool TSearcher::StartSearch(void)
{
VoidInt voidint;
Pointers->clear();
if(stream!=NULL)
 {
 char* str; int length;
  str=(char*)FFind->Memory;
  length=FFind->Size;
  char* Buffer=new char[FPageSize];
  int i;
  int realBufferLength;
  int posBufInStream;
  stream->Seek(0,soFromBeginning);
  i=0;
  while(stream->Position<stream->Size)
   {
    posBufInStream=stream->Position;
    realBufferLength=stream->Read(Buffer,FPageSize);
    for(i=0;i<=realBufferLength-length;i++)
     {
      if(memcmp(Buffer+i,str,length)==0)
       {voidint.i=i+posBufInStream;Pointers->push_back(voidint.ul);}
      }
     if(NotifyEvent!=NULL)NotifyEvent(stream->Position);
   }
//  memcpy(Buffer,Buffer+FPageSize,length-1);
//  i= 1-length;
 delete Buffer;
 }
return Pointers->size()>0;
}
__fastcall TSearcher::TSearcher(boost::shared_ptr<TStream> value)
{
Init();
Fstream=value;
}
__fastcall TSearcher::~TSearcher()
{
     delete FFind;
     delete FReplace;
}

bool __fastcall TSearcher::ContinueSearch(void)
{
VoidInt voidint;
if(stream!=NULL)
 {
 char* str; int len;
  str=(char*)FFind->Memory;
  len=FFind->Size;
  char* Buffer=new char[len];
  int i;
  boost::shared_ptr<std::vector<PointerType> > newPointers=boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
    for(i=0;i<Pointers->size();i++)
     {
       voidint.ul=(*Pointers)[i];
       stream->Seek(voidint.i,soFromBeginning);
       if(stream->Read(Buffer,len)==len)
          if(memcmp(Buffer,str,len)==0)
            newPointers->push_back(voidint.ul);
    }
  FPointers=newPointers;
  delete Buffer;
 }
else
 {
  Pointers->clear();
 }
return Pointers->size()>0;
}

void __fastcall TSearcher::Setstream(boost::shared_ptr<TStream> value)
{
     Fstream=value;
}
//----------------------------------------------------------------------------
bool __fastcall TSearcher::SlowSearch(void)
{
 VoidInt voidint;
  Pointers->clear();
if(stream!=NULL)
 {
 char* str; int length;
  str=(char*)FFind->Memory;
  length=FFind->Size;
  char* Buffer=new char[FPageSize+length-1];
  int i;
  int realBufferLength;
  int posBufInStream;
  stream->Seek(0,soFromBeginning);
  i=length-1;
  do
   {
    posBufInStream=stream->Position;
    realBufferLength=stream->Read(Buffer+length-1,FPageSize);
    for(;i<realBufferLength;i++)
     {
      if(memcmp(Buffer+i,str,length)==0)
       {voidint.i=i+posBufInStream-(length-1);Pointers->push_back(voidint.ul);}
      }
     if(realBufferLength==FPageSize){i=0;CopyMemory(Buffer,Buffer+FPageSize,length-1);}
      else i=length-1;
     if(NotifyEvent!=NULL)NotifyEvent(stream->Position);
   }while(stream->Position<stream->Size);
//  memcpy(Buffer,Buffer+FPageSize,length-1);
//  i= 1-length;
 delete Buffer;
 }
 return Pointers->size()>0;
}
bool __fastcall TSearcher::StartSearchDilim(void)
{
VoidInt voidint;
Pointers->clear();
if(stream!=NULL)
 {
 char* str; int length;
  str=(char*)FFind->Memory;
  length=FFind->Size;
  char* Buffer=new char[FPageSize];
  int i;
  int realBufferLength;
  int posBufInStream;
  stream->Seek(0,soFromBeginning);
  i=0;
  while(stream->Position<stream->Size)
   {
    posBufInStream=stream->Position;
    realBufferLength=stream->Read(Buffer,FPageSize);
    for(i=0;i<=realBufferLength-length;i++)
     {
      if(memcmp(Buffer+i,str,length)==0)
       {voidint.i=i+posBufInStream;Pointers->push_back(voidint.ul);i+=length-1;}
      }
     if(NotifyEvent!=NULL)NotifyEvent(stream->Position);
   }
//  memcpy(Buffer,Buffer+FPageSize,length-1);
//  i= 1-length;
 delete Buffer;
 }
return Pointers->size()>0;
}
//--------------------------------------------------------------------------
void __fastcall TSearcher::ReplaceAll(void)
{
for(int i=0;i<Pointers->size();i++)
 {
  stream->Position=(unsigned long)((*Pointers)[i]);
  stream->Write(Replace->Memory,Replace->Size);
 }
}
void __fastcall TSearcher::Assign(TSearcher* src)
{
if((src!=NULL)&&(src!=this))
 {
  FFind->Clear();
  FFind->CopyFrom(src->Find,0);
  FReplace->Clear();
  FReplace->CopyFrom(src->Replace,0);
  FPageSize=src->PageSize;
  Fstream=src->Fstream;
  (*FPointers) = *(src->FPointers);
 }
else
{
std::stringstream msg;
msg << "src is either null or itselt" << std::endl << std::endl
    << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNC__ << std::endl;
throw std::runtime_error( msg.str() );
}

}

__fastcall TSearcher::TSearcher(TSearcher* src)
{
Init();
Assign(src);
}

void TSearcher::Init(void)
{
  FPageSize=4096;
  Fstream=boost::shared_ptr<TStream>((TStream*)0);
  FPointers=boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
  FPointers->push_back(111);
  NotifyEvent=NULL;
  FFind=new TMemoryStream();
  FReplace=new TMemoryStream();
}
//--------------------------------------------------------------------------
void __fastcall TSearcher::SetPageSize(int value)
{
 if(value>0)
  FPageSize=value;
}

