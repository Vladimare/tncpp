#ifndef _CMX469_587_H_
#define _CMX469_587_H_

#include "types.h"


extern void cmx469_init(void);
extern void cmx469_write(unsigned char* data, int size, CALLBACK0 readyh);

extern void cmx469_rxEnable(unsigned char enable);
extern void cmx469_txEnable(unsigned char enable);




#endif /*_CMX469_587_H_*/
