#include "dht11.h"

void Dht11_Init_Outmode(){
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_DA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_DA;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOX_DA, &GPIO_InitStructure);
}


void Dht11_Init_Inmode(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_DA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOX_DA, &GPIO_InitStructure);
}


void Dht11_Signal_Start(){


	GPIO_WriteBit(GPIOX_DA,PIN_DA,Bit_RESET);
	Delay_ms(20);
	GPIO_WriteBit(GPIOX_DA,PIN_DA,Bit_SET);
	Delay_us(13);

}

uint8_t Dht11_Signal_Wait(){
	uint8_t num = 0;
	while(!GPIO_ReadInputDataBit(GPIOX_DA,PIN_DA)){
		num++;
		Delay_us(1);
		if (num>100) return 0;
	}
	num = 0;
	while(GPIO_ReadInputDataBit(GPIOX_DA,PIN_DA)){
		num++;
		Delay_us(1);
		if (num>100) return 0;
	}
	return 1;
}

void Dht11_Data_Read(int* temp, int*hum){	//指针传参更安全，低耦合高内聚
	uint8_t data[5] = {0};
	char str[20];
	Dht11_Init_Outmode();
	Dht11_Signal_Start();
	Dht11_Init_Inmode();
	if(Dht11_Signal_Wait()) {
		for(int i=0; i<40; i++){
			int n=0;
			while(!GPIO_ReadInputDataBit(GPIOX_DA,PIN_DA)){
				Delay_us(1);
				n++;
				if (n>100){
					*temp = 0;
					*hum = 0;
					return ;
				}
			}
			uint8_t num = 0;
			while(GPIO_ReadInputDataBit(GPIOX_DA,PIN_DA)){
				num++;
				Delay_us(1);
				if (num>100){
					*temp = 0;
					*hum = 0;	
					return ;
				}
			}
			data[i/8] <<= 1;
			if(num>0 && num<20){
			}
			else if(num>20 && num<50){
				data[i/8] += 1;
			}
			else{
				*temp = 0;
				*hum = 0;
				return ;
			}		
		}
		if(data[0]+data[1]+data[2]+data[3] == data[4]){
			*temp = data[2];
			*hum = data[0];
		}
		else{
				*temp = 0;
				*hum = 0;
		}
	}
	else{
		*temp = 0;
		*hum = 0;
	}
	
}
