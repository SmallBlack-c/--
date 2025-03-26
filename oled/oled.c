#include "oled.h"


void OLED_Init(void){
	OLED_SendCmd(0xAE);
	OLED_SendCmd(0x00);
	OLED_SendCmd(0x10);
	OLED_SendCmd(0x40);
	OLED_SendCmd(0x81);
	OLED_SendCmd(0xCF);
	OLED_SendCmd(0xA1);
	OLED_SendCmd(0xC8);
	OLED_SendCmd(0xA6);
	OLED_SendCmd(0xA8);
	OLED_SendCmd(0x3f);
	OLED_SendCmd(0xD3);
	OLED_SendCmd(0x00);
	OLED_SendCmd(0xD5);
	OLED_SendCmd(0x80);
	OLED_SendCmd(0xD9);
	OLED_SendCmd(0xF1);
	OLED_SendCmd(0xDA);
	OLED_SendCmd(0x12);
	OLED_SendCmd(0xDB);
	OLED_SendCmd(0x40);
	OLED_SendCmd(0x20);
	OLED_SendCmd(0x02);
	OLED_SendCmd(0x8D);
	OLED_SendCmd(0x14);
	OLED_SendCmd(0xA4);
	OLED_SendCmd(0xA6);
	OLED_SendCmd(0xAF);
}

void OLED_SendCmd(uint8_t cmd){
	I2C_Signal_Start();
	I2C_Byte_Send(0x78);
	I2C_ACK_Wait();
	I2C_Byte_Send(0x00);
	I2C_ACK_Wait();
	I2C_Byte_Send(cmd);
	I2C_ACK_Wait();
	I2C_Signal_Stop();
}

void OLED_SendData(uint8_t data){
	I2C_Signal_Start();
	I2C_Byte_Send(0x78);
	I2C_ACK_Wait();
	I2C_Byte_Send(0x40);
	I2C_ACK_Wait();
	I2C_Byte_Send(data);
	I2C_ACK_Wait();
	I2C_Signal_Stop();
}

void OLED_DrawPoint(uint8_t x, uint8_t y){
	uint8_t page = y/8;
	uint8_t row = y%8;
	uint8_t data = 0x01;
	for(int i=0; i<row; i++){
		data <<= 1;
	}
	OLED_SendCmd(0xB0+page);
	OLED_SendCmd(0x10+x/16);
	OLED_SendCmd(0x00+x%16);
	OLED_SendData(data);
}

