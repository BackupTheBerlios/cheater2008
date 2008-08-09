//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Functions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
int ReadPointers(TStream& i_stream,TList& o_list)
{
int num;
int i=0;
int Pointer;
 i_stream.Read(&num,sizeof(int));
  for(;i<num;i++)
   {
	i_stream.Read(&Pointer,sizeof(int));
	o_list.Add((void*)Pointer);
   }
  return i;
}
int WritePointers(TStream& io_stream,TList& i_list)
{
int num=i_list.Count;
int i=0;
int Pointer;
 io_stream.Write(&num,sizeof(int));
  for(;i<num;i++)
   {
    Pointer=(int)i_list.Items[i];   
	io_stream.Write(&Pointer,sizeof(int));

   }
  return i;
}
