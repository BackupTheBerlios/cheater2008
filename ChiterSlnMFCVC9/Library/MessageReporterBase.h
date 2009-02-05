#ifndef LIBRARY_MessageReporterBase_h
#define LIBRARY_MessageReporterBase_h
#include <string>
class MessageReporterBase
{
public:
  enum TYPE
  {
    TYPE_INFO,
    TYPE_WARNING,
    TYPE_EXCEPTION,

  };
  virtual void showMessage(const std::string& i_msg,TYPE type) const  = 0;

  MessageReporterBase(void);
  virtual ~MessageReporterBase(void);
};

const MessageReporterBase& getReporter();

#endif //! LIBRARY_MessageReporterBase_h

