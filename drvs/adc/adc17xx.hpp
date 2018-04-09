#ifndef _ADC17XX_H_
#define _ADC17XX_H_

#include "lpc17xx_defs.h"
#include "types.h"
#include "tn.h"

#include "thread.hpp"
#include "types_ext.hpp"
#include "unitimer17xx.hpp"
#include "core.h"
#include "corelib.h"
#include "errors.h"
#include "syslog.hpp"

#include <stdio.h>

                                      /* данные измерительного канала         */
struct adcChannelData
{
  unsigned char   chanNum;            /* номер канала                         */
  unsigned long   periodCounter;      /* количество пропущенных периодов      */
  unsigned short  value;              /* последнее измеренное значение        */
  unsigned short  getCounter;         /* индикатор пропусков                  */
};

                                      /* настройки измерительного канала      */
struct adcChannelSettings
{
  unsigned long period;               /* кратность опроса канала              */
  callback1* dataReceiver;            /* обработчик данных                    */
};


                                      /* настройки АЦП                        */
struct adcSettings
{
  unsigned long adcTick;              /* длительность измерительного цикла мкС*/
  unsigned long adcPclk;              /* тактовая частота поступающая на АЦП  */

  unitimer17xx*     adcTimer;

  unsigned long timerPclk;            /* тактовая частота поступающая на тайм */

  adcChannelSettings* chanSettings;   /* настройки измерительных каналов      */
  unsigned char chanCount;            /* количество каналов                   */
};

/* класс реализующий опрос мультиплексурованных каналов АЦП*/
class adc17xx
  : public thread
{
public:
  adc17xx(LPC_ADC_TypeDef* adcphy, basicCreationDisposition* adcCD_, const adcSettings& s);
  virtual ~adc17xx();
  int setChanParam(unsigned char channel,  const adcChannelSettings& chanSettings);

protected:
  adcSettings settings;
  adcChannelData* chData;

  virtual int prepare(unsigned char channel, unsigned char* ADx, unsigned int* delay) = 0;
  virtual int finalize(unsigned char channel, unsigned int* delay) = 0;
  virtual void run();
  void setDelay(unsigned int delay);

private:
  basicCreationDisposition adcCD;
  LPC_ADC_TypeDef*  adclow;           /* CMSIS interface to low level ADC registers*/
  

  unsigned char  measureChannel;      /* текущий измерительный канал          */
  unsigned int   mr0Val;

  binder1<adc17xx> m0Binder;
  binder1<adc17xx> m1Binder;

                                      /* обработчик прерывания АЦП            */
  static void adcCompleteHandler(void* instance);
                                      /* обработчик прерывания таймера        */
  void timerMatch0(int ch);
  void timerMatch1(int ch);

  static const threadCreationDisposition tcd;
};

#endif /*_ADC17XX_H_*/
