#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"

#define LONG_PRESS_START  30  
#define LONG_PRESS_SPEED  30   

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
