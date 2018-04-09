#ifndef _CIRCBUF_HPP_
#define _CIRCBUF_HPP_

#include "extstorage.hpp"

/* данные циклического буфера */
typedef struct tagCIRCBUF_INFO
{
  unsigned char valid;                /* буфер правильный, повреждений нет              */
  unsigned long block_first;          /* номер первого логического блока в хранилище    */
  unsigned long rec_first;            /* номер первой логической записи в хранилище     */
  unsigned long rec_count;            /* текущее количество записей в буфере            */
  unsigned short rec_block_count;     /* количество записей, помещающихся в блок        */
}CIRCBUF_INFO;

/* заголовок блока циклического буфера */
typedef struct tagCIRCBUF_BLOCKHDR
{
                                      /* сигнатура заголовка                            */
  unsigned char signature[SS_SIGNATURE_LENGTH];
  union
  {
    struct
    {
      unsigned char nfirst: 1;        /* 0 - данный блок является головой               */
      unsigned char nfull : 1;        /* 0 - блок полон                                 */
    }bits;
    unsigned char byte;
  }flags;                             /* флаги заголовка                                */
  unsigned long rec_first;            /* номер первой записи в блоке                    */
  SS_INFO       ssi;                  /* описание циклического буфера как хранилища     */
  unsigned short cs[4];               /* массив контрольных сумм                        */
                                      /* контрольные суммы рассчитываются (и пишутся    */
                                      /*   в память) один раз для всех комбинаций битов */
                                      /*   nfirst и nfull:                              */
                                      /*     nfull  nfirst  cs                          */
                                      /*       0       0     0                          */
                                      /*       0       1     1                          */
                                      /*       1       0     2                          */
                                      /*       1       1     3                          */
}CIRCBUF_BLOCKHDR;

/* заголовок элемента циклического буфера */
typedef struct tagCIRCBUF_ENTRYHDR
{
                                        /* признаки использования записи                */
#define CIRCBUF_ENTRYHDR_UNUSED 0xFFFF  /* запись свободна                              */
#define CIRCBUF_ENTRYHDR_USED   0x0000  /* запись занята                                */
  unsigned short uses;                  /* признак использования записи                 */
  unsigned short cs;                    /* контрольная сумма записи                     */
}CIRCBUF_ENTRYHDR;

/* данные для операций чтения / записи */
typedef struct tagCIRCBUF_RWDATA
{
  unsigned long number;               /* номер, присвоенный при записи, или запрошенный */
                                      /*   при чтении                                   */
  unsigned char* pdata;               /* указатель на данные                            */
}CIRCBUF_RWDATA;


class circularBuffer
{
public:
  circularBuffer(extstorage* es, const SS_INFO& ssi, const ATOMGEOM& bg);
  ~circularBuffer();

  CIRCBUF_INFO* getInfo() {return &this->hdr;}

  int init();                         /* инициализирует класс, проверяет данные во флэш,*/
                                      /*   при необходимости (пере)создает буфер        */
  int read_entry(CIRCBUF_RWDATA* rwd);/* считывает указанную запись                     */
                                      /* сохраняет запись в буфере                      */
  int write_entry(CIRCBUF_RWDATA* rwd);

private:
  extstorage*       sphy;             /* интерфейс доступа к хранилищу                  */
  const SS_INFO&    ssinfo;           /* описание хранилища                             */
  const ATOMGEOM&   blockgeom;        /* геометрия блока                                */
  CIRCBUF_BLOCKHDR  blockhdr;         /* заголовки блоков                               */
  CIRCBUF_INFO      hdr;              /* заголовок буфера                               */
  unsigned char*    entrybuf;         /* внутренний буфер под хранение записи           */


  int validate();                     /* проверяет правильность буфера                  */
  int create();                       /* создает буфер                                  */
                                      /* проверяет правильность блока                   */
  int validate_blockhdr(unsigned long block);
                                      /* создает блок                                   */
  int create_blockhdr(unsigned long block, unsigned char first, unsigned long rf);
                                      /* обновляет заголовок блока                      */
  int update_blockhdr(unsigned long block, unsigned char first, unsigned char full, unsigned long rf);
                                      /* проверяет запись                               */
  int validate_entry(unsigned long block, unsigned long entry);
                                      /* создает запись                                 */
  int create_entry(unsigned long block, unsigned long entry, unsigned char* pdata);
};

#endif /*_CIRCBUF_HPP_*/
