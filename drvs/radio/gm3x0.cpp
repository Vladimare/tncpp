#include "errors.h"
#include "gm3x0.hpp"


gm3x0::gm3x0(timer* t, unsigned long pclk)
{
  this->tmr     = t;

  timerSettings ts;                   /* таймер заводим с периодом 1мс                  */
  ts.prescalerValue         = (pclk / 1000) - 1;
  ts.channels[0].matchValue = 0;
  ts.channels[0].flags      = ENABLED | RESET_ON_MATCH | STOP_ON_MATCH;
  ts.pclk                   = pclk;

  this->tmr->stop();
  this->tmr->init(ts);

  this->flags = 0;
}


gm3x0::~gm3x0()
{
}


unsigned long gm3x0::transmitReadyDelaySec()
{
  return ((GM3x0_CH1_OFF_DELAY + GM3x0_PTT_ON_DELAY/* + GM3x0_SEND_DELAY*/) / 1000);
}

unsigned long gm3x0::transmitReadyDelayMsec()
{
  return ((GM3x0_CH1_OFF_DELAY + GM3x0_PTT_ON_DELAY/* + GM3x0_SEND_DELAY*/) % 1000);
}

/****************************************************************************************
 ******************************** включение радиостанции ********************************
 ****************************************************************************************/
int gm3x0::turnOn(callback0* ac)
{
  if(this->flags & ~GM3x0F_POWERING_ON)
    return ERR_DEVICE_BUSY;

  if(!this->isTurnedOn() && (!(this->flags & GM3x0F_POWERING_ON)))
  {                                   /* еще не включены и не включаемся                */
    this->flags |= GM3x0F_POWERING_ON;
    this->onCompleter = ac;

    this->offPower();                 /* отключаем реле управления питанием радиостанции*/
                                      /* стартуем таймер на включение реле              */
    this->bindAdapter.bind(&gm3x0::upStage1Handler, this);
    this->tmr->channelBind(0, &this->bindAdapter);
    this->tmr->channelInit(0, GM3x0_UP_OFF_RELAY_PERIOD);
    this->tmr->restart();
  }

  return ERR_OK;
}

void gm3x0::upStage1Handler(int)
{
  this->onPower();                    /* включаем реле управления питанием радиостанции */
  this->onOffButtonBlock();           /* блокируем кнопку ручного включения радиостанции*/

                                      /* стартуем таймер на проверку готовности         */
                                      /*   радиостанции                                 */
  this->bindAdapter.bind(&gm3x0::upStage2Handler, this);
  this->tmr->channelBind(0, &this->bindAdapter);
  this->tmr->channelInit(0, GM3x0_UP_READY_PERIOD);
  this->tmr->restart();
}

void gm3x0::upStage2Handler(int)
{
  if(!this->isTurnedOn())
  {                                   /* радиостанция не включилась                     */
                                      /* перезапускаем таймер                           */
    this->tmr->channelInit(0, GM3x0_UP_REREADY_PERIOD);
    this->tmr->restart();
  }
  else
  {
    this->flags &= ~GM3x0F_POWERING_ON;
    this->deactivateCH0();
    this->deactivateCH1();
    this->deactivatePTT();
    if(this->onCompleter)
      (*this->onCompleter)();
  }
}


/****************************************************************************************
 ******************************* выключение радиостанции ********************************
 ****************************************************************************************/
int gm3x0::turnOff(callback0* ac)
{
  if(this->flags & ~GM3x0F_POWERING_OFF)
    return ERR_DEVICE_BUSY;

  if(this->isTurnedOn() && (!(this->flags & GM3x0F_POWERING_OFF)))
  {                                   /* еще не выключены и не выключаемся              */
    this->flags |= GM3x0F_POWERING_OFF;

    this->offCompleter = ac;

    this->onOffButtonUnblock();       /* отпускаем кнопку управления питанием           */
                                      /*   радиостанции                                 */
                                      /* стартуем таймер на выключение реле             */
    this->bindAdapter.bind(&gm3x0::downStage1Handler, this);
    this->tmr->channelBind(0, &this->bindAdapter);
    this->tmr->channelInit(0, GM3x0_DOWN_OFF_DELAY);
    this->tmr->restart();
  }

  return ERR_OK;
}

void gm3x0::downStage1Handler(int)
{
  this->offPower();                   /* выключаем реле управления питанием радиостанции*/

                                      /* стартуем таймер на проверку отключения         */
                                      /*   радиостанции                                 */
  this->bindAdapter.bind(&gm3x0::downStage2Handler, this);
  this->tmr->channelBind(0, &this->bindAdapter);
  this->tmr->channelInit(0, GM3x0_DOWN_READY_PERIOD);
  this->tmr->restart();
}

void gm3x0::downStage2Handler(int)
{
  if(this->isTurnedOn())
  {                                   /* радиостанция не выключилась                    */
                                      /* перезапускаем таймер                           */
    this->tmr->channelInit(0, GM3x0_DOWN_REREADY_PERIOD);
    this->tmr->restart();
  }
  else
  {
    this->flags &= ~GM3x0F_POWERING_OFF;

    if(this->offCompleter)
      (*this->offCompleter)();
  }
}

/****************************************************************************************
 ***************************** подготовка и запуск передачи *****************************
 ****************************************************************************************/
int gm3x0::initTransmit(callback1* s)
{
  if(this->flags & ~GM3x0F_TRANSMITTING)
    return ERR_DEVICE_BUSY;

  this->sender = s;

  this->activateCH0();

  this->bindAdapter.bind(&gm3x0::transmitStage1Handler, this);
  this->tmr->channelBind(0, &this->bindAdapter);
  this->tmr->channelInit(0, GM3x0_CH1_OFF_DELAY);
  this->tmr->restart();

  return ERR_OK;
}

void gm3x0::transmitStage1Handler(int)
{
  this->activateCH1();

  this->bindAdapter.bind(&gm3x0::transmitStage2Handler, this);
  this->tmr->channelBind(0, &this->bindAdapter);
  this->tmr->channelInit(0, GM3x0_PTT_ON_DELAY);
  this->tmr->restart();
}

void gm3x0::transmitStage2Handler(int)
{
  this->activatePTT();

  this->tmr->channelBind(0, this->sender);
  this->tmr->channelInit(0, GM3x0_SEND_DELAY);
  this->tmr->restart();
}


/****************************************************************************************
 ************************** зачистка после окончания передачи ***************************
 ****************************************************************************************/
int gm3x0::finalizeTransmit()
{
  if(this->flags & ~GM3x0F_TRANSMITTING)
    return ERR_DEVICE_BUSY;

  this->deactivatePTT();
  this->deactivateCH1();

  this->bindAdapter.bind(&gm3x0::finalizeStage1Handler, this);
  this->tmr->channelBind(0, &this->bindAdapter);
  this->tmr->channelInit(0, GM3x0_CH0_OFF_DELAY);
  this->tmr->restart();

  return ERR_OK;
}

void gm3x0::finalizeStage1Handler(int)
{
  this->deactivateCH0();

  this->flags &= ~GM3x0F_TRANSMITTING;
}
