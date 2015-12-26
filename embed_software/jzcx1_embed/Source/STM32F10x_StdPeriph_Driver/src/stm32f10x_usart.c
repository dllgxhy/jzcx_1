/**
  ******************************************************************************
  * @文件    stm32f10x_usart.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 USART 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    14/07/2009
  ******************************************************************************
  * @复件
  *
  * 这个固件仅仅是提供给客户作为设计产品而编写程序的参考目的使客户节约时间。由于
  * 客户使用本固件在开发产品编程上产生的结果意法半导体公司不承担任何直接的和间接
  * 的责任，也不承担任何损害而引起的赔偿。
  *
  * <h2><center>&复制; 版权所有 2009 意法半导体公司</center></h2>
  * 翻译版本仅供学习，如与英文原版有出入应以英文原版为准。
  */ 

/* 包含 ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

#define CR1_UE_Set                ((uint16_t)0x2000)  /*!< USART Enable Mask */
#define CR1_UE_Reset              ((uint16_t)0xDFFF)  /*!< USART Disable Mask */

#define CR1_WAKE_Mask             ((uint16_t)0xF7FF)  /*!< USART WakeUp Method Mask */

#define CR1_RWU_Set               ((uint16_t)0x0002)  /*!< USART mute mode Enable Mask */
#define CR1_RWU_Reset             ((uint16_t)0xFFFD)  /*!< USART mute mode Enable Mask */
#define CR1_SBK_Set               ((uint16_t)0x0001)  /*!< USART Break Character send Mask */
#define CR1_CLEAR_Mask            ((uint16_t)0xE9F3)  /*!< USART CR1 Mask */
#define CR2_Address_Mask          ((uint16_t)0xFFF0)  /*!< USART address Mask */

#define CR2_LINEN_Set              ((uint16_t)0x4000)  /*!< USART LIN Enable Mask */
#define CR2_LINEN_Reset            ((uint16_t)0xBFFF)  /*!< USART LIN Disable Mask */

#define CR2_LBDL_Mask             ((uint16_t)0xFFDF)  /*!< USART LIN Break detection Mask */
#define CR2_STOP_CLEAR_Mask       ((uint16_t)0xCFFF)  /*!< USART CR2 STOP Bits Mask */
#define CR2_CLOCK_CLEAR_Mask      ((uint16_t)0xF0FF)  /*!< USART CR2 Clock Mask */

#define CR3_SCEN_Set              ((uint16_t)0x0020)  /*!< USART SC Enable Mask */
#define CR3_SCEN_Reset            ((uint16_t)0xFFDF)  /*!< USART SC Disable Mask */

#define CR3_NACK_Set              ((uint16_t)0x0010)  /*!< USART SC NACK Enable Mask */
#define CR3_NACK_Reset            ((uint16_t)0xFFEF)  /*!< USART SC NACK Disable Mask */

#define CR3_HDSEL_Set             ((uint16_t)0x0008)  /*!< USART Half-Duplex Enable Mask */
#define CR3_HDSEL_Reset           ((uint16_t)0xFFF7)  /*!< USART Half-Duplex Disable Mask */

#define CR3_IRLP_Mask             ((uint16_t)0xFFFB)  /*!< USART IrDA LowPower mode Mask */
#define CR3_CLEAR_Mask            ((uint16_t)0xFCFF)  /*!< USART CR3 Mask */

#define CR3_IREN_Set              ((uint16_t)0x0002)  /*!< USART IrDA Enable Mask */
#define CR3_IREN_Reset            ((uint16_t)0xFFFD)  /*!< USART IrDA Disable Mask */
#define GTPR_LSB_Mask             ((uint16_t)0x00FF)  /*!< Guard Time Register LSB Mask */
#define GTPR_MSB_Mask             ((uint16_t)0xFF00)  /*!< Guard Time Register MSB Mask */
#define IT_Mask                   ((uint16_t)0x001F)  /*!< USART Interrupt Mask */

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将外设 USARTx 寄存器重设为缺省值.
  * @参数  USARTx: 选择 USART 外围设备. 
  *                这个参数可以取下列值之一: USART1, USART2, USART3, UART4 或 UART5.
  * @返回  没有
  */
void USART_DeInit(USART_TypeDef* USARTx)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));

  if (USARTx == USART1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
  }
  else if (USARTx == USART2)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
  }
  else if (USARTx == USART3)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
  }    
  else if (USARTx == UART4)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);
  }    
  else
  {
    if (USARTx == UART5)
    { 
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);
    }
  }
}

/**
  * @简述  根据 USART_InitStruct 中指定的参数初始化外设 USARTx 寄存器.
  * @参数  USARTx: 选择 USART 外围设备. 
  *                这个参数可以取下列值之一: USART1, USART2, USART3, UART4 或 UART5.
  * @参数  USART_InitStruct: 指向结构 USART_InitTypeDef 的指针，包含了外设 USART 的配置信息.
  * @返回  没有
  */
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  uint32_t tmpreg = 0x00, apbclock = 0x00;
  uint32_t integerdivider = 0x00;
  uint32_t fractionaldivider = 0x00;
  uint32_t usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
  /* The hardware flow control is available only for USART1, USART2 and USART3 */
  if (USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }

  usartxbase = (uint32_t)USARTx;

/*---------------------------- USART CR2 Configuration -----------------------*/
  tmpreg = USARTx->CR2;
  /* 清除 STOP[13:12] 位 */
  tmpreg &= CR2_STOP_CLEAR_Mask;
  /* 配置 USART 结束位, 时钟, 时钟极性, 时钟相位 和 末位 ------------*/
  /* 设置 STOP[13:12] 位，按照 USART_StopBits 的值 */
  tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;
  
  /* 写 USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;

/*---------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = USARTx->CR1;
  /* 清除 M, PCE, PS, TE 和 RE 位 */
  tmpreg &= CR1_CLEAR_Mask;
  /* 配置 USART 字长, 奇偶 和 模式 ----------------------- */
  /* 设置 M   位，按照 USART_WordLength 的值 */
  /* 设置 PCE 和 PS 位，按照 USART_Parity 的值 */
  /* 设置 TE  和 RE 位，按照 USART_Mode 的值 */
  tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;
  /* 写 USART CR1 */
  USARTx->CR1 = (uint16_t)tmpreg;

/*---------------------------- USART CR3 Configuration -----------------------*/  
  tmpreg = USARTx->CR3;
  /* 清除 CTSE 和 RTSE 位 */
  tmpreg &= CR3_CLEAR_Mask;
  /* 配置 USART HFC -------------------------------------------------*/
  /* 设置 CTSE 和 RTSE 位，按照 USART_HardwareFlowControl 的值 */
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;
  /* 写 USART CR3 */
  USARTx->CR3 = (uint16_t)tmpreg;

/*---------------------------- USART BRR Configuration -----------------------*/
  /* 配置 USART 波特率 -------------------------------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  /* 确定整数部分 */
  integerdivider = ((0x19 * apbclock) / (0x04 * (USART_InitStruct->USART_BaudRate)));
  tmpreg = (integerdivider / 0x64) << 0x04;
  /* 确定分数部分 */
  fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
  tmpreg |= ((((fractionaldivider * 0x10) + 0x32) / 0x64)) & ((uint8_t)0x0F);
  /* 写 USART BRR */
  USARTx->BRR = (uint16_t)tmpreg;
}

/**
  * @简述  把 USART_InitStruct 中的每一个参数按缺省值填入.
  * @参数  USART_InitStruct: 指向结构 USART_InitTypeDef 的指针，待初始化.
  * @返回  没有
  */
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
  /* USART_InitStruct 结构体的缺省值 */
  USART_InitStruct->USART_BaudRate = 9600;
  USART_InitStruct->USART_WordLength = USART_WordLength_8b;
  USART_InitStruct->USART_StopBits = USART_StopBits_1;
  USART_InitStruct->USART_Parity = USART_Parity_No ;
  USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
}

/**
  * @简述  初始化 USARTx 外围时钟，按照 USART_ClockInitStruct 内的参数.
  * @参数  USARTx: 这里x可以取 1, 2, 3 来选择 USART 外围设备.
  * @参数  USART_ClockInitStruct: 指向 USART_ClockInitTypeDef 的指针，包含了外设 USART 的配置信息.  
  * @注解  这个智能卡模式不能应用于 UART4 和 UART5.
  * @返回  没有
  */
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  uint32_t tmpreg = 0x00;
  /* 检查参数 */
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_USART_CLOCK(USART_ClockInitStruct->USART_Clock));
  assert_param(IS_USART_CPOL(USART_ClockInitStruct->USART_CPOL));
  assert_param(IS_USART_CPHA(USART_ClockInitStruct->USART_CPHA));
  assert_param(IS_USART_LASTBIT(USART_ClockInitStruct->USART_LastBit));
  
/*---------------------------- USART CR2 Configuration -----------------------*/
  tmpreg = USARTx->CR2;
  /* 清除 CLKEN, CPOL, CPHA 和 LBCL 位 */
  tmpreg &= CR2_CLOCK_CLEAR_Mask;
  /* 配置 USART Clock, CPOL, CPHA and LastBit ------------*/
  /* 设置 CLKEN 位，按照 USART_Clock 的值 */
  /* 设置 CPOL  位，按照 USART_CPOL 的值 */
  /* 设置 CPHA  位，按照 USART_CPHA 的值 */
  /* 设置 LBCL  位，按照 USART_LastBit 的值 */
  tmpreg |= (uint32_t)USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL | 
                 USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit;
  /* 写 USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;
}

/**
  * @简述  用默认值填充每一个 USART_ClockInitStruct 的成员.
  * @参数  USART_ClockInitStruct: 指向 USART_ClockInitTypeDef 结构的指针，用于初始化.
  * @返回  没有
  */
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  /* USART_ClockInitStruct 结构体的缺省值 */
  USART_ClockInitStruct->USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct->USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct->USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStruct->USART_LastBit = USART_LastBit_Disable;
}

/**
  * @简述  使能或者失能 USART 外设.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @参数  NewState: USARTx 外围设备的新状态.
  *                  这个参数可以是: ENABLE or DISABLE.
  * @返回  没有
  */
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected USART by setting the UE bit in the CR1 register */
    USARTx->CR1 |= CR1_UE_Set;
  }
  else
  {
    /* Disable the selected USART by clearing the UE bit in the CR1 register */
    USARTx->CR1 &= CR1_UE_Reset;
  }
}

/**
  * @简述  使能或者失能指定的 USART 中断.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_IT: 指定要使能或失能的 USART 中断源.
  *          这个参数可以是下面的值之一 :
  *          USART_IT_CTS:  CTS 改变中断 (不能应用于 UART4 和 UART5)
  *          USART_IT_LBD:  LIN 间隔侦测中断
  *          USART_IT_TXE:  传输数据寄存器空中断
  *          USART_IT_TC:   传输完成中断
  *          USART_IT_RXNE: 接收数据寄存器不为空中断
  *          USART_IT_IDLE: 空闲总线中断
  *          USART_IT_PE:   奇偶错误中断
  *          USART_IT_ERR:  错误中断(桢错误, 噪声错误, 溢出错误)
  * @参数  NewState: 指定的 USARTx 中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)
{
  uint32_t usartreg = 0x00, itpos = 0x00, itmask = 0x00;
  uint32_t usartxbase = 0x00;
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  /* CTS 中断不能应用于 UART4 和 UART5 */
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  usartxbase = (uint32_t)USARTx;

  /* 得到 USART 寄存器变址 */
  usartreg = (((uint8_t)USART_IT) >> 0x05);

  /* 得到中断位置 */
  itpos = USART_IT & IT_Mask;
  itmask = (((uint32_t)0x01) << itpos);
    
  if (usartreg == 0x01) /* The IT is in CR1 register */
  {
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02) /* The IT is in CR2 register */
  {
    usartxbase += 0x10;
  }
  else /* The IT is in CR3 register */
  {
    usartxbase += 0x14; 
  }
  if (NewState != DISABLE)
  {
    *(__IO uint32_t*)usartxbase  |= itmask;
  }
  else
  {
    *(__IO uint32_t*)usartxbase &= ~itmask;
  }
}

/**
  * @简述  使能或者失能指定 USART 的 DMA 请求.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3 or UART4.  
  * @参数  USART_DMAReq:指定 DMA 请求.
  *          这个参数可以是下面任意值的组合:
  *          USART_DMAReq_Tx: USART DMA 传输请求
  *          USART_DMAReq_Rx: USART DMA 接收请求
  * @参数  NewState: DMA 请求源的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @注解  DMA 模式不能应用于 UART5.  
  * @返回  没有
  */
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_1234_PERIPH(USARTx));
  assert_param(IS_USART_DMAREQ(USART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  if (NewState != DISABLE)
  {
    /* 通过设置USART CR3 寄存器的DMAT、DMAT位组合,来使能选择DMA传输请求 */
    USARTx->CR3 |= USART_DMAReq;
  }
  else
  {
    /* 通过设置USART CR3 寄存器的DMAT、DMAT位组合,来失能选择DMA传输请求 */
    USARTx->CR3 &= (uint16_t)~USART_DMAReq;
  }
}

/**
  * @简述  设置 USART 节点的地址.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_Address: 设置 USART 节点的地址.
  * @返回  没有
  */ 
void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_ADDRESS(USART_Address)); 
    
  /* 清除 USART 地址 */
  USARTx->CR2 &= CR2_Address_Mask;
  /* 设置 USART 节点的地址 */
  USARTx->CR2 |= USART_Address;
}

/**
  * @简述  选择 USART 的唤醒方式.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_WakeUp: 指定 USART 的唤醒方式.
  *          这个参数可以是下面的值之一 :
  *          USART_WakeUp_IdleLine:    空闲总线唤醒
  *          USART_WakeUp_AddressMark: 地址标记唤醒
  * @返回  没有
  */
void USART_WakeUpConfig(USART_TypeDef* USARTx, uint16_t USART_WakeUp)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_WAKEUP(USART_WakeUp));
  
  USARTx->CR1 &= CR1_WAKE_Mask;
  USARTx->CR1 |= USART_WakeUp;
}

/**
  * @简述  检查 USART 是否处于静默模式.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  NewState: USART 静默模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  if (NewState != DISABLE)
  {
    /* 通过置位CR1寄存器的RWU位使能USART静音模式 */
    USARTx->CR1 |= CR1_RWU_Set;
  }
  else
  {
    /* 通过清零CR1寄存器的RWU位失能USART静音模式 */
    USARTx->CR1 &= CR1_RWU_Reset;
  }
}

/**
  * @简述  设置 USART LIN 中断检测长度.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @参数  USART_LINBreakDetectLength: LIN 中断检测长度.
  *          这个参数可以是下面的值之一 :
  *          USART_LINBreakDetectLength_10b: 10位间断检测
  *          USART_LINBreakDetectLength_11b: 11位间断检测
  * @返回  没有
  */
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_LIN_BREAK_DETECT_LENGTH(USART_LINBreakDetectLength));
  
  USARTx->CR2 &= CR2_LBDL_Mask;
  USARTx->CR2 |= USART_LINBreakDetectLength;  
}

/**
  * @简述  使能或者失能 USARTx 的 LIN 模式.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  NewState: USART LIN 模式的新状态.
  *                  这个参数可以是: ENABLE or DISABLE.
  * @返回  没有
  */
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 通过置位CR2寄存器的LINEN位使能LIN模式 */
    USARTx->CR2 |= CR2_LINEN_Set;
  }
  else
  {
    /* 通过清零CR2寄存器的LINEN位失能LIN模式 */
    USARTx->CR2 &= CR2_LINEN_Reset;
  }
}

/**
  * @简述  通过外设 USARTx 发送单个数据.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备.
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @参数  Data: 要发送的数据.
  * @返回  没有
  */
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* 发送数据 */
  USARTx->DR = (Data & (uint16_t)0x01FF);
}

/**
  * @简述  返回 USARTx 最近接收到的数据.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备.
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @返回  接收数据.
  */
uint16_t USART_ReceiveData(USART_TypeDef* USARTx)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* 接收数据 */
  return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}

/**
  * @简述  发送中断字.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @返回  没有
  */
void USART_SendBreak(USART_TypeDef* USARTx)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* 发送中断字 */
  USARTx->CR1 |= CR1_SBK_Set;
}

/**
  * @简述  设置指定的 USART 保护时间.
  * @参数  USARTx: x可以为1,2或3以选择USART的外围设备.
  * @参数  USART_GuardTime: 指定的保护时间.
  * @注解  这个保护时间位不能应用于 UART4 和 UART5.   
  * @返回  没有
  */
void USART_SetGuardTime(USART_TypeDef* USARTx, uint8_t USART_GuardTime)
{    
  /* 检查参数 */
  assert_param(IS_USART_123_PERIPH(USARTx));
  
  /* 清除 USART 保护时间 */
  USARTx->GTPR &= GTPR_LSB_Mask;
  /* 设置 USART 保护时间 */
  USARTx->GTPR |= (uint16_t)((uint16_t)USART_GuardTime << 0x08);
}

/**
  * @简述  设置 USART 时钟预分频.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_Prescaler: 指定预分频器.  
  * @注解  这个函数是用于 UART4 和 UART5 的 IrDA 模式.
  * @返回  没有
  */
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler)
{ 
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* 清除 USART 预分频器 */
  USARTx->GTPR &= GTPR_MSB_Mask;
  /* 设置 USART 预分频器 */
  USARTx->GTPR |= USART_Prescaler;
}

/**
  * @简述  使能或者失能指定 USART 的智能卡模式.
  * @参数  USARTx: x可以为1,2或3以选择USART的外围设备.
  * @参数  NewState: USART 智能卡模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.     
  * @注解  这个智能卡模式不能应用于 UART4 和 UART5 . 
  * @返回  没有
  */
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 通过置位CR3寄存器的SCEN位使能SC模式 */
    USARTx->CR3 |= CR3_SCEN_Set;
  }
  else
  {
    /* 通过清零CR3寄存器的SCEN位失能SC模式 */
    USARTx->CR3 &= CR3_SCEN_Reset;
  }
}

/**
  * @简述  使能或者失能 NACK 传输.
  * @参数  USARTx: x可以为1,2或3以选择USART的外围设备. 
  * @参数  NewState: NACK 传输的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.  
  * @注解  这个智能卡模式不能应用于 UART4 和 UART5 .
  * @返回  没有
  */
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_123_PERIPH(USARTx));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 通过置位CR3寄存器的NACK位使能NACK传输 */
    USARTx->CR3 |= CR3_NACK_Set;
  }
  else
  {
    /* 通过清零CR3寄存器的NACK位失能NACK传输 */
    USARTx->CR3 &= CR3_NACK_Reset;
  }
}

/**
  * @简述  使能或者失能 USART 半双工模式.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  NewState: USART 半双工模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 通过置位CR3寄存器的HDSEL位使能Half-Duplex模式 */
    USARTx->CR3 |= CR3_HDSEL_Set;
  }
  else
  {
    /* 通过清零CR3寄存器的HDSEL位失能Half-Duplex模式 */
    USARTx->CR3 &= CR3_HDSEL_Reset;
  }
}

/**
  * @简述  设置 USART IrDA 模式.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @参数  USART_IrDAMode: 指定 IrDA 模式.
  *                        这个参数可以是下面的值之一 :
  *                        USART_IrDAMode_LowPower: 低功耗模式
  *                        USART_IrDAMode_Normal  : 正常模式
  * @返回  没有
  */
void USART_IrDAConfig(USART_TypeDef* USARTx, uint16_t USART_IrDAMode)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_IRDA_MODE(USART_IrDAMode));
    
  USARTx->CR3 &= CR3_IRLP_Mask;
  USARTx->CR3 |= USART_IrDAMode;
}

/**
  * @简述  使能或者失能 USART IrDA 模式.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备.
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 或 UART5.
  * @参数  NewState: IrDA 模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
    
  if (NewState != DISABLE)
  {
    /* 通过置位CR3寄存器的IREN位使能IrDA模式 */
    USARTx->CR3 |= CR3_IREN_Set;
  }
  else
  {
    /* 通过清零CR3寄存器的IREN位失能IrDA模式 */
    USARTx->CR3 &= CR3_IREN_Reset;
  }
}

/**
  * @简述  检查指定的 USART 标志位设置与否.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_FLAG: 指定要检测的标记.
  *          这个参数可以是下面的值之一 :
  *          USART_FLAG_CTS:  CTS 改变标志 (不能应用于 UART4 和 UART5)
  *          USART_FLAG_LBD:  LIN 间隔侦测标志
  *          USART_FLAG_TXE:  发送数据寄存器为空标志
  *          USART_FLAG_TC:   传输完成标志
  *          USART_FLAG_RXNE: 接收数据寄存器不为空标志
  *          USART_FLAG_IDLE: 空闲总线标志
  *          USART_FLAG_ORE:  溢出错误标志
  *          USART_FLAG_NE:   噪声错误标志
  *          USART_FLAG_FE:   帧错误标志
  *          USART_FLAG_PE:   奇偶错误标志
  * @retval  USART_FLAG 的新状态(SET 或 RESET).
  */
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_FLAG(USART_FLAG));
  /* CTS 标志不能应用于 UART4 和 UART5 */
  if (USART_FLAG == USART_FLAG_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }  
  
  if ((USARTx->SR & USART_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @简述  清除 USARTx 的待处理标志位.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备.
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_FLAG: 指定要被清除的标记.
  *          这个参数可以是下面的值的任意组合:
  *          USART_FLAG_CTS:  CTS 改变标志 (不能应用于 UART4 和 UART5).
  *          USART_FLAG_LBD:  LIN 间隔侦测标志.
  *          USART_FLAG_TC:   传输完成标志.
  *          USART_FLAG_RXNE: 接收数据寄存器不为空标志.
  *   
  * @注解
*          - PE (奇偶校验错误), FE (桢错误), NE (噪声错误),
*            ORE (过速错误) 和 IDLE (找到空闲线) 标志标志被软件按顺序清除:
*            一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*            接着一个读USART_DR寄存器操作(USART_ReceiveData()).
*          - RXNE标志也可以被一个读USART_DR寄存器操作清除(USART_ReceiveData()).
*          - TC标志标志被软件按顺序清除: 一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*            接着一个读USART_DR寄存器操作(USART_ReceiveData()).                                                      
*          - TXE标志只能靠写USART_DR寄存器清除(USART_SendData()).     
  * @返回  没有
  */
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_FLAG(USART_FLAG));
  /* CTS 标志不能应用于 UART4 和 UART5 */
  if ((USART_FLAG & USART_FLAG_CTS) == USART_FLAG_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  } 
   
  USARTx->SR = (uint16_t)~USART_FLAG;
}

/**
  * @简述  检查指定的 USART 中断发生与否.
  * @参数  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_IT: 指定要检测的USART中断源.
  *          这个参数可以是下面的值之一 :
  *          USART_IT_CTS:  CTS 改变中断 (不能应用于 UART4 和 UART5)
  *          USART_IT_LBD:  LIN 间隔侦测中断
  *          USART_IT_TXE:  发送数据寄存器为空中断
  *          USART_IT_TC:   传输完成中断
  *          USART_IT_RXNE: 接收数据寄存器不为空中断
  *          USART_IT_IDLE: 空闲总线中断
  *          USART_IT_ORE:  溢出错误中断
  *          USART_IT_NE:   噪声错误中断
  *          USART_IT_FE:   帧错误中断
  *          USART_IT_PE:   奇偶错误中断
  * @返回  USART_IT 的新状态 (SET 或 RESET).
  */
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint32_t bitpos = 0x00, itmask = 0x00, usartreg = 0x00;
  ITStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_GET_IT(USART_IT));
  /* CTS 中断不能应用于 UART4 和 UART5 */ 
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  /* 得到 USART 寄存器引索 */
  usartreg = (((uint8_t)USART_IT) >> 0x05);
  /*得到中断位置 */
  itmask = USART_IT & IT_Mask;
  itmask = (uint32_t)0x01 << itmask;
  
  if (usartreg == 0x01) /* The IT  is in CR1 register */
  {
    itmask &= USARTx->CR1;
  }
  else if (usartreg == 0x02) /* The IT  is in CR2 register */
  {
    itmask &= USARTx->CR2;
  }
  else /* The IT  is in CR3 register */
  {
    itmask &= USARTx->CR3;
  }
  
  bitpos = USART_IT >> 0x08;
  bitpos = (uint32_t)0x01 << bitpos;
  bitpos &= USARTx->SR;
  if ((itmask != (uint16_t)RESET)&&(bitpos != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  
  return bitstatus;  
}

/**
  * @简述  清除 USARTx 的中断待处理位.
  * @返回  USARTx: 选择 USART 或者 UART 的外围设备. 
  *                这个参数可以是下面的值之一 : USART1, USART2, USART3, UART4 or UART5.
  * @参数  USART_IT: 指定要被清除的中断挂起位.
  *          这个参数可以是下面的值之一:
  *          USART_IT_CTS:  CTS 改变中断 (不能应用于 UART4 和 UART5)
  *          USART_IT_LBD:  LIN 间隔侦测中断
  *          USART_IT_TC:   发送完成中断
  *          USART_IT_RXNE: 接收数据寄存器不为空中断
  *   
  * @注解
*          - PE (奇偶校验错误), FE (桢错误), NE (噪声错误),
*            ORE (过速错误) 和 IDLE (找到空闲线)挂起位 
*            被软件按顺序清除: 一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*            接着一个读USART_DR寄存器操作(USART_ReceiveData()).
*          - RXNE挂起位也可以被一个读USART_DR寄存器操作清除(USART_ReceiveData()).
*          - TC挂起位也可以被软件按顺序清除: 一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*            接着一个读USART_DR寄存器操作(USART_ReceiveData()).                                                      
*          - TXE挂起位置可以被一个写USART_DR寄存器清除(USART_SendData()).  
  * @返回  没有
  */
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint16_t bitpos = 0x00, itmask = 0x00;
  /* 检查参数 */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_IT(USART_IT));
  /* CTS 中断不能应用于 UART4 和 UART5 */
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  bitpos = USART_IT >> 0x08;
  itmask = ((uint16_t)0x01 << (uint16_t)bitpos);
  USARTx->SR = (uint16_t)~itmask;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
