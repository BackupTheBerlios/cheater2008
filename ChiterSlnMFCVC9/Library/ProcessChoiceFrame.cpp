//---------------------------------------------------------------------------
#include "stdafx.h"
#include "ProcessChoiceFrame.h"
#include "Standart/Ansilib.h"
#include <boost/lexical_cast.hpp>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
//---------------------------------------------------------------------------

enum
{
  ProcessViewSplitter_ID = 0xE000,
  ProcessTree_ID,
  AdditionalInfo_ID

};


BOOL CALLBACK EnumResNameProc(
                              HANDLE hModule,	// resource-module handle
                              LPCTSTR lpszType,	// pointer to resource type
                              LPTSTR lpszName,	// pointer to resource name
                              LONG lParam 	// application-defined parameter
                              )
{
  TProcessChoice1* ptr=(TProcessChoice1*)lParam;
  //ptr->AdditionalInfo
  HTREEITEM root = ptr->AdditionalInfo.GetRootItem( );
  ptr->AdditionalInfo.InsertItem( lpszName, root);
  return TRUE;
}

IMPLEMENT_DYNAMIC(TProcessChoice1, CMyBaseForm)

BEGIN_MESSAGE_MAP(TProcessChoice1, CMyBaseForm)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
TProcessChoice1::TProcessChoice1(CWnd* pParent /*= NULL*/)
: CMyBaseForm(pParent)
{
}

void  TProcessChoice1::initialize(void)
{
  CRect clientRect;
  this->GetClientRect(&clientRect);

  Splitter1.Create(this, WS_CHILD|WS_VISIBLE, clientRect, ProcessViewSplitter_ID);

  ProcessTree.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),&Splitter1,ProcessTree_ID);
  AdditionalInfo.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),&Splitter1,AdditionalInfo_ID);

  Splitter1.AddRow();
  Splitter1.AddColumn();
  Splitter1.AddColumn();

  Splitter1.SetWindow(0,0,ProcessTree.m_hWnd);
  Splitter1.SetWindow(0,1,AdditionalInfo.m_hWnd);


  Splitter1.ShowWindow( SW_SHOW );
  ProcessTree.ShowWindow( SW_SHOW );
  AdditionalInfo.ShowWindow( SW_SHOW );

  Splitter1.Update();
  ProcessTree.UpdateWindow();
  AdditionalInfo.UpdateWindow();

}
//---------------------------------------------------------------------------

void TProcessChoice1::GetProcesses(void)
{
  MODULEINFO modinfo;
  HTREEITEM ProcessId;
  const DWORD cb=1024;            // size of array
  DWORD lpidProcess[cb * sizeof(DWORD)];  // array of process identifiers
  HMODULE  lphModule[cb * sizeof(HMODULE)];
  DWORD cbNeeded;      // number of bytes returned
  HANDLE handle;
  DWORD cbNeededModule;

  ProcessTree.DeleteAllItems();



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
        std::string procName = std::string("Process id ")+boost::lexical_cast<std::string>(lpidProcess[i]);
        ProcessId=ProcessTree.InsertItem(CString(procName.c_str()));
        for(DWORD j=0;j<cbNeededModule/sizeof(HANDLE);j++)
        {
          TCHAR FileName[1024];
          int realRead=GetModuleFileNameEx(
            handle,    // handle to process
            (HMODULE)lphModule[j],    // handle to module
            FileName,  // path buffer
            90         // maximum characters to retrieve
            );
          std::string childName = std::string(":")+ulongToHexAnsi((unsigned long)lphModule[j]).c_str()+std::string(" ")+std::string( CT2CA(CString(FileName,realRead)) );
          HTREEITEM Child =ProcessTree.InsertItem(CString(childName.c_str()),ProcessId);
          realRead=GetModuleBaseName(
            handle,    // handle to process
            lphModule[j],    // handle to module
            FileName,  // base name buffer
            200         // maximum characters to retrieve
            );
          std::string subChildName = std::string("BaseName ")+std::string( CT2CA(CString(FileName,realRead)) );
          ProcessTree.InsertItem(CString(subChildName.c_str()),Child);
          if( GetModuleInformation(
            handle,         // handle to process
            lphModule[j],         // handle to module
            &modinfo,  // information buffer
            sizeof(modinfo)                 // size of buffer
            )==TRUE)
          {
            std::string locName;
            locName = std::string("BaseOfDll       :")+ulongToHexAnsi((unsigned long)modinfo.lpBaseOfDll,8);
            ProcessTree.InsertItem(CString(locName.c_str()),Child);
            locName = std::string("Size of Image    ")+ulongToHexAnsi(modinfo.SizeOfImage,8);
            ProcessTree.InsertItem(CString(locName.c_str()),Child);
            locName = std::string("Entry Point     :")+ulongToHexAnsi((unsigned long)modinfo.EntryPoint,8);
            ProcessTree.InsertItem(CString(locName.c_str()),Child);
          }

        }

      }
    }
}
void TProcessChoice1::ProcessTreeEnter(TObject *Sender)
{
  GetProcesses();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void  TProcessChoice1::ProcessTreeDblClick(TObject *Sender)
{
  AdditionalInfo.DeleteAllItems();
  HTREEITEM procSelItem = ProcessTree.GetSelectedItem();
  if(  procSelItem !=NULL)
  {
    std::string selectedProcText = CT2CA ( ProcessTree.GetItemText( procSelItem ) );

    if(selectedProcText.substr(0,9)==std::string("Process id"))
    {
      int id= boost::lexical_cast<int> ( selectedProcText.substr(10,selectedProcText.size()-10) );
      HANDLE handle=OpenProcess(
        PROCESS_ALL_ACCESS	,	// access flag
        FALSE,	// handle inheritance flag
        id 	// process identifier
        );
      //   Get
    }
    else
      if  (selectedProcText.substr(0,0)==std::string(":"))
      {
        HTREEITEM WorkNode;
        std::string Type,Path;
        int i;
        HINSTANCE hModule;
        for(i=1;i<selectedProcText.size();i++)
        {
          if(selectedProcText.at(i)==' ') break;
        }
        //hModule=HexAnsiToulong(ProcessTree->Selected->Text.SubString(2,i-2));
        Path=selectedProcText.substr(i,selectedProcText.size()-i);
        AdditionalInfo.InsertItem( CString (Path.c_str()) );
        hModule=LoadLibraryEx(

          CString(Path.c_str()),	// points to name of executable module
          NULL,	// reserved, must be NULL
          LOAD_LIBRARY_AS_DATAFILE 	// entry-point execution flag
          );

        Type=std::string("RT_ACCELERATOR");//	Accelerator table
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(

          (HMODULE)hModule,	// resource-module handling
          RT_ACCELERATOR,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );

        Type=std::string("RT_ANICURSOR");//	Animated cursor
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_ANICURSOR,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_ANIICON");//	Animated icon
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_ANIICON,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_BITMAP");//	Bitmap resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_BITMAP,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );

        Type=std::string("RT_CURSOR");//	Hardware-dependent cursor resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_CURSOR,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_DIALOG");//	Dialog box
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_DIALOG,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_FONT");//	Font resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_FONT,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );

        Type=std::string("RT_FONTDIR");//	Font directory resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_FONTDIR,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_GROUP_CURSOR");//	Hardware-independent cursor resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_GROUP_CURSOR,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_GROUP_ICON");//	Hardware-independent icon resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_GROUP_ICON,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );

        Type=std::string("RT_ICON");//	Hardware-dependent icon resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_ICON,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_MENU");//	Menu resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_MENU,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_MESSAGETABLE");//	Message-table entry
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_MESSAGETABLE,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_RCDATA");//	Application-defined resource (raw data)
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_RCDATA,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_STRING") ;//	String-table entry
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_STRING,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
        Type=std::string("RT_VERSION");//	Version resource
        WorkNode=AdditionalInfo.InsertItem(CString ( Type.c_str() ));
        EnumResourceNames(
          (HMODULE)hModule,	// resource-module handling
          RT_VERSION,	// pointer to resource type
          (ENUMRESNAMEPROC)EnumResNameProc,	// pointer to callback function
          (LONG) ((void*)this) 	// application-defined parameter
          );
      }
  }
}

BOOL TProcessChoice1::OnInitDialog()
{
  CMyBaseForm::OnInitDialog();
  initialize();
  return TRUE;
}
//---------------------------------------------------------------------------


