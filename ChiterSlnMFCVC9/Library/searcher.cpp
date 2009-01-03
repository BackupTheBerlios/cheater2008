//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include <sstream>
#include <stdexcept>



#include "searcher.h"

//---------------------------------------------------------------------------

bool TSearcher::StartSearch(void)
{
  VoidInt voidint;
  FPointers->clear();
  if(getStream())
  {
    getStream()->clear();
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
    DWORD streamSize=getStream()->seekg(0,std::ios_base::end).tellg();
    getStream()->seekg(0,std::ios_base::beg);
    i=0;
    while(*(getStream()))
    {
      posBufInStream=getStream()->tellg();
      realBufferLength=getStream()->read(Buffer,FPageSize).gcount();
      for(i=0;i<=realBufferLength-length;i++)
      {
        if(memcmp(Buffer+i,str,length)==0)
        {voidint.i=i+posBufInStream;FPointers->push_back(voidint.ul);}
      }
      if(NotifyEvent!=NULL)NotifyEvent((int)(100 * (double)getStream()->tellg() / (double )streamSize)) ;
    }
    //  memcpy(Buffer,Buffer+FPageSize,length-1);
    //  i= 1-length;
    delete Buffer;
  }
  return FPointers->size()>0;
}
TSearcher::TSearcher(boost::shared_ptr<std::iostream> value)
{
  Init();
  Fstream=value;
}
TSearcher::~TSearcher()
{
}

bool  TSearcher::ContinueSearch(void)
{
  VoidInt voidint;
  if(getStream())
  {
    getStream()->clear();
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
    unsigned int i;
    boost::shared_ptr<std::vector<PointerType> > newPointers=boost::shared_ptr<std::vector<PointerType> >(new std::vector<PointerType>());
    for(i=0;i<FPointers->size();i++)
    {
      voidint.ul=(*FPointers)[i];
      getStream()->seekg(voidint.i,std::ios_base::beg);
      if(getStream()->read(Buffer,length).gcount()==length)
        if(memcmp(Buffer,str,length)==0)
          newPointers->push_back(voidint.ul);
    }
    FPointers=newPointers;
    delete Buffer;
  }
  else
  {
    FPointers->clear();
  }
  return FPointers->size()>0;
}

void  TSearcher::Setstream(boost::shared_ptr<std::iostream> value)
{
  Fstream=value;
}
//----------------------------------------------------------------------------
bool  TSearcher::SlowSearch(void)
{
  VoidInt voidint;
  FPointers->clear();
  if(getStream())
  {
    getStream()->clear();
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
    DWORD streamSize =  getStream()->seekg(0,std::ios_base::end).tellg();
    getStream()->seekg(0,std::ios_base::beg);
    i=length-1;

    do
    {
      posBufInStream=getStream()->tellg();
      realBufferLength=getStream()->read(Buffer+length-1,FPageSize).gcount();
      for(;i<realBufferLength;i++)
      {
        if(memcmp(Buffer+i,str,length)==0)
        {voidint.i=i+posBufInStream-(length-1);FPointers->push_back(voidint.ul);}
      }
      if(realBufferLength==FPageSize){i=0;CopyMemory(Buffer,Buffer+FPageSize,length-1);}
      else i=length-1;
      if(NotifyEvent!=NULL)NotifyEvent((int)(100 * (double)getStream()->tellg() / (double )streamSize)) ;
    }while(*(getStream()));
    //  memcpy(Buffer,Buffer+FPageSize,length-1);
    //  i= 1-length;
    delete Buffer;
  }
  return FPointers->size()>0;
}
bool  TSearcher::StartSearchDilim(void)
{
  VoidInt voidint;
  FPointers->clear();
  if(getStream())
  {
    getStream()->clear();
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
    DWORD streamSize = getStream()->seekg(0,std::ios_base::end).tellg();
    getStream()->seekg(0,std::ios_base::beg);
    i=0;
    while(*(getStream()))
    {
      posBufInStream=getStream()->tellg();
      realBufferLength=getStream()->read(Buffer,FPageSize).gcount();
      for(i=0;i<=realBufferLength-length;i++)
      {
        if(memcmp(Buffer+i,str,length)==0)
        {voidint.i=i+posBufInStream;FPointers->push_back(voidint.ul);i+=length-1;}
      }
      if(NotifyEvent!=NULL)NotifyEvent((int)(100 * (double)getStream()->tellg() / (double )streamSize)) ;
    }
    //  memcpy(Buffer,Buffer+FPageSize,length-1);
    //  i= 1-length;
    delete Buffer;
  }
  return FPointers->size()>0;
}
//--------------------------------------------------------------------------
void  TSearcher::ReplaceAll(void)
{
  if(getStream())
  {
    for(unsigned int i=0;i<FPointers->size();i++)
    {
      getStream()->seekp((unsigned long)((*FPointers)[i]), std::ios_base::beg);

      std::vector<char> buf;
      char* str;
      int length;
      std::stringstream& replaceStream = getReplace();
      length=replaceStream.str().length();
      buf.resize(length);
      str=&(buf[0]);
      replaceStream.seekg(0,std::ios_base::beg);
      replaceStream.read(str,length);

      getStream()->write(str,length);
    }
  }

}
void  TSearcher::Assign(boost::shared_ptr<TSearcher> src)
{
  if(((bool)src)&&(src.get()!=this))
  {
    getFind().str(src->getFind().str());
    getReplace().str(src->getReplace().str());
    FPageSize=src->FPageSize;
    Fstream=src->Fstream;
    (*FPointers) = *(src->FPointers);
  }
  else
  {
    std::stringstream msg;
    msg << "src is either null or itselt" << std::endl << std::endl
      << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
    throw std::runtime_error( msg.str() );
  }

}

TSearcher::TSearcher(boost::shared_ptr<TSearcher> src)
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
void  TSearcher::SetPageSize(int value)
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

