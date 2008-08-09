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

Memo->Lines->Values["BaseeAddress"]=AnsiString(ulongToHexAnsi((unsigned long)MemInfo.BaseAddress).c_str());
Memo->Lines->Values["AllocationBase"]=AnsiString(ulongToHexAnsi((unsigned long)MemInfo.AllocationBase).c_str());
Memo->Lines->Values["AllocationProtect"]=AnsiString(ulongToHexAnsi(MemInfo.AllocationProtect).c_str());
Memo->Lines->Values["RegionSize"]=AnsiString(ulongToHexAnsi(MemInfo.RegionSize).c_str());
Memo->Lines->Values["State"]=AnsiString(ulongToHexAnsi(MemInfo.State).c_str());
Memo->Lines->Values["Protect"]=AnsiString(ulongToHexAnsi(MemInfo.Protect).c_str());
Memo->Lines->Values["Type"]=AnsiString(ulongToHexAnsi(MemInfo.Type).c_str());
}


MEMORY_BASIC_INFORMATION __fastcall TMemoryInfoEdit::GetMemoryInfo(void)
{
 MEMORY_BASIC_INFORMATION MemInfo;
 MemInfo.BaseAddress=(void*)HexAnsiToulong(Memo->Lines->Values["BaseeAddress"].c_str());
 MemInfo.AllocationBase=(void*)HexAnsiToulong(Memo->Lines->Values["AllocationBase"].c_str());
 MemInfo.AllocationProtect=HexAnsiToulong(Memo->Lines->Values["AllocationProtect"].c_str());
 MemInfo.RegionSize=HexAnsiToulong(Memo->Lines->Values["RegionSize"].c_str());
 MemInfo.State=HexAnsiToulong(Memo->Lines->Values["State"].c_str());
 MemInfo.Protect=HexAnsiToulong(Memo->Lines->Values["Protect"].c_str());
 MemInfo.Type=HexAnsiToulong(Memo->Lines->Values["Type"].c_str());

 return MemInfo;
}

