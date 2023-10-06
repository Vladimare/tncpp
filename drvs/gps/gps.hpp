#ifndef _GPS_HPP_
#define _GPS_HPP_

#include <math.h>
#include <time.h>

#include "event.hpp"
#include "mutex.hpp"
#include "serial.hpp"
#include "thread.hpp"
#include "types_ext.hpp"


#define GPS_RECEIVE_TIMEOUT 500       /* таймаут на прием пакетов от GPS-модуля, тики   */
                                      /*   системного таймера                           */
#define GPS_VALID_TIMEOUT   3000      /* таймаут, после срабатывания которого сигнал    */
                                      /*   GPS считается невалидным                     */
                                      /* отсчет таймаута сбрасывается по получению      */
                                      /*   валидной посылки GPS                         */
                                      /* таймаут задается в тиках системного таймера    */

                                      /* структура данных RMC-посылки                   */
struct rmcData
{
           char source;               /* источник сигнала                               */
                                      /*   'P' - сигнал GPS                             */
                                      /*   'L' - сигнал ГЛОНАСС                         */
                                      /*   'N' - комбинированный сигнал                 */
           char valid;                /* статус данных:                                 */
                                      /*   'А' - данные достоверны                      */
                                      /*   'V' - данные недостоверны                    */
           char mode;                 /* режим работы:                                  */
                                      /*   'A' - автономный                             */
                                      /*   'D' - дифференциальный                       */
                                      /*   'E' - аппроксимация                          */
                                      /*   'N' - недостоверные данные                   */
  unsigned long date;                 /* содержимое поля date впрямую копируется из rmc */
                                      /*   посылки, и представляется в виде десятичного */
                                      /*   числа. Например, в 21 веке значение 211230   */
                                      /*   будет соответствовать 21 декабря 2030 года   */
                                      /* все double параметры копируются из rmc-строки  */
                                      /*   с той точностью, как получены                */
         double time;                 /* время фиксации местоположения                  */
         double latitude;             /* широта, отрицательное значение соответствует   */
                                      /*   западному полушарию                          */
         double longitude;            /* долгота, отрицательное значение соответствует  */
                                      /*   южному полушарию                             */
         double speed;                /* скорость в милях, как в RMC-посылке            */
         double trackAngle;           /* путевой угол                                   */
         double declination;          /* магнитное склонение (число со знаком)          */
};


class gps
  : public thread
{
public:
  gps();
  virtual ~gps();

  void init(serialPort* port, unsigned long pclk);


  void getGPS(rmcData* gd)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    *gd = this->actualGPS;
    this->synclock.unlock();
  }

  unsigned long getUTC()
  {
    this->synclock.lock(TN_WAIT_INFINITE);

    double        t = this->actualGPS.time;
    unsigned long d = this->actualGPS.date;

    this->synclock.unlock();

    return GPS2UTC(d, t);
  }

  static unsigned long GPS2UTC(unsigned long date, double time)
  {
    struct tm dt;
    unsigned long inttime = floor(time);

    dt.tm_sec =   inttime % 100;
    dt.tm_min =  (inttime % 10000) / 100;
    dt.tm_hour=   inttime / 10000;
    dt.tm_mday=   date / 10000;
    dt.tm_mon = ((date % 10000) / 100) - 1;
    dt.tm_year=  (date % 100) + 2000 - 1900;

    return mktime(&dt);
  }


  void setCallback_onGPSUpdated(callback0* cb)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    this->gpsUpdated = cb;
    this->synclock.unlock();
  }

  void setCallback_onGPSValid(callback0* cb)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    this->gpsValid = cb;
    this->synclock.unlock();
  }

  void setCallback_onGPSInvalid(callback0* cb)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    this->gpsInvalid = cb;
    this->synclock.unlock();
  }

                                      /* возвращает расстояние в метрах между двумя     */
                                      /*   точками на земной поверхности                */
                                      /* точки задаются в формате rmc                   */
  static unsigned long distance (double lat1, double long1, double lat2, double long2);
                                      /* переводит скорость в милях в скорость в км / ч */
  static unsigned long speed    (double speed_miles);

  static double rad(double a);        /* преобразование географической координаты в     */
                                      /*   формате rmc в радианы                        */

                                      /* радиус Земли в метрах (40000000 / (2 * pi))    */
  static const int    EARTH_RADIUS= 6371000;
  static const double PI;

protected:
  virtual void run();

private:
  bool          isValid;              /* true - есть валидность координат               */
  mutex         synclock;             /* мутекс для синхронизации доступа к данным класса */
  serialPort*   sourcePort;           /* порт-источник NMEA-посылок                     */

  bool          isValidPacket;        /* признак приема правильных RMC-пакетов          */
  unsigned long baudrateTimer;        /* таймер на смену скорости обмена                */
  unsigned long baudrateStartAt;      /* значение systimer, когда был начат счет        */
                                      /*   baudrateTimer                                */
  unsigned char baudrateIndex;        /* индекс в массиве baudrates, соответствующий    */
                                      /*   текущей скорости                             */

#define GPS_CHAR_RECEIVED   0x01      /* принят очередной символ                        */
  eventpack     events;               /* события gps-сервера                            */

#define NMEAFS_UNDEFINED        0x00  /* состояние не определено                        */
#define NMEAFS_HEADER           0x01  /* найден управляющий символ '$', прием заголовка */
#define NMEAFS_TIME             0x02  /* прием времени фиксации координат               */
#define NMEAFS_VALID            0x03  /* прием валидности                               */
#define NMEAFS_LATITUDE         0x04  /* прием широты                                   */
#define NMEAFS_LATSPHERE        0x05  /* полушарие по широте                            */
#define NMEAFS_LONGITUDE        0x06  /* прием долготы                                  */
#define NMEAFS_LONGSPHERE       0x07  /* полушарие по долготе                           */
#define NMEAFS_SPEED            0x08  /* прием горизонтальной составляющей скорости     */
#define NMEAFS_TRACKANGLE       0x09  /* прием путевого угла                            */
#define NMEAFS_DATE             0x0A  /* прием даты                                     */
#define NMEAFS_DECLINATION      0x0B  /* прием магнитного склонения                     */
#define NMEAFS_DECLINATION_SIGN 0x0C  /* прием знака магнитного склонения               */
#define NMEAFS_MODE             0x0D  /* прием режима работы                            */
#define NMEAFS_CRC              0x0E  /* найден управляющий символ '*', прием CRC       */
  unsigned char nmeastate;            /* состояние автомата разбора NMEA-строки         */
  unsigned char nmeabuf[32];          /* буфер под NMEA-токены                          */
  int           nmeacount;            /* количество данных в буфере                     */
  unsigned char nmeachecksum;         /* контрольная сумма                              */

  rmcData       actualGPS;            /* текущие актуальные данные GPS                  */
  rmcData       tempGPS;              /* промежуточные данные GPS, используются для     */
                                      /*   временного хранения при разборе NMEA-посылки */

                                      /* обработчики уведомлений от GPS-парсера         */
  callback0*    gpsUpdated;           /* данные обновились                              */
  callback0*    gpsValid;             /* валидность восстановилась                      */
  callback0*    gpsInvalid;           /* валидность потерялась                          */

  static const threadCreationDisposition tcd;

  void onCharReceived(int count, int err);
  bool gpsParseStep(unsigned char c); /* щаг парсера, возвращает true, если на очередном*/
                                      /*   шаге состоялось обновление данных GPS        */
  void processDataToken();

#define MARKER_COUNT    3             /* список поддерживаемых маркеров                 */
  static const char* markers[MARKER_COUNT];

#define BAUDRATE_COUNT  4             /* список поддерживаемых скоростей обмена с       */
                                      /*   GPS-модулем, для автоматического определения */
                                      /*   скорости                                     */
  static const unsigned long baudrates[BAUDRATE_COUNT];
};

#endif /*_GPS_HPP_*/
