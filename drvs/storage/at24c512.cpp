#include <string.h>

#include "at24c512.hpp"
#include "errors.h"
#include "lpc17xx_defs.h"

  //********************************* ВНИМАНИЕ!!!!! *****************************
  //************* ДРАЙВЕР ПОДДЕРЖИВАЕТ ТОЛЬКО ОДНУ МИКРОСХЕМУ EEPROM ************

const ATOMGEOM at24c512_geom[] =
{
  {1, at24c512::TOTAL_SIZE, GEOMF_WRITE}, /* байты, могут быть записаны */
};

const STORAGEGEOM at24c512::geom = {at24c512_geom, sizeof(at24c512_geom) / sizeof(ATOMGEOM)};


at24c512::at24c512(extstorageCreationDisposition* escd)
{
  this->low       = escd->phy;
  this->low_count = escd->phy_count;
                                       /* общий объем доступной памяти                   */
  this->full_size = TOTAL_SIZE * this->low_count;
}

at24c512::~at24c512()
{
}

/* записывает данные в память, начиная с адреса addr (адрес указывается в байтах)                 *
 * если данные пересекают границы страницы, то автоматически переходит на следующую страницу      *
 * если данные пересекают границы микросхемы, то запись не производится и возвращается ошибка     *
 * блокирует выполнение до окончания записи                                                       */
int at24c512::write(unsigned long addr, unsigned char* data, unsigned long size)
{
  unsigned long rest_to_page_end;      // число байт до конца страницы
  unsigned long cur_addr_ic = addr;    // текущий адрес в микросхеме
  unsigned long cur_addr_buf = 0;      // текущий адрес в буфере данных
  int res = ERR_OK;                    // возвращаемый функцией результат
  unsigned long sz = 0;                // объем записываемых через драйвер И2С данных


  if(!size)
    return ERR_OK;                    /* нечего записывать                              */

  if(addr >= this->full_size)
    return ERR_ADDRESS;               /* адрес выходит за границы доступной памяти      */

  if((addr + size) > this->full_size)
    return ERR_DATA_SIZE;             /* данные не помещаются в доступную память        */

                                      /*   запись                                       */


  while(size)                        // пишем пока есть данные
  {
    if(size < MAX_LENGTH_DATA_TO_WRITE)  // если осталось байт меньше чем размер буфера
      sz = size;                         // то будем копировать то что осталось
    else
      sz = MAX_LENGTH_DATA_TO_WRITE;     // иначе заполним буфер целиком

    rest_to_page_end = PAGE_SIZE - cur_addr_ic % PAGE_SIZE;            // смотрим сколько осталось до конца страницы

    if(rest_to_page_end < sz)                                          // если буфер не уместится
      sz = rest_to_page_end;                                           // то запишем сколько есть

    this->ucWriteBuffer[0] = EEPROM_ADDRESS;                           // готовим заголовок
    this->ucWriteBuffer[1] = (cur_addr_ic >> 8) & 0xff;
    this->ucWriteBuffer[2] =  cur_addr_ic       & 0xff;
    memcpy(&this->ucWriteBuffer[3], &data[cur_addr_buf], sz);          // буферизуем данные для записи

    res = this->low->write(this->ucWriteBuffer, sz+3);                 // собственно и пишем
    if(res < 0)                                                  // если были ошибки, то на выход
      return res;

    this->WaitReady();

    cur_addr_buf += sz;                                                // переходим к первому непереданному байту данных
    cur_addr_ic += sz;                                                 // переходим к следующему адресу
    size -= sz;                                                        // уменьшаем число непереданных байт на величину фактически
                                                                       // переданных байт
 }

  return ERR_OK;
}

/* считывает данные из памяти, начиная с адреса addr (адрес указывается в байтах)                 *
 * если данные пересекают границы страницы, то автоматически переходит на следующую страницу      *
 * если данные пересекают границы микросхемы, то чтение не производится и возвращается ошибка     *
 * блокирует выполнение до окончания чтения                                                       */
int at24c512::read(unsigned long addr, unsigned char* data, unsigned long size)
{
  unsigned long rest_to_page_end;      // число байт до конца страницы
  unsigned long cur_addr_ic = addr;    // текущий адрес в микросхеме
  unsigned long cur_addr_buf = 0;      // текущий адрес в буфере данных
  int res = ERR_OK;                    // возвращаемый функцией результат
  unsigned long sz = 0;                // объем запрашиваемых через драйвер И2С данных

  if(!size)
    return ERR_OK;                    // нечего считывать

  if(addr >= this->full_size)
    return ERR_ADDRESS;               // адрес выходит за границы доступной памяти

  if((addr + size) > this->full_size)
    return ERR_DATA_SIZE;             // данные не помещаются в доступную память


  while(size)                        // читаем пока есть данные
  {
    rest_to_page_end = PAGE_SIZE - cur_addr_ic%PAGE_SIZE;

    if(size < rest_to_page_end)
      sz = size;
    else
      sz = rest_to_page_end;

    data[cur_addr_buf] = EEPROM_ADDRESS;
    data[cur_addr_buf+1] = (cur_addr_ic>>8)&0xff;
    data[cur_addr_buf+2] = cur_addr_ic & 0xff;
    res = this->low->write(&data[cur_addr_buf], 3);
    if(res < 0)                                                  // если были ошибки, то на выход
      return res;
    res = this->low->read(&data[cur_addr_buf], sz);
    if(res < 0)                                                  // если были ошибки, то на выход
      return res;

    this->WaitReady();

    cur_addr_buf += sz;
    cur_addr_ic += sz;
    size -= sz;
 }
  return ERR_OK;
}

/* стирает указанную область памяти                                     */
int at24c512::erase(unsigned long addr, unsigned long size)
{
  unsigned long rest_to_page_end;      // число байт до конца страницы
  unsigned long cur_addr_ic = addr;    // текущий адрес в микросхеме
  int res = ERR_OK;                    // возвращаемый функцией результат
  unsigned long sz = 0;                // объем записываемых через драйвер И2С данных


  if(!size)
    return ERR_OK;                    /* нечего записывать                              */

  if(addr >= this->full_size)
    return ERR_ADDRESS;               /* адрес выходит за границы доступной памяти      */

  if((addr + size) > this->full_size)
    return ERR_DATA_SIZE;             /* данные не помещаются в доступную память        */

                                      /*   запись                                       */


  while(size)                        // пишем пока есть данные
  {
    if(size < MAX_LENGTH_DATA_TO_WRITE)  // если осталось байт меньше чем размер буфера
      sz = size;                         // то будем копировать то что осталось
    else
      sz = MAX_LENGTH_DATA_TO_WRITE;     // иначе заполним буфер целиком

    this->ucWriteBuffer[0] = EEPROM_ADDRESS;                           // готовим заголовок
    this->ucWriteBuffer[1] = (cur_addr_ic>>8)&0xff; 
    this->ucWriteBuffer[2] = cur_addr_ic & 0xff;
    memset(&this->ucWriteBuffer[3], EEPROM_CLEAR_VALUE, sz);          // буферизуем данные для записи

    rest_to_page_end = PAGE_SIZE - cur_addr_ic%PAGE_SIZE;              // смотрим сколько осталось до конца страницы

    if(rest_to_page_end < sz)                                          // если буфер не уместится
      sz = rest_to_page_end;                                           // то запишем сколько есть

    res = this->low->write(this->ucWriteBuffer, sz+3);                 // собственно и пишем
    if(res < 0)                                                  // если были ошибки, то на выход
      return res;

    this->WaitReady();


    cur_addr_ic += sz;                                                 // переходим к следующему адресу
    size -= sz;                                                        // уменьшаем число непереданных байт на величину фактически
                                                                       // переданных байт
 }

  return ERR_OK;
}

int at24c512::size()
{
  return this->full_size;
}

unsigned long at24c512::JEDECId()
{

  return 0;
}

void at24c512::WaitReady()
{
  ucWriteBuffer[0] = EEPROM_ADDRESS; 
  while(this->low->write(this->ucWriteBuffer, 1) < 0)
  {
  }
}
