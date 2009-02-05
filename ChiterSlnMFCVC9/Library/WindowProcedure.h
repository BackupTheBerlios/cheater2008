#ifndef Library_WindowProcedure_h
#define Library_WindowProcedure_h

typedef LRESULT (CALLBACK* WNDPROC_TYPE) (HWND, UINT, WPARAM, LPARAM); 

LRESULT CALLBACK CustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CWnd& getCWnd(HWND hwnd);

WNDPROC_TYPE replaceWndProc(CWnd& wnd,WNDPROC_TYPE newProc);

#endif //! Library_WindowProcedure_h

