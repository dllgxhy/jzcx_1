#include "user_sys_config.h"



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Init_NVIC
** 功能描述: 系统中断配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年5月14日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//定义一个NVIC向量表结构体变量

	#ifdef  VECT_TAB_RAM  							//向量表基地址选择

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//将0x20000000地址作为向量表基地址(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //将0x08000000地址作为向量表基地址(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断组 为2 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//配置串口1为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//设置占先优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//设置副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//使能串口1中断
	NVIC_Init(&NVIC_InitStructure);							  	//根据参数初始化中断寄存器
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//设置中断组为0

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//中断事件为TIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 						
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //响应优先级1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //允许中断 
	NVIC_Init(&NVIC_InitStructure);                         //根据指定参数初始化中断寄存器
	
}
