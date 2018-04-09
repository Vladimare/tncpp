#ifndef _USBSVC_HPP_
#define _USBSVC_HPP_

#include "usbdef.h"
/******************************************************************************
 ********************************** exports ***********************************
 ******************************************************************************/
class
  usbsvc
{
protected:
  LPC_USB_TypeDef* low;         /* CMSIS interface to low level USB registers    */

public:
  void           usbsvcEPSetMaxSize(unsigned char ep , unsigned long maxsize);
  void           usbsvcEPIntClear  (unsigned char ep                        );
  void           usbsvcSetAddress  (unsigned char adr                       );
  void           usbsvcSIEWr       (unsigned long cmd                       );
  void           usbsvcSIEWrData   (unsigned long cmd, unsigned long data   );
  unsigned long  usbsvcSIERdData   (unsigned long cmd                       );
  unsigned long  usbsvcEPAdr       (unsigned long ep                        );
};

#endif /*_USBSVC_HPP_*/
