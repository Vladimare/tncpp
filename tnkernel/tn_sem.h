#ifndef _TN_SEM_H_
#define _TN_SEM_H_

typedef struct _TN_SEM {
    int id_sem;
    int count;
    int max_count;
    CDLL_QUEUE wait_queue;
} TN_SEM;

void tn_sem_create(TN_SEM *sem, int startval, int maxval);
void tn_sem_delete(TN_SEM *sem);
int tn_sem_acquire(TN_SEM *sem, int timeout);
int tn_sem_ipolling(TN_SEM *sem);
int tn_sem_polling(TN_SEM *sem);
int tn_sem_isignal(TN_SEM *sem);
int tn_sem_signal(TN_SEM *sem);

#endif