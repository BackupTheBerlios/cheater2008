//---------------------------------------------------------------------------

#ifndef MemoryInfoEditH
#define MemoryInfoEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include "ansi_stream.h"
//---------------------------------------------------------------------------
class PACKAGE TMemoryInfoEdit : public TGroupBox
{
private:
        TMemo* Memo;
protected:
public:
        __fastcall TMemoryInfoEdit(TComponent* Owner);
        void __fastcall SetMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo);
        MEMORY_BASIC_INFORMATION __fastcall GetMemoryInfo(void);
__published:
};
//---------------------------------------------------------------------------
#endif
