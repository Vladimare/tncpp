#ifndef _CORE17xx_H_
#define _CORE17xx_H_

#define VECTORS_COUNT       35        /* количество реальных векторов прерываний        */
#define VECTORS_COUNT_FAKE  1         /* количество векторов, привязанных на один номер */
                                      /* общее количество векторов прерываний           */
#define VECTORS_COUNT_TOTAL (VECTORS_COUNT_FAKE + VECTORS_COUNT)

#include "core.h"

#endif /*_CORE17xx_H_*/
