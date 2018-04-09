#ifndef _SWHEADERS_H_
#define _SWHEADERS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"
#include <stdint.h>
#include <time.h>

/* структура заголовка основной программы */
typedef __packed struct tagMainSWheader
{
  uint32_t signature;                 /* сигнатура программы                  */
  uint32_t revision;                  /* версия программы                     */
  uint32_t size;                      /* размер программы                     */
  uint32_t crc;                       /* контрольная сумма программы          */
  time_t time;                        /* дата и время последнего обновления   *
                                       * программы                            */
  char* descriptor;                   /* строковое описание программы         */
}MainSWheader;

/* структура заголовка загрузчика */
typedef __packed struct tagBootSWheader
{
uint32_t ID;                          /* уникальный идентификатор устройства  */
uint32_t revision;                    /* версия программы                     */
uint32_t size;                        /* размер программы                     */
uint32_t crc;                         /* контрольная сумма                    */
time_t time;                          /* дата последнего обновления загрузчика*/
char* descriptor;                     /* строковое описание программы         */
uint64_t MAC;                         /* сетевой адрес устройства             */
uint32_t hardware;                    /* версия аппаратной платформы, соотв   *
                                       * номеру платы                         */
}BootSWheader;


#define MAIN_SW_OFFSET    (0x8000)
#define BOOT_SW_OFFSET    (0x0)

#define SW_HEADER_OFFSET  (0x200)

#define bootSWheader ((BootSWheader*) (BOOT_SW_OFFSET + SW_HEADER_OFFSET))
#define mainSWheader ((MainSWheader*) (BOOT_SW_OFFSET + SW_HEADER_OFFSET))

#ifdef __cplusplus
}
#endif


#endif /*_SWHEADERS_H_*/
