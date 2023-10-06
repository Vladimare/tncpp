#include "mutex.hpp"


mutex::mutex()
{
  this->m.id_mutex = 0;
  tn_mutex_create(&this->m, TN_MUTEX_ATTR_CEILING, 1);
}

mutex::~mutex()
{
  tn_mutex_delete(&this->m);
}

int mutex::lock(int timeout)
{
  return tn_mutex_lock(&this->m, timeout);
}


int mutex::lockPolling()
{
  return tn_mutex_lock_polling(&this->m);
}

int mutex::unlock()
{
  return tn_mutex_unlock(&this->m);
}
