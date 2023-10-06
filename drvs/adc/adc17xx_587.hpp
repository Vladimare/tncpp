#ifndef _ADC17XX_587_H_
#define _ADC17XX_587_H_

#include "adc17xx.hpp"
#include "mutex.hpp"

//Константы
#define ADC_VREF          3300 //Опорное напряжение в мВ           
#define ADC_WIDTH         12   //Разрядность АЦП
#define ADC_VIN_FACTOR    12444//Множитель для расчета напряжения бортовой сети
#define ADC_VACC_FACTOR   3083 //Множитель для расчета напряжения аккумулятора
#define ADC_IN2_FACTOR    133  //Множитель для расчета напряжения шлейфов
#define ADC_IN2_CONST1    47   //Некая константа 1 для расчета напряжения шлейфа IN2
#define ADC_IN2_CONST2    4342 //Некая константа 2 для расчета напряжения шлейфа IN2

//Нумерация каналов
#define ADC_CH_IN1        0x00
#define ADC_CH_IN2        0x01
#define ADC_CH_IN3        0x02
#define ADC_CH_IN4        0x03
#define ADC_CH_IN5        0x04
#define ADC_CH_VIN        0x05
#define ADC_CH_VREF       0x06
#define ADC_CH_VACC       0x07


//Типы подключения шлейфов
#define LCON_TYPE1        0x00
#define LCON_TYPE2        0x01
#define LCON_TYPE3        0x02
#define LCON_TYPE4        0x03
#define LCON_TYPE5        0x04
#define LCON_TYPE6        0x05

//Состояния шлейфов
#define LSTATE_NORM                 0x00
#define LSTATE_OPEN_CIRCUIT         0x01
#define LSTATE_ALARM                0x03
#define LSTATE_SHORT_CIRCUIT_PLUS   0x04
#define LSTATE_SHORT_CIRCUIT_MINUS  0x05

#define PSTATE_NORM                 0x00
#define PSTATE_LOW                  0x01
#define PSTATE_HI                   0x02

#define ADC_DATABUF_SIZE        3

#define ADC_CH_NUM              8

typedef struct tagCH_DATA
{
  unsigned char state;
  unsigned char state_old;
  unsigned char value_cnt;
  unsigned short value_raw[ADC_DATABUF_SIZE];
  unsigned short value_fltr;
}CH_DATA;

typedef struct tagZONE_DATA
{
  unsigned short zone1;
  unsigned short zone2;
  unsigned short zone3;
  unsigned short zone4;
}ZONE_DATA;

typedef struct _adc17xx_587Settings
{
  unsigned char con_type;
  ZONE_DATA     zones;
}adc17xx_587Settings;


class adc17xx_587
  : public adc17xx
{
public:
  adc17xx_587(LPC_ADC_TypeDef* adcphy, basicCreationDisposition* adcCD, const adcSettings& s);
  virtual ~adc17xx_587();

  void setChCallback(unsigned char ch, callback2 *cb);
  int configure(unsigned char ch, const adc17xx_587Settings& s);
  adc17xx_587Settings get_config(unsigned char ch);

  unsigned char getChState(unsigned char ch);
  unsigned short getChValue(unsigned char ch);

  static void adcSetupPins(unsigned char state);
  static char* stateToText(const char **line, unsigned char state);

protected:
  binder1<adc17xx_587> chBinder;

  virtual int prepare(unsigned char channel, unsigned char* ADx, unsigned int* delay);
  virtual int finalize(unsigned char channel, unsigned int* delay);

  /* обработчики измерений каналов АЦП */
  void ch(int data);

private:
  adc17xx_587Settings chSettings[ADC_CH_NUM];
  mutex synclock;
  CH_DATA cd[ADC_CH_NUM];
  callback2* ccb[ADC_CH_NUM];

  unsigned char adcGetState(unsigned char ch, unsigned short value);

  static const adc17xx_587Settings defSettings[ADC_CH_NUM];
};

#endif /*_ADC17XX_587_H_*/
