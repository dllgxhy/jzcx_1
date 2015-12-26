#ifndef _TIMER_H_
#define _TIMER_H_

#include "stdio.h"
#include "string.h"
#include "stdint.h"

//定义函数原型
void Init_TIMER(void);
void TIM2_IRQHandler(void);
void Delay_Ms(uint16_t time);
void Delay_Us(uint16_t time);  //延时函数
void TIM_Delay(uint16_t time);


#endif
