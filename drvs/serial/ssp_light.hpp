#ifndef _SSP_LIGHT_HPP_
#define _SSP_LIGHT_HPP_

#include "lpc17xx_defs.h"
#include "core.h"

#include "serial.hpp"
#include "event.hpp"


struct sspCreationDisposition
  : public serialCreationDisposition
{
#define SSP_FORMAT_SPI  0
#define SSP_FORMAT_SSI  1
#define SSP_FORMAT_MWI  2

  unsigned char format;
  unsigned char cpol;
  unsigned char cpha;
};

/* компактная версия драйвера SSP, 
для уменьшения размера кода в данной версии работают только синхронные функции */
class ssp_light
  : public serialPort
{
public:
  ssp_light(LPC_SSP_TypeDef* phy, sspCreationDisposition* creationDisposition);
  ~ssp_light();

  virtual int init      (const portSettings& ps);
  virtual const portSettings& settings() {return this->ps;}
  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);
  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);
  virtual int cancelReadAsync (int reason);
  virtual int cancelWriteAsync(int reason);
  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);

  static void pinSetupSSP0_0(unsigned char setDefault);
  static void pinSetupSSP0_1(unsigned char setDefault);
  static void pinSetupSSP1_0(unsigned char setDefault);
  static void pinSetupSSP1_1(unsigned char setDefault);

private:
  portSettings      ps;               // Настройки порта
  LPC_SSP_TypeDef*  low;              // Указатель на регистры периферийного блока SSP
  PIN_SETUP         pinSetupProc;     // Указатель на метод конфигурирования ножек SSP
  unsigned long     pwrMask;          // Битовая маска включения питания периферийного блока SSP
};


#endif /*_SSP_LIGHT_HPP_*/
