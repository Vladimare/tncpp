#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"
#include <stdint.h>
#include <time.h>

#define SYSTIMER_PERIOD   10ul        /* период срабатывания системного таймера, в мс     */


/* структура параметров для функции coreInit() */
typedef __packed struct tagCORE_SETTINGS
{
  unsigned char source;               /* источник частоты                                 *
                                       * для LPC13xx:                                     *
                                       *   nCLKSRC_INTRC  (0) - внутренний RC-генератор   *
                                       *   nCLKSRC_PLLIN  (1) - вход SYS PLL              *
                                       *   nCLKSRC_WDT    (2) - сторожевой таймер         *
                                       *   nCLKSRC_PLLOUT (3) - выход SYS PLL             *
                                       * для LPC17xx:                                     *
                                       *   nCLKSRC_INTRC  (0) - внутренний RC-генератор   *
                                       *   nCLKSRC_MAINOSC(1) - внешний генератор на XTAL *
                                       *   nCLKSRC_RTCOSC (2) - часовой генератор на RTCX */
  unsigned char pll0;                 /* для LPC13xx:                                     *
                                       *   не используется                                *
                                       * для LPC17xx:                                     *
                                       *   1 - запустить PLL0                             */
  unsigned long pclksel0;             /* для LPC13xx:                                     *
                                       *   не используется                                *
                                       * для LPC17xx:                                     *
                                       *   peripheral clock select 0                      */
  unsigned long pclksel1;             /* для LPC13xx:                                     *
                                       *   не используется                                *
                                       * для LPC17xx:                                     *
                                       *   peripheral clock select 1                      */
  unsigned long sourcefreq;           /* частота источника                                *
                                       *   для nCLKSRC_INTRC всегда должна быть:          *
                                       *   - 12МГц в LPC13xx                              *
                                       *   -  4МГц в LPC17xx                              */
  unsigned long corefreq;             /* желаемая частота ядра                            */
  unsigned long pllm;                 /* значение множителя для PLL0                      *
                                       *   поле используется, только если в поле pll0     *
                                       *   указано включение PLL0                         */
  unsigned long plln;                 /* значение делителя для PLL0                       *
                                       *   поле используется, только если в поле pll0     *
                                       *   указано включение PLL0                         */
  unsigned long offset;               /* смещение для таблицы вектров прерываний          */
}CORE_SETTINGS;

extern void           coreInit            (const CORE_SETTINGS* freq);
extern void           coreRegisterSVC     (unsigned char svcnum, SERVICE_DATA* svc);
extern void           coreUnregisterSVC   (unsigned char svcnum);
extern int            coreRequestSVC      (unsigned char svcnum, void* data);
extern void           coreRegisterIRQ     (unsigned char irqnum, unsigned char priority, IRQ_DATA* handler);
extern void           coreUnregisterIRQ   (unsigned char irqnum);
extern void           coreEnableIRQ       (unsigned char irqnum);
extern void           coreDisableIRQ      (unsigned char irqnum);
extern void           coreRestart         (void);
extern unsigned long  coreCurrentFrequency(void);

extern const CORE_SETTINGS core_settings;


#ifdef __cplusplus
}
#endif


#endif /*_CORE_H_*/
