#include "semaphore.hpp"


semaphore::semaphore(int startval, int maxval)
{
  this->sem.id_sem = 0;
  tn_sem_create(&this->sem, startval, maxval);
}

semaphore::~semaphore()
{
  tn_sem_delete(&this->sem);
}

int semaphore::acquire(int timeout)
{
  return tn_sem_acquire(&this->sem, timeout);
}

int semaphore::acquirePoll()
{
  if(tn_inside_int())
    return tn_sem_ipolling(&this->sem);

  return tn_sem_polling(&this->sem);
}

int semaphore::release()
{
  if(tn_inside_int())
    return tn_sem_isignal(&this->sem);

  return tn_sem_signal(&this->sem);
}
