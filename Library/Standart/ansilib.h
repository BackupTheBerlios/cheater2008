//---------------------------------------------------------------------------
#ifndef __Ansilib_H
#define __Ansilib_H

#define dlLeft 0x1
#define dlRight  0x2
#define dlLeftRight 0x3
#define dlNone      0x0

#include <string>
extern char* EMPTY_STRING;
extern char* SPACE_STRING;
extern char* StringType[];
enum  StringIndex{HEX_NUM,DEC_NUM,STRING,HEX_STRING,FLOAT_NUM,DOUBLE_NUM};
extern unsigned char* PRINT_TABLE;
/*extern char* HEX_NUM;
extern char* DEC_NUM;
extern char* STRING;
extern char* HEX_STRING;
extern char* SPACE_STRING;
extern char* EMPTY_STRING;*/
extern unsigned long HEX_MASK_4;
extern unsigned long HEX_MASK_3;
extern unsigned long HEX_MASK_2;
extern unsigned long HEX_MASK_1;
union WORK_ANSILIB_UNION_FOR_CONVERT
     {
      char char_;
      byte byte_;
      byte bt[sizeof(unsigned long)];
      char ch[sizeof(unsigned long)];
      unsigned long _ulong;
      int _int;
      double double_;
	  float float_;
      void* pvoid;
     };
std::string  __fastcall  byteptrToHexAnsi(byte *, unsigned long);
std::string  __fastcall  byteptrToHexAnsiWithSpace(byte *, unsigned long);
byte*      __fastcall   HexAnsiTobyteptr(const std::string&);
std::string  __fastcall  ulongToAnsi(unsigned long);
unsigned long __fastcall AnsiToulong(const std::string&);
std::string  __fastcall  ulongToHexAnsi(unsigned long);
std::string  __fastcall  ulongToHexAnsi(unsigned long,unsigned long);
unsigned long __fastcall HexAnsiToulong(const std::string&);
bool IsDouble(std::string&);
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"

                                       Text           Type                      len
   */
byte* __fastcall TypedAnsiTobyteptr( const std::string& , int , unsigned long*  );
unsigned long __fastcall PointerAnsiToulong( const std::string& , int );
unsigned long __fastcall FindInBuf(byte *,unsigned long,byte *,unsigned long );
int WhatAnsiType(const std::string& );
unsigned char __fastcall ConvertToPrintSign(unsigned char);
std::string __fastcall ConvertToPrintString(byte* ,unsigned long ) ;
std::string __fastcall ulongTo8digitHexString(unsigned long);
byte __fastcall KeyToHex(WORD key);
byte* __fastcall ulongTobyteptr(unsigned long,unsigned long*);
//unsigned long __fastcall byteptrToulong(byte*,unsigned long);
char* InsertDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int step,int flag,int* resLen=NULL);
char* RemoveDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int* resLen);
unsigned long __fastcall byteptrToulong(const byte* buf,unsigned long len);
//---------------------------------------------------------------------------

// Auxiliary functions
std::string IntToHex(int i_num);
int ToInt(const std::string& i_str);
double ToDouble(const std::string& i_str);
std::string IntToString(int i_num);
#endif

