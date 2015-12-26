/**
  ******************************************************************************
  * @文件    stm32f10x_wwdg.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 WWDG 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    09/07/2009
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* ----------- WWDG registers bit address in the alias region ----------- */
#define WWDG_OFFSET       (WWDG_BASE - PERIPH_BASE)

/* Alias word address of EWI bit */
#define CFR_OFFSET        (WWDG_OFFSET + 0x04)
#define EWI_BitNumber     0x09
#define CFR_EWI_BB        (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

/* --------------------- WWDG registers bit mask ------------------------ */

/* CR register bit mask */
#define CR_WDGA_Set       ((uint32_t)0x00000080)

/* CFR register bit mask */
#define CFR_WDGTB_Mask    ((uint32_t)0xFFFFFE7F)
#define CFR_W_Mask        ((uint32_t)0xFFFFFF80)
#define BIT_Mask          ((uint8_t)0x7F)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将外设 WWDG 寄存器重设为缺省值.
  * @参数  没有
  * @返回  没有
  */
void WWDG_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
}

/**
  * @简述  设置 WWDG 预分频值.
  * @参数  WWDG_Prescaler: 指定 WWDG 预分频值.
  *                        这个参数可以是下面的值之一:
  *                        WWDG_Prescaler_1: WWDG 计数器时钟 = (PCLK1/4096)/1
  *                        WWDG_Prescaler_2: WWDG 计数器时钟 = (PCLK1/4096)/2
  *                        WWDG_Prescaler_4: WWDG 计数器时钟 = (PCLK1/4096)/4
  *                        WWDG_Prescaler_8: WWDG 计数器时钟 = (PCLK1/4096)/8
  * @返回  没有
  */
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));
  /* 清除 WDGTB[1:0] 位 */
  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;
  /* 设置 WDGTB[1:0] 位，按照 WWDG_Prescaler 的值 */
  tmpreg |= WWDG_Prescaler;
  /* 存储新值 */
  WWDG->CFR = tmpreg;
}

/**
  * @简述  设置 WWDG 窗口值.
  * @参数  指明要和倒计数器比较的窗口值.
  *        这个个参数的值必须小于0x80.
  * @返回  没有
  */
void WWDG_SetWindowValue(uint8_t WindowValue)
{
  __IO uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));
  /* 清除 W[6:0] 位 */

  tmpreg = WWDG->CFR & CFR_W_Mask;

  /* 依照窗口值的值设置 W[6:0] 位 */
  tmpreg |= WindowValue & (uint32_t) BIT_Mask;

  /* 存储新值 */
  WWDG->CFR = tmpreg;
}

/**
  * @简述  使能 WWDG 早期唤醒中断 (EWI).
  * @参数  没有
  * @返回  没有
  */
void WWDG_EnableIT(void)
{
  *(__IO uint32_t *) CFR_EWI_BB = (uint32_t)ENABLE;
}

/**
  * @简述  设置 WWDG 计数器值.
  * @参数  Counter: 指定看门狗计数器值.
  *                 该参数取值必须在0x40与0x7F之间.
  * @返回  没有
  */
void WWDG_SetCounter(uint8_t Counter)
{
  /* 检查参数 */
  assert_param(IS_WWDG_COUNTER(Counter));
  /* 为配置计数器的值写 T[6:0]位，除了写0到WDG A位没有意义 */
  WWDG->CR = Counter & BIT_Mask;
}

/**
  * @简述  使能 WWDG 和载入计数器的值.                  
  * @参数  Counter: 指定看门狗载入计数器的值.
  *                 这个参数必须是0x40到0x7F之间的一个数.
  * @返回  没有
  */
void WWDG_Enable(uint8_t Counter)
{
  /* 检查参数 */
  assert_param(IS_WWDG_COUNTER(Counter));
  WWDG->CR = CR_WDGA_Set | Counter;
}

/**
  * @简述  检查 WWDG 早期唤醒中断标志位被设置与否.
  * @参数  没有
  * @返回  早期唤醒中断标志位的新状态 (SET 或 RESET)
  */
FlagStatus WWDG_GetFlagStatus(void)
{
  return (FlagStatus)(WWDG->SR);
}

/**
  * @简述  清除早期唤醒中断标志位.
  * @参数  没有
  * @返回  没有
  */
void WWDG_ClearFlag(void)
{
  WWDG->SR = (uint32_t)RESET;
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
