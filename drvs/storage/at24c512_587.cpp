#include <string.h>

#include "at24c512_587.hpp"
#include "lpc17xx_defs.h"


at24c512_587::at24c512_587(extstorageCreationDisposition* escd)
 : at24c512(escd)
{
  //********* P3.26 на выход "0" ************
  LPC_PINCON->PINSEL7 &= ~(3ul << ((P3_26 - 16) * 2));
  LPC_PINCON->PINMODE7 |= (2ul << ((P3_26 - 16) * 2));
  LPC_GPIO3->FIODIR |= (1ul << P3_26);
  LPC_GPIO3->FIOCLR = 1ul << P3_26;
  //*****************************************
}

at24c512_587::~at24c512_587()
{
  LPC_PINCON->PINSEL7 &= ~(3ul << ((P3_26 - 16) * 2));
}

void at24c512_587::wp_enable(bool enable)
{
  if(enable)
    LPC_GPIO3->FIOSET = 1ul << P3_26;
  else
    LPC_GPIO3->FIOCLR = 1ul << P3_26;
}

