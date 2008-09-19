//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Functions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
int ReadPointers(std::istream& io_stream,TList& o_list)
{
int num;
int i=0;
int Pointer;
 io_stream.read((char*)&num,sizeof(int));
  for(;i<num;i++)
   {
	io_stream.read((char*)&Pointer,sizeof(int));
	o_list.Add((void*)Pointer);
   }
  return i;
}
int WritePointers(std::ostream& io_stream,TList& i_list)
{
int num=i_list.Count;
int i=0;
int Pointer;
 io_stream.write((char*)&num,sizeof(int));
  for(;i<num;i++)
   {
    Pointer=(int)i_list.Items[i];
	io_stream.write((char*)&Pointer,sizeof(int));

   }
  return i;
}

