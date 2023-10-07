#ifndef _TN_MUTEX_H_
#define _TN_MUTEX_H_

#define TN_MUTEX_ATTR_CEILING 1

typedef struct _TN_MUTEX {
    int id_mutex;
} TN_MUTEX;

void tn_mutex_create(TN_MUTEX *mutex, int attr, int option);
void tn_mutex_delete(TN_MUTEX *mutex);
int tn_mutex_lock(TN_MUTEX *mutex, int timeout);
int tn_mutex_lock_polling(TN_MUTEX *mutex);
int tn_mutex_unlock(TN_MUTEX *mutex);

#endif