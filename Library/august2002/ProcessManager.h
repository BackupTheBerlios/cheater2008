//---------------------------------------------------------------------------
#ifndef ProcessManagerH
#define ProcessManagerH
#include <iostream>
#include <sstream>
#include <fstream>
class ProcessStreamBuf: public std::streambuf
{
    public:
        // Public constructor
        ProcessStreamBuf();
        virtual ~ProcessStreamBuf();
        int  seek(int Offset, Word Origin);
        void setStart(DWORD value);
        DWORD getStart();
        void setSize(DWORD value);
        DWORD getSize();
        bool  createProcess(const std::string& i_process);
        void  openProcess(DWORD id);
        void  setMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo);
        MEMORY_BASIC_INFORMATION  getMemoryInfo(DWORD Pos);        
    private:
        PROCESS_INFORMATION ProcessInformation;
        SYSTEM_INFO SystemInfo;
        DWORD FStart;
        DWORD FPosition;
        DWORD FSize;
        void  protect(DWORD pos, DWORD size,DWORD flag=PAGE_READWRITE);
        int  rawWrite(const void * Buffer, int Count);
        int  rawRead(void * Buffer, int Count);
    protected:
        /*std::streambuf interface*/
        virtual void imbue ( const std::locale & loc );
        virtual std::streambuf * setbuf ( char * s, std::streamsize n );
        virtual std::streampos seekoff ( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );
        virtual std::streampos seekpos ( std::streampos sp, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );
        virtual int sync ( );
        virtual std::streamsize showmanyc ( );
        virtual std::streamsize xsgetn ( char * s, std::streamsize n );
        virtual int underflow ( );
        virtual int uflow ( );
        virtual int pbackfail ( int c = EOF );
        virtual std::streamsize xsputn ( const char * s, std::streamsize n );
        virtual int overflow ( int c = EOF );
};
//---------------------------------------------------------------------------
class TProcessManager:public TStream
{
private:
        void __fastcall SetStart(DWORD value); 	// pointer to PROCESS_INFORMATION
        DWORD __fastcall GetStart(); 	// pointer to PROCESS_INFORMATION
        std::iostream d_processMemoryStream;
        ProcessStreamBuf d_processMemoryStreamBuffer;
public:
        __fastcall ~TProcessManager();
        void  createProcess(const std::string& i_process);
        void  openProcess(DWORD id);
        void  setMemoryInfo(MEMORY_BASIC_INFORMATION MemInfo);
        MEMORY_BASIC_INFORMATION  getMemoryInfo(DWORD Pos);                   
        std::iostream& getStreamInterface();
        __fastcall TProcessManager(void);
        virtual int __fastcall Seek(int Offset, Word Origin);
        virtual int __fastcall Write(const void * Buffer, int Count);
        virtual int __fastcall Read(void * Buffer, int Count);
//        void __fastcall Assign(TProcessManager* value);
        __property DWORD Start  = { read=GetStart, write=SetStart };
protected:
        virtual void __fastcall SetSize(DWORD NewSize);
        };
#endif
