#ifndef _SERIAL_HPP_
#define _SERIAL_HPP_

#include "tn.h"
#include "types.h"
#include "types_ext.hpp"


#define UARTSIZE_5    0
#define UARTSIZE_6    1
#define UARTSIZE_7    2
#define UARTSIZE_8    3

#define SSPSIZE_4     3
#define SSPSIZE_5     4
#define SSPSIZE_6     5
#define SSPSIZE_7     6
#define SSPSIZE_8     7
#define SSPSIZE_9     8
#define SSPSIZE_10    9
#define SSPSIZE_11    10
#define SSPSIZE_12    11
#define SSPSIZE_13    12
#define SSPSIZE_14    13
#define SSPSIZE_15    14
#define SSPSIZE_16    15

#define I2CSIZE_8     8

#define STOPBITS_1    0
#define STOPBITS_2    1

#define PARITY_NONE   0
#define PARITY_ODD    1
#define PARITY_EVEN   3
#define PARITY_MARK   5
#define PARITY_SPACE  7

#define BR_110        100
#define BR_300        300
#define BR_600        600
#define BR_1200       1200
#define BR_2400       2400
#define BR_4800       4800
#define BR_9600       9600
#define BR_10472      10472
#define BR_12800      12800
#define BR_14400      14400
#define BR_16457      16457
#define BR_19200      19200
#define BR_23040      23040
#define BR_28800      28800
#define BR_38400      38400
#define BR_57600      57600
#define BR_115200     115200

#define BR_100000     100000  // I2C 100kHz
#define BR_400000     400000  // I2C 400kHz

#define BR_5000000    5000000
#define BR_6000000    6000000

#define DCD  0x0001                   /* in                                             */
#define RXD  0x0002                   /* in                                             */
#define TXD  0x0004                   /* out                                            */
#define DTR  0x0008                   /* out                                            */
#define GND  0x0010                   /*  */
#define DSR  0x0020                   /* in                                             */
#define RTS  0x0040                   /* out                                            */
#define CTS  0x0080                   /* in                                             */
#define RI   0x0100                   /* in                                             */

#define PURGE_RX 0x01
#define PURGE_TX 0x02

typedef struct tagPortSettings
{
  unsigned char dataBits;
  unsigned char stopBits;
  unsigned char parity;
  unsigned long baudrate;
  unsigned long pclk;
}portSettings;


#ifdef __cplusplus

typedef basicCreationDisposition serialCreationDisposition;


class serialPort
{
public:
  virtual int init      (const portSettings& ps) = 0;
  virtual const portSettings& settings() = 0;

  /* синхронное блокирующее чтение, синхронное чтение с таймаутом, и асинхронное чтение *
   * все методы чтени€ должны возвращать доступное дл€ считывани€ количество данных во  *
   *   внутренних FIFO, если в параметре buf передан 0                                  *
   * при вызове асинхронного чтени€ допускаетс€ передавать нулевой обработчик           *
   * асинхронный обработчик вызываетс€ в контексте прерывани€!                          */
  virtual int read      (unsigned char* buf, int  bufsz) = 0;
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac) = 0;
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout) = 0;

  /* синхронна€ блокирующа€ запись, синхронна€ запись с таймаутом, и асинхронна€ запись *
   * асинхронный обработчик вызываетс€ в контексте прерывани€!                          */
  virtual int write     (unsigned char* buf, int  bufsz) = 0;
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac) = 0;
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout) = 0;

  /* отмена асинхронных операций                                                        *
   * эти вызовы также отмен€ют операции чтени€ с таймаутом (эти операции реализуютс€    *
   *   на основе асинхронных вызовов)                                                   */
  virtual int cancelReadAsync (int reason) = 0;
  virtual int cancelWriteAsync(int reason) = 0;

  /* очистка буферов */
  virtual int purge     (unsigned char queue) = 0;

  /* синхронна€ отправка символа в порт */
  virtual int sendchar  (unsigned char c) = 0;

  /* настройки порта, которые примен€ютс€ по умолчанию */
  static const portSettings defUARTSettings;
  static const portSettings defSSPSettings;
  static const portSettings defI2CSettings;
};

class rs232Port
  : public serialPort
{
public:
  virtual int setRS232(int  pinlist) = 0;
  virtual int getRS232(int* pinlist) = 0;
};

#endif


#endif /*_SERIAL_HPP_*/
