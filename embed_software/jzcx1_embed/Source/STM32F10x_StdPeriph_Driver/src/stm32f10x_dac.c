/**
  ******************************************************************************
  * @文件    stm32f10x_dac.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 DAC 固件函数.
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
#include "stm32f10x_dac.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* DAC EN mask */
#define CR_EN_Set                  ((uint32_t)0x00000001)

/* DAC DMAEN mask */
#define CR_DMAEN_Set               ((uint32_t)0x00001000)

/* CR register Mask */
#define CR_CLEAR_Mask              ((uint32_t)0x00000FFE)

/* DAC SWTRIG mask */
#define SWTRIGR_SWTRIG_Set         ((uint32_t)0x00000001)

/* DAC Dual Channels SWTRIG masks */
#define DUAL_SWTRIG_Set            ((uint32_t)0x00000003)
#define DUAL_SWTRIG_Reset          ((uint32_t)0xFFFFFFFC)

/* DHR registers offsets */
#define DHR12R1_Offset             ((uint32_t)0x00000008)
#define DHR12R2_Offset             ((uint32_t)0x00000014)
#define DHR12RD_Offset             ((uint32_t)0x00000020)

/* DOR register offset */
#define DOR_Offset                 ((uint32_t)0x0000002C)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将 DAC 部件的寄存器复位为默认值
  * @参数  没有
  * @返回  没有
  */
void DAC_DeInit(void)
{
  /* 使能 DAC 复位状态 */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
  /* 释放 DAC 复位状态 */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
}

/**
  * @简述  依照 DAC_InitStruct 指定的参数初始化 DAC 外部设备.
  * @参数  DAC_Channel: 选定的 DAC 通道. 
  *          这个参数可以是下列值之一:
  *          DAC_Channel_1: 选定 DAC 通道1
  *          DAC_Channel_2: 选定 DAC 通道2
  * @参数  DAC_InitStruct: 指向包含了指定 DAC 通道配置信息的 DAC_InitTypeDef 结构指针.
  * @返回  没有
  */
void DAC_Init(uint32_t DAC_Channel, DAC_InitTypeDef* DAC_InitStruct)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_DAC_TRIGGER(DAC_InitStruct->DAC_Trigger));
  assert_param(IS_DAC_GENERATE_WAVE(DAC_InitStruct->DAC_WaveGeneration));
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude));
  assert_param(IS_DAC_OUTPUT_BUFFER_STATE(DAC_InitStruct->DAC_OutputBuffer));
/*---------------------------- DAC CR Configuration --------------------------*/
  /* 得到 DAC CR 的值 */
  tmpreg1 = DAC->CR;
  /* 清除 BOFFx, TENx, TSELx, WAVEx 和 MAMPx 位 */
  tmpreg1 &= ~(CR_CLEAR_Mask << DAC_Channel);
  /* 配置选定的 DAC 通道: 缓冲输出, 触发器, 波形信号发生器,波形信号发生器的时标/振幅 */
  /* 设置 TSELx 和 TENx 位，按照 DAC_Trigger 的值 */
  /* 设置 WAVEx 位，按照 DAC_WaveGeneration 的值 */
  /* 设置 MAMPx 位，按照 DAC_LFSRUnmask_TriangleAmplitude 的值 */ 
  /* 设置 BOFFx 位，按照 DAC_OutputBuffer 的值 */   
  tmpreg2 = (DAC_InitStruct->DAC_Trigger | DAC_InitStruct->DAC_WaveGeneration |
             DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude | DAC_InitStruct->DAC_OutputBuffer);
  /* 根据DAC_Channel计算CR寄存器值 */
  tmpreg1 |= tmpreg2 << DAC_Channel;
  /* 写 DAC CR */
  DAC->CR = tmpreg1;
}

/**
  * @简述  用默认值填充 DAC_InitStruct 结构的每一个成员.
  * @参数  DAC_InitStruct : 指向将要被初始化的 DAC_InitTypeDef 结构指针.
  * @返回  没有
  */
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
/*--------------- Reset DAC init structure parameters values -----------------*/
  /* 初始化 DAC_Trigger 成员 */
  DAC_InitStruct->DAC_Trigger = DAC_Trigger_None;
  /* 初始化 DAC_WaveGeneration 成员 */
  DAC_InitStruct->DAC_WaveGeneration = DAC_WaveGeneration_None;
  /* 初始化 DAC_LFSRUnmask_TriangleAmplitude 成员 */
  DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  /* 初始化 DAC_OutputBuffer 成员 */
  DAC_InitStruct->DAC_OutputBuffer = DAC_OutputBuffer_Enable;
}

/**
  * @简述  使能或失能指定的 DAC 通道.
  * @参数  DAC_Channel: 选定 DAC 通道. 
  *          这个参数可以是下列值之一:
  *          DAC_Channel_1: 选定 DAC 通道1 
  *          DAC_Channel_2: 选定 DAC 通道2  
  * @参数  NewState: DAC 通道的新状态. 
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DAC_Cmd(uint32_t DAC_Channel, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能指定的 DAC 通道 */
    DAC->CR |= CR_EN_Set << DAC_Channel;
  }
  else
  {
    /* 失能指定的 DAC 通道 */
    DAC->CR &= ~(CR_EN_Set << DAC_Channel);
  }
}

/**
  * @简述  使能或失能指定的 DAC 通道 DMA 请求.
  * @参数  DAC_Channel: 选定的 DAC 通道. 
  *          这个参数可以是下列值之一:
  *          DAC_Channel_1: 选定 DAC 通道1 
  *          DAC_Channel_2: 选定 DAC 通道2  
  * @参数  NewState: 指定的 DAC 通道 DMA 请求的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DAC_DMACmd(uint32_t DAC_Channel, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能指定的 DAC 通道 DMA 请求 */
    DAC->CR |= CR_DMAEN_Set << DAC_Channel;
  }
  else
  {
    /* 失能指定的 DAC 通道 DMA 请求 */
    DAC->CR &= ~(CR_DMAEN_Set << DAC_Channel);
  }
}

/**
  * @简述  使能或失能选择的 DAC 通道软件触发.
  * @参数  DAC_Channel: 选定的 DAC 通道. 
  *          这个参数可以是下列值之一:
  *          DAC_Channel_1: 选定 DAC 通道1 
  *          DAC_Channel_2: 选定 DAC 通道2  
  * @参数  NewState: 选择的 DAC 通道软件触发的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DAC_SoftwareTriggerCmd(uint32_t DAC_Channel, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能选择的 DAC 通道软件触发 */
    DAC->SWTRIGR |= SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4);
  }
  else
  {
    /* 失能选择的 DAC 通道软件触发 */
    DAC->SWTRIGR &= ~(SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4));
  }
}

/**
  * @简述  使能或失能两个 DAC 通道同步软件触发.
  * @参数  NewState: 两个 DAC 通道同步软件触发的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能两个 DAC 通道同步软件触发 */
    DAC->SWTRIGR |= DUAL_SWTRIG_Set ;
  }
  else
  {
    /* 失能两个 DAC 通道同步软件触发 */
    DAC->SWTRIGR &= DUAL_SWTRIG_Reset;
  }
}

/**
  * @简述  使能或失能选择的 DAC 通道波形发生.
  * @参数  DAC_Channel: 选定的 DAC 通道. 
  *          这个参数可以是下列值之一:
  *          DAC_Channel_1: 选定 DAC 通道1 
  *          DAC_Channel_2: 选定 DAC 通道2 
  * @参数  DAC_Wave: 指定的波形类型.
  *          这个参数可以是下列值之一:
  *          DAC_Wave_Noise:    噪音波形发生
  *          DAC_Wave_Triangle: 三角波形发生
  * @参数  NewState: 选择的 DAC 通道波形发生的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DAC_WaveGenerationCmd(uint32_t DAC_Channel, uint32_t DAC_Wave, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_WAVE(DAC_Wave)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能选择的 DAC 通道选择的波形 */
    DAC->CR |= DAC_Wave << DAC_Channel;
  }
  else
  {
    /* 失能选择的 DAC 通道选择的波形 */
    DAC->CR &= ~(DAC_Wave << DAC_Channel);
  }
}

/**
  * @简述  设置 DAC 通道1 选定的数据保持寄存器值.
  * @参数  DAC_Align: DAC 通道1 指定的数据对齐.
  *          这个参数可以是下列值之一:
  *          DAC_Align_8b_R:  选择8位数据右对齐
  *          DAC_Align_12b_L: 选择12位数据左对齐
  *          DAC_Align_12b_R: 选择12位数据右对齐
  * @参数  Data : 装入选择的数据保持寄存器的数据.
  * @返回  没有
  */
void DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data)
{  
  __IO uint32_t tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));
  
  tmp = (uint32_t)DAC_BASE; 
  tmp += DHR12R1_Offset + DAC_Align;

  /* 设置 DAC 通道1 选定的数据保持寄存器值 */
  *(__IO uint32_t *) tmp = Data;
}

/**
  * @简述  设置 DAC 通道2 选定的数据保持寄存器值.
  * @参数  DAC_Align: DAC 通道2 指定的数据对齐.
  *          这个参数可以是下列值之一:
  *          DAC_Align_8b_R:  选择8位数据右对齐
  *          DAC_Align_12b_L: 选择12位数据左对齐
  *          DAC_Align_12b_R: 选择12位数据右对齐
  * @参数  Data : 装入选择的数据保持寄存器的数据.
  * @返回  没有
  */
void DAC_SetChannel2Data(uint32_t DAC_Align, uint16_t Data)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));
  
  tmp = (uint32_t)DAC_BASE;
  tmp += DHR12R2_Offset + DAC_Align;

  /* 设置 DAC 通道2 选定的数据保持寄存器值 */
  *(__IO uint32_t *)tmp = Data;
}

/**
  * @简述  设置双通道 DAC 指定的数据保持寄存器值.
  * @参数  DAC_Align: 双通道 DAC 指定数据对齐.
  *          这个参数可以是下列值之一:
  *          DAC_Align_8b_R:  选择8位数据右对齐
  *          DAC_Align_12b_L: 选择12位数据左对齐
  *          DAC_Align_12b_R: 选择12位数据右对齐
  * @参数  Data2: DAC通道2 装载到选择的数据保持寄存器的数据.
  * @参数  Data1: DAC通道1 装载到选择的数据保持寄存器的数据.
  * @返回  没有
  */
void DAC_SetDualChannelData(uint32_t DAC_Align, uint16_t Data2, uint16_t Data1)
{
  uint32_t data = 0, tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data1));
  assert_param(IS_DAC_DATA(Data2));
  
  /* 计算并设置双 DAC 数据保持寄存器的值 */
  if (DAC_Align == DAC_Align_8b_R)
  {
    data = ((uint32_t)Data2 << 8) | Data1; 
  }
  else
  {
    data = ((uint32_t)Data2 << 16) | Data1;
  }
  
  tmp = (uint32_t)DAC_BASE;
  tmp += DHR12RD_Offset + DAC_Align;

  /* 设置双 DAC 选择的数据保持寄存器 */
  *(__IO uint32_t *)tmp = data;
}

/**
  * @简述  返回最新的 DAC 通道数据寄存器输出值.
  * @参数  DAC_Channel: 选定的 DAC 通道. 
  *          这个参数可以是下列值之一:
  *          DAC_Channel_1: 选定 DAC 通道1 
  *          DAC_Channel_2: 选定 DAC 通道2 
  * @返回  选定的 DAC 数据输出值.
  */
uint16_t DAC_GetDataOutputValue(uint32_t DAC_Channel)
{
  __IO uint32_t tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  
  tmp = (uint32_t) DAC_BASE ;
  tmp += DOR_Offset + ((uint32_t)DAC_Channel >> 2);
  
  /* 返回选定的 DAC 数据寄存器输出值 */
  return (uint16_t) (*(__IO uint32_t*) tmp);
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
