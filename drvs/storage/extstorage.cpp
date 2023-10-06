#include <string.h>

#include "compilerspec.h"
#include "extstorage.hpp"

                                      /* сигнатуры типов хранилищ                       */
const unsigned char ss_signs[][SS_SIGNATURE_LENGTH] =
{
  {'X', 'X', 'X'},                    /* тип не определен, область не структурирована   */
  {'C', 'Y', 'C'},                    /* циклический буфер (файлы circbuf.*)            */
  {'R', 'A', 'W'},                    /* буфер с произвольным доступом, структуру данных*/
                                      /*   определяет пользователь                      */
  {'T', 'R', 'E'},                    /* дерево n-го порядка                            */
  {'T', 'B', 'L'},                    /* обычная последовательная таблица с доступом по */
                                      /*   индексу                                      */
  {'S', 'E', 'T'},                    /* хранилище настроек                             */
  {'M', 'A', 'N'}                     /* менеджер памяти                                */
};


#define ES_SUP_JEDEC_ID_COUNT   4
static unsigned long JEDEC_IDs[ES_SUP_JEDEC_ID_COUNT] =
{
  0x00152020,                         /* STM M25P16                                     */
  0x00162020,                         /* STM M25P32                                     */
  0x00182020,                         /* STM M25P128                                    */
  0x00148020                          /* STM M25PE80                                    */
};

static const char* JEDEC_strings[ES_SUP_JEDEC_ID_COUNT] =
{
  "STM, memtype M25P, capacity 16Mbit",
  "STM, memtype M25P, capacity 32Mbit",
  "STM, memtype M25P, capacity 128Mbit",
  "STM, memtype M25PE, capacity 8Mbit",
};


/* функция возвращает текстовую расшифровку JEDEC-идентификатора flash-памяти */
const char* es_JEDECDescription(unsigned long id)
{
  for(int i = 0; i < ES_SUP_JEDEC_ID_COUNT; i++)
  {                                   /* линейный поиск по таблице доступных JEDEC ID   */
    if(id == JEDEC_IDs[i])
      return JEDEC_strings[i];
  }

  return "unknown JEDEC id";
}
