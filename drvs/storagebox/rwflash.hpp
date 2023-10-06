#ifndef _RWFLASH_HPP_
#define _RWFLASH_HPP_

#include "extstorage.hpp"

                                      /* обертка интерфейса для работы с флэш-памятью    */

//#define RWFLASH_DEBUG


#define RWFLASH_PAGE_HEADER_SIZE 4       // размер заголовка страницы в flash памяти (в области для rwflash)

#define RWFLASHF_BE   0x01               // маска - "блок (сектор) пуст": 1 - пуст; 0 - не пуст
#define RWFLASHF_PE   0x02               // маска - "станица пуста": 1 - пуста; 0 - не пуста
#define RWFLASHF_PA   0x04               // маска - "страница актуальна": 1 - актуальна; 0 - не актуальна


class rwflash
  : public extstorage
{
public:

  rwflash(extstorage* es_flash,  const SS_INFO& ssi_flash,  const ATOMGEOM& ag_flash,
          extstorage* es_eeprom, const SS_INFO& ssi_eeprom, const ATOMGEOM& ag_eeprom);
  virtual ~rwflash();

  virtual int init();

  virtual STORAGEGEOM*  geometry() {return &this->geom;};
  virtual int write(unsigned long addr, unsigned char* data, unsigned long size);
  virtual int read (unsigned long addr, unsigned char* data, unsigned long size);
  virtual int erase(unsigned long addr, unsigned long size);
  virtual int size ();
  virtual unsigned long JEDECId();


  virtual int defragmentation();            // функция дефрагментации блока Flash памяти

  #ifdef RWFLASH_DEBUG
    void ShowFlashMemory(unsigned long addr, unsigned long size);
    void ShowEepromMemory(unsigned long addr, unsigned long size);
  #endif

  unsigned long current_block_number;          // номер текущего записываемого блока
  unsigned long current_empty_page_number;     // номер текущей пустой страницы
  unsigned long accessible_page_count;         // число доступных страниц
  unsigned long actual_page_count;             // число актуальных страниц
  unsigned long notactual_page_count;          // число неактуальных страниц


private:
  STORAGEGEOM geom;                        // геометрия flash
  ATOMGEOM    rwflash_geom;


  unsigned char* flPageBuff;                // буфер в ОЗУ для хранения страницы Flash памяти
  extstorage* es_fl;                        // указатель на flash
  extstorage* es_ee;                        // указатель на eeprom

  // информация о структуре FLash
  unsigned long  fl_block_size;           /* размер блока в байтах, для блока должна быть определена операция стирания */
  unsigned short fl_block_count;          /* размер хранилища в блоках */


  unsigned long g_fl_size;                     // размер физической страницы flash памяти

  unsigned long RWFLASH_FLASH_START_ADDRESS;   // адрес во flash начала области rwflash
  unsigned long RWFLASH_EEPROM_START_ADDRESS;  // адрес в eeprom начала области rwflash
  unsigned long RWFLASH_EEPROM_SIZE;           // размер области rwflash в eeprom

};

#endif /*_RWFLASH_HPP_*/
