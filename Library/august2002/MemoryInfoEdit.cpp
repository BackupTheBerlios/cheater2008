//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MemoryInfoEdit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMemoryInfoEdit *)
{
        new TMemoryInfoEdit(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMemoryInfoEdit::TMemoryInfoEdit(TComponent* Owner)
        : TGroupBox(Owner)
{
Memo=new TMemo(this);
Memo->Align=alClient;
InsertControl(Memo);
}
//---------------------------------------------------------------------------
namespace Memoryinfoedit
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TMemoryInfoEdit)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------


void __fastcall TMemoryInfoEdit::SetMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo)
{

Memo->Lines->Values["BaseeAddress"]=ulongToHexAnsi((unsigned long)MemInfo.BaseAddress);
Memo->Lines->Values["AllocationBase"]=ulongToHexAnsi((unsigned long)MemInfo.AllocationBase);
Memo->Lines->Values["AllocationProtect"]=ulongToHexAnsi(MemInfo.AllocationProtect);
Memo->Lines->Values["RegionSize"]=ulongToHexAnsi(MemInfo.RegionSize);
Memo->Lines->Values["State"]=ulongToHexAnsi(MemInfo.State);
Memo->Lines->Values["Protect"]=ulongToHexAnsi(MemInfo.Protect);
Memo->Lines->Values["Type"]=ulongToHexAnsi(MemInfo.Type);
}


MEMORY_BASIC_INFORMATION __fastcall TMemoryInfoEdit::GetMemoryInfo(void)
{
 MEMORY_BASIC_INFORMATION MemInfo;
 MemInfo.BaseAddress=(void*)HexAnsiToulong(Memo->Lines->Values["BaseeAddress"]);
 MemInfo.AllocationBase=(void*)HexAnsiToulong(Memo->Lines->Values["AllocationBase"]);
 MemInfo.AllocationProtect=HexAnsiToulong(Memo->Lines->Values["AllocationProtect"]);
 MemInfo.RegionSize=HexAnsiToulong(Memo->Lines->Values["RegionSize"]);
 MemInfo.State=HexAnsiToulong(Memo->Lines->Values["State"]);
 MemInfo.Protect=HexAnsiToulong(Memo->Lines->Values["Protect"]);
 MemInfo.Type=HexAnsiToulong(Memo->Lines->Values["Type"]);


return MemInfo;

}

