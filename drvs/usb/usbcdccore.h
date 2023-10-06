#ifndef _USBCDCCORE_H_
#define _USBCDCCORE_H_

#include "usbcoredef.h"

/* communication device class specification version 1.20 */
#define CDC_V1_20                               0x0120

/* communication interface USB class code             *
 *   (www.usb.org, table 2 USB CDC 1.2 specification) */
#define CDC_INTERFACE_CLASS   USB_DEVICE_CLASS_COMMUNICATIONS

/* data interface USB class code                      *
 *   (www.usb.org, table 6 USB CDC 1.2 specification) */
#define CDC_DATA_INTERFACE_CLASS                0x0A

/* comminucation interface class subclass codes *
 *   (table 4 USB CDC 1.2 specification)        */
#define CDC_SUBCLASS_DIRECT_LINE_CONTROL_MODEL  0x01
#define CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL     0x02
#define CDC_SUBCLASS_TELEPHONE_CONTROL_MODEL    0x03
#define CDC_SUBCLASS_MULTICHANNEL_CONTROL_MODEL 0x04
#define CDC_SUBCLASS_CAPI_CONTROL_MODEL         0x05
#define CDC_SUBCLASS_ETHERNET_NETWORKING_CONTROL_MODEL 0x06
#define CDC_SUBCLASS_ATM_NETWORKING_CONTROL_MODEL 0x07
#define CDC_SUBCLASS_WIRELESS_HANDSET_CONTROL_MODEL 0x08
#define CDC_SUBCLASS_DEVICE_MANAGEMENT          0x09
#define CDC_SUBCLASS_MOBILE_DIRECT_LINE_MODEL   0x0A
#define CDC_SUBCLASS_OBEX                       0x0B
#define CDC_SUBCLASS_ETHERNET_EMULATION_MODEL   0x0C

/*  values for bDescriptorType field of functional descriptors  *
 *   (table 12 USB CDC 1.2 specification)                       */
#define CDC_CS_INTERFACE                        0x24
#define CDC_CS_ENDPOINT                         0x25

/* values for bDescriptorSubtype field of functional descriptors  *
 *   (table 13 USB CDC 1.2 specification)                         */
#define CDC_HEADER                              0x00
#define CDC_CALL_MANAGEMENT                     0x01
#define CDC_ABSTRACT_CONTROL_MANAGEMENT         0x02
#define CDC_DIRECT_LINE_MANAGEMENT              0x03
#define CDC_TELEPHONE_RINGER                    0x04
#define CDC_REPORTING_CAPABILITIES              0x05
#define CDC_UNION                               0x06
#define CDC_COUNTRY_SELECTION                   0x07
#define CDC_TELEPHONE_OPERATIONAL_MODES         0x08
#define CDC_USB_TERMINAL                        0x09
#define CDC_NETWORK_CHANNEL                     0x0A
#define CDC_PROTOCOL_UNIT                       0x0B
#define CDC_EXTENSION_UNIT                      0x0C
#define CDC_MULTICHANNEL_MANAGEMENT             0x0D
#define CDC_CAPI_CONTROL_MANAGEMENT             0x0E
#define CDC_ETHERNET_NETWORKING                 0x0F
#define CDC_ATM_NETWORKING                      0x10
#define CDC_WIRELESS_HANDSET_CONTROL_MODEL      0x11
#define CDC_MOBILE_DIRECT_LINE_MODEL            0x12
#define CDC_MDLM_DETAIL                         0x13
#define CDC_DEVICE_MANAGEMENT_MODEL             0x14
#define CDC_OBEX                                0x15
#define CDC_COMMAND_SET                         0x16
#define CDC_COMMAND_SET_DETAIL                  0x17
#define CDC_TELEPHONE_CONTROL_MODEL             0x18
#define CDC_OBEX_SERVICE_IDENTIFIER             0x19

/* CDC class-specific request codes       *
 *   (table 19 USB CDC 1.2 specification) */
#define CDC_SEND_ENCAPSULATED_COMMAND           0x00
#define CDC_GET_ENCAPSULATED_RESPONSE           0x01
#define CDC_SET_COMM_FEATURE                    0x02
#define CDC_GET_COMM_FEATURE                    0x03
#define CDC_CLEAR_COMM_FEATURE                  0x04
#define CDC_SET_AUX_LINE_STATE                  0x10
#define CDC_SET_HOOK_STATE                      0x11
#define CDC_PULSE_SETUP                         0x12
#define CDC_SEND_PULSE                          0x13
#define CDC_SET_PULSE_TIME                      0x14
#define CDC_RING_AUX_JACK                       0x15
#define CDC_SET_LINE_CODING                     0x20
#define CDC_GET_LINE_CODING                     0x21
#define CDC_SET_CONTROL_LINE_STATE              0x22
#define CDC_SEND_BREAK                          0x23
#define CDC_SET_RINGER_PARMS                    0x30
#define CDC_GET_RINGER_PARMS                    0x31
#define CDC_SET_OPERATION_PARMS                 0x32
#define CDC_GET_OPERATION_PARMS                 0x33
#define CDC_SET_LINE_PARMS                      0x34
#define CDC_GET_LINE_PARMS                      0x35
#define CDC_DIAL_DIGITS                         0x36
#define CDC_SET_UNIT_PARAMETER                  0x37
#define CDC_GET_UNIT_PARAMETER                  0x38
#define CDC_CLEAR_UNIT_PARAMETER                0x39
#define CDC_GET_PROFILE                         0x3A
#define CDC_SET_ETHERNET_MULTICAST_FILTERS      0x40
#define CDC_SET_ETHERNET_PMP_FILTER             0x41
#define CDC_GET_ETHERNET_PMP_FILTER             0x42
#define CDC_SET_ETHERNET_PACKET_FILTER          0x43
#define CDC_GET_ETHERNET_STATISTIC              0x44
#define CDC_SET_ATM_DATA_FORMAT                 0x50
#define CDC_GET_ATM_DEVICE_STATISTICS           0x51
#define CDC_SET_ATM_DEFAULT_VC                  0x52
#define CDC_GET_ATM_VC_STATISTICS               0x53

#endif /*_USBCDCCORE_H_*/
