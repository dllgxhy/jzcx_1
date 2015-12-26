/**
  ******************************************************************************
  * @file    stm32f10x_pwr.c
  * @author  MCD 应用程序组
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   这个文件 提供所有 PWR 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    07/07/2009
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
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

/* 自用类型 ------------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/

/* --------- PWR registers bit address in the alias region ------------ */
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

/* --- CR 寄存器 ---*/

/* Alias word address of DBP bit */
#define CR_OFFSET                (PWR_OFFSET + 0x00)
#define DBP_BitNumber            0x08
#define CR_DBP_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

/* Alias word address of PVDE bit */
#define PVDE_BitNumber           0x04
#define CR_PVDE_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PVDE_BitNumber * 4))

/* --- CSR 寄存器 ---*/

/* Alias word address of EWUP bit */
#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))

/* ------------------ PWR 寄存器位屏蔽 ------------------------ */

/* CR register bit mask */
#define CR_PDDS_Set              ((uint32_t)0x00000002)
#define CR_DS_Mask               ((uint32_t)0xFFFFFFFC)
#define CR_CWUF_Set              ((uint32_t)0x00000004)
#define CR_PLS_Mask              ((uint32_t)0xFFFFFF1F)

/* --------- Cortex 系统控制寄存器位屏蔽 ---------------------- */

/* Cortex 系统控制寄存器地址 */
#define SCB_SysCtrl              ((uint32_t)0xE000ED10)

/* SLEEPDEEP 位屏蔽 */
#define SysCtrl_SLEEPDEEP_Set    ((uint32_t)0x00000004)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  复位 PWR 外围寄存器为默认复位值.
  * @参数  没有
  * @返回  没有
  */
void PWR_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/**
  * @简述  使能或者失能 RTC 和后备寄存器访问.
  * @参数  NewState: 访问 RTC 和备份寄存器的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)NewState;
}

/**
  * @简述  使能或者失能可编程电压探测器 (PVD).
  * @参数  NewState: PVD 的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void PWR_PVDCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)NewState;
}

/**
  * @简述  设置 PVD 的探测电压阈值 (PVD).
  * @参数  PWR_PVDLevel: PVD 的探测电压阈值
  *                      这个参数可以是下列值中的一个:
  *          PWR_PVDLevel_2V2: PVD 探测电压阈值 2.2V
  *          PWR_PVDLevel_2V3: PVD 探测电压阈值 2.3V
  *          PWR_PVDLevel_2V4: PVD 探测电压阈值 2.4V
  *          PWR_PVDLevel_2V5: PVD 探测电压阈值 2.5V
  *          PWR_PVDLevel_2V6: PVD 探测电压阈值 2.6V
  *          PWR_PVDLevel_2V7: PVD 探测电压阈值 2.7V
  *          PWR_PVDLevel_2V8: PVD 探测电压阈值 2.8V
  *          PWR_PVDLevel_2V9: PVD 探测电压阈值 2.9V
  * @返回  没有
  */
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));
  tmpreg = PWR->CR;
  /* 清除 PLS[7:5] 位 */
  tmpreg &= CR_PLS_Mask;
  /* 设置 PLS[7:5] 位 按照 PWR_PVDLevel 的值 */
  tmpreg |= PWR_PVDLevel;
  /* 存储新值 */
  PWR->CR = tmpreg;
}

/**
  * @简述  使能或者失能唤醒管脚功能.
  * @参数  NewState: 唤醒管脚功能的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void PWR_WakeUpPinCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_EWUP_BB = (uint32_t)NewState;
}

/**
  * @简述  进入停止(STOP)模式.
  * @参数  PWR_Regulator: 电压转换器在停止模式下的状态.
  *                       这个参数可以是下列值中的一个:
  *                       PWR_Regulator_ON: 停止模式下电压转换器 ON
  *                       PWR_Regulator_LowPower: 停止模式下电压转换器进入低功耗模式
  * @参数  PWR_STOPEntry: 选择使用指令 WFE 还是 WFI 来进入停止模式.
  *                       这个参数可以是下列值中的一个:
  *                       PWR_STOPEntry_WFI: 使用指令 WFI 来进入停止模式
  *                       PWR_STOPEntry_WFE: 使用指令 WFE 来进入停止模式
  * @返回  没有
  */
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
  assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));
  
  /* 在停止模式选择校准状态 ---------------------------------*/
  tmpreg = PWR->CR;
  /* 清 PDDS 和 LPDS 位 */
  tmpreg &= CR_DS_Mask;
  /* 依照 PWR_Regulator 值置位 LPDS */
  tmpreg |= PWR_Regulator;
  /* 保存新的值 */
  PWR->CR = tmpreg;
  /* 设置 SLEEPDEEP 位按照 Cortex 系统控制寄存器 */
  *(__IO uint32_t *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;
  
  /* 选择停止模式进入 --------------------------------------------------*/
  if(PWR_STOPEntry == PWR_STOPEntry_WFI)
  {   
    /* 请求等待中断 */
    __WFI();
  }
  else
  {
    /* 请求等待事件 */
    __WFE();
  }
}

/**
  * @简述  进入待机(STANDBY)模式.
  * @参数  没有
  * @返回  没有
  */
void PWR_EnterSTANDBYMode(void)
{
  /* 清除 Wake-up 标志 */
  PWR->CR |= CR_CWUF_Set;
  /* 选择 STANDBY 模式 */
  PWR->CR |= CR_PDDS_Set;
  /* 置位 Cortex 系统的控制寄存器 SLEEPDEEP 位 */
  *(__IO uint32_t *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;
/* 这个选择常常是确定存储操作已完成 */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* 请求等待中断 */
  __WFI();
}

/**
  * @简述  检查指定的 PWR 标志位设置与否.
  * @参数  PWR_FLAG: 要检查的标志.
  *                  这个参数可以是下面的值之一:
  *                  PWR_FLAG_WU: 唤醒标志
  *                  PWR_FLAG_SB: 待命标志
  *                  PWR_FLAG_PVDO: PVD 输出
  * @返回  PWR_FLAG 的新状态 (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));
  
  if ((PWR->CSR & PWR_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* 返回状态标志 */
  return bitstatus;
}

/**
  * @简述  清空 PWR 挂起标志位.
  * @参数  PWR_FLAG: 要清空的标志.
  *                  这个参数可以是下面的值之一:
  *                  PWR_FLAG_WU: 唤醒标志
  *                  PWR_FLAG_SB: 待命标志
  * @返回  没有
  */
void PWR_ClearFlag(uint32_t PWR_FLAG)
{
  /* 检查参数 */
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
  PWR->CR |=  PWR_FLAG << 2;
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
