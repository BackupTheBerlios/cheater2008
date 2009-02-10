//---------------------------------------------------------------------------

#ifndef __Ansilib_H
#define __Ansilib_H

#define dlLeft 0x1
#define dlRight  0x2
#define dlLeftRight 0x3
#define dlNone      0x0

#include <string>
#include "StandartFwd.h"
extern char* EMPTY_STRING;
extern char* SPACE_STRING;
STANDART_EXPORT_FUN(wchar_t**) getStringType();
enum  StringIndex{HEX_NUM=0,DEC_NUM,STRING,HEX_STRING,FLOAT_NUM,DOUBLE_NUM};
extern const char* PRINT_TABLE;
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
      char byte_;
      char bt[sizeof(unsigned long)];
      char ch[sizeof(unsigned long)];
      unsigned long _ulong;
      int _int;
      double double_;
	  float float_;
      void* pvoid;
     };
STANDART_EXPORT_FUN(std::string)    charptrToHexAnsi(char *, unsigned long);
STANDART_EXPORT_FUN(std::string)    charptrToHexAnsiWithSpace(char *, unsigned long);
STANDART_EXPORT_FUN(char*)         HexAnsiTocharptr(const std::string&);
STANDART_EXPORT_FUN(std::string)    ulongToAnsi(unsigned long);
STANDART_EXPORT_FUN(unsigned long)  AnsiToulong(const std::string&);
STANDART_EXPORT_FUN(std::string)    ulongToHexAnsi(unsigned long);
STANDART_EXPORT_FUN(std::string)    ulongToHexAnsi(unsigned long,unsigned long);
STANDART_EXPORT_FUN(unsigned long)  HexAnsiToulong(const std::string&);
STANDART_EXPORT_FUN(bool) IsDouble(std::string&);
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"

                                       Text           Type                      len
   */
STANDART_EXPORT_FUN(char*)  TypedAnsiTocharptr( const std::string& , int , unsigned long*  );
STANDART_EXPORT_FUN(unsigned long)  PointerAnsiToulong( const std::string& , int );
STANDART_EXPORT_FUN(unsigned long)  FindInBuf(char *,unsigned long,char *,unsigned long );
STANDART_EXPORT_FUN(int) WhatAnsiType(const std::string& );
STANDART_EXPORT_FUN(unsigned char)  ConvertToPrintSign(unsigned char);
STANDART_EXPORT_FUN(std::string)  ConvertToPrintString(char* ,unsigned long ) ;
STANDART_EXPORT_FUN(std::string)  ulongTo8digitHexString(unsigned long);
STANDART_EXPORT_FUN(char)  KeyToHex(WORD key);
STANDART_EXPORT_FUN(char*)  ulongTocharptr(unsigned long,unsigned long*);
//unsigned long  charptrToulong(char*,unsigned long);
STANDART_EXPORT_FUN(char*) InsertDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int step,int flag,int* resLen=NULL);
STANDART_EXPORT_FUN(char*) RemoveDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int* resLen);
STANDART_EXPORT_FUN(unsigned long)  charptrToulong(const char* buf,unsigned long len);
//---------------------------------------------------------------------------

// Auxiliary functions
// width minimum number of output digits 
STANDART_EXPORT_FUN(std::string) intToHex(unsigned int i_num,int i_width);
STANDART_EXPORT_FUN(int) ToInt(const std::string& i_str);
STANDART_EXPORT_FUN(double) ToDouble(const std::string& i_str);
STANDART_EXPORT_FUN(std::string) IntToString(int i_num);
#endif

