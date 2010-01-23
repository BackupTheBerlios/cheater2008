//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include "dbg.h"

void dprintf(const std::string& i_str)
{
  OutputDebugString(CString(i_str.c_str()) );
}

