// InputGroupBox.cpp : implementation file
//

#include "stdafx.h"

#include <assert.h>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include "Library.h"
#include "Standart/Ansi_Stream.h"
#include "InputGroupBox.h"

extern HINSTANCE  hInstance;

// InputGroupBox dialog

IMPLEMENT_DYNAMIC(InputGroupBox, CDialog)

BEGIN_MESSAGE_MAP(InputGroupBox, CDialog)
    ON_CBN_EDITCHANGE(IDC_COMBO_VALUE, &InputGroupBox::OnValueValue)
    ON_CBN_EDITCHANGE(IDC_COMBO_TYPE, &InputGroupBox::OnTypeValue)
    ON_WM_RBUTTONDOWN()
    ON_WM_CREATE()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()


InputGroupBox::InputGroupBox(CWnd* pParent /*=NULL*/)
:d_hDialogInit(0) //CDialog(CMemoryInfoEdit::IDD, pParent)
{
    VERIFY( InitModalIndirect( initDialog(hInstance,MAKEINTRESOURCE(InputGroupBox::IDD)), pParent ) );

	d_savePointerMenuItemState = (MF_DISABLED|MF_STRING);
	d_convertToMenuItemState = (MF_DISABLED|MF_STRING);
	d_clearPointersMenuItemState = (MF_DISABLED|MF_STRING);
}

InputGroupBox::~InputGroupBox()
{
}
void InputGroupBox::Create( CWnd* pParentWnd )
{
    CreateIndirect( initDialog(hInstance,MAKEINTRESOURCE(InputGroupBox::IDD)), pParentWnd );

    //CDialog::Create( MAKEINTRESOURCE(CMemoryInfoEdit::IDD), wnd );
}

HGLOBAL InputGroupBox::initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName )
{
    d_hModule = hinst;


    d_hDialogInit = NULL;
    HRSRC hDlgInit = ::FindResource( hinst, lpszTemplateName, RT_DLGINIT);
    if( hDlgInit )
    {
        // load it
        d_hDialogInit = LoadResource( hinst, hDlgInit );
        if( d_hDialogInit == NULL )
        {
            TRACE0("Warning: Load of RT_DLGINIT failed during dialog init.\n");
        }
        else
        {
            // lock it
            m_lpDialogInit = LockResource(d_hDialogInit);
            ASSERT(m_lpDialogInit != NULL);
        }
    }

    // load dialog
    HRSRC hrsrc = FindResource( hinst, lpszTemplateName, RT_DIALOG );
    if( hrsrc != 0 )
        return LoadResource( hinst, hrsrc );
    else
        return 0;
}
BOOL InputGroupBox::OnInitDialog()
{
	CDialog::OnInitDialog();
    CWnd* wndType = GetDlgItem(IDC_COMBO_TYPE);
    CWnd* wndValue = GetDlgItem(IDC_COMBO_VALUE);
    CRuntimeClass* cl= wndType->GetRuntimeClass();
    CComboBox* pValue = &d_valueBox; 
    CComboBox* pType= &d_typeCombo; 
    
	CComboBox* typeCombo = dynamic_cast<CComboBox*>((CComboBox *)wndType);
	CComboBox* textCombo = dynamic_cast<CComboBox*>((CComboBox *)wndValue);
	assert(typeCombo);
	assert(textCombo);

	typeCombo->AddString(LPCTSTR( getStringType()[HEX_NUM]) );
	typeCombo->AddString(LPCTSTR( getStringType()[DEC_NUM]) );
	typeCombo->AddString(LPCTSTR( getStringType()[STRING]) ) ;
	typeCombo->AddString(LPCTSTR( getStringType()[HEX_STRING]) );
	typeCombo->AddString(LPCTSTR( getStringType()[FLOAT_NUM]) );
	typeCombo->AddString(LPCTSTR( getStringType()[DOUBLE_NUM]) );
	return TRUE;
}


void InputGroupBox::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_VALUE, d_valueBox);
    DDX_Control(pDX, IDC_COMBO_TYPE, d_typeCombo);
}


// InputGroupBox message handlers

void InputGroupBox::OnValueValue()
{
	// TODO: Add your control notification handler code here
	BoxesChange();
}

void InputGroupBox::OnTypeValue()
{
	// TODO: Add your control notification handler code here
	BoxesChange();
}


void InputGroupBox::BoxesChange()
{
	CComboBox* typeCombo = dynamic_cast<CComboBox*>(GetDlgItem(IDC_COMBO_TYPE));
	CComboBox* textCombo = dynamic_cast<CComboBox*>(GetDlgItem(IDC_COMBO_VALUE));
	assert(typeCombo);
	assert(textCombo);
	CString type;
	typeCombo->GetLBText( typeCombo->GetCurSel(),type );
	CString text;
	textCombo->GetWindowText( text );
	//TODO: Add your source code here
	if (!text.IsEmpty()&&!type.IsEmpty())
	{
		int Work=WhatAnsiType(std::string( CT2CA(text)) );
		if((typeCombo->GetCurSel()!=DOUBLE_NUM)&&(typeCombo->GetCurSel()!=FLOAT_NUM))
		{
			if((Work!=DEC_NUM)&&(typeCombo->GetCurSel()==DEC_NUM))
				textCombo->SetWindowText(CString(""));
			else
				if((Work==STRING)&&(typeCombo->GetCurSel()!=STRING))
					textCombo->SetWindowText(CString(""));
		}
	}

}


//------------------------------------------------------------------------------------------------------------------

void  InputGroupBox::InitBox(const std::string& i_str,int Type)
{
	CComboBox* typeCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_TYPE));
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(typeCombo);
	assert(textCombo);

	//TODO: Add your source code here
	if(Type<=FLOAT_NUM)
	{
		typeCombo->SetCurSel(Type);
		textCombo->SetWindowText(CString(i_str.c_str()));
	}
}

void  InputGroupBox::writeDataToStream(std::ostream& io_stream)
{
	//TODO: Add your source code here
	CComboBox* typeCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_TYPE));
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(typeCombo);
	assert(textCombo);
	CString text;
	textCombo->GetWindowText(text);
	WriteTypedAnsiToStream(std::string(CT2CA(text)),typeCombo->GetCurSel(),io_stream);
}

unsigned long  InputGroupBox::GetPointer()
{
	//TODO: Add your source code here

	CComboBox* typeCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_TYPE));
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(typeCombo);
	assert(textCombo);
	CString text;
	textCombo->GetWindowText(text);

	return PointerAnsiToulong(std::string(CT2CA(text)),typeCombo->GetCurSel());
}

bool  InputGroupBox::IsEmpty(void) const
{
	//TODO: Add your source code here
	CComboBox* typeCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_TYPE));
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(typeCombo);
	assert(textCombo);
	CString text;
	CString type;
	textCombo->GetWindowText(text);
	typeCombo->GetWindowText(type);

	return text.IsEmpty()||type.IsEmpty();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

std::string  InputGroupBox::getText() const
{
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(textCombo);
	CString text;
	textCombo->GetWindowText(text);

	return std::string((CT2CA)text);
}

void  InputGroupBox::setText(const std::string& str)
{
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(textCombo);
	textCombo->SetWindowText(CString( str.c_str()) );

}

StringIndex InputGroupBox::getType() const
{
	CComboBox* typeCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_TYPE));
	assert(typeCombo);

	StringIndex sel = StringIndex(typeCombo->GetCurSel());
	return sel;	
}


void InputGroupBox::setType(const StringIndex & i_type)
{
	CComboBox* typeCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_TYPE));
	assert(typeCombo);

	int found = typeCombo->FindString(-1,CString(getStringType()[i_type]) );

	if( CB_ERR == found)
	{
		std::stringstream msg;
		msg << " Wrong value type!"  << std::endl << std::endl
			<< " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
		throw std::runtime_error( msg.str() );
	}
	typeCombo->SetCurSel(found);
}

void  InputGroupBox::Clear(void)
{
	//TODO: Add your source code here
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(textCombo);
	while(textCombo->GetCount()>0)
		textCombo->DeleteString(0);
}

void  InputGroupBox::addText(const std::string& str)
{
	//TODO: Add your source code here
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(textCombo);
	int found = textCombo->FindString(-1,CString(str.c_str()));
	if( CB_ERR == found)
		textCombo->AddString(CString( str.c_str()) );
}

std::vector<std::string>  InputGroupBox::getStrings()
{
	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(textCombo);
	std::vector<std::string> ret;
	for (int i =0; i< textCombo->GetCount();++i)
	{
		CString text;
		textCombo->GetLBText(i,text);
		ret.push_back( std::string(CT2CA( text ) ) );
	}

	return ret;
}

CMenu*  InputGroupBox::CreatePopupMenu(void)
{
	
	//TODO: Add your source code here
	CMenu* work=new CMenu();
	work->CreatePopupMenu();
/*
	work->OnPopup=InputGroupBoxPopupMenuAppear;
	TMenuItem* MenuItem;
	MenuItem=new TMenuItem(work);
	SavePointerMenuItem=MenuItem;
	MenuItem->Caption="Save value";
	SavePointerMenuItem->Enabled=false;
	MenuItem->OnClick=InputGroupBoxSavePointerMenuItemOnClick;
	work->Items->Add(MenuItem);
	MenuItem=new TMenuItem(work);
	ClearPointersMenuItem=MenuItem;
	MenuItem->Caption="Clear TextBox";
	ClearPointersMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxClearPointersMenuItemOnClick;
	work->Items->Add(MenuItem);

	MenuItem=new TMenuItem(work);
	MenuItem->Caption="-";
	MenuItem->Enabled=true;
	work->Items->Add(MenuItem);

	MenuItem=new TMenuItem(work);
	ConvertToMenuItem=MenuItem;
	MenuItem->Caption="Convert to...";
	ConvertToMenuItem->Enabled=true;
	work->Items->Add(MenuItem);

	MenuItem=new TMenuItem(ConvertToMenuItem);
	ConvertToDecNumMenuItem=MenuItem;
	MenuItem->Caption=getStringType()[DEC_NUM];
	ConvertToDecNumMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxConvertToDecNumMenuItemOnClick;
	ConvertToMenuItem->Add(MenuItem);

	MenuItem=new TMenuItem(ConvertToMenuItem);
	ConvertToHexNumMenuItem=MenuItem;
	MenuItem->Caption=getStringType()[HEX_NUM];
	ConvertToHexNumMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxConvertToHexNumMenuItemOnClick;
	ConvertToMenuItem->Add(MenuItem);

	MenuItem=new TMenuItem(ConvertToMenuItem);
	ConvertToStringMenuItem=MenuItem;
	MenuItem->Caption=getStringType()[STRING];
	ConvertToStringMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxConvertToStringMenuItemOnClick;
	ConvertToMenuItem->Add(MenuItem);

	MenuItem=new TMenuItem(ConvertToMenuItem);
	ConvertToHexStringMenuItem=MenuItem;
	MenuItem->Caption=getStringType()[HEX_STRING];
	ConvertToHexStringMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxConvertToHexStringMenuItemOnClick;
	ConvertToMenuItem->Add(MenuItem);

	MenuItem=new TMenuItem(ConvertToMenuItem);
	ConvertToDoubleNumMenuItem=MenuItem;
	MenuItem->Caption=getStringType()[DOUBLE_NUM];
	ConvertToDoubleNumMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxConvertToDoubleNumMenuItemOnClick;
	ConvertToMenuItem->Add(MenuItem);

	MenuItem=new TMenuItem(ConvertToMenuItem);
	ConvertToFloatNumMenuItem=MenuItem;
	MenuItem->Caption=getStringType()[FLOAT_NUM];
	ConvertToFloatNumMenuItem->Enabled=true;
	MenuItem->OnClick=InputGroupBoxConvertToFloatNumMenuItemOnClick;
	ConvertToMenuItem->Add(MenuItem);
*/
  work->AppendMenu(d_savePointerMenuItemState, ID_APP_EXIT, CString("Save value") );
  work->AppendMenu(d_clearPointersMenuItemState, ID_APP_EXIT, CString("Clear TextBox") );
  work->AppendMenu(d_convertToMenuItemState, ID_APP_EXIT, CString("Convert to...") );
  work->AppendMenu(MF_STRING, HEX_NUM, CString( getStringType()[HEX_NUM] ) );
  work->AppendMenu(MF_STRING, DEC_NUM, CString( getStringType()[DEC_NUM] ) );
  work->AppendMenu(MF_STRING, STRING, CString( getStringType()[STRING]) );
  work->AppendMenu(MF_STRING, HEX_STRING, CString( getStringType()[HEX_STRING]) );
  work->AppendMenu(MF_STRING, DOUBLE_NUM, CString( getStringType()[DOUBLE_NUM]) );
  work->AppendMenu(MF_STRING, FLOAT_NUM, CString( getStringType()[FLOAT_NUM]) );
  return work;
}

BOOL InputGroupBox::OnCmdMsg(UINT nID, int nCode, void* pExtra,
                       AFX_CMDHANDLERINFO* pHandlerInfo)
{

    if (nCode== CN_COMMAND)
    { // pop-up menu sent CN_COMMAND
        switch (nID)
        {
        case HEX_NUM:
            InputGroupBoxConvertToHexNumMenuItemOnClick();
            break;
        case DEC_NUM:
            InputGroupBoxConvertToDecNumMenuItemOnClick();
            break;
        case STRING:
            InputGroupBoxConvertToStringMenuItemOnClick();
            break;
        case HEX_STRING:
            InputGroupBoxConvertToHexStringMenuItemOnClick();
            break;
        case DOUBLE_NUM:
            InputGroupBoxConvertToDoubleNumMenuItemOnClick();
            break;
        case FLOAT_NUM:
            InputGroupBoxConvertToFloatNumMenuItemOnClick();
            break;

        }
        return TRUE;
    }
    // If the object(s) in the extended command route don't handle
    // the command, then let the base class OnCmdMsg handle it.
    return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void  InputGroupBox::InputGroupBoxSavePointerMenuItemOnClick()
{
	//TODO: Add your source code here
	//adds input text to list

	CComboBox* textCombo = dynamic_cast<CComboBox*>(this->GetDlgItem(IDC_COMBO_VALUE));
	assert(textCombo);
	addText(getText());
}

void  InputGroupBox::InputGroupBoxClearPointersMenuItemOnClick()
{
	//TODO: Add your source code here
	Clear();
}

void  InputGroupBox::InputGroupBoxPopupMenuAppear()
{
	//TODO: Add your source code here
	d_savePointerMenuItemState=IsEmpty() ? (MF_DISABLED|MF_STRING) : (MF_ENABLED|MF_STRING);
	d_convertToMenuItemState=IsEmpty() ? (MF_DISABLED|MF_STRING) : (MF_ENABLED|MF_STRING);
	d_clearPointersMenuItemState=(getStrings().empty()) ? (MF_DISABLED|MF_STRING) : (MF_ENABLED|MF_STRING);
}

void  InputGroupBox::InputGroupBoxConvertToHexNumMenuItemOnClick()
{
	//TODO: Add your source code here
	unsigned long num;
	num=0;
	num=GetPointer();
	setType(HEX_NUM);
	setText( ulongToHexAnsi(num).c_str() );
}

void  InputGroupBox::InputGroupBoxConvertToHexStringMenuItemOnClick()
{
	//TODO: Add your source code here
	unsigned long len;
	char* x;
	x=TypedAnsiTocharptr(getText(),getType(),&len);
	setType(HEX_STRING);
	setText( charptrToHexAnsi(x,len).c_str() );
	delete[] x;
}

void  InputGroupBox::InputGroupBoxConvertToStringMenuItemOnClick()
{
	//TODO: Add your source code here
	unsigned long len;
	char* x;
	std::string res("");
	char ch[2];
	ch[1]=0;
	x=TypedAnsiTocharptr(getText(),getType(),&len);
	setType(STRING);
	for(unsigned long i=0;i<len;i++)
	{
		ch[0]=ConvertToPrintSign(x[i]);
		res.append(ch);
	}
	setText(res);
	delete[] x;
}

void  InputGroupBox::InputGroupBoxConvertToDecNumMenuItemOnClick()
{
	//TODO: Add your source code here
	unsigned long num;
	num=0;
	num=GetPointer();
	setType(DEC_NUM);
	setText( ulongToAnsi(num).c_str() );
}

void  InputGroupBox::InputGroupBoxConvertToDoubleNumMenuItemOnClick()
{
	//TODO: Add your source code here
	WORK_ANSILIB_UNION_FOR_CONVERT work;
	work.double_=0;
	unsigned long len;
	char* x;
	x=TypedAnsiTocharptr(getText(),getType(),&len);
	memcpy(&work,x,sizeof(double));
	setType(DOUBLE_NUM);
	std::stringstream msg;
	msg << work.double_;
	setText( msg.str() );
	delete[] x;
}
//------------------------
void  InputGroupBox::InputGroupBoxConvertToFloatNumMenuItemOnClick()
{
	//TODO: Add your source code here
	WORK_ANSILIB_UNION_FOR_CONVERT work;
	work.double_=0;
	unsigned long len;
	char* x;
	x=TypedAnsiTocharptr(getText(),getType(),&len);
	memcpy(&work,x,sizeof(float));
	setType( FLOAT_NUM );

	std::stringstream msg;
	msg << work.float_;
	setText( msg.str() );
	delete[] x;
}

//--------------------
void  InputGroupBox::loadPointers(const std::vector<DWORD>&   list)
{
	if(list.size()<=200)
	{
		for(unsigned int i=0;i<list.size();i++)
		{
			addText(ulongToHexAnsi((unsigned long)list[i]));
		}
	}
}
//----------------------------------------------------------------------------
void  InputGroupBox::OnResizeBox()
{
	// TextBox->Width=Width-6;
	// TypeBox->Width=Width-6;
}


void InputGroupBox::OnCleartextbox()
{
	// TODO: Add your command handler code here
}

void InputGroupBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnRButtonDown(nFlags, point);
	CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
	boost::scoped_ptr<CMenu> myMenu(CreatePopupMenu());    
	ASSERT(myMenu.get());
	myMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, 
		point.y, this);


}

int InputGroupBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here

    return 0;
}

void InputGroupBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
