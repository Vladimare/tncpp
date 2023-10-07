#ifndef _TN_H_
#define _TN_H_

#ifdef __cplusplus
extern "C"  {
#endif

#include "types.h"

typedef CIRC_DLL CDLL_QUEUE;

#include "tn_port.h"
#include "tn_task.h"
#include "tn_event.h"
#include "tn_mutex.h"
#include "tn_sem.h"

#define TRUE 1
#define FALSE 0
#define NULL (void *)0
#define TERR_OVERFLOW -1
#define TERR_UNDERFLOW -2
#define TERR_WRONG_PARAM -3
#define TERR_TIMEOUT -4
#define TERR_NOEXS -5
#define TERR_DLT -6
#define TERR_WCONTEXT -7
#define TERR_NO_ERR 0

#define TN_ID_SEMAPHORE 0
#define TN_ID_FSMEMORYPOOL 1

#define NO_TIME_SLICE 1
#define TN_ST_STATE_NOT_RUN 1

typedef struct _TN_DQUE {
    int num_entries;
    int tail_cnt;
    int header_cnt;
    void *data_fifo[10];
} TN_DQUE;

typedef struct _TN_FMP {
    int id_fmp;
    int fblkcnt;
    int num_blocks;
    int block_size;
    void *free_list;
    void *start_addr;
    CDLL_QUEUE wait_queue;
} TN_FMP;

void tn_start_system(void);
int tn_sys_tslice_ticks(int priority,int value);
void tn_tick_int_processing();

#ifdef __cplusplus
}
#endif

#endif
