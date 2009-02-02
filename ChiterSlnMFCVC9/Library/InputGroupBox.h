#pragma once

#include <string>
#include <vector>
#include "Standart/Ansilib.h"
#include "Library/august2002Fwd.h"
#include "Library/resource.h"
#include "Library/MyBaseForm.h"
#include "afxwin.h"
// InputGroupBox dialog

class AUGUST2002_EXPORT InputGroupBox : public CMyBaseForm
{
  // imposes DECLARE_DYNAMIC and enables dynamic consruction
  DECLARE_DYNCREATE(InputGroupBox)
	void BoxesChange();
public:
	InputGroupBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~InputGroupBox();
// Dialog Data
	enum { IDD = IDD_INPUTGROUPBOX };

	void  InitBox(const std::string& i_str,int Type);
	void  writeDataToStream(std::ostream& io_stream);
	unsigned long  GetPointer();
	bool  IsEmpty(void) const;

	StringIndex getType() const;
	void setType(const StringIndex & i_type);

	void  Clear(void);

	void  addText(const std::string& str);
	void  setText(const std::string& str);
	std::string  getText() const;
	std::vector<std::string>  getStrings();

  void setCaption(const std::string& i_caption);


private:
  DWORD d_savePointerMenuItemState;
  DWORD d_convertToMenuItemState;
  DWORD d_clearPointersMenuItemState;
	CMenu*  CreatePopupMenu(void);
  void initialize();
  CComboBox d_valueBox;
  CComboBox d_typeBox;

protected:
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
        AFX_CMDHANDLERINFO* pHandlerInfo);

	void  InputGroupBoxSavePointerMenuItemOnClick();
	void  InputGroupBoxClearPointersMenuItemOnClick();
	void  InputGroupBoxPopupMenuAppear();
	void  InputGroupBoxConvertToHexNumMenuItemOnClick();
	void  InputGroupBoxConvertToHexStringMenuItemOnClick();
	void  InputGroupBoxConvertToStringMenuItemOnClick();
	void  InputGroupBoxConvertToDecNumMenuItemOnClick();
	void  InputGroupBoxConvertToDoubleNumMenuItemOnClick();
	void  InputGroupBoxConvertToFloatNumMenuItemOnClick();

	void  loadPointers(const std::vector<DWORD>&   list);

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnValueValue();
  afx_msg void OnTypeValue();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void  OnResizeBox();
  afx_msg void  OnCleartextbox();

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()


};


/*
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
*/