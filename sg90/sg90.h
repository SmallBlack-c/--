#ifndef __SG90_H
#define __SG90_H

#include "sys.h"
#include "stdlib.h"

#define PIN_S GPIO_Pin_2 ///signal port
#define GPIOX_S GPIOB
#define RCC_S RCC_APB2Periph_GPIOB

void SG90_Init(void);
void SG90_Signal_Angle(uint8_t angle);

#endif