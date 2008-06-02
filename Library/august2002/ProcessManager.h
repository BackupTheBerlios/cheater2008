//---------------------------------------------------------------------------

#ifndef ProcessManagerH
#define ProcessManagerH
//---------------------------------------------------------------------------
class TProcessManager:public TStream
{
private:

    PROCESS_INFORMATION ProcessInformation;

    SYSTEM_INFO SystemInfo;
        DWORD FStart;
        DWORD FPosition;
        DWORD FSize;
        void __fastcall SetStart(DWORD value); 	// pointer to PROCESS_INFORMATION



public:
        __fastcall TProcessManager(void);
        virtual int __fastcall Seek(int Offset, Word Origin);
        virtual int __fastcall Write(const void * Buffer, int Count);
        virtual int __fastcall Read(void * Buffer, int Count);
        bool __fastcall CreateProcess(AnsiString FileName);
        void __fastcall OpenProcess(DWORD id);
        void __fastcall Protect(DWORD pos, DWORD size,DWORD flag=PAGE_READWRITE);
        void __fastcall Assign(TProcessManager* value);
        MEMORY_BASIC_INFORMATION __fastcall GetMemoryInfo(DWORD Pos);
        void __fastcall SetMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo);
        __property DWORD Start  = { read=FStart, write=SetStart };
protected:
        virtual void __fastcall SetSize(DWORD NewSize);
        };
#endif



