//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Functions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
int ReadPointers(std::istream& io_stream,std::vector<DWORD>& o_list)
{
int num;
int i=0;
DWORD Pointer;
 io_stream.read((char*)&num,sizeof(int));
  for(;i<num;i++)
   {
	io_stream.read((char*)&Pointer,sizeof(int));
	o_list.push_back(Pointer);
   }
  return i;
}
int WritePointers(std::ostream& io_stream,const std::vector<DWORD>& i_list)
{
int num=i_list.size();
int i=0;
int Pointer;
 io_stream.write((char*)&num,sizeof(int));
  for(;i<num;i++)
   {
    Pointer=i_list[i];
	io_stream.write((char*)&Pointer,sizeof(int));

   }
  return i;
}

