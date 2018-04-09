#ifndef _USBUSERDEF_H_
#define _USBUSERDEF_H_

/******************************************************************************
 ****************************** constant defines ******************************
 ******************************************************************************/

/* USB EP callback events */
#define USB_EVT_SETUP              1  /* setup packet                                   */
#define USB_EVT_OUT                2  /* OUT packet                                     */
#define USB_EVT_IN                 3  /*  IN packet                                     */
#define USB_EVT_OUT_NAK            4  /* OUT packet - not acknowledged                  */
#define USB_EVT_IN_NAK             5  /*  IN packet - not acknowledged                  */
#define USB_EVT_OUT_STALL          6  /* OUT packet - stalled                           */
#define USB_EVT_IN_STALL           7  /*  IN packet - stalled                           */
#define USB_EVT_OUT_DMA_EOT        8  /* DMA OUT EP - end of transfer                   */
#define USB_EVT_IN_DMA_EOT         9  /* DMA  IN EP - end of transfer                   */
#define USB_EVT_OUT_DMA_NDR       10  /* DMA OUT EP - new descriptor request            */
#define USB_EVT_IN_DMA_NDR        11  /* DMA  IN EP - new descriptor request            */
#define USB_EVT_OUT_DMA_ERR       12  /* DMA OUT EP - error                             */
#define USB_EVT_IN_DMA_ERR        13  /* DMA  IN EP - error                             */

#endif /*_USBUSERDEF_H_*/
