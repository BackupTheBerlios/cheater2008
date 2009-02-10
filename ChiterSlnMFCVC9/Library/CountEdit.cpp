//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include <sstream>
#include <stdexcept>


#include "CountEdit.h"
#include "Standart/Ansi_stream.h"

extern HINSTANCE hInstance;

//---------------------------------------------------------------------------
unsigned long TCountEdit::Instance=0;

IMPLEMENT_DYNAMIC(TCountEdit, CMyBaseForm)

BEGIN_MESSAGE_MAP(TCountEdit, CMyBaseForm)
    ON_WM_VSCROLL()
    ON_WM_SIZE()
END_MESSAGE_MAP()

enum TCountEditIDS
{
IDC_TCountEdit_FRAME = 0x1212,

};
//----------------------------------------------------------------------------
TCountEdit::TCountEdit(CWnd* pParent/* = NULL*/):
CMyBaseForm(pParent)
{

}
//---------------------------------------------------------------------------
 TCountEdit::~TCountEdit(void)
{
    //TODO: Add your source code here
}

void TCountEdit::CreateInstance(void)
{
    CRect crect;
    this->GetWindowRect(&crect);
    crect.right = crect.left + 90;
    crect.bottom = crect.top + 34;
    this->MoveWindow(crect);
    //TODO: Add your source code here
    
    CFont* font = new CFont();
    font->CreatePointFont(90, CString("Arial"));

    d_staticFrame.Create(CString("Static"),SS_LEFT ,CRect(0,0,0,0),this,IDC_TCountEdit_FRAME);
    d_staticFrame.SetFont(font);
    d_staticFrame.ShowWindow(SW_SHOW);


    d_editField.Create(ES_LEFT | ES_MULTILINE | ES_WANTRETURN ,CRect(0,0,0,0),this,IDC_TCOUNTEDIT_EDITFIELD);
    d_editField.ShowWindow(TRUE);
    d_editField.SetFont(font);

    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(SCROLLINFO);     
    scrollInfo.fMask = SIF_ALL;     
    scrollInfo.nMin = 1;     
    scrollInfo.nMax = 255; 
    scrollInfo.nPage = 2;     
    scrollInfo.nPos = 16;    
    scrollInfo.nTrackPos = 2; 


    d_upDownField.Create(SBS_VERT ,CRect(0,0,0,0),this,IDC_TCOUNTEDIT_UPDOWNFIELD);
    d_upDownField.SetScrollInfo(&scrollInfo);
    d_upDownField.ShowWindow(TRUE);
  //UpDownField->OnChangingEx=UpDownFieldChangingEventEx;

 OnChangeEvent.reset();
 //OnResize=OnResizeEvent;

setMin(1);
setMax(100);
adjustSizes(-1,-1);
}

void TCountEdit::adjustSizes(int cx,int cy)
{
  CRect ret;
  this->GetClientRect(&ret);
  CRect staticFrameRect(ret);
  staticFrameRect.DeflateRect(2,2,2,2);
  d_staticFrame.MoveWindow(ret);

  CRect editRect(ret);
  editRect.DeflateRect(2,15,15,2);
  d_editField.MoveWindow(editRect);


  CRect upDownFieldRect(editRect);
  upDownFieldRect.left = editRect.right;
  upDownFieldRect.right = staticFrameRect.right-2;
  d_upDownField.MoveWindow(upDownFieldRect);

}

void TCountEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
// void  TCountEdit::UpDownFieldChangingEventEx(System::TObject* Sender, bool &AllowChange, short NewValue, TUpDownDirection Direction)
    /*if(Direction==updNone)
    {
        std::stringstream msg;
        msg << "Direction==updNone" << std::endl << std::endl
            << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
        throw std::runtime_error( msg.str() );
    }*/
    CString cstr;
    d_editField.GetWindowText(cstr);
    DWORD value=AnsiToulong(std::string(CT2CA(cstr)));
    UINT Direction = nSBCode;
    if(Direction==SB_LINEUP )
    {
        value++;
    }
    else
    {
        if(Direction==SB_LINEDOWN )
        {
            value--;
        }
    }
    if((value>=getMin())&&(value<=getMax()))
    {
        d_editField.SetWindowText( CString(ulongToAnsi(value).c_str()) );

        if(OnChangeEvent) (*OnChangeEvent)();

    }
    //AllowChange=false;
    CMyBaseForm::OnVScroll(nSBCode, nPos, pScrollBar);
}

void TCountEdit::OnSize(UINT nType, int cx, int cy)
{
    OnResizeEvent();
}


void  TCountEdit::OnResizeEvent()
{
    CRect editRect;
    editRect.top=15;
    editRect.left=2;
if(d_editField && d_upDownField)
{
    CRect UpDownFieldRect;
    UpDownFieldRect.top=editRect.top;
    UpDownFieldRect.left=editRect.right + 1;
    UpDownFieldRect.right=editRect.left + 10;
    CRect rect;
    this->GetClientRect(rect);
    // UpDownField->Width=10;
    int UpDownFieldWidth = 10;
    editRect.right=editRect.left + rect.Width()-UpDownFieldWidth-2;
    editRect.bottom=editRect.top + rect.Height()-editRect.top-2;

    editRect.right=editRect.left + 30;
    editRect.bottom=editRect.top + 50;

    UpDownFieldRect.top=editRect.top;
    UpDownFieldRect.left=editRect.left+editRect.Width()+1;
    UpDownFieldRect.bottom=UpDownFieldRect.top + editRect.Height();

    d_editField.MoveWindow(&editRect,TRUE);
    d_upDownField.MoveWindow(&UpDownFieldRect,TRUE);
}


}

void TCountEdit::Create( CWnd* pParentWnd )
{
CMyBaseForm::Create(pParentWnd);
CreateInstance();
}

void  TCountEdit::setText(const std::string & value)
{
        //TODO: Add your source code here
d_staticFrame.SetWindowText(CString(value.c_str()));
}
std::string TCountEdit::getText()
{
        //TODO: Add your source code here
    CString cstr;
    d_editField.GetWindowText(cstr);
    return    std::string(CT2CA(cstr));
}

void  TCountEdit::setMin(DWORD value)
{
        //TODO: Add your source code here
        FMin=value;
}

void  TCountEdit::setMax(DWORD value)
{
        //TODO: Add your source code here
        FMax=value;
}

