#include "apptask.h"
#include "core.h"
#include "errors.h"
#include "rtc17xx.hpp"


rtc17xx* rtc17xx::g_rtc = 0;


rtc17xx::rtc17xx(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd)
{
  g_rtc = this;

  this->flags               = 0;
  this->prescaler_corrector = rcd->pclk / 2000000;

  this->pconpMask = rcd->pwrMask;     /* сохраняем входные данные                       */
  this->irqnum    = rcd->irqNumber;
  this->low       = phy;
  this->valid_tout= rcd->valid_timeout;

  this->alarmutc  = 0;
  this->alarmmsec = 0;
  this->alarmer   = 0;

  this->utc   = RTC_UTC_START;        /* инициализируем внутренние поля                 */

  LPC_SC->PCONP  |= this->pconpMask;  /* даем частоту на таймер                         */
                                      /* разрешаем работу таймера, но держим его в      */
                                      /*   сбросе до окончания инициализации            */
  this->low->TCR  = (0x01ul << CNTR_ENABLE) | (0x01ul << CNTR_RESET);
                                      /* предделитель настраиваем на 1мс (1кГц)         */
  this->low->PR   = (rcd->pclk / 1000) - 1;

  this->low->MR0  = 1000;             /* нулевой канал отсчитывает секунды              */
  this->low->MR3  = 2000;             /* на третьем канале - таймаут импульса           */
                                      /*   синхронизации; после пропуска одного импульса*/
                                      /*   часы считаются несихронизированными          */
  this->low->MCR  = (0x01ul << MR0I) |/* таким образом, таймер обнуляется если долго нет*/
                    (0x01ul << MR3I) |/*   внешних импульсов синхронизации; при этом раз*/
                    (0x01ul << MR3R); /*   в секунду генерируются прерывания            */
  
  this->low->CTCR = 0x00000000;       /* режим таймера                                  */

                                      /* регистрируем прерывание таймера                */
  IRQ_DATA irqd = {rtc17xx::rtch, this};
  coreRegisterIRQ(this->irqnum, rcd->irqPriority, &irqd);

                                      /* регистрируем сервис RTC                        */
  SERVICE_DATA svcd = {rtc17xx::rtc_service, this};
  coreRegisterSVC(APPSVC_RTC, &svcd);

  this->low->TCR = (1 << CNTR_ENABLE);/* запускаем таймер                               */
}

rtc17xx::~rtc17xx()
{
  coreUnregisterSVC(APPSVC_RTC);      /* останавливаем сервис RTC                       */
  coreUnregisterIRQ(this->irqnum);    /* снимаем прерывание с регистрации               */
  this->low->TCR  = 0x00;             /* останавливаем таймер                           */
  LPC_SC->PCONP  &= ~this->pconpMask; /* снимаем частоту с таймера                      */
}

int rtc17xx::syncPhase()
{
#if 0
#warning !
  if(LPC_GPIO1->FIOPIN & (0x01ul << P1_4))
    LPC_GPIO1->FIOCLR = (0x01ul << P1_4);
  else
    LPC_GPIO1->FIOSET = (0x01ul << P1_4);
#endif

  static const rtc17xx::svc_data svcd = {RTCSVC_CALLID_SYNC, 0, 0, 0};

  return coreRequestSVC(APPSVC_RTC, (void*)&svcd);
}

int rtc17xx::setRTC(unsigned long utc)
{
  rtc17xx::svc_data svcd = {RTCSVC_CALLID_SET, utc, 0, 0};

  return coreRequestSVC(APPSVC_RTC, &svcd);
}

int rtc17xx::getRTC(unsigned long* utc, unsigned long* msec)
{
  rtc17xx::svc_data svcd = {RTCSVC_CALLID_GET, (unsigned long)utc, (unsigned long)msec, 0};

  return coreRequestSVC(APPSVC_RTC, &svcd);
}

int rtc17xx::setValid(bool valid)
{
  rtc17xx::svc_data svcd = {valid ? RTCSVC_CALLID_VALIDUP : RTCSVC_CALLID_VALIDDOWN, 0, 0, 0};

  return coreRequestSVC(APPSVC_RTC, &svcd);
}

int rtc17xx::setAlarm(unsigned long  utc, unsigned long  msec, callback0* acb)
{
  rtc17xx::svc_data svcd = {RTCSVC_CALLID_BINDALARM, utc, msec, acb};

  return coreRequestSVC(APPSVC_RTC, &svcd);
}

int rtc17xx::rtc_service(void* instance, void* d)
{
  rtc17xx* _this = (rtc17xx*)instance;
  rtc17xx::svc_data* _data = (rtc17xx::svc_data*)d;

  switch(_data->callID)
  {
  case RTCSVC_CALLID_SYNC:            /* синхронизация фазы часов RTC                   */
                                      /* корректируем предделитель, чтобы компенсировать*/
                                      /*   неточности источника частоты                 */
    if(_this->flags & RTCF_SYNC_PHASE)/* можем корректировать, только если фаза         */
    {                                 /*   синхронизирована                             */
      if(_this->low->TC > (1000 - 1)) /* наш таймер убегает вперед, замедляем его       */
        _this->low->PR += (_this->low->TC - (1000 - 1)) * _this->prescaler_corrector;
      else
      {                               /* таймер отстает, ускоряем его                   */
        _this->low->PR -= ((1000 - 1) - _this->low->TC) * _this->prescaler_corrector;
        _this->utc_inc();             /* инкрементируем часы здесь, поскольку внутренний*/
                                      /*   таймер не досчитал до секунды, и не          */
                                      /*   сформировал свое прерывание; а извне часы    */
                                      /*   обновятся неизвестно когда                   */
      }
    }
                                      /* сбрасываем внутренний таймер                   */
    _this->low->TCR = (1 << CNTR_ENABLE) | (1 << CNTR_RESET);
    _this->low->TCR = (1 << CNTR_ENABLE);

    _this->flags |= RTCF_SYNC_PHASE;  /* фаза часов теперь синхронизирована             */
    break;

  case RTCSVC_CALLID_SYNC_SKIP:       /* таймаут импульса синхронизации                 */
    _this->flags &= ~RTCF_SYNC_PHASE;

  case RTCSVC_CALLID_SECOND:          /* сработал таймер секунды (этот обработчик       */
    _this->utc_inc();                 /*   вызывается также и для таймаута импульса     */
    break;                            /*   синхронизации)                               */

  case RTCSVC_CALLID_SET:
    if(_this->flags & RTCF_SYNC_UTC)
    {                                 /* для установки часов должен быть установлен     */
                                      /*   признак валидности времени UTC               */
      if(_data->utc != 0xFFFFFFFF)
      {
        _this->utc      = _data->utc;
        _this->valid_top= _this->utc + _this->valid_tout;
      }
    }
    break;

  case RTCSVC_CALLID_GET:
    if(_data->utc)  *((unsigned long*)(_data->utc )) = _this->utc;
    if(_data->msec) *((unsigned long*)(_data->msec)) = _this->low->TC % 1000;
    break;

  case RTCSVC_CALLID_VALIDUP:
    _this->flags |= RTCF_SYNC_UTC;
    break;

  case RTCSVC_CALLID_VALIDDOWN:
    _this->flags &= ~RTCF_SYNC_UTC;
    break;

  case RTCSVC_CALLID_BINDALARM:
    if(_this->utc > _data->utc)
      return ERR_PARAM;               /* время уже убежало вперед                       */

    if(_this->utc == _data->utc)
    {
      if((_this->low->TC % 1000) >= _data->msec)
        return ERR_PARAM;             /* время уже убежало вперед                       */
    }

    _this->alarmutc = _data->utc;
    _this->alarmmsec= _data->msec;
    _this->alarmer  = (callback0*)_data->data;

    _this->low->MR1  = _this->alarmmsec;
    _this->low->MR2  = _this->alarmmsec + 1000;
    _this->low->MCR |= (0x01ul << MR1I) | (0x01ul << MR2I);
    break;

  case RTCSVC_CALLID_RUNALARM:
    if(_this->alarmer)
      (*(_this->alarmer))();

    _this->low->MCR &= ~((0x01ul << MR1I) | (0x01ul << MR2I));
    break;

  default:
    return ERR_NOT_SUPPORTED;
  }

  if(!((_this->flags & RTCF_SYNC_UTC) &&
       (_this->flags & RTCF_SYNC_PHASE)))
    return ERR_DEVICE_NOSYNC;
  else
    return ERR_OK;
}

void rtc17xx::rtch(void* instance)
{
  rtc17xx* _this        = (rtc17xx*)instance;
  unsigned long intlist = _this->low->IR;

  _this->low->IR = intlist;           /* сбрасываем прерывания                          */

  if(intlist & (0x01ul << _MR0))
  {                                   /* сработал таймер секунды                        */
    static const rtc17xx::svc_data svcd = {RTCSVC_CALLID_SECOND, 0, 0, 0};
    coreRequestSVC(APPSVC_RTC, (void*)&svcd);
  }

  if((intlist & (0x01ul << _MR1)) || (intlist & (0x01ul << _MR2)))
  {                                   /* сработал будильник                             */
    if(_this->utc == _this->alarmutc)
    {
      static const rtc17xx::svc_data svcd = {RTCSVC_CALLID_RUNALARM, 0, 0, 0};
      coreRequestSVC(APPSVC_RTC, (void*)&svcd);
    }
  }

  if(intlist & (0x01ul << _MR3))
  {                                   /* таймаут импульса синхронизации                 */
    static const rtc17xx::svc_data svcd = {RTCSVC_CALLID_SYNC_SKIP, 0, 0, 0};
    coreRequestSVC(APPSVC_RTC, (void*)&svcd);
  }
}
