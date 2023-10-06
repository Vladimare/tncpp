#ifndef _GM3x0_HPP_
#define _GM3x0_HPP_

#include "radioset.hpp"
#include "timer.hpp"


#define GM3x0_UP_OFF_RELAY_PERIOD 3000/* длительность отключения реле управления        */
                                      /*   питанием радиостанции при включении          */
                                      /*   радиостанции, в мс                           */
#define GM3x0_UP_READY_PERIOD     3000/* время готовности радиостанции к работе после   */
                                      /*   включения питания, в мс                      */
#define GM3x0_UP_REREADY_PERIOD   500 /* период повторных проверок готовности           */
                                      /*   радиостанции к работе, если она не готова    */
                                      /*   после истечения основного времени готовности */
#define GM3x0_DOWN_OFF_DELAY      1000/* задержка перед отключением реле управления     */
                                      /*   питанием радиостанции при выключении         */
                                      /*   радиостанции, в мс                           */
#define GM3x0_DOWN_READY_PERIOD   1000/* время отключения радиостанции, в мс            */
#define GM3x0_DOWN_REREADY_PERIOD 500 /* период повторных проверок после отключения     */
                                      /*   радиостанции                                 */
#define GM3x0_CH1_OFF_DELAY       10  /* задержка деактивации сигнала CH1 после         */
                                      /*   активации сигнала CH0, в мс                  */
#define GM3x0_PTT_ON_DELAY        190 /* задержка активации сигнала PTT после           */
                                      /*   деактивации сигнала CH1, в мс                */
#define GM3x0_SEND_DELAY          10  /* задержка начала передачи после активации       */
                                      /*   сигнала PTT, в мс                            */
#define GM3x0_CH0_OFF_DELAY       10  /* задержка деактивации сигнала CH0 после         */
                                      /*   активации сигнала CH1, в мс                  */


class gm3x0
  : public radioset
{
public:
  gm3x0(timer* t, unsigned long pclk);
  virtual ~gm3x0();

  /*************************** реализация интерфейса radioset ***************************/
  virtual unsigned long transmitReadyDelaySec();
  virtual unsigned long transmitReadyDelayMsec();
  virtual int  turnOn(callback0* ac);
  virtual int  turnOff(callback0* ac);
  virtual bool isTurnedOn() = 0;      /* этот метод должен быть определен в классе - потомке*/
  virtual int  initTransmit(callback1* sender);
  virtual int  finalizeTransmit();

protected:
  virtual inline void onOffButtonBlock  () = 0;
  virtual inline void onOffButtonUnblock() = 0;

  virtual inline void onPower           () = 0;
  virtual inline void offPower          () = 0;

  virtual inline void activateCH0       () = 0;
  virtual inline void deactivateCH0     () = 0;
  virtual inline void activateCH1       () = 0;
  virtual inline void deactivateCH1     () = 0;
  virtual inline void activatePTT       () = 0;
  virtual inline void deactivatePTT     () = 0;

  virtual inline bool isVoiceChannel    () = 0;

private:
  timer*      tmr;
  callback1*  sender;
  callback0*  onCompleter;
  callback0*  offCompleter;

#define GM3x0F_POWERING_ON  0x00000001
#define GM3x0F_POWERING_OFF 0x00000002
#define GM3x0F_TRANSMITTING 0x00000004
  unsigned long flags;

  binder1<gm3x0> bindAdapter;         /* вспомогательный класс для использования методов*/
                                      /*   в качестве функций обратного вызова таймера  */

  void upStage1Handler(int);          /* первый этап включения радиостанции             */
  void upStage2Handler(int);          /* второй этап включения радиостанции             */
  void downStage1Handler(int);        /* первый этап выключения радиостанции            */
  void downStage2Handler(int);        /* второй этап выключения радиостанции            */
  void transmitStage1Handler(int);    /* первый этап подготовки передачи                */
  void transmitStage2Handler(int);    /* второй этап подготовки передачи                */
  void finalizeStage1Handler(int);    /* первый этап завершения передачи                */
};

#endif /*_GM3x0_HPP_*/
