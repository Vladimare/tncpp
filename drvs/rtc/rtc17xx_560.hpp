#ifndef _RTC17xx_560_HPP_
#define _RTC17xx_560_HPP_

#include "rtc17xx.hpp"
#include "systimer.h"


class rtc17xx_560
  : public rtc17xx
{
public:
  rtc17xx_560(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd);
  ~rtc17xx_560();

  static rtc17xx_560* g_rtc;

private:
  deferred_task einth_binder;

                                      /* отложенная регистрация прерывания таймпульса,  */
                                      /*   для защиты от дребезга                       */
  static void register_einth(void* param);
  static void einth(void* instance);  /* обработчик прерываний таймпульса               */
};

#endif /*_RTC17xx_560_HPP_*/
