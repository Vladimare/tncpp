#include "errors.h"
#include "serialsafe_hdpx.hpp"


/*****************************************************************************************
 ********************************** create and destroy ***********************************
 *****************************************************************************************/
ssafe_hdpx::ssafe_hdpx(serialPort* sp)
  : unsafe(sp)
{
  this->synclock.release();           /* port is free to use                            */

  this->asynccompleterInner.bind(&ssafe_hdpx::onAsyncComplete, this);
}

ssafe_hdpx::~ssafe_hdpx()
{
}


/*****************************************************************************************
 ***************************************** init ******************************************
 *****************************************************************************************/
int ssafe_hdpx::init(const portSettings& ps)
{
                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

  err = this->unsafe->init(ps);       /* delegate call to unsafe entity                 */

  this->synclock.release();

  return err;
}

/* WARNING! NOT SUPPORTED! */
const portSettings& ssafe_hdpx::settings()
{
  return serialPort::defSSPSettings;
}


/*****************************************************************************************
 ***************************************** write *****************************************
 *****************************************************************************************/
int ssafe_hdpx::write(unsigned char* buf, int bufsz)
{
                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

                                      /* delegate call to unsafe entity                 */
  err = this->unsafe->write(buf, bufsz);

  this->synclock.release();

  return err;
}

int ssafe_hdpx::writeAsync(unsigned char* buf, int bufsz, callback2* ac)
{
                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

  this->asynccompleter = ac;          /* save external completer, we must call it before*/
                                      /*   release lock                                 */

                                      /* delegate call to unsafe entity                 */
  return this->unsafe->writeAsync(buf, bufsz, &this->asynccompleterInner);
                                      /* lock released after write completed!           */
}

void ssafe_hdpx::onAsyncComplete(int bytesCount, int err)
{
  if(this->asynccompleter)
  {                                   /* call external async complete handler, if exist */
    (*(this->asynccompleter))(bytesCount, err);
    this->asynccompleter = 0;
  }

  this->synclock.release();           /* async activity completed, release lock         */
}

int ssafe_hdpx::cancelWriteAsync(int reason)
{
                                      /* any task can cancel tx operation               */
  int err = this->unsafe->cancelWriteAsync(reason);

  this->synclock.release();           /* tx cancelled, release lock, if set             */

  return err;
}

int ssafe_hdpx::writeTimed(unsigned char* buf, int* bufsz, int timeout)
{
                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

                                      /* delegate call to unsafe entity                 */
  err = this->unsafe->writeTimed(buf, bufsz, timeout);

  this->synclock.release();

  return err;
}


/*****************************************************************************************
 ***************************************** read ******************************************
 *****************************************************************************************/
int ssafe_hdpx::read(unsigned char* buf, int bufsz)
{
  if(!buf || !bufsz)
    return ERR_NOT_SUPPORTED;         /* this features not supported                    */

                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

                                      /* delegate call to unsafe entity                 */
  err = this->unsafe->read(buf, bufsz);

  this->synclock.release();

  return err;
}

int ssafe_hdpx::readAsync(unsigned char* buf, int bufsz, callback2* ac)
{
  if(!buf || !bufsz)
    return ERR_NOT_SUPPORTED;         /* this features not supported                    */

                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

  this->asynccompleter = ac;          /* save external completer, we must call it before*/
                                      /*   release lock                                 */

                                      /* delegate call to unsafe entity                 */
  return this->unsafe->readAsync(buf, bufsz, &this->asynccompleterInner);
                                      /* lock released after read completed!            */
}

int ssafe_hdpx::cancelReadAsync(int reason)
{
                                      /* any task can cancel rx operation               */
  int err = this->unsafe->cancelReadAsync(reason);

  this->synclock.release();           /* activity cancelled, release lock, if set       */

  return err;
}

int ssafe_hdpx::readTimed(unsigned char* buf, int* bufsz, int timeout)
{
  if(!buf || !bufsz)
    return ERR_NOT_SUPPORTED;         /* this features not supported                    */

                                      /* wait until current activity completed          */
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock activity                        */

  err = this->unsafe->readTimed(buf, bufsz, timeout);

  this->synclock.release();           /* async activity completed,  release lock        */

  return err;
}


/*****************************************************************************************
 ***************************************** stuff *****************************************
 *****************************************************************************************/
int ssafe_hdpx::purge(unsigned char queue)
{
  int err = ERR_OK;

  if((queue & PURGE_RX) || (queue & PURGE_TX))
  {
    err = this->synclock.acquire(TN_WAIT_INFINITE);
    if(err != ERR_OK)
      return err;

    if(queue & PURGE_RX)
      err = this->unsafe->purge(queue);

    if(queue & PURGE_TX)
      err = this->unsafe->purge(queue);

    this->synclock.release();
  }

  return err;
}

int ssafe_hdpx::sendchar(unsigned char c)
{
  int err = this->synclock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;

  err = this->unsafe->sendchar(c);

  this->synclock.release();

  return err;
}
