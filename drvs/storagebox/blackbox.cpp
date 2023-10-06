#include <string.h>

#include "blackbox.hpp"
#include "errors.h"
#include "rtc17xx.hpp"
#include "syslog.hpp"


blackbox::blackbox()
{
}

blackbox::~blackbox()
{
  delete this->cbuf;
  delete this->intbuf;
}

int blackbox::init(extstorage* es, const SS_INFO& ssi, const ATOMGEOM& ag)
{
  this->cbuf    = new circularBuffer(es, ssi, ag);
  this->intbuf  = new unsigned char[lifesignal_base::LSBIN_MAX_SZ + sizeof (unsigned long)];

  return this->cbuf->init();
}

int blackbox::addEntry(unsigned char* entry, unsigned long* number)
{
  int err = ERR_OK;

  this->rwd.number = 0;               /* готовим данные для записи                      */
  this->rwd.pdata  = this->intbuf;

  unsigned long utc = 0;
  if(rtc17xx::g_rtc)
  {                                   /* часы существуют                                */
    err = rtc17xx::g_rtc->getRTC(&utc, 0);
    if((err != ERR_OK) && (err != ERR_DEVICE_NOSYNC))
      return err;
  }

                                      /* добавляем время в начало                       */
  memcpy(this->intbuf, (unsigned char *) &utc, sizeof(unsigned long));
  memcpy(this->intbuf + sizeof (unsigned long), entry, lifesignal_base::LSBIN_MAX_SZ);
                                      /* пишем                                          */
  err = this->cbuf->write_entry(&this->rwd);
  if(err != ERR_OK)
  {
    if(syslog::g_log)
      syslog::g_log->write(LOG_LEVEL_ALERT, "blackbox: failed to add entry (err == %d)\r\n", err);

    return err;
  }

  if(number)
    *number = this->rwd.number;       /* если есть указатель на номер, копируем в него  */
                                      /*   номер новой записи                           */

  if(syslog::g_log)
    syslog::g_log->write(LOG_LEVEL_FLOW, "blackbox: entry %010d added\r\n", this->rwd.number);

  return ERR_OK;
}

int blackbox::getEntry(unsigned char* entry, unsigned long  number, unsigned long* utc)
{
  int err = ERR_OK;

  this->rwd.number = number;          /* готовим данные для чтения                      */
  this->rwd.pdata  =  this->intbuf;
                                      /* читаем                                         */
  err = this->cbuf->read_entry(&this->rwd);
  if(err != ERR_OK)
    return err;
                                      /* извлекаем время, если требуется                */
  if(utc)
    memcpy((unsigned char*)utc, this->intbuf, sizeof(unsigned long));
                                      /* копируем запись со смещением на длину времени  */
  memcpy(entry, this->intbuf + sizeof(unsigned long), lifesignal_base::LSBIN_MAX_SZ);

  return ERR_OK;
}

/* бинарный поиск начала временного интервала в журнале 
  запрошенное время может оказаться между 2-х записей журнала
  например, журнал содержит 2 3 4 8 9 10
  запрос tbegin = 5 выдаст индекс четвертой записи равной 8,
  так как 5 находится между 4 и 8 
  */
int blackbox::findBegin(unsigned long tbegin, unsigned long* value)
{
  unsigned long first, last, center;
  unsigned long utcFirst, utcLast, utcCenter;
  int ret;

                                      /* иниициализация переменых цикла                 */
  first  = firstEntry();
  last   = lastEntry();
                                      /* каждую иттерацию интервал делится на половину  * 
                                       * так что бы искомое значение было внутри        * 
                                       * цикл завершается когда границы сойдуться вместе*/
  while (1)
  {
                                      /* до начала считывания можно прерваться          */
    if(this->breakFlag)
      return ERR_TERMINATED;
                                      /* получаем значение времени в начале интервала   */
    ret = this->getEntry(intbuf, first, &utcFirst);
    if (ret < ERR_OK) 
      return ret;

                                      /* до начала считывания можно прерваться          */
    if(this->breakFlag)
      return ERR_TERMINATED;
                                      /* получаем значение времени в конце интервала    */
    ret = this->getEntry(intbuf, last, &utcLast);
    if (ret < ERR_OK) 
      return ret;
                                      /* проверяем корректность границ интервала        */
    if ((tbegin <utcFirst) || (utcLast < tbegin))
      return ERR_SS_ENTRY_NOTEXIST;

                                      /* проверяем на досрочное завершение, возможно нам* 
                                       * повезло и мы попали пальцем в небо             */
    if (utcFirst == tbegin)
    {
      *value = first;
      return ERR_OK;
    }
    if (utcLast == tbegin)
    {
      *value = last;
      return ERR_OK;
    }

                                      /* проверяем условие завершения цикла             */
    if ((last - first) <= 1)
    {
                                      /* попадает ли искомое значение в интервал?       */
      if ((utcFirst < tbegin) && (tbegin < utcLast))
      {
        *value = last;
        return ERR_OK;
      }
      else
        return ERR_SS_ENTRY_NOTEXIST;
    }
    else
    {                                 /* формируем новый интревал                       */

                                      /* индекс пробного центрального значения          */
      center = first + (last - first) / 2;

                                      /* до начала считывания можно преваться           */
      if(this->breakFlag)
        return ERR_TERMINATED;
                                      /* получаем значение времени в середине интервала */
      ret = this->getEntry(intbuf, center, &utcCenter);
      if (ret < ERR_OK) 
        return ret;

                                      /* выбираем какую половину интрервала будем       */
                                      /* использовать в следующую итерацию              */
      if (utcCenter < tbegin)
      {
        first = center;
      }
      else
        last = center;
    }
  } 
}

/*  */
int blackbox::getEntryStop()
{
  this->breakFlag = 1;                /* признак прекращения считывания черного ящика   */
  return ERR_OK;
}

/*  */
int blackbox::getEntry(unsigned long tbegin, unsigned long tend, callback2* entry_receiver)
{
  if (this->totalEntries() == 0) return ERR_SS_EMPTY;
  
  unsigned long first, last, i, utc;
  int ret;
                                      /* если начали считывание, то нет запроса на      * 
                                       * прекращение считывания                         */
  this->breakFlag = 0;
                                      /* ищем начало интервала                          */
  ret = this->findBegin(tbegin, &first);
  if (ret < ERR_OK) return ret;

                                      /* запоминаем последнее значение                  */
  last = lastEntry();
                                      /* начинаем перебор                               */
  for (i = first; i <= last; i++)
  {
                                      /* до начала считывания можно преваться           */
    if(this->breakFlag)
      return ERR_TERMINATED;
                                      /* считываем текущую запись                       */
    this->rwd.number = i;
    this->rwd.pdata  = this->intbuf;
    ret = this->cbuf->read_entry(&this->rwd);
    if(ret != ERR_OK)
      return ret;
                                      /* извлекаем время                                */
    memcpy((unsigned char *) utc, this->intbuf, sizeof(unsigned long));
                                      /* проверяем окончание временного интервала       */
    if (utc > tend) break;
                                      /* передаем считанное значение в callback         */
    if (entry_receiver)
      (*entry_receiver)((unsigned long) (intbuf + sizeof(unsigned long)), i);
  }

  return ERR_OK;
}
