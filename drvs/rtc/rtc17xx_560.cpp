#include "appirq.h"
#include "core.h"
#include "rtc17xx_560.hpp"
#include "upiobo_vhfmaster_560.hpp"


rtc17xx_560* rtc17xx_560::g_rtc = 0;


rtc17xx_560::rtc17xx_560(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd)
  : rtc17xx(phy, rcd)
{
  g_rtc = this;

                                      /* внешний импульс синхронизации времени          */
                                      /*   (таймпульс с модуля GPS / ГЛОНАСС) приходит  */
                                      /*   на внешнее прерывание EINT2 (нога P2.12)     */
  LPC_PINCON->PINSEL4 &= ~(0x03ul << (P2_12 * 2));
  LPC_PINCON->PINSEL4 |=  P2_12_EINT2;
                                      /* включаем подтяжку                              */
  LPC_PINCON->PINMODE4&= ~(0x03ul << (P2_12 * 2));
  LPC_PINCON->PINMODE4|=  (PULLUP << (P2_12 * 2));

  coreUnregisterIRQ(IRQ0_EINT2);      /* запрещаем прерывание EINT2 перед настройкой    */
  LPC_SC->EXTMODE     |=  0x04;       /* прерывание EINT2 работает по фронту            */
  LPC_SC->EXTPOLAR    |=  0x04;       /* фронт передний                                 */
  LPC_SC->EXTINT      |=  0x04;       /* чистим прерывания                              */

  this->einth_binder.task       = &rtc17xx_560::register_einth;
  this->einth_binder.param      = this;
  this->einth_binder.ticksTotal = 750 / SYSTIMER_PERIOD;
  this->einth_binder.forever    = 0;
  systimerExecLater(&this->einth_binder);
}

rtc17xx_560::~rtc17xx_560()
{
  coreUnregisterIRQ(IRQ0_EINT2);      /* снимаем прерывание с регистрации               */
                                      /* ножку в исходное состояние                     */
  LPC_PINCON->PINSEL4 &= ~(0x03ul << (P2_12 * 2));
  LPC_PINCON->PINMODE4&= ~(0x03ul << (P2_12 * 2));
}

void rtc17xx_560::register_einth(void* param)
{
  IRQ_DATA irqd = {rtc17xx_560::einth, (rtc17xx_560*)param};
  coreRegisterIRQ(IRQ0_EINT2, IRQPRIO_EINT2, &irqd);
}

void rtc17xx_560::einth(void* instance)
{
  LPC_SC->EXTINT |= 0x04;             /* чистим прерывания                              */

                                      /* обнуляем миллисекунды                          */
  ((rtc17xx_560*)instance)->syncPhase();

  if(upb_vhfmaster_560::g_vhf560)
  {                                   /* выдаем синхроимпульс в УКВ-канал               */
    unsigned long utc = 0;
    int err = ((rtc17xx_560*)instance)->getRTC(&utc, 0);

    upb_vhfmaster_560::g_vhf560->syncpulse(utc, err);
  }
}
