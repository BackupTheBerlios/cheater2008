#include "stdafx.h"

#include "MyListBox.h"

IMPLEMENT_DYNAMIC(MyListBox, CListBox)

BEGIN_MESSAGE_MAP(MyListBox, CListBox)
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void MyListBox::OnRButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CListBox::OnRButtonDown(nFlags, point);
  if (GetOwner())
  {

    CPoint screen(point);
    MapWindowPoints(GetOwner(), &screen,1);
    WPARAM wParam = nFlags;
    LPARAM lParam = MAKELONG(screen.x,screen.y);
    


    ::SendMessage(GetOwner()->m_hWnd,WM_RBUTTONDOWN,wParam,lParam);
  }
}
