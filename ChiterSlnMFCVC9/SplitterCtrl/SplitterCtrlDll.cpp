#include "stdafx.h"

HINSTANCE hInstance = 0;

/*
Force linker to choose own DLLMain implementation instead of default MFC DLLMain from dllmain.cpp
*/
#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif


BOOL WINAPI DllMain (HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    hInstance = hinstDLL;

  return TRUE;
}