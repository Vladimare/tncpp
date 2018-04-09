#include "errors.h"
#include "ssp_light.hpp"


ssp_light::ssp_light(LPC_SSP_TypeDef* phy, sspCreationDisposition* cd)
{
  //Инициализация полей объекта
  this->low = phy;
  this->pinSetupProc = cd->pinProc;
  this->pwrMask = cd->pwrMask;

  LPC_SC->PCONP |= this->pwrMask;//Включение питания периферийного блока SSP

  if(this->pinSetupProc)
    this->pinSetupProc(false);//Конфигурирование портов SSP
  this->low->CR1 = 0x00;
                                      /* Наcтройка параметров: разрядность,   *
                                       * тип протокола, CPOL и CPHA для SPI   */
  this->low->CR0 = (SSP_DSS_8 | (cd->format << 4) | (cd->cpol << 6) | (cd->cpha << 7));

  this->low->CPSR = 0x02;//Делитель частоты, должен быть кратным 2
  this->low->IMSC = 0x00;//Сброс маски прерываний

  this->ps  = serialPort::defSSPSettings;
  this->init(this->ps);
}

ssp_light::~ssp_light()
{
  if(this->pinSetupProc)//Разконфигурирование портов SSP
    this->pinSetupProc(true);
  LPC_SC->PCONP &= ~this->pwrMask;//Отключение питания от перефирийного блока SSP
}


//Инициализация/изменение настроек SSP
int ssp_light::init(const portSettings& ps)
{
  unsigned int preclk;
  unsigned int cpsr;
  unsigned char scr;
  //Сохранение настроек порта
  this->ps = ps;
  //Проверка на корректность разрядности данных
  if((ps.dataBits < SSPSIZE_4) || (ps.dataBits > SSPSIZE_16))
    return TERR_WRONG_PARAM;
  //Проверка на корректность соответсвия частоты периферийного блока SSP и скорости работы интерфейса
  if(ps.baudrate > (ps.pclk / 2))
    return TERR_WRONG_PARAM;
  //Настройка делителей SSP в соответсвии с частотой периферийного блока SSP и скоростью работы интерфейса
  //в соответсвии с формулой BaudRate = PCLK/(CPSDVSR*(SCR+1)),где SCR(0..255), CPSDVSR(2..4..6..8.....254)
  preclk = ps.pclk / ps.baudrate;
  cpsr = (preclk >> 8);
  if(!cpsr)
    cpsr = 2;
  if(cpsr % 2)
   cpsr++;
  if(cpsr > 254)
    return TERR_WRONG_PARAM;
  scr = (preclk / cpsr) - 1;

  //Изменение параметров SSP
  this->low->CR1 = 0x00;
  this->low->CR0 &= ~0x0F;
  this->low->CR0 |= ps.dataBits;
  this->low->CPSR = cpsr;
  this->low->CR0 |= (scr << 8);
  this->low->CR1 = (1 << SSP_SSE);

  return ERR_OK;
}

int ssp_light::sendchar(unsigned char c)
{
  return ERR_NOT_SUPPORTED;
}


//Статический метод конфигурирования ножек SSP0 вариант 1
//SCK0  - P0.15
//MISO0 - P0.17
//MOSI0 - P0.18
void ssp_light::pinSetupSSP0_0(unsigned char setDefault)
{
  LPC_PINCON->PINSEL0 &= ~(3ul << 30);
  LPC_PINCON->PINSEL1 &= ~((3 << 2) | (3 << 4));

  if(!setDefault)
  {
    LPC_PINCON->PINSEL0 |= (2ul << 30);
    LPC_PINCON->PINSEL1 |= ((2 << 2) | (2 << 4));
  }
}

//Статический метод конфигурирования ножек SSP0 вариант 2
//SCK0  - P1.20
//MISO0 - P1.23
//MOSI0 - P1.24
void ssp_light::pinSetupSSP0_1(unsigned char setDefault)
{
  LPC_PINCON->PINSEL3 &= ~((3 << 8) | (3 << 14) | (3 << 16));

  if(!setDefault)
    LPC_PINCON->PINSEL3 |= ((3 << 8) | (3 << 14) | (3 << 16));
}

//Статический метод конфигурирования ножек SSP1 вариант 1
//SCK1  - P0.7
//MISO1 - P0.8
//MOSI1 - P0.9
void ssp_light::pinSetupSSP1_0(unsigned char setDefault)
{
  LPC_PINCON->PINSEL0 &= ~((3 << 14) | (3 << 16) | (3 << 18));

  if(!setDefault)
    LPC_PINCON->PINSEL0 |= ((2 << 14) | (2 << 16) | (2 << 18));
}

//Статический метод конфигурирования ножек SSP1 вариант 2
//SCK1  - P1.31
//MISO1 - P0.8
//MOSI1 - P0.9
void ssp_light::pinSetupSSP1_1(unsigned char setDefault)
{
  LPC_PINCON->PINSEL0 &= ~((3 << 16) | (3 << 18));
  LPC_PINCON->PINSEL3 &= ~(3ul << 30);

  if(!setDefault)
  {
    LPC_PINCON->PINSEL0 |= ((2 << 16) | (2 << 18));
    LPC_PINCON->PINSEL3 |= (2ul << 30);
  }
}

//Метод очитстки буферов
int ssp_light::purge(unsigned char queue)
{
  switch(queue)
  {
    case PURGE_RX:

      while(this->low->SR & (1 << RNE))//Очистка аппаратного FIFO буфера SSP
        if(this->low->DR);
    break;

    case PURGE_TX:
      return TERR_WRONG_PARAM;
  }
  return ERR_OK;
}

/*  */
int ssp_light::readAsync(unsigned char* buf, int bufsz, callback2* ac)
{
  return ERR_NOT_SUPPORTED;
}

/*  */
int ssp_light::readTimed(unsigned char* buf, int* bufsz, int timeout)
{
  return this->read(buf, *bufsz);
}

/*  */
int ssp_light::read(unsigned char* buf, int  bufsz)
{
  int i, j;

  if(!bufsz)
    return ERR_OK;
                                      /* блочная передача с использованием    * 
                                       * аппаратного FIFO                     */
  for(i = 0; i < (bufsz / 4); i++)
  {
    this->low->DR = 0;
    this->low->DR = 0;
    this->low->DR = 0;
    this->low->DR = 0;
    while (this->low->SR & (1 << BSY));

    *buf++ = this->low->DR;
    *buf++ = this->low->DR;
    *buf++ = this->low->DR;
    *buf++ = this->low->DR;
  }

  i = bufsz - (i * 4);                /* count for remaining bytes             */
  if(i)
  {                                   /* there is more data in buffer          */
    j = i;
    while(i--)
      this->low->DR = 0;
    while (this->low->SR & (1 << BSY));

    while(j--)
      *buf++ = this->low->DR;
  }
  return bufsz;

}

/*  */
int ssp_light::writeAsync(unsigned char* buf, int bufsz, callback2* ac)
{
  return ERR_NOT_SUPPORTED;
}

/*  */
int ssp_light::writeTimed(unsigned char* buf, int* bufsz, int timeout)
{
                                      /* игнорируем таймаут                   */
  return this->write(buf, *bufsz);
}

/*  */
int ssp_light::write(unsigned char* buf, int  bufsz)
{
  int bs = bufsz;
  int i, j;
  unsigned long dwordvar;

  if(!bufsz)
    return ERR_OK;
                                      /* блочная передача с использованием    * 
                                       * аппаратного FIFO                     */
  for(i = 0; i < (bufsz / 4); i++)
  {
    dwordvar = *((__packed unsigned long*)buf);/* read next 4 bytes           */
    buf += 4;                         /* move buffer pointer                  */
    this->low->DR = dwordvar >> 0;
    this->low->DR = dwordvar >> 8;
    this->low->DR = dwordvar >> 16;
    this->low->DR = dwordvar >> 24;
    while (this->low->SR & (1 << BSY));
                                       /* фиктивное чтения, для очистки RXFIFO*/
    dwordvar = this->low->DR;
    dwordvar = this->low->DR;
    dwordvar = this->low->DR;
    dwordvar = this->low->DR;
  }

  i = bufsz - (i * 4);                /* count for remaining bytes            */
  if(i)
  {                                   /* there is more data in buffer         */
    j = i;
    while(i--)
      this->low->DR = *buf++;
    while (this->low->SR & (1 << BSY));

    while(j--)
      dwordvar = this->low->DR;
  }
  return bs;
}


int ssp_light::cancelReadAsync(int reason)
{
  return ERR_NOT_SUPPORTED;
}

int ssp_light::cancelWriteAsync(int reason)
{
  return ERR_NOT_SUPPORTED;
}


