#ifndef _GSM_HPP_
#define _GSM_HPP_


#include "mutex.hpp"
#include "serial.hpp"
#include "thread.hpp"
#include "event.hpp"
#include "systimer.h"

//Комманды
#define GSM_AT          "AT"
#define GSM_ECHO        "E"
#define GSM_DIAL        "D"
#define GSM_HANGUP      "H"
#define GSM_BAUDRATE    "+IPR"
#define GSM_MANUFACTUR  "+CGMI"
#define GSM_MODEL       "+CGMM"
#define GSM_SOFT_VER    "+CGMR"
#define GSM_IMEI        "+CGSN"
#define GSM_INFO        "I"
#define GSM_DEFAULT     "&F"
#define GSM_SHUTDOWN    "^SMSO"
#define GSM_SIM_STATE   "^SCKS"
#define GSM_SIM_ID      "^SCID"
#define GSM_PIN         "+CPIN"
#define GSM_SET_PIN     "+CLCK"
#define GSM_CHANGE_PIN  "+CPWD"
#define GSM_REG         "+CREG"
#define GSM_SQ          "+CSQ"
#define GSM_SMS_URC     "+CNMI"
#define GSM_SMS_FORMAT  "+CMGF"
#define GSM_SMS_LIST    "+CMGL"
#define GSM_SMS_DEL     "+CMGD"
#define GSM_SMS_RD      "+CMGR"
#define GSM_SMS_WR      "+CMGS"
#define GSM_PDP_CONT    "+CGDCONT"
#define GSM_TO_CMD      "+++"
#define GSM_TO_DATA     "ATO"
#define GSM_GPRS_REG    "+CGREG"

#define GSM_DATA_SHUTDOWN   "MS OFF"

//URC незапрашиваемые коды результата
#define GSM_URC_START       "^SYSSTART"
#define GSM_URC_SHUTDOWN    "^SHUTDOWN"
#define GSM_URC_RING        "RING"
#define GSM_URC_NET_REG     "+CREG"
#define GSM_URC_GPRS_REG    "+CGREG"
#define GSM_URC_SMS         "+CMTI"
#define GSM_URC_SIM         "^SCKS"
#define GSM_URC_OK          "OK"
#define GSM_URC_ERROR       "ERROR"
#define GSM_URC_CONNECT     "CONNECT"
#define GSM_URC_NO_CARRIER  "NO CARRIER"
#define GSM_URC_NO_DIALTONE "NO DIALTONE"
#define GSM_URC_BUSY        "BUSY"
#define GSM_URC_ALERTING    "ALERTING"
#define GSM_URC_DIALING     "DIALING"

//Безопасность
#define GSM_SIM_PIN           "SIM PIN"
#define GSM_SIM_PUK           "SIM PUK"
#define GSM_SIM_PIN2          "SIM PIN2"
#define GSM_SIM_PUK2          "SIM PUK2"
#define GSM_SIM_READY         "READY"

//SMS
#define GSM_SMS_UNREAD        "REC UNREAD"
#define GSM_SMS_READ          "REC READ"
#define GSM_SMS_UNSENT        "STO UNSENT"
#define GSM_SMS_SENT          "STO SENT"
#define GSM_SMS_ALL           "ALL"

#define GSM_RESP_STEP_ECHO    0
#define GSM_RESP_STEP_DATA    1
#define GSM_RESP_STEP_CODE    2
#define GSM_RESP_STEP_SMS_TXT 3

#define GSM_SIM_REMOVED       0
#define GSM_SIM_INSERTED      1

#define GSM_AUTH_READY        0
#define GSM_AUTH_PIN          1
#define GSM_AUTH_PUK          2
#define GSM_AUTH_PIN2         3
#define GSM_AUTH_PUK2         4

#define GSM_PIN_UNBLOCK       0
#define GSM_PIN_BLOCK         1
#define GSM_PIN_STATE         2

#define GSM_REG_NOT           0
#define GSM_REG_HOME          1
#define GSM_REG_PROGRESS      2
#define GSM_REG_DENIED        3
#define GSM_REG_UNKNOWN       4
#define GSM_REG_ROAMING       5

#define GSM_SMS_FORMAT_PDU    0
#define GSM_SMS_FORMAT_TXT    1

#define GSM_CID_MAX_NUM       2


#define GSM_CR        '\r' //Перевод коретки
#define GSM_LF        '\n' //Новая строка
#define GSM_STR_SEP   '#'  //Разделитель строк
#define GSM_STR_FRAME '"'  //Обрамление строки
#define GSM_PARAM_SEP ','  //Разделитель параметров
#define GSM_SMS_INV   '>'  //Приглашение ввода текста отправляемой смс

//Таймауты
#define GSM_CMD_TIMEOUT       100 //Пауза между посылаемыми командами 100мс
#define GSM_RX_CHAR_TIMEOUT   (100  / SYSTIMER_PERIOD)//Таймаут на прием символа 100мс
#define GSM_RX_LINE_TIMEOUT   (1000 / SYSTIMER_PERIOD)//Таймаут на прием строки 1000мс
#define GSM_RX_HANDLE_TIMEOUT (1000 / SYSTIMER_PERIOD)//Таймаут на обработку принятого ответа 1000мс
#define GSM_TX_LINE_TIMEOUT   (1000 / SYSTIMER_PERIOD)//Таймаут на передачу 1000мс
#define GSM_CONNECT_TIMEOUT   (10000 / SYSTIMER_PERIOD)//Таймаут на подключение
#define GSM_RX_SZ        128 //Размер буфера на прием
#define GSM_TX_SZ        64  //Размер буфера на передачу

#define GSM_SMS_FROM_SZ   16
#define GSM_SMS_TIME_SZ   24


//Абстрактный класс событий GSM
class gsm_events
{
public:
  gsm_events();
  virtual ~gsm_events();
  virtual int OnSysStart() = 0;   //Произошел перезапуск модуля
  virtual int OnShutDown() = 0;   //Происходит отключение модуля
  virtual int OnRing() = 0;       //Входящий звонок
  virtual int OnSMS(int num) = 0; //Новое смс
  virtual int OnChangeNetState(int state) = 0; //Изменение состояния регистрации в сети
  virtual int OnChangeSIMSlotState(int state) = 0; //Изменение состояния слота сим карты
};

//Класс работы с GSM модулем
class gsm
  : public thread
{
public:
  gsm(rs232Port *port, unsigned long pclk);
  virtual ~gsm();
  bool  getATReady();//Получение признака готовности принимать AT комманды
  void  setATReady(bool on = true);//Установка или сброс признака готовности принимать AT комманды
  int bind_events(gsm_events* events);//Подвязка обработчиков событий
  int at(); //Команда проверки связи с gsm модемом
  int setBaudrate(unsigned int baudrate=BR_115200); //Установка скорости связи с gsm модемом
  int enableSMSURC(); //Активация URC при приеме нового sms сообщения
  int disableSMSURC(); //Деактивация URC при приеме нового sms сообщения
  int enableRegURC(); //Активация URC при изменении состояния регистрации в сети
  int disableRegURC(); //Деактивация URC при изменении состояния регистрации в сети
  int enableSIMURC(); //Активация URC при изменении состояния слота SIM карты
  int disableSIMURC(); //Деактивация URC при изменении состояния слота SIM карты
  int getManufacturer(char *str);
  int getModel(char *str);
  int getSoftVer(char *str);
  int getIMEI(char *str);
  int getInfo(char *str);
  int setDefaults();
  int shutDown();
  int getSIMState();
  int getSIMId(char *str);
  int getPINAuthState();
  int enterPIN(unsigned int pin);
  int enterPUK(unsigned int puk, unsigned int new_pin);
  int getPINState();
  int setPIN(unsigned int pin);
  int clrPIN(unsigned int old_pin);
  int changePIN(unsigned int old_pin, unsigned int new_pin);

  int  getNetworkRegistration();
  int  getGPRSRegistration();
  bool isGSMRegistered();

  int getSignalQuality();
  int setSMSFormat(unsigned int format = GSM_SMS_FORMAT_TXT);
  int getSMSFormat();
  int deleteSMS(int idx);
  int readSMS(int idx, char *from, char * time, char *text);
  int writeSMS(char *to, char *text);
  int setPDP(unsigned int cid, char *apn = "", char *addr = "");
  int connectGPRS(unsigned int cid);
  int disconnectGPRS();
  int switchCmdMode();
  int switchDataMode();


protected:
  rs232Port* port;
  virtual void run();

private:
  static const threadCreationDisposition tcd;

  char          rx[GSM_RX_SZ]; //Буфер на прием
  char          tx[GSM_TX_SZ]; //Буфер на передачу

#define GSM_RX_FSM_URC  0 //Шаг когда могут приходить только URC
#define GSM_RX_FSM_ECHO 1 //Шаг ожидания эхо
#define GSM_RX_FSM_RESP 2 //Шаг приема ответа
  unsigned char rx_fsm;   //Автомат приема
#define GSM_MODE_CMD    0
#define GSM_MODE_DATA   1
  unsigned char mode;     //Режим связи с GSM модемом
  unsigned long ips;  //Системное время для межкомандной паузы
  bool          at_ready; //Признак готовности принимать AT комманды
  unsigned char net_reg; //Состояние регистрации в сети
  int           err;  //Ошибка для передачи между потоками

#define GSM_EP_RX_LINE  0x01 //Принята строка
#define GSM_EP_RX_FULL  0x02 //Буфер заполнен частью строки
#define GSM_EP_RX_EMPTY 0x04 //Принята пустая строка
#define GSM_EP_RX_FREE  0x08 //Приемный буфер свободен
#define GSM_EP_RX_ERR   0x10 //Ошибка при приеме
  eventpack     ep;   //События для синхронизации потоков приема/передачи данных gsm модема

  gsm_events*   events;   //Указатель на класс с обработчиками событий
  mutex         synclock; //Мутекс для синхронизации доступа к данным класса

  int readln(int timeout = GSM_RX_CHAR_TIMEOUT);
  int write(int sz);
  int getResponseCode();
  int getResponseString(char *str);
  int getURC();
  int setEcho(bool on = true);
  int excludeCmd(char *str, const char *cmd);
  int excludeParam(char *str);
  int getIntParam(char *str);
  int getStringParam(char *to, char *from);
};
#endif /*_GSM_HPP_*/
