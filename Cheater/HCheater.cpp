//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdexcept>
USEFORM("Main.cpp", Form1);
USEFORM("..\Library\ProcessChoiceFrame.cpp", ProcessChoice1);
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
        catch(std::exception &exception)
        {
            Application->MessageBox( exception.what(), "std::exception");
        }

        return 0;
}
//---------------------------------------------------------------------------

