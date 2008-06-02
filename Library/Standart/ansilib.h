//---------------------------------------------------------------------------
#ifndef __Ansilib_H
#define __Ansilib_H
#define dlLeft 0x1
#define dlRight  0x2
#define dlLeftRight 0x3
#define dlNone      0x0
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
    AnsiString  __fastcall  byteptrToHexAnsi(byte *, unsigned long);
    AnsiString  __fastcall  byteptrToHexAnsiWithSpace(byte *, unsigned long);    
	byte*      __fastcall   HexAnsiTobyteptr(const AnsiString&);

    AnsiString  __fastcall  ulongToAnsi(unsigned long);
    unsigned long __fastcall AnsiToulong(const AnsiString&);

    AnsiString  __fastcall  ulongToHexAnsi(unsigned long);
    AnsiString  __fastcall  ulongToHexAnsi(unsigned long,unsigned long);
    unsigned long __fastcall HexAnsiToulong(const AnsiString&);
    bool IsDouble(AnsiString&);
/*
  porible types:
  "dec num"
  "hex num"
  "string"
  "hex string"
                                       Text           Type                      len
   */
byte* __fastcall TypedAnsiTobyteptr( const AnsiString& , int , unsigned long*  );
unsigned long __fastcall PointerAnsiToulong( const AnsiString& , int );
unsigned long __fastcall FindInBuf(byte *,unsigned long,byte *,unsigned long );
int WhatAnsiType(const AnsiString& );
unsigned char __fastcall ConvertToPrintSign(unsigned char);
AnsiString __fastcall ConvertToPrintString(byte* ,unsigned long ) ;
AnsiString __fastcall ulongTo8digitHexString(unsigned long);
byte __fastcall KeyToHex(WORD key);
byte* __fastcall ulongTobyteptr(unsigned long,unsigned long*);
//unsigned long __fastcall byteptrToulong(byte*,unsigned long);
char* InsertDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int step,int flag,int* resLen=NULL);
char* RemoveDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int* resLen);
unsigned long __fastcall byteptrToulong(byte* buf,int len);
//---------------------------------------------------------------------------
#endif
