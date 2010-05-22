#ifndef LIBRARY_ExceptionReporter_h
#define LIBRARY_ExceptionReporter_h
#include "august2002Fwd.h"
#include <string>

enum TerminationStatus
{
  //! application succeeded
  TS_OK = 0,

  TS_Failed = 1,
  TS_WinException = 2,

  TS_Unknown = 0xffff,
};

AUGUST2002_EXPORT LONG  WINAPI reportWinExcp(LPEXCEPTION_POINTERS i_ep);
AUGUST2002_EXPORT int  processExceptions( const std::string& context );

#endif