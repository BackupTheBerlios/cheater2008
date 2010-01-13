//---------------------------------------------------------------------------
#ifndef Ansi_StreamH
#define Ansi_StreamH
#include "ansilib.h"
//#include "searcher.h"
#include <vector>
#include <iostream>
#include "StandartFwd.h"
typedef unsigned int PointerType;
//---------------------------------------------------------------------------
//------------------------------------------------------
STANDART_EXPORT_FUN(std::string)    ConvertStreamToHexAnsi(std::istream& io_stream);
STANDART_EXPORT_FUN(std::string)    ConvertStreamToHexAnsiWithSpace(std::istream& io_stream);
STANDART_EXPORT_FUN(int)         WriteHexAnsiToStream(const std::string& Str,std::ostream& io_stream);
//-----------------------------------------------------------------------------
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"
                                       Text           Type                      len
   */

STANDART_EXPORT_FUN(int)  WriteTypedAnsiToStream(const std::string& Text, int Type,std::ostream& io_stream);
STANDART_EXPORT_FUN(std::string)  ConvertStreamToPrintString(std::istream& io_stream);
STANDART_EXPORT_FUN(int)  WriteulongToStream(unsigned long num ,std::ostream& io_stream);
STANDART_EXPORT_FUN(unsigned long)  ConvertStreamToulong(std::istream& io_stream);
//int InsertDilimeters(TStream* src,TStream* dest,TStream* dilim, int step,int flag);
//int RemoveDilimeters(TStream* src, TStream* dest,TStream* dilim );

template<class CommandsContainer>
void WriteTListToStream(const CommandsContainer& list,std::ostream& io_stream)
 {
    WORK_ANSILIB_UNION_FOR_CONVERT work;
    work._ulong=list.size();
  io_stream.write((char*)&work._ulong,sizeof(unsigned long));
  CommandsContainer::const_iterator it = list.begin();
  CommandsContainer::const_iterator itEnd = list.end();
  for(; it!=itEnd; ++it)
   {
    work._ulong=*it;
    io_stream.write((char*)&work._ulong,sizeof(unsigned long));
   }
 }

STANDART_EXPORT_FUN(void) ReadTListFromStream(std::vector<PointerType>& list,std::istream& io_stream);

#endif

