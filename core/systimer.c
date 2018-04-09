#ifdef LPC17xx_PROJECT
# include <lpc17xx.h>

# include "tn.h"
# include "tn_utils.h"
#endif

#ifdef LPC13xx_PROJECT
# include <lpc13xx.h>
#endif

#include "systimer.h"


volatile unsigned long sys_uptime_lo = 0;   /* младшая половина значения системного времени */
volatile unsigned long sys_uptime_hi = 0;   /* старшая половина значения системного времени */


#ifdef TNKERNEL_PORT_CORTEXM3

CDLL_QUEUE defer_task_queue;                /* очередь отложенных задач                     */

#endif


void systimerInit()
{
  if(SYSTIMER_PERIOD > 1000)
    return;

  SysTick->CTRL   = 0x07;
  SysTick->LOAD   = ((coreCurrentFrequency() / 1000ul) * SYSTIMER_PERIOD) - 1;
  SysTick->VAL    = 0x00000000;             /* clear timer                              */

#ifdef TNKERNEL_PORT_CORTEXM3

  queue_reset(&defer_task_queue);           /* сброс очереди отложенных задач           */

#endif
}

/* returns system up time in ms */
unsigned long systimerUptime(unsigned long* hi)
{
  unsigned long res = 0;

  __disable_irq();

  res = sys_uptime_lo * SYSTIMER_PERIOD;
  if(hi)
    *hi = (sys_uptime_hi * SYSTIMER_PERIOD);

  __enable_irq();

  return res;
}

/* returns system up time in sec */
unsigned long systimerUptimeSec()
{
  unsigned long long ticks = 0;

  __disable_irq();

  ticks = ((unsigned long long)sys_uptime_hi << 32ull) + sys_uptime_lo;

  __enable_irq();

  ticks = ticks / (1000 / SYSTIMER_PERIOD);/* приоритеты операций не менять!                 */

  return (unsigned long)(ticks & 0xFFFFFFFFul);
}

/* system timer handler                                                     *
 * counts for uptime and start tasks, listed in systimer_tasklist structure */
void irqSysTick()
{
#ifdef TNKERNEL_PORT_CORTEXM3

  CDLL_QUEUE* tasklist = 0;
  deferred_task* dt = 0;

#endif

  sys_uptime_lo++;
  if(!sys_uptime_lo)
    sys_uptime_hi++;

#ifdef TNKERNEL_PORT_CORTEXM3

  for(tasklist = defer_task_queue.next; tasklist != &defer_task_queue; tasklist = tasklist->next)
  {
    dt = CONTAINING_RECORD(tasklist, deferred_task, task_queue);
    if(dt->ticksToRun)
      dt->ticksToRun--;
    else
    {
      if(dt->task)
        (*dt->task)(dt->param);

      if(dt->forever)
        dt->ticksToRun = dt->ticksTotal;
      else
        queue_remove_entry(tasklist);
    }
  }

  tn_tick_int_processing();
  tn_int_exit();
#endif
}

/* delay function */
void delay(unsigned long msec)
{
  unsigned long endtick = 0;

  __disable_irq();
  endtick = sys_uptime_lo + (msec / SYSTIMER_PERIOD);
  __enable_irq();

  while(sys_uptime_lo < endtick);
}


#ifdef TNKERNEL_PORT_CORTEXM3

/* принимает описатель задачи, и вставляет его в список отложенных задач */
void systimerExecLater(deferred_task* task)
{
  if(!task->task)
    return;

  queue_add_tail(&defer_task_queue, &(task->task_queue));
  task->ticksToRun = task->ticksTotal;
}

/* принимает описатель задачи, и удаляет его из списка отложенных задач */
void systimerCancelExec(deferred_task* task)
{
  if(queue_contains_entry(&defer_task_queue, &(task->task_queue)))
    queue_remove_entry(&(task->task_queue));
}

#endif
