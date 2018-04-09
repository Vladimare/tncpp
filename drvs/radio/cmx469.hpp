#ifndef _CMX469_HPP_
#define _CMX469_HPP_

#include "event.hpp"
#include "serial.hpp"


/******************************************************************************
 ************* статическая конфигурация драйвера радиомикросхемы **************
 ******************************************************************************/
#define RuC_CARRIER_HYST_BITS       5 /* количество битов, которое драйвер обрабатывает *
                                       *   после пропадания сигнала CARRIER_DETECT      *
                                       * если сигнал не восстанавливается, то прием     *
                                       *   пакета прекращается                          */
#define RuC_PREAMBLE_DIFFS_COUNT    5 /* максимальное количество битов, на которое      *
                                       *   принятая преамбула может отличаться от       *
                                       *   эталонной                                    */


/****************************************************************************************
 ************************************* type defines *************************************
 ****************************************************************************************/

/* базовый класс для работы с радиомикросхемой CMX469                                   *
 * для каждой конкретной платы должен быть определен свой класс-наследник от cmx469,    *
 *   в котором пользователь должен реализовать абстрактные методы работы с ногами       *
 * класс является реализацией интерфейса потоковых устройств serialPort, однако, по     *
 *   сути, работает как блочное устройство: данные принимаются и передаются блоками     *
 * передача блока инициируется операциями writeXXX. В начало каждого блока при передаче *
 *   добавляется преамбула {0xAA 0xAA 0x22 0xE4 0x6B}, а в конец - два нулевых бита     *
 *   данные блока перед передачей кодируются самовосстанавливающимся кодом Хемминга     *
 * если нет передачи блока, драйвер находится в режиме ожидания приема блока. В этом    *
 *   режиме постоянно анализируется состояние сигнала CARRIER_DETECT. Если этот сигнал  *
 *   неактивен, то драйвер не принимает данные. Если сигнал активизируется, то драйвер  *
 *   начинает фильтровать входной битовый поток в поисках преамбулы {0x22 0xE4 0x49}.   *
 *   При обнаружении преамбулы драйвер начинает принимать все последующие биты пакетами *
 *   по 12 бит. Каждый такой пакет считается кодом Хемминга, по которому собирается     *
 *   исходный байт. Таким образом, преамбула служит признаком начала очередного блока.  *
 * Окончанием блока считается пропадание сигнала CARRIER_DETECT на время, более чем     *
 *   RuC_CARRIER_HYST_BITS бит. Кратковременные пропадания сигнала CARRIER_DETECT       *
 *   игнорируются драйвером.                                                            *
 * При фильтрации преамбулы драйвер допускает неполное совпадение принятой преамбулы c  *
 *   эталонной, но не более, чем в RuC_PREAMBLE_DIFFS_COUNT битах                       *
 * Принятый драйвером блок сохраняется во внутреннем буфере, и доступен на чтение с     *
 *   помощью методов readXXX() только целиком (т.е., если во внутреннем буфере есть     *
 *   данные, но сигнал CARRIER_DETECT еще не пропал, то методы readXXX будут возвращать *
 *   нулевой результат)                                                                 *
 * Каждый раз при приеме нового блока внутренний буфер очищается. Если при этом буфер   *
 *   не пуст, то инкрементируется счетчик потерянных блоков, доступный через специальный*
 *   акцессор                                                                           */
class cmx469
  : public serialPort
{
public:
  class stat_events
  {                                   /* интерфейс к обработчику статистики модуля      */
  public:
    /* вызывается каждый раз при приеме преамбулы */
    virtual void preamble_hit() = 0;

    /* вызывается при инкременте счетчика lostblocks */
    virtual void block_missed(unsigned long lost_total) = 0;
  };

  cmx469(stat_events* se);
  virtual ~cmx469();

  /******************************* реализация serialPort ********************************/
  virtual int init      (const portSettings& ps);
  virtual const portSettings& settings();

  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int cancelReadAsync (int reason);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);

  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int cancelWriteAsync(int reason);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);

  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);

protected:
                                      /* метод первоначальной настройки ног контроллера */
  virtual void pinSetup(bool setDefault = false) = 0;
                                      /* разрешение / запрет приемника микросхемы       */
  virtual void rxEnable(unsigned char enable = 1) = 0;
                                      /* разрешение / запрет передатчика микросхемы     */
  virtual void txEnable(unsigned char enable = 1) = 0;
                                      /* возвращает 0, если сигнала CARRIER_DETECT нет, *
                                       *   иначе возвращает 1                           */
  virtual unsigned char isCarrierDetected() = 0;
                                      /* выставляет на линию бит                        */
  virtual void txbit(unsigned int bit) = 0;
  virtual unsigned char rxbit() = 0;  /* считывает с линии бит                          */

  void rxSync();                      /* обработчик rx-синхроимпульсов                  */
  void txSync();                      /* обработчик tx-синхроимпульсов                  */

private:
  stat_events*  stath;                /* обработчик статистики                          */
  CIRC_BUF128   inbuf;                /* приемный буфер                                 */
  unsigned long lostblocks;           /* счетчик потерянных пакетов (пакет был принят,  */
                                      /*   но его никто не прочитал)                    */
  unsigned char carrierHystCntr;      /* счетчик гистерезиса сигнала CARRIER_DETECT     */

#define CMX469EVT_TXCOMPLETE  0x01
#define CMX469EVT_RXCOMPLETE  0x02
  eventpack      epack;               /* набор событий драйвера                         */

  int            txerr;               /* результат асинхронной записи                   */
  unsigned char* txbuf;               /* внешний буфер для передачи                     */
  unsigned long  txbufsz;             /* размер внешнего буфера для передачи            */
  unsigned long  txbufrest;           /* количество еще не отправленных данных          */
  callback2*     txcompleter;         /* обработчик завершения асинхронной записи       */
  unsigned char  txpreambling;        /* 1 - идет передача преамбулы, счетчик txbufrest */
                                      /*   содержит количество оставшихся для передачи  */
                                      /*   байт преамбулы                               */
  unsigned char  txbits;              /* счетчик отправленных бит                       */
  unsigned long  txbitstream;         /* битовый массив на передачу                     */

  int            rxerr;               /* результат асинхронного чтения                  */
  unsigned char* rxbuf;               /* внешний приемный буфер                         */
  unsigned long  rxbufsz;             /* размер внешнего приемного буфера               */
  unsigned long  rxbufread;
  callback2*     rxcompleter;         /* обработчик завершения асинхронного чтения      */
  unsigned char  rxpreambling;        /* 1 - идет прием преамбулы, пакет не принимается */
  unsigned char  rxbits;              /* счетчик принятых бит                           */
  unsigned long  rxbitstream;         /* битовый массив на прием                        */

                                      /* служебный метод для зачистки после окончания   */
                                      /*   передачи                                     */
  void finalizeTx(int bytesSent, int status);
                                      /* служебный метод для зачистки после окончания   */
                                      /*   приема                                       */
  void finalizeRx(int bytesRead, int status);
                                      /* служебный метод, используется при timed-записи */
  void onTimedWriteComplete(int bytesSent, int err);
                                      /* служебный метод, используется при timed-чтении */
  void onTimedReadComplete(int bytesRead, int err);

                                      /* преамбула на прием                             */
  static const unsigned long rxpreamble;
                                      /* преамбула на передачу                          */
  static const unsigned char txpreamble[5];
};

#endif /*_CMX469_HPP_*/
