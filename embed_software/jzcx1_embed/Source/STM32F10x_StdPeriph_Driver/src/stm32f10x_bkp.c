/**
  ******************************************************************************
  * @文件    stm32f10x_bkp.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件提供了所有 BKP 固件函数.
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
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* ------------ BKP registers bit address in the alias region --------------- */
#define BKP_OFFSET        (BKP_BASE - PERIPH_BASE)

/* --- CR Register ----*/

/* Alias word address of TPAL bit */
#define CR_OFFSET         (BKP_OFFSET + 0x30)
#define TPAL_BitNumber    0x01
#define CR_TPAL_BB        (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPAL_BitNumber * 4))

/* Alias word address of TPE bit */
#define TPE_BitNumber     0x00
#define CR_TPE_BB         (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPE_BitNumber * 4))

/* --- CSR Register ---*/

/* Alias word address of TPIE bit */
#define CSR_OFFSET        (BKP_OFFSET + 0x34)
#define TPIE_BitNumber    0x02
#define CSR_TPIE_BB       (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TPIE_BitNumber * 4))

/* Alias word address of TIF bit */
#define TIF_BitNumber     0x09
#define CSR_TIF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TIF_BitNumber * 4))

/* Alias word address of TEF bit */
#define TEF_BitNumber     0x08
#define CSR_TEF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TEF_BitNumber * 4))

/* ---------------------- BKP registers bit mask ------------------------ */

/* RTCCR register bit mask */
#define RTCCR_CAL_Mask    ((uint16_t)0xFF80)
#define RTCCR_Mask        ((uint16_t)0xFC7F)

/* CSR register bit mask */
#define CSR_CTE_Set       ((uint16_t)0x0001)
#define CSR_CTI_Set       ((uint16_t)0x0002)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将 BKP 外设寄存器复位到它们的默认值.
  * @参数  没有
  * @返回  没有
  */
void BKP_DeInit(void)
{
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
}

/**
  * @简述  设置侵入检测管脚的有效电平.
  * @参数  BKP_TamperPinLevel: 侵入检测管脚的有效电平.
  *                            这个参数可以取下面中的一个:
  *                            BKP_TamperPinLevel_High: 侵入检测管脚高电平有效
  *                            BKP_TamperPinLevel_Low:  侵入检测管脚低电平有效
  * @返回  没有
  */
void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel)
{
  /* 检查参数 */
  assert_param(IS_BKP_TAMPER_PIN_LEVEL(BKP_TamperPinLevel));
  *(__IO uint32_t *) CR_TPAL_BB = BKP_TamperPinLevel;
}

/**
  * @简述  使能或失能侵入检测管脚.
  * @参数  NewState: 侵入检测管脚的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void BKP_TamperPinCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_TPE_BB = (uint32_t)NewState;
}

/**
  * @简述  使能或失能侵入检测管脚的中断.
  * @参数  NewState: 侵入检测管脚的中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void BKP_ITConfig(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_TPIE_BB = (uint32_t)NewState;
}

/**
  * @简述  选择在侵入检测管脚上输出的 RTC 时钟源.
  * @参数  BKP_RTCOutputSource: 指定 RTC 输出的来源.
  *                             这个参数可以取下面中的一个:
  *            BKP_RTCOutputSource_None: 侵入检测管脚上没有实时时钟输出.
  *            BKP_RTCOutputSource_CalibClock: 侵入检测管脚上输出实时时钟告警脉冲上输出的实时时钟等于频率除以64.
  *            BKP_RTCOutputSource_Alarm: 侵入检测管脚上输出实时时钟告警脉冲.
  *            BKP_RTCOutputSource_Second: 侵入检测管脚上输出实时时钟秒脉冲.  
  * @返回  没有
  */
void BKP_RTCOutputConfig(uint16_t BKP_RTCOutputSource)
{
  uint16_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_BKP_RTC_OUTPUT_SOURCE(BKP_RTCOutputSource));
  tmpreg = BKP->RTCCR;
  /* 清除 CCO, ASOE 和 ASOS 位 */
  tmpreg &= RTCCR_Mask;
  
  /* 设置 CCO, ASOE 和 ASOS 位，按照 BKP_RTCOutputSource 的值 */
  tmpreg |= BKP_RTCOutputSource;
  /* 存储新值 */
  BKP->RTCCR = tmpreg;
}

/**
  * @简述  设置 RTC 时钟校准值.
  * @参数  CalibrationValue: 指定 RTC 时钟的校准值.
  *                          这个参数必须在 0 和 0x7F 之间.
  * @返回  没有
  */
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue)
{
  uint16_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_BKP_CALIBRATION_VALUE(CalibrationValue));
  tmpreg = BKP->RTCCR;
  /* 清除 CAL[6:0] 位 */
  tmpreg &= RTCCR_CAL_Mask;
  /* 设置 CAL[6:0] 位，按照 CalibrationValue 的值 */
  tmpreg |= CalibrationValue;
  /* 存储新值 */
  BKP->RTCCR = tmpreg;
}

/**
  * @简述  向指定的后备寄存器中写入用户程序数据.
  * @参数  BKP_DR: 指定的后备寄存器.
  *                这个参数 BKP_DRx 中x可以取[1, 42]
  * @参数  Data: 需要写入的数据
  * @返回  没有
  */
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKP_BASE; 
  tmp += BKP_DR;

  *(__IO uint32_t *) tmp = Data;
}

/**
  * @简述  从指定的数据备份寄存器中读出数据.
  * @参数  BKP_DR: 指定的数据备份寄存器.
  *                这个参数 BKP_DRx 中x可以取[1, 42]
  * @返回  指定的数据备份寄存器的内容
  */
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKP_BASE; 
  tmp += BKP_DR;

  return (*(__IO uint16_t *) tmp);
}

/**
  * @简述  检查侵入检测管脚事件的标志位被设置与否 输.
  * @参数  没有
  * @返回  侵入检测管脚事件的标志的新状态 (SET 或 RESET).
  */
FlagStatus BKP_GetFlagStatus(void)
{
  return (FlagStatus)(*(__IO uint32_t *) CSR_TEF_BB);
}

/**
  * @简述  清除侵入检测管脚事件的待处理标志位.
  * @参数  没有
  * @返回  没有
  */
void BKP_ClearFlag(void)
{
  /* 设置 CTE 位，清除侵入检测管脚事件的待处理标志位 */
  BKP->CSR |= CSR_CTE_Set;
}

/**
  * @简述  检查侵入检测中断发生与否.
  * @参数  没有
  * @返回  侵入检测中断标志位的新状态 (SET 或 RESET).
  */
ITStatus BKP_GetITStatus(void)
{
  return (ITStatus)(*(__IO uint32_t *) CSR_TIF_BB);
}

/**
  * @简述  清除侵侵入检测中断的待处理位.
  * @参数  没有
  * @返回  没有
  */
void BKP_ClearITPendingBit(void)
{
  /* 设置 CTI 位， 清除侵侵入检测中断的待处理位 */
  BKP->CSR |= CSR_CTI_Set;
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
