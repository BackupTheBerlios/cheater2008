#ifndef Library_Command_h
#define Library_Command_h
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <map>
#include <vector>
#include "august2002Fwd.h"
typedef boost::function<void (void)> Command;
typedef boost::shared_ptr<Command> CommandPtr;

class AUGUST2002_EXPORT Container
{
public:
  Container();
  UINT createCommand(CommandPtr);
  bool hasCommand(UINT idx) const;
  const Command& getCommand(UINT) const; //throw
  void removeCommand(UINT); //throw
private:
  std::map<UINT,CommandPtr> d_map;
  UINT d_maxUsed;
  std::vector<UINT> d_freeIDs;
};

#endif