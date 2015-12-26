/**
  ******************************************************************************
  * @文件    stm32f10x_gpio.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 GPIO 固件函数.
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
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/

/* ------------ RCC registers bit address in the alias region ----------------*/
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

/* --- EVENTCR Register -----*/

/* Alias word address of EVOE bit */
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((uint8_t)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))


/* ---  MAPR Register ---*/ 
/* Alias word address of MII_RMII_SEL bit */ 
#define MAPR_OFFSET                 (AFIO_OFFSET + 0x04) 
#define MII_RMII_SEL_BitNumber      ((u8)0x17) 
#define MAPR_MII_RMII_SEL_BB        (PERIPH_BB_BASE + (MAPR_OFFSET * 32) + (MII_RMII_SEL_BitNumber * 4))


#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/**
  * @简述   将GPIO部件的寄存器复位为默认值
  * @参数   GPIOx：x可为A到G来选择特定的GPIO设备
  * @返回   没有
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* 检查核对参数*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  if (GPIOx == GPIOA)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
  }    
  else if (GPIOx == GPIOE)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
  } 
  else if (GPIOx == GPIOF)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
  }
  else
  {
    if (GPIOx == GPIOG)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
    }
  }
}

/**
  * @简述  将备用功能（重映射，事件控制及EXTI配置）寄存器复位为默认值
  * @参数  没有
  * @返回  没有
  */
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/**
  * @简述  按照GPIO_InitStruct(结构体)的特定参数初始化GPIO部件
  * @参数  GPIOx：x可为A到G来选择特定的GPIO设备，
  *        GPIO_InitStruct: 指向GPIO_InitTypeDef结构的指针，它包含特定GPIO部件的配置信息.
  * @返回  没有
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO 模式配置 --------------------------------*/
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
  if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
  { 
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode */
    currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
  }
/*-------------------- GPIO CRL(端口控制寄存器低字节) 配置 -------------------*/
  /* 配置低8位端口管脚 */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* 取得端口引脚的位置 */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* 清空相应的低位控制寄存器 */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* 写相应的模式配置位 */
        tmpreg |= (currentmode << pos);
        /* 复位相应的输出数据寄存器 (ODR) 位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        }
        else
        {
          /* 置位相应的输出数据寄存器 (ODR) 位 */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }
/*-------------------- GPIO CRH(端口控制寄存器高字节) 配置 -------------------*/
  /* 配置高8位端口引脚 */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* 取得端口引脚的位置 */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* 清空相应的低位控制寄存器 */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* 写相应的模式配置位 */
        tmpreg |= (currentmode << pos);
        /* 复位相应的输出数据寄存器 (ODR) 位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* 置位相应的输出数据寄存器 (ODR) 位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

/**
  * @简述  把GPIO_InitStruct中的每一个参数按缺省值填入.
  * @参数  GPIO_InitStruct : 指向GPIO_InitTypeDef结构，该结构将被初始化
  * @返回  没有
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* 复位GPIO初始化结构参数值 */
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/**
  * @简述  读取指定端口管脚的输入.
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  *        GPIO_Pin:  被读取的引脚位.
  *                   这个参数GPIO_Pin_x可以选择0至15.
  * @返回  指定端口管脚的输入值.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @简述  从特定的GPIO输入数据端口读取.
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  * @返回  指定的 GPIO 端口管脚的输入值.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/**
  * @简述  读取指定端口管脚的输出(位).
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  *        GPIO_Pin: 需要读取的端口位.
  *                  这个参数GPIO_Pin_x可以选择0至15.
  * @返回  输出端口的输出值.
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @简述  读取指定的GPIO端口输出.
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  * @返回  指定 GPIO 输出数据端口值.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((uint16_t)GPIOx->ODR);
}

/**
  * @简述  设置指定的数据端口位.
  * @参数  x可为A到G来选择特定的GPIO设备.
  *        GPIO_Pin: 确定端口位.
  *                  该参数可为GPIO_Pin_x的任意组合,其中x为0到15.
  * @返回  没有
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/**
  * @简述  清除指定的数据端口位.
  * @参数  x可为A到G来选择特定的GPIO设备.
  *        GPIO_Pin: 确定端口位.
  *                  该参数可为GPIO_Pin_x的任意组合,其中x为0到15.
  * @返回  没有
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/**
  * @简述  设置或者清除指定的数据端口位.
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  *        GPIO_Pin: 指定被写入的端口位.
  *                  该参数可为GPIO_Pin_x的任意组合,其中x为0到15.
  *        BitVal:   该参数指定被写入到选定位.
  *                  BitVal必须为BitAction枚举类型值: 
  *                  @arg Bit_RESET:清除端口引脚，
  *                  @arg Bit_SET:置位端口引脚
  * @返回  没有
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

/**
  * @简述  向指定GPIO数据端口写入数据.
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  *        PortVal: 写入到数据端口寄存器的值.
  * @返回  没有
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/**
  * @简述  锁定GPIO引脚配置寄存器.
  * @参数  GPIOx: x可为A到G来选择特定的GPIO设备.
  *        GPIO_Pin:指定被写入的端口位.
  *                 该参数可为GPIO_Pin_x的任意组合,其中x为0到15.
  * @返回  没有
  */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint32_t tmp = 0x00010000;
  
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  tmp |= GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Reset LCKK bit */
  GPIOx->LCKR =  GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Read LCKK bit*/
  tmp = GPIOx->LCKR;
  /* Read LCKK bit*/
  tmp = GPIOx->LCKR;
}

/**
  * @简述  选择 GPIO 管脚用作事件输出.
  * @参数  GPIO_PortSource: 选择用作事件输出的GPIO端口
  *                         GPIOx:x可为A到E来选择特定的GPIO设备.
  *        GPIO_PinSource: 事件输出的管脚.
  *                        该参数可为GPIO_PinSourcex,这里x可为0到15.
  * @返回  没有
  */
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmpreg = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    
  tmpreg = AFIO->EVCR;
  /* Clear the PORT[6:4] and PIN[3:0] bits */
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (uint32_t)GPIO_PortSource << 0x04;
  tmpreg |= GPIO_PinSource;
  AFIO->EVCR = tmpreg;
}

/**
  * @简述  使能或者失能事件输出.
  * @参数  NewState: 事件输出的新状态.
  *                  这个参数可以取：ENABLE或者DISABLE.
  * @返回  没有
  */
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) EVCR_EVOE_BB = (uint32_t)NewState;
}

/**
  * @简述  改变指定管脚的映射.
  * @参数  GPIO_Remap: 选择重映射的管脚.
  *                    GPIO_Remap 用来改变备用功能映射,它的可用值:
  *     @arg GPIO_Remap_SPI1
  *     @arg GPIO_Remap_I2C1
  *     @arg GPIO_Remap_USART1
  *     @arg GPIO_Remap_USART2
  *     @arg GPIO_PartialRemap_USART3
  *     @arg GPIO_FullRemap_USART3
  *     @arg GPIO_PartialRemap_TIM1
  *     @arg GPIO_FullRemap_TIM1
  *     @arg GPIO_PartialRemap1_TIM2
  *     @arg GPIO_PartialRemap2_TIM2
  *     @arg GPIO_FullRemap_TIM2
  *     @arg GPIO_PartialRemap_TIM3
  *     @arg GPIO_FullRemap_TIM3
  *     @arg GPIO_Remap_TIM4
  *     @arg GPIO_Remap1_CAN1
  *     @arg GPIO_Remap2_CAN1
  *     @arg GPIO_Remap_PD01
  *     @arg GPIO_Remap_TIM5CH4_LSI
  *     @arg GPIO_Remap_ADC1_ETRGINJ
  *     @arg GPIO_Remap_ADC1_ETRGREG
  *     @arg GPIO_Remap_ADC2_ETRGINJ
  *     @arg GPIO_Remap_ADC2_ETRGREG
  *     @arg GPIO_Remap_ETH
  *     @arg GPIO_Remap_CAN2  
  *     @arg GPIO_Remap_SWJ_NoJTRST
  *     @arg GPIO_Remap_SWJ_JTAGDisable
  *     @arg GPIO_Remap_SWJ_Disable
  *     @arg GPIO_Remap_SPI3
  *     @arg GPIO_Remap_TIM2ITR1_PTP_SOF
  *     @arg GPIO_Remap_PTP_PPS  
  * @注解  如果 GPIO_Remap_TIM2ITR1_PTP_SOF 被激活  TIM2 ITR1 连接到以太网 PTP 输出
  *        当复位的时候 TIM2 ITR1 连接到 USB OTG SOF 输出.       
  * @参数  NewState: 端口引脚重映射的新状态.
  *                  该参数可为：ENABLE或DISABLE.
  * @返回  没有
  */
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
  uint32_t tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

  /* 检查参数 */
  assert_param(IS_GPIO_REMAP(GPIO_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  tmpreg = AFIO->MAPR;

  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
  tmp = GPIO_Remap & LSB_MASK;

  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
  {
    tmp1 = ((uint32_t)0x03) << tmpmask;
    tmpreg &= ~tmp1;
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  else
  {
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }

  if (NewState != DISABLE)
  {
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }

  AFIO->MAPR = tmpreg;
}

/**
  * @简述  选择GPIO引脚作为 EXTI 线.
  * @参数  GPIO_PortSource: 选择将要用作 EXTI 线的源的 GPIO 口.
  *                         GPIOx:x可为A到G来选择特定的GPIO设备.
  *        GPIO_PinSource:  指定需要配置的EXTI线.
  *                         这个参数可以是GPIO_PinSourcex,其中x为0到15.
  * @返回  没有
  */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}

/**
  * @简述  指定以太网接口.
  * @注解  此功能仅适用于STM32连接线配置.  
  * @参数  GPIO_ETH_MediaInterface: 指定接口模式.
  *                                 这个参数可以选择:
  *                                 GPIO_ETH_MediaInterface_MII: MII mode    ->独立接口
  *                                 GPIO_ETH_MediaInterface_RMII: RMII mode  ->精简独立接口  
  * @返回  没有
  */
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface) 
{ 
  assert_param(IS_GPIO_ETH_MEDIA_INTERFACE(GPIO_ETH_MediaInterface)); 

  /* 配置 MII_RMII 选择位 */ 
  *(__IO uint32_t *) MAPR_MII_RMII_SEL_BB = GPIO_ETH_MediaInterface; 
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
