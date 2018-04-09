#include "gsm_587.hpp"
#include "lpc17xx_defs.h"
#include "syslog.hpp"
#include "systimer.h"
#include "errors.h"

gsm_587::gsm_587(rs232Port *port, unsigned long pclk)
  : gsm(port, pclk)
{
  this->pinSetup();
}

gsm_587::~gsm_587()
{
  this->pinSetup(true);
}

void gsm_587::pinSetup(bool setDefault)
{

  LPC_PINCON->PINSEL9 &= ~(0x03ul << ((P4_29 - 16) * 2));
  LPC_PINCON->PINSEL3 &= ~(0x03ul << ((P1_17 - 16) * 2));
  LPC_PINCON->PINSEL3 &= ~(0x03ul << ((P1_23 - 16) * 2));
  LPC_GPIO4->FIODIR   &=  ~(0x01ul << P4_29);
  LPC_GPIO1->FIODIR   &=  ~(0x01ul << P1_17);
  LPC_GPIO1->FIODIR   &=  ~(0x01ul << P1_23);

  if(!setDefault)
  {
    LPC_GPIO1->FIOCLR  = 0x01ul << P1_23;
    LPC_GPIO4->FIOCLR  = 0x01ul << P4_29;
    LPC_GPIO1->FIOSET  = 0x01ul << P1_17;
    LPC_GPIO4->FIODIR   |=  0x01ul << P4_29;
    LPC_GPIO1->FIODIR   |=  0x01ul << P1_17;
    LPC_GPIO1->FIODIR   |=  0x01ul << P1_23;

  }
}

void gsm_587::off(void)
{
  LPC_GPIO1->FIOCLR  = 0x01ul << P1_23; //Отключение питания

  g_writelog(LOG_LEVEL_INFO, "gsm: module off\r\n");
}

int gsm_587::on(unsigned char simnum)
{
  int pin;
  unsigned long ms;

  if(!this->port)
    return ERR_DEVICE_NOINIT;

  LPC_GPIO1->FIOSET  = 0x01ul << P1_23; // Включение питания
  this->sleep(50);
  LPC_GPIO4->FIOSET  = 0x01ul << P4_29; // Включение зажигания
  this->sleep(50);
  LPC_GPIO4->FIOCLR  = 0x01ul << P4_29;

  this->port->purge(PURGE_RX);
  ms = systimerUptime(0);

  this->sim(simnum);

  while((systimerUptime(0) - ms) < GSM_STARTUP_TIMEOUT)
  {
    this->port->getRS232(&pin);       // Считывание состояния ног последовательного порта
    if(pin & CTS)                     // Модем готов обмену данными
    {
      this->activate();

      g_writelog(LOG_LEVEL_INFO, "gsm: module on in %dms\r\n", (systimerUptime(0) - ms));

      this->setATReady();
      this->enableSIMURC();
      this->enableSMSURC();
      this->enableRegURC();

      return ERR_OK;
    }
  }

  g_writelog(LOG_LEVEL_INFO, "gsm: module startup timeout\r\n");

  return ERR_TIMEOUT;
}

void gsm_587::sim(unsigned char num)
{
  if(num > 1)
    return;

  if(num)
    LPC_GPIO1->FIOCLR  = 0x01ul << P1_17;
  else
    LPC_GPIO1->FIOSET  = 0x01ul << P1_17;

  g_writelog(LOG_LEVEL_INFO, "gsm: SIM%d (XP%d on PCB) selected\r\n", num, 5 - num);
}
