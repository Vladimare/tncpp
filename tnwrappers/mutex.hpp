#ifndef _MUTEX_HPP_
#define _MUTEX_HPP_

#include "tn.h"


/* обертка к TN_MUTEX */
class mutex
{
public:
  mutex();
  ~mutex();

  int lock(int timeout);
  int lockPolling();
  int unlock();

private:
  TN_MUTEX  m;
};

/* мутекс с автоматической разблокировкой */
class mutexLocker
{
public:
  mutexLocker(mutex* m)
    : _m(m)
  {
    this->_m->lock(TN_WAIT_INFINITE);
  }

  ~mutexLocker()
  {
    this->_m->unlock();
  }

private:
  mutex*  _m;
};

#endif /*_MUTEX_HPP_*/
