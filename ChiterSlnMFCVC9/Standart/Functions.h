//---------------------------------------------------------------------------

#ifndef FunctionsH
#define FunctionsH
#include <iostream>
#include <vector>
#include "StandartFwd.h"
//---------------------------------------------------------------------------
STANDART_EXPORT_FUN(int) WritePointers(std::ostream& io_stream,const std::vector<DWORD>& i_list);//returm number of pointer was writed
STANDART_EXPORT_FUN(int) ReadPointers(std::istream& io_stream,std::vector<DWORD>& o_list);//return number of pointers was read
#endif

