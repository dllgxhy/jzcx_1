/**************************************************************
** 	火牛开发板
**	功能描述: STM32串口操作
**  版本：V1.0  
**	论坛：www.openmcu.com
**	淘宝：http://shop36995246.taobao.com/   
**  技术支持群：121939788 
***************************************************************/ 
#include "stm32f10x.h"
#include "USART.h"
#include <string.h>

/*****************************************************************************
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void USART1_Send_Byte(u16 Data)
{ 
   	while (!(USART1->SR & USART_FLAG_TXE));
   	USART1->DR = (Data & (uint16_t)0x01FF);	   
}
/*****************************************************************************
** 函数名称: USART_Send_Str
** 功能描述: 串口发送字符串
				s:指向字符串的指针 
					注：如果在字符串中有'\n'，则会发送一个回车换行
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void USART_Send_Str(const char* data)
{
	u16 i;
	u16 len = strlen(data)-1;
	for (i=0; i<len; i++)
	{
		USART1_Send_Byte(data[i]);
	}
	if(data[i]=='\n') 
	{
		USART_Send_Enter();
	}
	else
	{
		USART1_Send_Byte(data[i]);
	}		
}
/*****************************************************************************
** 函数名称: USART_Send_Enter
** 功能描述: 串口发送字符串
				串口发送0d 0a ，即回车换行
					注：此函数就是发送0d 0a这两个字节，在“超级终端”上会有回车换行的效果
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void USART_Send_Enter(void)
{
	USART1_Send_Byte(0x0d);
	USART1_Send_Byte(0x0a);
}
/*****************************************************************************
** 函数名称: USART_Put_Num
** 功能描述: 串口发送数值
				函数中会将数值转为相应的字符串，发送出去。比如 4567 转为 "4567" 
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/	
void USART_Put_Num(u32 dat)
{
 	char temp[20];
 	u32tostr(dat,temp);
 	USART_Send_Str(temp);
}
/*****************************************************************************
** 函数名称: USART_Put_Inf
** 功能描述: 串口发送调试信息
				inf:指向提示信息字符串的指针
             dat:一个数值，前面的提示信息就是在说明这个数值的意义 
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void USART_Put_Inf(char *inf,u32 dat)
{
 	USART_Send_Str(inf);
 	USART_Put_Num(dat);
 	USART_Send_Str("\n");  
}
/*****************************************************************************
** 函数名称: USART_Configuration
** 功能描述: 串口配置函数
				配置STM32串口1 波特率115200 
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate            = 115200  ;	  //波特率115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8位数据
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //停止位1位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);     
}
/*****************************************************************************
** 函数名称: USART1_IRQHandler
** 功能描述: 串口中断函数
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART1->DR;
		USART1_Send_Byte(c); 	    
	} 
	 
}
/*****************************************************************************
** 函数名称: u32tostr
** 功能描述: 将一个32位的变量dat转为字符串，比如把1234转为"1234"
				dat:带转的long型的变量
             str:指向字符数组的指针，转换后的字节串放在其中
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void u32tostr(u32 dat,char *str) 
{
 	char temp[20];
 	u8 i=0,j=0;
 	i=0;
 	while(dat)
 	{
  		temp[i]=dat%10+0x30;
  		i++;
  		dat/=10;
 	}
 	j=i;
 	for(i=0;i<j;i++)
 	{
  		str[i]=temp[j-i-1];
 	}
 	if(!i) {str[i++]='0';}
 	str[i]=0;
}
/*********************************************************************************************************
** End of File
*********************************************************************************************************/
