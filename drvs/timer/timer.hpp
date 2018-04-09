#ifndef _TIMER_HPP_
#define _TIMER_HPP_


#include "types.h"
#include "types_ext.hpp"

#define ENABLED           1
#define RESET_ON_MATCH    2
#define STOP_ON_MATCH     4


struct timerChannelSettings
{
  timerChannelSettings()
  {
    this->clear();
  }

  void clear()
  {
    this->matchValue= 0;
    this->flags     = 0;
  }

  unsigned long matchValue;
  unsigned int  flags;
};

struct timerSettings
{
  unsigned long         prescalerValue;
  timerChannelSettings  channels[4];
  unsigned long         pclk;
};

typedef basicCreationDisposition timerCreationDisposition;


class timer
{
public:
  virtual int                   init            (const timerSettings& ts) = 0;
  virtual timerSettings&        settings        () = 0;
  virtual int                   channelInit     (unsigned char channel, const timerChannelSettings& cs) = 0;
  virtual int                   channelInit     (unsigned char channel, unsigned long match) = 0;
  virtual int                   channelInitAddTC(unsigned char channel, unsigned long match) = 0;
  virtual timerChannelSettings& channelSettings (unsigned char channel) = 0;
  virtual int                   channelEnable   (unsigned char channel) = 0;
  virtual int                   channelDisable  (unsigned char channel) = 0;
  virtual int                   start           () = 0;
  virtual int                   stop            () = 0;
  virtual int                   reset           () = 0;
  virtual int                   restart         () = 0;
  virtual inline unsigned long  tickCount       () = 0;
  virtual inline unsigned long  prescalerCount  () = 0;
  virtual int                   channelWait     (unsigned char channel, unsigned long timeout) = 0;
  virtual int                   channelBind     (unsigned char channel, callback1* cb) = 0;
  virtual inline unsigned long  timerFreq       () = 0;
};


#endif /*_TIMER_HPP_*/
