//---------------------------------------------------------------------------
#ifndef MemoryInfoEditH
#define MemoryInfoEditH
//---------------------------------------------------------------------------
#include "Standart/ansi_stream.h"
#include "august2002Fwd.h"
#include "resource.h"	

class AUGUST2002_EXPORT CMemoryInfoEdit : public CDialog
{
	DECLARE_DYNAMIC(CMemoryInfoEdit)
	
public:
	CMemoryInfoEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMemoryInfoEdit();

	// Dialog Data
	enum { IDD = IDD_MEMORYINFOEDIT };
	
	void Create( CWnd* pParentWnd );

	void  SetMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo);
	MEMORY_BASIC_INFORMATION  GetMemoryInfo(void);

protected:
    HGLOBAL initDialog( HINSTANCE hinst, LPCTSTR lpszTemplateName );
    HMODULE d_hModule;
    HGLOBAL d_hDialogInit; 

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()
public:

};

//---------------------------------------------------------------------------
#endif

