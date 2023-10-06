#ifndef _SERIALSAFE_FDPX_HPP_
#define _SERIALSAFE_FDPX_HPP_

#include "semaphore.hpp"
#include "serial.hpp"


/* full duplex thread safe serial port wrapper */
class ssafe_fdpx
  : public serialPort
{
public:
  ssafe_fdpx(serialPort* sp);
  virtual ~ssafe_fdpx();

  /***************************** serialPort implementation ******************************/
  virtual int init      (const portSettings& ps);
  virtual const portSettings& settings();   /* WARNING! NOT SUPPORTED!                  */

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

private:
  serialPort* unsafe;                 /* serial port to make safe                       */

                                      /* if both locks acquired, txlock MUST always be  */
                                      /*   locked / unlocked first!                     */
  semaphore txlock;                   /* tx sync lock                                   */
  semaphore rxlock;                   /* rx sync lock                                   */

  callback2* txcompleter;             /* async tx complete handler                      */
  binder2<ssafe_fdpx> txcompleterInner;
  void onAsyncTxComplete(int bytesSent, int err);

  callback2* rxcompleter;             /* async rx complete handler                      */
  binder2<ssafe_fdpx> rxcompleterInner;
  void onAsyncRxComplete(int bytesRead, int err);
};

#endif /*_SERIALSAFE_FDPX_HPP_*/
