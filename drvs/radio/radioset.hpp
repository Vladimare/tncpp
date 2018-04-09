#ifndef _RADIOSET_HPP_
#define _RADIOSET_HPP_

#include "types_ext.hpp"


/* интерфейс работы с радиостанциями */
class radioset
{
public:
  /* возвращает время, необходимое для подготовки радиостанции к передаче */
  virtual unsigned long transmitReadyDelaySec() = 0;
  virtual unsigned long transmitReadyDelayMsec()= 0;

  /* инициирует включение радиостанции, после окончания включения         */
  /*   вызывается обработчик ac                                           */
  virtual int  turnOn(callback0* ac) = 0;

  /* инициирует выключение радиостанции, после окончания выключения       */
  /*   вызывается обработчик ac                                           */
  virtual int  turnOff(callback0* ac) = 0;

  /* возвращает true, если радиостанция включена */
  virtual bool isTurnedOn() = 0;

  /* инициирует передачу: переключает каналы, выставляет сигнал PTT,  *
   *   после чего вызывает собственно метод передачи данных           */
  virtual int initTransmit(callback1* sender) = 0;
  /* финализирует передачу: снимает сигнал PTT, переключает каналы,   *
   *   отслеживает, что канал действительно вернулся на прием         */
  virtual int finalizeTransmit() = 0;
};

#endif /*_RADIOSET_HPP_*/
