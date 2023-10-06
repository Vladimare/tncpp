#ifndef _GM3x0_560_HPP_
#define _GM3x0_560_HPP_

#include "gm3x0.hpp"
#include "lpc17xx_defs.h"


#define LED_TXBEGIN {LPC_GPIO1->FIOCLR = (0x01ul << P1_18);}
#define LED_TXEND   {LPC_GPIO1->FIOSET = (0x01ul << P1_18);}

class gm3x0_560
  : public gm3x0
{
public:
  gm3x0_560(timer* t, unsigned long pclk);
  virtual ~gm3x0_560();

  /*************************** реализация интерфейса radioset ***************************/
  virtual bool isTurnedOn() {return ((LPC_GPIO2->FIOPIN & (0x01ul << P2_3)) ? true : false);}

protected:
  virtual inline void onOffButtonBlock  ()  {LPC_GPIO4->FIOSET = (0x01ul << P4_29);}
  virtual inline void onOffButtonUnblock()  {LPC_GPIO4->FIOCLR = (0x01ul << P4_29);}

  virtual inline void onPower           ()  {LPC_GPIO1->FIOCLR = (0x01ul << P1_23);}
  virtual inline void offPower          ()  {LPC_GPIO1->FIOSET = (0x01ul << P1_23);}

  virtual inline void activateCH0       ()
  {
    LED_TXBEGIN;
    LPC_GPIO2->FIOCLR = (0x01ul << P2_7);
  }
  virtual inline void deactivateCH0     ()
  {
    LED_TXEND;
    LPC_GPIO2->FIOSET = (0x01ul << P2_7);
  }
  virtual inline void activateCH1       ()  {LPC_GPIO2->FIOSET = (0x01ul << P2_4);}
  virtual inline void deactivateCH1     ()  {LPC_GPIO2->FIOCLR = (0x01ul << P2_4);}
  virtual inline void activatePTT       ()  {LPC_GPIO1->FIOSET = (0x01ul << P1_28);}
  virtual inline void deactivatePTT     ()  {LPC_GPIO1->FIOCLR = (0x01ul << P1_28);}

  virtual inline bool isVoiceChannel    ()  {return ((LPC_GPIO1->FIOPIN & (0x01ul << P1_29)) ? false : true);}
};

#endif /*_GM3x0_560_HPP_*/
