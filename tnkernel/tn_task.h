#ifndef _TN_TASK_H_
#define _TN_TASK_H_

#define TSK_WAIT_REASON_SEM 0
#define TSK_WAIT_REASON_WFIXMEM 1

typedef int BOOL;

enum {
    TN_TASK_TIMER,
    TN_TASK_IDLE
};

#define TSK_WAIT_REASON_SLEEP 0
#define TSK_STATE_RUNNABLE 1
#define MAX_TIME_SLICE 10

typedef struct _TN_TCB {
    int id_task;
    int task_wait_rc;
    int tick_count;
    int priority;
    int tslice_count;
    int task_wait_reason;
    int task_state;
    int stk_size;
    unsigned int *stk_start;
    CDLL_QUEUE *pwait_queue;
    CDLL_QUEUE task_queue;
    void *data_elem;
    void *user_data;
} TN_TCB;

extern TN_TCB * tn_curr_run_task;

void tn_task_exit(int attr);
int tn_task_terminate(TN_TCB *task);
int tn_task_activate(TN_TCB *task);
int tn_task_iactivate(TN_TCB *task);
int tn_task_suspend(TN_TCB *task);
int tn_task_resume(TN_TCB *task);
int tn_task_wakeup(TN_TCB *task);
int tn_task_iwakeup(TN_TCB *task);
int tn_task_release_wait(TN_TCB *task);
int tn_task_irelease_wait(TN_TCB *task);
int tn_task_sleep(unsigned int timeout);
int tn_task_change_priority(TN_TCB *task, int priority);
void tn_task_create(TN_TCB *task,         //-- task TCB
                  void (*task_func)(void *),              //-- task function
                  int priority,              //-- task priority
                  unsigned int *task_stack,
                  int stack_size,             //-- task stack size (in int,not bytes)
                  void *param,                           //-- task function parameter
                  int option);
void tn_task_delete(TN_TCB *task);

TN_TCB *get_task_by_tsk_queue(CDLL_QUEUE *que);
BOOL task_wait_complete(TN_TCB *task);

#endif