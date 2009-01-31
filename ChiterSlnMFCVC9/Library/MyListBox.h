#ifndef LIBRARY_MyListBox_h
#define LIBRARY_MyListBox_h

class MyListBox : public CListBox
{
    	DECLARE_DYNAMIC(MyListBox)
public:
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
protected:
    DECLARE_MESSAGE_MAP()
};

#endif