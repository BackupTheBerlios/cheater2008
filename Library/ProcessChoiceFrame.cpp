//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProcessChoiceFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TProcessChoice *ProcessChoice;



BOOL CALLBACK EnumResNameProc(

    HANDLE hModule,	// resource-module handle
    LPCTSTR lpszType,	// pointer to resource type 
    LPTSTR lpszName,	// pointer to resource name
    LONG lParam 	// application-defined parameter
   )
{
TProcessChoice1* ptr=(TProcessChoice1*)lParam;
//ptr->AdditionalInfo
ptr->AdditionalInfo->Items->AddChild(ptr->AdditionalInfo->Items->operator[](1),AnsiString(lpszName));
return TRUE;
}
//---------------------------------------------------------------------------
__fastcall TProcessChoice1::TProcessChoice1(TComponent* Owner)
        : TFrame(Owner)
{
}                     
//---------------------------------------------------------------------------

void __fastcall TProcessChoice1::GetProcesses(void)
{
MODULEINFO modinfo;
   TTreeNode* ProcessId;
  DWORD cb=200;            // size of array
  DWORD lpidProcess[200];  // array of process identifiers
  DWORD cbNeeded;      // number of bytes returned
ProcessTree->Items->Clear();
HANDLE handle;
HANDLE  lphModule[200];
  DWORD cbNeededModule;
if( EnumProcesses(
  lpidProcess,  // array of process identifiers
   cb*sizeof(DWORD),            // size of array
  &cbNeeded      // number of bytes returned
)==TRUE)
 for(DWORD i=0;i<cbNeeded/sizeof(DWORD);i++)
  {
    HANDLE handle=OpenProcess(
    PROCESS_ALL_ACCESS	,	// access flag
    FALSE,	// handle inheritance flag
    lpidProcess[i] 	// process identifier
   );
 if(EnumProcessModules(
              handle,      // handle to process
              lphModule,   // array of module handles
               cb*sizeof(HANDLE),             // size of array
              &cbNeededModule    // number of bytes required
                    )==TRUE)
   {

   ProcessId=ProcessTree->Items->Add(NULL,AnsiString("Process id ")+AnsiString(lpidProcess[i]));
    for(DWORD j=0;j<cbNeededModule/sizeof(HANDLE);j++)
      {
        char FileName[1024];
        int realRead=GetModuleFileNameEx(
            handle,    // handle to process
            lphModule[j],    // handle to module
            FileName,  // path buffer
            90         // maximum characters to retrieve
            );
       TTreeNode* Child =ProcessTree->Items->AddChild(ProcessId,AnsiString(":")+ulongToHexAnsi((unsigned long)lphModule[j])+AnsiString(" ")+AnsiString(FileName,realRead));
          realRead=GetModuleBaseName(
                handle,    // handle to process
                 lphModule[j],    // handle to module
                FileName,  // base name buffer
                   200         // maximum characters to retrieve
                         );
              ProcessTree->Items->AddChild(Child,AnsiString("BaseName ")+AnsiString(FileName,realRead));
           if( GetModuleInformation(
                     handle,         // handle to process
                     lphModule[j],         // handle to module
                     &modinfo,  // information buffer
                     sizeof(modinfo)                 // size of buffer
                                )==TRUE)
                  {
                   ProcessTree->Items->AddChild(Child,AnsiString("BaseOfDll       :")+ulongToHexAnsi((unsigned long)modinfo.lpBaseOfDll,8));
                   ProcessTree->Items->AddChild(Child,AnsiString("Size of Image    ")+ulongToHexAnsi(modinfo.SizeOfImage,8));
                   ProcessTree->Items->AddChild(Child,AnsiString("Entry Point     :")+ulongToHexAnsi((unsigned long)modinfo.EntryPoint,8));
                  }

      }

   }
  }

}
void __fastcall TProcessChoice1::ProcessTreeEnter(TObject *Sender)
{
GetProcesses();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TProcessChoice1::ProcessTreeDblClick(TObject *Sender)
{
  AdditionalInfo->Items->Clear();
if(ProcessTree->Selected!=NULL)
 {
 if(ProcessTree->Selected->Text.SubString(1,10)==AnsiString("Process id"))
{
  int id=ProcessTree->Selected->Text.SubString(11,ProcessTree->Selected->Text.Length()-11).ToInt();
HANDLE handle=OpenProcess(
    PROCESS_ALL_ACCESS	,	// access flag
    FALSE,	// handle inheritance flag
    id 	// process identifier
   );
//   Get
}
else
if  (ProcessTree->Selected->Text.SubString(1,1)==AnsiString(":"))
 {
   TTreeNode* WorkNode;
   AnsiString Type,Path;
   int i;
  HINSTANCE hModule;
   for(i=2;i<ProcessTree->Selected->Text.Length();i++)
    {
     if(ProcessTree->Selected->Text[i]==' ') break;
    }
//hModule=HexAnsiToulong(ProcessTree->Selected->Text.SubString(2,i-2));
Path=ProcessTree->Selected->Text.SubString(i+1,ProcessTree->Selected->Text.Length()-i);
AdditionalInfo->Items->AddFirst(NULL,Path);
hModule=LoadLibraryEx(

    Path.c_str(),	// points to name of executable module
    NULL,	// reserved, must be NULL
    LOAD_LIBRARY_AS_DATAFILE 	// entry-point execution flag 
   );

Type=AnsiString("RT_ACCELERATOR");//	Accelerator table
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    hModule,	// resource-module handling
    RT_ACCELERATOR,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );



Type=AnsiString("RT_ANICURSOR");//	Animated cursor
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    hModule,	// resource-module handling
    RT_ANICURSOR,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_ANIICON");//	Animated icon
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    hModule,	// resource-module handling
    RT_ANIICON,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_BITMAP");//	Bitmap resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    hModule,	// resource-module handling
    RT_BITMAP,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_CURSOR");//	Hardware-dependent cursor resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    hModule,	// resource-module handling
    RT_CURSOR,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_DIALOG");//	Dialog box
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    hModule,	// resource-module handling
    RT_DIALOG,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_FONT");//	Font resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_FONT,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_FONTDIR");//	Font directory resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_FONTDIR,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_GROUP_CURSOR");//	Hardware-independent cursor resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_GROUP_CURSOR,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_GROUP_ICON");//	Hardware-independent icon resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_GROUP_ICON,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_ICON");//	Hardware-dependent icon resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_ICON,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_MENU");//	Menu resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_MENU,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_MESSAGETABLE");//	Message-table entry
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_MESSAGETABLE,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_RCDATA");//	Application-defined resource (raw data)
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_RCDATA,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_STRING") ;//	String-table entry
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_STRING,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );


Type=AnsiString("RT_VERSION");//	Version resource
WorkNode=AdditionalInfo->Items->AddFirst(NULL,Type);
EnumResourceNames(

    (void*)hModule,	// resource-module handling
    RT_VERSION,	// pointer to resource type
    (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
    (LONG) ((void*)this) 	// application-defined parameter
   );
 }
 }
}
//---------------------------------------------------------------------------

