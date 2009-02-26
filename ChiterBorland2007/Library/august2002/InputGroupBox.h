//---------------------------------------------------------------------------
#ifndef InputGroupBoxH
#define InputGroupBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include <string>
#include <vector>
#include "TypeComboBox.h"
#include "Ansilib.h"

extern char* FOR_POINTERS ;
//---------------------------------------------------------------------------
class PACKAGE TInputGroupBox : public TGroupBox
{
private:
    TComboBox* CreateTextBox(void);
    TTypeComboBox* CreateTypeBox(void);
    void CreateLabels(void);
//    void __fastcall SetTypeOfBox(std::string  value);
//    std::string  __fastcall GetTypeOfBox();
    std::string FTypeOfBox;
    std::string __fastcall GetText();
    void __fastcall BoxesChange(void);
static unsigned long Instance;
    TMenuItem* SavePointerMenuItem;
    TMenuItem* ClearPointersMenuItem;
    TMenuItem* ConvertToMenuItem;
    TMenuItem* ConvertToHexNumMenuItem;
    TMenuItem* ConvertToHexStringMenuItem;
    TMenuItem* ConvertToStringMenuItem;
    TMenuItem* ConvertToDecNumMenuItem;
    TMenuItem* ConvertToFloatNumMenuItem;
    TMenuItem* ConvertToDoubleNumMenuItem;

    TStrings* __fastcall GetStrings();
    TPopupMenu* __fastcall CreatePopupMenu(void);
    void __fastcall TInputGroupBoxSavePointerMenuItemOnClick(TObject*);
    void __fastcall TInputGroupBoxPopupMenuAppear(TObject*);
    void __fastcall TInputGroupBoxConvertToHexNumMenuItemOnClick(TObject*);
    void __fastcall TInputGroupBoxConvertToHexStringMenuItemOnClick(TObject*);
    void __fastcall TInputGroupBoxConvertToStringMenuItemOnClick(TObject*);
    void __fastcall TInputGroupBoxConvertToDecNumMenuItemOnClick(TObject*);
    void __fastcall TInputGroupBoxConvertToDoubleNumMenuItemOnClick(TObject*);
    void __fastcall TInputGroupBoxConvertToFloatNumMenuItemOnClick(TObject*);
    void __fastcall OnResizeBox(TObject*);
    void __fastcall SetList(TList* value);
protected:
    TComboBox* TextBox;
    TComboBox* TypeBox;
public:
    __property TStrings* Strings  = { read = GetStrings };
     void __fastcall TInputGroupBoxTextBoxChange(TObject*);
     void __fastcall TInputGroupBoxTypeBoxChange(TObject*);
     void __fastcall TInputGroupBoxOnEnter(TObject*);
     __fastcall TInputGroupBox(TComponent* Owner);
    __fastcall ~TInputGroupBox(void);
    void __fastcall WriteDataToStream(std::ostream& io_stream);
    unsigned long __fastcall GetPointer(void);
    bool __fastcall IsEmpty(void);
    void __fastcall Clear(void);
    void __fastcall Add(const std::string&);
    void __fastcall TInputGroupBoxClearPointersMenuItemOnClick(TObject*);
    void __fastcall InitBox(const std::string&,int);
//        void __fastcall LoadPointers(SMemoryPointers* list);
    void __fastcall LoadPointers(const std::vector<DWORD>& list);
    __property std::string Text  = { read = GetText };

//    __property std::string TypeOfBox  = { read = GetTypeOfBox, write = SetTypeOfBox };
  __published:
};

//---------------------------------------------------------------------------
#endif




