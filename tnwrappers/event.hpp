#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include "tn.h"

/* класс-набор событий, обертка к событию TNKernel */
class eventpack
{
public:
  eventpack(int inimask = 0x00000000);
  ~eventpack();

  int set    (int mask = 0xFFFFFFFF);
  int reset  (int mask = 0xFFFFFFFF);
  int waitAny(int mask = 0xFFFFFFFF, int timeout = TN_WAIT_INFINITE, unsigned int* pattern = 0);
  int waitAll(int mask = 0x00000000, int timeout = TN_WAIT_INFINITE, unsigned int* pattern = 0);
  int waitAnyPoll(int mask = 0xFFFFFFFF, unsigned int* pattern = 0);
  int waitAllPoll(int mask = 0x00000000, unsigned int* pattern = 0);

private:
  TN_EVENT evt;
};

#endif /*_EVENT_HPP_*/
