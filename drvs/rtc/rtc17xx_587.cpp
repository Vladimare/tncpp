#include "appirq.h"
#include "core.h"
#include "rtc17xx_587.hpp"
#include "upiobo_vhfslave_587.hpp"


rtc17xx_587::rtc17xx_587(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd)
  : rtc17xx(phy, rcd)
{
                                      /* ножка таймпульса - вход без подтяжки, и с      */
                                      /*   прерыванием по спадающему фронту             */
  LPC_PINCON->PINSEL0 &= ~(0x03ul << (P0_5 * 2));
  LPC_PINCON->PINSEL0 |=  P0_5_GPIO;
  LPC_PINCON->PINMODE0&= ~(0x03ul << (P0_5 * 2));
  LPC_PINCON->PINMODE0|=  (NOPULL << (P0_5 * 2));
  LPC_GPIO0->FIODIR   &= ~(1ul << P0_5);

  LPC_GPIOINT->IO0IntEnF |= (1ul << P0_5);

  IRQ_DATA irqd = {rtc17xx_587::timePulser, this};
  coreRegisterIRQ(IRQFAKE_GPIOINT, IRQPRIO_GPIOINT, &irqd);

#if 0
#warning !
                                      /* отладочная нога BOOT_CPU                       */
  LPC_PINCON->PINSEL4 &= ~(0x03ul << (P2_10 * 2));
  LPC_PINCON->PINSEL4 |=  P2_10_GPIO;
  LPC_GPIO2->FIODIR   |=  (1ul << P2_10);
  LPC_GPIO2->FIOSET    =  (1ul << P2_10);
#endif
}

void rtc17xx_587::timePulser(void* instance)
{
                                      /* сбрасываем прерывание таймпульса               */
  LPC_GPIOINT->IO0IntClr = (0x01ul << P0_5);

                                      /* обнуляем миллисекунды                          */
  ((rtc17xx_587*)instance)->syncPhase();

  if(upb_vhfslave_587::g_vhf587)
  {
    unsigned long utc = 0;
    int err = ((rtc17xx_587*)instance)->getRTC(&utc, 0);

    upb_vhfslave_587::g_vhf587->syncpulse(utc, err);
  }
}
