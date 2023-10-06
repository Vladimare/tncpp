#ifndef _SYSLOG_HPP_
#define _SYSLOG_HPP_


#define LOG_LEVEL_DISABLED      0     // запись лога отключена
#define LOG_LEVEL_CRITICAL      1     // критические ошибки (сбои памяти, исключения, ...)
#define LOG_LEVEL_ERROR         2     // ошибки, ПО не работоспособно (неправильная конфигурация, ...)
#define LOG_LEVEL_ALERT         3     // ошибки, ПО работоспособно (нет связи с сервером, ...)
#define LOG_LEVEL_WARNING       4     // предупреждения (потеря / восстановление связи с девайсом, ...)
#define LOG_LEVEL_INFO          5     // информационные сообщения (старт, стоп, статистика)
#define LOG_LEVEL_USER          6     // команды инициированные пользователем
#define LOG_LEVEL_HDATA         7     // важные данные - команды/ответы/события (не в циклах)
#define LOG_LEVEL_PROCESS       8     // логика приложения - создание/удаление, обработка чего-либо (не в циклах/опросах)
#define LOG_LEVEL_LDATA         9     // данные низкого уровня - команды/ответы/события (передаваемые по сети и т.д не в циклах)
#define LOG_LEVEL_FLOW          10    // циклы, опросы, всё что замусоривает лог
#define LOG_LEVEL_FLOW_DATA     11    // все данные в циклах и т.п
#define LOG_LEVEL_SYNC          12    // состояние/работа с объектами синхронизации, потоками и т.д
#define LOG_LEVEL_DEBUG         13    // низкоуровневая отладочная информация
#define LOG_LEVEL_DBG_ALL       14    // вообще все данные (включая буферы данных и т.п)

#ifdef __cplusplus

#include <stdarg.h>

#if defined(__ARMCC_VERSION)
# include "mutex.hpp"
# include "serial.hpp"
#endif


//#define SYSLOG_NOFLOAT                /* если определить эту константу, то вместо       */
                                      /*   обычной функции printf() будет использоваться*/
                                      /*   _printf() (без поддержки плавающей точки),   */
                                      /*   что позволяет поэкономить на размере кода    */

class syslog
{
public:

#if defined(__ARMCC_VERSION)
  syslog(unsigned int v, serialPort* sp);
#endif

  syslog(unsigned int v);
  ~syslog();

  int write   (unsigned int v, const char* fmt, ...);
  int writeva (unsigned int v, const char* fmt, va_list args);
  int writebuf(unsigned int v, const unsigned char* buf, unsigned long length);

#if defined(__ARMCC_VERSION)
  int read    (unsigned char* c);
  static int sendchar(int ch);
#endif

  void changeVerbosity(unsigned int new_v);

  static syslog* g_log;

  static const char* log_level_names[];

private:
  unsigned int verbosity;

#if defined(__ARMCC_VERSION)
  serialPort* port;
  mutex       synclock;
#endif
};

extern "C"
{

#endif /*__cplusplus */

extern int g_writelog(unsigned int v, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /*_SYSLOG_HPP_*/
