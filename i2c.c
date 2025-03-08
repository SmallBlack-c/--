#include "i2c.h"

void I2C_Init_Outmode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_DATA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_DATA;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOX_DATA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOX_DATA,PIN_DATA);	
	RCC_APB2PeriphClockCmd(RCC_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_CLK;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOX_CLK, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOX_CLK,PIN_CLK);
}
void I2C_Init_Inmode()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = PIN_DATA;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOX_DATA, &GPIO_InitStruct);	
}
void I2C_Signal_Start()//在时钟高电平是，产生一个下降沿信号
{
	GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
	GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
}
void I2C_Signal_Stop()//在时钟高电平是，产生一个上升沿信号
{
	GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
	GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
}
void I2C_Byte_Send(uint8_t data)
{
	uint8_t data = data;
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
	for(uint8_t i=0;i<8,i++){
		if(data & 0x80){
			GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
		}else:{
			GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
		}
		GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
		GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
		data <<=1;
	}
}
uint8_t I2C_Byte_Receive(){
	uint8_t data = 0;
	for(uint8_t i=0;i<8,i++){
		GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
		if(GPIO_ReadInputDataBit(GPIOX_DATA,PIN_DATA)){
			data += 1;
		}
		GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
		data <<=1;
	}
	return data;
}
void I2C_ACK_Send()
{
	GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
	GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
}
uint8_t I2C_ACK_Wait()
{
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
	if(GPIO_ReadInputDataBit(GPIOX_DATA,PIN_DATA)){
		GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
		return 1;
	}
	GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
	return 0;

	
}