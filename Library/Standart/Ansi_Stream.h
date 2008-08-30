//---------------------------------------------------------------------------
#ifndef Ansi_StreamH
#define Ansi_StreamH
#include "ansilib.h"
#include "searcher.h"
#include <vector>
typedef unsigned int PointerType;
//---------------------------------------------------------------------------
//------------------------------------------------------
std::string  __fastcall  ConvertStreamToHexAnsi(TStream* stream);
std::string  __fastcall  ConvertStreamToHexAnsiWithSpace(TStream* stream);
int      __fastcall   WriteHexAnsiToStream(const std::string& Str,TStream* stream);
//-----------------------------------------------------------------------------
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"
                                       Text           Type                      len
   */

int __fastcall WriteTypedAnsiToStream(const std::string& Text, int Type, TStream*);
std::string __fastcall ConvertStreamToPrintString(TStream* stream);
int __fastcall WriteulongToStream(unsigned long num ,TStream* stream);
unsigned long __fastcall ConvertStreamToulong(TStream* stream);
int InsertDilimeters(TStream* src,TStream* dest,TStream* dilim, int step,int flag);
int RemoveDilimeters(TStream* src, TStream* dest,TStream* dilim );

template<class Container>
void WriteTListToStream(const Container& list,TStream* stream)
 {
    WORK_ANSILIB_UNION_FOR_CONVERT work;
    work._ulong=list.size();
  stream->Write(&work._ulong,sizeof(unsigned long));
  Container::const_iterator it = list.begin();
  Container::const_iterator itEnd = list.end();
  for(; it!=itEnd; ++it)
   {
    work._ulong=*it;
    stream->Write(&work._ulong,sizeof(unsigned long));
   }
 }
void ReadTListFromStream(std::vector<PointerType>& list,TStream* stream);
#endif
