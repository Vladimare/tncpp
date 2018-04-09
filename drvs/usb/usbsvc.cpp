#include "usbsvc.hpp"


/* set up EP max packet size */
void usbsvc::usbsvcEPSetMaxSize(unsigned char ep, unsigned long maxsize)
{
  this->low->USBEpInd   = ep;           /* select EP                                      */
  this->low->USBMaxPSize= maxsize;      /* write maxsize                                  */
  while(!(this->low->USBDevIntSt & (1ul << EP_RLZED)));/* wait until process ends         */
  this->low->USBDevIntClr = (1ul << EP_RLZED);/* clear interrupt                          */
}

/* clear EP interrupt */
void usbsvc::usbsvcEPIntClear(unsigned char ep)
{
  this->low->USBDevIntClr = (1ul << CDFULL);/* clear interrupt                            */
  if(ep == EP_ALL)
  {
    unsigned char i = 0;
    for(i = 0; i < 32; i++)
      usbsvcEPIntClear(i);
    return;
  }
  else
    this->low->USBEpIntClr = (1ul << ep);/* clear target EP interrupt                     */

#if 0
  /* THIS IS ETERNAL CYCLE. CDFULL IS NEVER SET. DON'T KNOW WHY.              *
   * SAMPLE CODE FROM KEIL (..\Keil\ARM\Boards\Keil\MCB1700\USBHID\) DOESN'T  *
   *   CONTAIN THIS CYCLE                                                     */
  while(!(this->low->USBDevIntSt & (1ul << CDFULL)));/* wait until done                   */
  this->low->USBDevIntClr = (1ul << CDFULL);/* clear interrupt                            */
#endif
}

/* send command with no data to SIE */
void usbsvc::usbsvcSIEWr(unsigned long cmd)
{
  this->low->USBDevIntClr = (1ul << CCEMPTY);/* clear interrupt bit if set                */
  this->low->USBCmdCode   = CMD_PHASE_CMD | (cmd << 16);/* write command                  */
  if(cmd != SIE_VALIDATEBUFFER)
    while(!(this->low->USBDevIntSt & (1ul << CCEMPTY)));/* wait until ready               */
  this->low->USBDevIntClr = (1ul << CCEMPTY);/* clear interrupt                           */
}

/* send command with data to SIE */
void usbsvc::usbsvcSIEWrData(unsigned long cmd, unsigned long data)
{
  this->low->USBDevIntClr = (1ul << CCEMPTY);/* clear interrupt bit if set                */
  this->low->USBCmdCode   = CMD_PHASE_CMD | (cmd << 16);/* write command                  */
  while(!(this->low->USBDevIntSt & (1ul << CCEMPTY)));/* wait until ready                 */

  this->low->USBDevIntClr = (1ul << CCEMPTY);/* clear interrupt bit if set                */
  this->low->USBCmdCode   = CMD_PHASE_WR | (data << 16);/* write data                     */
  while(!(this->low->USBDevIntSt & (1ul << CCEMPTY)));/* wait until ready                 */
  this->low->USBDevIntClr = (1ul << CCEMPTY);/* clear interrupt                           */
}

/* read data from SIE */
unsigned long usbsvc::usbsvcSIERdData(unsigned long cmd)
{
  this->low->USBDevIntClr = (1ul << CCEMPTY);/* clear interrupt bit if set                */
  this->low->USBCmdCode   = CMD_PHASE_CMD | (cmd << 16);/* write command                  */
  while(!(this->low->USBDevIntSt & (1ul << CCEMPTY)));/* wait until ready                 */

  this->low->USBDevIntClr = (1ul << CCEMPTY) | (1ul << CDFULL);/* clear interrupt bits if set */
  this->low->USBCmdCode   = CMD_PHASE_RD | (cmd << 16);/* read data                       */
  while(!(this->low->USBDevIntSt & (1ul << CDFULL)));/* wait until ready                  */
  this->low->USBDevIntClr = (1ul << CCEMPTY) | (1ul << CDFULL);/* clear interrupt         */

  return this->low->USBCmdData;         /* return data                                    */
}

/* get endpoint physical address from special encoded value *
 *   parameters:      ep     : endpoint number              *
 *                    ep.0..3: address                      *
 *                    ep.7   : dir                          *
 *   return value:    endpoint physical address             */
unsigned long usbsvc::usbsvcEPAdr(unsigned long ep)
{
  unsigned long value = 0;

  value = (ep & 0x0F) << 1;           /* endpoint base                                  */
  if(ep & 0x80)
    value += 1;                       /* consider direction                             */

  return value;
}

void usbsvc::usbsvcSetAddress(unsigned char adr)
{
  usbsvcSIEWrData(SIE_SETADDRESS, adr | SIED_DEV_EN);/* set address and enable device   */
  usbsvcSIEWrData(SIE_SETADDRESS, adr | SIED_DEV_EN);
}
