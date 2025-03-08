#ifndef __I2C_H
#define __I2C_H 

#include "sys.h"
#include "stdlib.h"

#define GPIOX_DATA GPIOA // 自定义，看自己设置哪些IO为通信端口。
#define GPIOX_CLK GPIOA //
#define PIN_DATA GPIO_PIN_2 //
#define PIN_CLK GPIO_PIN_2 //


void I2C_Init_Outmode();
void I2C_Init_Inmode();
void I2C_Signal_Start();
void I2C_Signal_Stop();
void I2C_Byte_Send();
uint8_t I2C_Byte_Receive();
void I2C_ACK_Send();
void I2C_ACK_Wait();

#endif