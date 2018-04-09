#include "gm3x0_560.hpp"


gm3x0_560::gm3x0_560(timer* t, unsigned long pclk)
  : gm3x0(t, pclk)
{
  LPC_PINCON->PINSEL3 &= ~((0x03ul << ((P1_23 - 16) * 2)) | (0x03ul << ((P1_28 - 16) * 2)) |
                           (0x03ul << ((P1_29 - 16) * 2)));
  LPC_PINCON->PINSEL3 |= P1_23_GPIO | P1_28_GPIO | P1_29_GPIO;
  LPC_PINCON->PINMODE3&= ~(0x03ul <<  ((P1_29 - 16) * 2));
  LPC_PINCON->PINMODE3|=  (NOPULL <<  ((P1_29 - 16) * 2));
  LPC_GPIO1->FIODIR   &= ~(0x01ul << P1_29);
  LPC_GPIO1->FIODIR   |=  (0x01ul << P1_23) | (0x01ul << P1_28);

  LPC_PINCON->PINSEL4 &= ~((0x03ul <<  (P2_3 * 2)) | (0x03ul << (P2_4 * 2)) | (0x03ul << (P2_7 * 2)));
  LPC_PINCON->PINSEL4 |= P2_3_GPIO | P2_4_GPIO | P2_7_GPIO;
  LPC_PINCON->PINMODE4&= ~(0x03ul <<  (P2_3 * 2));
  LPC_PINCON->PINMODE4|=  (NOPULL <<  (P2_3 * 2));
  LPC_GPIO2->FIODIR   &= ~(0x01ul <<   P2_3);
  LPC_GPIO2->FIODIR   |=  (0x01ul <<   P2_4) | (0x01ul <<  P2_7);

  LPC_PINCON->PINSEL9 &= ~(0x03ul << ((P4_29 - 16) * 2));
  LPC_PINCON->PINSEL9 |=               P4_29_GPIO;
  LPC_GPIO4->FIODIR   |=  (0x01ul <<   P4_29);

  this->onOffButtonUnblock();
  this->offPower();
  this->deactivateCH0();              /* для разрешения переключения голосовых каналов  *
                                       *   заменить на вызов activateCH0()              */
  this->deactivateCH1();
  this->deactivatePTT();
}

gm3x0_560::~gm3x0_560()
{
  LPC_PINCON->PINSEL3 &= ~((0x03ul << ((P1_23 - 16) * 2)) | (0x03ul << ((P1_28 - 16) * 2)) |
                           (0x03ul << ((P1_29 - 16) * 2)));
  LPC_PINCON->PINMODE3&= ~(0x03ul <<  ((P1_29 - 16) * 2));
  LPC_GPIO1->FIODIR   &= ~((0x01ul <<   P1_23)            | (0x01ul <<   P1_28));

  LPC_PINCON->PINSEL4 &= ~((0x03ul <<  (P2_3 * 2)) | (0x03ul << (P2_4 * 2)) | (0x03ul << (P2_7 * 2)));
  LPC_PINCON->PINMODE4&= ~ (0x03ul <<  (P2_3 * 2));
  LPC_GPIO2->FIODIR   &= ~((0x01ul <<   P2_4) | (0x01ul <<  P2_7));

  LPC_PINCON->PINSEL9 &= ~ (0x03ul << ((P4_29 - 16) * 2));
  LPC_GPIO4->FIODIR   &= ~ (0x01ul <<   P4_29);
}
