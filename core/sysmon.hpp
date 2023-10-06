#ifndef _SYSMON_HPP_
#define _SYSMON_HPP_

#include "termansi.h"
#include "thread.hpp"


class sysmon
  : public thread
{
public:
  sysmon(const char* name = 0, unsigned char baseverb = LOG_LEVEL_DBG_ALL);
  virtual ~sysmon();

protected:
  virtual void run();

private:
  unsigned char verbosity;
  const char*   prjname;

  unsigned long refresh_counter;
  void refresh();

  static const threadCreationDisposition tcd;
};

#endif /*_SYSMON_HPP_*/
