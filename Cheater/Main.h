//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "searcher.h"
#include "processmanager.h"
//#include "HexEdit.h"
//#include "HexControl.h"
//#include "EditMemory.h"
#include "inputdlg.h"
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "psapi.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include "ProcessChoiceFrame.h"
#include "SearcherProperties.h"
#include "ChitThreadProperties.h"
#include "ChitThread.h"
#include "StreamEdit.h"
#include "ansi_stream.h"
#include "ansilib.h"
#include "InputGroupBox.h"
#include "MemoryInfoEdit.h"
#include "CountEdit.h"
#include <Sockets.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TStatusBar *StatusBar1;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Process1;
        TMenuItem *OpenFileMenuItem;
        TMenuItem *ChitOptions;
        TMenuItem *CreateProcessMenuItem;
        TMenuItem *SetStartMenuItem;
        TMenuItem *SetSizeMenuItem;
        TTabSheet *TabSheet2;
        TButton *Button1;

        TListBox *ListBox;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TButton *Button2;
        TButton *Button3;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TProcessChoice1 *ProcessChoice11;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TTabSheet *TabSheet5;
        TButton *Button7;
        TStreamEdit *EditMemoryFrame;
        TChitThreadProperties *ChitProperties;
        TMenuItem *Suspend;
        TMenuItem *Interval;
        TMenuItem *N1;
        TMenuItem *AddSearcherMenuItem;
        TMemoryInfoEdit *MemoryInfo;
        TInputGroupBox *MemPtr;
        TButton *Set;
        TButton *Reload;
        TMemo *Memo1;
        TTcpServer *TcpServer1;
        void __fastcall OpenFileMenuItemClick(TObject *Sender);

        void __fastcall FormCreate(TObject *Sender);
        void __fastcall CreateProcessMenuItemClick(TObject *Sender);
        void __fastcall SetStartMenuItemClick(TObject *Sender);
        void __fastcall SetSizeMenuItemClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ListBoxClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall SuspendClick(TObject *Sender);
        void __fastcall IntervalClick(TObject *Sender);
        void __fastcall AddSearcherMenuItemClick(TObject *Sender);
        void __fastcall ChitOptionsClick(TObject *Sender);
        void __fastcall SetClick(TObject *Sender);
        void __fastcall ReloadClick(TObject *Sender);
        void __fastcall ServerSocket1ClientRead(TObject *Sender,
          TCustomWinSocket *Socket);

private:
        DWORD idProcess[50];
        int processNum;
        TFileStream* file;
        int pos;
        bool IsProcessKeyDown;
void __fastcall TcpServer1OnRecieveEvent(TObject* Sender, char* buf, int & DataLen);
        TProcessManager* Process;
        TInputForm* InputDialog;	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        __fastcall ~TForm1();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
