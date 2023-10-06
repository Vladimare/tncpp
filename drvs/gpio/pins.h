#ifndef _PINS_H_
#define _PINS_H_

#include "lpc17xx_defs.h"

typedef struct tagPIN
{
  unsigned char pin;
  unsigned char pint_mask;
  LPC_GPIO_TypeDef* port;
  LPC_GPIOINT_TypeDef2* pint;
}PIN;

extern const PIN p0_00;
extern const PIN p0_01;
extern const PIN p0_02;
extern const PIN p0_03;
extern const PIN p0_04;
extern const PIN p0_05;
extern const PIN p0_06;
extern const PIN p0_07;
extern const PIN p0_08;
extern const PIN p0_09;
extern const PIN p0_10;
extern const PIN p0_11;
extern const PIN p0_15;
extern const PIN p0_16;
extern const PIN p0_17;
extern const PIN p0_18;
extern const PIN p0_19;
extern const PIN p0_20;
extern const PIN p0_21;
extern const PIN p0_22;
extern const PIN p0_23;
extern const PIN p0_24;
extern const PIN p0_25;
extern const PIN p0_26;
extern const PIN p0_27;
extern const PIN p0_28;
extern const PIN p0_29;
extern const PIN p0_30;
extern const PIN p1_00;
extern const PIN p1_01;
extern const PIN p1_04;
extern const PIN p1_08;
extern const PIN p1_09;
extern const PIN p1_10;
extern const PIN p1_14;
extern const PIN p1_15;
extern const PIN p1_16;
extern const PIN p1_17;
extern const PIN p1_18;
extern const PIN p1_19;
extern const PIN p1_20;
extern const PIN p1_21;
extern const PIN p1_22;
extern const PIN p1_23;
extern const PIN p1_24;
extern const PIN p1_25;
extern const PIN p1_26;
extern const PIN p1_27;
extern const PIN p1_28;
extern const PIN p1_29;
extern const PIN p1_30;
extern const PIN p1_31;
extern const PIN p2_00;
extern const PIN p2_01;
extern const PIN p2_02;
extern const PIN p2_03;
extern const PIN p2_04;
extern const PIN p2_05;
extern const PIN p2_06;
extern const PIN p2_07;
extern const PIN p2_08;
extern const PIN p2_09;
extern const PIN p2_10;
extern const PIN p2_11;
extern const PIN p2_12;
extern const PIN p2_13;
extern const PIN p3_25;
extern const PIN p3_26;
extern const PIN p4_28;
extern const PIN p4_29;

#endif /*_PINS_H_*/
