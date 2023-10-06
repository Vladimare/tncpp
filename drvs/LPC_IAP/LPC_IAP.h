#ifndef __LPC_IAP__H__
#define __LPC_IAP__H__

//IAP
#ifdef __cplusplus
extern "C"
{
#endif
extern unsigned long CCLK;            // CCLK in kHz
int EraseSector (unsigned long adr);
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf);

#ifdef __cplusplus
}
#endif

#endif
