#include "errors.h"
#include "serialsafe_fdpx.hpp"


/*****************************************************************************************
 ********************************** create and destroy ***********************************
 *****************************************************************************************/
ssafe_fdpx::ssafe_fdpx(serialPort* sp)
  : unsafe(sp)
{
  this->txlock.release();             /* port is free to use                            */
  this->rxlock.release();

  this->txcompleterInner.bind(&ssafe_fdpx::onAsyncTxComplete, this);
  this->rxcompleterInner.bind(&ssafe_fdpx::onAsyncRxComplete, this);
}

ssafe_fdpx::~ssafe_fdpx()
{
}


/*****************************************************************************************
 ***************************************** init ******************************************
 *****************************************************************************************/
int ssafe_fdpx::init(const portSettings& ps)
{
                                      /* wait until current tx completed                */
  int err = this->txlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock tx activity                     */

                                      /* wait until current rx completed                */
  err = this->rxlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock rx activity                     */

  err = this->unsafe->init(ps);       /* delegate call to unsafe entity                 */

  this->txlock.release();
  this->rxlock.release();

  return err;
}

/* WARNING! NOT SUPPORTED! */
const portSettings& ssafe_fdpx::settings()
{
  return serialPort::defUARTSettings;
}


/*****************************************************************************************
 ***************************************** write *****************************************
 *****************************************************************************************/
int ssafe_fdpx::write(unsigned char* buf, int bufsz)
{
                                      /* wait until current tx completed                */
  int err = this->txlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock tx activity                     */

                                      /* delegate call to unsafe entity                 */
  err = this->unsafe->write(buf, bufsz);

  this->txlock.release();

  return err;
}

int ssafe_fdpx::writeAsync(unsigned char* buf, int bufsz, callback2* ac)
{
                                      /* wait until current tx completed                */
  int err = this->txlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock tx activity                     */

  this->txcompleter = ac;             /* save external completer, we must call it before*/
                                      /*   release lock                                 */

                                      /* delegate call to unsafe entity                 */
  return this->unsafe->writeAsync(buf, bufsz, &this->txcompleterInner);
                                      /* tx lock released after write completed!        */
}

void ssafe_fdpx::onAsyncTxComplete(int bytesSent, int err)
{
  if(this->txcompleter)
  {                                   /* call external async tx complete handler, if    */
                                      /*   exist                                        */
    (*(this->txcompleter))(bytesSent, err);
    this->txcompleter = 0;
  }

  this->txlock.release();             /* async tx completed,  release lock              */
}

int ssafe_fdpx::cancelWriteAsync(int reason)
{
                                      /* any task can cancel tx operation               */
  int err = this->unsafe->cancelWriteAsync(reason);

  this->txlock.release();             /* tx cancelled, release tx lock, if set          */

  return err;
}

int ssafe_fdpx::writeTimed(unsigned char* buf, int* bufsz, int timeout)
{
                                      /* wait until current tx completed                */
  int err = this->txlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock tx activity                     */

                                      /* delegate call to unsafe entity                 */
  err = this->unsafe->writeTimed(buf, bufsz, timeout);

  this->txlock.release();

  return err;
}


/*****************************************************************************************
 ***************************************** read ******************************************
 *****************************************************************************************/
int ssafe_fdpx::read(unsigned char* buf, int bufsz)
{
  if(!buf || !bufsz)
    return ERR_NOT_SUPPORTED;         /* this features not supported                    */

                                      /* wait until current rx completed                */
  int err = this->rxlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock rx activity                     */

                                      /* delegate call to unsafe entity                 */
  err = this->unsafe->read(buf, bufsz);

  this->rxlock.release();

  return err;
}

int ssafe_fdpx::readAsync(unsigned char* buf, int bufsz, callback2* ac)
{
  if(!buf || !bufsz)
    return ERR_NOT_SUPPORTED;         /* this features not supported                    */

                                      /* wait until current rx completed                */
  int err = this->rxlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock rx activity                     */

  this->rxcompleter = ac;             /* save external completer, we must call it before*/
                                      /*   release lock                                 */

                                      /* delegate call to unsafe entity                 */
  return this->unsafe->readAsync(buf, bufsz, &this->rxcompleterInner);
                                      /* rx lock released after read completed!         */
}

void ssafe_fdpx::onAsyncRxComplete(int bytesRead, int err)
{
  if(this->rxcompleter)
  {                                   /* call external async rx complete handler, if    */
                                      /*   exist                                        */
    (*(this->rxcompleter))(bytesRead, err);
    this->rxcompleter = 0;
  }

  this->rxlock.release();             /* async rx completed,  release lock              */
}

int ssafe_fdpx::cancelReadAsync(int reason)
{
                                      /* any task can cancel rx operation               */
  int err = this->unsafe->cancelReadAsync(reason);

  this->rxlock.release();             /* rx cancelled, release rx lock, if set          */

  return err;
}

int ssafe_fdpx::readTimed(unsigned char* buf, int* bufsz, int timeout)
{
  if(!buf || !bufsz)
    return ERR_NOT_SUPPORTED;         /* this features not supported                    */

                                      /* wait until current rx completed                */
  int err = this->rxlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;                       /* failed to lock rx activity                     */

  err = this->unsafe->readTimed(buf, bufsz, timeout);

  this->rxlock.release();             /* async rx completed,  release lock              */

  return err;
}


/*****************************************************************************************
 ***************************************** stuff *****************************************
 *****************************************************************************************/
int ssafe_fdpx::purge(unsigned char queue)
{
  int err = ERR_OK;

  if(queue & PURGE_RX)
  {
    err = this->rxlock.acquire(TN_WAIT_INFINITE);
    if(err != ERR_OK)
      return err;
  
    err = this->unsafe->purge(queue);
  
    this->rxlock.release();
  }

  if(err != ERR_OK)
    return err;

  if(queue & PURGE_TX)
  {
    err = this->txlock.acquire(TN_WAIT_INFINITE);
    if(err != ERR_OK)
      return err;
  
    err = this->unsafe->purge(queue);
  
    this->txlock.release();
  }

  return err;
}

int ssafe_fdpx::sendchar(unsigned char c)
{
  int err = this->txlock.acquire(TN_WAIT_INFINITE);
  if(err != ERR_OK)
    return err;

  err = this->unsafe->sendchar(c);

  this->txlock.release();

  return err;
}
