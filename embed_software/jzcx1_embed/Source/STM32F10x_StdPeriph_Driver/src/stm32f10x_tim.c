/**
  ******************************************************************************
  * @文件    stm32f10x_tim.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 TIM 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    10/07/2009
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
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* ---------------------- TIM registers bit mask ------------------------ */
#define CR1_CEN_Set                 ((uint16_t)0x0001)
#define CR1_CEN_Reset               ((uint16_t)0x03FE)
#define CR1_UDIS_Set                ((uint16_t)0x0002)
#define CR1_UDIS_Reset              ((uint16_t)0x03FD)
#define CR1_URS_Set                 ((uint16_t)0x0004)
#define CR1_URS_Reset               ((uint16_t)0x03FB)
#define CR1_OPM_Reset               ((uint16_t)0x03F7)
#define CR1_CounterMode_Mask        ((uint16_t)0x038F)
#define CR1_ARPE_Set                ((uint16_t)0x0080)
#define CR1_ARPE_Reset              ((uint16_t)0x037F)
#define CR1_CKD_Mask                ((uint16_t)0x00FF)
#define CR2_CCPC_Set                ((uint16_t)0x0001)
#define CR2_CCPC_Reset              ((uint16_t)0xFFFE)
#define CR2_CCUS_Set                ((uint16_t)0x0004)
#define CR2_CCUS_Reset              ((uint16_t)0xFFFB)
#define CR2_CCDS_Set                ((uint16_t)0x0008)
#define CR2_CCDS_Reset              ((uint16_t)0xFFF7)
#define CR2_MMS_Mask                ((uint16_t)0xFF8F)
#define CR2_TI1S_Set                ((uint16_t)0x0080)
#define CR2_TI1S_Reset              ((uint16_t)0xFF7F)
#define CR2_OIS1_Reset              ((uint16_t)0x7EFF)
#define CR2_OIS1N_Reset             ((uint16_t)0x7DFF)
#define CR2_OIS2_Reset              ((uint16_t)0x7BFF)
#define CR2_OIS2N_Reset             ((uint16_t)0x77FF)
#define CR2_OIS3_Reset              ((uint16_t)0x6FFF)
#define CR2_OIS3N_Reset             ((uint16_t)0x5FFF)
#define CR2_OIS4_Reset              ((uint16_t)0x3FFF)
#define SMCR_SMS_Mask               ((uint16_t)0xFFF8)
#define SMCR_ETR_Mask               ((uint16_t)0x00FF)
#define SMCR_TS_Mask                ((uint16_t)0xFF8F)
#define SMCR_MSM_Reset              ((uint16_t)0xFF7F)
#define SMCR_ECE_Set                ((uint16_t)0x4000)
#define CCMR_CC13S_Mask             ((uint16_t)0xFFFC)
#define CCMR_CC24S_Mask             ((uint16_t)0xFCFF)
#define CCMR_TI13Direct_Set         ((uint16_t)0x0001)
#define CCMR_TI24Direct_Set         ((uint16_t)0x0100)
#define CCMR_OC13FE_Reset           ((uint16_t)0xFFFB)
#define CCMR_OC24FE_Reset           ((uint16_t)0xFBFF)
#define CCMR_OC13PE_Reset           ((uint16_t)0xFFF7)
#define CCMR_OC24PE_Reset           ((uint16_t)0xF7FF)
#define CCMR_OC13M_Mask             ((uint16_t)0xFF8F)
#define CCMR_OC24M_Mask             ((uint16_t)0x8FFF) 
#define CCMR_OC13CE_Reset           ((uint16_t)0xFF7F)
#define CCMR_OC24CE_Reset           ((uint16_t)0x7FFF)
#define CCMR_IC13PSC_Mask           ((uint16_t)0xFFF3)
#define CCMR_IC24PSC_Mask           ((uint16_t)0xF3FF)
#define CCMR_IC13F_Mask             ((uint16_t)0xFF0F)
#define CCMR_IC24F_Mask             ((uint16_t)0x0FFF)
#define CCMR_Offset                 ((uint16_t)0x0018)
#define CCER_CCE_Set                ((uint16_t)0x0001)
#define	CCER_CCNE_Set               ((uint16_t)0x0004)
#define CCER_CC1P_Reset             ((uint16_t)0xFFFD)
#define CCER_CC2P_Reset             ((uint16_t)0xFFDF)
#define CCER_CC3P_Reset             ((uint16_t)0xFDFF)
#define CCER_CC4P_Reset             ((uint16_t)0xDFFF)
#define CCER_CC1NP_Reset            ((uint16_t)0xFFF7)
#define CCER_CC2NP_Reset            ((uint16_t)0xFF7F)
#define CCER_CC3NP_Reset            ((uint16_t)0xF7FF)
#define CCER_CC1E_Set               ((uint16_t)0x0001)
#define CCER_CC1E_Reset             ((uint16_t)0xFFFE)
#define CCER_CC1NE_Reset            ((uint16_t)0xFFFB)
#define CCER_CC2E_Set               ((uint16_t)0x0010)
#define CCER_CC2E_Reset             ((uint16_t)0xFFEF)
#define CCER_CC2NE_Reset            ((uint16_t)0xFFBF)
#define CCER_CC3E_Set               ((uint16_t)0x0100)
#define CCER_CC3E_Reset             ((uint16_t)0xFEFF)
#define CCER_CC3NE_Reset            ((uint16_t)0xFBFF)
#define CCER_CC4E_Set               ((uint16_t)0x1000)
#define CCER_CC4E_Reset             ((uint16_t)0xEFFF)
#define BDTR_MOE_Set                ((uint16_t)0x8000)
#define BDTR_MOE_Reset              ((uint16_t)0x7FFF)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/

static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);

/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将外设 TIMx 寄存器重设为缺省值.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @返回  没有
  */
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
 
  if (TIMx == TIM1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);  
  }     
  else if (TIMx == TIM2)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
  }
  else if (TIMx == TIM3)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);
  }
  else if (TIMx == TIM4)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
  } 
  else if (TIMx == TIM5)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, DISABLE);
  } 
  else if (TIMx == TIM6)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, DISABLE);
  } 
  else if (TIMx == TIM7)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, DISABLE);
  } 
  else
  {
    if (TIMx == TIM8)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, DISABLE);
    }  
  }
}

/**
  * @简述  根据 TIM_TimeBaseInitStruct 中指定的参数初始化 TIMx 的时间基数单位.
  * @参数  TIMx: 这里 x 可以是 1, 2, 3, 4, 5 或 8 设定 TIM 外设.
  * @参数  TIM_TimeBaseInitStruct: 指向一个包含 TIM 时间基配置信息的 TIM_BaseInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));
  /* 选择计数器模式 ，设置时钟频率 */
  TIMx->CR1 &= CR1_CKD_Mask & CR1_CounterMode_Mask;
  TIMx->CR1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_ClockDivision |
                TIM_TimeBaseInitStruct->TIM_CounterMode;
  
  /* 设置自动装载值 */
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;
 
  /* 设置预分频器值 */
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    
  if ((((uint32_t) TIMx) == TIM1_BASE) || (((uint32_t) TIMx) == TIM8_BASE))  
  {
    /* 设置重复计数器值 */
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }

  /* 产生一个更新事件立即重新装载预分频器值 */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;          
}

/**
  * @简述  根据 TIM_OCInitStruct 中的特定参数初始化TIMx通道1.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCInitStruct: 指向一个包含特定TIMx外设的配置信息的 TIM_OCInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* 失能通道 1: 复位 CC1E 位 */
  TIMx->CCER &= CCER_CC1E_Reset;
  
  /* 得到 TIMx CCER(捕获/比较使能) 寄存器的值 */
  tmpccer = TIMx->CCER;
  /* 得到 TIMx CR2(控制寄存器2) 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 得到 TIMx CCMR1(捕获/比较模式) 寄存器的值 */
  tmpccmrx = TIMx->CCMR1;
    
  /* 复位输出比较模式位 */
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* 选择输出比较模式 */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* 复位输出极性等级 */
  tmpccer &= CCER_CC1P_Reset;
  /* 设置输出比较极性 */
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* 设置输出状态 */
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
  
  /* 设置捕获比较寄存器值 */
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* 复位输出 N 极性等级 */
    tmpccer &= CCER_CC1NP_Reset;
    /* 设置输出 N 极性 */
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;
    /* 复位输出 N 状态 */
    tmpccer &= CCER_CC1NE_Reset;
    
    /* 设置输出 N 状态 */
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;
    /* 复位输出比较，输出比较 N 空闲状态 */
    tmpcr2 &= CR2_OIS1_Reset;
    tmpcr2 &= CR2_OIS1N_Reset;
    /* 设置输出空闲状态 */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;
    /* 设置输出 N 空闲状态 */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* 写 TIMx CR2(控制寄存器2) */
  TIMx->CR2 = tmpcr2;
  
  /* 写 TIMx CCMR1(捕获/比较模式寄存器1) */
  TIMx->CCMR1 = tmpccmrx;
  
  /* 写 TIMx CCER(捕获/比较使能寄存器) */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  根据 TIM_OCInitStruct 中的特定参数初始化TIMx通道2.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCInitStruct: 指向一个包含特定TIMx外设的配置信息的 TIM_OCInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* 失能通道 2: 复位 CC2E 位 */
  TIMx->CCER &= CCER_CC2E_Reset;
  
  /* 得到 TIMx CCER(捕获/比较使能) 寄存器的值 */  
  tmpccer = TIMx->CCER;
  /* 得到 TIMx CR2(控制寄存器2) 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 得到 TIMx CCMR1(捕获/比较模式) 寄存器的值 */
  tmpccmrx = TIMx->CCMR1;
    
  /* 复位输出比较模式位 */
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* 选择输出比较模式 */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* 复位输出极性等级 */
  tmpccer &= CCER_CC2P_Reset;
  /* 设置输出比较极性 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* 设置输出状态 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 4);
  
  /* 设置捕获比较寄存器值 */
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* 复位输出 N 极性等级 */
    tmpccer &= CCER_CC2NP_Reset;
    /* 设置输出 N 极性 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 4);
    /* 复位输出 N 状态 */
    tmpccer &= CCER_CC2NE_Reset;
    
    /* 设置输出 N 状态 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 4);
    /* 复位输出比较，输出比较 N 空闲状态 */
    tmpcr2 &= CR2_OIS2_Reset;
    tmpcr2 &= CR2_OIS2N_Reset;
    /* 设置输出空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 2);
    /* 设置输出 N 空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
  }
  /* 写 TIMx CR2(控制寄存器2) */
  TIMx->CR2 = tmpcr2;
  
  /* 写 TIMx CCMR1(捕获/比较模式寄存器1) */
  TIMx->CCMR1 = tmpccmrx;
  
  /* 写 TIMx CCER(捕获/比较使能寄存器) */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  根据 TIM_OCInitStruct 中的特定参数初始化TIMx通道3.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCInitStruct: 指向一个包含特定TIMx外设的配置信息的 TIM_OCInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* 失能 3: 复位 CC3E 位 */
  TIMx->CCER &= CCER_CC3E_Reset;
  
  /* 得到 TIMx CCER(捕获/比较使能) 寄存器的值 */
  tmpccer = TIMx->CCER;
  /* 得到 TIMx CR2(控制寄存器2) 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 得到 TIMx CCMR2(捕获/比较模式) 寄存器的值 */
  tmpccmrx = TIMx->CCMR2;
    
  /* 复位输出比较模式位 */
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* 选择输出比较模式 */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* 复位输出极性等级 */
  tmpccer &= CCER_CC3P_Reset;
  /* 设置输出比较极性 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* 设置输出状态 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 8);
  
  /* 设置捕获比较寄存器值 */
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* 复位输出 N 极性等级 */
    tmpccer &= CCER_CC3NP_Reset;
    /* 设置输出 N 极性 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 8);
    /* 复位输出 N 状态 */
    tmpccer &= CCER_CC3NE_Reset;
    
    /* 设置输出 N 状态 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 8);
    /* 复位输出比较，输出比较 N 空闲状态 */
    tmpcr2 &= CR2_OIS3_Reset;
    tmpcr2 &= CR2_OIS3N_Reset;
    /* 设置输出空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 4);
    /* 设置输出 N 空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
  }
  /* 写 TIMx CR2(控制寄存器2) */
  TIMx->CR2 = tmpcr2;
  
  /* 写 TIMx CCMR2(捕获/比较模式寄存器2) */
  TIMx->CCMR2 = tmpccmrx;
  
  /* 写 TIMx CCER(捕获/比较使能寄存器) */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  根据 TIM_OCInitStruct 中的特定参数初始化TIMx通道4.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCInitStruct: 指向一个包含特定TIMx外设的配置信息的 TIM_OCInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* 失能通道 4: 复位 CC4E 位 */
  TIMx->CCER &= CCER_CC4E_Reset;
  
  /* 得到 TIMx CCER(捕获/比较使能) 寄存器的值 */
  tmpccer = TIMx->CCER;
  /* 得到 TIMx CR2(控制寄存器2) 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 得到 TIMx CCMR2(捕获/比较模式) 寄存器的值 */
  tmpccmrx = TIMx->CCMR2;
    
  /* 复位输出比较模式位 */
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* 选择输出比较模式 */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* 复位输出极性等级 */
  tmpccer &= CCER_CC4P_Reset;
  /* 设置输出比较极性 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* 设置输出状态 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 12);
  
  /* 设置捕获比较寄存器值 */
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    /* 复位输出比较空闲状态 */
    tmpcr2 &= CR2_OIS4_Reset;
    /* 设置输出空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 6);
  }
  /* 写 TIMx CR2(控制寄存器2) */
  TIMx->CR2 = tmpcr2;
  
  /* 写 TIMx CCMR2(捕获/比较模式寄存器2) */  
  TIMx->CCMR2 = tmpccmrx;
  
  /* 写 TIMx CCER(捕获/比较使能寄存器) */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  根据 TIM_ ICInitStruct 中的特定参数初始化 TIMx.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICInitStruct: 指向一个包含特定 TIMx 外设的配置信息的 TIM_OCInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 配置 */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    /* TI2 配置 */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    /* TI3 配置 */
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    /* TI4 配置 */
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @简述  依照TIM_ICInitStruct指定的参数设置TIM设备去配置外部的PWM信号.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICInitStruct: 指向一个包含特定 TIMx 外设的配置信息的 TIM_ICInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  uint16_t icoppositepolarity = TIM_ICPolarity_Rising;
  uint16_t icoppositeselection = TIM_ICSelection_DirectTI;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 选择相反的输入极性 */
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }
  /* 选择相反的输入 */
  if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI)
  {
    icoppositeselection = TIM_ICSelection_IndirectTI;
  }
  else
  {
    icoppositeselection = TIM_ICSelection_DirectTI;
  }
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 配置 */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI2 配置 */
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 配置 */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI1 配置 */
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕捉预分频器值 */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @简述  配置: 设置刹车特性，死区时间，锁电平，OSSI，OSSR状态和AOE(自动输出使能).
  * @参数  TIMx: 此处x可以是1到8以选择外设
  * @参数  TIM_BDTRInitStruct: 指向一个包含特定 TIMx 外设的配置信息的 TIM_BDTRInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
  assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
  assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
  assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
  assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));
  /* 配置：锁电平级别，间断允许位和极性，OSSR状态，OSSI状态，滞后时间和自动输出使能位 */
  TIMx->BDTR = (uint32_t)TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
             TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
             TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
             TIM_BDTRInitStruct->TIM_AutomaticOutput;
}

/**
  * @简述  把 TIM1_TimeBaseInitStruct 中的每一个参数按缺省值填入.
  * @参数  TIM_TimeBaseInitStruct : 指向一个将被初始化的 TIM_TimeBaseInitTypeDef 结构的指针.
  * @返回  没有
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* 设置成默认配置 */
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

/**
  * @简述  把 TIM1_OCInitStruct 中的每一个参数按缺省值填入.
  * @参数  TIM_OCInitStruct : 指向一个将被初始化的 TIM_OCInitStruct 结构的指针.
  * @返回  没有
  */
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* 设置成默认配置 */
  TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct->TIM_Pulse = 0x0000;
  TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

/**
  * @简述  把 TIM1_ICInitStruct 中的每一个参数按缺省值填入.
  * @参数  TIM_ICInitStruct : 指向一个将被初始化的 TIM_ICInitStruct 结构的指针.
  * @返回  没有
  */
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* 设置成默认配置 */
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/**
  * @简述  把 TIM1_BDTRInitStruct 中的每一个参数按缺省值填入.
  * @参数  TIM_BDTRInitStruct: 指向一个将被初始化的 TIM1_BDTRInitStruct 结构体.
  * @返回  没有
  */
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
{
  /* 设置成默认配置 */
  TIM_BDTRInitStruct->TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct->TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct->TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct->TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct->TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct->TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

/**
  * @简述  使能或者失能指定的 TIM 外设.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  NewState: TIMx 外设的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能 TIM 计数器 */
    TIMx->CR1 |= CR1_CEN_Set;
  }
  else
  {
    /* 失能 TIM 计数器 */
    TIMx->CR1 &= CR1_CEN_Reset;
  }
}

/**
  * @简述  使能或失能 TIM 外围主输出.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  NewState: TIM 外围主输出的新状态.
  *                  这个参数可以是: ENABLE or DISABLE.
  * @返回  没有
  */
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能 TIM 主输出 */
    TIMx->BDTR |= BDTR_MOE_Set;
  }
  else
  {
    /* 失能 TIM 主输出 */
    TIMx->BDTR &= BDTR_MOE_Reset;
  }  
}

/**
  * @简述  使能或失能指定的 TIM 中断.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_IT: 将被使能或失能的 TIM 中断源.
  *                这个参数可以取下列值之一:
  *                TIM_IT_Update:  TIM 更新中断源
  *                TIM_IT_CC1:     TIM 获/比较 1 中断源
  *                TIM_IT_CC2:     TIM 获/比较 2 中断源
  *                TIM_IT_CC3:     TIM 获/比较 3 中断源
  *                TIM_IT_CC4:     TIM 获/比较 4 中断源
  *                TIM_IT_COM:     TIM COM中断源
  *                TIM_IT_Trigger: TIM 触发中断源
  *                TIM_IT_Break:   TIM 刹车中断源
  * @注解 
  *   - TIM6 和 TIM7 只能产生校正中断. 
  *   - TIM_IT_COM 和 TIM_IT_Break 只能和 TIM1 和 TIM8 一起使用.  
  * @参数  NewState: TIM 中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)
{  
  /* 检查参数s */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能中断源 */
    TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* 失能中断源 */
    TIMx->DIER &= (uint16_t)~TIM_IT;
  }
}

/**
  * @简述  配置将由软件引发的 TIM 事件.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_EventSource: 指定事件源.
  *                         这个参数可以取下列一个或更多的值:	   
  *          TIM_EventSource_Update: 定时器更新事件源
  *          TIM_EventSource_CC1:     定时器捕捉比较 1 事件源
  *          TIM_EventSource_CC2:     定时器捕捉比较 2 事件源
  *          TIM_EventSource_CC3:     定时器捕捉比较 3 事件源
  *          TIM_EventSource_CC4:     定时器捕捉比较 4 事件源
  *          TIM_EventSource_COM:     定时器 COM 事件源  
  *          TIM_EventSource_Trigger: 定时器触发器事件源
  *          TIM_EventSource_Break:   定时器刹车事件源
  * @注解 
  *   - TIM6 和 TIM7 只能产生校正中断. 
  *   - TIM_IT_COM 和 TIM_IT_Break 只能和 TIM1 和 TIM8 一起使用. 
  * @返回  没有
  */
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource)
{ 
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));
  
  /* 设置事件源 */
  TIMx->EGR = TIM_EventSource;
}

/**
  * @简述  配置 TIMx 的DMA接口.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_DMABase: DMA 基地址.
  *                     这个参数可以取下面的值之一:
  *   TIM_DMABase_CR, TIM_DMABase_CR2, TIM_DMABase_SMCR,
  *   TIM_DMABase_DIER, TIM1_DMABase_SR, TIM_DMABase_EGR,
  *   TIM_DMABase_CCMR1, TIM_DMABase_CCMR2, TIM_DMABase_CCER,
  *   TIM_DMABase_CNT, TIM_DMABase_PSC, TIM_DMABase_ARR,
  *   TIM_DMABase_RCR, TIM_DMABase_CCR1, TIM_DMABase_CCR2,
  *   TIM_DMABase_CCR3, TIM_DMABase_CCR4, TIM_DMABase_BDTR,
  *   TIM_DMABase_DCR.
  * @参数  TIM_DMABurstLength: DMA 脉冲串长度.
  *                            这个参数可以取下面两个值之一:
  *        TIM_DMABurstLength_1Byte 和 TIM_DMABurstLength_18Bytes.
  * @返回  没有
  */
void TIM_DMAConfig(TIM_TypeDef* TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
  assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));
  /* 设置 DMA 基地址和 DMA 脉冲串长度 */
  TIMx->DCR = TIM_DMABase | TIM_DMABurstLength;
}

/**
  * @简述  使能或失能 TIMx 的 DMA 请求.
  * @参数  TIMx: 此处x可以是1到8以选择外设. 
  * @参数  TIM_DMASource: 指定 DMA 请求来源.
  *                       这个参数可以是下面任何值的组合:
  *                       TIM_DMA_Update:  TIM 更新中断源
  *                       TIM_DMA_CC1:     TIM 捕获比较 1 DMA 源
  *                       TIM_DMA_CC2:     TIM 捕获比较 2 DMA 源
  *                       TIM_DMA_CC3:     TIM 捕获比较 3 DMA 源
  *                       TIM_DMA_CC4:     TIM 捕获比较 4 DMA 源
  *                       TIM_DMA_COM:     TIM COM DMA 源
  *                       TIM_DMA_Trigger: TIM 触发 DMA 源
  * @参数  NewState: DMA 请求新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_DMACmd(TIM_TypeDef* TIMx, uint16_t TIM_DMASource, FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能 DMA 源 */
    TIMx->DIER |= TIM_DMASource; 
  }
  else
  {
    /* 失能 DMA 源 */
    TIMx->DIER &= (uint16_t)~TIM_DMASource;
  }
}

/**
  * @简述  配置 TIMx 内部时钟
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @返回  没有
  */
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 失能内部时钟预分频器从模式 */
  TIMx->SMCR &=  SMCR_SMS_Mask;
}

/**
  * @简述  配置 TIMx 内部触发器为外部时钟模式
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ITRSource: 触发器.
  *                       这个参数可以取下面的值之一:
  *                       TIM_TS_ITR0: 内部触发器 0
  *                       TIM_TS_ITR1: 内部触发器 1
  *                       TIM_TS_ITR2: 内部触发器 2
  *                       TIM_TS_ITR3: 内部触发器 3
  * @返回  没有
  */
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* 选择内部触发器 */
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);
  /* 选择外部时钟模式1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @简述  设置 TIMx 触发为外部时钟
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_TIxExternalCLKSource: 触发器.
  *          这个参数可以取下面的值之一:
  *          TIM_TIxExternalCLK1Source_TI1ED: TI1 边缘检测器
  *          TIM_TIxExternalCLK1Source_TI1: 已筛选的定时器输入 1
  *          TIM_TIxExternalCLK1Source_TI2: 已筛选的定时器输入 2
  * @参数  TIM_ICPolarity: 指定 TIx 极性.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @参数  ICFilter : 指定输入捕获过滤器.
  *                   该参数可以是0x0到0xF之间的值.
  * @返回  没有
  */
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));
  /* 配置定时器输入时钟源 */
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  /* 选择触发器 */
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);
  /* 选择外部时钟模式1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @简述  配置外部时钟模式1
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ExtTRGPrescaler: 外部触发器预分频数.
  *          这个参数可以取下面的值之一:
  *          TIM_ExtTRGPSC_OFF:  ETRP 预分频器 关.
  *          TIM_ExtTRGPSC_DIV2: ETRP 频率除以 2.
  *          TIM_ExtTRGPSC_DIV4: ETRP 频率除以 4.
  *          TIM_ExtTRGPSC_DIV8: ETRP 频率除以 8.
  * @参数  TIM_ExtTRGPolarity: 外部触发器极性.
  *          这个参数可以取下面的值之一:
  *          TIM_ExtTRGPolarity_Inverted:    低电平或下降沿.
  *          TIM_ExtTRGPolarity_NonInverted: 高电平或上升沿.
  * @参数  ExtTRGFilter: 外部触发器过滤器.
  *          该参数可以是0x0到0xF之间的值
  * @返回  没有
  */ 
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  /* 配置 ETR 时钟源 */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* 得到 TIMx SMCR(从模式控制) 寄存器的值 */
  tmpsmcr = TIMx->SMCR;
  /* 复位 SMS(从模式选择) 位 */
  tmpsmcr &= SMCR_SMS_Mask;
  /* 选择外部时钟模式1 */
  tmpsmcr |= TIM_SlaveMode_External1;
  /* 选择触发器 ，选择 : ETRF */
  tmpsmcr &= SMCR_TS_Mask;
  tmpsmcr |= TIM_TS_ETRF;
  /* 写 TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @简述  配置外部时钟模式2
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ExtTRGPrescaler: 外部触发器预分频数.
  *          它可以是下面的值之一:
  *          TIM_ExtTRGPSC_OFF:  ETRP 预分频器 关.
  *          TIM_ExtTRGPSC_DIV2: ETRP 频率除以 2.
  *          TIM_ExtTRGPSC_DIV4: ETRP 频率除以 4.
  *          TIM_ExtTRGPSC_DIV8: ETRP 频率除以 8.
  * @参数  TIM_ExtTRGPolarity: 外部触发器极性.
  *          它可以是下面的值之一:
  *          TIM_ExtTRGPolarity_Inverted:    低电平或下降沿.
  *          TIM_ExtTRGPolarity_NonInverted: 高电平或上升沿.
  * @参数  ExtTRGFilter: 外部触发器过滤器.
  *          该参数可以是0x0到0xF之间的值
  * @返回  没有
  */
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  /* 配置 ETR 时钟源 */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  /* 使能外部时钟模式2 */
  TIMx->SMCR |= SMCR_ECE_Set;
}

/**
  * @简述  配置 TIMx 外部触发 (ETR).
  * @简述  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ExtTRGPrescaler: 外部触发器预分频数.
  *          它可以是下面的值之一:
  *          TIM_ExtTRGPSC_OFF:  ETRP 预分频器 关.
  *          TIM_ExtTRGPSC_DIV2: ETRP 频率除以 2.
  *          TIM_ExtTRGPSC_DIV4: ETRP 频率除以 4.
  *          TIM_ExtTRGPSC_DIV8: ETRP 频率除以 8.
  * @参数  TIM_ExtTRGPolarity: 外部触发器极性.
  *          它可以是下面的值之一:
  *          TIM_ExtTRGPolarity_Inverted:    低电平或下降沿.
  *          TIM_ExtTRGPolarity_NonInverted: 高电平或上升沿.
  * @参数  ExtTRGFilter: 外部触发器过滤器.
  *          该参数可以是0x0到0xF之间的值
  * @返回  没有
  */
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  tmpsmcr = TIMx->SMCR;
  /* 复位 ETR 位 */
  tmpsmcr &= SMCR_ETR_Mask;
  /* 设置预分频器，过滤器值和极性 */
  tmpsmcr |= (uint16_t)(TIM_ExtTRGPrescaler | (uint16_t)(TIM_ExtTRGPolarity | (uint16_t)(ExtTRGFilter << (uint16_t)8)));
  /* 写TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @简述  配置 TIMx 预分频器.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  Prescaler: 指定预分频数的新值
  * @参数  TIM_PSCReloadMode: 指定预分频数重载模式
  *          这个参数可以取下面的值之一:
  *          TIM_PSCReloadMode_Update:    TIM 预分频值在更新事件装入.
  *          TIM_PSCReloadMode_Immediate: TIM 预分频值即时装入.
  * @返回  没有
  */
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));
  /* 设置预分频器值 */
  TIMx->PSC = Prescaler;
  /* 设置 或 复位 UG 位 */
  TIMx->EGR = TIM_PSCReloadMode;
}

/**
  * @简述  指定 TIMx 计数器模式.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_CounterMode: 待使用的计数器模式
  *          这个参数可以取下面的值之一:
  *          TIM_CounterMode_Up:             TIM 向上计数模式
  *          TIM_CounterMode_Down:           TIM 向下计数模式
  *          TIM_CounterMode_CenterAligned1: TIM 中心对齐模式1
  *          TIM_CounterMode_CenterAligned2: TIM 中心对齐模式2
  *          TIM_CounterMode_CenterAligned3: TIM 中心对齐模式3
  * @返回  没有
  */
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode)
{
  uint16_t tmpcr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));
  tmpcr1 = TIMx->CR1;
  /* 复位 CMS 和 DIR 位 */
  tmpcr1 &= CR1_CounterMode_Mask;
  /* 设置计数模式 */
  tmpcr1 |= TIM_CounterMode;
  /* 写 TIMx CR1 寄存器 */
  TIMx->CR1 = tmpcr1;
}

/**
  * @简述  选择输入触发源
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_InputTriggerSource: 输入触发源.
  *          这个参数可以取下面的值之一:
  *          TIM_TS_ITR0:    内部触发器 0
  *          TIM_TS_ITR1:    内部触发器 1
  *          TIM_TS_ITR2:    内部触发器 2
  *          TIM_TS_ITR3:    内部触发器 3
  *          TIM_TS_TI1F_ED: TI1 边缘检测其
  *          TIM_TS_TI1FP1:  过滤器时钟输入 1
  *          TIM_TS_TI2FP2:  过滤器时钟输入 2
  *          TIM_TS_ETRF:    外部触发输入
  * @返回  没有
  */
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  uint16_t tmpsmcr = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* 得到 TIMx SMCR 寄存器值 */
  tmpsmcr = TIMx->SMCR;
  /* 复位 TS 位 */
  tmpsmcr &= SMCR_TS_Mask;
  /* 设置输入触发源 */
  tmpsmcr |= TIM_InputTriggerSource;
  /* 写 TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @简述  配置 TIMx 编码器界面.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_EncoderMode: 指定 TIMx 编码器模式.
  *          这个参数可以取下面的值之一:
  *          TIM_EncoderMode_TI1:  TIM编码器使用模式1.
  *          TIM_EncoderMode_TI2:  TIM编码器使用模式2.
  *          TIM_EncoderMode_TI12: TIM编码器使用模式3.
  * @参数  TIM_IC1Polarity: 指定 IC1 极性
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Falling: IC 下降沿.
  *          TIM_ICPolarity_Rising:  IC 上升沿.
  * @参数  TIM_IC2Polarity: 指定 IC2 极性
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Falling: IC 下降沿.
  *          TIM_ICPolarity_Rising:  IC 上升沿.
  * @返回  没有
  */
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
  uint16_t tmpsmcr = 0;
  uint16_t tmpccmr1 = 0;
  uint16_t tmpccer = 0;
    
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

  /* 得到 TIMx SMCR 寄存器的值 */
  tmpsmcr = TIMx->SMCR;

  /* 得到 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;

  /* 得到 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 设置编码器模式 */
  tmpsmcr &= SMCR_SMS_Mask;
  tmpsmcr |= TIM_EncoderMode;

  /* 选择捕获比较 1 和捕获比较 2 作为输入 */
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_CC24S_Mask;
  tmpccmr1 |= CCMR_TI13Direct_Set | CCMR_TI24Direct_Set;

  /* 设置 TI1 和 TI2 极性 */
  tmpccer &= CCER_CC1P_Reset & CCER_CC2P_Reset;
  tmpccer |= (uint16_t)(TIM_IC1Polarity | (uint16_t)(TIM_IC2Polarity << (uint16_t)4));

  /* 写 TIMx SMCR */
  TIMx->SMCR = tmpsmcr;

  /* 写 TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;

  /* 写 TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  置 TIMx 输出1为活动或者非活动电平.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ForcedAction: 输出信号的设置动作.
  *          这个参数可以取下面的值之一:
  *          TIM_ForcedAction_Active:   强制OCxREF为有效电平
  *          TIM_ForcedAction_InActive: 强制OCxREF为无效电平
  * @返回  没有
  */
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC1M 位 */
  tmpccmr1 &= CCMR_OC13M_Mask;
  /* 配置强制输出模式 */
  tmpccmr1 |= TIM_ForcedAction;
  /* 写 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  置 TIMx 输出2为活动或者非活动电平.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ForcedAction: 输出信号的设置动作.
  *          这个参数可以取下面的值之一:
  *          TIM_ForcedAction_Active:   强制OCxREF为有效电平
  *          TIM_ForcedAction_InActive: 强制OCxREF为无效电平
  * @返回  没有
  */
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC2M 位 */
  tmpccmr1 &= CCMR_OC24M_Mask;
  /* 配置强制输出模式 */
  tmpccmr1 |= (uint16_t)(TIM_ForcedAction << 8);
  /* 写 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  置 TIMx 输出3为活动或者非活动电平.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ForcedAction: 输出信号的设置动作.
  *          这个参数可以取下面的值之一:
  *          TIM_ForcedAction_Active:   强制OCxREF为有效电平
  *          TIM_ForcedAction_InActive: 强制OCxREF为无效电平
  * @返回  没有
  */
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC1M 位 */
  tmpccmr2 &= CCMR_OC13M_Mask;
  /* 配置强制输出模式 */
  tmpccmr2 |= TIM_ForcedAction;
  /* 写 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  置 TIMx 输出4为活动或者非活动电平.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ForcedAction: 输出信号的设置动作.
  *          这个参数可以取下面的值之一:
  *          TIM_ForcedAction_Active:   强制OCxREF为有效电平
  *          TIM_ForcedAction_InActive: 强制OCxREF为无效电平
  * @返回  没有
  */
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC2M 位 */
  tmpccmr2 &= CCMR_OC24M_Mask;
  /* 配置强制输出模式 */
  tmpccmr2 |= (uint16_t)(TIM_ForcedAction << 8);
  /* 写 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  使能或者失能 TIMx 在 ARR 上的预装载寄存器.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  NewState: 在TIM1_CR1寄存器中的ARPE位的新状态
  *                  这个参数可以是: ENABLE or DISABLE.
  * @返回  没有
  */
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置 ARR 的预装载位 */
    TIMx->CR1 |= CR1_ARPE_Set;
  }
  else
  {
    /* 复位 ARR 的预装载位 */
    TIMx->CR1 &= CR1_ARPE_Reset;
  }
}

/**
  * @简述  选择 TIM 外设的通讯事件.
  * @参数  TIMx: 此处x可以是1到8以选择外设
  * @参数  NewState: 通讯事件的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置 COM 位 */
    TIMx->CR2 |= CR2_CCUS_Set;
  }
  else
  {
    /* 复位 COM 位 */
    TIMx->CR2 &= CR2_CCUS_Reset;
  }
}

/**
  * @简述  选择 TIMx 外设的捕获比较 DMA 源.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  NewState: 捕获比较 DMA 源的新状态
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置 CCDS 位 */
    TIMx->CR2 |= CR2_CCDS_Set;
  }
  else
  {
    /* 复位 CCDS 位 */
    TIMx->CR2 &= CR2_CCDS_Reset;
  }
}

/**
  * @简述  设置或者复位 TIMx 捕获比较控制位.
  * @参数  TIMx: 此处x可以是1到8以选择外设
  * @参数  NewState: 捕获比较预载控制位的新状态
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置 CCPC 位 */
    TIMx->CR2 |= CR2_CCPC_Set;
  }
  else
  {
    /* 复位 CCPC 位 */
    TIMx->CR2 &= CR2_CCPC_Reset;
  }
}

/**
  * @简述  使能或者失能 TIMx 在 CCR1 上的预装载寄存器.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPreload: 输出比较预装载状态
  *                       这个参数可以取下面的值之一:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @返回  没有
  */
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC1PE 位 */
  tmpccmr1 &= CCMR_OC13PE_Reset;
  /* 使能或者失能预装载寄存器 */
  tmpccmr1 |= TIM_OCPreload;
  /* 写 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  使能或者失能 TIMx 在 CCR2 上的预装载寄存器.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPreload: 输出比较预装载状态
  *                       这个参数可以取下面的值之一:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @返回  没有
  */
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC2PE 位 */
  tmpccmr1 &= CCMR_OC24PE_Reset;
  /* 使能或者失能预装载寄存器 */
  tmpccmr1 |= (uint16_t)(TIM_OCPreload << 8);
  /* 写 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  使能或者失能 TIMx 在 CCR3 上的预装载寄存器.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPreload: 输出比较预装载状态
  *                       这个参数可以取下面的值之一:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @返回  没有
  */
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC3PE 位 */
  tmpccmr2 &= CCMR_OC13PE_Reset;
  /* 使能或者失能预装载寄存器 */
  tmpccmr2 |= TIM_OCPreload;
  /* 写 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  使能或者失能 TIMx 在 CCR4 上的预装载寄存器.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPreload: 输出比较预装载状态
  *                       这个参数可以取下面的值之一:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @返回  没有
  */
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC4PE 位 */
  tmpccmr2 &= CCMR_OC24PE_Reset;
  /* 使能或者失能预装载寄存器 */
  tmpccmr2 |= (uint16_t)(TIM_OCPreload << 8);
  /* 写 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  设置 TIMx 捕获比较1快速特征.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCFast: 输出比较快速特性新状态.
  *                    这个参数可以取下面的值之一:
  *                    TIM_OCFast_Enable:  TIM 输出比较快速性能使能
  *                    TIM_OCFast_Disable: TIM 输出比较快速性能失能
  * @返回  没有
  */
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 得到 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC1FE 位 */
  tmpccmr1 &= CCMR_OC13FE_Reset;
  /* 使能或失能捕获比较快速特征 */
  tmpccmr1 |= TIM_OCFast;
  /* 写 TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  设置 TIMx 捕获比较2快速特征.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCFast: 输出比较快速特性新状态.
  *                    这个参数可以取下面的值之一:
  *                    TIM_OCFast_Enable:  TIM 输出比较快速性能使能
  *                    TIM_OCFast_Disable: TIM 输出比较快速性能失能
  * @返回  没有
  */
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 得到 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC2FE 位 */
  tmpccmr1 &= CCMR_OC24FE_Reset;
  /* 使能或失能捕获比较快速特征 */
  tmpccmr1 |= (uint16_t)(TIM_OCFast << 8);
  /* 写 TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  设置 TIMx 捕获比较3快速特征.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCFast: 输出比较快速特性新状态.
  *                    这个参数可以取下面的值之一:
  *                    TIM_OCFast_Enable:  TIM 输出比较快速性能使能
  *                    TIM_OCFast_Disable: TIM 输出比较快速性能失能
  * @返回  没有
  */
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 得到 TIMx CCMR2 寄存器的值 */
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC3FE 位 */
  tmpccmr2 &= CCMR_OC13FE_Reset;
  /* 使能或失能捕获比较快速特征 */
  tmpccmr2 |= TIM_OCFast;
  /* 写 TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  设置 TIMx 捕获比较4快速特征.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCFast: 输出比较快速特性新状态.
  *                    这个参数可以取下面的值之一:
  *                    TIM_OCFast_Enable:  TIM 输出比较快速性能使能
  *                    TIM_OCFast_Disable: TIM 输出比较快速性能失能
  * @返回  没有
  */
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 得到 TIMx CCMR2 寄存器的值 */
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC4FE 位 */
  tmpccmr2 &= CCMR_OC24FE_Reset;
  /* 使能或失能捕获比较快速特征 */
  tmpccmr2 |= (uint16_t)(TIM_OCFast << 8);
  /* 写 TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  在一个外部事件时清除或者保持 OCREF1 信号
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCClear: 输出比较清除使能位的新状态.
  *                     这个参数可以取下面的值之一:
  *                     TIM_OCClear_Enable:  TIM 输出比较清除使能
  *                     TIM_OCClear_Disable: TIM 输出比较清除失能
  * @返回  没有
  */
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC1CE 位 */
  tmpccmr1 &= CCMR_OC13CE_Reset;
  /* 使能或失能输出比较清除位 */
  tmpccmr1 |= TIM_OCClear;
  /* 写 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  在一个外部事件时清除或者保持 OCREF2 信号
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCClear: 输出比较清除使能位的新状态.
  *                     这个参数可以取下面的值之一:
  *                     TIM_OCClear_Enable:  TIM 输出比较清除使能
  *                     TIM_OCClear_Disable: TIM 输出比较清除失能
  * @返回  没有
  */
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr1 = TIMx->CCMR1;
  /* 复位 OC2CE 位 */
  tmpccmr1 &= CCMR_OC24CE_Reset;
  /* 使能或失能输出比较清除位 */
  tmpccmr1 |= (uint16_t)(TIM_OCClear << 8);
  /* 写 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @简述  在一个外部事件时清除或者保持 OCREF3 信号
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCClear: 输出比较清除使能位的新状态.
  *                     这个参数可以取下面的值之一:
  *                     TIM_OCClear_Enable:  TIM 输出比较清除使能
  *                     TIM_OCClear_Disable: TIM 输出比较清除失能
  * @返回  没有
  */
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC3CE 位 */
  tmpccmr2 &= CCMR_OC13CE_Reset;
  /* 使能或失能输出比较清除位 */
  tmpccmr2 |= TIM_OCClear;
  /* 写 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  在一个外部事件时清除或者保持 OCREF4 信号
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCClear: 输出比较清除使能位的新状态.
  *                     这个参数可以取下面的值之一:
  *                     TIM_OCClear_Enable:  TIM 输出比较清除使能
  *                     TIM_OCClear_Disable: TIM 输出比较清除失能
  * @返回  没有
  */
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr2 = TIMx->CCMR2;
  /* 复位 OC4CE 位 */
  tmpccmr2 &= CCMR_OC24CE_Reset;
  /* 使能或失能输出比较清除位 */
  tmpccmr2 |= (uint16_t)(TIM_OCClear << 8);
  /* 写 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @简述  设置 TIMx 通道1极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPolarity: 指定的 OC1 极性
  *                        这个参数可以取下面的值之一:
  *                        TIM_OCPolarity_High: 输出比较极性高
  *                        TIM_OCPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC1P 位 */
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_OCPolarity;
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  配置 TIMx 通道 1N 极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCNPolarity: 指定 OC1N 极性
  *                         这个参数可以取下面的值之一:
  *                         TIM_OCNPolarity_High: 输出比较极性高
  *                         TIM_OCNPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
   
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC1NP 位 */
  tmpccer &= CCER_CC1NP_Reset;
  tmpccer |= TIM_OCNPolarity;
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  设置 TIMx 通道2极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPolarity: 指定的 OC2 极性
  *                        这个参数可以取下面的值之一:
  *                        TIM_OCPolarity_High: 输出比较极性高
  *                        TIM_OCPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC2P 位 */
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 4);
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  配置 TIMx 通道 2N 极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCNPolarity: 指定 OC2N 极性
  *                         这个参数可以取下面的值之一:
  *                         TIM_OCNPolarity_High: 输出比较极性高
  *                         TIM_OCNPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
  
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC2NP 位 */
  tmpccer &= CCER_CC2NP_Reset;
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 4);
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  设置 TIMx 通道3极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPolarity: 指定的 OC3 极性
  *                        这个参数可以取下面的值之一:
  *                        TIM_OCPolarity_High: 输出比较极性高
  *                        TIM_OCPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC3P 位 */
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 8);
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  配置 TIMx 通道 3N 极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCNPolarity: 指定 OC3N 极性
  *                         这个参数可以取下面的值之一:
  *                         TIM_OCNPolarity_High: 输出比较极性高
  *                         TIM_OCNPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
 
  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
    
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC3NP 位 */
  tmpccer &= CCER_CC3NP_Reset;
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 8);
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  设置 TIMx 通道4极性.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_OCPolarity: 指定的 OC4 极性
  *                        这个参数可以取下面的值之一:
  *                        TIM_OCPolarity_High: 输出比较极性高
  *                        TIM_OCPolarity_Low:  输出比较极性低
  * @返回  没有
  */
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 设置或复位 CC4P 位 */
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 12);
  /* 写 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @简述  使能或失能 TIM 捕获比较通道 x.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_Channel: 指定的 TIM 通道
  *          这个参数可以取下面的值之一:
  *          TIM_Channel_1: TIM 通道 1
  *          TIM_Channel_2: TIM 通道 2
  *          TIM_Channel_3: TIM 通道 3
  *          TIM_Channel_4: TIM 通道 4
  * @参数  TIM_CCx: 指定 TIM 通道 CCxE 位的新状态.
  *                 这个参数可以是: TIM_CCx_Enable 或 TIM_CCx_Disable. 
  * @返回  没有
  */
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
  uint16_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCX(TIM_CCx));

  tmp = CCER_CCE_Set << TIM_Channel;

  /* 复位 CCxE 位 */
  TIMx->CCER &= (uint16_t)~ tmp;

  /* 设置或复位 CCxE 位 */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCx << TIM_Channel);
}

/**
  * @简述  使能或失能 TIM 捕获比较通道 xN.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_Channel: 指定 TIM 通道
  *          这个参数可以取下面的值之一:
  *          TIM_Channel_1: TIM 通道 1
  *          TIM_Channel_2: TIM 通道 2
  *          TIM_Channel_3: TIM 通道 3
  * @参数  TIM_CCxN: 指定 TIM 通道 CCxNE 位的新状态.
  *                  这个参数可以是: TIM_CCxN_Enable 或 TIM_CCxN_Disable. 
  * @返回  没有
  */
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN)
{
  uint16_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCXN(TIM_CCxN));

  tmp = CCER_CCNE_Set << TIM_Channel;

  /* 复位 CCxNE 位 */
  TIMx->CCER &= (uint16_t) ~tmp;

  /* 设置或复位 CCxNE 位 */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCxN << TIM_Channel);
}

/**
  * @简述  选择 TIM 输出比较模式.
  * @注解   在改变输出比较模式之前，该函数失能选择的通道.
  *         用户需要使用 TIM1_CCxCmd 和 TIM1_CCxNCmd 函数使能通道.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_Channel: 指定的 TIM 通道
  *          这个参数可以取下面的值之一:
  *          TIM_Channel_1: TIM 通道 1
  *          TIM_Channel_2: TIM 通道 2
  *          TIM_Channel_3: TIM 通道 3
  *          TIM_Channel_4: TIM 通道 4
  * @参数  TIM_OCMode: 指定 TIM 输出比较模式.
  *          这个参数可以取下面的值之一:
  *          TIM_OCMode_Timing
  *          TIM_OCMode_Active
  *          TIM_OCMode_Toggle
  *          TIM_OCMode_PWM1
  *          TIM_OCMode_PWM2
  *          TIM_ForcedAction_Active
  *          TIM_ForcedAction_InActive
  * @返回  没有
  */
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
{
  uint32_t tmp = 0;
  uint16_t tmp1 = 0;

  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_OCM(TIM_OCMode));

  tmp = (uint32_t) TIMx;
  tmp += CCMR_Offset;

  tmp1 = CCER_CCE_Set << (uint16_t)TIM_Channel;

  /* 失能通道: 复位 CCxE 位 */
  TIMx->CCER &= (uint16_t) ~tmp1;

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    tmp += (TIM_Channel>>1);

    /* 复位 OCxM 位，在 CCMRx 寄存器里 */
    *(__IO uint32_t *) tmp &= CCMR_OC13M_Mask;
   
    /* 配置 OCxM 位，在 CCMRx 寄存器里 */
    *(__IO uint32_t *) tmp |= TIM_OCMode;
  }
  else
  {
    tmp += (uint16_t)(TIM_Channel - (uint16_t)4)>> (uint16_t)1;

    /* 复位 OCxM 位，在 CCMRx 寄存器里 */
    *(__IO uint32_t *) tmp &= CCMR_OC24M_Mask;
    
    /* 配置 OCxM 位，在 CCMRx 寄存器里 */
    *(__IO uint32_t *) tmp |= (uint16_t)(TIM_OCMode << 8);
  }
}

/**
  * @简述  使能或者失能 TIM 更新事件.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  NewState: TIM1_CR1寄存器UDIS位的新状态
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置更新失能位 */
    TIMx->CR1 |= CR1_UDIS_Set;
  }
  else
  {
    /* 复位更新失能位 */
    TIMx->CR1 &= CR1_UDIS_Reset;
  }
}

/**
  * @简述  配置 TIMx 更新请求源.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_UpdateSource: 指定更新来源.
  *          这个参数可以取下面的值之一:
  *          TIM_UpdateSource_Regular: 生成重复的脉冲：在更新事件时计数器不停止.
  *          TIM_UpdateSource_Global:  生成单一的脉冲：计数器在下一个更新事件停止.
  * @返回  没有
  */
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint16_t TIM_UpdateSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));
  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* 设置 URS 位 */
    TIMx->CR1 |= CR1_URS_Set;
  }
  else
  {
    /* 复位 URS 位 */
    TIMx->CR1 &= CR1_URS_Reset;
  }
}

/**
  * @简述  使能或者失能 TIM 霍尔传感器接口.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  NewState: 霍尔感应器的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 设置 TI1S 位 */
    TIMx->CR2 |= CR2_TI1S_Set;
  }
  else
  {
    /* 复位 TI1S 位 */
    TIMx->CR2 &= CR2_TI1S_Reset;
  }
}

/**
  * @简述  设置 TIM 单脉冲模式.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_OPMode: 指定 OPM 模式.
  *                    这个参数可以取下面的值之一:
  *                    TIM_OPMode_Single
  *                    TIM_OPMode_Repetitive
  * @返回  没有
  */
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));
  /* 复位 OPM 位 */
  TIMx->CR1 &= CR1_OPM_Reset;
  /* 配置 OPM 模式 */
  TIMx->CR1 |= TIM_OPMode;
}

/**
  * @简述  选择 TIM 触发输出模式.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_TRGOSource: 指定的触发输出模式.
  *                        这个参数可以是下面的值:
  *
  *  - 对于所有的 TIMx
  *          TIM_TRGOSource_Reset:  使用寄存器TIM1_EGR的UG位作为触发输出 (TRGO).
  *          TIM_TRGOSource_Enable: 使用计数器使能CEN作为触发输出 (TRGO).
  *          TIM_TRGOSource_Update: 使用更新事件作为触发输出 (TRGO).
  *
  *  - 对于所有的 TIMx 除了 TIM6 和 TIM7
  *          TIM_TRGOSource_OC1:    一旦捕获或者比较匹配发生，当标志位CC1F被设置时触发输出发送一个肯定脉冲 (TRGO).
  *          TIM_TRGOSource_OC1Ref: 使用 OC1REF 作为触发输出 (TRGO).
  *          TIM_TRGOSource_OC2Ref: 使用 OC2REF 作为触发输出 (TRGO).
  *          TIM_TRGOSource_OC3Ref: 使用 OC3REF 作为触发输出 (TRGO).
  *          TIM_TRGOSource_OC4Ref: 使用 OC4REF 作为触发输出 (TRGO).
  *
  * @返回  没有
  */
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
  /* 复位 MMS 位 */
  TIMx->CR2 &= CR2_MMS_Mask;
  /* 选择 TRGO 的来源 */
  TIMx->CR2 |=  TIM_TRGOSource;
}

/**
  * @简述  选择 TIMx 从模式.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_SlaveMode: 指定定时器从模式.
  *          这个参数可以取下面的值之一:
  *          TIM_SlaveMode_Reset:     选中触发信号 (TRGI) 的上升沿重初始化计数器并触发寄存器的更新.
  *          TIM_SlaveMode_Gated:     当触发信号 (TRGI) 为高电平计数器时钟使能.
  *          TIM_SlaveMode_Trigger:   计数器在触发 (TRGI) 的上升沿开始.
  *          TIM_SlaveMode_External1: 选中触发 (TRGI) 的上升沿作为计数器时钟.
  * @返回  没有
  */
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));
  /* 复位 SMS 位 */
  TIMx->SMCR &= SMCR_SMS_Mask;
  /* 选择从模式 */
  TIMx->SMCR |= TIM_SlaveMode;
}

/**
  * @简述  设置或复位 TIMx 主/从模式.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_MasterSlaveMode: 定时器主/从模式.
  *          这个参数可以取下面的值之一:
  *          TIM_MasterSlaveMode_Enable:  主/从模式使能.
  *          TIM_MasterSlaveMode_Disable: 主/从模式失能
  * @返回  没有
  */
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_MasterSlaveMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));
  /* 复位 MSM 位 */
  TIMx->SMCR &= SMCR_MSM_Reset;
  
  /* 设置或复位 MSM 位 */
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/**
  * @简述  设置 TIMx 计数寄存器的值
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  Counter: 指定计数寄存器的新值.
  * @返回  没有
  */
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 设置计数寄存器的值 */
  TIMx->CNT = Counter;
}

/**
  * @简述  设置 TIMx 自动重装载寄存器值
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  Autoreload: 指定自动重装载寄存器新值.
  * @返回  没有
  */
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 设置自动重装载寄存器值 */
  TIMx->ARR = Autoreload;
}

/**
  * @简述  设置 TIMx 捕获比较1寄存器值
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  Compare1: 捕获比较1寄存器新值.
  * @返回  没有
  */
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 设置捕获比较1寄存器值 */
  TIMx->CCR1 = Compare1;
}

/**
  * @简述  设置 TIMx 捕获比较2寄存器值
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  Compare2: 捕获比较2寄存器新值.
  * @返回  没有
  */
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 设置捕获比较2寄存器值 */
  TIMx->CCR2 = Compare2;
}

/**
  * @简述  设置 TIMx 捕获比较3寄存器值
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  Compare3: 捕获比较3寄存器新值.
  * @返回  没有
  */
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 设置捕获比较3寄存器值 */
  TIMx->CCR3 = Compare3;
}

/**
  * @简述  设置 TIMx 捕获比较4寄存器值
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  Compare4: 捕获比较4寄存器新值.
  * @返回  没有
  */
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 设置捕获比较4寄存器值 */
  TIMx->CCR4 = Compare4;
}

/**
  * @简述  设置 TIMx 输入捕获1预分频.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPSC: 输入捕获1预分频的新值.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPSC_DIV1: 没有预分频数
  *          TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
  * @返回  没有
  */
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 复位 IC1PSC 位 */
  TIMx->CCMR1 &= CCMR_IC13PSC_Mask;
  /* 设置 IC1PSC 的值 */
  TIMx->CCMR1 |= TIM_ICPSC;
}

/**
  * @简述  设置 TIMx 输入捕获2预分频.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPSC: 输入捕获2预分频的新值.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPSC_DIV1: 没有预分频数
  *          TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
  * @返回  没有
  */
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 复位 IC2PSC 位 */
  TIMx->CCMR1 &= CCMR_IC24PSC_Mask;
  /* 设置 IC2PSC 的值 */
  TIMx->CCMR1 |= (uint16_t)(TIM_ICPSC << 8);
}

/**
  * @简述  设置 TIMx 输入捕获3预分频.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPSC: 输入捕获3预分频的新值.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPSC_DIV1: 没有预分频数
  *          TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
  * @返回  没有
  */
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 复位 IC3PSC 位 */
  TIMx->CCMR2 &= CCMR_IC13PSC_Mask;
  /* 设置 IC3PSC 的值 */
  TIMx->CCMR2 |= TIM_ICPSC;
}

/**
  * @简述  设置 TIMx 输入捕获4预分频.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPSC: 输入捕获4预分频的新值.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPSC_DIV1: 没有预分频数
  *          TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
  *          TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
  * @返回  没有
  */
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{  
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 复位 IC4PSC 位 */
  TIMx->CCMR2 &= CCMR_IC24PSC_Mask;
  /* 设置 IC4PSC 的值 */
  TIMx->CCMR2 |= (uint16_t)(TIM_ICPSC << 8);
}

/**
  * @简述  设置 TIMx 的时钟分割值.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_CKD: 时钟分割值.
  *          这个参数可以取下面的值之一:
  *          TIM_CKD_DIV1: TDTS = Tck_tim
  *          TIM_CKD_DIV2: TDTS = 2*Tck_tim
  *          TIM_CKD_DIV4: TDTS = 4*Tck_tim
  * @返回  没有
  */
void TIM_SetClockDivision(TIM_TypeDef* TIMx, uint16_t TIM_CKD)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CKD_DIV(TIM_CKD));
  /* 复位 CKD 位 */
  TIMx->CR1 &= CR1_CKD_Mask;
  /* 设置 CKD 的值 */
  TIMx->CR1 |= TIM_CKD;
}

/**
  * @简述  获得 TIMx 输入捕获1的值.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @返回  获得输入捕获1的值.
  */
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 获得输入捕获1的值 */
  return TIMx->CCR1;
}

/**
  * @简述  获得 TIMx 输入捕获2的值.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @返回  获得输入捕获2的值.
  */
uint16_t TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 获得输入捕获2的值 */
  return TIMx->CCR2;
}

/**
  * @简述  获得 TIMx 输入捕获3的值.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @返回  获得输入捕获3的值.
  */
uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  /* 获得输入捕获3的值 */
  return TIMx->CCR3;
}

/**
  * @简述  获得 TIMx 输入捕获4的值.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @返回  获得输入捕获4的值.
  */
uint16_t TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* 获得输入捕获4的值 */
  return TIMx->CCR4;
}

/**
  * @简述  获得 TIMx 计数器的值.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @返回  计数器的值.
  */
uint16_t TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 获得计数器的值 */
  return TIMx->CNT;
}

/**
  * @简述  获得 TIMx 预分频值.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @返回  预分频的值.
  */
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 获得预分频的值 */
  return TIMx->PSC;
}

/**
  * @简述  检查指定的 TIM 标志位设置与否.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_FLAG: 待检查的 TIM 标志位.
  *          这个参数可以取下面的值之一:
  *          TIM_FLAG_Update: TIM 更新标志
  *          TIM_FLAG_CC1: TIM 捕获/比较 1 标志位
  *          TIM_FLAG_CC2: TIM 捕获/比较 2 标志位
  *          TIM_FLAG_CC3: TIM 捕获/比较 3 标志位
  *          TIM_FLAG_CC4: TIM 捕获/比较 4 标志位
  *          TIM_FLAG_COM: TIM Com标志
  *          TIM_FLAG_Trigger: TIM 触发标志位
  *          TIM_FLAG_Break: TIM 休息标志
  *          TIM_FLAG_CC1OF: TIM 捕获/比较 1 溢出标志位
  *          TIM_FLAG_CC2OF: TIM 捕获/比较 2 溢出标志位
  *          TIM_FLAG_CC3OF: TIM 捕获/比较 3 溢出标志位
  *          TIM_FLAG_CC4OF: TIM 捕获/比较 4 溢出标志位
  * @注解
  *   - TIM6 和 TIM7 仅仅有一个更新中断. 
  *   - TIM_FLAG_COM 和 TIM_FLAG_Break 仅仅可以和 TIM1 和 TIM8 一起使用.    
  * @返回  TIM_FLAG 的新状态(SET 或 RESET).
  */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_FLAG(TIM_FLAG));
  
  if ((TIMx->SR & TIM_FLAG) != (uint16_t)RESET)
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
  * @简述  清除 TIMx 的待处理标志位.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_FLAG: 用于清除的标记.
  *          这个参数可以是下面值的任意组合:
  *          TIM_FLAG_Update: TIM 更新标志
  *          TIM_FLAG_CC1: TIM 捕获/比较 1 标志位
  *          TIM_FLAG_CC2: TIM 捕获/比较 2 标志位
  *          TIM_FLAG_CC3: TIM 捕获/比较 3 标志位
  *          TIM_FLAG_CC4: TIM 捕获/比较 4 标志位
  *          TIM_FLAG_COM: TIM Com标志
  *          TIM_FLAG_Trigger: TIM 触发标志位
  *          TIM_FLAG_Break: TIM 中断标志
  *          TIM_FLAG_CC1OF: TIM 捕获/比较 1 溢出标志位
  *          TIM_FLAG_CC2OF: TIM 捕获/比较 2 溢出标志位
  *          TIM_FLAG_CC3OF: TIM 捕获/比较 3 溢出标志位
  *          TIM_FLAG_CC4OF: TIM 捕获/比较 4 溢出标志位
  * @注解
  *   - TIM6 和 TIM7 仅仅有一个更新中断. 
  *   - TIM_FLAG_COM 和 TIM_FLAG_Break 仅仅可以和 TIM1 和 TIM8 一起使用.    
  * @返回  没有.
  */
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{  
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_CLEAR_FLAG(TIM_FLAG));
   
  /* 清除标志位 */
  TIMx->SR = (uint16_t)~TIM_FLAG;
}

/**
  * @简述  检查指定的 TIM 中断发生与否.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_IT: 待检查的 TIM 中断源.
  *          这个参数可以取下面的值之一:
  *          TIM_IT_Update: TIM 更新中断源
  *          TIM_IT_CC1: TIM 捕获/比较 1 中断源
  *          TIM_IT_CC2: TIM 捕获/比较 2 中断源
  *          TIM_IT_CC3: TIM 捕获/比较 3 中断源
  *          TIM_IT_CC4: TIM 捕获/比较 4 中断源
  *          TIM_IT_COM: TIM Com 中断源
  *          TIM_IT_Trigger: TIM 触发中断源
  *          TIM_IT_Break: TIM 休息中断源
  * @注解
  *   - TIM6 和 TIM7 仅仅有一个更新中断. 
  *   - TIM_FLAG_COM 和 TIM_FLAG_Break 仅仅可以和 TIM1 和 TIM8 一起使用.   
  * @返回  TIM_IT 的新状态(SET 或 RESET).
  */
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  ITStatus bitstatus = RESET;  
  uint16_t itstatus = 0x0, itenable = 0x0;
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_IT(TIM_IT));
   
  itstatus = TIMx->SR & TIM_IT;
  
  itenable = TIMx->DIER & TIM_IT;
  if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
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
  * @简述  清除 TIMx 的中断待处理位.
  * @参数  TIMx: 此处x可以是1到8以选择外设.
  * @参数  TIM_IT: 指定中断待处理位.
  *          这个参数可以是下面值的任意组合:
  *          TIM_IT_Update: TIM 更新中断源
  *          TIM_IT_CC1: TIM 捕获/比较 1 中断源
  *          TIM_IT_CC2: TIM 捕获/比较 2 中断源
  *          TIM_IT_CC3: TIM 捕获/比较 3 中断源
  *          TIM_IT_CC4: TIM 捕获/比较 4 中断源
  *          TIM_IT_COM: TIM Com 中断源
  *          TIM_IT_Trigger: TIM 触发中断源
  *          TIM_IT_Break: TIM 休息中断源
  * @注解
  *   - TIM6 和 TIM7 仅仅有一个更新中断. 
  *   - TIM_FLAG_COM 和 TIM_FLAG_Break 仅仅可以和 TIM1 和 TIM8 一起使用.  
  * @返回  没有
  */
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  /* 清除 TIMx 的中断待处理位 */
  TIMx->SR = (uint16_t)~TIM_IT;
}

/**
  * @简述  配置 TI1 作为输入.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPolarity : 输入极性.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @参数  TIM_ICSelection: 使用指定的输入.
  *          这个参数可以取下面的值之一:
  *          TIM_ICSelection_DirectTI:   选择TIM 输入1连接IC1.
  *          TIM_ICSelection_IndirectTI: 选择TIM 输入1连接IC2.
  *          TIM_ICSelection_TRC:        选择TIM 输入1连接TRC.
  * @参数  TIM_ICFilter: 指定的输入捕获滤波器.
  *                      这个参数的值必须在 0x00 和 0x0F之间.
  * @返回  没有
  */
static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0;
  /* 失能通道 1: 复位 CC1E 位 */
  TIMx->CCER &= CCER_CC1E_Reset;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  /* 选择输入和滤波器 */
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr1 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));
  /* 选择极性和置位CC1E */
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)CCER_CC1E_Set);
  /* 写TIMx CCMR1和CCER寄存器 */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @简述  配置 TI2 作为输入.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPolarity : 输入极性.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @参数  TIM_ICSelection: 使用指定的输入.
  *          这个参数可以取下面的值之一:
  *          TIM_ICSelection_DirectTI:   选择TIM 输入2连接IC2.
  *          TIM_ICSelection_IndirectTI: 选择TIM 输入2连接IC1.
  *          TIM_ICSelection_TRC:        选择TIM 输入2连接TRC.
  * @参数  TIM_ICFilter: 指定的输入捕获滤波器.
  *                      这个参数的值必须在 0x00 和 0x0F之间.
  * @返回  没有
  */
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;
  /* 失能通道 2: 复位 CC2E 位 */
  TIMx->CCER &= CCER_CC2E_Reset;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 4);
  /* 选择输入和滤波器 */
  tmpccmr1 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr1 |= (uint16_t)(TIM_ICFilter << 12);
  tmpccmr1 |= (uint16_t)(TIM_ICSelection << 8);
  /* 选择极性和置位CC2E */
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |=  (uint16_t)(tmp | (uint16_t)CCER_CC2E_Set);
  /* 写TIMx CCMR1和CCER寄存器 */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @简述  配置 TI3 作为输入.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPolarity : 输入极性.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @参数  TIM_ICSelection: 使用指定的输入.
  *          这个参数可以取下面的值之一:
  *          TIM_ICSelection_DirectTI:   选择TIM 输入3连接IC3.
  *          TIM_ICSelection_IndirectTI: 选择TIM 输入3连接IC4.
  *          TIM_ICSelection_TRC:        选择TIM 输入3连接TRC.
  * @参数  TIM_ICFilter: 指定的输入捕获滤波器.
  *                      这个参数的值必须在 0x00 和 0x0F之间.
  * @返回  没有
  */
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;
  /* 失能通道 3: 复位 CC3E 位 */
  TIMx->CCER &= CCER_CC3E_Reset;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 8);
  /* 选择输入和滤波器 */
  tmpccmr2 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr2 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));
  /* 选择极性和置位CC3E */
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (uint16_t)(tmp | (uint16_t)CCER_CC3E_Set);
  /* 写TIMx CCMR2和CCER寄存器 */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @简述  配置 TI4 作为输入.
  * @参数  TIMx: 此处x可以是1,2,3,4,5或8以选择外设.
  * @参数  TIM_ICPolarity : 输入极性.
  *          这个参数可以取下面的值之一:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @参数  TIM_ICSelection: 使用指定的输入.
  *          这个参数可以取下面的值之一:
  *          TIM_ICSelection_DirectTI:   选择TIM 输入3连接IC4.
  *          TIM_ICSelection_IndirectTI: 选择TIM 输入3连接IC3.
  *          TIM_ICSelection_TRC:        选择TIM 输入3连接TRC.
  * @参数  TIM_ICFilter: 指定的输入捕获滤波器.
  *                      这个参数的值必须在 0x00 和 0x0F之间.
  * @返回  没有
  */
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* 失能通道 4: 复位 CC4E 位 */
  TIMx->CCER &= CCER_CC4E_Reset;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 12);

  /* 选择输入和设置过滤器 */
  tmpccmr2 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8);
  tmpccmr2 |= (uint16_t)(TIM_ICFilter << 12);

  /* 选择极性和置位CC4E */
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (uint16_t)(tmp | (uint16_t)CCER_CC4E_Set);
  /* 写TIMx CCMR2和CCER寄存器 */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
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
