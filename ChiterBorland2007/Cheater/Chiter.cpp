//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Chiter.res");
USEFORM("Main.cpp", Form1);
USELIB("$(BCB)\Lib\PSDK\psapi.lib");
USEFORM("..\Library\ProcessChoiceFrame.cpp", ProcessChoice1); /* TFrame: File Type */
USELIB("..\Library\standart\Library.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
