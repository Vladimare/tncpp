#ifndef _TN_NET_TYPES_H_
#define _TN_NET_TYPES_H_


#include "corelib.h"


#ifndef   FALSE
# define  FALSE 0
#endif

#ifndef   TRUE
# define  TRUE  1
#endif


typedef unsigned char   u_char;
typedef unsigned char   u_int8_t;

typedef unsigned short  u_short;
typedef unsigned short  u_int16_t;

typedef unsigned int    u32_t;
typedef unsigned int    u_int32_t;

/*
 * Add your compiler pack directives here (if not defined)
 */
#if defined (__GNUC__)                //-- GCC (CrossWorks, CodeSourcery, etc)
# define __pkt_field
# define __pkt_struct     __attribute((packed))
# define __pkt_struct_ex  __attribute((packed))
#elif defined (__CC_ARM)              //-- ARM(c) (ARM, KEIL, etc)
# define __pkt_field   __packed
# define __pkt_struct  __attribute((packed))
# define __pkt_struct_ex
#elif defined (__ICCARM__)            //-- IAR ARM
# define __pkt_field
# define __pkt_struct
# define __pkt_struct_ex
#elif defined (WIN32)
# define __pkt_field
# define __pkt_struct
# define __pkt_struct_ex
#else                                 //-- Undefined compiler
# error Unknown compiler settings for the packet data structures !!!
#endif

/*
 * Convert network byte order to the CPU byte order
 */
#if 0                                 //-- Defined by corelib.h
# if (CPU_BYTE_ORDER == BIG_ENDIAN)
#  define ntohl(val)  (val)
#  define ntohs(val)  (val)
# endif
# if (CPU_BYTE_ORDER == LITTLE_ENDIAN)
#  define ntohl(val)  ((((val) & 0x000000FF) << 24) | (((val) & 0x0000FF00) <<  8) |  \
                       (((val) & 0x00FF0000) >>  8) | (((val) & 0xFF000000) >> 24))
#  define ntohs(val)  ((((val) & 0xFF) << 8) | (((val) & 0xFF00) >> 8))
# endif
#endif

#define htonl(val)  ntohl(val)
#define htons(val)  ntohs(val)

#define NTOHL(x)    (x) = ntohl(x)
#define NTOHS(x)    (x) = ntohs(x)
#define HTONL(x)    (x) = htonl(x)
#define HTONS(x)    (x) = htons(x)

#if defined (__ICCARM__)              // IAR ARM
# pragma pack(push, 1)
#endif

/*
 * It's a BSD 'in_addr' & 's_addr', extra '_' was added for the Win32
 * names conflict resolving only
 */
struct in__addr
{
  u32_t s__addr;
}__pkt_struct;

#if defined (__ICCARM__)              // IAR ARM
# pragma pack(pop)
#endif


#endif /* _TN_NET_TYPES_H_ */
