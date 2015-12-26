/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
**  启航号开发板
**	串口底层函数驱动
**  版本：Rev1.0 
**  日期：2011-6-20
**	论坛：www.openmcu.com
**	淘宝：http://shop36995246.taobao.com/   
**  技术支持群：153631682     技术QQ：77642732 
**  Copyright(c)@旺宝电子	 Dream_etd@163.com	
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
//头文件调用
#include "usually.h"
#include "usart.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int Data, FILE *f)
{   
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
	   
	return Data;										  //返回一个值
}
#endif 

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 参数描述：Data 要发送的数据
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Send_Byte(uint16_t Data)
{ 
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 参数描述：Data 要发送的数据
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
uint8_t USART1_Receive_Byte(void)
{ 
   	while(!(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))); //USART_GetFlagStatus：得到接收状态位
														   //USART_FLAG_RXNE:接收数据寄存器非空标志位 
														   //1：忙状态  0：空闲(没收到数据，等待。。。)
	return USART_ReceiveData(USART1);					   //接收一个字符
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Usart_Init
** 功能描述: 串口引脚初始化
** 参数描述: 无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_Usart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);	
															//使能各个端口时钟，重要！！！

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//配置串口接收端口挂接到9端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//复用功能输出开漏
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	//根据参数初始化GPIOA寄存器	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入(复位状态);	   				
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	//根据参数初始化GPIOA寄存器	
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Usart_Configuration
** 功能描述: 串口配置函数
** 参数描述: BaudRate设置波特率 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Usart_Configuration(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;							    	//定义一个串口结构体

	USART_InitStructure.USART_BaudRate            =BaudRate ;	  			//波特率115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 	//传输过程中使用8位数据
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 	//在帧结尾传输1位停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 	//奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流失能
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式
	USART_Init(USART1, &USART_InitStructure);								//根据参数初始化串口寄存器
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);							//使能串口中断接收
	USART_Cmd(USART1, ENABLE);     											//使能串口外设
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_IRQHandler
** 功能描述: 串口中断函数
** 参数描述: 无 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_IRQHandler()
{
	uint8_t ReceiveDate;								//定义一个变量存放接收的数据
	if(!(USART_GetITStatus(USART1,USART_IT_RXNE))); 	//读取接收中断标志位USART_IT_RXNE 
														//USART_FLAG_RXNE:接收数据寄存器非空标志位 
														//1：忙状态  0：空闲(没收到数据，等待。。。)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//清楚中断标志位
		ReceiveDate=USART_ReceiveData(USART1);			//接收字符存入数组
		printf("%d",(char*)ReceiveDate);				//以十进制输出输入的值,从这里我们可以看到键盘上所有的键值对应的十进制数
														//比如输入"1"对应的输出49 、 输入"A" 对应的值为65
		printf("\n\r");									//换行置顶
		LED3=~LED3;										//输入一次LED3翻转一次
	}  
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
