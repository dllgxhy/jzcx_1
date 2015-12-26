#include "led.h"
#include "usually.h"
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Init_LED
** 功能描述: LED IO引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD 
							| RCC_APB2Periph_GPIOE,ENABLE);	//使能各个端口时钟，重要！！！
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_12 
	| GPIO_Pin_13; 											//配置LED端口挂接到6、12、13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//根据参数初始化GPIOD寄存器
}

//超声波LED灯开启
//
//
void UltraSonic_LED_ON(uint8_t channel,uint16_t time)
{
}


//超声波LED灯开启
//
//
void UltraSonic_LED_OFF(uint8_t channel)
{
}
