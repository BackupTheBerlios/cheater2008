// ChiterApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ChiterApp.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ChiterAppDoc.h"
#include "ChiterAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChiterAppApp

BEGIN_MESSAGE_MAP(CChiterAppApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CChiterAppApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CChiterAppApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CChiterAppApp construction

CChiterAppApp::CChiterAppApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CChiterAppApp object

CChiterAppApp theApp;


// CChiterAppApp initialization

BOOL CChiterAppApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ChiterAppTYPE,
		RUNTIME_CLASS(CChiterAppDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CChiterAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CChiterAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CChiterAppApp::openFile(const std::string& i_name)
{

  boost::shared_ptr<std::iostream> file = boost::shared_ptr<std::iostream>(new std::fstream(i_name.c_str(),std::ios_base::in | std::ios_base::out));
  if(!file)
  {
    std::stringstream msg;
    msg << "Cant open file "  << i_name;
    throw std::runtime_error(msg.str());
  }
  else 
  {
    if(! (*(file.get())) )
    {
      std::stringstream msg;
      msg << "Failed to  open file "  << i_name << " for read/write";
      throw std::runtime_error(msg.str());
    }
  }
  //---------------------------------------------------------------------------
  POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
  CDocTemplate* pTemplate =  m_pDocManager->GetNextDocTemplate(pos);


  CString lpszPathName = CString("");
  BOOL bMakeVisible = TRUE;
  CDocument* pDocument = pTemplate->CreateNewDocument();
  if (pDocument == NULL)
  {
    TRACE(traceAppMsg, 0, "CDocTemplate::CreateNewDocument returned NULL.\n");
    AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
    return;
  }
  ASSERT_VALID(pDocument);

  BOOL bAutoDelete = pDocument->m_bAutoDelete;
  pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
  CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, NULL);
  pDocument->m_bAutoDelete = bAutoDelete;
  if (pFrame == NULL)
  {
    AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
    delete pDocument;       // explicit delete on error
    return;
  }
  ASSERT_VALID(pFrame);

  if (lpszPathName.GetLength() == 0)
  {
    // create a new document - with default document name
    pTemplate->SetDefaultTitle(pDocument);

    // avoid creating temporary compound file when starting up invisible
    if (!bMakeVisible)
      pDocument->m_bEmbedded = TRUE;

    if (!pDocument->OnNewDocument())
    {
      // user has be alerted to what failed in OnNewDocument
      TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
      pFrame->DestroyWindow();
      return;
    }
  }
  else
  {
    // open an existing document
    CWaitCursor wait;
    if (!pDocument->OnOpenDocument(lpszPathName))
    {
      // user has be alerted to what failed in OnOpenDocument
      TRACE(traceAppMsg, 0, "CDocument::OnOpenDocument returned FALSE.\n");
      pFrame->DestroyWindow();
      return;
    }
    pDocument->SetPathName(lpszPathName);
  }
  pTemplate->InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
  //---------------------------------------------------------------------------
  CChiterAppView* view = dynamic_cast<CChiterAppView*>  (pFrame->GetActiveView());
  if(!view)
  {
    std::stringstream msg;
    msg << "Unknown view ";
    throw std::runtime_error(msg.str());
  }
  view->setStream(file);
  CString frameTitle ("File: ");
  frameTitle+= CString(i_name.c_str());
  pFrame->SetWindowText(frameTitle);
}





void CChiterAppApp::OnFileNew()
{
  CFileDialog dlg(TRUE);
  if (IDOK == dlg.DoModal())
  {
    CString name = dlg.GetFileName();
    CString folder = dlg.GetFolderPath();
    CString ext = dlg.GetFileExt();
    CString title = dlg.GetFileTitle();
    CString fullpath = folder + CString("\\") + name;
    std::string fileName = std::string(CT2CA(fullpath));
    openFile(fileName);
  }

 //return pDocument;
  //CWinApp::OnFileNew();
}


// CChiterAppApp message handlers

