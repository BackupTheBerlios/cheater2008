//---------------------------------------------------------------------------
#ifndef CountEditH
#define CountEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <ComCtrls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include "Ansilib.h"

typedef void __fastcall (__closure *TCountEditValueChangedEvent)(TObject *Sender,DWORD value);
//---------------------------------------------------------------------------
class PACKAGE TCountEdit : public TGroupBox
{
private:

     void CreateInstance(void);
    void __fastcall OnResizeEvent(TObject*);
static unsigned long Instance;
        DWORD FMin;
        DWORD FMax;
        void __fastcall SetText(AnsiString value);
        AnsiString __fastcall GetText();
        void __fastcall SetMin(DWORD value);
        void __fastcall SetMax(DWORD value);
protected:
    TEdit* EditField;
    TUpDown* UpDownField;
public:
    __fastcall TCountEdit(TComponent* Owner);
    __fastcall ~TCountEdit(void);
void __fastcall UpDownFieldChangingEventEx(System::TObject* Sender, bool &AllowChange, short NewValue, TUpDownDirection Direction);
        TCountEditValueChangedEvent OnChangeEvent;
        __property AnsiString Text  = { read=GetText, write=SetText };
        __property DWORD Min  = { read=FMin, write=SetMin };
        __property DWORD Max  = { read=FMax, write=SetMax };

};
//---------------------------------------------------------------------------
#endif


