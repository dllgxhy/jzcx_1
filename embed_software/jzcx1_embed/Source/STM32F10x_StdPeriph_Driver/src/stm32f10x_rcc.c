/**
  ******************************************************************************
  * @文件    stm32f10x_rcc.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @brief   这个文件 提供所有 RCC 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    03/07/2009
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
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)

/* --- CR 寄存器 ---*/

/* Alias word address of HSION bit */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))

/* Alias word address of PLLON bit */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))

#ifdef STM32F10X_CL
 /* Alias word address of PLL2ON bit */
 #define PLL2ON_BitNumber          0x1A
 #define CR_PLL2ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL2ON_BitNumber * 4))

 /* Alias word address of PLL3ON bit */
 #define PLL3ON_BitNumber          0x1C
 #define CR_PLL3ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL3ON_BitNumber * 4))
#endif /* STM32F10X_CL */ 

/* Alias word address of CSSON bit */
#define CSSON_BitNumber           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))

/* --- CFGR 寄存器 ---*/

/* Alias word address of USBPRE bit */
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)

#ifndef STM32F10X_CL
 #define USBPRE_BitNumber          0x16
 #define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))
#else
 #define OTGFSPRE_BitNumber        0x16
 #define CFGR_OTGFSPRE_BB          (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (OTGFSPRE_BitNumber * 4))
#endif /* STM32F10X_CL */ 

/* --- BDCR 寄存器 ---*/

/* Alias word address of RTCEN bit */
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

/* Alias word address of BDRST bit */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR 寄存器 ---*/

/* Alias word address of LSION bit */
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

#ifdef STM32F10X_CL
/* --- CFGR2 寄存器 ---*/

 /* Alias word address of I2S2SRC bit */
 #define CFGR2_OFFSET              (RCC_OFFSET + 0x2C)
 #define I2S2SRC_BitNumber         0x11
 #define CFGR2_I2S2SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S2SRC_BitNumber * 4))

 /* Alias word address of I2S3SRC bit */
 #define I2S3SRC_BitNumber         0x12
 #define CFGR2_I2S3SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S3SRC_BitNumber * 4))
#endif /* STM32F10X_CL */

/* ---------------------- RCC 寄存器 bit mask ------------------------ */

/* CR register bit mask */
#define CR_HSEBYP_Reset           ((uint32_t)0xFFFBFFFF)
#define CR_HSEBYP_Set             ((uint32_t)0x00040000)
#define CR_HSEON_Reset            ((uint32_t)0xFFFEFFFF)
#define CR_HSEON_Set              ((uint32_t)0x00010000)
#define CR_HSITRIM_Mask           ((uint32_t)0xFFFFFF07)

/* CFGR register bit mask */
#ifndef STM32F10X_CL
 #define CFGR_PLL_Mask            ((uint32_t)0xFFC0FFFF)
#else
 #define CFGR_PLL_Mask           ((uint32_t)0xFFC2FFFF)
#endif /* STM32F10X_CL */ 

#define CFGR_PLLMull_Mask         ((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask          ((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask        ((uint32_t)0x00020000)
#define CFGR_SWS_Mask             ((uint32_t)0x0000000C)
#define CFGR_SW_Mask              ((uint32_t)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask      ((uint32_t)0xFFFFFF0F)
#define CFGR_HPRE_Set_Mask        ((uint32_t)0x000000F0)
#define CFGR_PPRE1_Reset_Mask     ((uint32_t)0xFFFFF8FF)
#define CFGR_PPRE1_Set_Mask       ((uint32_t)0x00000700)
#define CFGR_PPRE2_Reset_Mask     ((uint32_t)0xFFFFC7FF)
#define CFGR_PPRE2_Set_Mask       ((uint32_t)0x00003800)
#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)
#define CFGR_ADCPRE_Set_Mask      ((uint32_t)0x0000C000)

/* CSR register bit mask */
#define CSR_RMVF_Set              ((uint32_t)0x01000000)

#ifdef STM32F10X_CL
/* CFGR2 register bit mask */
 #define CFGR2_PREDIV1SRC         ((uint32_t)0x00010000)
 #define CFGR2_PREDIV1            ((uint32_t)0x0000000F)
 #define CFGR2_PREDIV2            ((uint32_t)0x000000F0)
 #define CFGR2_PLL2MUL            ((uint32_t)0x00000F00)
 #define CFGR2_PLL3MUL            ((uint32_t)0x0000F000)
#endif /* STM32F10X_CL */ 

/* RCC Flag Mask */
#define FLAG_Mask                 ((uint8_t)0x1F)

#ifndef HSI_Value
/* Typical Value of the HSI in Hz */
 #define HSI_Value                 ((uint32_t)8000000)
#endif /* HSI_Value */

/* CIR register byte 2 (Bits[15:8]) base address */
#define CIR_BYTE2_ADDRESS         ((uint32_t)0x40021009)

/* CIR register byte 3 (Bits[23:16]) base address */
#define CIR_BYTE3_ADDRESS         ((uint32_t)0x4002100A)

/* CFGR register byte 4 (Bits[31:24]) base address */
#define CFGR_BYTE4_ADDRESS        ((uint32_t)0x40021007)

/* BDCR register base address */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

#ifndef HSEStartUp_TimeOut
/* Time out for HSE start up */
 #define HSEStartUp_TimeOut        ((uint16_t)0x0500)
#endif /* HSEStartUp_TimeOut */

/* 自用宏 ------------------------------------------------------------*/
/* 自用变量 ----------------------------------------------------------*/

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

/* 自用函数原型 ------------------------------------------------------*/
/* 自用函数 ----------------------------------------------------------*/


/**
  * @简述  将外设RCC寄存器重设为缺省值.
  * @参数  没有
  * @返回  没有
  */
void RCC_DeInit(void)
{
  /* 置位HSION (内部<8MHz>高速时钟使能) */
  RCC->CR |= (uint32_t)0x00000001;

  /* 复位 SW, HPRE, PPRE1, PPRE2, ADCPRE , MCO 位 */
  /* (HIS被选作系统时钟，HSI振荡器被用作系统时钟，SYSCLK未被分频，HCLK未被分频，
     ADC预分频器<PLCK2>被2分频，HSI振荡器时钟/2被选作PLL输入时钟)，微控制器无时钟输出 */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;   /* ?在保留区置1 */
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;   /* ?在保留区清0 */
#endif /* STM32F10X_CL */   
  
  /* 复位 HSEON, CSSON, PLLON 位 (PLL禁用，时钟检测器禁用，HSE振荡器禁用) */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* 复位 HSEBYP 位 (外部1~25MHz振荡器未被旁路) */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* 复位 PLLSRC, PLLXTPRE, PLLMUL , USBPRE/OTGFSPRE 位 */
  /*(USB预分频器为PLL时钟1.5分频，PLL倍频参数X2，HSE未被分频作为PLL输入，HSI振荡器时钟/2被当作PLL输入) */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifndef STM32F10X_CL
  /* 使所有时钟中断失能 */
  RCC->CIR = 0x009F0000;
#else
  /* 复位 PLL2ON , PLL3ON 位 () */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* 使所有时钟中断失能  */
  RCC->CIR = 0x00FF0000;

  /* 复位 CFGR2 寄存器 */
  RCC->CFGR2 = 0x00000000;
#endif /* STM32F10X_CL */
}

/**
  * @简述  配置外部高速振荡器 (HSE).
  * @注解  如果 HSE 被直接使用或者通过 PLL 作为系统时钟，则它不能停止.
  * @参数  RCC_HSE: 指定 HSE 新的状态.
  *                 这个参数可以选择下面的一项:
  *             RCC_HSE_OFF: HSE 振荡器 OFF
  *             RCC_HSE_ON : HSE 振荡器 ON
  *             RCC_HSE_Bypass: HSE 振荡器旁路使用外部时钟
  * @返回  没有
  */
void RCC_HSEConfig(uint32_t RCC_HSE)
{
  /* 检查参数 */
  assert_param(IS_RCC_HSE(RCC_HSE));
  /* 复位 HSEON , HSEBYP 位 在配置 HSE 以前 ------------------*/
  /* Reset HSEON bit (禁用 HSE 振荡器)*/
  RCC->CR &= CR_HSEON_Reset;
  /* Reset HSEBYP bit (外部1~25MHz振荡器未被旁路)*/
  RCC->CR &= CR_HSEBYP_Reset;
  /* 配置 HSE (RCC_HSE_OFF 已经被code核心代码覆盖了) */
  switch(RCC_HSE)
  {
    case RCC_HSE_ON:
      /* Set HSEON bit (打开 HSE 振荡器)*/
      RCC->CR |= CR_HSEON_Set;
      break;
      
    case RCC_HSE_Bypass:
      /* Set HSEBYP and HSEON bits (禁用 HSE 振荡器被外部1~25MHz振荡器和外部时钟旁路)*/
      RCC->CR |= CR_HSEBYP_Set | CR_HSEON_Set;
      break;
      
    default:
      break;
  }
}

/**
  * @简述  等待 HSE 启动.
  * @参数  没有
  * @返回  一个ErrorStatus枚举值:
  *        SUCCESS: HSE 振荡器已经稳定并且可以使用
  *        ERROR  : HSE 振荡器尚未准备好
  */
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  __IO uint32_t StartUpCounter = 0;
  ErrorStatus status = ERROR;
  FlagStatus HSEStatus = RESET;
  
  /* 等待 HSE 准备好和超时的时间到来 */
  do
  {
    HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
    StartUpCounter++;  
  } while((StartUpCounter != HSEStartUp_TimeOut) && (HSEStatus == RESET));
  
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }  
  return (status);
}

/**
  * @简述  调整内部高速振荡器 (HSI) 标度值.
  * @参数  HSICalibrationValue: 校正标度值，这个参数一定是在0到0x1F之间的值.
  * @返回  没有
  */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));
  tmpreg = RCC->CR;
  /* 清除 HSITRIM[4:0] 位 (默认值是16使 HIS 严格工作在8MHz) */
  tmpreg &= CR_HSITRIM_Mask;
  /* 设置 HSITRIM[4:0] 位，根据 HSICalibrationValue 的值 */
  tmpreg |= (uint32_t)HSICalibrationValue << 3;
  /* 存储新的值 */
  RCC->CR = tmpreg;
}

/**
  * @简述  使能或失能内部高速振荡器 (HSI).
  * @注解  如果 HSE 被直接使用或者通过 PLL 作为系统时钟，则它不能停止.
  * @参数  NewState: HSI 的新状态. 
  *                 这个参数可取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_HSICmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_HSION_BB = (uint32_t)NewState;
}

/**
  * @简述  设置PLL时钟源及倍频系数.
  * @注解  这个函数仅仅应用在 PLL 被禁止的时候.
  * @参数  RCC_PLLSource: 指定进入 PLL 的时钟源.
  *                       STM32_互联系列微处理器, 这个参数可取下列的值:
  *                                      RCC_PLLSource_HSI_Div2: PLL 时钟输入等于 HSI 振荡器时钟二分频
  *                                      RCC_PLLSource_PREDIV1: PREDIV1 clock selected as PLL clock entry
  *                       其它_STM32_微处理器, 这个参数可取下列的值:
  *                                      RCC_PLLSource_HSI_Div2: PLL 时钟输入等于 HSI 振荡器时钟二分频
  *                                      RCC_PLLSource_HSE_Div1: PLL 时钟输入等于 HSE 振荡器时钟
  *                                      RCC_PLLSource_HSE_Div2: PLL 时钟输入等于 HSE 振荡器时钟二分频
  * @参数  RCC_PLLMul: 指定 PLL 倍频系数.
  *                    STM32_Connectivity_line_devices, 这个参数可以是 RCC_PLLMul_x x可以取{[4,9], 6_5}
  *                    other_STM32_devices, 这个参数可以是 RCC_PLLMul_x x可以取:[2,16]
  * @返回  没有
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

  tmpreg = RCC->CFGR;
  /* 清 PLLSRC, PLLXTPRE , PLLMUL[3:0] 位 */
  tmpreg &= CFGR_PLL_Mask;
  /* 设置 PLL 配置位 */
  tmpreg |= RCC_PLLSource | RCC_PLLMul;
  /* 存储新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @简述  使能或失能 PLL.
  * @注解  如果 PLL 作为系统时钟，则它不能停止.
  * @参数  NewState: PLL 的新状态. 这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_PLLCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

#ifdef STM32F10X_CL
/**
  * @简述  配置 PREDIV1 分频参数.
  * @注解 
  *   - 这个函数仅仅应用在 PLL 被禁止的时候.
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_PREDIV1_Source: 指定 PREDIV1 时钟源.
  *                            这个参数可以取下列值中的一个:
  *                            RCC_PREDIV1_Source_HSE: HSE 作为 PREDIV1 时钟
  *                            RCC_PREDIV1_Source_PLL2: PLL2 作为 PREDIV1 时钟
  * @参数  RCC_PREDIV1_Div: 选定 PREDIV1 分频系数.
  *                         这个参数 RCC_PREDIV1_Divx 中 x可以取:[1,16]
  * @返回  没有
  */
void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div)
{
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_RCC_PREDIV1_SOURCE(RCC_PREDIV1_Source));
  assert_param(IS_RCC_PREDIV1(RCC_PREDIV1_Div));

  tmpreg = RCC->CFGR2;
  /* 清 PREDIV1[3:0] 和 PREDIV1SRC 位 */
  tmpreg &= ~(CFGR2_PREDIV1 | CFGR2_PREDIV1SRC);
  /* Set the PREDIV1 clock source and division factor */
  tmpreg |= RCC_PREDIV1_Source | RCC_PREDIV1_Div ;
  /* 存储新值 */
  RCC->CFGR2 = tmpreg;
}


/**
  * @简述  配置 PREDIV2 分频参数.
  * @注解 
  *   - 这个函数仅仅应用在 PLL2,PLL3 被禁止的时候.
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_PREDIV2_Div: 选定 PREDIV2 分频系数.
  *                         这个参数 RCC_PREDIV2_Divx 中 x可以取:[1,16]
  * @返回  没有
  */
void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PREDIV2(RCC_PREDIV2_Div));

  tmpreg = RCC->CFGR2;
  /* Clear PREDIV2[3:0] bits */
  tmpreg &= ~CFGR2_PREDIV2;
  /* Set the PREDIV2 division factor */
  tmpreg |= RCC_PREDIV2_Div;
  /* Store the new value */
  RCC->CFGR2 = tmpreg;
}

/**
  * @简述  配置 PLL2 倍频系数.
  * @注解
  *   - 这个函数仅仅应用在 PLL2 被禁止的时候.
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_PLL2Mul: 指定 PLL2 倍频系数.
  *                     这个参数 RCC_PLL2Mul_x 中 x可以取:{[8,14], 16, 20}
  * @返回  没有
  */
void RCC_PLL2Config(uint32_t RCC_PLL2Mul)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PLL2_MUL(RCC_PLL2Mul));

  tmpreg = RCC->CFGR2;
  /* 清 PLL2Mul[3:0] 位 */
  tmpreg &= ~CFGR2_PLL2MUL;
  /* 设置 PLL2 配置位 */
  tmpreg |= RCC_PLL2Mul;
  /* 存储参数 */
  RCC->CFGR2 = tmpreg;
}


/**
  * @简述  使能或失能 PLL2.
  * @注解 
  *   - 如果 PLL2 被用做系统时钟，它就不能被禁止
  *     (i.e.它被用作 PLL 时钟输入源，就是说被用作系统时钟).
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  NewState: PLL2 新的状态. 这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_PLL2Cmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLL2ON_BB = (uint32_t)NewState;
}


/**
  * @简述  配置 PLL3 倍频系数.
  * @注解
  *   - 这个函数仅仅应用在 PLL3 被禁止的时候.
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_PLL3Mul: 指定 PLL3 倍频系数.
  *                     这个参数 RCC_PLL3Mul_x 中 x可以取:{[8,14], 16, 20}
  * @返回  没有
  */
void RCC_PLL3Config(uint32_t RCC_PLL3Mul)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PLL3_MUL(RCC_PLL3Mul));

  tmpreg = RCC->CFGR2;
  /* 清 PLL3Mul[3:0] 位 */
  tmpreg &= ~CFGR2_PLL3MUL;
  /* 设置 PLL3 配置位 */
  tmpreg |= RCC_PLL3Mul;
  /* 存储新值 */
  RCC->CFGR2 = tmpreg;
}


/**
  * @简述  使能或失能 PLL3.
  * @注解  这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  NewState: PLL3 新的状态. 这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_PLL3Cmd(FunctionalState NewState)
{
  /* Check the parameters */

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLL3ON_BB = (uint32_t)NewState;
}
#endif /* STM32F10X_CL */

/**
  * @简述  配置系统时钟 (SYSCLK).
  * @参数  RCC_SYSCLKSource: 指定作为系统时钟的时钟源.
  *                          这个参数可以选择下列中的一个:
  *                          RCC_SYSCLKSource_HSI:   选定 HSI 作为系统时钟
  *                          RCC_SYSCLKSource_HSE:   选定 HSE 作为系统时钟
  *                          RCC_SYSCLKSource_PLLCLK:选定 PLL 作为系统时钟
  * @返回  没有
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));
  tmpreg = RCC->CFGR;
  /* 清 SW[1:0] 位  */
  tmpreg &= CFGR_SW_Mask;
  /* 设置 SW[1:0] 位为 RCC_SYSCLKSource 的值 */
  tmpreg |= RCC_SYSCLKSource;
  /* 存储新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @简述  获得用作系统时钟的时钟源.
  * @参数  没有
  * @返回  作为系统时钟的时钟源. 
  *        这个返回值可以是下列之中的一个:
  *        0x00: HSI 被用作系统时钟
  *        0x04: HSE 被用作系统时钟
  *        0x08: PLL 被用作系统时钟
  */
uint8_t RCC_GetSYSCLKSource(void)
{
  return ((uint8_t)(RCC->CFGR & CFGR_SWS_Mask));
}

/**
  * @简述  配置 AHB 时钟 (HCLK).
  * @参数  RCC_SYSCLK: 定义 AHB 时钟分频. 这个时钟来源于系统时钟
  *                    这个参数可以取下列值中的一个:
  *                    RCC_SYSCLK_Div1: AHB clock = SYSCLK
  *                    RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  *                    RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  *                    RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  *                    RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  *                    RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  *                    RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *                    RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *                    RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @返回  没有
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));
  tmpreg = RCC->CFGR;
  /* 清 HPRE[3:0] 位 */
  tmpreg &= CFGR_HPRE_Reset_Mask;
  /* 设置 HPRE[3:0] 位为 RCC_SYSCLK 的值 */
  tmpreg |= RCC_SYSCLK;
  /* 存储新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @简述  配置低速 APB 时钟 (PCLK1).
  * @参数  RCC_HCLK: 配置 APB1 时钟分频. 这个时钟来源于 AHB 时钟 (HCLK).
  *                  这个参数可以取下列值中的一个:
  *                  RCC_HCLK_Div1: APB1 clock = HCLK
  *                  RCC_HCLK_Div2: APB1 clock = HCLK/2
  *                  RCC_HCLK_Div4: APB1 clock = HCLK/4
  *                  RCC_HCLK_Div8: APB1 clock = HCLK/8
  *                  RCC_HCLK_Div16: APB1 clock = HCLK/16
  * @返回  没有
  */
void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* 清 PPRE1[2:0] 位 */
  tmpreg &= CFGR_PPRE1_Reset_Mask;
  /* 设置 PPRE1[2:0] 位为 RCC_HCLK 的值 */
  tmpreg |= RCC_HCLK;
  /* 存储新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @简述  配置高速 APB 时钟 (PCLK2).
  * @参数  RCC_HCLK: 配置 APB2 时钟分频. 这个时钟来源于 AHB 时钟 (HCLK).
  *                  这个参数可以取下列值中的一个:
  *                  RCC_HCLK_Div1: APB1 clock = HCLK
  *                  RCC_HCLK_Div2: APB1 clock = HCLK/2
  *                  RCC_HCLK_Div4: APB1 clock = HCLK/4
  *                  RCC_HCLK_Div8: APB1 clock = HCLK/8
  *                  RCC_HCLK_Div16: APB1 clock = HCLK/16
  * @返回  没有
  */
void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* 清 PPRE2[2:0] 位 */
  tmpreg &= CFGR_PPRE2_Reset_Mask;
  /* 设置 PPRE2[2:0] 位为 RCC_HCLK 的值 */
  tmpreg |= RCC_HCLK << 3;
  /* 存储新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @简述  使能或失能指定的 RCC 中断.
  * @参数  RCC_IT: 待使能或者失能的RCC中断源.
  * 
  *                 STM32_互联系列微处理器, 这个参数可以取下列值中的一个:      
  *                                         RCC_IT_LSIRDY: LSI 准备中断
  *                                         RCC_IT_LSERDY: LSE 准备中断
  *                                         RCC_IT_HSIRDY: HSI 准备中断
  *                                         RCC_IT_HSERDY: HSE 准备中断
  *                                         RCC_IT_PLLRDY: PLL 准备中断
  *                                         RCC_IT_PLL2RDY: PLL2 准备中断
  *                                         RCC_IT_PLL3RDY: PLL3 准备中断
  * 
  *                    其它_STM32_微处理器, 这个参数可以取下列值中的一个:      
  *                                         RCC_IT_LSIRDY: LSI 准备中断
  *                                         RCC_IT_LSERDY: LSE 准备中断
  *                                         RCC_IT_HSIRDY: HSI 准备中断
  *                                         RCC_IT_HSERDY: HSE 准备中断
  *                                         RCC_IT_PLLRDY: PLL 准备中断
  *       
  * @参数  NewState: RCC 中断新的状态.
  *                   这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 执行RCC_CIR[12:8]位的字节存取来使能选择的中断s */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS |= RCC_IT;
  }
  else
  {
    /* 执行RCC_CIR[12:8]位的字节存取来禁止选择的中断 */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS &= (uint8_t)~RCC_IT;
  }
}

#ifndef STM32F10X_CL
/**
  * @简述  配置 USB 时钟 (USBCLK).
  * @参数  RCC_USBCLKSource: 指定 USB 的时钟源. 这个时钟来源于 PLL 输出.
  *                          这个参数可以取下列值中的一个:
  *                          RCC_USBCLKSource_PLLCLK_1Div5: PLL 时钟的1.5分频用作 USB 时钟源
  *                          RCC_USBCLKSource_PLLCLK_Div1:  PLL 时钟用作 USB 时钟源
  * @返回  没有
  */
void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_USBCLK_SOURCE(RCC_USBCLKSource));

  *(__IO uint32_t *) CFGR_USBPRE_BB = RCC_USBCLKSource;
}
#else
/**
  * @简述  配置 USB OTG FS 时钟 (OTGFSCLK).
  *        这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_OTGFSCLKSource: 指定 USB OTG FS 时钟源.
  *                            这个时钟来源于 PLL 的输出.
  *                            这个参数可以取下列值中的一个:
  *                            RCC_OTGFSCLKSource_PLLVCO_Div3: PLL VCO clock divided by 2 selected as USB OTG FS clock source
  *                            RCC_OTGFSCLKSource_PLLVCO_Div2: PLL VCO clock divided by 2 selected as USB OTG FS clock source
  * @返回  没有
  */
void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_OTGFSCLK_SOURCE(RCC_OTGFSCLKSource));

  *(__IO uint32_t *) CFGR_OTGFSPRE_BB = RCC_OTGFSCLKSource;
}
#endif /* STM32F10X_CL */ 

/**
  * @简述  配置 ADC 时钟 (ADCCLK).
  * @参数  RCC_PCLK2: 定义 ADC 时钟分频. 这个时钟来源于 APB2 时钟 (PCLK2).
  *                                      这个参数可以取下列值中的一个:
  *                                      RCC_PCLK2_Div2: ADC clock = PCLK2/2
  *                                      RCC_PCLK2_Div4: ADC clock = PCLK2/4
  *                                      RCC_PCLK2_Div6: ADC clock = PCLK2/6
  *                                      RCC_PCLK2_Div8: ADC clock = PCLK2/8
  * @返回  没有
  */
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_ADCCLK(RCC_PCLK2));
  tmpreg = RCC->CFGR;
  /* 清 ADCPRE[1:0] 位 */
  tmpreg &= CFGR_ADCPRE_Reset_Mask;
  /* 设置 ADCPRE[1:0] 位取决于 RCC_PCLK2 的值 */
  tmpreg |= RCC_PCLK2;
  /* 存储新值 */
  RCC->CFGR = tmpreg;
}

#ifdef STM32F10X_CL
/**
  * @简述  配置 I2S2 时钟源(I2S2CLK).
  * @注解
  *   - 这个函数必须在使能I2S2 APB时钟之前调用.
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_I2S2CLKSource: 指定 I2S2 时钟源.
  *                           这个参数可以取下列值中的一个:
  *                           RCC_I2S2CLKSource_SYSCLK: 系统时钟作为 I2S2 时钟源
  *                           RCC_I2S2CLKSource_PLL3_VCO: PLL3 VCO 作为 I2S2 时钟源
  * @返回  没有
  */
void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_I2S2CLK_SOURCE(RCC_I2S2CLKSource));

  *(__IO uint32_t *) CFGR2_I2S2SRC_BB = RCC_I2S2CLKSource;
}

/**
  * @简述  配置 I2S3 时钟源(I2S2CLK).
  * @注解
  *   - 这个函数必须在使能I2S3 APB时钟之前调用.
  *   - 这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_I2S3CLKSource: 指定 I2S3 时钟源.
  *                           这个参数可以取下列值中的一个:
  *                           RCC_I2S2CLKSource_SYSCLK: 系统时钟作为 I2S3 时钟源
  *                           RCC_I2S2CLKSource_PLL3_VCO: PLL3 VCO 作为 I2S3 时钟源
  * @返回  没有
  */
void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource)
{
  /* 指定参数 */
  assert_param(IS_RCC_I2S3CLK_SOURCE(RCC_I2S3CLKSource));

  *(__IO uint32_t *) CFGR2_I2S3SRC_BB = RCC_I2S3CLKSource;
}
#endif /* STM32F10X_CL */

/**
  * @简述  配置外部低速振荡器 (LSE).
  * @参数  RCC_LSE: 指定 LSE 的新状态.
  *                 这个参数可以取下列值中的一个:
  *                 RCC_LSE_OFF: LSE 振荡器 OFF
  *                 RCC_LSE_ON: LSE 振荡器 ON
  *                 RCC_LSE_Bypass: LSE 振荡器与外部时钟旁路
  * @返回  没有
  */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
  /* 检查参数 */
  assert_param(IS_RCC_LSE(RCC_LSE));
  /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
  /* 复位 LSEON 位 */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* 复位 LSEBYP 位 */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* Configure LSE (RCC_LSE_OFF 已经在 code 代码部分被覆盖) */
  switch(RCC_LSE)
  {
    case RCC_LSE_ON:
      /* 设置 LSEON 位 */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
      
    case RCC_LSE_Bypass:
      /* 设置 LSEBYP , LSEON 位 */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;            
      
    default:
      break;      
  }
}

/**
  * @简述  使能或失能内部低速振荡器 (LSI).
  * @注解  如果 IWDG 正在运行，LSI 就不能被禁用.
  * @参数  NewState: LSI 新的状态. 这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_LSICmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

/**
  * @简述  配置 RTC 时钟 (RTCCLK).
  * @注解  只要选择了 RTC 时钟,RTC时钟就不能改变直到备份域复位.
  * @参数  RCC_RTCCLKSource: 指定 RTC 时钟源.
  *                          这个参数可以取下列值中的一个:
  *                          RCC_RTCCLKSource_LSE: LSE 作为 RTC 时钟
  *                          RCC_RTCCLKSource_LSI: LSI 作为 RTC 时钟
  *                          RCC_RTCCLKSource_HSE_Div128: HSE 时钟的 128 分频作为 RTC 时钟
  * @返回  没有
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));
  /* 指定 RTC 时钟源 */
  RCC->BDCR |= RCC_RTCCLKSource;
}

/**
  * @简述  使能或失能 RTC 时钟.
  * @注解  仅当使用 RCC_RTCCLKConfig 函数选择 RTC 时钟后,才使用这个函数.
  * @参数  NewState: RTC 时钟的新状态. 这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * @简述  返回片上时钟的不同频率.
  * @参数  RCC_Clocks: 指向包括时钟频率的 RCC_ClocksTypeDef 结构的指针.
  * @返回  没有
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */
    
  /* 得到系统时钟源 -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;
  
  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
    case 0x04:  /* HSE used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSE_Value;
      break;
    case 0x08:  /* PLL used as system clock */

      /* 得到 PLL 时钟源和倍频因数 ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;
      
#ifndef STM32F10X_CL      
      pllmull = ( pllmull >> 18) + 2;
      
      if (pllsource == 0x00)
      {/* HSI 振荡时钟的2分频作为 PLL 时钟源 */
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* HSE 作为 PLL 时钟源 */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE 振荡时钟被2分频 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_Value >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_Value * pllmull;
        }
      }
#else
      pllmull = pllmull >> 18;
      
      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL 倍频因数 = PLL 输入时钟 * 6.5 */
        pllmull = 13 / 2; 
      }
            
      if (pllsource == 0x00)
      {/* HSI 振荡时钟被2分频作为 PLL 时钟源 */
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* PREDIV1 作为 PLL 时钟源 */
        
        /* 指定 PREDIV1 时钟源和分频系数 */
        prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
        
        if (prediv1source == 0)
        { /* HSE 振荡时钟被选定作为 PREDIV1 时钟源 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_Value / prediv1factor) * pllmull;          
        }
        else
        {/* PLL2 时钟作为 PREDIV1 时钟源 */
          
          /* 指定 PREDIV2 分频系数 和 PLL2 倍频系数 */
          prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2; 
          RCC_Clocks->SYSCLK_Frequency = (((HSE_Value / prediv2factor) * pll2mull) / prediv1factor) * pllmull;                         
        }
      }
#endif /* STM32F10X_CL */ 
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
  }

  /* 计算 HCLK, PCLK1, PCLK2 , ADCCLK 时钟频率 ----------------*/
  /* 得到 HCLK 预分频器 */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK 时钟频率 */
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
  /* 得到 PCLK1 预分频器 */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 时钟频率 */
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* 得到 PCLK2 预分频器 */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 时钟频率 */
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* 得到 ADCCLK 预分频器 */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK 时钟频率 */
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}

/**
  * @简述  使能或失能 AHB 外围设备时钟.
  * @参数  RCC_AHBPeriph: 用于门控时钟的 AHB 外围设备.
  *   
  *        STM32_互联系列微处理器, 这个参数可以取下列值中的一个:      
  *                                RCC_AHBPeriph_DMA1
  *                                RCC_AHBPeriph_DMA2
  *                                RCC_AHBPeriph_SRAM
  *                                RCC_AHBPeriph_FLITF
  *                                RCC_AHBPeriph_CRC
  *                                RCC_AHBPeriph_OTG_FS    
  *                                RCC_AHBPeriph_ETH_MAC   
  *                                RCC_AHBPeriph_ETH_MAC_Tx
  *                                RCC_AHBPeriph_ETH_MAC_Rx
  * 
  *           其它_STM32_微处理器, 这个参数可以取下列值中的一个:         
  *                                RCC_AHBPeriph_DMA1
  *                                RCC_AHBPeriph_DMA2
  *                                RCC_AHBPeriph_SRAM
  *                                RCC_AHBPeriph_FLITF
  *                                RCC_AHBPeriph_CRC
  *                                RCC_AHBPeriph_FSMC
  *                                RCC_AHBPeriph_SDIO
  *   
  * @注解  SRAM 和 FLITF 时钟只有在睡眠的模式下可以无效.
  * @参数  NewState: 指定外围设备时钟新的状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
  }
}

/**
  * @简述  使能或失能高速 APB (APB2) 外围设备时钟.
  * @参数  RCC_APB2Periph: 用于门控时钟的 AHB2 外围设备.
  *                        这个参数可以是下面任意值的组合:
  *          RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3
  * @参数  NewState: 指定外围设备时钟的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/**
  * @简述  使能或失能低速 APB (APB1) 外围设备时钟.
  * @参数  RCC_APB1Periph: 用于门控时钟的 APB1 外围设备.
  *                        这个参数可以是下面任意值的组合:
  *          RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC
  * @参数  NewState: 指定外围设备时钟的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}

#ifdef STM32F10X_CL
/**
  * @简述  控制或释放 AHB 外围设备复位.
  * @注解  这个函数仅仅应用在 STM32 互联系列微处理器.
  * @参数  RCC_AHBPeriph: 指定复位的外围设备.
  *                       这个参数可以是下面任意值的组合:
  *                       RCC_AHBPeriph_OTG_FS 
  *                       RCC_AHBPeriph_ETH_MAC
  * @参数  NewState: 指定外围设备复位的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_AHB_PERIPH_RESET(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBRSTR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBRSTR &= ~RCC_AHBPeriph;
  }
}
#endif /* STM32F10X_CL */ 

/**
  * @简述  控制或释放高速 APB(APB2) 外围设备复位.
  * @参数  RCC_APB2Periph: 指定复位的 APB2 外围设备.
  *                        这个参数可以是下面任意值的组合:
  *          RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3
  * @参数  NewState: 指定外围设备复位的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}

/**
  * @简述  控制或释放低速 APB(APB1) 外围设备复位.
  * @参数  RCC_APB1Periph: 指定复位的 APB1 外围设备.
  *                        这个参数可以是下面任意值的组合:
  *          RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC
  * @参数  NewState: 指定外围设备复位的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1RSTR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1RSTR &= ~RCC_APB1Periph;
  }
}

/**
  * @简述  控制或释放备份域复位.
  * @参数  NewState: 备份域复位的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)NewState;
}

/**
  * @简述  使能或失能时钟安全系统.
  * @参数  NewState: 时钟安全系统的新状态.
  *                  这个参数可以取: ENABLE 或 DISABLE.
  * @返回  没有
  */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_CSSON_BB = (uint32_t)NewState;
}

/**
  * @简述  选择 MCO 引脚上的输出时钟源.
  * @参数  RCC_MCO: 指定要输出的时钟源.
  *   
  *        STM32_互联系列微处理器, 这个参数可以取下列值中的一个:      
  *                                RCC_MCO_NoClock:      没有选择时钟
  *                                RCC_MCO_SYSCLK:       选择系统时钟
  *                                RCC_MCO_HSI:          选择HIS振荡时钟
  *                                RCC_MCO_HSE:          选择HSE振荡时钟
  *                                RCC_MCO_PLLCLK_Div2:  选择PLL时钟两分频
  *                                RCC_MCO_PLL2CLK:      选择PLL2振荡时钟                    
  *                                RCC_MCO_PLL3CLK_Div2: 选择PLL3时钟两分频   
  *                                RCC_MCO_XT1:          选择外部 3-25 MHz 振荡时钟  
  *                                RCC_MCO_PLL3CLK:      选择PLL3振荡时钟 
  * 
  *           其它_STM32_微处理器, 这个参数可以取下列值中的一个:        
  *                                RCC_MCO_NoClock:      没有选择时钟
  *                                RCC_MCO_SYSCLK:       选择系统时钟
  *                                RCC_MCO_HSI:          选择HIS振荡时钟
  *                                RCC_MCO_HSE:          选择HSE振荡时钟
  *                                RCC_MCO_PLLCLK_Div2:  选择PLL时钟两分频
  *   
  * @返回  没有
  */
void RCC_MCOConfig(uint8_t RCC_MCO)
{
  /* 检查参数 */
  assert_param(IS_RCC_MCO(RCC_MCO));

  /* 执行MCO[2:0]位的字节存取来选择MCO源 */
  *(__IO uint8_t *) CFGR_BYTE4_ADDRESS = RCC_MCO;
}

/**
  * @简述  检查指定的 RCC 标志设置与否.
  * @参数  RCC_FLAG: 指定要检查的标志.
  *   
  *        STM32_互联系列微处理器, 这个参数可以取下列值中的一个: 
  *                                RCC_FLAG_HSIRDY:  HSI 振荡时钟就绪
  *                                RCC_FLAG_HSERDY:  HSE 振荡时钟就绪
  *                                RCC_FLAG_PLLRDY:  PLL 时钟就绪
  *                                RCC_FLAG_PLL2RDY: PLL2 时钟就绪    
  *                                RCC_FLAG_PLL3RDY: PLL3 时钟就绪                          
  *                                RCC_FLAG_LSERDY:  LSE 振荡时钟就绪
  *                                RCC_FLAG_LSIRDY:  LSI 振荡时钟就绪
  *                                RCC_FLAG_PINRST:  引脚复位
  *                                RCC_FLAG_PORRST:  POR/PDR复位
  *                                RCC_FLAG_SFTRST:  软件复位
  *                                RCC_FLAG_IWDGRST: 独立看门狗复位
  *                                RCC_FLAG_WWDGRST: 窗口看门狗复位
  *                                RCC_FLAG_LPWRRST: 低电量复位
  * 
  *           其它_STM32_微处理器, 这个参数可以取下列值中的一个:            
  *                                RCC_FLAG_HSIRDY:  HSI 振荡时钟就绪
  *                                RCC_FLAG_HSERDY:  HSE 振荡时钟就绪
  *                                RCC_FLAG_PLLRDY:  PLL 时钟就绪
  *                                RCC_FLAG_LSERDY:  LSE 振荡时钟就绪
  *                                RCC_FLAG_LSIRDY:  LSI 振荡时钟就绪
  *                                RCC_FLAG_PINRST:  引脚复位
  *                                RCC_FLAG_PORRST:  POR/PDR复位
  *                                RCC_FLAG_SFTRST:  软件复位
  *                                RCC_FLAG_IWDGRST: 独立看门狗复位
  *                                RCC_FLAG_WWDGRST: 窗口看门狗复位
  *                                RCC_FLAG_LPWRRST: 低电量复位
  *   
  * @返回  RCC_FLAG 的新状态(SET或RESET).
  */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* 取得 RCC 寄存器引索 */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* CR 寄存器中的检查标志 */
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* BDCR 寄存器中的检查标志 */
  {
    statusreg = RCC->BDCR;
  }
  else                       /* CSR 寄存器中的检查标志 */
  {
    statusreg = RCC->CSR;
  }

  /* 取得标志的位置 */
  tmp = RCC_FLAG & FLAG_Mask;
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* 返回标志的状态 */
  return bitstatus;
}

/**
  * @简述  清除 RCC 复位标志.
  * @注解   这些复位标志是: RCC_FLAG_PINRST, RCC_FLAG_PORRST, RCC_FLAG_SFTRST,
  *                         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST
  * @参数  没有
  * @返回  没有
  */
void RCC_ClearFlag(void)
{
  /* 通过置位 RMVF 位来清除复位标志 */
  RCC->CSR |= CSR_RMVF_Set;
}

/**
  * @简述  检查指定的 RCC 中断是否发生.
  * @参数  RCC_IT: 指定检查 RCC 中断源.
  *   
  *        STM32_互联系列微处理器, 这个参数可以取下列值中的一个: 
  *                                RCC_IT_LSIRDY:  LSI 就绪中断
  *                                RCC_IT_LSERDY:  LSE 就绪中断
  *                                RCC_IT_HSIRDY:  HSI 就绪中断
  *                                RCC_IT_HSERDY:  HSE 就绪中断
  *                                RCC_IT_PLLRDY:  PLL 就绪中断
  *                                RCC_IT_PLL2RDY: PLL2 就绪中断 
  *                                RCC_IT_PLL3RDY: PLL3 就绪中断                    
  *                                RCC_IT_CSS:     时钟安全系统中断
  * 
  *           其它_STM32_微处理器, 这个参数可以取下列值中的一个:      
  *                                RCC_IT_LSIRDY: LSI 就绪中断
  *                                RCC_IT_LSERDY: LSE 就绪中断
  *                                RCC_IT_HSIRDY: HSI 就绪中断
  *                                RCC_IT_HSERDY: HSE 就绪中断
  *                                RCC_IT_PLLRDY: PLL 就绪中断
  *                                RCC_IT_CSS:    时钟安全系统中断
  *   
  * @返回  RCC_IT 的新状态(SETorRESET).
  */
ITStatus RCC_GetITStatus(uint8_t RCC_IT)
{
  ITStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* 检查指定的 RCC 中断状态 */
  if ((RCC->CIR & RCC_IT) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* 返回 RCC_IT 状态 */
  return  bitstatus;
}

/**
  * @简述  清除 RCC 中断挂起位.
  * @参数  RCC_IT: 指定要清除的中断挂起位.
  *   
  *        STM32_互联系列微处理器, 这个参数可以取下列值中的一个: 
  *                                RCC_IT_LSIRDY:  LSI 就绪中断
  *                                RCC_IT_LSERDY:  LSE 就绪中断
  *                                RCC_IT_HSIRDY:  HSI 就绪中断
  *                                RCC_IT_HSERDY:  HSE 就绪中断
  *                                RCC_IT_PLLRDY:  PLL 就绪中断
  *                                RCC_IT_PLL2RDY: PLL2 就绪中断
  *                                RCC_IT_PLL3RDY: PLL3 就绪中断                      
  *                                RCC_IT_CSS:     时钟安全系统中断
  * 
  *           其它_STM32_微处理器, 这个参数可以取下列值中的一个:           
  *                                RCC_IT_LSIRDY:  LSI 就绪中断
  *                                RCC_IT_LSERDY:  LSE 就绪中断
  *                                RCC_IT_HSIRDY:  HSI 就绪中断
  *                                RCC_IT_HSERDY:  HSE 就绪中断
  *                                RCC_IT_PLLRDY:  PLL 就绪中断
  *                                RCC_IT_CSS:     时钟安全系统中断
  * @返回  没有
  */
void RCC_ClearITPendingBit(uint8_t RCC_IT)
{
  /* 检查参数 */
  assert_param(IS_RCC_CLEAR_IT(RCC_IT));

  /* 执行 RCC_CIR[23:16] 位的字节存取来清除选择的中断挂起位*/
  *(__IO uint8_t *) CIR_BYTE3_ADDRESS = RCC_IT;
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
