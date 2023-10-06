#ifndef _GM3x0_587_H_
#define _GM3x0_587_H_

#include "types.h"


extern int            gm3x0_init       (void);
extern unsigned char  gm3x0_isReady    (void);
extern int            gm3x0_turnOn     (CALLBACK0 readyh);
extern int            gm3x0_turnOff    (CALLBACK0 readyh);
extern int            gm3x0_txInit     (CALLBACK0 readyh);
extern int            gm3x0_txFinalize (CALLBACK0 readyh);

#endif /*_GM3x0_587_H_*/
