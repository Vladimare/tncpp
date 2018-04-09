#ifndef _RTC17xx_HPP_
#define _RTC17xx_HPP_

#include "lpc17xx_defs.h"
#include "types.h"
#include "types_ext.hpp"


struct rtcCreationDisposition
  : public basicCreationDisposition
{
  unsigned long pclk;                 /* тактовая частота, подаваемая на аппаратный     */
                                      /*   таймер часов                                 */
  unsigned long valid_timeout;        /* таймаут валидности, сек                        */
};


class rtc17xx
{
public:
                                      /* UTC-время по умолчанию: 00:00:00 01.10.2010    */
  static const unsigned long RTC_UTC_START = 0x4CA52480;


  rtc17xx(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd);
  ~rtc17xx();

  int syncPhase();                    /* сигнал синхронизации фазы таймера RTC          */
  int setRTC  (unsigned long  utc);
  int getRTC  (unsigned long* utc, unsigned long* msec);
  int setValid(bool valid = true);
  int setAlarm(unsigned long  utc, unsigned long  msec, callback0* acb);

  static rtc17xx* g_rtc;

private:
  struct svc_data                     /* данные RTC-сервиса                             */
  {
#define RTCSVC_CALLID_SYNC        0   /* синхронизация фазы часов RTC                   */
#define RTCSVC_CALLID_SYNC_SKIP   1   /* пропущен импульс синхронизации часов           */
#define RTCSVC_CALLID_SECOND      2   /* сработал таймер секунды                        */
#define RTCSVC_CALLID_SET         3   /* установка часов                                */
#define RTCSVC_CALLID_GET         4   /* чтение часов                                   */
#define RTCSVC_CALLID_VALIDUP     5   /* восстановление валидности синхроимпульсов      */
#define RTCSVC_CALLID_VALIDDOWN   6   /* потеря валидности синхроимпульсов              */
#define RTCSVC_CALLID_BINDALARM   7   /* установка будильника                           */
#define RTCSVC_CALLID_RUNALARM    8   /* срабатывание будильника                        */
    unsigned char callID;             /* идентификатор вызова                           */
    unsigned long utc;                /* время utc, или указатель на него               */
    unsigned long msec;               /* время ms, или указатель на него                */
    void* data;                       /* указатель на данные, структура данных          */
                                      /*   определяется запрашиваемым сервисом          */
  };

                                      /* флаги часов                                    */
#define RTCF_SYNC_PHASE   0x01        /* фаза синхронизирована                          */
#define RTCF_SYNC_UTC     0x02        /* время UTC синхронизировано                     */
  unsigned long flags;
  unsigned long prescaler_corrector;  /* коэффициент для коррекции предделителя         */

  unsigned long utc;                  /* текущее значение времени с точностью до сек    */
                                      /* значение мс хранится в счетном регистре        */
                                      /*   аппаратного таймера                          */
  unsigned long valid_top;            /* момент окончания валидности                    */
  unsigned long valid_tout;           /* таймаут валидности                             */


  unsigned long alarmutc;             /* секунда срабатывания будильника                */
  unsigned long alarmmsec;            /* миллисекунда срабатывания будильника           */
  callback0* alarmer;                 /* обработчик будильника                          */

  LPC_TIM_TypeDef* low;               /* регистры таймера                               */
  unsigned long pconpMask;            /* маска аппаратного таймера в регистре PCONP     */
  unsigned long irqnum;               /* номер прерывания аппаратного таймера           */

                                      /* обработчик прерываний аппаратного таймера RTC  */
  static void rtch        (void* instance);
  static int  rtc_service (void* instance, void* d);

  __inline void utc_inc()
  {
    this->utc++;
    if(this->utc > this->valid_top)
      this->flags &= ~RTCF_SYNC_UTC;
  }
};

#endif /*_RTC17xx_HPP_*/
