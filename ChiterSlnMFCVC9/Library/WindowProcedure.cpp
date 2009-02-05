#include "stdafx.h"

#include "Command.h"
#include "WindowProcedure.h"


CWnd& getCWnd(HWND hwnd)
{
  CWnd* pWnd = CWnd::FromHandle(hwnd);
  if(!pWnd)
  {
    throw std::runtime_error("Failed to retrieve CWnd");
  }
  return *pWnd;
}

WNDPROC_TYPE replaceWndProc(CWnd& wnd,WNDPROC_TYPE newProc)
{
   LONG ret = ::SetWindowLong(wnd.GetSafeHwnd(), GWL_WNDPROC, (LONG) newProc);
  return (WNDPROC_TYPE)ret;
}