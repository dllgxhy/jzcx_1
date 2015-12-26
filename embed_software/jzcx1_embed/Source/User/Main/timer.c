#include "usually.h"
#include "timer.h"
#include "usart.h"

uint16_t TIM_Count;	//定时器计数

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: TIMER_Init
** 功能描述: 定时器2初始化配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_TIMER(void)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//定义一个定时器结构体变量

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   	//使能定时器2，重要！！

	TIM_DeInit( TIM2);                              		//将IM2定时器初始化位复位值

	TIM_InternalClockConfig(TIM2); 							//配置 TIM2 内部时钟
	   
	TIM_BaseInitStructure.TIM_Period = 1000; 				//设置自动重载寄存器值为最大值	0~65535之间  1000000/1000=1000us=1ms													
															//TIM_Period（TIM1_ARR）=1000，计数器向上计数到1000后产生更新事件，
															//计数值归零 也就是 1MS产生更新事件一次
	TIM_BaseInitStructure.TIM_Prescaler = 71;  				//自定义预分频系数，提供给定时器的时钟	0~65535之间
															//设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz 
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
//	TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		//此参数只适合于TIM1和TIM8
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure); 		//根据指定参数初始化TIM时间基数寄存器
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); 					//清中断，以免一启用中断后立即产生中断 
      
    TIM_ARRPreloadConfig(TIM2, DISABLE);					//禁止ARR 预装载缓冲器

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 				//使能TIM2中断源 
	TIM_Cmd(TIM2, ENABLE); 									//TIM2总开关：开启 
}
/*****************************************************************************
** 函数名称: TIM2_IRQHandler
** 功能描述: 定时器2中断处理函数
				1ms的中断更新
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
*****************************************************************************/ 
void TIM2_IRQHandler(void)
{
	static uint16_t count=0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
    {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       	//清楚中断挂起位
		count++;
		TIM_Count--;										//定时延时变量减1
		if(count==500)										//计数到500MS，1MS*500
		{
			LED2 =~LED2;									//以0.5秒得周期闪烁
			count=0;	    								//计数清零
		}			
	}
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: TIM_Delay
** 功能描述: 利用定时器延时			
** 参数描述： 		延时时间T = time*1(MS) 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void TIM_Delay(uint16_t time)  //延时函数
{ 
	TIM_Count = time;
	while(TIM_Count);
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Ms
** 功能描述: 延时1MS (可通过仿真来判断他的准确度)			
** 参数描述：time (ms) 注意time<65535
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<10260;j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Us
** 功能描述: 延时1us (可通过仿真来判断他的准确度)
** 参数描述：time (us) 注意time<65535				 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<9;j++);
}
