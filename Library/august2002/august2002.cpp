//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("EditForm.cpp", EditMemoryForm);
USEFORM("InputDlg.cpp", InputForm);
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
