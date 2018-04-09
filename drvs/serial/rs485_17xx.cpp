#include "core.h"
#include "errors.h"
#include "rs485_17xx.hpp"


rs485_17xx_base::rs485_17xx_base(LPC_UART1_TypeDef* phy)
{
  this->low = phy;
}

/* деструктор */
rs485_17xx_base::~rs485_17xx_base()
{
}

/* настройка аппаратного управления направлением передачи */
int rs485_17xx_base::cfgDirectionControl(unsigned char pin, unsigned char polarity, unsigned char delay)
{
  unsigned char rs485ctrl = (1ul << DCTRL);

  switch(pin)
  {
  case DTR:
    rs485ctrl |=  (1ul << SEL);
    break;

  case RTS:
    rs485ctrl &= ~(1ul << SEL);
    break;

  default:
    return TERR_WRONG_PARAM;
  }

  if(polarity)                        /* положительная логика управления драйвером      */
    rs485ctrl |= (1ul << OINV);       /* true  - включается единицей,                   */
                                      /*         выключается нулем                      */
  this->low->RS485CTRL= rs485ctrl;    /* false - включается единицей,                   */
  this->low->RS485DLY = delay;        /*         выключается нулем                      */

  return ERR_OK;
}

/* статические методы, которые непосредственно рулят ножками процессора *
 * параметр задает вариант установки:                                   *
 *   0 - рабочее для функционала положение                              *
 *   1 - исходное положение ножек процессора (как после сброса)         */
void rs485_17xx_base::pinSetupRS485_0_DTR(unsigned char setDefault)
{
  LPC_PINCON->PINSEL1 &= ~((0x03ul << ((P0_16 - 16) * 2)) | (0x03ul << ((P0_20 - 16) * 2)));
  LPC_PINCON->PINMODE1&= ~ (0x03ul << ((P0_16 - 16) * 2));
  LPC_PINCON->PINSEL0 &= ~ (0x03ul <<  (P0_15       * 2));

  if(!setDefault)
  {
    LPC_PINCON->PINSEL0 |= P0_15_TXD1;
    LPC_PINCON->PINSEL1 |= P0_16_RXD1 | P0_20_DTR1;
    LPC_PINCON->PINMODE1|= (NOPULL << ((P0_16 - 16) * 2));
  }
}

void rs485_17xx_base::pinSetupRS485_0_RTS(unsigned char setDefault)
{
  LPC_PINCON->PINSEL1 &= ~((0x03ul << ((P0_16 - 16) * 2)) | (0x03ul << ((P0_22 - 16) * 2)));
  LPC_PINCON->PINMODE1&= ~ (0x03ul << ((P0_16 - 16) * 2));
  LPC_PINCON->PINSEL0 &= ~ (0x03ul <<  (P0_15       * 2));

  if(!setDefault)
  {
    LPC_PINCON->PINSEL0 |= P0_15_TXD1;
    LPC_PINCON->PINSEL1 |= P0_16_RXD1 | P0_22_RTS1;
    LPC_PINCON->PINMODE1|= (NOPULL << ((P0_16 - 16) * 2));
  }
}

void rs485_17xx_base::pinSetupRS485_1_DTR(unsigned char setDefault)
{
  LPC_PINCON->PINSEL4 &= ~((0x03ul << (P2_0 * 2)) | (0x03ul << (P2_1 * 2)));
  LPC_PINCON->PINMODE4&= ~ (0x03ul << (P2_1 * 2));
  LPC_PINCON->PINSEL4 &= ~ (0x03ul << (P2_5 * 2));

  if(!setDefault)
  {
    LPC_PINCON->PINSEL4 |= P2_0_TXD1 | P2_1_RXD1 | P2_5_DTR1;
    LPC_PINCON->PINMODE4|= (NOPULL << (P2_1 * 2));
  }
}

void rs485_17xx_base::pinSetupRS485_1_RTS(unsigned char setDefault)
{
  LPC_PINCON->PINSEL4 &= ~((0x03ul << (P2_0 * 2))  | (0x03ul << (P2_1 * 2)));
  LPC_PINCON->PINMODE4&= ~ (0x03ul << (P2_1 * 2));
  LPC_PINCON->PINSEL4 &= ~ (0x03ul << (P2_7 * 2));

  if(!setDefault)
  {
    LPC_PINCON->PINSEL4 |= P2_0_TXD1 | P2_1_RXD1 | P2_7_RTS1;
    LPC_PINCON->PINMODE4|= (NOPULL << (P2_1 * 2));
  }
}


rs485_17xx::rs485_17xx(LPC_UART1_TypeDef* phy, const serialCreationDisposition& cd)
  : uart17xx((LPC_UART_TypeDef*)phy, cd)
  , rs485_17xx_base(phy)
{
}

rs485_17xx::~rs485_17xx()
{
}
