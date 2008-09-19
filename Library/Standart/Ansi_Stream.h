//---------------------------------------------------------------------------
#ifndef Ansi_StreamH
#define Ansi_StreamH
#include "ansilib.h"
#include "searcher.h"
#include <vector>
#include <iostream>
typedef unsigned int PointerType;
//---------------------------------------------------------------------------
//------------------------------------------------------
std::string  __fastcall  ConvertStreamToHexAnsi(std::istream& io_stream);
std::string  __fastcall  ConvertStreamToHexAnsiWithSpace(std::istream& io_stream);
int      __fastcall   WriteHexAnsiToStream(const std::string& Str,std::ostream& io_stream);
//-----------------------------------------------------------------------------
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"
                                       Text           Type                      len
   */

int __fastcall WriteTypedAnsiToStream(const std::string& Text, int Type,std::ostream& io_stream);
std::string __fastcall ConvertStreamToPrintString(std::istream& io_stream);
int __fastcall WriteulongToStream(unsigned long num ,std::ostream& io_stream);
unsigned long __fastcall ConvertStreamToulong(std::istream& io_stream);
//int InsertDilimeters(TStream* src,TStream* dest,TStream* dilim, int step,int flag);
//int RemoveDilimeters(TStream* src, TStream* dest,TStream* dilim );

template<class Container>
void WriteTListToStream(const Container& list,std::ostream& io_stream)
 {
    WORK_ANSILIB_UNION_FOR_CONVERT work;
    work._ulong=list.size();
  io_stream.write((char*)&work._ulong,sizeof(unsigned long));
  Container::const_iterator it = list.begin();
  Container::const_iterator itEnd = list.end();
  for(; it!=itEnd; ++it)
   {
    work._ulong=*it;
    io_stream.write((char*)&work._ulong,sizeof(unsigned long));
   }
 }
void ReadTListFromStream(std::vector<PointerType>& list,std::istream& io_stream);
#endif
