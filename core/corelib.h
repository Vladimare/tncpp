#ifndef _CORELIB_H_
#define _CORELIB_H_

#ifdef LPC17xx_PROJECT
# include <lpc17xx.h>
#endif

#ifdef LPC13xx_PROJECT
# include <lpc13xx.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef PROJECTLDR
void crc32_table_make(void);
#endif
extern   signed char      sign             (double             x);

//extern unsigned long      ntohl            (unsigned long      a);
#define ntohl       __REV
#define ntohs(val)  ((unsigned short)((__REV(val) >> 16) & 0xFFFF))

extern unsigned char      hex2ascii        (unsigned char      hex);
extern unsigned short     encodeSelfCorrect(unsigned char      byte);
extern unsigned char      decodeSelfCorrect(unsigned short     code);
extern unsigned char      ascii2hex        (unsigned char      ascii);
extern unsigned char      lowestBitPos     (unsigned long      value);
extern unsigned long      get_bit_count    (unsigned long      value);
extern unsigned long long get_bit_count64  (unsigned long long value);

//extern unsigned long      revbin           (unsigned long      value);
#define revbin  __RBIT

//extern unsigned char      revbin8          (unsigned char      value);
#define revbin8 (__RBIT(value) >> 24)

//extern unsigned long long revbin64         (unsigned long long value);
#define revbin64  (((unsigned long long)__RBIT(value) << 32ull) | \
                   ((unsigned long long)__RBIT(value  >> 32ull)))

extern unsigned int       bindecValue      (unsigned char*     buf, int            bufsz);
extern unsigned char      bin2bindec       (unsigned char*     buf, unsigned char* bufsz,  unsigned int   num);
extern unsigned long      crc32Step        (unsigned long      crc, unsigned char  data);
extern unsigned char      cs8bitStep       (unsigned char      crc, unsigned char  data);
extern unsigned char      cs8bit           (unsigned char*     buf, unsigned char  length);
extern unsigned short     crc16            (unsigned char*     buf, unsigned short size,   unsigned short init);
extern unsigned long      crc32            (unsigned char*     buf, unsigned long  length, unsigned long  scrc);
extern unsigned long      linear_approx    (unsigned long*     rv,  unsigned long* av,     unsigned char  l);
extern unsigned short     median_filter3   (unsigned short*    rv);
extern void               readDigitString  (unsigned char*     out, unsigned char* in,     unsigned char  idx, unsigned char count);

#ifdef __cplusplus
}
#endif

#endif /*_CORELIB_H_*/
