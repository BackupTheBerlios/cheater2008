// MmemoryInfoEdit.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop

#include "MemoryInfoEdit.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <map>
#include <boost/regex.hpp>
extern HINSTANCE  hInstance;
// CMemoryInfoEdit dialog
std::string stripSpaces(const std::string& i_str)
{
	size_t beg = i_str.find_first_not_of (' ');
	size_t end = i_str.find_last_not_of (' ');
	std::string ret;
	if ( beg!=std::string::npos && end!=std::string::npos)
	{
		ret=i_str.substr(beg,end-beg+1);
	}
	else
	{
		std::stringstream msg;
		msg << "Exception:\n" << __FUNCTION__ << "\n" << __FILE__ << " (" << __LINE__ << ")" << std::endl
			<< "Wrong string for stripping [" << i_str << "]";
		throw std::runtime_error( msg.str() );
	}
	return ret;
}
IMPLEMENT_DYNAMIC(CMemoryInfoEdit, CDialog)

BEGIN_MESSAGE_MAP(CMemoryInfoEdit, CDialog)
    ON_WM_VSCROLL()
END_MESSAGE_MAP()


CMemoryInfoEdit::CMemoryInfoEdit(CWnd* pParent /*=NULL*/)
:d_hDialogInit(0) //CDialog(CMemoryInfoEdit::IDD, pParent)
{
  VERIFY( InitModalIndirect( initDialog(hInstance,MAKEINTRESOURCE(CMemoryInfoEdit::IDD)), pParent ) );
}

CMemoryInfoEdit::~CMemoryInfoEdit()
{
}

HGLOBAL CMemoryInfoEdit::initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName )
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


void CMemoryInfoEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CMemoryInfoEdit::Create( CWnd* pParentWnd )
{
    CreateIndirect( initDialog(hInstance,MAKEINTRESOURCE(CMemoryInfoEdit::IDD)), pParentWnd );

	//CDialog::Create( MAKEINTRESOURCE(CMemoryInfoEdit::IDD), wnd );
}

void  CMemoryInfoEdit::SetMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo)
{
	CEdit*  memo = dynamic_cast<CEdit*> ( GetDlgItem(IDC_EDIT_MEMO) );
	if (!memo)
	{
		std::stringstream msg;
		msg << "Exception:\n" << __FUNCTION__ << "\n" << __FILE__ << " (" << __LINE__ << ")";
		throw std::runtime_error( msg.str() );
	}
	std::stringstream msg;
	msg << "BaseeAddress=" << ulongToHexAnsi((unsigned long)MemInfo.BaseAddress).c_str() << "\r\n";
	msg << "AllocationBase" << ulongToHexAnsi((unsigned long)MemInfo.AllocationBase).c_str() << "\r\n";
	msg << "AllocationProtect" << ulongToHexAnsi(MemInfo.AllocationProtect).c_str() << "\r\n";
	msg << "RegionSize" << ulongToHexAnsi(MemInfo.RegionSize).c_str() << "\r\n";
	msg << "State" << ulongToHexAnsi(MemInfo.State).c_str() << "\r\n";
	msg << "Protect" << ulongToHexAnsi(MemInfo.Protect).c_str() << "\r\n";
	msg << "Type" << ulongToHexAnsi(MemInfo.Type).c_str() << "\r\n";

	memo->SetWindowText(CString(msg.str().c_str()));
}

MEMORY_BASIC_INFORMATION  CMemoryInfoEdit::GetMemoryInfo(void)
{
	CEdit*  memo = dynamic_cast<CEdit*> ( GetDlgItem(IDC_EDIT_MEMO) );
	if (!memo)
	{
		std::stringstream msg;
		msg << "Exception:\n" << __FUNCTION__ << "\n" << __FILE__ << " (" << __LINE__ << ")";
		throw std::runtime_error( msg.str() );
	}
	MEMORY_BASIC_INFORMATION MemInfo;
	CString text;
	memo->GetWindowText(text);
	CT2CA pszConvertedAnsiString (text);
	std::string str(pszConvertedAnsiString);
	std::map<std::string, std::string> map; 

	boost::regex expression("([^=])*=([^=])*"); 

	std::string::const_iterator start, end; 
	start = str.begin(); 
	end = str.end(); 
	boost::match_results<std::string::const_iterator> what; 
	boost::match_flag_type flags = boost::match_default; 
	while(regex_search(start, end, what, expression, flags)) 
	{ 
		// what[0] contains the whole string 
		// what[1] contains name. 
		// what[2] contains value
		map[std::string(what[1].first, what[1].second)] 
		= std::string(what[2].first, what[2].second);
		// update search position: 
		start = what[0].second; 
		// update flags: 
		flags |= boost::match_prev_avail; 
		flags |= boost::match_not_bob; 
	}
	std::map<std::string,std::string>::iterator it;

	if ( (it=map.find("BaseeAddress"))!=map.end())
		MemInfo.BaseAddress=(void*)HexAnsiToulong(it->second);
	if ( (it=map.find("AllocationBase"))!=map.end())
		MemInfo.AllocationBase=(void*)HexAnsiToulong(it->second);
	MemInfo.AllocationProtect=HexAnsiToulong(it->second);
	if ( (it=map.find("RegionSize"))!=map.end())
		MemInfo.RegionSize=HexAnsiToulong(it->second);
	MemInfo.State=HexAnsiToulong(it->second);
	if ( (it=map.find("RegionSize"))!=map.end())
		MemInfo.Protect=HexAnsiToulong(it->second);
	if ( (it=map.find("Type"))!=map.end())
		MemInfo.Type=HexAnsiToulong(it->second);

	return MemInfo;
}


// CMemoryInfoEdit message handlers

