#ifndef _UNITIMER17xx_HPP_
#define _UNITIMER17xx_HPP_

#include "lpc17xx_defs.h"

#include "tn.h"
#include "timer.hpp"

//Маски для для использования с TN_EVENTS
#define CHANNEL_1         0x01
#define CHANNEL_2         0x02
#define CHANNEL_3         0x04
#define CHANNEL_4         0x08


/* структуры данных */
class unitimer17xx
  : public timer
{
public:
  unitimer17xx(LPC_TIM_TypeDef* phy, const timerCreationDisposition& creationDisposition);
  ~unitimer17xx();

  virtual int                   init            (const timerSettings& ts);
  virtual timerSettings&        settings        ();
  virtual int                   channelInit     (unsigned char channel, const timerChannelSettings& cs);
  virtual int                   channelInit     (unsigned char channel, unsigned long match);
  virtual int                   channelInitAddTC(unsigned char channel, unsigned long match);
  virtual timerChannelSettings& channelSettings (unsigned char channel);
  virtual int                   channelEnable   (unsigned char channel);
  virtual int                   channelDisable  (unsigned char channel);
  virtual int                   start           ();
  virtual int                   stop            ();
  virtual int                   reset           ();
  virtual int                   restart         ();
  virtual unsigned long         tickCount       ();
  virtual unsigned long         prescalerCount  ();
  virtual int                   channelWait     (unsigned char channel, unsigned long timeout);
  virtual int                   channelBind     (unsigned char channel, callback1* cb);
  virtual unsigned long         timerFreq       ();

private:
  callback1*                asyncCB[4];
  unsigned char             irqNum;
  timerSettings             ts;
  LPC_TIM_TypeDef*          low;
  TN_EVENT                  evts;
  unsigned long             pconpMask;
  static const unsigned int masks[4];

  static void irqh(void* _this);
};


#endif /*_UNITIMER17xx_HPP_*/
