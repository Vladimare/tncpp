#ifndef _AT24C512_HPP_
#define _AT24C512_HPP_

#include "extstorage.hpp"

  //********************************* ВНИМАНИЕ!!!!! ***************************** 
  //************* ДРАЙВЕР ПОДДЕРЖИВАЕТ ТОЛЬКО ОДНУ МИКРОСХЕМУ EEPROM ************


//********************* Параметры микросхемы **************************
#define EEPROM_ADDRESS 0xA0
#define EEPROM_CLEAR_VALUE 0xff

#define MAX_LENGTH_DATA_TO_WRITE 32  // максимальное количество записываемых байт 
                                     // в микросхему памяти. Т.е. входной буфер с N байт данными
                                     // будет передан в микросхему N/32 посылками
  
class at24c512
  : public extstorage
{
public:
                                        /* геометрия                                    */
  static const int TOTAL_SIZE = 65536;  /* размер микросхемы в байтах                   */
  static const int PAGE_COUNT = 512;    /* количество страниц                           */
  static const int PAGE_SIZE  = 128;    /* размер страницы в байтах                     */


  at24c512(extstorageCreationDisposition* escd);
  virtual ~at24c512();

  virtual const STORAGEGEOM* geometry(){return &this->geom;}

  virtual int write(unsigned long addr, unsigned char* data, unsigned long size);
  virtual int read (unsigned long addr, unsigned char* data, unsigned long size);
  virtual int erase(unsigned long addr, unsigned long size);
  virtual int size ();
  virtual unsigned long JEDECId();

protected:

private:

  unsigned char ucWriteBuffer[MAX_LENGTH_DATA_TO_WRITE + 3];
  static const STORAGEGEOM geom;

  serialPort*   low;
  unsigned char low_count;
  unsigned long full_size;
  PIN_SETUP     pinSetupProc;

  void WaitReady();
};

#endif /*_AT24C512_HPP_*/
