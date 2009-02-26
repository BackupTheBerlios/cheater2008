//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "ProcessManager.h"
#pragma package(smart_init)
#include <boost/filesystem/path.hpp>

ProcessStreamBuf::ProcessStreamBuf()
{
GetSystemInfo((LPSYSTEM_INFO)&SystemInfo);
}
ProcessStreamBuf::~ProcessStreamBuf()
{
    // do nothing
}

void ProcessStreamBuf::imbue ( const std::locale & loc )
{
    std::streambuf::imbue(loc);
}
std::streambuf * ProcessStreamBuf::setbuf ( char * s, std::streamsize n )
{
    return std::streambuf::setbuf(s,n);
}
std::streampos ProcessStreamBuf::seekoff ( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which/* = std::ios_base::in | std::ios_base::out */)
{
switch(way)
{
case   std::ios_base::beg:
    if(off>=0 && FSize > off)
        FPosition=off;
break;
case   std::ios_base::cur:
    if( (off<0 && FPosition > abs(off)) )
        FPosition+=off;
    else if (off>0 && FSize-FPosition > off)
        FPosition+=off;
break;
case   std::ios_base::end:
    if(off>=0 && FSize > off)
        FPosition=FStart+FSize-off;
break;
}
return  FPosition;
//    return std::streambuf::seekoff(off,way,which);
}
std::streampos ProcessStreamBuf::seekpos ( std::streampos sp, std::ios_base::openmode which /*= ios_base::in | ios_base::out */)
{
    if(sp>0 && sp< FSize)
        FPosition=sp;
    return  FPosition;        
    //return std::streambuf::seekpos(sp,which);
}
int ProcessStreamBuf::sync ( )

{
    return std::streambuf::sync();
}
std::streamsize ProcessStreamBuf::showmanyc ( )

{
    return std::streambuf::showmanyc();
}
std::streamsize ProcessStreamBuf::xsgetn ( char * s, std::streamsize n )

{
    return rawRead(s, n);
//    return std::streambuf::xsgetn(s,n);
}
int ProcessStreamBuf::underflow ( )

{
    return std::streambuf::underflow();
}
int ProcessStreamBuf::uflow ( )

{
    return std::streambuf::uflow();
}
int ProcessStreamBuf::pbackfail ( int c /*= EOF */)

{
    return std::streambuf::pbackfail(c);
}
std::streamsize ProcessStreamBuf::xsputn ( const char * s, std::streamsize n )

{
    return rawWrite(s, n);
//    return std::streambuf::xsputn(s,n);
}
int ProcessStreamBuf::overflow ( int c /*= EOF */)

{
    return std::streambuf::overflow(c);
}

bool  ProcessStreamBuf::createProcess(const std::string& i_process)
{
    AnsiString CommandLine;	//  command line string
    SECURITY_ATTRIBUTES ProcessAttributes;	//  process security attributes
    SECURITY_ATTRIBUTES ThreadAttributes;	// security attributes
    BOOL bInheritHandles;	// handle inheritance flag
    DWORD dwCreationFlags;	// creation flags
    LPVOID lpEnvironment;	// pointer to new environment block
    STARTUPINFO StartupInfo;	// pointer to STARTUPINFO

CommandLine="";
bInheritHandles=TRUE;
dwCreationFlags=CREATE_NEW_CONSOLE|NORMAL_PRIORITY_CLASS;
lpEnvironment=NULL;
ProcessAttributes.nLength=sizeof(SECURITY_ATTRIBUTES);
ProcessAttributes.bInheritHandle=TRUE;
ProcessAttributes.lpSecurityDescriptor=NULL;
StartupInfo.cb=sizeof(STARTUPINFO);
StartupInfo.lpReserved=NULL;
StartupInfo.lpDesktop=NULL;
StartupInfo.lpTitle=NULL;
StartupInfo.dwFlags=STARTF_USESHOWWINDOW;
StartupInfo.wShowWindow=SW_SHOW;
StartupInfo.cbReserved2=0;
StartupInfo.lpReserved2=NULL;

ThreadAttributes.nLength=sizeof(SECURITY_ATTRIBUTES);
ThreadAttributes.bInheritHandle=TRUE;
ThreadAttributes.lpSecurityDescriptor=NULL;
std::string CurrentDirectory= boost::filesystem::path(i_process).parent_path().string();
   FPosition=0;
   FStart=0x01000000;
   FSize=0x7fff0000;
  return ::CreateProcess(
    i_process.c_str(),	// pointer to name of executable module
    CommandLine.c_str(),	// pointer to command line string
    &ProcessAttributes,	// pointer to process security attributes
    &ThreadAttributes,	// pointer to thread security attributes
    bInheritHandles,	// handle inheritance flag
    dwCreationFlags,	// creation flags
    lpEnvironment,	// pointer to new environment block
    CurrentDirectory.c_str(),	// pointer to current directory name
    &StartupInfo,	// pointer to STARTUPINFO
    &ProcessInformation 	// pointer to PROCESS_INFORMATION
   )==TRUE;

}

void  ProcessStreamBuf::openProcess(DWORD id)
{
ProcessInformation.dwProcessId=id;
 ProcessInformation.hProcess=::OpenProcess(
    PROCESS_ALL_ACCESS	,	// access flag
    FALSE,	// handle inheritance flag
    id 	// process identifier
   );
   FPosition=0;
   FStart=0x01000000;
   FSize=0x7FFF0000;
}

void  ProcessStreamBuf::protect(DWORD pos, DWORD size,DWORD flag/*=PAGE_READWRITE*/)
{
DWORD OldProtection;
DWORD end=pos+size+FStart;
if((pos>=0)&&(size>0))
  for(DWORD i=FStart+pos;i<end;i+=SystemInfo.dwPageSize)
     VirtualProtectEx(
      ProcessInformation.hProcess,// handle of process
      (void*)i,	// address of region of committed pages
      SystemInfo.dwPageSize,	// size of region
      flag,	// desired access protection
      &OldProtection 	// address of variable to get old protection
      );
}

MEMORY_BASIC_INFORMATION  ProcessStreamBuf::getMemoryInfo(DWORD Pos)
{
MEMORY_BASIC_INFORMATION MemoryInfo;
VirtualQueryEx(ProcessInformation.hProcess,
         (LPCVOID) (Pos+FStart),	// address of region
         &MemoryInfo,	// address of information buffer
         sizeof(MemoryInfo)       //size of buffer
        );
return MemoryInfo;
}

void  ProcessStreamBuf::setMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo)
{
DWORD work;
 VirtualProtectEx(
 ProcessInformation.hProcess,	// handle of process
    MemInfo.BaseAddress,	// address of region of committed pages
    MemInfo.RegionSize,	// size of region
    MemInfo.Protect,	// desired access protection
    &work 	// address of variable to get old protection
   );

}

int  ProcessStreamBuf::seek(int Offset, Word Origin)
{
 if(Origin==soFromEnd) FPosition=FSize-(DWORD)Offset;
  else
    if(Origin==soFromCurrent) FPosition=(DWORD)Offset+FPosition;
     else
      FPosition=(DWORD)Offset;
 if(FPosition<0) FPosition=0;
  else
 if(FPosition>FSize) FPosition=FSize;
 return (int)FPosition;

}
void ProcessStreamBuf::setStart(DWORD value)
{
    FStart=value;
}

DWORD ProcessStreamBuf::getStart()
{
    return FStart;
}

void ProcessStreamBuf::setSize(DWORD NewSize)
{
if (NewSize>0)
FSize=NewSize;
 if(FPosition>NewSize)
 FPosition=NewSize;
if(FSize>0)
 protect(0,FSize);
}

DWORD ProcessStreamBuf::getSize()
{
    return FSize;
}

int  ProcessStreamBuf::rawWrite(const void * Buffer, int Count)
{
MEMORY_BASIC_INFORMATION MemoryInfo;
int RealWrite=0;
int NeedWrite;
if((Count>0)&&(Buffer!=NULL))
 {
  if((DWORD)Count+FPosition>=FSize) NeedWrite=FSize-FPosition;
   else
  NeedWrite=(DWORD)Count;

if( WriteProcessMemory(
    ProcessInformation.hProcess,// handle to process whose memory is written to
    (void*)(FPosition+FStart),	// address to start writing to
    (void*)Buffer,	// pointer to buffer to write data to
    NeedWrite,	// number of bytes to write
    (DWORD*)&RealWrite 	// actual number of bytes written
   )!=TRUE)
 {      VirtualQueryEx(ProcessInformation.hProcess,
         (LPCVOID) ((unsigned int)FPosition+FStart),	// address of region
         &MemoryInfo,	// address of information buffer
         sizeof(MemoryInfo)       //size of buffer
        );
DWORD OldProtection;
   VirtualProtectEx(
    ProcessInformation.hProcess,// handle of process
    MemoryInfo.BaseAddress,	// address of region of committed pages
    MemoryInfo.RegionSize,	// size of region
    PAGE_READWRITE,	// desired access protection
    &OldProtection 	// address of variable to get old protection
   );
if( WriteProcessMemory(
    ProcessInformation.hProcess,// handle to process whose memory is written to
    (void*)(FPosition+FStart),	// address to start writing to
    (void*)Buffer,	// pointer to buffer to write data to
    NeedWrite,	// number of bytes to write
    (unsigned long*)&RealWrite 	// actual number of bytes written
   )!=TRUE)
    {
     RealWrite=0;
        FPosition=(DWORD)MemoryInfo.BaseAddress+MemoryInfo.RegionSize-FStart;
    }
   else
    FPosition+=RealWrite;
 }
else
  FPosition+=RealWrite;
 }
return RealWrite;
}

int  ProcessStreamBuf::rawRead(void * Buffer, int Count)
{
if( FPosition>=FSize )
    return 0;
MEMORY_BASIC_INFORMATION MemoryInfo;

DWORD RealRead=0;
DWORD NeedRead;
DWORD newPos;
if((Count>0)&&(Buffer!=NULL))
 {
  if((DWORD)Count+FPosition>=FSize) NeedRead=FSize-FPosition;
   else
  NeedRead=(DWORD)Count;
if( ReadProcessMemory(
    ProcessInformation.hProcess,// handle to process whose memory is read
    (void*)(FPosition+FStart),	// address to start reading
    Buffer,	// pointer to buffer to read
    NeedRead,	// number of bytes to read
    (DWORD*)&RealRead 	// actual number of bytes read
   )!=TRUE)
 {
    VirtualQueryEx(ProcessInformation.hProcess,
         (LPCVOID) (FPosition+FStart),	// address of region
         &MemoryInfo,	// address of information buffer
         sizeof(MemoryInfo)       //size of buffer
        );

    newPos=(DWORD)MemoryInfo.BaseAddress+MemoryInfo.RegionSize-FStart;

    if(newPos<=FPosition)
     FPosition+=SystemInfo.dwPageSize;
    else
     FPosition=newPos;
    RealRead+=rawRead((char*)Buffer+RealRead,Count-RealRead);
 }
else
 FPosition+=RealRead;
 }
return RealRead;
}

__fastcall TProcessManager::TProcessManager(void) :
std::iostream(0)

{
    this->rdbuf(&d_processMemoryStreamBuffer);
}

TProcessManager::~TProcessManager()
{

}

std::iostream& TProcessManager::getStreamInterface()
{
    return (*this);
}

//-------------------------------------------------------
int __fastcall TProcessManager::Seek(int Offset, Word Origin)
{
return d_processMemoryStreamBuffer.seek(Offset,Origin);

}

void  TProcessManager::setStart(DWORD value)
{
d_processMemoryStreamBuffer.setStart(value);
}

DWORD   TProcessManager::getStart()
{
return d_processMemoryStreamBuffer.getStart();
}

void  TProcessManager::setSize(DWORD NewSize)
{
d_processMemoryStreamBuffer.setSize(NewSize);
}


DWORD  TProcessManager::getSize()
{
return  d_processMemoryStreamBuffer.getSize();
}

int __fastcall TProcessManager::Write(const void * Buffer, int Count)
{
int pos1 =this->tellp();
this->write(static_cast<const char*>(Buffer),Count);
int pos2 =this->tellp();
return (pos2-pos1);
}

//-----------------------------------------------------------------
int __fastcall TProcessManager::Read(void * Buffer, int Count)
{

int pos1 =this->tellp();
this->read(static_cast<char*>(Buffer),Count);
// if failed to read a desired number of bytes
// stream has a fail state. But we my still continue reading
int pos2 =this->tellp();
int realRead=0;
if(getStreamInterface())
    realRead=(pos2-pos1);
this->clear();
return realRead;

}
void  TProcessManager::createProcess(const std::string& i_process)
{
   d_processMemoryStreamBuffer.createProcess(i_process);
}

void  TProcessManager::openProcess(DWORD id)
{
   d_processMemoryStreamBuffer.openProcess(id);
}
void  TProcessManager::setMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo)
{
   d_processMemoryStreamBuffer.setMemoryInfo(MemInfo);
}
MEMORY_BASIC_INFORMATION  TProcessManager::getMemoryInfo(DWORD pos)
{
   return d_processMemoryStreamBuffer.getMemoryInfo(pos);
}
/*
//--------------------------------------------------------------
void __fastcall TProcessManager::Assign(TProcessManager* value)
{

FPosition=value->FPosition;
FSize=value->FSize;
FStart=value->FStart;

SystemInfo=value->SystemInfo;

}
//----------------------------------------------------------------------------
  */
