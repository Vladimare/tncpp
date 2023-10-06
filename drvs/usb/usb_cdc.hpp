#ifndef _USB_CDC_HPP_
#define _USB_CDC_HPP_

#include "lpc17xx_defs.h"
//#include "mutex.hpp"
#include "serial.hpp"
#include "event.hpp"
#include "usbsvc.hpp"
#include "usbcoredef.h"
#include "usbdef.h"
#include "usbuserdef.h"
#include "errors.h"

struct usbCreationDisposition
  : public serialCreationDisposition
{
  unsigned short SWversion;
  unsigned long  serial;
};

class usb_cdc
  : private usbsvc,
    public rs232Port
{
public:
  usb_cdc(LPC_USB_TypeDef* phy, usbCreationDisposition* cd);
  virtual ~usb_cdc();

  /******************************* реализация serialPort ********************************/
  virtual int init      (const portSettings& ps);
  virtual const portSettings& settings();
  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);
  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);
  virtual int cancelReadAsync (int reason);
  virtual int cancelWriteAsync(int reason);
  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);

  /******************************** реализация rs232Port ********************************/
  virtual int setRS232(int  pinlist);
  virtual int getRS232(int* pinlist);


private:
  PIN_SETUP       pinSetupProc;       /* метод управления ногами порта                  */
  unsigned char   irqNum;             /* номер прерывания порта                         */
  unsigned long   pconpMask;          /* маска в PCONP                                  */
  portSettings    ps;                 /* текущие настройки порта                        */
  unsigned char   irqPrior;
protected:
  unsigned short  SWversion;          /* версия ПО                                      */
  unsigned char*  serialString;       /* серийный номер                                 */

#define EVT_RXCOMPLETE  0x01
#define EVT_TXCOMPLETE  0x02
  eventpack*      evts;               /* набор событий                                  */
  //CIRC_BUF128     inbuf;              /* программный буфер-расширитель аппаратного FIFO */
  bool            isExtReceiver;      /* true - приемное прерывание пишет данные в буфер*/
                                      /*   rxbuf, false - в буфер inbuf                 */

  callback2*      rxcompleter;        /* обработчик завершения асинхронного чтения      */
  unsigned char*  rxbuf;              /* указатель на буфер-аргумент функции чтения     */
  unsigned long   rxbufLength;        /* длина буфера по указателю rxbuf                */
  unsigned long   rxbufRemain;        /* количество байт, которое осталось прочитать в  */
                                      /*   буфер rxbuf                                  */

  callback2*      txcompleter;        /* обработчик завершения асинхронной записи       */
  unsigned char*  txbuf;              /* указатель на буфер-аргумент функции записи     */
  unsigned long   txbufLength;        /* длина буфера по указателю txbuf                */
  unsigned long   txbufRemain;        /* количество байт, которое осталось записать из  */
                                      /*   буфера txbuf                                 */

  void onTimedReadComplete(int, int); /* внутренний обработчик асинхронного чтения,     */
                                      /*   используется методом readTimed()             */
  void onTimedWriteComplete(int, int);/* внутренний обработчик асинхронной записи,      */
                                      /*   используется методом writeTimed()            */

  /******************************* экспорт файла usb.c   ********************************/
  /****************** методы настройки ножек под аппаратные реализации ******************/
public:
  static void pinSetupUSB0   (unsigned char setDefault);
  static void pinSetupUSB1   (unsigned char setDefault);
private:
  static void irqh(void* _this);      /* обработчик прерываний порта                    */
  void usbReset();
  //unsigned long  usbOpen         (void                                                                 );
  unsigned long  usbInit         (const portSettings* ps                                              );
  unsigned long  usbClose        (void                                                                 );
  unsigned long  usbReadEP       (unsigned long EPNum,       unsigned char* pData, unsigned long length);
  unsigned long  usbWriteEP      (unsigned long EPNum, const unsigned char* pData, unsigned long count );
  void           usbSetStallEP   (unsigned long EPNum                                                  );
  void           usbClearStallEP (unsigned long EPNum                                                  );
  void           usbDisableEP    (unsigned long EPNum                                                  );
  void           usbEnableEP     (unsigned long EPNum                                                  );
  void           usbResetEP      (unsigned long EPNum                                                  );
  void           usbConfigure    (unsigned char cfg                                                    );
  void           usbConfigureEP  (USB_ENDPOINT_DESCRIPTOR* pEPD                                        );

  /* USB interrupts handlers list */
  void usbIRQhSlowEP  (void);
  void usbIRQhDevState(void);

  #if 0
  void usbIRQhSOF     (void);
  void usbIRQhFastEP  (void);
  void usbIRQhCCempty (void);
  void usbIRQhCDfull  (void);
  void usbIRQhRxEndPkt(void);
  void usbIRQhTxEndPkt(void);
  void usbIRQhEPrlzed (void);
  #endif

  void usbIRQhErrInt  (void);

  /******************************* экспорт файла usbuser.c ******************************/
  //void (*const usbEPHandler[16])(unsigned long event);
  callback1* usbEPHandler[16];
  USB_SETUP_PACKET setupPacket;
  USB_EP_DATA      ep0data;

  CIRC_BUF1024         usbbuf_in;
  CIRC_BUF1024         usbbuf_out;

public:
  unsigned long  usbuserAvailChar    (unsigned long* availChar);
  unsigned long  usbuserRead         (unsigned char* buffer, unsigned long length);
  //unsigned long  usbuserWrite        (unsigned char* buffer, unsigned long length);
private:

  void usbEPHandler0 (int event);
  void usbEPHandler1 (int event);
  void usbEPHandler2 (int event);
  void usbEPHandler3 (int event);
  void usbEPHandler4 (int event);
  void usbEPHandler5 (int event);
  void usbEPHandler6 (int event);
  void usbEPHandler7 (int event);
  void usbEPHandler8 (int event);
  void usbEPHandler9 (int event);
  void usbEPHandler10(int event);
  void usbEPHandler11(int event);
  void usbEPHandler12(int event);
  void usbEPHandler13(int event);
  void usbEPHandler14(int event);
  void usbEPHandler15(int event);
  unsigned char usbuserGetLineCoding(void);

  /******************************* экспорт файла usbcore.c ******************************/
  unsigned short  usbcore_deviceStatus;
  unsigned char   usbcore_deviceAddress;
  unsigned char   usbcore_configuration;
  unsigned long   usbcore_epMask;
  unsigned long   usbcore_epHalt;
  unsigned char   usbcore_numIfaces;
  unsigned char   ep0buf[USB_PACKET0_MAXSIZE];
  unsigned char   usbcore_altSettings[4];

  void           usbcoreReset           (void);
  void           usbcoreSetupStage      (void);
  void           usbcoreDataInStage     (void);
  void           usbcoreDataOutStage    (void);
  void           usbcoreStatusInStage   (void);
  void           usbcoreStatusOutStage  (void);
  unsigned char  usbcoreGetStatus       (void);
  unsigned char  usbcoreSetClearFeature (unsigned long sc);
  unsigned char  usbcoreGetDescriptor   (void);
  unsigned char  usbcoreSetConfiguration(void);
  unsigned char  usbcoreSetInterface    (void);

  /******************************* экспорт файла usbdescriptors.h ******************************/
protected:
  unsigned char  usb_idx_serial;
  const unsigned char* usb_deviceDescriptor;
  const unsigned char* usb_configurationDescriptor;
  const unsigned char* usb_stringDescriptor;

};

/* реентерабельная версия uart17xx (требует наличия класса mutex, неважно на чем сделанного) */
#ifdef _MUTEX_HPP__

class usb_cdc_safe
  : public usb_cdc
{
public:
  uart17xx_safe(LPC_UART_TypeDef* phy, serialCreationDisposition* cd);
  virtual ~uart17xx_safe();

  /******************************* реализация serialPort ********************************/
  virtual int init      (const portSettings& ps);
  //virtual const portSettings& settings();
  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);
  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);
  virtual int cancelReadAsync (int reason);
  virtual int cancelWriteAsync(int reason);
  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);

  /******************************** реализация rs232Port ********************************/
  //virtual int setRS232(int  pinlist);
  //virtual int getRS232(int* pinlist);

private:
  mutex rlock;
  mutex wlock;
};

#endif /*_MUTEX_HPP_*/

#endif /*_USB_CDC_HPP_*/
