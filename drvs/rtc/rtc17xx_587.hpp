#ifndef _RTC17xx_587_HPP_
#define _RTC17xx_587_HPP_

#include "rtc17xx.hpp"


class rtc17xx_587
  : public rtc17xx
{
public:
  rtc17xx_587(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd);

private:
                                      /* обработчик прерываний таймпульса               */
  static void timePulser(void* instance);
};

#endif /*_RTC17xx_HPP_*/
