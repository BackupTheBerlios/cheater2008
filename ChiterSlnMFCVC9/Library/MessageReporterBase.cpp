#include "StdAfx.h"
#include "MessageReporterBase.h"

MessageReporterBase::MessageReporterBase(void)
{
}

MessageReporterBase::~MessageReporterBase(void)
{
}


class MfcMessageReporter:public MessageReporterBase
{
public:
  virtual void showMessage(const std::string& i_msg,TYPE type) const 
  {
    switch (type)
    {
    case TYPE_EXCEPTION:
      AfxMessageBox( CString(i_msg.c_str()), MB_ICONHAND| MB_ICONSTOP | MB_ICONERROR );
      break;
    case TYPE_WARNING:
      AfxMessageBox( CString(i_msg.c_str()),  MB_ICONEXCLAMATION | MB_ICONWARNING );
      break;
    case TYPE_INFO:
      AfxMessageBox( CString(i_msg.c_str()),   MB_ICONASTERISK | MB_ICONINFORMATION );
      break;

    }


  }
};

const MessageReporterBase& getReporter()
{
  static MfcMessageReporter reporter;
  return reporter;

}
