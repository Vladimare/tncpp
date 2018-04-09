#ifndef _USB_CDC_560_HPP_
#define _USB_CDC_560_HPP_

#include "usb_cdc.hpp"

/* драйвер usb_cdc для 560 платы */
class usb_cdc_560
  : public usb_cdc
{
public:
  usb_cdc_560(LPC_USB_TypeDef* phy, usbCreationDisposition* cd);
  virtual ~usb_cdc_560();

private:
                                      /* инициализация указателей на          *
                                       * дескрипторы                          */
  void initDescriptors();
                                      /* дескрипторы USB специфичные для 587  */
                                      /* платы                                */
  static const unsigned char deviceDescriptor[];
  static const unsigned char configurationDescriptor[];
  static const unsigned char stringDescriptor[];
};


#endif /*_USB_CDC_560_HPP_*/
