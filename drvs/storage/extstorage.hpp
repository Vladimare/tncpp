#ifndef _EXTSTORAGE_HPP_
#define _EXTSTORAGE_HPP_

#if defined(__ARMCC_VERSION)
# include "serial.hpp"
#endif

/************************ типы данных для работы с флэш-памятью *************************/

                                      /* флаги операций, доступных  для атома           */
#define GEOMF_PROGRAM     0x01        /* разрешено программирование (биты 1->0)         */
#define GEOMF_ERASE       0x02        /* разрешено стирание (биты 0->1)                 */
#define GEOMF_WRITE       0x04        /* разрешена запись (стирание + программирование) */

#define ES_WREN           0x06        // разрешение записи
#define ES_WRDI           0x04        // запрет записи

#define ES_RDID           0x9F        // чтение идентификатора

#define ES_RDSR           0x05        // чтение регистра состояния
#define ES_WRSR           0x01        // запись регистра состояния
 
#define ES_RD             0x03        // чтение
#define ES_RDF            0x0B        // быстрое чтение

#define ES_PW             0x0A        // запись страницы
#define ES_PP             0x02        // программирование страницы

#define ES_PE             0xDB        // стирание страницы

#define ES_BE             0xC7        // полное стирание
#define ES_BE4            0x20        // стирание блока 4K
#define ES_BE32           0x52        // стирание блока 32K
#define ES_SE             0xD8        // стирание сектора


                                      /* описание атома флэш-памяти                     */
typedef struct tagATOMGEOM
{
  unsigned long size;                 /* размер атома в байтах                          */
  unsigned long count;                /* общее количество атомов в памяти               */
  unsigned long flags;                /* флаги атома                                    */
}ATOMGEOM;

                                      /* описание геометрии флэш-памяти                 */
typedef struct tagSTORAGEGEOM
{
  const ATOMGEOM* atoms;              /* список атомов                                  */
  unsigned long   atomsCount;         /* количество атомов в списке                     */
}STORAGEGEOM;

#if defined(__ARMCC_VERSION)
                                      /* данные, необходимые для создания драйвера      */
                                      /*   микросхемы флэш-памяти                       */
struct extstorageCreationDisposition
  : public basicCreationDisposition
{
  serialPort*   phy;                  /* список низкоуровневых интерфейсов, которые     */
  unsigned char phy_count;            /*   облуживает данный драйвер                    */
};

#endif

                                      /* интерфейс для работы с флэш-памятью            */
class extstorage
{
public:
  virtual const STORAGEGEOM*  geometry() = 0;
  virtual int write(unsigned long addr, unsigned char* data, unsigned long size) = 0;
  virtual int read (unsigned long addr, unsigned char* data, unsigned long size) = 0;
  virtual int erase(unsigned long addr, unsigned long size) = 0;
  virtual int size ()             = 0;
  virtual unsigned long JEDECId() = 0;
};

/************** типы для поддержки структурированной работы с флэш-памятью **************/

#define SS_SIGNATURE_LENGTH 3         /* длина сигнатуры заголовка                      */

/* описание структуры данных во флэш (structured storage info) */
typedef struct tagSS_INFO
{
#define SSTYPE_UNDEFINED    0         /* тип не определен, область не структурирована   */
#define SSTYPE_CIRCULAR     1         /* циклический буфер (файлы circbuf.*)            */
#define SSTYPE_RAW          2         /* буфер с произвольным доступом, структуру данных*/
                                      /*   определяет пользователь                      */
#define SSTYPE_TREE         3         /* дерево n-го порядка                            */
#define SSTYPE_TABLE        4         /* обычная последовательная таблица с доступом по */
                                      /*   индексу                                      */
#define SSTYPE_SETTINGS     5         /* хранилище настроек                             */
#define SSTYPE_MEMMAN       6         /* менеджер памяти                                */
#define SSTYPE_UDB          7         /* БД пользователей                               */

#define SSTYPE_RWFLASH_FLASH  8       /*                                                */
#define SSTYPE_RWFLASH_EEPROM 9       /*                                                */


  unsigned char type;                 /* тип хранилища                                  */
  unsigned char reserved[3];          /* резерв, выравнивание по границе 4 байт         */
  unsigned long record_size;          /* размер одного элемента хранилища в байтах      */
  unsigned long block_size;           /* размер блока в байтах, для блока должна быть   */
                                      /*   определена операция стирания                 */
  unsigned short block_start;         /* порядковый номер блока, с которого начинается  */
                                      /*   хранилище в памяти, нумерация с 0            */
  unsigned short block_count;         /* размер хранилища в блоках                      */
}SS_INFO;


extern const char* es_JEDECDescription(unsigned long id);
extern const unsigned char ss_signs[][SS_SIGNATURE_LENGTH];


#endif /*_EXTSTORAGE_HPP_*/
