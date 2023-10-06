#ifndef _RS485_17XX_HPP_
#define _RS485_17XX_HPP_

#include "uart17xx.hpp"


class rs485_17xx_base
{
public:
  rs485_17xx_base(LPC_UART1_TypeDef* phy);
  virtual ~rs485_17xx_base();

  int cfgDirectionControl(unsigned char pin, unsigned char polarity, unsigned char delay);

  static void pinSetupRS485_0_DTR(unsigned char setDefault);
  static void pinSetupRS485_0_RTS(unsigned char setDefault);
  static void pinSetupRS485_1_DTR(unsigned char setDefault);
  static void pinSetupRS485_1_RTS(unsigned char setDefault);

private:
  LPC_UART1_TypeDef* low;
};


class rs485_17xx
  : public uart17xx
  , public rs485_17xx_base
{
public:
  rs485_17xx(LPC_UART1_TypeDef* phy, const serialCreationDisposition& cd);
  virtual ~rs485_17xx();
};


#endif /*_RS485_17XX_HPP_*/
