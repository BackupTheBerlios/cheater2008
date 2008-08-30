//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Ansi_Stream.h"

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//------------------------------------------------------
std::string  __fastcall  ConvertStreamToHexAnsi(TStream* stream)
 {
  std::string ret;
  stream->Position=0;
   WORK_ANSILIB_UNION_FOR_CONVERT work;
      work._ulong=0;
   while(stream->Position<stream->Size)
    {
      stream->Read(&work._ulong,1);
	  ret += intToHex(work._int,2);
    }
  return ret;
 }
//-----------------------------------------------------------------------------
std::string  __fastcall  ConvertStreamToHexAnsiWithSpace(TStream* stream)
 {
  std::string res=EMPTY_STRING;
   WORK_ANSILIB_UNION_FOR_CONVERT work;
      work._ulong=0;
  stream->Position=0;
  if(stream->Size!=stream->Position)
   {
     while(stream->Position<stream->Size-1)
      {
        stream->Read(&work._ulong,1);
		res+=intToHex(work._int,2);
       res+=std::string(SPACE_STRING);
      }
      stream->Read(&work._ulong,1);
      res+=intToHex(work._int,2);
   }
  return res;
 }
//-----------------------------------------------------------------------------
int      __fastcall   WriteHexAnsiToStream(const std::string& Str,TStream* stream)
 {
  WORK_ANSILIB_UNION_FOR_CONVERT work;
unsigned long i=0;
  if(Str.size()!=0)
   {
	for(i;i<Str.size()/2;i++)
     {
	  work._ulong=HexAnsiToulong(Str.substr(2*i,2));
      stream->Write(&work.byte_,1);
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
int __fastcall WriteTypedAnsiToStream(const std::string& Text, int Type, TStream* stream)
 {

  int len=0;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  switch(Type)
   {
    case DEC_NUM:
	   work._int=ToInt(Text);
       len=WriteulongToStream(work._ulong,stream);
    break;
    case HEX_NUM:
       len=WriteulongToStream(HexAnsiToulong(Text),stream);
    break;
    case HEX_STRING:
        len=WriteHexAnsiToStream(Text,stream);
    break;
    case DOUBLE_NUM:
      len=sizeof(double);
	  work.double_=ToDouble(Text);
      stream->Write(&work.double_,len);
    break;
    case FLOAT_NUM:
      len=sizeof(float);
      work.float_=(float)ToDouble(Text);
      stream->Write(&work.float_,len);
    break;
    default:
            len=Text.size();
            stream->Write(Text.c_str(),len);
   }


 return len;
 }

//-----------------------------------------------------------------------------
//unsigned long __fastcall FindInBuf(byte *,unsigned long,byte *,unsigned long );
//-----------------------------------------------------------------------------
std::string __fastcall ConvertStreamToPrintString(TStream* stream)
 {
  stream->Position=0;
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work.ch[1]=0;
 while(stream->Position<stream->Size)
      {
       stream->Read(&work.char_,1);
       ConvertToPrintSign(work.byte_);
       res+=std::string(work.ch);
      }

  return res;
 }
//-----------------------------------------------------------------------------
int __fastcall WriteulongToStream(unsigned long num ,TStream* stream)
 {
  int len=4;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=num;
   for(;len>0;len=len-1)
    {
      if(work.bt[len-1]!=0) break;
    }
   if(len!=0)
      len=stream->Write(work.bt,len);
   else
    {len++;
     len=stream->Write(work.bt,len);
    }
  return len;
 }
//-----------------------------------------------------------------------------
unsigned long __fastcall ConvertStreamToulong(TStream* stream)
 {
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=0;
  stream->Position=0;
  if(stream->Size>4)
     stream->Read(&work._ulong,4);
  else
    stream->Read(&work._ulong,stream->Size);
  return work._ulong;
 }
//--------------------------------------------------------------------------------------------------------
int InsertDilimeters(TStream* src,TStream* dest,TStream* dilim, int step,int flag)
 {
   int Len=0;
   src->Position=0;
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
 }
//--------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
void WriteTListToStream(TList* list,TStream* stream)
 {
WORK_ANSILIB_UNION_FOR_CONVERT work;
work._ulong=list->Count;
  stream->Write(&work._ulong,sizeof(unsigned long));
  for(int i=0;i<list->Count;i++)
   {
    work.pvoid=list->Items[i];
    stream->Write(&work._ulong,sizeof(unsigned long));
   }
 }
//--------------------------------------------------------------------------

void ReadTListFromStream(TList* list,TStream* stream)
 {
WORK_ANSILIB_UNION_FOR_CONVERT work;
unsigned long len;
    stream->Read(&len,sizeof(unsigned long));
  for(int i=0;i<len;i++)
   {
    stream->Read(&work._ulong,sizeof(unsigned long));
    list->Add(work.pvoid);
   }
 }
//--------------------------------------------------------------------------
#pragma package(smart_init)

