#ifndef _SYSTIMER_H_
#define _SYSTIMER_H_


#include "core.h"

#ifdef TNKERNEL_PORT_CORTEXM3
# include "tn.h"

typedef struct tagDeferredTask
{
  CDLL_QUEUE  task_queue;             /* очередь                                        */
  CALLBACK1   task;                   /* задача                                         */
  void*       param;                  /* параметр задачи                                */
  unsigned long ticksTotal;           /* период вызова, в тиках таймера                 */
  unsigned long ticksToRun;           /* оставшееся до старта количество тиков          */
  unsigned char forever:1;            /* признак непрерывности вызова                   */
}deferred_task;

#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern void           systimerInit      (void);
extern unsigned long  systimerUptime    (unsigned long* hi);
extern unsigned long  systimerUptimeSec (void);
extern void           delay             (unsigned long msec);

#ifdef TNKERNEL_PORT_CORTEXM3
extern void systimerExecLater (deferred_task* task);
extern void systimerCancelExec(deferred_task* task);
#endif

#ifdef __cplusplus
}
#endif

#endif /*_SYSTIMER_H_*/
