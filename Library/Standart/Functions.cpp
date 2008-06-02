//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Functions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
int ReadPointers(TStream* stream,TList* list)
{
int num;
int i=0;
int Pointer;
 stream->Read(&num,sizeof(int));
  for(;i<num;i++)
   {
    stream->Read(&Pointer,sizeof(int));
    list->Add((void*)Pointer);
   }
  return i;
}
int WritePointers(TStream* stream,TList* list)
{
int num=list->Count;
int i=0;
int Pointer;
 stream->Write(&num,sizeof(int));
  for(;i<num;i++)
   {
    Pointer=(int)list->Items[i];   
    stream->Write(&Pointer,sizeof(int));

   }
  return i;
}
