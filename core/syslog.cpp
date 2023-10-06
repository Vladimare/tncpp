#include <stdio.h>

#include "errors.h"
#include "syslog.hpp"


syslog* syslog::g_log = 0;

const char* syslog::log_level_names[] =
{
  "disabled",       // 0
  "critical",       // 1
  "error",          // 2
  "alert",          // 3
  "warning",        // 4
  "info",           // 5
  "user",           // 6
  "hdata",          // 7
  "process",        // 8
  "ldata",          // 9
  "flow",           // 10
  "flowdata",       // 11
  "sync",           // 12
  "debug",          // 13
  "all"             // 14
};

#if defined(__ARMCC_VERSION)

syslog::syslog(unsigned int v, serialPort* sp)
{
  this->verbosity = v;
  this->port      = sp;

  syslog::g_log = this;
}

#elif defined(_WIN32)

syslog::syslog(unsigned int v)
{
  this->verbosity = v;

  syslog::g_log = this;
}

#endif

syslog::~syslog()
{
}

int syslog::write(unsigned int v, const char* fmt, ...)
{
  if(v > this->verbosity)
    return ERR_OK;                    /* недостаточная глубина лога                     */

  va_list marker;

  va_start(marker, fmt);

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.lock(TN_WAIT_INFINITE);
#endif

#ifdef SYSLOG_NOFLOAT
  _vprintf(fmt, marker);
#else
  vprintf (fmt, marker);
#endif

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.unlock();
#endif

  va_end(marker);

  return ERR_OK;
}

int syslog::writeva(unsigned int v, const char* fmt, va_list args)
{
  if(v > this->verbosity)
    return ERR_OK;                    /* недостаточная глубина лога                     */

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.lock(TN_WAIT_INFINITE);
#endif

#ifdef SYSLOG_NOFLOAT
  _vprintf(fmt, args);
#else
  vprintf (fmt, args);
#endif

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.unlock();
#endif

  return ERR_OK;
}

int syslog::writebuf(unsigned int v, const unsigned char* buf, unsigned long length)
{
  if(v > this->verbosity)
    return ERR_OK;                    /* недостаточная глубина лога                     */

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.lock(TN_WAIT_INFINITE);
#endif

  for(unsigned long i = 0; i < length; i++)
#ifdef SYSLOG_NOFLOAT

    _printf("%X%X ", buf[i] >> 4, buf[i] & 0x0F);

  _printf("\r\n");

#else

    printf("%X%X ", buf[i] >> 4, buf[i] & 0x0F);

  printf("\r\n");

#endif

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.unlock();
#endif

  return ERR_OK;
}

#if defined(__ARMCC_VERSION)

int syslog::read(unsigned char* c)
{
  int res = ERR_OK;

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.lock(TN_WAIT_INFINITE);
#endif

  if(this->port->read(0, 0) > 0)
    res = this->port->read(c, 1);

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.unlock();
#endif

  return res;
}

int syslog::sendchar(int ch)
{
  if(syslog::g_log)
    syslog::g_log->port->sendchar((unsigned char)(ch & 0xFF));

  return ch;
}

#endif

void syslog::changeVerbosity(unsigned int new_v)
{
#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.lock(TN_WAIT_INFINITE);
#endif

  this->verbosity = new_v;

#ifdef TNKERNEL_PORT_CORTEXM3
  this->synclock.unlock();
#endif
}


int g_writelog(unsigned int v, const char* fmt, ...)
{
  if(syslog::g_log)
  {
    va_list marker;

    va_start(marker, fmt);
    syslog::g_log->writeva(v, fmt, marker);
    va_end(marker);

    return ERR_OK;
  }

  return ERR_NOT_SUPPORTED;
}


/****************************************************************************************
 *********************************** retarget section ***********************************
 ****************************************************************************************/
#if defined(__ARMCC_VERSION)

namespace std
{
  struct __FILE
  {
    int handle;
  };

  FILE __stdout;

  int fputc(int ch, FILE *f)
  {
    return syslog::sendchar(ch);
  }

  int ferror(FILE *f)
  {
    return EOF;
  }

  void _ttywrch(int ch)
  {
    syslog::sendchar(ch);
  }

  void _sys_exit(int return_code)
  {
  label:
    goto label;
  }
}

#endif
