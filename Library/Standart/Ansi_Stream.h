//---------------------------------------------------------------------------

#ifndef Ansi_StreamH
#define Ansi_StreamH
#include "ansilib.h"
#include "searcher.h"
//---------------------------------------------------------------------------
//------------------------------------------------------
AnsiString  __fastcall  ConvertStreamToHexAnsi(TStream* stream);
AnsiString  __fastcall  ConvertStreamToHexAnsiWithSpace(TStream* stream);
int      __fastcall   WriteHexAnsiToStream(const AnsiString& Str,TStream* stream);
//-----------------------------------------------------------------------------
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"
                                       Text           Type                      len
   */
int __fastcall WriteTypedAnsiToStream(const AnsiString& Text, int Type, TStream*);
AnsiString __fastcall ConvertStreamToPrintString(TStream* stream);
int __fastcall WriteulongToStream(unsigned long num ,TStream* stream);
unsigned long __fastcall ConvertStreamToulong(TStream* stream);
int InsertDilimeters(TStream* src,TStream* dest,TStream* dilim, int step,int flag);
int RemoveDilimeters(TStream* src, TStream* dest,TStream* dilim );
void WriteTListToStream(TList* list,TStream* stream);
void ReadTListFromStream(TList* list,TStream* stream);
#endif
