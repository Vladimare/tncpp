#ifndef _USBCOREDEF_H_
#define _USBCOREDEF_H_

/* usbcore_deviceStatus bit values */
#define USB_STATUS_SELF_POWERED        (1ul << 0)
#define USB_STATUS_REMOTE_WAKEUP       (1ul << 1)

/* bmRequestType types (table 9.2 USB 2.0 specification) */
#define REQUEST_STANDARD                0
#define REQUEST_CLASS                   1
#define REQUEST_VENDOR                  2
#define REQUEST_RESERVED                3

/* bmRequestType recipients (table 9.2 USB 2.0 specification) */
#define REQUEST_TO_DEVICE               0
#define REQUEST_TO_INTERFACE            1
#define REQUEST_TO_ENDPOINT             2
#define REQUEST_TO_OTHER                3

/* USB standard request codes (table 9.4 USB 2.0 specification) */
#define USB_REQUEST_GET_STATUS          0
#define USB_REQUEST_CLEAR_FEATURE       1
#define USB_REQUEST_SET_FEATURE         3
#define USB_REQUEST_SET_ADDRESS         5
#define USB_REQUEST_GET_DESCRIPTOR      6
#define USB_REQUEST_SET_DESCRIPTOR      7
#define USB_REQUEST_GET_CONFIGURATION   8
#define USB_REQUEST_SET_CONFIGURATION   9
#define USB_REQUEST_GET_INTERFACE       10
#define USB_REQUEST_SET_INTERFACE       11
#define USB_REQUEST_SYNC_FRAME          12

/* USB descriptor types (table 9.5 USB 2.0 specification) */
#define USB_DESCRIPTOR_TYPE_DEVICE      1
#define USB_DESCRIPTOR_TYPE_CONFIGURATION 2
#define USB_DESCRIPTOR_TYPE_STRING      3
#define USB_DESCRIPTOR_TYPE_INTERFACE   4
#define USB_DESCRIPTOR_TYPE_ENDPOINT    5
#define USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER 6
#define USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG 7
#define USB_DESCRIPTOR_TYPE_INTERFACE_POWER 8

/* USB standard feature selectors (table 9.6 USB 2.0 specification) */
#define USB_FEATURE_REMOTE_WAKEUP       1
#define USB_FEATURE_ENDPOINT_HALT       0

/* USB device classes (www.usb.org) */
#define USB_DEVICE_CLASS_RESERVED       0x00
#define USB_DEVICE_CLASS_AUDIO          0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS 0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE 0x03
#define USB_DEVICE_CLASS_MONITOR        0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER          0x06
#define USB_DEVICE_CLASS_PRINTER        0x07
#define USB_DEVICE_CLASS_STORAGE        0x08
#define USB_DEVICE_CLASS_HUB            0x09
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC 0xFF

/* bmAttributes in standard configuration descriptor (table 9.10 in USB 2.0 specification) */
#define USB_CONFIG_SELF_POWERED         0x40
#define USB_CONFIG_REMOTE_WAKEUP        0x20

/* bMaxPower in standard configuration descriptor (table 9.10 in USB 2.0 specification) */
#define USB_CONFIG_POWER_mA(mA)         ((mA) / 2)

/* bEndpointAddress in endpoint descriptor (table 9.13 in USB 2.0 specification) */
#define USB_ENDPOINT_DIRECTION_MASK     0x80
#define USB_ENDPOINT_OUT(addr)          ((addr) | 0x00)
#define USB_ENDPOINT_IN(addr)           ((addr) | 0x80)

/* bmAttributes in endpoint descriptor (table 9.13 in USB 2.0 specification) */
#define USB_ENDPOINT_TYPE_MASK          0x03
#define USB_ENDPOINT_TYPE_CONTROL       0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS   0x01
#define USB_ENDPOINT_TYPE_BULK          0x02
#define USB_ENDPOINT_TYPE_INTERRUPT     0x03

#define USB_ENDPOINT_SYNC_MASK          0x0C
#define USB_ENDPOINT_SYNC_NO_SYNCHRONIZATION 0x00
#define USB_ENDPOINT_SYNC_ASYNCHRONOUS  0x04
#define USB_ENDPOINT_SYNC_ADAPTIVE      0x08
#define USB_ENDPOINT_SYNC_SYNCHRONOUS   0x0C

#define USB_ENDPOINT_USAGE_MASK         0x30
#define USB_ENDPOINT_USAGE_DATA         0x00
#define USB_ENDPOINT_USAGE_FEEDBACK     0x10
#define USB_ENDPOINT_USAGE_IMPLICIT_FEEDBACK 0x20
#define USB_ENDPOINT_USAGE_RESERVED     0x30

/* service types */
typedef __packed union
{
  unsigned short w;
  __packed struct
  {
    unsigned char l;
    unsigned char h;
  }wb;
}WORD_BYTE;

/* bmRequestType field definition */
typedef __packed union _REQUEST_TYPE
{
  __packed struct _bm
  {
    unsigned char recipient :5;
    unsigned char type      :2;
    unsigned char dir       :1;
  }bm;
  unsigned char b;
}REQUEST_TYPE;

/* USB endpoint data structure */
typedef struct _USB_EP_DATA
{
  unsigned char* pData;
  unsigned short count;
}USB_EP_DATA;

/* USB default control pipe setup packet  *
 *   (see p.9.3 of USB 2.0 specification) */
typedef __packed struct _USB_SETUP_PACKET
{
  REQUEST_TYPE    bmRequestType;
  unsigned char   bRequest;
  WORD_BYTE       wValue;
  WORD_BYTE       wIndex;
  unsigned short  wLength;
}USB_SETUP_PACKET;

/* USB standard device descriptor (table 9.8 USB 2.0 specification) */
typedef __packed struct _USB_DEVICE_DESCRIPTOR
{
  unsigned char   bLength;
  unsigned char   bDescriptorType;
  unsigned short  bcdUSB;
  unsigned char   bDeviceClass;
  unsigned char   bDeviceSubClass;
  unsigned char   bDeviceProtocol;
  unsigned char   bMaxPacketSize0;
  unsigned short  idVendor;
  unsigned short  idProduct;
  unsigned short  bcdDevice;
  unsigned char   iManufacturer;
  unsigned char   iProduct;
  unsigned char   iSerialNumber;
  unsigned char   bNumConfigurations;
}USB_DEVICE_DESCRIPTOR;

/* USB standard configuration descriptor (table 9.10 USB 2.0 specification) */
typedef __packed struct _USB_CONFIGURATION_DESCRIPTOR
{
  unsigned char   bLength;
  unsigned char   bDescriptorType;
  unsigned short  wTotalLength;
  unsigned char   bNumInterfaces;
  unsigned char   bConfigurationValue;
  unsigned char   iConfiguration;
  unsigned char   bmAttributes;
  unsigned char   bMaxPower;
}USB_CONFIGURATION_DESCRIPTOR;

/* USB standard interface descriptor (table 9.12 USB 2.0 specification) */
typedef __packed struct _USB_INTERFACE_DESCRIPTOR
{
  unsigned char   bLength;
  unsigned char   bDescriptorType;
  unsigned char   bInterfaceNumber;
  unsigned char   bAlternateSetting;
  unsigned char   bNumEndpoints;
  unsigned char   bInterfaceClass;
  unsigned char   bInterfaceSubClass;
  unsigned char   bInterfaceProtocol;
  unsigned char   iInterface;
}USB_INTERFACE_DESCRIPTOR;

/* USB standard endpoint descriptor (table 9.13 USB 2.0 specification) */
typedef __packed struct _USB_ENDPOINT_DESCRIPTOR
{
  unsigned char   bLength;
  unsigned char   bDescriptorType;
  unsigned char   bEndpointAddress;
  unsigned char   bmAttributes;
  unsigned short  wMaxPacketSize;
  unsigned char   bInterval;
}USB_ENDPOINT_DESCRIPTOR;

/* USB unicode string descriptor (table 9.16 USB 2.0 specification) */
typedef __packed struct _USB_STRING_DESCRIPTOR
{
  unsigned char   bLength;
  unsigned char   bDescriptorType;
  unsigned short  bString;
}USB_STRING_DESCRIPTOR;

/* USB common descriptor */
typedef __packed struct _USB_COMMON_DESCRIPTOR
{
  unsigned char   bLength;
  unsigned char   bDescriptorType;
}USB_COMMON_DESCRIPTOR;


#endif /*_USBCOREDEF_H_*/
