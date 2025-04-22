#include "sg90.h"


void SG90_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_S, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_S;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOX_S, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeStructure.TIM_Period=1999;
	TIM_TimeBaseInitTypeStructure.TIM_Prescaler=719;
	TIM_TimeBaseInitTypeStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitTypeStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitTypeStructure);
	
	TIM_OCInitTypeDef TIM_OCInitTypeStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	TIM_OCInitTypeStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitTypeStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitTypeStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM2,&TIM_OCInitTypeStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

}

void SG90_Signal_Angle(uint8_t angle){
	//0-180буг║50-250
	uint8_t value = 50+(angle*(10/9));
	TIM_SetCompare3(TIM2,value);
}