#ifndef _I2C_MASTER_HPP_
#define _I2C_MASTER_HPP_

#include "event.hpp"
#include "core.h"
#include "lpc17xx_defs.h"
#include "serial.hpp"


class i2c_master
  : public serialPort
{
public:
  i2c_master(LPC_I2C_TypeDef* phy, const serialCreationDisposition& cd);
  virtual ~i2c_master();

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

  /******************************** typical pin presets *********************************/
  static void pinSetupI2C0  (unsigned char setDefault);
  static void pinSetupI2C1_0(unsigned char setDefault);
  static void pinSetupI2C1_1(unsigned char setDefault);
  static void pinSetupI2C2  (unsigned char setDefault);

private:
  LPC_I2C_TypeDef* low;               /* CMSIS interface to low level UART registers    */
  PIN_SETUP     pinSetupProc;         /* pin setup procedure                            */
  unsigned char irqNum;               /* port irq number                                */
  unsigned long pconpMask;            /* PCONP mask                                     */
  portSettings  ps;                   /* current port settings                          */

  CIRC_BUF128   inbuf;                /* software FIFO extender                         */

#define I2C_MASTER_EVT_WRITECOMPLETE  0x01
#define I2C_MASTER_EVT_READCOMPLETE   0x02
  eventpack      epack;               /* events pack                                    */

  int            wrerr;               /* async wr error                                 */
  unsigned char* wrbuf;               /* external wr data buffer                        */
  unsigned long  wrbufsz;             /* external wr data buffer size                   */
  unsigned long  wrbufrest;           /* unsended data size                             */
  callback2*     wrcompleter;         /* async wr complete handler                      */

  int            rderr;               /* async rd error                                 */
  unsigned char* rdbuf;               /* external rd data buffer                        */
  unsigned long  rdbufsz;             /* external rd data buffer size                   */
  unsigned long  rdbufrest;           /* data to read rest                              */
  callback2*     rdcompleter;         /* async rd complete handler                      */


  void finalizeWrite(int bytesSent, int status);     /* service method to clear after Write completed     */
  void finalizeRead(int bytesRead, int status);      /* service method, used by timed write            */
  void onTimedWriteComplete(int bytesSent, int err); /* service method, used by timed read             */
  void onTimedReadComplete(int bytesRead, int err);  

  static void irqh(void* _this);      /* port irq handler                               */

  //**************************************************************************************
  unsigned char     isWrite;          // Признак записи данных
  unsigned char     ucAddr;           // Адрес адресуемого устройства на шине  

};





#endif /*_I2C_HPP_*/
