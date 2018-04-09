#ifndef _UART17xx_HPP_
#define _UART17xx_HPP_

#include "event.hpp"
#include "lpc17xx_defs.h"
#include "serial.hpp"


class uart17xx
  : public rs232Port
{
public:
  uart17xx(LPC_UART_TypeDef* phy, const serialCreationDisposition& cd);
  virtual ~uart17xx();

  /***************************** serialPort implementation ******************************/
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

  /****************************** rs232Port implementation ******************************/
  virtual int setRS232(int  pinlist);
  virtual int getRS232(int* pinlist);

  /******************************** typical pin presets *********************************/
  static void pinSetupUART0  (unsigned char setDefault);
  static void pinSetupUART1_0(unsigned char setDefault);
  static void pinSetupUART1_0_fullmodem(unsigned char setDefault);
  static void pinSetupUART1_1(unsigned char setDefault);
  static void pinSetupUART2_0(unsigned char setDefault);
  static void pinSetupUART2_1(unsigned char setDefault);
  static void pinSetupUART3_0(unsigned char setDefault);
  static void pinSetupUART3_1(unsigned char setDefault);
  static void pinSetupUART3_2(unsigned char setDefault);

private:
  LPC_UART_TypeDef* low;              /* CMSIS interface to low level UART registers    */
  PIN_SETUP     pinSetupProc;         /* pin setup procedure                            */
  unsigned char irqNum;               /* port irq number                                */
  unsigned long pconpMask;            /* PCONP mask                                     */
  portSettings  ps;                   /* current port settings                          */

  CIRC_BUF128   inbuf;                /* software FIFO extender                         */

#define UART17xxEVT_TXCOMPLETE  0x01
#define UART17xxEVT_RXCOMPLETE  0x02
  eventpack      epack;               /* events pack                                    */

  int            txerr;               /* async tx error                                 */
  unsigned char* txbuf;               /* external tx data buffer                        */
  unsigned long  txbufsz;             /* external tx data buffer size                   */
  unsigned long  txbufrest;           /* unsended data size                             */
  callback2*     txcompleter;         /* async tx complete handler                      */

  int            rxerr;               /* async rx error                                 */
  unsigned char* rxbuf;               /* external rx data buffer                        */
  unsigned long  rxbufsz;             /* external rx data buffer size                   */
  unsigned long  rxbufrest;           /* data to read rest                              */
  callback2*     rxcompleter;         /* async rx complete handler                      */


  void fill_TxHWFIFO();               /* service method to fill hardware Tx FIFO from   */
                                      /*   input txbuf buffer                           */
                                      /* service method to clear after Tx completed     */
  void finalizeTx(int bytesSent, int status);
                                      /* service method to clear after Rx completed     */
  void finalizeRx(int bytesRead, int status);
                                      /* service method, used by timed write            */
  void onTimedWriteComplete(int bytesSent, int err);
                                      /* service method, used by timed read             */
  void onTimedReadComplete(int bytesRead, int err);

  static void irqh(void* _this);      /* port irq handler                               */
};

#endif /*_UART17xx_HPP_*/
