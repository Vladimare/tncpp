#include "cfgtree.hpp"
#include "errors.h"
#include "corelib.h"
#include <string.h>

/* конструктор */
cfgtree::cfgtree(const SS_INFO& ssi, extstorage* estrg)
{
  this->bufInt  = 0;
  this->bHeaders= 0;
  this->bCount	= 0;

  this->layout  = ssi;
  this->carrier = estrg;
}

/* деструктор */
cfgtree::~cfgtree()
{
  if (this->bufInt != NULL) 
    delete  this->bufInt;

  if (this->bHeaders != NULL)
    for(int i = 0; i < this->bCount; i++)
      if (this->bHeaders[i].cache != NULL)
        delete this->bHeaders[i].cache;
}

/* инициализация дерева конфирурации с проверкой всего дерева */
int cfgtree::init(CFGENTRY* headers, int size)
{
  int i, offset, ret = ERR_OK, maxSize;

  this->bHeaders  = new CFGHDR[size];

  this->bCount    = size;
                                      /* начальное смещение                   */
  offset = layout.block_size * (layout.block_start);
  maxSize = 0;

                                      /* формируем внутреннюю таблицу         *
                                       * заголовков                           */
  for( i = 0; i < size; i++)
  {
    this->bHeaders[i].number  = headers[i].blk_number;
                                      /* вычисляем смещение для каждого блока */
    this->bHeaders[i].lookup  = offset;
    this->bHeaders[i].size    = headers[i].blk_sz;
    this->bHeaders[i].lev     = 1 + (headers[i].blk_flags & 3);
    offset += this->bHeaders[i].size + 5;/* плюс заголовок и CRC16            */

                                      /* выч. максимальную длинну блока данных*/
    if (maxSize  < this->bHeaders[i].size) 
      maxSize = this->bHeaders[i].size;

                                      /* разервируем буферы в ОЗУ только для  *
                                       * тех блоков которые требуют           *
                                       *  дублирования                        */
    if(CFG_CACHED & headers[i].blk_flags)
      this->bHeaders[i].cache = new unsigned char [bHeaders[i].size];
    else
      this->bHeaders[i].cache = NULL;
  }
                                      /* макс размер + CRC + заголовок        */
  this->bufInt = new unsigned char[maxSize + 3 + 2];


                                      /* проверка содержания блоков           */
  for( i = 0; i < size; i++)
  {
                                      /* читаем блок                          */
    ret = this->carrier->read(this->bHeaders[i].lookup, bufInt, this->bHeaders[i].size + 5);
    if (ret < ERR_OK) return ret;

    unsigned short currentBsize;
    currentBsize = bufInt[1];
    currentBsize |= ( 0x1f & bufInt[2]) << 8;
                                      /* проверка длинны                      */
    if (currentBsize != this->bHeaders[i].size)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }

    unsigned char  lev;
    lev = 1 + (0x3 & (bufInt[2] >> 5));
                                      /* проверка уровня вложенности          */
    if (lev != this->bHeaders[i].lev)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }

                                      /* проверка номера блока                */
    if (bufInt[0]  != this->bHeaders[i].number)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }
                                      /* вычисление контрольной суммы         */
    unsigned short crc, crcStrored;

    crc = crc16(bufInt, 3 + currentBsize, 0);

    crcStrored = bufInt[3 + currentBsize];
    crcStrored |= bufInt[3 + currentBsize + 1] << 8;

                                      /* проверка контрольной суммы           */
    if (crc != crcStrored)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }
                                      /* если блок требует дублирования, то   * 
                                       * копируем во внутренний буфер         */
    if(this->bHeaders[i].cache != NULL)
      memcpy(this->bHeaders[i].cache, &this->bufInt[3], this->bHeaders[i].size);
  }
  return ret;
}

/* поиск номера блока по индексу */
int cfgtree::index2num(unsigned long index)
{
  int i, j, lev;
  unsigned long curIndex = 0;

  for( i = 0; i < this->bCount; i++)
  {
                                      /* вычисляем индекс для текущего блока  */
    lev = this->bHeaders[i].lev;
                                      /* обнуляем все не старшие значения     *
                                       * уровня                               *
                                       * старшие уровни в младших байтах.     */
    for( j = 0; j <= (4 - lev) ; j++)
      curIndex &= ~(0xff << (8 * (3 - j)));
                                      /* обновляем текущее значение уровня    */
    curIndex |= (this->bHeaders[i].number << (8 * (lev - 1)));

    if (curIndex == index)            /* попали ?                             */
      return i;                       /* поиск завершился удачно              */
  }
  return ERR_ADDRESS;                 /* записи с указанным индексом нет      */
}

/* формирование заголовка, для записи во внешний носитель  */
int cfgtree::packHeadr(int num)
{
  this->bufInt[0] = this->bHeaders[num].number;
  this->bufInt[1] = (unsigned char)this->bHeaders[num].size;
  this->bufInt[2] = 0x1f & (this->bHeaders[num].size >> 8);
  this->bufInt[2] |= (this->bHeaders[num].lev - 1) << 5;
  if (this->bHeaders[num].cache != NULL)
    this->bufInt[2] |= 0x80;
  return ERR_OK;
}

/* чтение конфигурации 
приемный буфер мб больше нашей записи если он будет меньше, 
то считаем только запрошенное количество.
При bufsz = 0 будет не считывание, а контроль целостности блока */
int cfgtree::read(unsigned long index, unsigned char* bufExt, int bufsz)
{
  int bN, ret;
  CFGHDR* curHeader;

  bN = index2num(index);
  if (bN < ERR_OK)
    return bN;                        /* мимо                                 */

  curHeader = &this->bHeaders[bN];

  if (bufsz > curHeader->size)        /* корректируем длинну                  */
    bufsz = curHeader->size;

  if(curHeader->cache != NULL)
  {
                                      /* если запись дублирована в ОЗУ, то ее *
                                       * не нужно контролировать              */
    memcpy(bufExt, curHeader->cache, bufsz);
  }else
  {
                                      /* читаем выбранный блок                */
    ret = this->carrier->read(curHeader->lookup, bufInt, curHeader->size + 5);
    if (ret < ERR_OK) return ret;
                                      /* вычисление контрольной суммы         */
    unsigned short crc, crcStrored;

    crc = crc16(bufInt, 3 + curHeader->size, 0);

    crcStrored = bufInt[3 + curHeader->size];
    crcStrored |= bufInt[3 + curHeader->size + 1] << 8;

                                      /* проверка контрольной суммы           */
    if (crc != crcStrored)
    {
      return ERR_DATA_CORRUPTED;
    }
                                      /* данные корректны, обновим внешний    *
                                       * буффер                               */
    memcpy(bufExt, &this->bufInt[3], bufsz);
  }
  return ERR_OK;
}

/* запись конфигурации */
int cfgtree::write(unsigned long index, unsigned char* bufExt, int bufsz)
{
  int blockNumber, ret;
  CFGHDR* curHeader;

  blockNumber = index2num(index);
  if (blockNumber < ERR_OK)
    return blockNumber;               /* мимо                                 */

  curHeader = &this->bHeaders[blockNumber];

  if (bufsz > curHeader->size)
  {                                   /* меньше можно, больше - нет           */
    return ERR_DATA_SIZE;
  }

  this->packHeadr(blockNumber);       /* копируем заголовок                   */

                                      /* копируем изменяемую часть данных     */
  memcpy(&this->bufInt[3], bufExt, bufsz); 

                                      /* если изменяемая часть меньше общей   */
                                      /* длинны, дочитываем остаток           */
  if (bufsz != curHeader->size)
  {
    ret = this->carrier->read(
            curHeader->lookup + 3 + bufsz,
            bufInt,
            curHeader->size - bufsz);
    if (ret < ERR_OK) return ret;
  }

                                      /* пересчитываем CRC                    */
  unsigned short crc = crc16(bufInt, 3 + this->bHeaders[blockNumber].size, 0);

                                      /* добавляем новую CRC в конец пакета   */
  bufInt[3 + curHeader->size]     = (unsigned char)crc;
  bufInt[3 + curHeader->size + 1] = crc >> 8;

                                      /* финал, пишем все - данные, заголовок */
                                      /* и контрольную сумму                  */
  ret = this->carrier->write(curHeader->lookup, bufInt, curHeader->size + 5);
  if (ret < ERR_OK) return ret;

  return ERR_OK;
}
