#ifndef _SERIALSAFE_HDPX_HPP_
#define _SERIALSAFE_HDPX_HPP_

#include "semaphore.hpp"
#include "serial.hpp"


/* half duplex thread safe serial port wrapper */
class ssafe_hdpx
  : public serialPort
{
public:
  ssafe_hdpx(serialPort* sp);
  virtual ~ssafe_hdpx();

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

  semaphore synclock;                 /* sync lock                                      */

  callback2* asynccompleter;          /* async complete handler                         */
  binder2<ssafe_hdpx> asynccompleterInner;
  void onAsyncComplete(int bytesCount, int err);
};

#endif /*_SERIALSAFE_ÐDPX_HPP_*/
