#include "appirq.h"
#include "cmx469_560.hpp"
#include "core.h"
#include "lpc17xx_defs.h"


cmx469_560::cmx469_560(stat_events* se)
  :  cmx469(se)
{
  this->pinSetup();
}

cmx469_560::~cmx469_560()
{
  this->pinSetup(true);
}

void cmx469_560::pinSetup(bool setDefault)
{
                                                        /* чистим ножки                     */
  LPC_PINCON->PINSEL0 &= ~ (0x03ul << ( P0_15       * 2));
  LPC_PINCON->PINSEL1 &= ~((0x03ul << ((P0_16 - 16) * 2)) |
                           (0x03ul << ((P0_20 - 16) * 2)) |
                           (0x03ul << ((P0_18 - 16) * 2)) |
                           (0x03ul << ((P0_19 - 16) * 2)));
  LPC_PINCON->PINSEL2 &= ~ (0x03ul << ( P1_8        * 2));
  LPC_PINCON->PINSEL4 &= ~((0x03ul << ( P2_6        * 2)) |
                           (0x03ul << ( P2_8        * 2)) |
                           (0x03ul << ( P2_11       * 2)) |
                           (0x03ul << ( P2_13       * 2)));
  LPC_PINCON->PINMODE4&= ~ (NOPULL << ( P2_11       * 2));

  if(!setDefault)
  {
    LPC_PINCON->PINSEL0 |=  P0_15_GPIO;                 /* нога TXD_CPU (P0.20)- выход TX   */
    LPC_PINCON->PINSEL1 |=  P0_16_GPIO  | P0_18_GPIO |  /* нога TXSYNC_CPU (P2.13)- вход    */
                            P0_19_GPIO  | P0_20_GPIO;   /*   прерывани€ EINT3, срабатывание */
    LPC_PINCON->PINSEL2 |=  P1_8_GPIO;                  /*   по спадающему фронту           */
    LPC_PINCON->PINSEL4 |=  P2_6_GPIO   | P2_8_GPIO  |  /* нога TXE_CPU (P0.18)- выход      */
                            P2_11_EINT1 | P2_13_EINT3;  /*   разрешени€ Tx                  */
    LPC_PINCON->PINMODE4|= (PULLUP  << (P2_11 * 2));    /* нога RXSYNC_CPU (P2.11) - вход   */
    LPC_GPIO0->FIODIR &= ~(0x01ul << P0_15);            /*   прерывани€ EINT1, срабатывание */
    LPC_GPIO0->FIODIR |=  (0x01ul << P0_16) |           /*   по спадающему фронту           */
                          (0x01ul << P0_18) |           /* нога CLD_CPU (P0.15) - вход RX   */
                          (0x01ul << P0_19) |           /* нога RXE_CPU (P0.19) - выход     */
                          (0x01ul << P0_20);            /*   разрешени€ Rx                  */
    LPC_GPIO0->FIOSET  =  (0x01ul << P0_16) |           /* нога 4800_CPU (P2.8) - выход     */
                          (0x01ul << P0_18) |           /*   выбора скорости 4800бод в      */
                          (0x01ul << P0_19) |           /*   радиоканале, всегда 1          */
                          (0x01ul << P0_20);            /* нога 1200/2400_CPU (P2.6) - выход*/
    LPC_GPIO1->FIODIR &= ~(0x01ul << P1_8);             /*   выбора скорости 4800бод в      */
    LPC_GPIO2->FIODIR |=  (0x01ul << P2_6)  |           /*   радиоканале, всегда 0          */
                          (0x01ul << P2_8);             /* нога CAR_DETECT (P1.8) - вход    */
    LPC_GPIO2->FIOSET  =  (0x01ul << P2_8);             /*   CARRIER DETECT, получаемый с   */
    LPC_GPIO2->FIOCLR  =  (0x01ul << P2_6);             /*   радиомикросхемы                */
                                                        /* нога CR_CPU (P0.16) - выход      */
                                                        /*   выбора кварца радиомикросхемы  */
                                                        /*   всегда 1 (4.032ћ√ц)            */
                                                        
  }

  coreUnregisterIRQ(IRQ0_EINT1);      /* снимаем прерывани€ с регистрации               */
  coreUnregisterIRQ(IRQ0_EINT3);

  if(!setDefault)
  {
    IRQ_DATA irq;

    LPC_SC->EXTMODE   |=  (0x02 | 0x08);
    LPC_SC->EXTPOLAR  &= ~(0x02 | 0x08);
    LPC_SC->EXTINT    |=  (0x02 | 0x08);

    irq.h        = cmx469_560::rxSyncIrq;
    irq.instance = this;
    coreRegisterIRQ(IRQ0_EINT1, IRQPRIO_EINT1, &irq);

    irq.h        = cmx469_560::txSyncIrq;
    irq.instance = this;
    coreRegisterIRQ(IRQ0_EINT3, IRQPRIO_EINT3, &irq);
  }
}

void cmx469_560::rxEnable(unsigned char enable)
{
  if(enable)
    LPC_GPIO0->FIOSET = (0x01ul << 19);
  else
    LPC_GPIO0->FIOCLR = (0x01ul << 19);
}

void cmx469_560::txEnable(unsigned char enable)
{
  if(enable)
    LPC_GPIO0->FIOCLR = (0x01ul << 18);
  else
    LPC_GPIO0->FIOSET = (0x01ul << 18);
}

unsigned char cmx469_560::isCarrierDetected()
{
  return (LPC_GPIO1->FIOPIN & (0x01ul << P1_8)) ? 1 : 0;
}

void cmx469_560::txbit(unsigned int bit)
{
  if(bit)
    LPC_GPIO0->FIOSET = (0x01ul << 20);
  else
    LPC_GPIO0->FIOCLR = (0x01ul << 20);
}

unsigned char cmx469_560::rxbit()
{
  return (LPC_GPIO0->FIOPIN & (0x01ul << 15)) ? 1 : 0;
}

void cmx469_560::txSyncIrq(void* instance)
{
  cmx469_560* _this = (cmx469_560*)instance;

  _this->txSync();

  LPC_SC->EXTINT |= 0x08;
}

void cmx469_560::rxSyncIrq(void* instance)
{
  cmx469_560* _this = (cmx469_560*)instance;

  _this->rxSync();

  LPC_SC->EXTINT |= 0x02;
}
