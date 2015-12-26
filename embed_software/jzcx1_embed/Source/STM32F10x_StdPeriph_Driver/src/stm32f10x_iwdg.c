/**
  ******************************************************************************
  * @文件    stm32f10x_iwdg.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 IWDG 固件函数.
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

/* 包含 ------------------------------------------------------------------*/
#include "stm32f10x_iwdg.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* ---------------------- IWDG registers bit mask ----------------------------*/

/* KR register bit mask */
#define KR_KEY_Reload    ((uint16_t)0xAAAA)
#define KR_KEY_Enable    ((uint16_t)0xCCCC)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/


/**
  * @简述  使能或者失能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作.
  * @参数  IWDG_WriteAccess: 对 IWDG_PR 和 IWDG_RLR 寄存器的写访问的新的状态.
  *                          这个参数可以是下面的值之一:
  *                          IWDG_WriteAccess_Enable:  使能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作
  *                          IWDG_WriteAccess_Disable: 失能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作
  * @返回  没有
  */
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{
  /* 检查参数 */
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
  IWDG->KR = IWDG_WriteAccess;
}

/**
  * @简述  设置 IWDG 预分频值.
  * @参数  IWDG_Prescaler: 指定 IWDG 预分频值.
  *                        这个参数可以是下面的值之一:
  *                        IWDG_Prescaler_4:   设置IWDG预分频值为 4
  *                        IWDG_Prescaler_8:   设置IWDG预分频值为 8
  *                        IWDG_Prescaler_16:  设置IWDG预分频值为 16
  *                        IWDG_Prescaler_32:  设置IWDG预分频值为 32
  *                        IWDG_Prescaler_64:  设置IWDG预分频值为 64
  *                        IWDG_Prescaler_128: 设置IWDG预分频值为 128
  *                        IWDG_Prescaler_256: 设置IWDG预分频值为 256
  * @返回  没有
  */
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{
  /* 检查参数 */
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
  IWDG->PR = IWDG_Prescaler;
}

/**
  * @简述  设置 IWDG 重装载值.
  * @参数  Reload: 指定 IWDG 重装载值.
  *                这个参数一定是在'0'到'0x0FFF'之间的数.
  * @返回  没有
  */
void IWDG_SetReload(uint16_t Reload)
{
  /* 检查参数 */
  assert_param(IS_IWDG_RELOAD(Reload));
  IWDG->RLR = Reload;
}

/**
  * @简述  按照 IWDG 重装载寄存器的值重装载 IWDG 计数器
  *        (对 IWDG_PR 和 IWDG_RLR 寄存器的写访问被禁能).
  * @参数  没有
  * @返回  没有
  */
void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

/**
  * @简述  使能 IWDG (对 IWDG_PR 和 IWDG_RLR 寄存器的写访问被禁能).
  * @参数  没有
  * @返回  没有
  */
void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_Enable;
}

/**
  * @简述  检查指定的 IWDG 标志位被设置与否.
  * @参数  IWDG_FLAG: 要检查的标志.
  *                   这个参数可以是下面的值之一:
  *                   IWDG_FLAG_PVU: 预分频因子值更新了
  *                   IWDG_FLAG_RVU: 重载值更新了
  * @返回  IWDG_FLAG 的新状态 (SET 或 RESET).
  */
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_IWDG_FLAG(IWDG_FLAG));
  if ((IWDG->SR & IWDG_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* 返回标志状态 */
  return bitstatus;
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
