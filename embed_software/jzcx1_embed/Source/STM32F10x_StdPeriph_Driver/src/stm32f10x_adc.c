/**
  ******************************************************************************
  * @文件    stm32f10x_adc.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 ADC 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    17/07/2009
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
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* ADC DISCNUM mask */
#define CR1_DISCNUM_Reset           ((uint32_t)0xFFFF1FFF)

/* ADC DISCEN mask */
#define CR1_DISCEN_Set              ((uint32_t)0x00000800)
#define CR1_DISCEN_Reset            ((uint32_t)0xFFFFF7FF)

/* ADC JAUTO mask */
#define CR1_JAUTO_Set               ((uint32_t)0x00000400)
#define CR1_JAUTO_Reset             ((uint32_t)0xFFFFFBFF)

/* ADC JDISCEN mask */
#define CR1_JDISCEN_Set             ((uint32_t)0x00001000)
#define CR1_JDISCEN_Reset           ((uint32_t)0xFFFFEFFF)

/* ADC AWDCH mask */
#define CR1_AWDCH_Reset             ((uint32_t)0xFFFFFFE0)

/* ADC Analog watchdog enable mode mask */
#define CR1_AWDMode_Reset           ((uint32_t)0xFF3FFDFF)

/* CR1 register Mask */
#define CR1_CLEAR_Mask              ((uint32_t)0xFFF0FEFF)

/* ADC ADON mask */
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_ADON_Reset              ((uint32_t)0xFFFFFFFE)

/* ADC DMA mask */
#define CR2_DMA_Set                 ((uint32_t)0x00000100)
#define CR2_DMA_Reset               ((uint32_t)0xFFFFFEFF)

/* ADC RSTCAL mask */
#define CR2_RSTCAL_Set              ((uint32_t)0x00000008)

/* ADC CAL mask */
#define CR2_CAL_Set                 ((uint32_t)0x00000004)

/* ADC SWSTART mask */
#define CR2_SWSTART_Set             ((uint32_t)0x00400000)

/* ADC EXTTRIG mask */
#define CR2_EXTTRIG_Set             ((uint32_t)0x00100000)
#define CR2_EXTTRIG_Reset           ((uint32_t)0xFFEFFFFF)

/* ADC Software start mask */
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)
#define CR2_EXTTRIG_SWSTART_Reset   ((uint32_t)0xFFAFFFFF)

/* ADC JEXTSEL mask */
#define CR2_JEXTSEL_Reset           ((uint32_t)0xFFFF8FFF)

/* ADC JEXTTRIG mask */
#define CR2_JEXTTRIG_Set            ((uint32_t)0x00008000)
#define CR2_JEXTTRIG_Reset          ((uint32_t)0xFFFF7FFF)

/* ADC JSWSTART mask */
#define CR2_JSWSTART_Set            ((uint32_t)0x00200000)

/* ADC injected software start mask */
#define CR2_JEXTTRIG_JSWSTART_Set   ((uint32_t)0x00208000)
#define CR2_JEXTTRIG_JSWSTART_Reset ((uint32_t)0xFFDF7FFF)

/* ADC TSPD mask */
#define CR2_TSVREFE_Set             ((uint32_t)0x00800000)
#define CR2_TSVREFE_Reset           ((uint32_t)0xFF7FFFFF)

/* CR2 register Mask */
#define CR2_CLEAR_Mask              ((uint32_t)0xFFF1F7FD)

/* ADC SQx mask */
#define SQR3_SQ_Set                 ((uint32_t)0x0000001F)
#define SQR2_SQ_Set                 ((uint32_t)0x0000001F)
#define SQR1_SQ_Set                 ((uint32_t)0x0000001F)

/* SQR1 register Mask */
#define SQR1_CLEAR_Mask             ((uint32_t)0xFF0FFFFF)

/* ADC JSQx mask */
#define JSQR_JSQ_Set                ((uint32_t)0x0000001F)

/* ADC JL mask */
#define JSQR_JL_Set                 ((uint32_t)0x00300000)
#define JSQR_JL_Reset               ((uint32_t)0xFFCFFFFF)

/* ADC SMPx mask */
#define SMPR1_SMP_Set               ((uint32_t)0x00000007)
#define SMPR2_SMP_Set               ((uint32_t)0x00000007)

/* ADC JDRx registers offset */
#define JDR_Offset                  ((uint8_t)0x28)

/* ADC1 DR register base address */
#define DR_ADDRESS                  ((uint32_t)0x4001244C)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将外设 ADCx 的全部寄存器重设为缺省值.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  没有
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  
  if (ADCx == ADC1)
  {
    /* 使能 ADC1 复位状态 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* 从复位状态释放 ADC1  */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
  }
  else if (ADCx == ADC2)
  {
    /* 使能 ADC2 复位状态 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
    /* 从复位状态释放 ADC2 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
  }
  else
  {
    if (ADCx == ADC3)
    {
      /* 使能 ADC3 复位状态 */
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
      /* 从复位状态释放 ADC3 */
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
    }
  }
}

/**
  * @简述  根据 ADC_InitStruct 中指定的参数初始化外设 ADCx 的寄存器.
  * @参数  ADCx: 其中x 可以是1 2或3，用来选择ADC 外围模块.
  * @参数  ADC_InitStruct: 指向结构体 ADC_InitTypeDef 的指针,该结构包括了指定ADC外围模块的配置信息.
  * @返回  没有
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  uint32_t tmpreg1 = 0;
  uint8_t tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
  assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign)); 
  assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

  /*---------------------------- ADCx CR1 Configuration -----------------*/
  /* 得到 ADCx CR1 的值 */
  tmpreg1 = ADCx->CR1;
  /* 清除 DUALMOD 和 SCAN 位 */
  tmpreg1 &= CR1_CLEAR_Mask;
  /* 配置 ADCx: 双重模式和扫描转换模式 */
  /* 设置 DUALMOD 位， 按照 ADC_Mode 的值 */
  /* 设置 SCAN 位，按照 ADC_ScanConvMode 的值 */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_Mode | ((uint32_t)ADC_InitStruct->ADC_ScanConvMode << 8));
  /* 写 ADCx CR1 */
  ADCx->CR1 = tmpreg1;

  /*---------------------------- ADCx CR2 Configuration -----------------*/
  /* 得到 ADCx CR2 的值 */
  tmpreg1 = ADCx->CR2;
  /* 清除 CONT, ALIGN 和 EXTSEL 位 */
  tmpreg1 &= CR2_CLEAR_Mask;
  /* 配置 ADCx: 外触发事件和连续转换模式 */
  /* 设置 ALIGN 位，按照 ADC_DataAlign 的值 */
  /* 设置 EXTSEL 位，按照 ADC_ExternalTrigConv 的值 */
  /* 设置 CONT 位，按照 ADC_ContinuousConvMode 的值 */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
            ((uint32_t)ADC_InitStruct->ADC_ContinuousConvMode << 1));
  /* 写 ADCx CR2 */
  ADCx->CR2 = tmpreg1;

  /*---------------------------- ADCx SQR1 Configuration -----------------*/
  /* 得到 ADCx SQR1 的值 */
  tmpreg1 = ADCx->SQR1;
  /* 清除 L 位 */
  tmpreg1 &= SQR1_CLEAR_Mask;
  /* 配置 ADCx: 规则信道长度 */
  /* 设置 L 位，按照 ADC_NbrOfChannel 的值 */
  tmpreg2 |= (uint8_t) (ADC_InitStruct->ADC_NbrOfChannel - (uint8_t)1);
  tmpreg1 |= (uint32_t)tmpreg2 << 20;
  /* 写 ADCx SQR1 */
  ADCx->SQR1 = tmpreg1;
}

/**
  * @简述  把 ADC_InitStruct 中的每一个参数按缺省值填入.
  * @参数  ADC_InitStruct : 指向结构体 ADC_InitTypeDef 的指针，用来初始化.
  * @返回  没有
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* 复位 ADC 初始结构变量 */
  /* 初始化 ADC_Mode 成员 */
  ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;
  /* 初始化 ADC_ScanConvMode 成员 */
  ADC_InitStruct->ADC_ScanConvMode = DISABLE;
  /* 初始化 ADC_ContinuousConvMode 成员 */
  ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;
  /* 初始化 ADC_ExternalTrigConv 成员 */
  ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* 初始化 ADC_DataAlign 成员 */
  ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;
  /* 初始化 ADC_NbrOfChannel 成员 */
  ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/**
  * @简述  使能或者失能指定的 ADC .
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: ADCx 设备的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置 ADON 位，从省电模式下唤醒ADC */
    ADCx->CR2 |= CR2_ADON_Set;
  }
  else
  {
    /* 失能指定的 ADC 设备 */
    ADCx->CR2 &= CR2_ADON_Reset;
  }
}

/**
  * @简述  使能或失能指定的 ADC DMA 请求.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  *        注解: ADC2 没有 DMA 能力.
  * @参数  NewState: 选定的 ADC DMA 转移器的新状态.
  *        这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_DMA_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能选定的 ADC DMA 请求 */
    ADCx->CR2 |= CR2_DMA_Set;
  }
  else
  {
    /* 失能选定的 ADC DMA 请求 */
    ADCx->CR2 &= CR2_DMA_Reset;
  }
}

/**
  * @简述  使能或者失能指定的ADC的中断.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_IT: 指定的 ADC 中断源. 
  *          可以使用下述值的一个或者几个值的组合:
  *          ADC_IT_EOC:  换算结束中断屏蔽
  *          ADC_IT_AWD:  模拟看门狗中断屏蔽
  *          ADC_IT_JEOC: 注入通道完成中断屏蔽
  * @参数  NewState: 指定的 ADC 中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState)
{
  uint8_t itmask = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT));
  /* 得到 ADC IT 中断号 */
  itmask = (uint8_t)ADC_IT;
  if (NewState != DISABLE)
  {
    /* 使能选定的 ADC 中断 */
    ADCx->CR1 |= itmask;
  }
  else
  {
    /* 失能指定的 ADC 中断 */
    ADCx->CR1 &= (~(uint32_t)itmask);
  }
}

/**
  * @简述  重置指定的ADC的校准寄存器.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  没有
  */
void ADC_ResetCalibration(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 重置指定的ADC的校准寄存器 */  
  ADCx->CR2 |= CR2_RSTCAL_Set;
}

/**
  * @简述  获取ADC重置校准寄存器的状态.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  ADC重置校准寄存器的新状态 (SET 或 RESET).
  */
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 RSTCAL 位的状态 */
  if ((ADCx->CR2 & CR2_RSTCAL_Set) != (uint32_t)RESET)
  {
    /* 设置 RSTCAL 位 */
    bitstatus = SET;
  }
  else
  {
    /* 复位 RSTCAL 位 */
    bitstatus = RESET;
  }
  /* 返回 RSTCAL 位的状态 */
  return  bitstatus;
}

/**
  * @简述  开始指定ADC的校准状态.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  没有
  */
void ADC_StartCalibration(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 使能选定的 ADC 校准处理 */  
  ADCx->CR2 |= CR2_CAL_Set;
}

/**
  * @简述  获取指定ADC的校准状态.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  指定ADC的校准的新状态 (SET 或 RESET).
  */
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 CAL 位的状态 */
  if ((ADCx->CR2 & CR2_CAL_Set) != (uint32_t)RESET)
  {
    /* 设置 CAL 位: 较准进行中 */
    bitstatus = SET;
  }
  else
  {
    /* 复位 CAL 位: 校准结束 */
    bitstatus = RESET;
  }
  /* 返回 CAL 位的状态 */
  return  bitstatus;
}

/**
  * @简述  使能或者失能指定的ADC的软件转换启动功能.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: 选中的由软件发出开始信号的ADC的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 允许选择的ADC转换外部事件和启动选择的ADC转换 */
    ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
  }
  else
  {
    /* 禁止选择的ADC转换外部事件和停止选择的ADC转换 */
    ADCx->CR2 &= CR2_EXTTRIG_SWSTART_Reset;
  }
}

/**
  * @简述  获取ADC软件转换启动状态.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  ADC软件开始转换的新状态 (SET 或 RESET).
  */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 SWSTART 位的新状态 */
  if ((ADCx->CR2 & CR2_SWSTART_Set) != (uint32_t)RESET)
  {
    /* 设置 SWSTART 位 */
    bitstatus = SET;
  }
  else
  {
    /* 复位 SWSTART 位 */
    bitstatus = RESET;
  }
  /* 返回 SWSTART 位的状态 */
  return  bitstatus;
}

/**
  * @简述  对ADC规则组通道配置间断模式.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  Number: 模式下常规信道计数值，该值范围为1－8. 
  * @返回  没有
  */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));
  /* 得到过去的寄存器值 */
  tmpreg1 = ADCx->CR1;
  /* 清除过去的不间断模式通道计数器 */
  tmpreg1 &= CR1_DISCNUM_Reset;
  /* 设置间断模式通道计数器 */
  tmpreg2 = Number - 1;
  tmpreg1 |= tmpreg2 << 13;
  /* 存储寄存器的新值 */
  ADCx->CR1 = tmpreg1;
}

/**
  * @简述  使能或者失能指定的ADC规则组通道的间断模式.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: 指定的ADC规则组通道的间断模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能指定的ADC规则组通道的间断模式 */
    ADCx->CR1 |= CR1_DISCEN_Set;
  }
  else
  {
    /* 失能指定的ADC规则组通道的间断模式 */
    ADCx->CR1 &= CR1_DISCEN_Reset;
  }
}

/**
  * @简述  设置指定ADC的规则组通道，设置它们的转化顺序和采样时间.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_Channel: 配置 ADC 通道. 
  *          这个参数可以使用下述值的一个:
  *          ADC_Channel_0:  选择ADC通道0  
  *          ADC_Channel_1:  选择ADC通道1  
  *          ADC_Channel_2:  选择ADC通道2  
  *          ADC_Channel_3:  选择ADC通道3  
  *          ADC_Channel_4:  选择ADC通道4  
  *          ADC_Channel_5:  选择ADC通道5  
  *          ADC_Channel_6:  选择ADC通道6  
  *          ADC_Channel_7:  选择ADC通道7  
  *          ADC_Channel_8:  选择ADC通道8  
  *          ADC_Channel_9:  选择ADC通道9  
  *          ADC_Channel_10: 选择ADC通道10  
  *          ADC_Channel_11: 选择ADC通道11  
  *          ADC_Channel_12: 选择ADC通道12  
  *          ADC_Channel_13: 选择ADC通道13  
  *          ADC_Channel_14: 选择ADC通道14  
  *          ADC_Channel_15: 选择ADC通道15  
  *          ADC_Channel_16: 选择ADC通道16  
  *          ADC_Channel_17: 选择ADC通道17  
  * @参数  Rank: 规则组采样顺序. 取值范围1到16.
  * @参数  ADC_SampleTime: 指定ADC通道的采样时间值. 
  *          这个参数可以使用下述值的一个:
  *          ADC_SampleTime_1Cycles5:   采样时间等于 1.5   周期
  *          ADC_SampleTime_7Cycles5:   采样时间等于 7.5   周期
  *          ADC_SampleTime_13Cycles5:  采样时间等于 13.5  周期
  *          ADC_SampleTime_28Cycles5:  采样时间等于 28.5  周期	
  *          ADC_SampleTime_41Cycles5:  采样时间等于 41.5  周期	
  *          ADC_SampleTime_55Cycles5:  采样时间等于 55.5  周期	
  *          ADC_SampleTime_71Cycles5:  采样时间等于 71.5  周期	
  *          ADC_SampleTime_239Cycles5: 采样时间等于 239.5 周期	
  * @返回  没有
  */
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_REGULAR_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
  /* 如果选定 ADC_Channel_10 ... ADC_Channel_17 */
  if (ADC_Channel > ADC_Channel_9)
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SMPR1;
    /* 计算需要清除的标志 */
    tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    /* 清除旧的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] */
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SMPR2;
    /* 计算需要清除的标志 */
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* 清除旧的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SMPR2 = tmpreg1;
  }
  /* For Rank 1 to 6 */
  if (Rank < 7)
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SQR3;
    /* 计算需要清除的标志 */
    tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
    /* 根据选择的序列清除旧的 SQx 位 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
    /* 根据选择的序列设置新的 SQx 位 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SQR3 = tmpreg1;
  }
  /* For Rank 7 to 12 */
  else if (Rank < 13)
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SQR2;
    /* 计算需要清除的标志 */
    tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
    /* 根据选择的序列清除旧的 SQx 位 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
    /* 根据选择的序列设置新的 SQx 位 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SQR2 = tmpreg1;
  }
  /* For Rank 13 to 16 */
  else
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SQR1;
    /* 计算需要清除的标志 */
    tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
    /* 根据选择的序列清除旧的 SQx 位 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
    /* 根据选择的序列设置新的 SQx 位 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SQR1 = tmpreg1;
  }
}

/**
  * @简述  使能或者失能 ADCx 的经外部触发启动转换功能.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: 选定的 ADCx 的经外部触发启动转换功能的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能 ADCx 的经外部触发启动转换功能 */
    ADCx->CR2 |= CR2_EXTTRIG_Set;
  }
  else
  {
    /* 失能 ADCx 的经外部触发启动转换功能 */
    ADCx->CR2 &= CR2_EXTTRIG_Reset;
  }
}

/**
  * @简述  返回最近一次 ADCx 规则组的转换结果.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  转换结果.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 返回最近一次 ADCx 规则组的转换结果 */
  return (uint16_t) ADCx->DR;
}

/**
  * @简述  返回最近一次双 ADC 模式下的 ADC1 和 ADC2 转换结果.
  * @retval 转换结果.
  */
uint32_t ADC_GetDualModeConversionValue(void)
{
  /* 返回双转换下的转换结果 */
  return (*(__IO uint32_t *) DR_ADDRESS);
}

/**
  * @简述  使能或者失能指定 ADC 在规则组转化后自动开始注入组转换.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: 选中的进行自动注入转换的ADC新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能指定 ADC 在规则组转化后自动开始注入组转换 */
    ADCx->CR1 |= CR1_JAUTO_Set;
  }
  else
  {
    /* 失能指定 ADC 在规则组转化后自动开始注入组转换 */
    ADCx->CR1 &= CR1_JAUTO_Reset;
  }
}

/**
  * @简述  使能或者失能指定 ADC 的注入组间断模式
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: ADC 注入组通道上间断模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 失能指定 ADC 的注入组间断模式 */
    ADCx->CR1 |= CR1_JDISCEN_Set;
  }
  else
  {
    /* 失能指定 ADC 的注入组间断模式 */
    ADCx->CR1 &= CR1_JDISCEN_Reset;
  }
}

/**
  * @简述  配置 ADCx 的外部触发启动注入组转换功能.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_ExternalTrigInjecConv: 开始注入转换的ADC触发器. 
  *          这个参数可以取下列值之一:
  *          ADC_ExternalTrigInjecConv_T1_TRGO: 选择定时器1的TRGO作为注入转换外部触发 (for ADC1, ADC2 and ADC3)
  *          ADC_ExternalTrigInjecConv_T1_CC4:  选择定时器1的捕获比较4作为注入转换外部触发 (for ADC1, ADC2 and ADC3)
  *          ADC_ExternalTrigInjecConv_T2_TRGO: 选择定时器2的 TRGO 作为注入转换外部触发 (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_T2_CC1:  选择定时器2的捕获比较1作为注入转换外部触发 (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_T3_CC4:  选择定时器3的捕获比较4作为注入转换外部触发 (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_T4_TRGO: 选择定时器4的TRGO作为注入转换外部触发 (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4: 外部中断15或者timer8比较事件作为注入转换的外部触发器 (for ADC1 and ADC2)                       
  *          ADC_ExternalTrigInjecConv_T4_CC3:  选择定时器4捕获比较3 (for ADC3 only)
  *          ADC_ExternalTrigInjecConv_T8_CC2:  选择定时器8捕获比较2 (for ADC3 only)                         
  *          ADC_ExternalTrigInjecConv_T8_CC4:  选择定时器8捕获比较4 (for ADC3 only)
  *          ADC_ExternalTrigInjecConv_T5_TRGO: 选择定时器5触发事件 (for ADC3 only)                         
  *          ADC_ExternalTrigInjecConv_T5_CC4:  选择定时器5捕获比较4 (for ADC3 only)                        
  *          ADC_ExternalTrigInjecConv_None:    注入转换由软件启动，而不是由外部触发器启动 (for ADC1, ADC2 and ADC3)
  * @返回  没有
  */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));
  /* 得到旧的寄存器的值 */
  tmpreg = ADCx->CR2;
  /* 清除选择的注入通道群的外部触发事件 */
  tmpreg &= CR2_JEXTSEL_Reset;
  /* 设置选择的注入通道群的外部触发事件 */
  tmpreg |= ADC_ExternalTrigInjecConv;
  /* 存储新的寄存器的值 */
  ADCx->CR2 = tmpreg;
}

/**
  * @简述  使能或者失能 ADCx 的经外部触发启动注入组转换功能.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: 指定ADC外部触发启动注入转换的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能 ADCx 的经外部触发启动注入组转换功能 */
    ADCx->CR2 |= CR2_JEXTTRIG_Set;
  }
  else
  {
    /* 失能 ADCx 的经外部触发启动注入组转换功能 */
    ADCx->CR2 &= CR2_JEXTTRIG_Reset;
  }
}

/**
  * @简述  使能或者失能 ADCx 软件启动注入组转换功能.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  NewState: 指定 ADC 软件触发启动注入转换的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能 ADCx 软件启动注入组转换功能并且启动注入选择 */
    ADCx->CR2 |= CR2_JEXTTRIG_JSWSTART_Set;
  }
  else
  {
    /* 失能 ADCx 软件启动注入组转换功能并且启动注入选择 */
    ADCx->CR2 &= CR2_JEXTTRIG_JSWSTART_Reset;
  }
}

/**
  * @简述  获取指定 ADC 的软件启动注入组转换状态.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @返回  指定的 ADC 软件触发启动注入转换的新状态 (SET 或 RESET).
  */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 JSWSTART 位的状态 */
  if ((ADCx->CR2 & CR2_JSWSTART_Set) != (uint32_t)RESET)
  {
    /* 设置 JSWSTART 位 */
    bitstatus = SET;
  }
  else
  {
    /* 复位 JSWSTART 位 */
    bitstatus = RESET;
  }
  /* 返回 JSWSTART 位的状态 */
  return  bitstatus;
}

/**
  * @简述  设置指定 ADC 的注入组通道，设置它们的转化顺序和采样时间.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_Channel: 被设置的ADC通道. 
  *          这个参数可以取下列值之一:
  *          ADC_Channel_0:  选择ADC通道0  
  *          ADC_Channel_1:  选择ADC通道1  
  *          ADC_Channel_2:  选择ADC通道2  
  *          ADC_Channel_3:  选择ADC通道3  
  *          ADC_Channel_4:  选择ADC通道4  
  *          ADC_Channel_5:  选择ADC通道5  
  *          ADC_Channel_6:  选择ADC通道6  
  *          ADC_Channel_7:  选择ADC通道7  
  *          ADC_Channel_8:  选择ADC通道8  
  *          ADC_Channel_9:  选择ADC通道9  
  *          ADC_Channel_10: 选择ADC通道10  
  *          ADC_Channel_11: 选择ADC通道11  
  *          ADC_Channel_12: 选择ADC通道12  
  *          ADC_Channel_13: 选择ADC通道13  
  *          ADC_Channel_14: 选择ADC通道14  
  *          ADC_Channel_15: 选择ADC通道15  
  *          ADC_Channel_16: 选择ADC通道16  
  *          ADC_Channel_17: 选择ADC通道17 
  * @参数  Rank: 规则组采样顺序.取值范围1到4.
  * @参数  ADC_SampleTime: 指定ADC通道的采样时间值. 
  *          这个参数可以使用下述值的一个:
  *          ADC_SampleTime_1Cycles5:   采样时间等于 1.5   周期
  *          ADC_SampleTime_7Cycles5:   采样时间等于 7.5   周期
  *          ADC_SampleTime_13Cycles5:  采样时间等于 13.5  周期
  *          ADC_SampleTime_28Cycles5:  采样时间等于 28.5  周期	
  *          ADC_SampleTime_41Cycles5:  采样时间等于 41.5  周期	
  *          ADC_SampleTime_55Cycles5:  采样时间等于 55.5  周期	
  *          ADC_SampleTime_71Cycles5:  采样时间等于 71.5  周期	
  *          ADC_SampleTime_239Cycles5: 采样时间等于 239.5 周期		
  * @返回  没有
  */
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_INJECTED_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
  /* 如果选择 ADC_Channel_10 ... ADC_Channel_17 */
  if (ADC_Channel > ADC_Channel_9)
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SMPR1;
    /* 计算需要清除的标志 */
    tmpreg2 = SMPR1_SMP_Set << (3*(ADC_Channel - 10));
    /* 清除旧的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3*(ADC_Channel - 10));
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] */
  {
    /* 得到旧的寄存器的值 */
    tmpreg1 = ADCx->SMPR2;
    /* 计算需要清除的标志 */
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* 清除旧的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算需要置位的标志 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 存储新的寄存器的值 */
    ADCx->SMPR2 = tmpreg1;
  }
  /* 配置 Rank 规则组采样顺序 */
  /* 得到旧的寄存器的值 */
  tmpreg1 = ADCx->JSQR;
  /* 得到 JL 的值: Number = JL+1 */
  tmpreg3 =  (tmpreg1 & JSQR_JL_Set)>> 20;
  /* 计算需要清除的标志: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = JSQR_JSQ_Set << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
  /* 清除选中序列 JSQx 位 */
  tmpreg1 &= ~tmpreg2;
  /* 计算需要置位的标志: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = (uint32_t)ADC_Channel << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
  /* 置位选中序列旧的 JSQx 位 */
  tmpreg1 |= tmpreg2;
  /* 存储新的寄存器的值 */
  ADCx->JSQR = tmpreg1;
}

/**
  * @简述  设置注入组通道的转换序列长度
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  Length: 转换序列长度. 
  *                这个参数必须是 1 到 4 之间的值.
  * @返回  没有
  */
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_LENGTH(Length));
  
  /* 得到旧的寄存器的值 */
  tmpreg1 = ADCx->JSQR;
  /* 清除注入信道音序器的长度 JL 位 */
  tmpreg1 &= JSQR_JL_Reset;
  /* 置位注入信道音序器的长度 JL 位 */
  tmpreg2 = Length - 1; 
  tmpreg1 |= tmpreg2 << 20;
  /* 存储新的寄存器的值 */
  ADCx->JSQR = tmpreg1;
}

/**
  * @简述  设置注入组通道的转换偏移值
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_InjectedChannel: 需要设置偏移值的ADC注入信道. 
  *          这个参数可以使用下述值的一个:
  *          ADC_InjectedChannel_1: 选择注入通道1 
  *          ADC_InjectedChannel_2: 选择注入通道2 
  *          ADC_InjectedChannel_3: 选择注入通道3 
  *          ADC_InjectedChannel_4: 选择注入通道4 
  * @参数  Offset: 选中的ADC注入信道的偏移值，该参数是一个12位值.
  * @返回  没有
  */
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset)
{
  __IO uint32_t tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
  assert_param(IS_ADC_OFFSET(Offset));  
  
  tmp = (uint32_t)ADCx;
  tmp += ADC_InjectedChannel;
  
  /* 设置选定的注入通道的数据偏移量 */
  *(__IO uint32_t *) tmp = (uint32_t)Offset;
}

/**
  * @简述  返回 ADC 指定注入通道的转换结果
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_InjectedChannel: 指定注入通道.
  *          这个参数可以使用下述值的一个:
  *          ADC_InjectedChannel_1: 选定注入通道1 
  *          ADC_InjectedChannel_2: 选定注入通道2
  *          ADC_InjectedChannel_3: 选定注入通道3
  *          ADC_InjectedChannel_4: 选定注入通道4
  * @返回  转换结果.
  */
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel)
{
  __IO uint32_t tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

  tmp = (uint32_t)ADCx;
  tmp += ADC_InjectedChannel + JDR_Offset;
  
  /* 返回 ADC 指定注入通道的转换结果 */
  return (uint16_t) (*(__IO uint32_t*)  tmp);   
}

/**
  * @简述  使能或者失能指定单个/全体，规则/注入组通道上的模拟看门狗
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_AnalogWatchdog: ADC模拟看门狗配置.
  *          这个参数可以使用下述值的一个:
  *          ADC_AnalogWatchdog_SingleRegEnable:        单个规则通道上设置模拟看门狗
  *          ADC_AnalogWatchdog_SingleInjecEnable:      单个注入通道上设置模拟看门狗
  *          ADC_AnalogWatchdog_SingleRegOrInjecEnable: 单个规则通道或者注入通道上设置模拟看门狗
  *          ADC_AnalogWatchdog_AllRegEnable:           所有规则通道上设置模拟看门狗
  *          ADC_AnalogWatchdog_AllInjecEnable:         所有注入通道上设置模拟看门狗
  *          ADC_AnalogWatchdog_AllRegAllInjecEnable:   所有规则通道和所有注入通道上上设置模拟看门狗
  *          ADC_AnalogWatchdog_None:                   不设置模拟看门狗
  * @返回  没有	  
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));
  /* 得到旧的寄存器的值 */
  tmpreg = ADCx->CR1;
  /* 清除 AWDEN, AWDENJ 和 AWDSGL 位 */
  tmpreg &= CR1_AWDMode_Reset;
  /* 设置模拟看门狗允许模式 */
  tmpreg |= ADC_AnalogWatchdog;
  /* 存储新的寄存器的值 */
  ADCx->CR1 = tmpreg;
}

/**
  * @简述  设置模拟看门狗的高/低阈值.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  HighThreshold: 模拟看门狗的高阈值.
  *                       这个参数是一个12位的值.
  * @参数  LowThreshold:  模拟看门狗的低阈值.
  *                       这个参数是一个12位的值.
  * @返回  没有
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_THRESHOLD(HighThreshold));
  assert_param(IS_ADC_THRESHOLD(LowThreshold));
  /* 设置模拟看门狗的高阈值 */
  ADCx->HTR = HighThreshold;
  /* 设置模拟看门狗的低阈值 */
  ADCx->LTR = LowThreshold;
}

/**
  * @简述  对单个ADC通道设置模拟看门狗
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_Channel: 将要为其配置模拟看门狗的信道. 
  *          这个参数可以使用下述值的一个:
  *          ADC_Channel_0:  选择ADC通道0  
  *          ADC_Channel_1:  选择ADC通道1  
  *          ADC_Channel_2:  选择ADC通道2  
  *          ADC_Channel_3:  选择ADC通道3  
  *          ADC_Channel_4:  选择ADC通道4  
  *          ADC_Channel_5:  选择ADC通道5  
  *          ADC_Channel_6:  选择ADC通道6  
  *          ADC_Channel_7:  选择ADC通道7  
  *          ADC_Channel_8:  选择ADC通道8  
  *          ADC_Channel_9:  选择ADC通道9  
  *          ADC_Channel_10: 选择ADC通道10  
  *          ADC_Channel_11: 选择ADC通道11  
  *          ADC_Channel_12: 选择ADC通道12  
  *          ADC_Channel_13: 选择ADC通道13  
  *          ADC_Channel_14: 选择ADC通道14  
  *          ADC_Channel_15: 选择ADC通道15  
  *          ADC_Channel_16: 选择ADC通道16  
  *          ADC_Channel_17: 选择ADC通道17 
  * @返回  没有
  */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  /* 得到旧的寄存器的值 */
  tmpreg = ADCx->CR1;
  /* 清除模拟看门狗通道选择位 */
  tmpreg &= CR1_AWDCH_Reset;
  /* 设置模拟看门狗通道选择位 */
  tmpreg |= ADC_Channel;
  /* 存储寄存器的新值 */
  ADCx->CR1 = tmpreg;
}

/**
  * @简述  使能或者失能温度传感器和内部参考电压通道.
  * @参数  NewState: 温度传感器和内部参考电压通道的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能温度传感器和内部参考电压通道 */
    ADC1->CR2 |= CR2_TSVREFE_Set;
  }
  else
  {
    /* 失能温度传感器和内部参考电压通道 */
    ADC1->CR2 &= CR2_TSVREFE_Reset;
  }
}

/**
  * @简述  检查制定 ADC 标志位置1与否.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_FLAG: 指定需要检查的标志. 
  *          这个参数可以使用下述值的一个:
  *          ADC_FLAG_AWD:   模拟看门狗标志位
  *          ADC_FLAG_EOC:   转换结束标志位
  *          ADC_FLAG_JEOC:  注入组转换结束标志位
  *          ADC_FLAG_JSTRT: 注入组转换开始标志位
  *          ADC_FLAG_STRT:  规则组转换开始标志位
  * @返回  ADC_FLAG 的新状态 (SET 或 RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
  /* 检查指定ADC状态位 */
  if ((ADCx->SR & ADC_FLAG) != (uint8_t)RESET)
  {
    /* 设置 ADC_FLAG 位 */
    bitstatus = SET;
  }
  else
  {
    /* 复位 ADC_FLAG 位 */
    bitstatus = RESET;
  }
  /* 返回 ADC_FLAG 的状态 */
  return  bitstatus;
}

/**
  * @简述  清除 ADCx 的待处理标志位.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_FLAG: 待处理的标志位. 
  *          这个参数可以是下列值的组合:
  *          ADC_FLAG_AWD:   模拟看门狗标志位
  *          ADC_FLAG_EOC:   转换结束标志位
  *          ADC_FLAG_JEOC:  注入组转换结束标志位
  *          ADC_FLAG_JSTRT: 注入组转换开始标志位
  *          ADC_FLAG_STRT:  规则组转换开始标志位
  * @返回  没有
  */
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
  /* 清除指定的 ADC 标志 */
  ADCx->SR = ~(uint32_t)ADC_FLAG;
}

/**
  * @简述  检查指定的ADC中断是否发生.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_IT: 需要检查的ADC中断源. 
  *          这个参数可以使用下述值的一个:
  *          ADC_IT_EOC:  转换结束中断屏蔽
  *          ADC_IT_AWD:  模拟看门狗中断屏蔽
  *          ADC_IT_JEOC: 注入组转换结束中断屏蔽
  * @返回  ADC_IT 的新状态 (SET 或 RESET).
  */
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t itmask = 0, enablestatus = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));
  /* 得到 ADC IT 中断号 */
  itmask = ADC_IT >> 8;
  /* 设置 ADC_IT 允许位状态 */
  enablestatus = (ADCx->CR1 & (uint8_t)ADC_IT) ;
  /* 检查指定的 ADC 中断 */
  if (((ADCx->SR & itmask) != (uint32_t)RESET) && enablestatus)
  {
    /* 设置 ADC_IT */
    bitstatus = SET;
  }
  else
  {
    /* 复位 ADC_IT */
    bitstatus = RESET;
  }
  /* 返回 ADC_IT 的状态 */
  return  bitstatus;
}

/**
  * @简述  清除 ADCx 的中断待处理位.
  * @参数  ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
  * @参数  ADC_IT: 带清除的 ADC 中断待处理位.
  *          可以使用下述值的一个或者几个值的组合:
  *          ADC_IT_EOC:  转换结束中断
  *          ADC_IT_AWD:  模拟看门狗中断
  *          ADC_IT_JEOC: 注入组转换结束中断
  * @返回  没有
  */
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
  uint8_t itmask = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_IT(ADC_IT));
  /* 得到 ADC IT 中断号 */
  itmask = (uint8_t)(ADC_IT >> 8);
  /* 清除选择的ADC中断挂起位 */
  ADCx->SR = ~(uint32_t)itmask;
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
