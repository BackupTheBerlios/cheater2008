//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include "Ansi_Stream.h"

//---------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <stdexcept>
//------------------------------------------------------
std::string    ConvertStreamToHexAnsi(std::istream& io_stream)
 {
  std::string ret;
  io_stream.clear();
  std::istream::streampos backupPos = io_stream.tellg();
  io_stream.seekg(0,std::ios_base::beg);
   WORK_ANSILIB_UNION_FOR_CONVERT work;
      work._ulong=0;
   while(io_stream.read((char*)&work._ulong,1))
    {
	  ret += intToHex(work._int,2);
    }
  io_stream.clear();
  io_stream.seekg(backupPos,std::ios_base::beg);
  return ret;
 }
//-----------------------------------------------------------------------------
std::string    ConvertStreamToHexAnsiWithSpace(std::istream& io_stream)
 {
  io_stream.clear();
  std::istream::streampos backupPos = io_stream.tellg();
  std::string res=EMPTY_STRING;
   WORK_ANSILIB_UNION_FOR_CONVERT work;
      work._ulong=0;
  io_stream.seekg(0,std::ios_base::beg);
     if( io_stream.read((char*)&work._ulong,1) )
         res+=intToHex(work._int,2);
     while(io_stream.read((char*)&work._ulong,1))
      {
        res+=std::string(SPACE_STRING);
		res+=intToHex(work._int,2);

      }
  io_stream.clear();
  io_stream.seekg(backupPos,std::ios_base::beg);
  return res;
 }
//-----------------------------------------------------------------------------
int         WriteHexAnsiToStream(const std::string& Str,std::ostream& io_stream)
 {
    io_stream.clear();
  WORK_ANSILIB_UNION_FOR_CONVERT work;
unsigned long i=0;
  if(Str.size()!=0)
   {
	for(i;i<Str.size()/2;i++)
     {
	  work._ulong=HexAnsiToulong(Str.substr(2*i,2));
      io_stream.write((char*)&work.byte_,1);
     }
   }
  return i;
 }

//-----------------------------------------------------------------------------
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"
                                       Text           Type                      len
   */
int  WriteTypedAnsiToStream(const std::string& Text, int Type, std::ostream& io_stream)
 {

  int len=0;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  switch(Type)
   {
    case DEC_NUM:
	   work._int=ToInt(Text);
       len=WriteulongToStream(work._ulong,io_stream);
    break;
    case HEX_NUM:
       len=WriteulongToStream(HexAnsiToulong(Text),io_stream);
    break;
    case HEX_STRING:
        len=WriteHexAnsiToStream(Text,io_stream);
    break;
    case DOUBLE_NUM:
      len=sizeof(double);
	  work.double_=ToDouble(Text);
      io_stream.write((char*)&work.double_,len);
    break;
    case FLOAT_NUM:
      len=sizeof(float);
      work.float_=(float)ToDouble(Text);
      io_stream.write((char*)&work.float_,len);
    break;
    default:
            len=Text.size();
            io_stream.clear();             
            io_stream.write(Text.c_str(),len);
   }


 return len;
 }

//-----------------------------------------------------------------------------
//unsigned long  FindInBuf(byte *,unsigned long,byte *,unsigned long );
//-----------------------------------------------------------------------------
std::string  ConvertStreamToPrintString(std::istream& io_stream)
 {
  io_stream.clear(); 
  std::istream::streampos backupPos = io_stream.tellg();
  io_stream.seekg(0,std::ios_base::beg);
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work.ch[1]=0;
 while(io_stream.read((char*)&work.char_,1))
      {
       ConvertToPrintSign(work.byte_);
       res+=std::string(work.ch);
      }
  io_stream.clear();
  io_stream.seekg(backupPos,std::ios_base::beg);
  return res;
 }
//-----------------------------------------------------------------------------
int  WriteulongToStream(unsigned long num ,std::ostream& io_stream)
 {
  int len=4;
  io_stream.clear();  
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=num;
  std::ostream::streampos beg = io_stream.tellp();
   for(;len>0;len--)
    {
      if(work.bt[len-1]!=0) break;
    }
   if(len!=0)
      io_stream.write((char*)work.bt,len);
   else
    {len++;
     io_stream.write((char*)work.bt,len);
    }
  std::ostream::streampos end = io_stream.tellp();
  return (end-beg);
 }
//-----------------------------------------------------------------------------
unsigned long  ConvertStreamToulong(std::istream& io_stream)
 {
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=0;
  io_stream.clear();
  std::istream::streampos backupPos = io_stream.tellg();
  std::istream::streampos size=io_stream.seekg(0,std::ios_base::end).tellg();
  io_stream.seekg(0,std::ios_base::beg);
  if(size>4)
     io_stream.read((char*)&work._ulong,4);
  else
    io_stream.read((char*)&work._ulong,size);
  io_stream.clear();
  io_stream.seekg(backupPos,std::ios_base::beg);
  return work._ulong;
 }
 /*
//--------------------------------------------------------------------------------------------------------
int InsertDilimeters(std::istream& src,
std::ostream& dest,
std::istream& dilim, int step,int flag)
 {
   int Len=0;
  src.seekg(0,std::ios_base::beg);
       if(flag&dlLeft!=0) Len+=dest->CopyFrom(dilim,0);
    for(int j=0;j<src->Size-step;j+=step)
     {
       Len+=dest->CopyFrom(src,step);
        Len+=dest->CopyFrom(dilim,0);
     }
        dest->CopyFrom(src,step);
//        memcpy(res+i,dilim,dilimLen);
//        i+=dilimLen;

  //  if(j!=srcLen) {memcpy(res+i,src+j-step,srcLen-(j-step));i+=srcLen-(j-step);}
       if((flag&dlRight)!=0) Len+=dest->CopyFrom(dilim,0);
   return Len;
 }         */
//--------------------------------------------------------------------------------------------------------
/*
int RemoveDilimeters(TStream* src, TStream* dest,TStream* dilim )
 {
   int Len=0;
   int i;
   src->Position=0;
   char ch,work;
   while(src->Position<src->Size)
    {
     dilim->Position=0;
     do
      {
       src->Read(&ch,1);
       dilim->Read(&work,1);
       if(ch!=work) break;
      }while(dilim->Position<dilim->Size);
     if(dilim->Position!=dilim->Size) dest->Write(&ch,1);
    }
   return Len;
 }
 */
//--------------------------------------------------------------------------

/*void WriteTListToStream(TList* list,TStream* stream)
 {
WORK_ANSILIB_UNION_FOR_CONVERT work;
work._ulong=list->Count;
  stream->Write(&work._ulong,sizeof(unsigned long));
  for(int i=0;i<list->Count;i++)
   {
    work.pvoid=list->Items[i];
    stream->Write(&work._ulong,sizeof(unsigned long));
   }
 }*/
//--------------------------------------------------------------------------

void ReadTListFromStream(std::vector<PointerType>&  list,std::istream& io_stream)
 {
WORK_ANSILIB_UNION_FOR_CONVERT work;
unsigned long len;
  io_stream.clear();
  io_stream.read((char*)&len,sizeof(unsigned long));
  if(!io_stream)
   {
      std::stringstream msg;
      msg << "Wrong data format" << std::endl << std::endl
          << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
      throw std::runtime_error( msg.str() );
   }

  for(unsigned int i=0;i<len;i++)
   {
    io_stream.read((char*)&work._ulong,sizeof(PointerType));
    if(!io_stream)
    {
        std::stringstream msg;
        msg << "Wrong data format" << std::endl << std::endl
            << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
        throw std::runtime_error( msg.str() );
    }
    list.push_back(work._ulong);
   }
 }
//--------------------------------------------------------------------------

