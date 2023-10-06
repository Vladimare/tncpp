#include "gps_587.hpp"
#include "lpc17xx_defs.h"


gps_587::gps_587()
{
  this->pinSetup();
}

gps_587::~gps_587()
{
  this->pinSetup(true);
}

void gps_587::pinSetup(bool setDefault)
{
  LPC_PINCON->PINSEL3 &= ~(0x03ul << ((P1_27 - 16) * 2));
  LPC_GPIO1->FIODIR   &= ~(0x01ul <<   P1_27);

  if(!setDefault)
  {
    LPC_PINCON->PINSEL3 |=  P1_27_GPIO;
    LPC_GPIO1->FIODIR   |=  0x01ul << P1_27;
    LPC_GPIO1->FIOCLR    =  0x01ul << P1_27;
  }
}
