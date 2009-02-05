#ifndef LIBRARY_CWndMessageHandler_h
#define LIBRARY_CWndMessageHandler_h

class CWndMessageHandler
{

public:
  CWndMessageHandler(CWnd* base);
  
private:
CWnd* d_base;
};

#endif