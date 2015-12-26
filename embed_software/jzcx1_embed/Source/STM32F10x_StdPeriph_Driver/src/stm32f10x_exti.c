/**
  ******************************************************************************
  * @文件    stm32f10x_exti.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件提供全部 EXTI 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    06/07/2009
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
#include "stm32f10x_exti.h"

/* 自用类型 ------------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/

#define EXTI_LineNone    ((uint32_t)0x00000)  /* No interrupt selected */

/* 自用宏 --------------------------------------------------------------*/
/* 自用变量 ------------------------------------------------------------*/
/* 自用函数原型 --------------------------------------------------------*/
/* 自用函数 ------------------------------------------------------------*/

/**
  * @简述  将 EXTI 外设寄存器复位到默认值.
  * @参数  没有
  * @返回  没有
  */
void EXTI_DeInit(void)
{
  EXTI->IMR = 0x00000000;
  EXTI->EMR = 0x00000000;
  EXTI->RTSR = 0x00000000; 
  EXTI->FTSR = 0x00000000; 
  EXTI->PR = 0x000FFFFF;
}

/**
  * @简述  根据EXIT_InitStruct中指定的参数初始化 EXTI 外设.
  * @参数  EXTI_InitStruct: 指向 EXTI_InitTypeDef 的结构，它包含特定EXTI的配置信息.
  * @返回  没有
  */
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* 清除 EXTI 外部中断线配置 */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    tmp += EXTI_InitStruct->EXTI_Mode;

    *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;

    /* 清除上升沿下降沿边沿配置 */
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* 为选定的外部中断选择触发(事件) */
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* 上升沿下降沿 */
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_InitStruct->EXTI_Trigger;

      *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
    }
  }
  else
  {
    tmp += EXTI_InitStruct->EXTI_Mode;

    /* 禁止选中的外部中断线 */
    *(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;
  }
}

/**
  * @简述  将EXTI_InitStruct成员置为默认值.
  * @参数  EXTI_InitStruct: 指向EXTI_InitTypeDef结构，该结构将被初始化.
  * @返回  没有
  */
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = EXTI_LineNone;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/**
  * @简述  产生一个软件中断.
  * @参数  EXTI_Line: 指定 EXTI 线激活或禁止.
  *                   这个参数可以任意组合 EXTI_Linex 中 x可以取(0..19).
  * @返回  没有
  */
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->SWIER |= EXTI_Line;
}

/**
  * @简述  检查特定的 EXTI 线标志是否被置位.
  * @参数  EXTI_Line: 特定的 EXTI 线标志.
  *                   这个参数可以是:
  *                                  EXTI_Linex: 外部中断线 x可以取(0..19)
  * @返回  EXTI_Line 的新状态 (SET 或 RESET).
  */
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  if ((EXTI->PR & EXTI_Line) != (uint32_t)RESET)
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
  * @简述  清除 EXTI 线的挂起标志.
  * @参数  EXTI_Line: 指定要清除标志的 EXTI 线.
  *                   这个参数可以是任意EXTI_Linex组合，x可以是(0..19).
  * @返回  没有
  */
void EXTI_ClearFlag(uint32_t EXTI_Line)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/**
  * @简述  检查指定的 EXTI 线路触发请求发生与否.
  * @参数  EXTI_Line: 待检查 EXTI 线路的挂起位.
  *                   这个参数可以是:
  *                                  EXTI_Linex: 外部中断线 x可以是(0..19)
  * @返回  EXTI_Line 的新状态 (SET 或 RESET).
  */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* 检查参数 */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  enablestatus =  EXTI->IMR & EXTI_Line;
  if (((EXTI->PR & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
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
  * @简述  清除EXTI线路挂起位.
  * @参数  EXTI_Line: 待清除EXTI线路的挂起位.
  *                    这个参数可以是任意 EXTI_Linex 组合， x可以是(0..19).
  * @返回  没有
  */
void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
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
