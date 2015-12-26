/**
  ******************************************************************************
  * @文件    stm32f10x_dbgmcu.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 DBGMCU 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    21/07/2009
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
#include "stm32f10x_dbgmcu.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

#define IDCODE_DEVID_Mask    ((uint32_t)0x00000FFF)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  返回设备的校正标示符.
  * @参数  None
  * @返回  设备的校正标示符
  */
uint32_t DBGMCU_GetREVID(void)
{
   return(DBGMCU->IDCODE >> 16);
}

/**
  * @简述  返回设备的标示符.
  * @参数  没有
  * @返回  设备的标示符
  */
uint32_t DBGMCU_GetDEVID(void)
{
   return(DBGMCU->IDCODE & IDCODE_DEVID_Mask);
}

/**
  * @简述  配置MCU在调试模式下指定外设和低功耗模式状态.
  * @参数  DBGMCU_Periph: 指定的外设和低功耗模式.
  *          可以使用下述任意值的组合:
  *          DBGMCU_SLEEP:     在睡眠模式保持调试器的连接              
  *          DBGMCU_STOP:      在停止模式保持调试器的连接               
  *          DBGMCU_STANDBY:   在待机模式保持调试器的连接            
  *          DBGMCU_IWDG_STOP: 当内核停止时停止调试 IWDG          
  *          DBGMCU_WWDG_STOP: 当内核停止时停止调试 WWDG          
  *          DBGMCU_TIM1_STOP: 当内核停止时停止 TIM1 计数器          
  *          DBGMCU_TIM2_STOP: 当内核停止时停止 TIM2 计数器         
  *          DBGMCU_TIM3_STOP: 当内核停止时停止 TIM3 计数器         
  *          DBGMCU_TIM4_STOP: 当内核停止时停止 TIM4 计数器        
  *          DBGMCU_CAN1_STOP: 当内核停止时停止调试 CAN1 
  *          DBGMCU_I2C1_SMBUS_TIMEOUT: 当内核停止时停止 I2C1 总线超时模式
  *          DBGMCU_I2C2_SMBUS_TIMEOUT: 当内核停止时停止 I2C2 总线超时模式
  *          DBGMCU_TIM5_STOP: 当内核停止时停止 TIM5 计数器          
  *          DBGMCU_TIM6_STOP: 当内核停止时停止 TIM6 计数器         
  *          DBGMCU_TIM7_STOP: 当内核停止时停止 TIM7 计数器         
  *          DBGMCU_TIM8_STOP: 当内核停止时停止 TIM8 计数器
  *          DBGMCU_CAN2_STOP: 当内核停止时停止调试 CAN2  
  * @参数  NewState: 指定的外设在低功耗模式的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DBGMCU_PERIPH(DBGMCU_Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    DBGMCU->CR |= DBGMCU_Periph;
  }
  else
  {
    DBGMCU->CR &= ~DBGMCU_Periph;
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
