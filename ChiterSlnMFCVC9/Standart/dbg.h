//---------------------------------------------------------------------------

#ifndef STANDADRT_DBG_H
#define STANDADRT_DBG_H
#include <sstream>
#include <string>
#include "Standart/StandartFwd.h"
//---------------------------------------------------------------------------
STANDART_EXPORT_FUN(void) dprintf(const std::string& );

#define DPRINTF(m)\
  { \
  std::stringstream msg; \
  msg << __FILE__ << "(" << __LINE__ << ") " << __FUNCTION__ << std::endl; \
  msg << m; \
  dprintf(msg.str()); \
  }  

#endif

