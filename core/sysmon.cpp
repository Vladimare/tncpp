#include <rt_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apptask.h"
#include "lpc17xx_defs.h"
#include "syslog.hpp"
#include "sysmon.hpp"


const threadCreationDisposition sysmon::tcd =
{
  APPPRIO_SYSMON,                     /*  int priority                                  */
  APPSTACK_SYSMON,                    /*  int sp_size                                   */
  0,                                  /*  int options                                   */
  0,                                  /*  unsigned int* sp                              */
  "sysmon"                            /*  const char* name                              */
};


sysmon::sysmon(const char* name, unsigned char baseverb)
  : thread(sysmon::tcd)
  , verbosity(baseverb)
  , prjname(name)
{
  this->activate();
}

sysmon::~sysmon()
{
  this->terminate();
}

void sysmon::run()
{
  unsigned char verb = verbosity;
  char control = '\0';

  if(!syslog::g_log)                  /* если не крутится системный лог, то монитору    */
    this->exit(0);                    /*   тоже делать нечего                           */

  syslog::g_log->changeVerbosity(verb);

  syslog::g_log->write(verb, "sysmon: started\r\n");
  syslog::g_log->write(verb, "sysmon: hit 'w'/'s' to increase/decrease log verbosity\r\n");
  syslog::g_log->write(verb, "sysmon: task monitor appears on 0 verbosity level\r\n");
  syslog::g_log->write(verb, "sysmon: current verbosity is %d (%s)\r\n", verb, syslog::log_level_names[verb]);

  for(;;)
  {
    if(verb == LOG_LEVEL_DISABLED)
      this->refresh();

    if(syslog::g_log->read((unsigned char*)&control) > 0)
    {
      switch(control)
      {
      case 'w':
      case 'W':
        if(verb < LOG_LEVEL_DBG_ALL)
        {
          verb++;
          syslog::g_log->changeVerbosity(verb);
        }
        syslog::g_log->write(verb, "sysmon: current verbosity is %d (%s)\n\r\n\r", verb, syslog::log_level_names[verb]);
        break;

      case 's':
      case 'S':
        if(verb)
        {
          verb--;
          syslog::g_log->changeVerbosity(verb);
        }
        if(!verb)
        {
          this->refresh_counter = 0;
          syslog::g_log->write(LOG_LEVEL_DISABLED, TERMANSI_ERASE TERMANSI_SET_CURSOR(0, 0));
        }
        else
          syslog::g_log->write(verb, "sysmon: current verbosity is %d (%s)\n\r\n\r", verb, syslog::log_level_names[verb]);
        break;
      }
    }
  }
}

void sysmon::refresh()
{
  CDLL_QUEUE* tasklist= 0;
  TN_TCB*     task    = 0;
  thread*     pthr    = 0;
  char buf[90] = {0};
  int cntr = 0;

  syslog::g_log->write(LOG_LEVEL_DISABLED, TERMANSI_SET_CURSOR(0, 0));

  this->refresh_counter++;

  if(this->prjname)
  {
    _snprintf(buf, sizeof(buf), "%s: (%d)\n\r\0", this->prjname, this->refresh_counter);
    syslog::g_log->write(LOG_LEVEL_DISABLED, buf);
  }
  syslog::g_log->write(LOG_LEVEL_DISABLED, "--------------------------------------------------\n\r");
  syslog::g_log->write(LOG_LEVEL_DISABLED, "------------------ task monitor ------------------\n\r");
  syslog::g_log->write(LOG_LEVEL_DISABLED, "--------------------------------------------------\n\r");
  syslog::g_log->write(LOG_LEVEL_DISABLED, "   TN_TCB   | task name | stack used / total | P  \n\r");
  syslog::g_log->write(LOG_LEVEL_DISABLED, "------------|-----------|--------------------|----\n\r");
                                       //012345678901234567890123456789012345678901234567890123 4
                                       //0         1         2         3         4         5

  for(tasklist = tn_create_queue.next; tasklist != &tn_create_queue; tasklist = tasklist->next)
  {
    task = CONTAINING_RECORD(tasklist, TN_TCB, create_queue);
    pthr = (thread*)task->user_data;

    if(pthr)
      cntr = _snprintf(buf, sizeof(buf), " 0x%08X | %s", task, pthr->taskname());
    else
      cntr = _snprintf(buf, sizeof(buf), " 0x%08X | ", task);

    if(cntr < 24)
      memset(buf + cntr, ' ', 24 - cntr);

    _snprintf((buf + 24), sizeof(buf) - 24,
              "|    %04d / %04d     | %02d\n\r",
              thread::getStackUsage(task),
              task->stk_size,
              task->base_priority);

    syslog::g_log->write(LOG_LEVEL_DISABLED, buf);
  }

  syslog::g_log->write(LOG_LEVEL_DISABLED, "--------------------------------------------------\n\r");

  __initial_stackheap shinfo = __user_initial_stackheap(0, 0, 0, 0);

  syslog::g_log->write(LOG_LEVEL_DISABLED, "current MSP position is %d (%d max)\n\r",
                                           shinfo.stack_base - __get_MSP(),
                                           shinfo.stack_base - shinfo.stack_limit);

  syslog::g_log->write(LOG_LEVEL_DISABLED, "heap size is %d,\n\r", shinfo.heap_limit - shinfo.heap_base);
  __heapstats((__heapprt)g_writelog, (void*)LOG_LEVEL_DISABLED);

  syslog::g_log->write(LOG_LEVEL_DISABLED, "--------------------------------------------------\n\r");
}

