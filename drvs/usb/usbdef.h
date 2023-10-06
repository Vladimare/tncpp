#ifndef _USBDEF_H_
#define _USBDEF_H_

#include <lpc17xx.h>

#include "core.h"

#include "lpc17xx_defs.h"
#include "usbcoredef.h"
#include "usbcdccore.h"

/******************************************************************************
 ******************************* configuration ********************************
 ******************************************************************************/
#define USB_IRQH_PRIORITY   3         /* USB IRQ handler priority                       */

                                      /* USB UP_LED control modes                       */
#define USB_UPLED_MODE_AUTO 0         /* USB UP_LED controlled by hardware              */
#define USB_UPLED_MODE_MANUAL 1       /* USB UP_LED controlled by software              */
#define USB_UPLED_MODE_OFF  2         /* USB UP_LED not controlled at all               */
#define USB_UPLED_MODE      USB_UPLED_MODE_MANUAL

#define USB_PACKET0_MAXSIZE     64    /* max size for EP0 packets                       */
#define USB_PACKETCDC_MAXSIZE   64    /* max packet size for CDC interface              *
                                       *   should not be over 64 bytes, because of      *
                                       *   inadequate intermediate hosts                */

  #define USB_EP0          (1ul <<  0)/*  */
  #define USB_EP1          (1ul <<  1)/*  */
  #define USB_EP2          (1ul <<  2)/*  */
  #define USB_EP3          (1ul <<  3)/*  */
  #define USB_EP4          (1ul <<  4)/*  */
  #define USB_EP5          (1ul <<  5)/*  */
  #define USB_EP6          (1ul <<  6)/*  */
  #define USB_EP7          (1ul <<  7)/*  */
  #define USB_EP8          (1ul <<  8)/*  */
  #define USB_EP9          (1ul <<  9)/*  */
  #define USB_EP10         (1ul << 10)/*  */
  #define USB_EP11         (1ul << 11)/*  */
  #define USB_EP12         (1ul << 12)/*  */
  #define USB_EP13         (1ul << 13)/*  */
  #define USB_EP14         (1ul << 14)/*  */
  #define USB_EP15         (1ul << 15)/*  */
#define USB_EP_EVENT_LIST  (USB_EP0 | USB_EP1 | USB_EP2) /* used endpoints              */

#define USB_SELF_POWERED         1    /* 1 - device powered from local source           *
                                       * 0 - device powered from USB bus                */
#define USB_SELF_DRAIN         100    /* power consumption from USB bus (mA)            *
                                       *   used if (USB_SELF_POWERED == 1) only         */

/******************************************************************************
 ****************************** constant defines ******************************
 ******************************************************************************/

/* endpoints */
#define EPphy(n)               (n)    /* endpoint numbers                               */
#define EP_ALL                0xFF    /* all endpoints                                  */
#define EP_MASK_CTRL        0x0001    /* control endpoints logical address mask         */
#define EP_MASK_BULK        0xC924    /* bulk endpoint logical address mask             */
#define EP_MASK_INT         0x2492    /* interrupt endpoint logical address mask        */
#define EP_MASK_ISO         0x1248    /* isochronous endpoint logical address mask      */

/******************************************************************************
 ************************* service macros definitions *************************
 ******************************************************************************/

#define USB_UPLED_UP    (LPC_GPIO1->FIOCLR |= (1ul << P1_18))
#define USB_UPLED_DOWN  (LPC_GPIO1->FIOSET |= (1ul << P1_18))

/******************************************************************************
 ***************************** SIE command codes ******************************
 ******************************************************************************/
#define SIE_SETADDRESS        0xD0    /*  */
  #define SIED_DEV_EN       (1ul << 7)/*  */

#define SIE_CONFIGDEVICE      0xD8    /*  */
  #define SIED_CONF_DEVICE  (1ul << 0)/*  */

#define SIE_SETMODE           0xF3    /*  */
  #define SIED_AP_CLK       (1ul << 0)/*  */
  #define SIED_INAK_CI      (1ul << 1)/*  */
  #define SIED_INAK_CO      (1ul << 2)/*  */
  #define SIED_INAK_II      (1ul << 3)/*  */
  #define SIED_INAK_IO      (1ul << 4)/*  */
  #define SIED_INAK_BI      (1ul << 5)/*  */
  #define SIED_INAK_BO      (1ul << 6)/*  */

#define SIE_RD_CURFRAME_NMB   0xF5    /*  */

#define SIE_RD_TESTREGISTER   0xFD    /*  */

#define SIE_SETDEVSTATUS      0xFE    /*  */
  #define SIED_CON          (1ul << 0)/*  */
  #define SIED_CON_CH       (1ul << 1)/*  */
  #define SIED_SUS          (1ul << 2)/*  */
  #define SIED_SUS_CH       (1ul << 3)/*  */
  #define SIED_RST          (1ul << 4)/*  */

#define SIE_GETDEVSTATUS      0xFE    /*  */

#define SIE_GETERRCODE        0xFF    /*  */
  #define SIERR_NOERR         0x00    /*  */
  #define SIERR_PIDENCODING   0x01    /*  */
  #define SIERR_PIDUNKNOWN    0x02    /*  */
  #define SIERR_PACKETUNEXPECT 0x03   /*  */
  #define SIERR_CRC_TOKEN     0x04    /*  */
  #define SIERR_CRC_DATA      0x05    /*  */
  #define SIERR_TIMEOUT       0x06    /*  */
  #define SIERR_BABBLE        0x07    /*  */
  #define SIERR_ENDOFPACKET   0x08    /*  */
  #define SIERR_NAK           0x09    /*  */
  #define SIERR_SENTSTALL     0x0A    /*  */
  #define SIERR_BUFOVERRUN    0x0B    /*  */
  #define SIERR_SENTEMPTYPACKET 0x0C  /*  */
  #define SIERR_BITSTUFF      0x0D    /*  */
  #define SIERR_SYNC          0x0E    /*  */
  #define SIERR_TOGGLEBITinPID 0x0F   /*  */

#define SIE_RD_ERRSTATUS      0xFB    /*  */
  #define SIERRSTAT_PID_ERR (1ul << 0)/*  */
  #define SIERRSTAT_UEPKT   (1ul << 1)/*  */
  #define SIERRSTAT_DCRC    (1ul << 2)/*  */
  #define SIERRSTAT_TIMEOUT (1ul << 3)/*  */
  #define SIERRSTAT_EOP     (1ul << 4)/*  */
  #define SIERRSTAT_B_OVRN  (1ul << 5)/*  */
  #define SIERRSTAT_BTSTF   (1ul << 6)/*  */
  #define SIERRSTAT_TGL_ERR (1ul << 7)/*  */

#define SIE_SELECT_EP(n)    (n)       /*  */
  #define SIED_FE           (1ul << 0)/*  */
  #define SIED_STi          (1ul << 1)/*  */
  #define SIED_STP          (1ul << 2)/*  */
  #define SIED_PO           (1ul << 3)/*  */
  #define SIED_EPN          (1ul << 4)/*  */
  #define SIED_B_1_FULL     (1ul << 5)/*  */
  #define SIED_B_2_FULL     (1ul << 6)/*  */

#define SIE_SELECT_EP_CLEAR_INT(n) (0x40 + (n)) /*  */

#define SIE_SET_EPSTATUS(n) (0x40 + (n))/*  */
  #define SIED_STb          (1ul << 0)/*  */
  #define SIED_DA           (1ul << 5)/*  */
  #define SIED_RF_MO        (1ul << 6)/*  */
  #define SIED_CND_ST       (1ul << 0)/*  */

#define SIE_CLEARBUFFER       0xF2    /*  */

#define SIE_VALIDATEBUFFER    0xFA    /*  */

#endif /*_USBDEF_H_*/
