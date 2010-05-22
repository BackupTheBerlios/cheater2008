#include "stdafx.h"

#include "ExceptionReporter.h"
#include "MessageReporterBase.h"

#include "imagehlp.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

//!
//! 3 functions below manage windows exceptions, (see AlignApp.cpp)
//! * getWinExcpStr
//! * reportErrorAndDump
//! * reportWinExcp

//! copy-paste from AlignApp.cpp
static const char *getWinExcpStr(LPEXCEPTION_POINTERS i_ep) throw()
{
  if (i_ep == 0 || i_ep->ExceptionRecord == 0)
    return "Bad EXCEPTION_POINTERS structure.\n";
  switch (i_ep->ExceptionRecord->ExceptionCode) 
  {
#define CASE_CODE( code, hint ) case code: return #code "\n" hint "\n";
    CASE_CODE( EXCEPTION_ACCESS_VIOLATION, 
      "The thread attempted to read from or write to a virtual address for which it does not have the appropriate access." );
    CASE_CODE( EXCEPTION_BREAKPOINT, 
      "A breakpoint was encountered" );
    CASE_CODE( EXCEPTION_DATATYPE_MISALIGNMENT, 
      "The thread attempted to read or write data that is misaligned on hardware\n"
      "that does not provide alignment. For example, 16-bit values must be aligned\n"
      "on 2-byte boundaries, 32-bit values on 4-byte boundaries, and so on." );
    CASE_CODE( EXCEPTION_SINGLE_STEP, 
      "A trace trap or other single-instruction mechanism signaled that one instruction has been executed." );
    CASE_CODE( EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 
      "The thread attempted to access an array element that is out of bounds,\n"
      "and the underlying hardware supports bounds checking." );
    CASE_CODE( EXCEPTION_FLT_DENORMAL_OPERAND, 
      "One of the operands in a floating-point operation is denormal.\n"
      "A denormal value is one that is too small to represent as a standard floating-point value." );
    CASE_CODE( EXCEPTION_FLT_DIVIDE_BY_ZERO, 
      "The thread attempted to divide a floating-point value by a floating-point divisor of zero." );
    CASE_CODE( EXCEPTION_FLT_INEXACT_RESULT, 
      "The result of a floating-point operation cannot be represented exactly as a decimal fraction." );
    CASE_CODE( EXCEPTION_FLT_INVALID_OPERATION, 
      "This exception represents any floating-point exception not included in this list." );
    CASE_CODE( EXCEPTION_FLT_OVERFLOW, 
      "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type." );
    CASE_CODE( EXCEPTION_FLT_STACK_CHECK, 
      "The stack overflowed or underflowed as the result of a floating-point operation." );
    CASE_CODE( EXCEPTION_FLT_UNDERFLOW,
      "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type." );
    CASE_CODE( EXCEPTION_INT_DIVIDE_BY_ZERO, 
      "The thread attempted to divide an integer value by an integer divisor of zero." );
    CASE_CODE( EXCEPTION_INT_OVERFLOW, 
      "The result of an integer operation caused a carry out of the most significant bit of the result." );
    CASE_CODE( EXCEPTION_PRIV_INSTRUCTION, 
      "The thread attempted to execute an instruction whose operation is not allowed in the current machine mode." );
    CASE_CODE( EXCEPTION_NONCONTINUABLE_EXCEPTION, 
      "The thread attempted to continue execution after a noncontinuable exception occurred." );
#undef CASE_CODE
    default:
      return "unknown exception code";
  }
}

//! enhanced copy-paste from AlignApp.cpp
//! the "Report" part removed
//!
//! Crash dump support:
//!  1. ask the user for dump file path
//!  2. write the dump
//! The function works inside Windows exception handler in very unstable
//! environment. Keep it as simple as possible. In particular, use only
//! simplest GUI (like standard Message Box and File Save dialog).
//!
static void reportErrorAndDump(LPEXCEPTION_POINTERS i_ep)
{

  CFileDialog dlg(FALSE, _T("dmp"), _T("crash.dmp"),
    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
    _T("Dump Files (*.dmp)|*.dmp|All Files (*.*)|*.*||")
    );
  if(IDOK != dlg.DoModal())
    return; // lazy-bones got bored by our questions
  CString path = dlg.GetPathName();

  // 
  HANDLE file = ::CreateFile( path.GetBuffer(),
    GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
  if( file == INVALID_HANDLE_VALUE )
    return;

  MINIDUMP_TYPE mdt = MiniDumpWithFullMemory;

  MINIDUMP_EXCEPTION_INFORMATION mdei = { GetCurrentThreadId(), i_ep, TRUE };
  MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), file,
    mdt, &mdei, NULL, NULL );
  ::CloseHandle( file );

}

//! enhanced copy-paste from AlignApp.cpp
LONG WINAPI reportWinExcp(LPEXCEPTION_POINTERS i_ep)
{
  // unconditionally inform user
  getReporter().showMessage(std::string("Unhandled Windows exception:\n")+ std::string(getWinExcpStr(i_ep)),MessageReporterBase::TYPE_EXCEPTION );

  reportErrorAndDump(i_ep);

  return EXCEPTION_EXECUTE_HANDLER;
}

template<class T>
static void showBoostFilesystemException( boost::filesystem::basic_filesystem_error<T>& e, const std::string& i_context )
{
  std::ostringstream stream;
  if( !i_context.empty() )
    stream << i_context << "\n";
  stream << e.what() << "\n";
  getReporter().showMessage("boost::filesystem::basic_filesystem_error derived exception",stream.str(),MessageReporterBase::TYPE_EXCEPTION);
}



static void showBoostBadLexicalCastException( boost::bad_lexical_cast& e, const std::string& i_context )
{
  std::ostringstream stream;
  if( !i_context.empty() )
    stream << i_context << "\n";
  stream << e.what() << "\n";
  stream << " source_type [" << e.source_type().name() << "] raw name [" << e.source_type().raw_name() <<  "]\n";
  stream << " target_type [" << e.target_type().name() << "] raw name [" << e.target_type().raw_name() <<  "]\n";
  getReporter().showMessage("boost::bad_lexical_cast derived exception",stream.str(),MessageReporterBase::TYPE_EXCEPTION);
}


static void showStdException( std::exception& e, const std::string& i_context )
{
  std::ostringstream stream;
  if( !i_context.empty() )
    stream << i_context << "\n";
  stream << e.what() << "\n";
  stream << "typeinfo: name " << typeid(e).name() << " raw name " << typeid(e).raw_name();
  getReporter().showMessage("std::exception derived exception",stream.str(),MessageReporterBase::TYPE_EXCEPTION);
}


static void showMFCException( CException* e, const std::string& i_context )

{
  std::ostringstream stream;
  TCHAR   szErrorMessage[1024];
  UINT    nHelpContext;

  e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage), &nHelpContext);

  if( !i_context.empty() )
    stream << i_context << "\n";
  stream << szErrorMessage << "\n";
  stream << "HelpContext " << nHelpContext << "\n";
  stream << "typeinfo: name " << typeid(*e).name() << " raw name " << typeid(*e).raw_name();
  getReporter().showMessage("CException derived exception",stream.str(),MessageReporterBase::TYPE_EXCEPTION);
}

static void showUnknownException( const std::string& i_context )
{
  std::ostringstream stream;
  if( !i_context.empty() )
    stream << i_context;
  getReporter().showMessage("Unknown Exception",stream.str(),MessageReporterBase::TYPE_EXCEPTION);
}

int processExceptions( const std::string& i_context )
{
  try
  {
    throw;
  }
  catch (boost::filesystem::basic_filesystem_error<std::string>& e) 
  {
    showBoostFilesystemException(e,i_context);
    return TS_Failed;
  }
  catch (boost::filesystem::basic_filesystem_error<boost::filesystem::path>& e) 
  {
    showBoostFilesystemException(e,i_context);
    return TS_Failed;
  }
  catch (  boost::bad_lexical_cast& e)
  {
    showBoostBadLexicalCastException(e,i_context);
    return TS_Failed;

  }
  catch( std::exception& e )
  {
    showStdException( e, i_context );
    return TS_Failed;
  }
  catch( CException* e )
  {
    showMFCException( e, i_context );
    e->Delete();
    return TS_Failed;
  }
  catch( ... )
  {
    showUnknownException( i_context );
    return TS_Failed;
  }
}
