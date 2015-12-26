/**
  ******************************************************************************
  * @文件    misc.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件提供给所有其他的固件函数 (追加 CMSIS<微控制器软件接口标准> 函数).
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    02/07/2009
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
#include "misc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

/* 自用宏 ------------------------------------------------------------*/
/* 自用变量 ----------------------------------------------------------*/

/* 自用函数原型 ------------------------------------------------------*/
/* 自用函数 ----------------------------------------------------------*/


/**
  * @简述  设置优先级分组：先占优先级和从优先级.
  * @参数  NVIC_PriorityGroup: 优先级分组位长度. 
  *                            这个参数可以取下列值中的一个:
  *          NVIC_PriorityGroup_0: 先占优先级0位
  *                                从优先级4位
  *          NVIC_PriorityGroup_1: 先占优先级1位
  *                                从优先级3位
  *          NVIC_PriorityGroup_2: 先占优先级2位
  *                                从优先级2位
  *          NVIC_PriorityGroup_3: 先占优先级3位
  *                                从优先级1位
  *          NVIC_PriorityGroup_4: 先占优先级4位
  *                                从优先级0位
  * @返回  没有
  */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  /* 检查参数 */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* 依照NVIC_PriorityGroup的值设置PRIGROUP[10:8]的位 */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/**
  * @简述  通过NVIC_InitStruct中的指定参数来初始化外围设备.
  * @参数  NVIC_InitStruct: 一个指向包含指定 NVIC 外围设备配置信息的 NVIC_InitTypeDef 结构的指针.
  * @返回  没有
  */
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* 计算符合 IRQ(中断请求) 优先权 --------------------------------*/    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;
    
    /* 使能选择的 IRQ(中断请求) 通道 --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* 失能选择的 IRQ(中断请求) 通道 -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}

/**
  * @简述  设置向量表的位置和偏移量.
  * @参数  NVIC_VectTab: 指定向量表在 RAM 或 FLASH .
  *                      这个参数可以取下列值中的一个:
  *                      NVIC_VectTab_RAM
  *                      NVIC_VectTab_FLASH
  * @参数  Offset: 向量表基地址的偏移量. 这个值必须是 0x100 的整倍数.
  * @返回  没有
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* 检查参数 */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/**
  * @简述  选择系统进入低功耗模式的条件.
  * @参数  LowPowerMode: 系统为进入低功耗模式的新模式.
  *                      这个参数可以取下面的值之一:
  *                      NVIC_LP_SEVONPEND
  *                      NVIC_LP_SLEEPDEEP
  *                      NVIC_LP_SLEEPONEXIT
  * @参数  NewState: LP 条件的新状态.这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_NVIC_LP(LowPowerMode));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (uint32_t)(~(uint32_t)LowPowerMode);
  }
}

/**
  * @简述  设置 SysTick 时钟源.
  * @参数  SysTick_CLKSource: 指定 SysTick 时钟源.
  *                           这个参数可以取下面的值之一:
  *          SysTick_CLKSource_HCLK_Div8: SysTick 时钟源为AHB时钟的1/8.
  *          SysTick_CLKSource_HCLK:      SysTick 时钟源为AHB时钟.
  * @返回  没有
  */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
  /* 检查参数 */
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
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
