#include "stdafx.h"

#include "Command.h"

#include <stdexcept>
#include <sstream>
#include <boost/bind.hpp>


  CommandsContainer::CommandsContainer():
  d_maxUsed (0x1000)
  {
    
  }

  UINT CommandsContainer::createCommand(CommandPtr cmd)
  {
    UINT idx = 0;
    if(!d_freeIDs.empty())
    {
      idx = d_freeIDs.back();
      d_freeIDs.pop_back();
    }
    else
    {
      d_maxUsed++;
      idx = d_maxUsed;
    }

    if(0==idx)
    {
      std::stringstream msg;
      msg << "Index overflow idx =="  << 0 << std::endl << std::endl
          << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
      throw std::runtime_error(msg.str());
    }

    if(d_map.find(idx) != d_map.end() )
    {
      std::stringstream msg;
      msg << "Index idx ("  << idx <<") is already present" <<  std::endl << std::endl
          << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
      throw std::runtime_error(msg.str());
    }

    d_map[idx] = cmd;

    return idx;
  }

  bool CommandsContainer::hasCommand(UINT idx) const
  {
    std::map<UINT,CommandPtr>::const_iterator it = d_map.find(idx);

    return (d_map.find(idx) != d_map.end() );
  }

  const Command& CommandsContainer::getCommand(UINT idx) const
  {
    std::map<UINT,CommandPtr>::const_iterator it = d_map.find(idx);

    if(d_map.find(idx) == d_map.end() )
    {
      std::stringstream msg;
      msg << "Command with index "  << idx <<" is not present" <<  std::endl << std::endl
        << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;
      throw std::runtime_error(msg.str());
    }
    return *(it->second);
  }

  void CommandsContainer::removeCommand(UINT idx)
  {
    std::map<UINT,CommandPtr>::iterator it = d_map.find(idx);
    if(it==d_map.end())
    {
      std::stringstream msg;
      msg << "Command idx "  << idx << " does not exist" << std::endl << std::endl
        << " File: " << __FILE__ << std::endl << " Line: " << __LINE__ << std::endl << " Function: " << __FUNCTION__ << std::endl;

      throw std::runtime_error(msg.str());
    }
    d_map.erase( it );
    d_freeIDs.push_back(idx);

  }

  void dymmy(int a)
  {
    a++;
  }

  void test()
  {
    CommandsContainer container;
    boost::bind(dymmy,1)();
    container.createCommand( CommandPtr( new Command(boost::bind(dymmy,1)) ) );
  }
