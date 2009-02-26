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
if(stream)
 {
  stream->clear();
  std::vector<char> buf;
  char* str;
  int length;
  std::stringstream& findStream = getFind();
  length=findStream.str().length();
  buf.resize(length);
  str=&(buf[0]);
  findStream.seekg(0,std::ios_base::beg);
  findStream.read(str,length);

  char* Buffer=new char[FPageSize];
  int i;
  int realBufferLength;
  int posBufInStream;
  DWORD streamSize=stream->seekg(0,std::ios_base::end).tellg();
  stream->seekg(0,std::ios_base::beg);
  i=0;
  while(*stream)
   {
    posBufInStream=stream->tellg();
    realBufferLength=stream->read(Buffer,FPageSize).gcount();
    for(i=0;i<=realBufferLength-length;i++)
     {
      if(memcmp(Buffer+i,str,length)==0)
       {voidint.i=i+posBufInStream;Pointers->push_back(voidint.ul);}
      }
     if(NotifyEvent!=NULL)NotifyEvent((int)(100 * (double)stream->tellg() / (double )streamSize)) ;
   }
//  memcpy(Buffer,Buffer+FPageSize,length-1);
//  i= 1-length;
 delete Buffer;
 }
return Pointers->size()>0;
}
__fastcall TSearcher::TSearcher(boost::shared_ptr<std::iostream> value)
{
Init();
Fstream=value;
}
__fastcall TSearcher::~TSearcher()
{
}

bool __fastcall TSearcher::ContinueSearch(void)
{
VoidInt voidint;
if(stream)
 {
  stream->clear();
  std::vector<char> buf;
  char* str;
  int length;
  std::stringstream& findStream = getFind();
  length=findStream.str().length();
  buf.resize(length);
  str=&(buf[0]);
  findStream.seekg(0,std::ios_base::beg);
  findStream.read(str,length);
  
  char* Buffer=new char[length];
  int i;
  boost::shared_ptr<std::vector<PointerType> > newPointers=boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
    for(i=0;i<Pointers->size();i++)
     {
       voidint.ul=(*Pointers)[i];
       stream->seekg(voidint.i,std::ios_base::beg);
       if(stream->read(Buffer,length).gcount()==length)
          if(memcmp(Buffer,str,length)==0)
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

void __fastcall TSearcher::Setstream(boost::shared_ptr<std::iostream> value)
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
  stream->clear();
  std::vector<char> buf;
  char* str;
  int length;
  std::stringstream& findStream = getFind();
  length=findStream.str().length();
  buf.resize(length);
  str=&(buf[0]);
  findStream.seekg(0,std::ios_base::beg);
  findStream.read(str,length);
  
  char* Buffer=new char[FPageSize+length-1];
  int i;
  int realBufferLength;
  int posBufInStream;
  DWORD streamSize =  stream->seekg(0,std::ios_base::end).tellg();
  stream->seekg(0,std::ios_base::beg);
  i=length-1;

  do
   {
    posBufInStream=stream->tellg();
    realBufferLength=stream->read(Buffer+length-1,FPageSize).gcount();
    for(;i<realBufferLength;i++)
     {
      if(memcmp(Buffer+i,str,length)==0)
       {voidint.i=i+posBufInStream-(length-1);Pointers->push_back(voidint.ul);}
      }
     if(realBufferLength==FPageSize){i=0;CopyMemory(Buffer,Buffer+FPageSize,length-1);}
      else i=length-1;
     if(NotifyEvent!=NULL)NotifyEvent((int)(100 * (double)stream->tellg() / (double )streamSize)) ;
   }while(*stream);
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
if(stream)
 {
  stream->clear();
  std::vector<char> buf;
  char* str;
  int length;
  std::stringstream& findStream = getFind();
  length=findStream.str().length();
  buf.resize(length);
  str=&(buf[0]);
  findStream.seekg(0,std::ios_base::beg);
  findStream.read(str,length);

  char* Buffer=new char[FPageSize];
  int i;
  int realBufferLength;
  int posBufInStream;
  DWORD streamSize = stream->seekg(0,std::ios_base::end).tellg();
  stream->seekg(0,std::ios_base::beg);
  i=0;
  while(*stream)
   {
    posBufInStream=stream->tellg();
    realBufferLength=stream->read(Buffer,FPageSize).gcount();
    for(i=0;i<=realBufferLength-length;i++)
     {
      if(memcmp(Buffer+i,str,length)==0)
       {voidint.i=i+posBufInStream;Pointers->push_back(voidint.ul);i+=length-1;}
      }
     if(NotifyEvent!=NULL)NotifyEvent((int)(100 * (double)stream->tellg() / (double )streamSize)) ;
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
if(stream)
{
for(int i=0;i<Pointers->size();i++)
 {
  stream->seekp((unsigned long)((*Pointers)[i]), std::ios_base::beg);

  std::vector<char> buf;
  char* str;
  int length;
  std::stringstream& replaceStream = getReplace();
  length=replaceStream.str().length();
  buf.resize(length);
  str=&(buf[0]);
  replaceStream.seekg(0,std::ios_base::beg);
  replaceStream.read(str,length);

  stream->write(str,length);
 }
}

}
void __fastcall TSearcher::Assign(boost::shared_ptr<TSearcher> src)
{
if(((bool)src)&&(src.get()!=this))
 {
  getFind().str(src->getFind().str());
  getReplace().str(src->getReplace().str());
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

__fastcall TSearcher::TSearcher(boost::shared_ptr<TSearcher> src)
{
Init();
Assign(src);
}

void TSearcher::Init(void)
{
  FPageSize=4096;
  Fstream=boost::shared_ptr<std::iostream>((std::iostream*)0);
  FPointers=boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
  FPointers->push_back(111);
  NotifyEvent=NULL;
}
//--------------------------------------------------------------------------
void __fastcall TSearcher::SetPageSize(int value)
{
 if(value>0)
  FPageSize=value;
}

std::stringstream& TSearcher::getFind()
{
    return FFind;
}

std::stringstream& TSearcher::getReplace()
{
    return FReplace;
}

