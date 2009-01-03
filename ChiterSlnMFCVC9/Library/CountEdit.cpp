//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include <sstream>
#include <stdexcept>


#include "CountEdit.h"
#include "Standart/Ansi_stream.h"


//---------------------------------------------------------------------------
unsigned long TCountEdit::Instance=0;

IMPLEMENT_DYNAMIC(TCountEdit, CDialog)

BEGIN_MESSAGE_MAP(TCountEdit, CDialog)
    ON_WM_VSCROLL()
    ON_WM_SIZE()
END_MESSAGE_MAP()


//----------------------------------------------------------------------------
 TCountEdit::TCountEdit(CWnd* pParent/* = NULL*/)
{
CreateInstance();
}
//---------------------------------------------------------------------------
 TCountEdit::~TCountEdit(void)
{
    //TODO: Add your source code here
}

void TCountEdit::CreateInstance(void)
{
    //TODO: Add your source code here
 EditField=new CEdit();
    RECT editRect;
    editRect.top=2;
    editRect.left=15;
    editRect.right=editRect.left + 30;
    editRect.bottom=editRect.top + 50;
 EditField->Create(ES_LEFT | ES_MULTILINE ,editRect,this,IDC_TCOUNTEDIT_EDITFIELD);

 UpDownField=new TUpDown();
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(SCROLLINFO);     
    scrollInfo.fMask = SIF_ALL;     
    scrollInfo.nMin = 1;     
    scrollInfo.nMax = 255; 
    scrollInfo.nPage = 2;     
    scrollInfo.nPos = 16;    
    scrollInfo.nTrackPos = 2; 

    RECT UpDownFieldRect;
    UpDownFieldRect.top=editRect.top;
    UpDownFieldRect.left=editRect.right + 1;
    UpDownFieldRect.right=editRect.left + 10;
    UpDownFieldRect.bottom=editRect.top + (editRect.bottom - editRect.top);
  UpDownField->Create(ES_LEFT | ES_MULTILINE ,UpDownFieldRect,this,IDC_TCOUNTEDIT_UPDOWNFIELD);
  UpDownField->SetScrollInfo(&scrollInfo);
 
  //UpDownField->OnChangingEx=UpDownFieldChangingEventEx;

 OnChangeEvent=NULL;
 //OnResize=OnResizeEvent;

setMin(1);
setMax(100);
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
    EditField->GetWindowText(cstr);
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
        EditField->SetWindowText( CString(ulongToAnsi(value).c_str()) );

        if(OnChangeEvent) OnChangeEvent(dynamic_cast<TObject *>(this),value);

    }
    //AllowChange=false;
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
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

 EditField->MoveWindow(&editRect,TRUE);
 UpDownField->MoveWindow(&UpDownFieldRect,TRUE);
}

void  TCountEdit::setText(const std::string & value)
{
        //TODO: Add your source code here
EditField->SetWindowText(CString(value.c_str()));
}
std::string TCountEdit::getText()
{
        //TODO: Add your source code here
    CString cstr;
    EditField->GetWindowText(cstr);
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

