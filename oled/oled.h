#ifndef __OLED_H
#define __OLED_H

#include "sys.h"
#include "stdlib.h"
#include "i2c.h"

#define ON 0xAF
#define OFF 0xAE


void OLED_Init(void);
void OLED_SendCmd(uint8_t cmd);
void OLED_SendData(uint8_t data);
void OLED_DrawPoint(uint8_t x, uint8_t y);



#endif
