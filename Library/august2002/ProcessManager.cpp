//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "ProcessManager.h"
#pragma package(smart_init)
__fastcall TProcessManager::TProcessManager(void) :TStream()
{


GetSystemInfo((LPSYSTEM_INFO)&SystemInfo);
}
//-------------------------------------------------------
int __fastcall TProcessManager::Seek(int Offset, Word Origin)
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

void __fastcall TProcessManager::SetStart(DWORD value)
{
FStart=value;
}

void __fastcall TProcessManager::SetSize(DWORD NewSize)
{
if (NewSize>0)
 FSize=NewSize;
 if(FPosition>NewSize)
  FPosition=NewSize;
if(FSize>0)
  Protect(0,FSize);
}

int __fastcall TProcessManager::Write(const void * Buffer, int Count)
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
        Position=(DWORD)MemoryInfo.BaseAddress+MemoryInfo.RegionSize-FStart;
    }
   else
    FPosition+=RealWrite;
 }
else
  FPosition+=RealWrite;

 }
return RealWrite;
}
//-----------------------------------------------------------------
int __fastcall TProcessManager::Read(void * Buffer, int Count)
{
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
    RealRead=0;
    VirtualQueryEx(ProcessInformation.hProcess,
         (LPCVOID) (FPosition+FStart),	// address of region
         &MemoryInfo,	// address of information buffer
         sizeof(MemoryInfo)       //size of buffer
        );

    newPos=(DWORD)MemoryInfo.BaseAddress+MemoryInfo.RegionSize-FStart;
    if(newPos<=FPosition)
     Position+=SystemInfo.dwPageSize;
    else
     Position=newPos;
 }
else
 FPosition+=RealRead;
 }
return RealRead;
}

bool __fastcall TProcessManager::CreateProcess(AnsiString FileName)
{
    AnsiString ApplicationName;	//  name of executable module
    AnsiString CommandLine;	//  command line string
    SECURITY_ATTRIBUTES ProcessAttributes;	//  process security attributes
    SECURITY_ATTRIBUTES ThreadAttributes;	// security attributes
    BOOL bInheritHandles;	// handle inheritance flag
    DWORD dwCreationFlags;	// creation flags
    LPVOID lpEnvironment;	// pointer to new environment block
    AnsiString CurrentDirectory;	// pointer to current directory name
    STARTUPINFO StartupInfo;	// pointer to STARTUPINFO

CommandLine="";
bInheritHandles=TRUE;
CurrentDirectory="";
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
ApplicationName=AnsiString(FileName);
CurrentDirectory=ApplicationName.SubString(1,ApplicationName.LastDelimiter("\\"));
   FPosition=0;
   FStart=0x01000000;
   FSize=0x7fff0000;
  return ::CreateProcess(
    ApplicationName.c_str(),	// pointer to name of executable module
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


void __fastcall TProcessManager::OpenProcess(DWORD id)
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

void __fastcall TProcessManager::Protect(DWORD pos, DWORD size,DWORD flag)
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
//--------------------------------------------------------------
void __fastcall TProcessManager::Assign(TProcessManager* value)
{

FPosition=value->FPosition;
FSize=value->FSize;
FStart=value->FStart;

SystemInfo=value->SystemInfo;

}
//----------------------------------------------------------------------------


MEMORY_BASIC_INFORMATION __fastcall TProcessManager::GetMemoryInfo(DWORD Pos)
{
MEMORY_BASIC_INFORMATION MemoryInfo;
VirtualQueryEx(ProcessInformation.hProcess,
         (LPCVOID) (Pos+FStart),	// address of region
         &MemoryInfo,	// address of information buffer
         sizeof(MemoryInfo)       //size of buffer
        );
return MemoryInfo;
}
void __fastcall TProcessManager::SetMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo)
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
