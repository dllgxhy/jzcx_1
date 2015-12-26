/**
  ******************************************************************************
  * @文件    stm32f10x_rtc.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 RTC 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    08/07/2009
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
#include "stm32f10x_rtc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/
#define CRL_CNF_Set      ((uint16_t)0x0010)      /*!< Configuration Flag Enable Mask */
#define CRL_CNF_Reset    ((uint16_t)0xFFEF)      /*!< Configuration Flag Disable Mask */
#define RTC_LSB_Mask     ((uint32_t)0x0000FFFF)  /*!< RTC LSB Mask */
#define PRLH_MSB_Mask    ((uint32_t)0x000F0000)  /*!< RTC Prescaler MSB Mask */

/* 自用宏 -----------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------------------*/
/* 自用函数 ---------------------------------------------------------------*/

/**
  * @简述  使能或失能指定的 RTC 中断.
  * @参数  RTC_IT: 要使能或失能指定的 RTC 中断.
  *                这个参数可以是下面值的任意组合:
  *                RTC_IT_OW:  溢出中断
  *                RTC_IT_ALR: 警报中断
  *                RTC_IT_SEC: 秒中断
  * @参数  NewState: 指定 RTC 中断的新的状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RTC_IT(RTC_IT));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    RTC->CRH |= RTC_IT;
  }
  else
  {
    RTC->CRH &= (uint16_t)~RTC_IT;
  }
}

/**
  * @简述  进入 RTC 配置模式.
  * @参数  没有
  * @返回  没有
  */
void RTC_EnterConfigMode(void)
{
  /* 设置 CNF 标志进入配置模式 */
  RTC->CRL |= CRL_CNF_Set;
}

/**
  * @简述  退出 RTC 配置模式.
  * @参数  没有
  * @返回  没有
  */
void RTC_ExitConfigMode(void)
{
  /* 复位 CNF 标志退出配置模式 */
  RTC->CRL &= CRL_CNF_Reset;
}

/**
  * @简述  获得 RTC 计数器值.
  * @参数  没有
  * @返回  RTC 计数器值.
  */
uint32_t RTC_GetCounter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (((uint32_t)RTC->CNTH << 16 ) | tmp) ;
}

/**
  * @简述  设置 RTC 计数器值.
  * @参数  CounterValue: RTC 计数器的新值.
  * @返回  没有
  */
void RTC_SetCounter(uint32_t CounterValue)
{ 
  RTC_EnterConfigMode();
  /* 设置 RTC 计数器高位字 */
  RTC->CNTH = CounterValue >> 16;
  /* 设置 RTC 计数器低位字 */
  RTC->CNTL = (CounterValue & RTC_LSB_Mask);
  RTC_ExitConfigMode();
}

/**
  * @简述  设置 RTC 预分频装在寄存器的值.
  * @参数  PrescalerValue: RTC 预分频装在寄存器的新值.
  * @返回  没有
  */
void RTC_SetPrescaler(uint32_t PrescalerValue)
{
  /* 检查参数 */
  assert_param(IS_RTC_PRESCALER(PrescalerValue));
  
  RTC_EnterConfigMode();
  /* 设置 RTC 预分频装在寄存器高位字 */
  RTC->PRLH = (PrescalerValue & PRLH_MSB_Mask) >> 16;
  /* 设置 RTC 预分频装在寄存器低位字 */
  RTC->PRLL = (PrescalerValue & RTC_LSB_Mask);
  RTC_ExitConfigMode();
}

/**
  * @简述  设置 RTC 报警寄存器的值.
  * @参数  AlarmValue: RTC 报警寄存器的新值.
  * @返回  没有
  */
void RTC_SetAlarm(uint32_t AlarmValue)
{  
  RTC_EnterConfigMode();
  /* 设置报警寄存器高位字 */
  RTC->ALRH = AlarmValue >> 16;
  /* 设置报警寄存器低位字 */
  RTC->ALRL = (AlarmValue & RTC_LSB_Mask);
  RTC_ExitConfigMode();
}

/**
  * @简述  获取 RTC 预分频分频因子的值.
  * @参数  没有
  * @返回  RTC 预分频分频因子的值.
  */
uint32_t RTC_GetDivider(void)
{
  uint32_t tmp = 0x00;
  tmp = ((uint32_t)RTC->DIVH & (uint32_t)0x000F) << 16;
  tmp |= RTC->DIVL;
  return tmp;
}

/**
  * @简述  等待最近一次对RTC寄存器的写操作完成.
  * @注解  这个函数一定要在任何对RTC寄存器写操作之前调用.
  * @参数  没有
  * @返回  没有
  */
void RTC_WaitForLastTask(void)
{
  /* 循环直到 RTOFF 标志置位 */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
  }
}

/**
  * @简述  等待RTC寄存器(RTC_CNT,RTC_ALR和RTC_PRL)与RTC APB时钟同步.
  * @注解  这个函数一定要在任何对APB复位或APB时钟停止后的写操作之前调用.
  * @参数  没有
  * @返回  没有
  */
void RTC_WaitForSynchro(void)
{
  /* 清除 RSF 标志 */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* 循环直到 RSF 标志置位 */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
  }
}

/**
  * @简述  检查指定的 RTC 标志设置与否.
  * @参数  RTC_FLAG: 指定要检查的标志.
  *                  这个参数可以选下面的值之一:
  *                  RTC_FLAG_RTOFF: RTC 操作关闭标志
  *                  RTC_FLAG_RSF:   寄存器同步标志
  *                  RTC_FLAG_OW:    溢出中断标志
  *                  RTC_FLAG_ALR:   警报中断标志
  *                  RTC_FLAG_SEC:   秒中断标志
  * @返回  RTC_FLAG 的新状态 (SET 或 RESET).
  */
FlagStatus RTC_GetFlagStatus(uint16_t RTC_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* 检查参数 */
  assert_param(IS_RTC_GET_FLAG(RTC_FLAG)); 
  
  if ((RTC->CRL & RTC_FLAG) != (uint16_t)RESET)
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
  * @简述  清除 RTC 挂起标志.
  * @参数  RTC_FLAG: 指定被清除的标志.
  *                  这个参数可以是下面值的任意组合:
  *                  RTC_FLAG_RSF: 同步寄存器标志. 只有在SPB复位或APB时钟停止之后清除.
  *                  RTC_FLAG_OW:  溢出中断标志
  *                  RTC_FLAG_ALR: 报警中断标志
  *                  RTC_FLAG_SEC: 秒中断标志
  * @返回  没有
  */
void RTC_ClearFlag(uint16_t RTC_FLAG)
{
  /* 检查参数 */
  assert_param(IS_RTC_CLEAR_FLAG(RTC_FLAG)); 
    
  /* 清除 RTC 挂起标志 */
  RTC->CRL &= (uint16_t)~RTC_FLAG;
}

/**
  * @简述  检查指定的 RTC 中断发生与否.
  * @param  RTC_IT: 要检查的RTC中断源.
  *                 这个参数可以选下面的值之一:
  *                 RTC_IT_OW:  溢出中断
  *                 RTC_IT_ALR: 报警中断
  *                 RTC_IT_SEC: 秒中断
  * @返回  RTC_IT 的新状态 (SET 或 RESET).
  */
ITStatus RTC_GetITStatus(uint16_t RTC_IT)
{
  ITStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_RTC_GET_IT(RTC_IT)); 
  
  bitstatus = (ITStatus)(RTC->CRL & RTC_IT);
  if (((RTC->CRH & RTC_IT) != (uint16_t)RESET) && (bitstatus != (uint16_t)RESET))
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
  * @简述  清除 RTC 的中断待处理位.
  * @参数  RTC_IT: 待清除的 RTC 中断待处理位.
  *                这个参数可以是下面值的任意组合:
  *                RTC_IT_OW:  溢出中断
  *                RTC_IT_ALR: 报警中断
  *                RTC_IT_SEC: 秒中断
  * @返回  没有
  */
void RTC_ClearITPendingBit(uint16_t RTC_IT)
{
  /* 检查参数 */
  assert_param(IS_RTC_IT(RTC_IT));  
  
  /* 清除 RTC 挂起标志 */
  RTC->CRL &= (uint16_t)~RTC_IT;
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
