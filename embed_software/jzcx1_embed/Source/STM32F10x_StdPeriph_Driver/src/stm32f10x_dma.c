/**
  ******************************************************************************
  * @文件    stm32f10x_dma.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 DMA 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    18/07/2009
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
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* DMA ENABLE mask */
#define CCR_ENABLE_Set          ((uint32_t)0x00000001)
#define CCR_ENABLE_Reset        ((uint32_t)0xFFFFFFFE)

/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_Channel1_IT_Mask    ((uint32_t)0x0000000F)
#define DMA1_Channel2_IT_Mask    ((uint32_t)0x000000F0)
#define DMA1_Channel3_IT_Mask    ((uint32_t)0x00000F00)
#define DMA1_Channel4_IT_Mask    ((uint32_t)0x0000F000)
#define DMA1_Channel5_IT_Mask    ((uint32_t)0x000F0000)
#define DMA1_Channel6_IT_Mask    ((uint32_t)0x00F00000)
#define DMA1_Channel7_IT_Mask    ((uint32_t)0x0F000000)

/* DMA2 Channelx interrupt pending bit masks */
#define DMA2_Channel1_IT_Mask    ((uint32_t)0x0000000F)
#define DMA2_Channel2_IT_Mask    ((uint32_t)0x000000F0)
#define DMA2_Channel3_IT_Mask    ((uint32_t)0x00000F00)
#define DMA2_Channel4_IT_Mask    ((uint32_t)0x0000F000)
#define DMA2_Channel5_IT_Mask    ((uint32_t)0x000F0000)

/* DMA2 FLAG mask */
#define FLAG_Mask                ((uint32_t)0x10000000)

/* DMA registers Masks */
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将 DM A的通道x 寄存器重设为缺省值.
  * @参数  DMAy_Channelx: y 可以是1 或者2，DMA1的x可以是1 到7 和 DMA2 的x 可以是1 到5 来选择各通道.
  * @返回  没有
  */
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* 失能选定的 DMA y 通道 x */
  DMAy_Channelx->CCR &= CCR_ENABLE_Reset;
  /* 复位 DMA y 通道 x 控制寄存器 */
  DMAy_Channelx->CCR  = 0;
  
  /* 复位 DMA y 通道 x 剩余字节寄存器 */
  DMAy_Channelx->CNDTR = 0;
  
  /* 复位 DMA y 通道x 外围设备地址寄存器 */
  DMAy_Channelx->CPAR  = 0;
  
  /* 复位 DMA y 通道 x 内存地址寄存器 */
  DMAy_Channelx->CMAR = 0;
  
  if (DMAy_Channelx == DMA1_Channel1)
  {
    /* 复位 DMA1 通道1 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel2)
  {
    /* 复位 DMA1 通道2 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel3)
  {
    /* 复位 DMA1 通道3 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel4)
  {
    /* 复位 DMA1 通道4 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel4_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel5)
  {
    /* 复位 DMA1 通道5 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel5_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel6)
  {
    /* 复位 DMA1 通道6 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel6_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel7)
  {
    /* 复位 DMA1 通道7 中断挂起位 */
    DMA1->IFCR |= DMA1_Channel7_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel1)
  {
    /* 复位 DMA2 通道1 中断挂起位 */
    DMA2->IFCR |= DMA2_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel2)
  {
    /* 复位 DMA2 通道2 中断挂起位 */
    DMA2->IFCR |= DMA2_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel3)
  {
    /* 复位 DMA2 通道3 中断挂起位 */
    DMA2->IFCR |= DMA2_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel4)
  {
    /* 复位 DMA2 通道4 中断挂起位 */
    DMA2->IFCR |= DMA2_Channel4_IT_Mask;
  }
  else
  { 
    if (DMAy_Channelx == DMA2_Channel5)
    {
      /* 复位 DMA2 通道5 中断挂起位 */
      DMA2->IFCR |= DMA2_Channel5_IT_Mask;
    }
  }
}

/**
  * @简述  根据 DMA_InitStruct 中指定的参数初始化 DMA 的通道x 寄存器.
  * @参数  DMAy_Channelx: y 可以是1 或者2 ，DMA1 的x 可以是 1到 7 和DMA2 的x 可以是1 到5 来选择各通道.
  * @参数  DMA_InitStruct: 指向 DMA_InitTypeDef 的结构体，它包含特定 DMA 通道的配置信息.
  * @返回  没有
  */
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
  assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));   
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
  assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
  assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
  assert_param(IS_DMA_M2M_STATE(DMA_InitStruct->DMA_M2M));

/*--------------------------- DMAy Channelx CCR Configuration -----------------*/
  /* Get the DMAy_Channelx CCR value */
  tmpreg = DMAy_Channelx->CCR;
  /* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
  tmpreg &= CCR_CLEAR_Mask;
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode */
  /* Set DIR bit according to DMA_DIR value */
  /* Set CIRC bit according to DMA_Mode value */
  /* Set PINC bit according to DMA_PeripheralInc value */
  /* Set MINC bit according to DMA_MemoryInc value */
  /* Set PSIZE bits according to DMA_PeripheralDataSize value */
  /* Set MSIZE bits according to DMA_MemoryDataSize value */
  /* Set PL bits according to DMA_Priority value */
  /* Set the MEM2MEM bit according to DMA_M2M value */
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;

  /* Write to DMAy Channelx CCR */
  DMAy_Channelx->CCR = tmpreg;

/*--------------------------- DMAy Channelx CNDTR Configuration ---------------*/
  /* Write to DMAy Channelx CNDTR */
  DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;

/*--------------------------- DMAy Channelx CPAR Configuration ----------------*/
  /* Write to DMAy Channelx CPAR */
  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

/*--------------------------- DMAy Channelx CMAR Configuration ----------------*/
  /* Write to DMAy Channelx CMAR */
  DMAy_Channelx->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct : pointer to a DMA_InitTypeDef structure which will
  *   be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  /* 初始化 DMA_PeripheralBaseAddr 成员 */
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
  /* 初始化 DMA_MemoryBaseAddr 成员 */
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;
  /* 初始化 DMA_DIR 成员 */
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
  /* 初始化 DMA_BufferSize 成员 */
  DMA_InitStruct->DMA_BufferSize = 0;
  /* 初始化 DMA_PeripheralInc 成员 */
  DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* 初始化 DMA_MemoryInc 成员 */
  DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
  /* 初始化 DMA_PeripheralDataSize 成员 */
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* 初始化 DMA_MemoryDataSize 成员 */
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* 初始化 DMA_Mode 成员 */
  DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
  /* 初始化 DMA_Priority 成员 */
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
  /* 初始化 DMA_M2M 成员 */
  DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/**
  * @简述  使能或失能指定的 DMA y 通道 x.
  * @参数  DMAy_Channelx: y 可以是1 或者2 ，DMA1 的x 可以是1 到7 和DMA2 的x 可以是1 到5 来选择各通道.
  * @参数  NewState: DMA y 通道x 的新状态. 
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* 使能指定的 DMA y 通道 x */
    DMAy_Channelx->CCR |= CCR_ENABLE_Set;
  }
  else
  {
    /* 失能指定的 DMA y 通道 x */
    DMAy_Channelx->CCR &= CCR_ENABLE_Reset;
  }
}

/**
  * @简述  使能或失能指定的 DMA y 通道 x 中断.
  * @参数  DMAy_Channelx: y 可以是1 或者2 ，DMA1 的x 可以是1 到7 和DMA2 的x 可以是1 到5 来选择各通道.
  * @参数  DMA_IT: 指定 DMA 的中断. 
  *            这个参数可以是下面值的任意组合:
  *            DMA_IT_TC:  传输完成中断屏蔽
  *            DMA_IT_HT:  传输过半中断屏蔽
  *            DMA_IT_TE:  传输错误中断屏蔽
  * @参数  NewState: 指定的 DMA 中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能指定的 DMA 中断 */
    DMAy_Channelx->CCR |= DMA_IT;
  }
  else
  {
    /* 失能指定的 DMA 中断 */
    DMAy_Channelx->CCR &= ~DMA_IT;
  }
}

/**
  * @简述  返回当前 DMA y 通道x 剩余的待传输数据数目.
  * @参数  DMAy_Channelx: y 可以是1 或者2 ，DMA1 的x 可以是1 到7 和DMA2 的x 可以是1 到5 来选择各通道.
  * @返回  当前 DMA y 通道x 传输中剩余的数据单元的数量.
  */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* 返回 DMA y 通道x 传输中剩余的数据单元的数量 */
  return ((uint16_t)(DMAy_Channelx->CNDTR));
}

/**
  * @简述  检查指定的 DMA y 通道 x 标志位设置与否.
  * @参数  DMA_FLAG: 指定的标志.
  *          这个参数可以是下面的值之一:
*            DMA1_FLAG_GL1: DMA1通道1全局标志
*            DMA1_FLAG_TC1: DMA1通道1传输结束标志
*            DMA1_FLAG_HT1: DMA1通道1半传输标志
*            DMA1_FLAG_TE1: DMA1通道1传输错误标志
*            DMA1_FLAG_GL2: DMA1 通道2 全局标志.
*            DMA1_FLAG_TC2: DMA1 通道2 传输结束标志.
*            DMA1_FLAG_HT2: DMA1 通道2 半传输标志.
*            DMA1_FLAG_TE2: DMA1 通道2 传输错误标志.
*            DMA1_FLAG_GL3: DMA1 通道3 全局标志.
*            DMA1_FLAG_TC3: DMA1 通道3 传输结束标志.
*            DMA1_FLAG_HT3: DMA1 通道3 半传输标志.
*            DMA1_FLAG_TE3: DMA1 通道3 传输错误标志.
*            DMA1_FLAG_GL4: DMA1 通道4 全局标志.
*            DMA1_FLAG_TC4: DMA1 通道4 传输结束标志.
*            DMA1_FLAG_HT4: DMA1 通道4 半传输标志.
*            DMA1_FLAG_TE4: DMA1 通道4 传输错误标志.
*            DMA1_FLAG_GL5: DMA1 通道5 全局标志.
*            DMA1_FLAG_TC5: DMA1 通道5 传输结束标志.
*            DMA1_FLAG_HT5: DMA1 通道5 半传输标志.
*            DMA1_FLAG_TE5: DMA1 通道5 传输错误标志.
*            DMA1_FLAG_GL6: DMA1 通道6 全局标志.
*            DMA1_FLAG_TC6: DMA1 通道6 传输结束标志.
*            DMA1_FLAG_HT6: DMA1 通道6 半传输标志.
*            DMA1_FLAG_TE6: DMA1 通道6 传输错误标志.
*            DMA1_FLAG_GL7: DMA1 通道7 全局标志.
*            DMA1_FLAG_TC7: DMA1 通道7 传输结束标志.
*            DMA1_FLAG_HT7: DMA1 通道7 半传输标志.
*            DMA1_FLAG_TE7: DMA1 通道7 传输错误标志.
*            DMA2_FLAG_GL1: DMA2 通道1 全局标志.
*            DMA2_FLAG_TC1: DMA2 通道1 传输结束标志.
*            DMA2_FLAG_HT1: DMA2 通道1 半传输标志.
*            DMA2_FLAG_TE1: DMA2 通道1 传输错误标志.
*            DMA2_FLAG_GL2: DMA2 通道2 全局标志.
*            DMA2_FLAG_TC2: DMA2 通道2 传输结束标志.
*            DMA2_FLAG_HT2: DMA2 通道2 半传输标志.
*            DMA2_FLAG_TE2: DMA2 通道2 传输错误标志.
*            DMA2_FLAG_GL3: DMA2 通道3 全局标志.
*            DMA2_FLAG_TC3: DMA2 通道3 传输结束标志.
*            DMA2_FLAG_HT3: DMA2 通道3 半传输标志.
*            DMA2_FLAG_TE3: DMA2 通道3 传输错误标志.
*            DMA2_FLAG_GL4: DMA2 通道4 全局标志.
*            DMA2_FLAG_TC4: DMA2 通道4 传输结束标志.
*            DMA2_FLAG_HT4: DMA2 通道4 半传输标志.
*            DMA2_FLAG_TE4: DMA2 通道4 传输错误标志.
*            DMA2_FLAG_GL5: DMA2 通道5 全局标志.
*            DMA2_FLAG_TC5: DMA2 通道5 传输结束标志.
*            DMA2_FLAG_HT5: DMA2 通道5 半传输标志.
*            DMA2_FLAG_TE5: DMA2 通道5 传输错误标志.
  * @返回  DMA_FLAG 的新状态 (SET 或 RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_DMA_GET_FLAG(DMA_FLAG));

  /* 计算使用的 DMA */
  if ((DMA_FLAG & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 得到 DMA2 ISR 寄存器的值 */
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* 得到 DMA1 ISR 寄存器的值 */
    tmpreg = DMA1->ISR ;
  }

  /* 检查指定的 DMA 标志的新状态 */
  if ((tmpreg & DMA_FLAG) != (uint32_t)RESET)
  {
    /* 设置 DMA_FLAG */
    bitstatus = SET;
  }
  else
  {
    /* 复位 DMA_FLAG */
    bitstatus = RESET;
  }
  
  /* 返回 DMA_FLAG 状态 */
  return  bitstatus;
}

/**
  * @简述  清除定的 DMA y 通道 x 待处理标志位.
  * @参数  DMA_FLAG: 指定的待处理标志位.
  *          这个参数可以是下面值的任意组合 (对于同一个 DMA) :
*            DMA1_FLAG_GL1: DMA1通道1全局标志
*            DMA1_FLAG_TC1: DMA1通道1传输结束标志
*            DMA1_FLAG_HT1: DMA1通道1半传输标志
*            DMA1_FLAG_TE1: DMA1通道1传输错误标志
*            DMA1_FLAG_GL2: DMA1 通道2 全局标志.
*            DMA1_FLAG_TC2: DMA1 通道2 传输结束标志.
*            DMA1_FLAG_HT2: DMA1 通道2 半传输标志.
*            DMA1_FLAG_TE2: DMA1 通道2 传输错误标志.
*            DMA1_FLAG_GL3: DMA1 通道3 全局标志.
*            DMA1_FLAG_TC3: DMA1 通道3 传输结束标志.
*            DMA1_FLAG_HT3: DMA1 通道3 半传输标志.
*            DMA1_FLAG_TE3: DMA1 通道3 传输错误标志.
*            DMA1_FLAG_GL4: DMA1 通道4 全局标志.
*            DMA1_FLAG_TC4: DMA1 通道4 传输结束标志.
*            DMA1_FLAG_HT4: DMA1 通道4 半传输标志.
*            DMA1_FLAG_TE4: DMA1 通道4 传输错误标志.
*            DMA1_FLAG_GL5: DMA1 通道5 全局标志.
*            DMA1_FLAG_TC5: DMA1 通道5 传输结束标志.
*            DMA1_FLAG_HT5: DMA1 通道5 半传输标志.
*            DMA1_FLAG_TE5: DMA1 通道5 传输错误标志.
*            DMA1_FLAG_GL6: DMA1 通道6 全局标志.
*            DMA1_FLAG_TC6: DMA1 通道6 传输结束标志.
*            DMA1_FLAG_HT6: DMA1 通道6 半传输标志.
*            DMA1_FLAG_TE6: DMA1 通道6 传输错误标志.
*            DMA1_FLAG_GL7: DMA1 通道7 全局标志.
*            DMA1_FLAG_TC7: DMA1 通道7 传输结束标志.
*            DMA1_FLAG_HT7: DMA1 通道7 半传输标志.
*            DMA1_FLAG_TE7: DMA1 通道7 传输错误标志.
*            DMA2_FLAG_GL1: DMA2 通道1 全局标志.
*            DMA2_FLAG_TC1: DMA2 通道1 传输结束标志.
*            DMA2_FLAG_HT1: DMA2 通道1 半传输标志.
*            DMA2_FLAG_TE1: DMA2 通道1 传输错误标志.
*            DMA2_FLAG_GL2: DMA2 通道2 全局标志.
*            DMA2_FLAG_TC2: DMA2 通道2 传输结束标志.
*            DMA2_FLAG_HT2: DMA2 通道2 半传输标志.
*            DMA2_FLAG_TE2: DMA2 通道2 传输错误标志.
*            DMA2_FLAG_GL3: DMA2 通道3 全局标志.
*            DMA2_FLAG_TC3: DMA2 通道3 传输结束标志.
*            DMA2_FLAG_HT3: DMA2 通道3 半传输标志.
*            DMA2_FLAG_TE3: DMA2 通道3 传输错误标志.
*            DMA2_FLAG_GL4: DMA2 通道4 全局标志.
*            DMA2_FLAG_TC4: DMA2 通道4 传输结束标志.
*            DMA2_FLAG_HT4: DMA2 通道4 半传输标志.
*            DMA2_FLAG_TE4: DMA2 通道4 传输错误标志.
*            DMA2_FLAG_GL5: DMA2 通道5 全局标志.
*            DMA2_FLAG_TC5: DMA2 通道5 传输结束标志.
*            DMA2_FLAG_HT5: DMA2 通道5 半传输标志.
*            DMA2_FLAG_TE5: DMA2 通道5 传输错误标志.
  * @返回  没有
  */
void DMA_ClearFlag(uint32_t DMA_FLAG)
{
  /* 检查参数 */
  assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));
  /* 计算使用的 DMA */

  if ((DMA_FLAG & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 清除选定的 DMA 标志 */
    DMA2->IFCR = DMA_FLAG;
  }
  else
  {
    /* 清除选定的 DMA 标志 */
    DMA1->IFCR = DMA_FLAG;
  }
}

/**
  * @简述  检查指定的 DMA y 通道 x 中断发生与否.
  * @参数  DMA_IT: 指定的 DMA 中断源. 
*             这个参数可以是下面的值之一:
*             DMA1_IT_GL1: DMA1 通道1 全局中断.
*             DMA1_IT_TC1: DMA1 通道1 传输完成中断.
*             DMA1_IT_HT1: DMA1 通道1 半传输中断.
*             DMA1_IT_TE1: DMA1 通道1 传输错误中断.
*             DMA1_IT_GL2: DMA1 通道2 全局中断.
*             DMA1_IT_TC2: DMA1 通道2 传输完成中断.
*             DMA1_IT_HT2: DMA1 通道2 半传输中断.
*             DMA1_IT_TE2: DMA1 通道2 传输错误中断.
*             DMA1_IT_GL3: DMA1 通道3 全局中断.
*             DMA1_IT_TC3: DMA1 通道3 传输完成中断.
*             DMA1_IT_HT3: DMA1 通道3 半传输中断.
*             DMA1_IT_TE3: DMA1 通道3 传输错误中断.
*             DMA1_IT_GL4: DMA1 通道4 全局中断.
*             DMA1_IT_TC4: DMA1 通道4 传输完成中断.
*             DMA1_IT_HT4: DMA1 通道4 半传输中断.
*             DMA1_IT_TE4: DMA1 通道4 传输错误中断.
*             DMA1_IT_GL5: DMA1 通道5 全局中断.
*             DMA1_IT_TC5: DMA1 通道5 传输完成中断.
*             DMA1_IT_HT5: DMA1 通道5 半传输中断.
*             DMA1_IT_TE5: DMA1 通道5 传输错误中断.
*             DMA1_IT_GL6: DMA1 通道6 全局中断.
*             DMA1_IT_TC6: DMA1 通道6 传输完成中断.
*             DMA1_IT_HT6: DMA1 通道6 半传输中断.
*             DMA1_IT_TE6: DMA1 通道6 传输错误中断.
*             DMA1_IT_GL7: DMA1 通道7 全局中断.
*             DMA1_IT_TC7: DMA1 通道7 传输完成中断.
*             DMA1_IT_HT7: DMA1 通道7 半传输中断.
*             DMA1_IT_TE7: DMA1 通道7 传输错误中断.
*             DMA2_IT_GL1: DMA2 通道1 全局中断.
*             DMA2_IT_TC1: DMA2 通道1 传输完成中断.
*             DMA2_IT_HT1: DMA2 通道1 半传输中断.
*             DMA2_IT_TE1: DMA2 通道1 传输错误中断.
*             DMA2_IT_GL2: DMA2 通道2 全局中断.
*             DMA2_IT_TC2: DMA2 通道2 传输完成中断.
*             DMA2_IT_HT2: DMA2 通道2 半传输中断.
*             DMA2_IT_TE2: DMA2 通道2 传输错误中断.
*             DMA2_IT_GL3: DMA2 通道3 全局中断.
*             DMA2_IT_TC3: DMA2 通道3 传输完成中断.
*             DMA2_IT_HT3: DMA2 通道3 半传输中断.
*             DMA2_IT_TE3: DMA2 通道3 传输错误中断.
*             DMA2_IT_GL4: DMA2 通道4 全局中断.
*             DMA2_IT_TC4: DMA2 通道4 传输完成中断.
*             DMA2_IT_HT4: DMA2 通道4 半传输中断.
*             DMA2_IT_TE4: DMA2 通道4 传输错误中断.
*             DMA2_IT_GL5: DMA2 通道5 全局中断.
*             DMA2_IT_TC5: DMA2 通道5 传输完成中断.
*             DMA2_IT_HT5: DMA2 通道5 半传输中断.
*             DMA2_IT_TE5: DMA2 通道5 传输错误中断.
  * @返回  DMA_IT 的新状态 (SET 或 RESET).
  */
ITStatus DMA_GetITStatus(uint32_t DMA_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_DMA_GET_IT(DMA_IT));

  /* 计算使用的 DMA */
  if ((DMA_IT & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 得到 DMA2 ISR 寄存器的值 */
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* 得到 DMA1 ISR 寄存器的值 */
    tmpreg = DMA1->ISR ;
  }

  /* 检查指定的 DMA 中断 */
  if ((tmpreg & DMA_IT) != (uint32_t)RESET)
  {
    /* 设置 DMA_IT */
    bitstatus = SET;
  }
  else
  {
    /* 复位 DMA_IT */
    bitstatus = RESET;
  }
  /* 返回 DMA_IT 状态 */
  return  bitstatus;
}

/**
  * @简述  清除 DMA y 通道 x 中断待处理标志位.
  * @参数  DMA_IT: 指定的 DMA 中断挂起位.
*             这个参数可以是下面的值之一:
*             DMA1_IT_GL1: DMA1 通道1 全局中断.
*             DMA1_IT_TC1: DMA1 通道1 传输完成中断.
*             DMA1_IT_HT1: DMA1 通道1 半传输中断.
*             DMA1_IT_TE1: DMA1 通道1 传输错误中断.
*             DMA1_IT_GL2: DMA1 通道2 全局中断.
*             DMA1_IT_TC2: DMA1 通道2 传输完成中断.
*             DMA1_IT_HT2: DMA1 通道2 半传输中断.
*             DMA1_IT_TE2: DMA1 通道2 传输错误中断.
*             DMA1_IT_GL3: DMA1 通道3 全局中断.
*             DMA1_IT_TC3: DMA1 通道3 传输完成中断.
*             DMA1_IT_HT3: DMA1 通道3 半传输中断.
*             DMA1_IT_TE3: DMA1 通道3 传输错误中断.
*             DMA1_IT_GL4: DMA1 通道4 全局中断.
*             DMA1_IT_TC4: DMA1 通道4 传输完成中断.
*             DMA1_IT_HT4: DMA1 通道4 半传输中断.
*             DMA1_IT_TE4: DMA1 通道4 传输错误中断.
*             DMA1_IT_GL5: DMA1 通道5 全局中断.
*             DMA1_IT_TC5: DMA1 通道5 传输完成中断.
*             DMA1_IT_HT5: DMA1 通道5 半传输中断.
*             DMA1_IT_TE5: DMA1 通道5 传输错误中断.
*             DMA1_IT_GL6: DMA1 通道6 全局中断.
*             DMA1_IT_TC6: DMA1 通道6 传输完成中断.
*             DMA1_IT_HT6: DMA1 通道6 半传输中断.
*             DMA1_IT_TE6: DMA1 通道6 传输错误中断.
*             DMA1_IT_GL7: DMA1 通道7 全局中断.
*             DMA1_IT_TC7: DMA1 通道7 传输完成中断.
*             DMA1_IT_HT7: DMA1 通道7 半传输中断.
*             DMA1_IT_TE7: DMA1 通道7 传输错误中断.
*             DMA2_IT_GL1: DMA2 通道1 全局中断.
*             DMA2_IT_TC1: DMA2 通道1 传输完成中断.
*             DMA2_IT_HT1: DMA2 通道1 半传输中断.
*             DMA2_IT_TE1: DMA2 通道1 传输错误中断.
*             DMA2_IT_GL2: DMA2 通道2 全局中断.
*             DMA2_IT_TC2: DMA2 通道2 传输完成中断.
*             DMA2_IT_HT2: DMA2 通道2 半传输中断.
*             DMA2_IT_TE2: DMA2 通道2 传输错误中断.
*             DMA2_IT_GL3: DMA2 通道3 全局中断.
*             DMA2_IT_TC3: DMA2 通道3 传输完成中断.
*             DMA2_IT_HT3: DMA2 通道3 半传输中断.
*             DMA2_IT_TE3: DMA2 通道3 传输错误中断.
*             DMA2_IT_GL4: DMA2 通道4 全局中断.
*             DMA2_IT_TC4: DMA2 通道4 传输完成中断.
*             DMA2_IT_HT4: DMA2 通道4 半传输中断.
*             DMA2_IT_TE4: DMA2 通道4 传输错误中断.
*             DMA2_IT_GL5: DMA2 通道5 全局中断.
*             DMA2_IT_TC5: DMA2 通道5 传输完成中断.
*             DMA2_IT_HT5: DMA2 通道5 半传输中断.
*             DMA2_IT_TE5: DMA2 通道5 传输错误中断.
  * @返回  没有
  */
void DMA_ClearITPendingBit(uint32_t DMA_IT)
{
  /* 检查参数 */
  assert_param(IS_DMA_CLEAR_IT(DMA_IT));

  /* 计算使用的 DMA */
  if ((DMA_IT & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 清除选择的DMA中断挂起位 */
    DMA2->IFCR = DMA_IT;
  }
  else
  {
    /* 清除选择的DMA中断挂起位 */
    DMA1->IFCR = DMA_IT;
  }
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
