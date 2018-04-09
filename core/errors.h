#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "errno.h"

#ifdef TNKERNEL_PORT_CORTEXM3
# include "tn.h"
#endif

                                      /* общие ошибки                                   */
#define ERR_OK                  0     /* нет ошибок, операция выполнена успешно         */
#define ERR_DEVICE_OFF        (-256)  /* устройство отключено или не инициализировано   */
#define ERR_DEVICE_ON         (-257)  /* устройство включено                            */
#define ERR_DEVICE_BUSY       (-EBUSY)/* устройство занято                              */
#define ERR_DEVICE_NOINIT     (-ENXIO)/* устройство не инициализировано                 */
#define ERR_DEVICE_NOSYNC     (-260)  /* устройство не синхронизировано                 */
#define ERR_DEVICE_NOLINK     (-261)  /* устройство не отвечает                         */
#define ERR_BUFFER_TOO_SMALL  (-262)  /* буфер слишком мал                              */
#define ERR_BUFFER_NOT_EXIST  (-263)  /* буфер не задан                                 */
#define ERR_DATA_CORRUPTED    (-264)  /* данные повреждены, неверная контрольная сумма  */
                                      /*   или недопустимое содержимое                  */
#define ERR_DATA_SIZE         (-265)  /* неверный размер данных                         */
#define ERR_HOST_UNKNOWN      (-266)  /* получен пакет от неизвестного хоста            */
#define ERR_HOST_UNREGISTERED (-267)  /* получен пакет от неизвестного хоста, хост может*/
                                      /*   быть зарегистрирован                         */
#define ERR_PARAM             (-EINVAL)/* недопустимое значение параметра               */
#define ERR_ADDRESS           (-269)  /* недопустимое значение адреса                   */
#define ERR_NOT_SUPPORTED     (-ENODEV)/* операция не поддерживается                    */
#ifdef TNKERNEL_PORT_CORTEXM3         /* таймаут                                        */
# define ERR_TIMEOUT          TERR_TIMEOUT
#else
# define ERR_TIMEOUT          (-ETIMEOUT)
#endif
#define ERR_TERMINATED        (-272)  /* выполнение прервано                            */

//Ошибки GSM
#define ERR_GSM_ERR           (-400)  /* GSM модем вернул ERROR                         */
#define ERR_GSM_NO_CARRIER    (-401)  /* Ответ GSM модема "Нооу карьиер"                */
#define ERR_GSM_MODE          (-402)  /* Не верный режим модема, например если нужно    */
                                      /* послать команду а модем в режиме данных        */
#define ERR_GSM_NOT_REG       (-403)  /* Не зарегистрирован в сети                      */

                                      /* ошибки драйверов структурированных хранилищ    */
#define ERR_SS_TYPE           (-1000) /* неверный тип хранилища                         */
#define ERR_SS_BADHDR         (-1001) /* заголовок хранилища не соответствует оригиналу */
#define ERR_SS_DAMAGED        (-1002) /* хранилище повреждено                           */
#define ERR_SS_RECREATED      (-1003) /* хранилище создано повторно                     */
#define ERR_SS_EMPTY          (-1004) /* хранилище пусто                                */
#define ERR_SS_FULL           (-1005) /* хранилище заполнено полностью                  */
#define ERR_SS_BLOCK_DAMAGED  (-1006) /* блок хранилища поврежден                       */
#define ERR_SS_BLOCK_NOTEXIST (-1007) /* блок хранилища не существует                   */
#define ERR_SS_ENTRY_DAMAGED  (-1008) /* элемент хранилища поврежден                    */
#define ERR_SS_ENTRY_EMPTY    (-1009) /* элемент хранилища пуст                         */
#define ERR_SS_ENTRY_NOEMPTY  (-1010) /* элемент хранилища не пуст                      */
#define ERR_SS_ENTRY_NOTEXIST (-1011) /* элемент хранилища не существует                */
#define ERR_SS_ENTRY_NOACTUAL (-1012) /* элемент хранилища не актуален                  */
#define ERR_SS_ATOM_NOTEXIST  (-1013) /* отсутствие атома                               */

                                      /* ошибки пользовательской базы данных            */
#define ERR_UDB_NOT_FOUND     (-1100) /* пользователь не найден                         */
#define ERR_UDB_ALREADY_EXIST (-1101) /* пользователь уже существует                    */
#define ERR_UDB_NODE_OVF_OK   (-1102) /* узел индексного дерева переполнен (только что, */
                                      /*   операция вставки выполнена успешно), перед   */
                                      /*   следующей вставкой требуется выполнить       */
                                      /*   деление узла                                 */
#define ERR_UDB_NODE_OVF_ERR  (-1103) /* узел индексного дерева переполнен предыдущей   */
                                      /*   операцией, вставка не выполнена, требуется   */
                                      /*   сначала разделить узел                       */
#define ERR_UDB_NODE_NOOVF    (-1104) /* узел индексного дерева не переполнен, деление  */
                                      /*   узла не может быть выполнено                 */
#define ERR_UDB_NODE_NOT_LEAF (-1105) /* узел индексного дерева не является листом,     */
                                      /*   вставка нарушит балансировку дерева          */
#define ERR_UDB_PROFILE_BAD   (-1106) /* обращение к несуществующему профилю            */
                                      /*   редактирования                               */
#define ERR_UDB_GEOMETRY_BAD  (-1107) /* БД не может работать с носителем заданной      */
                                      /*   геометрии                                    */

#endif /*_ERRORS_H_*/
