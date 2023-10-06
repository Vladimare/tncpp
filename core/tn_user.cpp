#include "apptask.h"
#include "lpc17xx_defs.h"
#include "tn.h"


void tn_cpu_int_enable()
{
  tn_arm_enable_interrupts();
}

int tn_inside_int()
{
  if(SCB->ICSR & VECTACTIVE)
    return 1;

  return 0;
}

extern void app_init();

static const int app_slices[TN_NUM_PRIORITY - 2] = APP_SLICES;

void tn_app_init()
{
  for(int i = 1; i < (TN_NUM_PRIORITY - 1); i++)
    tn_sys_tslice_ticks(i, app_slices[i]);

  app_init();
}
