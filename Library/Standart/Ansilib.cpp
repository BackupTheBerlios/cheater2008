//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <sstream>
#include <iomanip>
#include "Ansilib.h"
char* StringType[]={"hex num","dec num","string","hex string","float num","double num"};

/* char* HEX_NUM="hex num" ;
 char* DEC_NUM="dec num" ;
 char* STRING="string";
 char* HEX_STRING="hex string";
 char* FLOAT_NUM="float num";
 char* DOUBLE_NUM="double num";*/
 char* EMPTY_STRING="";
 char* SPACE_STRING=" ";
 unsigned long HEX_MASK_4=0xFF000000;
 unsigned long HEX_MASK_3=0x00FF0000;
 unsigned long HEX_MASK_2=0x0000FF00;
 unsigned long HEX_MASK_1=0x000000FF;
 unsigned char* PRINT_TABLE="`1234567890-=qwertyuiop[]';lkjhgfdsazxcvbnm,./ \~!@#$%^&*()_+}{POIUYTREWQASDFGHJKL:""?><MNBVCXZ|éöóêåíãøùçõúýæäëîðïàâûôÿ÷ñìèòüáþ.\ÉÖÓÊÅÍÃØÙÇÕÚÝÆÄËÎÐÏÀÂÛÔß×ÑÌÈÒÜÁÞ,/ ";

std::string IntToHex(int i_num)
{
	std::stringstream ret;
	ret << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << i_num;
	return ret.str();
}

int ToInt(const std::string& i_str)
{
	int ret;
	std::stringstream stream(i_str);
	stream >> ret;
	return ret;
}

double ToDouble(const std::string& i_str)
{
	double ret;
	std::stringstream stream(i_str);
	stream >> ret;
	return ret;
}

std::string IntToString(int i_num)
{
	std::stringstream ret;
	ret << i_num;
	return ret.str();
}

//------------------------------------------------------
std::string  __fastcall  byteptrToHexAnsi(byte * buf, unsigned long len)
 {
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  if(buf!=NULL)
   {
    for(unsigned long i=0;i<len;i++)
     {
      work._ulong=buf[i];
	  res+=IntToHex(work._int);
     }
   }
  return res;
 }

//-----------------------------------------------------------------------------
std::string  __fastcall  byteptrToHexAnsiWithSpace(byte * buf, unsigned long len)
 {
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  unsigned long i=0;
  if(buf!=NULL)
   {
    if(len!=0)
     {
      for(;i<len-1;i++)
       {
        work._ulong=buf[i];
		res+=IntToHex(work._int);
        res+=std::string(SPACE_STRING);
       }
      work._ulong=buf[i];
      res+=IntToHex(work._int);
     }
   }
  return res;
 }

//-----------------------------------------------------------------------------
byte*      __fastcall   HexAnsiTobyteptr(const std::string& Str)
 {
  byte* buf=NULL;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  if(Str.size()!=0)
   {
	buf=new byte[Str.size()/2];
	for(unsigned long i=0;i<Str.size()/2;i++)
     {
      work._ulong=HexAnsiToulong(Str.substr(2*i,2));
      buf[i]=work.byte_;
     }
   }
  return buf;
 }

//-----------------------------------------------------------------------------
std::string  __fastcall  ulongToAnsi(unsigned long num)
 {
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  while(num>0)
   {
    work._ulong=num%10;
    num=num/10;
	res.insert(0,IntToString(work._int));
   }
  if(res.size()==0) res=std::string("0");
  return res;
 }

//-----------------------------------------------------------------------------
unsigned long __fastcall AnsiToulong(const std::string& Str)
 {
  unsigned long res=0;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  unsigned long pow=1;
  const char* str_=Str.c_str();
  for(unsigned long i=Str.size();i>0;i--)
   {
    work._ulong=0;
    work.char_=str_[i-1];
    if((work.byte_>='0')&&(work.byte_<='9'))
     {
      work.char_=work.char_-'0';
     }
    res+=pow*work.byte_;
    pow=pow*10;
   }
return res;
 }
//-----------------------------------------------------------------------------
std::string  __fastcall  ulongToHexAnsi(unsigned long num)
 {
  std::stringstream ret;
  ret << std::hex << std::uppercase << num;
  if(ret.str().size()==0) ret << "0";
  return ret.str();
 }
//-----------------------------------------------------------------------------
std::string  __fastcall  ulongToHexAnsi(unsigned long num,unsigned long len)
 {
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  for(unsigned long i=0;i<len;i++)
   {
    work._ulong=num%16;
    num=num/16;
    res.insert(0,IntToHex(work._int));
   }
  return res;
 }
//-----------------------------------------------------------------------------
unsigned long __fastcall HexAnsiToulong(const std::string& Str )
 {
  unsigned long res=0;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  unsigned long pow=1;
  const char* str_=Str.c_str();
  for(unsigned long i=Str.size();i>0;i--)
   {
    work._ulong=0;
    work.char_=str_[i-1];
    if((work.byte_>='0')&&(work.byte_<='9'))
     {
      work.char_=work.char_-'0';
     }
    else
     {
       if((work.byte_>='a')&&(work.byte_<='f'))
        {
         work.char_=work.char_-'a'+10;
        }
       else
        {
         if((work.byte_>='A')&&(work.byte_<='F'))
          {
           work.char_=work.char_-'A'+10;
          }
        }
     }
    res+=pow*work.byte_;
    pow=pow*16;
   }
return res;
 }

//-----------------------------------------------------------------------------
bool IsDouble(std::string& Str)
 {
  bool res,IsPoint;
  IsPoint=false;
  res=true;
  unsigned long i=0;
  const char * str_=Str.c_str();
  if(Str.size()>0)
   if (str_[0]=='-') i++;
  for(;i<Str.size();i++)
   {
    if((str_[i]<='0')||(str_[i]>='9'))
     {
      if(str_[i]=='.')
       {
         if(IsPoint)
          {
           res=false;
           break;
          }
         else
          {
           IsPoint=true;
          }
       }
      else
       {
        res=false;
        break;
       }
     }
   }
  return res;
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
byte* __fastcall TypedAnsiTobyteptr(const std::string& Text, int Type, unsigned long* len )
 {
  byte* res=NULL;
  *len=0;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  switch(Type)
   {
    case DEC_NUM:
	   work._int=ToInt(Text);
       res=ulongTobyteptr(work._ulong,len);
    break;
    case HEX_NUM:
       work._ulong=HexAnsiToulong(Text);
       res=ulongTobyteptr(work._ulong,len);
    break;
    case HEX_STRING:
        res=HexAnsiTobyteptr(Text);
		*len=Text.size()/2;
    break;
    case DOUBLE_NUM:
      *len=sizeof(double);
      res=new char[*len];
	  work.double_=ToDouble(Text);
      memcpy(res,&(work.double_),*len);
    break;
    case FLOAT_NUM:
      *len=sizeof(float);
      res=new char[*len];
      work.float_=(float)ToDouble(Text);
      memcpy(res,&(work.float_),*len);
    break;
    default:
			*len=Text.size();
             res=new byte[*len];
             memcpy(res,Text.c_str(),*len);
   }
 return res;
 }

unsigned long __fastcall PointerAnsiToulong(const std::string& Text, int Type)
 {
unsigned long res=0;
unsigned long len;
const byte* buf;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
 if(Text.size()!=0)
{
  if(Type==DEC_NUM)
   {
    work._int=ToInt(Text);
    res=work._ulong;
   }
  else
   {
    if(Type==HEX_NUM)
     {
       res=HexAnsiToulong(Text);
     }
    else
     {
      if(Type==HEX_STRING)
       {
         buf=HexAnsiTobyteptr(Text);
         len=Text.size()/2;
       }
      else
       {
         buf=Text.c_str();
		 len=Text.size();
       }
      if((buf!=NULL)&&(len!=0))
        {
         res=byteptrToulong(buf,len);
        }
     }
   }
}
 return res;
 }
//-----------------------------------------------------------------------------
//unsigned long __fastcall FindInBuf(byte *,unsigned long,byte *,unsigned long );
//-----------------------------------------------------------------------------
int WhatAnsiType(const std::string& value)
{
  int res=DEC_NUM;
  for (int i=0;i<value.size();i++)
   {
    if((value[i]<'0')||(value[i]>'9'))
     if(((value[i]>='a')&&(value[i]<='f'))||((value[i]>='A')&&(value[i]<='F')))
      {
        if(res==DEC_NUM)
         res=HEX_STRING;
      }
     else
      {
       res=STRING;
       break;
      }
   }
  return res;
 }

//-----------------------------------------------------------------------------
unsigned char __fastcall ConvertToPrintSign(unsigned char value)
 {
  unsigned char ch='.';
  unsigned int len=strlen(PRINT_TABLE);
  bool Is=false;
  for (unsigned int i=0;i<len;i++)
   {
     if(value==PRINT_TABLE[i])
      {
       Is=true;
       break;
      }
   }
  if(Is) ch=value;
   return ch;
 }

//-----------------------------------------------------------------------------
std::string __fastcall ConvertToPrintString(byte* buf,unsigned long len)
 {
  std::string res=EMPTY_STRING;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work.ch[1]=0;
  if((buf!=NULL)&&(len>0))
   {
     for(unsigned long i=0;i<len;i++)
      {
       work.bt[0]=ConvertToPrintSign(buf[i]);
       res+=std::string(work.ch);
      }
   }
  return res;
 }
//-----------------------------------------------------------------------------
std::string __fastcall ulongTo8digitHexString(unsigned long num)
 {
  return ulongToHexAnsi(num,8);
 }

//-----------------------------------------------------------------------------
byte __fastcall KeyToHex(WORD key)
 {
 if ((key>=0x30)&&(key<=0x39)) return key-0x30;
 if ((key>=0x41)&&(key<=0x46)) return key-0x41+10;
 return 0xff;
 }

//-----------------------------------------------------------------------------
byte* __fastcall ulongTobyteptr(unsigned long num ,unsigned long* len)
 {
  byte* res=NULL;
  *len=4;
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=num;
   for(;*len>0;*len=*len-1)
    {
      if(work.bt[*len-1]!=0) break;
    }
   if(*len!=0)
    {
     res=new byte[*len];
     memcpy(res,work.bt,*len);
    }
  return res;
 }

//-----------------------------------------------------------------------------
unsigned long __fastcall byteptrToulong(const byte* buf,unsigned long len)
 {
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=0;
  if((buf!=NULL)&&(len!=0))
   {
    work._ulong=0;
     if(len>4) len=4;
     for(unsigned long i=0;i<len;i++)
      {
       work.bt[i]=buf[len-i-1];
      }
   }
  return work._ulong;
 }

//--------------------------------------------------------------------------------------------------------
char* InsertDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int step,int flag,int* resLen)
 {
   char* res;
   int i,j;
    int newLen=dilimLen*(srcLen/step)+srcLen;
    if(srcLen!=0)
     if(srcLen%step==0) newLen-=dilimLen;
    if((flag&dlLeft)!=0) newLen+=dilimLen;
    if((flag&dlRight)!=0) newLen+=dilimLen;
   res=new byte[newLen];
   i=0;
       if(flag&dlLeft!=0) {memcpy(res,dilim,dilimLen);i+=dilimLen;}
    for(j=0;j<srcLen-step;j+=step)
     {
        memcpy(res+i,src+j,step);
        i+=step;
        memcpy(res+i,dilim,dilimLen);
        i+=dilimLen;
     }
        memcpy(res+i,src+j,srcLen-j);
        i+=srcLen-j;
//        memcpy(res+i,dilim,dilimLen);
//        i+=dilimLen;

  //  if(j!=srcLen) {memcpy(res+i,src+j-step,srcLen-(j-step));i+=srcLen-(j-step);}
       if((flag&dlRight)!=0) {memcpy(res+i,dilim,dilimLen);i+=dilimLen;}
   if(resLen!=NULL) *resLen=i;
   return res;
 }
//--------------------------------------------------------------------------------------------------------
char* RemoveDilimeters(char* src, int srcLen,char* dilim, int dilimLen,int* resLen)
 {
   byte* res;
   int i;
   int DilimetrsCount=0;
   int* DilimetrsPos=new int[srcLen/dilimLen+1];
   i=0;
   while(i<=srcLen-dilimLen)
    {
     if(memcmp(src+i,dilim,dilimLen)==0)
      {DilimetrsPos[DilimetrsCount]=i;DilimetrsCount++;i+=dilimLen;}
     else
      i++;
    }
    *resLen=0;
    res=new byte[srcLen-DilimetrsCount*dilimLen];
   if(DilimetrsCount>0)
    {
      if(DilimetrsPos[0]!=0)
        { memcpy(res,src,DilimetrsPos[0]);
          *resLen+=DilimetrsPos[0];
        }
     for(i=1;i<DilimetrsCount;i++)
      {
       memcpy(res+*resLen,src+DilimetrsPos[i-1]+dilimLen,DilimetrsPos[i]-DilimetrsPos[i-1]-dilimLen);
       *resLen+=DilimetrsPos[i]-DilimetrsPos[i-1]-dilimLen;
      }
     if(DilimetrsCount>1)
        { memcpy(res+*resLen,src+DilimetrsPos[DilimetrsCount-1]+dilimLen,srcLen-DilimetrsPos[DilimetrsCount-1]-dilimLen);
          *resLen+=srcLen-DilimetrsPos[DilimetrsCount-1]-dilimLen;
        }
       else
              if(DilimetrsPos[0]==0)
           { memcpy(res,src+dilimLen,dilimLen);
             *resLen=dilimLen;
           }
    }
   else
    {
     memcpy(res,src,*resLen=srcLen);
    }
   delete DilimetrsPos;
   return res;
 }
//--------------------------------------------------------------------------
unsigned long __fastcall byteptrToulong(byte* buf,int len)
 {
  WORK_ANSILIB_UNION_FOR_CONVERT work;
  work._ulong=0;
  if(len>4)
     memcpy(&work._ulong,buf,4);
  else
    memcpy(&work._ulong,buf,len);
  return work._ulong;
 }
 #pragma package(smart_init)

