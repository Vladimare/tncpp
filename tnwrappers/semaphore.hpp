#ifndef _SEMAPHORE_HPP_
#define _SEMAPHORE_HPP_

#include "tn.h"


/* семафор, обертка к семафору TNKernel */
class semaphore
{
public:
  semaphore(int startval = 1, int maxval = 1);
  ~semaphore();

  int acquire(int timeout = TN_WAIT_INFINITE);
  int acquirePoll();
  int release();

private:
  TN_SEM sem;
};

#endif /*_SEMAPHORE_HPP_*/
