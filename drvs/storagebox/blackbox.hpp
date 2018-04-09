#ifndef _BLACKBOX_HPP_
#define _BLACKBOX_HPP_

#include "circbuf.hpp"

#include "upiobo_data.hpp"

class blackbox
{
public:
  blackbox();
  ~blackbox();

  /* инициализирует черный ящик */
  int init(extstorage* es, const SS_INFO& ssi, const ATOMGEOM& ag);

  /* добавляет запись в черный ящик, возвращает код ошибки                          *
   * если указатель number ненулевой, то возвращает также номер, присвоенный записи */
  int addEntry(unsigned char* entry, unsigned long* number = 0);

  /* считывает из черного ящика запись с указанным номером, возвращает код ошибки */
  /* если указатель utc ненулевой, то возвращает также время записи в черный ящик */
  int getEntry(unsigned char* entry, unsigned long  number, unsigned long* utc = 0);

  /* считывает набор записей из черного ящика по указанному временному диапазону */
  int getEntry(unsigned long tbegin, unsigned long tend, callback2* entry_receiver);

  /* досрочное прерывание чтения набора записей из журнала */
  int getEntryStop();

  /* возвращает общее количество записей в журнале */
  __inline unsigned long totalEntries() { return this->cbuf->getInfo()->rec_count;}

  /* возвращает номер первой записи в журнале */
  __inline unsigned long firstEntry()   { return this->cbuf->getInfo()->rec_first;}

  /* возвращает номер последней записи в журнале */
  __inline unsigned long lastEntry()    { return (this->firstEntry() + this->totalEntries() - 1);}

private:
  circularBuffer* cbuf;

  CIRCBUF_RWDATA rwd;

  /* внутренний буфер */
  unsigned char* intbuf;

  /* признак прекращения считывания */
  int breakFlag;

  /* бинарный поиск первой записи временного интервала */
  int findBegin(unsigned long tbegin, unsigned long* value);
};

#endif /*_BLACKBOX_HPP_*/
