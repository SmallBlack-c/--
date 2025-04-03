#ifndef __DHT11_H
#define __DHT11_H

#include "sys.h"
#include "stdlib.h"
#include "Delay.h"

#define PIN_DA GPIO_Pin_2
#define GPIOX_DA GPIOB
#define RCC_DA RCC_APB2Periph_GPIOB

void Dht11_Init_Outmode(void);
void Dht11_Init_Inmode(void);
void Dht11_Signal_Start(void);
uint8_t Dht11_Signal_Wait(void);
void Dht11_Data_Read(int* temp, int*hum); //¸ü°²È«

#endif
