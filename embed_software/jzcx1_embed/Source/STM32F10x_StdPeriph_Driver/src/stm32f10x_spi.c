/**
  ******************************************************************************
  * @文件    stm32f10x_spi.c
  * @作者    MCD Application Team
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 SPI 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    25/07/2009
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
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/

/* SPI SPE mask */
#define CR1_SPE_Set          ((uint16_t)0x0040)
#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

/* I2S I2SE mask */
#define I2SCFGR_I2SE_Set     ((uint16_t)0x0400)
#define I2SCFGR_I2SE_Reset   ((uint16_t)0xFBFF)

/* SPI CRCNext mask */
#define CR1_CRCNext_Set      ((uint16_t)0x1000)

/* SPI CRCEN mask */
#define CR1_CRCEN_Set        ((uint16_t)0x2000)
#define CR1_CRCEN_Reset      ((uint16_t)0xDFFF)

/* SPI SSOE mask */
#define CR2_SSOE_Set         ((uint16_t)0x0004)
#define CR2_SSOE_Reset       ((uint16_t)0xFFFB)

/* SPI registers Masks */
#define CR1_CLEAR_Mask       ((uint16_t)0x3040)
#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)

/* SPI or I2S mode selection masks */
#define SPI_Mode_Select      ((uint16_t)0xF7FF)
#define I2S_Mode_Select      ((uint16_t)0x0800) 

/* I2S clock source selection masks */
#define I2S2_CLOCK_SRC       ((u32)(0x00020000))
#define I2S3_CLOCK_SRC       ((u32)(0x00040000))
#define I2S_MUL_MASK         ((u32)(0x0000F000))
#define I2S_DIV_MASK         ((u32)(0x000000F0))

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/

/**
  * @简述  将外设 SPIx 寄存器重设为缺省值 (也作用 I2Ss).
  * @参数  SPIx: x为1，2或3用于选定 SPI 外设.
  * @返回  没有
  */
void SPI_I2S_DeInit(SPI_TypeDef* SPIx)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  if (SPIx == SPI1)
  {
    /* 使能 SPI1 复位状态 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
    /* 释放 SPI1 从复位状态 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
  }
  else if (SPIx == SPI2)
  {
    /* 使能 SPI2 复位状态 */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
    /* 释放 SPI2 从复位状态 */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
  }
  else
  {
    if (SPIx == SPI3)
    {
      /* 使能 SPI3 复位状态 */
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
      /* 释放 SPI3 从复位状态 */
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
    }
  }
}

/**
  * @简述  根据 SPI_InitStruct 中指定的参数初始化外设 SPIx 寄存器.
  * @参数  SPIx: x 为1，2或3用于选定 SPI 外设.
  * @参数  SPI_InitStruct: 指向一个包含特定 SPI 外设配置信息的 SPI_InitTypeDef 结构体的指针.
  * @返回  没有
  */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  uint16_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));   
  
  /* 检查 SPI 的参数 */
  assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_DATASIZE(SPI_InitStruct->SPI_DataSize));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
  assert_param(IS_SPI_CRC_POLYNOMIAL(SPI_InitStruct->SPI_CRCPolynomial));

/*---------------------------- SPIx CR1 配置 ------------------------*/
  /* 得到 SPIx CR1 的值 */
  tmpreg = SPIx->CR1;
  /* 清除 BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL 和 CPHA 位 */
  tmpreg &= CR1_CLEAR_Mask;
  /* 配置 SPIx: direction(方向), NSS management(NSS管理), first transmitted bit(第一个发送位), BaudRate prescaler(波特率预分频)
     master/salve mode(主/从模式), CPOL(时钟极性) 和 CPHA(时钟相位) */
  /* 设置 BIDImode, BIDIOE 和 RxONLY 位，按照 SPI_Direction 的值 */
  /* 设置 SSM, SSI and MSTR 位，按照 SPI_Mode 和 SPI_NSS 的值 */
  /* 设置 LSBFirst 位，按照 SPI_FirstBit 的值 */
  /* 设置 BR 位，按照 SPI_BaudRatePrescaler 的值 */
  /* 设置 CPOL 位，按照 SPI_CPOL 的值 */
  /* 设置 CPHA 位，按照 SPI_CPHA 的值 */
  tmpreg |= (uint16_t)((uint32_t)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_Mode |
                  SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |  
                  SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_NSS |  
                  SPI_InitStruct->SPI_BaudRatePrescaler | SPI_InitStruct->SPI_FirstBit);
  /* 写 SPIx CR1 */
  SPIx->CR1 = tmpreg;
  
  /* 激活 SPI 模式 (复位 I2SMOD 位，在 I2SCFGR 寄存器) */
  SPIx->I2SCFGR &= SPI_Mode_Select;		

/*---------------------------- SPIx CRCPOLY 配置 --------------------*/
  /* 写 SPIx CRCPOLY */
  SPIx->CRCPR = SPI_InitStruct->SPI_CRCPolynomial;
}

/**
  * @简述  根据 SPI_InitStruct 中的特定参数初始化 SPIx 外设.
  * @参数  SPIx: x为1，2或3用于选定SPI外设 (配置 I2S 模式).
  * @参数  I2S_InitStruct: 指向一个在模式 I2S 包含特定 SPI 外设配置信息的 SPI_InitTypeDef 结构体的指针.
  * @注解
  *  The function calculates the optimal prescaler needed to obtain the most 
  *  accurate audio frequency (depending on the I2S clock source, the PLL values 
  *  and the product configuration). But in case the prescaler value is greater 
  *  than 511, the default value (0x02) will be configured instead.  *    
  * @返回  没有
  */
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct)
{
  uint16_t tmpreg = 0, i2sdiv = 2, i2sodd = 0, packetlength = 1;
  uint32_t tmp = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  uint32_t sourceclock = 0;
  
  /* 检查 I2S 参数 */
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_I2S_MODE(I2S_InitStruct->I2S_Mode));
  assert_param(IS_I2S_STANDARD(I2S_InitStruct->I2S_Standard));
  assert_param(IS_I2S_DATA_FORMAT(I2S_InitStruct->I2S_DataFormat));
  assert_param(IS_I2S_MCLK_OUTPUT(I2S_InitStruct->I2S_MCLKOutput));
  assert_param(IS_I2S_AUDIO_FREQ(I2S_InitStruct->I2S_AudioFreq));
  assert_param(IS_I2S_CPOL(I2S_InitStruct->I2S_CPOL));  

/*----------------------- SPIx I2SCFGR & I2SPR 配置 -----------------*/
  /* 清除 I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN 和 CHLEN 位 */
  SPIx->I2SCFGR &= I2SCFGR_CLEAR_Mask; 
  SPIx->I2SPR = 0x0002;
  
  /* 得到 I2SCFGR 寄存器的值 */
  tmpreg = SPIx->I2SCFGR;
  
  /* 如果默认值已经写了，重新初始化 i2sdiv 和 i2sodd */
  if(I2S_InitStruct->I2S_AudioFreq == I2S_AudioFreq_Default)
  {
    i2sodd = (uint16_t)0;
    i2sdiv = (uint16_t)2;   
  }
  /* 如果请求的的音频不是默认值，计算预分频 */
  else
  {
    /* 检查祯长(计算预分频器) */
    if(I2S_InitStruct->I2S_DataFormat == I2S_DataFormat_16b)
    {
      /* 包的长度16位 */
      packetlength = 1;
    }
    else
    {
      /* 包的长度32位 */
      packetlength = 2;
    }

    /* 取得 I2S 系统时钟频率 */
    if(((uint32_t)SPIx) == SPI2_BASE)
    {
      /* I2S2 */
      tmp = I2S2_CLOCK_SRC;
    }
    else 
    {
      /* I2S3 */      
      tmp = I2S3_CLOCK_SRC;
    }

    /* 检查 I2S 时钟脉冲源: 仅仅是通用性设备有 PLL3 VCO 时钟 */
#ifdef STM32F10X_CL
    if((RCC->CFGR2 & tmp) != 0)
    {
      /* 得到 RCC PLL3 倍增值的配置位 */
      tmp = (uint32_t)((RCC->CFGR2 & I2S_MUL_MASK) >> 12);

      /* 得到 PLL3 倍增器的值 */      
      if((tmp > 5) && (tmp < 15))
      {
        /* 倍增值在 8 和 14 之间 (禁止 15 ) */
        tmp += 2;
      }
      else
      {
        if (tmp == 15)
        {
          /* 倍增数是 20 */
          tmp = 20;
        }
      }      
      /* 得到 PREDIV2 的值 */
      sourceclock = (uint32_t)(((RCC->CFGR2 & I2S_DIV_MASK) >> 4) + 1);
      
      /* 计算时钟频率，根据 PLL3 和 PREDIV2 的值 */
      sourceclock = (uint32_t) ((HSE_Value / sourceclock) * tmp * 2); 
    }
    else
    {
      /* I2S 时钟是系统时钟: 得到系统时钟频率 */
      RCC_GetClocksFreq(&RCC_Clocks);      
      
      /* 得到来源时钟的值: 根据系统时钟的值 */
      sourceclock = RCC_Clocks.SYSCLK_Frequency;
    }        
#else /* STM32F10X_HD */
    /* I2S 时钟源是系统时钟: 得到系统时钟频率 */
    RCC_GetClocksFreq(&RCC_Clocks);      
      
    /* 得到来源时钟的值: 根据系统时钟的值 */
    sourceclock = RCC_Clocks.SYSCLK_Frequency;    
#endif /* STM32F10X_CL */    

    /* 用浮点指针根据MCLK输出状态计算真实分割器 */
    if(I2S_InitStruct->I2S_MCLKOutput == I2S_MCLKOutput_Enable)
    {
      /* MCLK输出使能 */
      tmp = (uint16_t)(((((sourceclock / 256) * 10) / I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    else
    {
      /* MCLK输出失能 */
      tmp = (uint16_t)(((((sourceclock / (32 * packetlength)) *10 ) / I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    
    /* 删除浮点指针 */
    tmp = tmp / 10;  
      
    /* 检查除数的奇偶 */
    i2sodd = (uint16_t)(tmp & (u16)0x0001);
   
    /* 计算i2sdiv预分频器 */
    i2sdiv = (uint16_t)((tmp - i2sodd) / 2);
   
    /* 取得(SPI_I2SPR[8])寄存器旧的标志位 */
    i2sodd = (uint16_t) (i2sodd << 8);
  }
  
  /* 测试除数是1还是0 或大于 0xFF */
  if ((i2sdiv < 2) || (i2sdiv > 0xFF))
  {
    /* 设置缺省值 */
    i2sdiv = 2;
    i2sodd = 0;
  }

  /* 写 SPIx I2SPR 寄存器的计算值 */
  SPIx->I2SPR = (uint16_t)(i2sdiv | (uint16_t)(i2sodd | (uint16_t)I2S_InitStruct->I2S_MCLKOutput));  
 
  /* 用 SPI_InitStruct 的值，配置 I2S  */
  tmpreg |= (uint16_t)(I2S_Mode_Select | (uint16_t)(I2S_InitStruct->I2S_Mode | \
                  (uint16_t)(I2S_InitStruct->I2S_Standard | (uint16_t)(I2S_InitStruct->I2S_DataFormat | \
                  (uint16_t)I2S_InitStruct->I2S_CPOL))));
 
  /* 写 SPIx I2SCFGR */  
  SPIx->I2SCFGR = tmpreg;   
}

/**
  * @简述  使用缺省值填充 SPI_InitStruct 每一个成员.
  * @参数  SPI_InitStruct : 指向一个将被初始化的 SPI_InitTypeDef 结构的指针.
  * @返回  没有
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
/*--------------- 重置 SPI I初始化结构参数值 -----------------*/
  /* 初始化 SPI_Direction 成员 */
  SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  /* 初始化 SPI_Mode 成员 */
  SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
  /* 初始化 SPI_DataSize 成员 */
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
  /* 初始化 SPI_CPOL 成员 */
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
  /* 初始化 SPI_CPHA 成员 */
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
  /* 初始化 SPI_NSS 成员 */
  SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;
  /* 初始化 SPI_BaudRatePrescaler 成员 */
  SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  /* 初始化 SPI_FirstBit 成员 */
  SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
  /* 初始化 SPI_CRCPolynomial 成员 */
  SPI_InitStruct->SPI_CRCPolynomial = 7;
}

/**
  * @简述  用默认值填充每个 I2S_InitStruct 结构成员.
  * @参数  I2S_InitStruct : 指向将要被初始化的I2S_InitTypeDef 结构指针.
  * @返回  没有
  */
void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct)
{
/*--------------- 重置 I2S 初始化结构参数值 -----------------*/
  /* 初始化 I2S_Mode 成员 */
  I2S_InitStruct->I2S_Mode = I2S_Mode_SlaveTx;
  
  /* 初始化 I2S_Standard 成员 */
  I2S_InitStruct->I2S_Standard = I2S_Standard_Phillips;
  
  /* 初始化 I2S_DataFormat 成员 */
  I2S_InitStruct->I2S_DataFormat = I2S_DataFormat_16b;
  
  /* 初始化 I2S_MCLKOutput 成员 */
  I2S_InitStruct->I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  
  /* 初始化 I2S_AudioFreq 成员 */
  I2S_InitStruct->I2S_AudioFreq = I2S_AudioFreq_Default;
  
  /* 初始化 I2S_CPOL 成员 */
  I2S_InitStruct->I2S_CPOL = I2S_CPOL_Low;
}

/**
  * @简述  使能或者失能 SPI 外设.
  * @参数  SPIx: x 为1，2或3用于选定 SPI 外设.
  * @参数  NewState: SPIx 外围设备的新状态. 
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能 SPI 外设 */
    SPIx->CR1 |= CR1_SPE_Set;
  }
  else
  {
    /* 失能 SPI 外设 */
    SPIx->CR1 &= CR1_SPE_Reset;
  }
}

/**
  * @简述  使能或禁止指定的 I2S 外设 (在 I2S 模式).
  * @参数  SPIx: x为1，2或3用于选定 SPI 外设.
  * @参数  NewState: SPIx 外围设备的新状态. 
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能选定的 SPI 外设 (在 I2S 模式) */
    SPIx->I2SCFGR |= I2SCFGR_I2SE_Set;
  }
  else
  {
    /* 失能选定的 SPI 外设 (在 I2S 模式) */
    SPIx->I2SCFGR &= I2SCFGR_I2SE_Reset;
  }
}

/**
  * @简述  使能或者失能指定的 SPI/I2S 中断.
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  *            - 2 或 3 在 I2S 模式
  * @参数  SPI_I2S_IT: 指定的中断源. 
  *          这个参数可以是下面的值之一:
  *          SPI_I2S_IT_TXE:  发送缓存空中断
  *          SPI_I2S_IT_RXNE: 接收缓存非空中断
  *          SPI_I2S_IT_ERR:  错误中断
  * @参数  NewState: 指定的 SPI/I2S 中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @retval None
  */
void SPI_I2S_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT, FunctionalState NewState)
{
  uint16_t itpos = 0, itmask = 0 ;
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_CONFIG_IT(SPI_I2S_IT));

  /* 得到 SPI/I2S 中断索引 */
  itpos = SPI_I2S_IT >> 4;

  /* 设置 IT */
  itmask = (uint16_t)1 << (uint16_t)itpos;

  if (NewState != DISABLE)
  {
    /* 使能 SPI/I2S 中断 */
    SPIx->CR2 |= itmask;
  }
  else
  {
    /* 失能 SPI/I2S 中断 */
    SPIx->CR2 &= (uint16_t)~itmask;
  }
}

/**
  * @简述  使能或者失能指定 SPIx/I2Sx 的DMA请求.
  * @参数  SPIx: x 可以是
  *             - 1, 2 或 3 在 SPI 模式 
  *             - 2 或 3 在 I2S 模式
  * @参数  SPI_I2S_DMAReq: 指定的 SPI/I2S DMA 请求. 
  *          这个参数可以是下面任意值的组合:
  *          SPI_I2S_DMAReq_Tx: 发送缓冲 DMA 传输请求
  *          SPI_I2S_DMAReq_Rx: 接收缓冲 DMA 传输请求
  * @参数  NewState: 选定的 SPI/I2S DMA 请求的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @retval None
  */
void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_DMAREQ(SPI_I2S_DMAReq));
  if (NewState != DISABLE)
  {
    /* 使能选定的 SPI/I2S DMA 请求 */
    SPIx->CR2 |= SPI_I2S_DMAReq;
  }
  else
  {
    /* 失能选定的 SPI/I2S DMA 请求 */
    SPIx->CR2 &= (uint16_t)~SPI_I2S_DMAReq;
  }
}

/**
  * @简述  通过 SPIx/I2Sx 外设发送数据.
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  *            - 2 or 3 在 I2S 模式
  * @参数  Data : 待发送的数据.
  * @返回  没有
  */
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* 把数据写入 DR 寄存器发送 */
  SPIx->DR = Data;
}

/**
  * @简述  返回最近从 SPIx/I2Sx 外部设备接收的数据. 
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  *            - 2 或 3 在 I2S 模式
  * @返回  接收的数据.
  */
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* 返回 DR 寄存器的数据 */
  return SPIx->DR;
}

/**
  * @简述  为选定的 SPI 软件配置内部 NSS 管脚.
  * @参数  SPIx: x 为1，2或3用于选定SPI外设.
  * @参数  SPI_NSSInternalSoft: 指定的 SPI NSS 内部状态.
  *          这个参数可以是下面的值之一:
  *          SPI_NSSInternalSoft_Set:   设置 NSS 内部引脚
  *          SPI_NSSInternalSoft_Reset: 复位 NSS 内部引脚
  * @返回  没有
  */
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));
  if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
  {
    /* 软件设置 NSS 内部引脚 */
    SPIx->CR1 |= SPI_NSSInternalSoft_Set;
  }
  else
  {
    /* 软件复位 NSS 内部引脚 */
    SPIx->CR1 &= SPI_NSSInternalSoft_Reset;
  }
}

/**
  * @简述  使能或者失能指定的 SPI SS 输出.
  * @参数  SPIx: x 为1，2或3用于选定 SPI 外设.
  * @参数  NewState: 选定的 SPI SS 输出的新状态. 
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能指定的 SPI SS 输出 */
    SPIx->CR2 |= CR2_SSOE_Set;
  }
  else
  {
    /* 失能指定的 SPI SS 输出 */
    SPIx->CR2 &= CR2_SSOE_Reset;
  }
}

/**
  * @简述  设置选定的 SPI 数据大小.
  * @参数  SPIx: x 为1，2或3用于选定 SPI 外设.
  * @参数  SPI_DataSize: 指定的 SPI 数据大小.
  *          这个参数可以是下面的值之一:
  *          SPI_DataSize_16b: 设置数据大小为16位
  *          SPI_DataSize_8b:  设置数据大小为 8位
  * @返回  没有
  */
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DATASIZE(SPI_DataSize));
  /* 清除 DFF 位 */
  SPIx->CR1 &= (uint16_t)~SPI_DataSize_16b;
  /* 设置新的 DFF 位的值 */
  SPIx->CR1 |= SPI_DataSize;
}

/**
  * @简述  传送某个 SPI 的 CRC 校验值.
  * @参数  SPIx: x 为1，2或3用于选定SPI外设.
  * @返回  没有
  */
void SPI_TransmitCRC(SPI_TypeDef* SPIx)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* 使能选中的 SPI CRC 发送 */
  SPIx->CR1 |= CR1_CRCNext_Set;
}

/**
  * @简述  使能或者失能指定 SPI 的传输字 CRC 值计算.
  * @参数  SPIx: x 为1，2或3用于选定SPI外设.
  * @参数  NewState: 选定 SPI 接口的 CRC 计算的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  没有
  */
void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能选定的 SPI CRC 计算 */
    SPIx->CR1 |= CR1_CRCEN_Set;
  }
  else
  {
    /* 失能选定的 SPI CRC 计算 */
    SPIx->CR1 &= CR1_CRCEN_Reset;
  }
}

/**
  * @简述  返回指定 SPI 发送或接收的 CRC 值.
  * @参数  SPIx: x 为1，2或3用于选定 SPI 外设.
  * @参数  SPI_CRC: 将被读取的 CRC 寄存器.
  *          这个参数可以是下面的值之一:
  *          SPI_CRC_Tx: 选择 Tx CRC 寄存器
  *          SPI_CRC_Rx: 选择 Rx CRC 寄存器
  * @返回  选定的 CRC 寄存器的值.
  */
uint16_t SPI_GetCRC(SPI_TypeDef* SPIx, uint8_t SPI_CRC)
{
  uint16_t crcreg = 0;
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_CRC(SPI_CRC));
  if (SPI_CRC != SPI_CRC_Rx)
  {
    /* 得到 Tx CRC 寄存器的值 */
    crcreg = SPIx->TXCRCR;
  }
  else
  {
    /* 得到 Rx CRC 寄存器的值 */
    crcreg = SPIx->RXCRCR;
  }
  /* 返回指定的 CRC 寄存器的值 */
  return crcreg;
}

/**
  * @简述  返回指定 SPI 的 CRC 多项式寄存器值.
  * @参数  SPIx: x 为1，2或3用于选定SPI外设.
  * @返回  指定的 CRC 多项式寄存器值.
  */
uint16_t SPI_GetCRCPolynomial(SPI_TypeDef* SPIx)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* 返回指定 CRC 多项式寄存器值 */
  return SPIx->CRCPR;
}

/**
  * @简述  选择指定 SPI 在双向模式下的数据传输方向.
  * @参数  SPIx: x 为1，2或3用于选定SPI外设.
  * @参数  SPI_Direction: 在双向模式中的数据传输方向. 
  *          这个参数可以是下面的值之一:
  *          SPI_Direction_Tx: 选择 Tx 传送方向
  *          SPI_Direction_Rx: 选择 Rx 接收方向
  * @返回  没有
  */
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, uint16_t SPI_Direction)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DIRECTION(SPI_Direction));
  if (SPI_Direction == SPI_Direction_Tx)
  {
    /* 设置只发送模式 */
    SPIx->CR1 |= SPI_Direction_Tx;
  }
  else
  {
    /* 设置只接收模式 */
    SPIx->CR1 &= SPI_Direction_Rx;
  }
}

/**
  * @简述  检查指定的 SPI/I2S 标记是否被置位.
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  *            - 2 或 3 在 I2S 模式
  * @参数  SPI_I2S_FLAG: 指定的 SPI/I2S 标志. 
  *          这个参数可以是下面的值之一:
  *          SPI_I2S_FLAG_TXE:  传输缓冲为空标志.
  *          SPI_I2S_FLAG_RXNE: 接收缓冲不空标志.
  *          SPI_I2S_FLAG_BSY:  忙标志.
  *          SPI_I2S_FLAG_OVR:  溢出标志.
  *          SPI_FLAG_MODF:     模式错误标志.
  *          SPI_FLAG_CRCERR:   CRC 校验错误标志.
  *          I2S_FLAG_UDR:      空栈读出错标志.
  *          I2S_FLAG_CHSIDE:   通道边界标志.
  * @返回  SPI_I2S_FLAG 的新状态 (SET 或 RESET).
  */
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_FLAG(SPI_I2S_FLAG));
  /* 检查指定的 SPI/I2S 标志状态 */
  if ((SPIx->SR & SPI_I2S_FLAG) != (uint16_t)RESET)
  {
    /* 设置 SPI_I2S_FLAG */
    bitstatus = SET;
  }
  else
  {
    /* 复位 SPI_I2S_FLAG */
    bitstatus = RESET;
  }
  /* 返回 SPI_I2S_FLAG 状态 */
  return  bitstatus;
}

/**
  * @简述  清除 SPIx CRC 错误 (CRCERR) 标志.
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  * @参数  SPI_I2S_FLAG: 指定要清除的 SPI 标志. 
  *           这个函数仅仅用于清除 CRCERR 标志.
  * @注解
*        - OVR (过速错误)中断挂起位可以被软件按顺序清除: 一个读  
*          SPI_DR寄存器操作 (SPI_I2S_ReceiveData()) 跟着一个读  
*          SPI_SR寄存器操作 (SPI_I2S_GetITStatus()).
*        - UDR (空栈读错误) 中断挂起位可以被一个读SPI_SR寄存器操
*          作清除(SPI_I2S_GetITStatus())。
*        - MODF (模式错误) 中断挂起位被软件按顺序清除:一个读/写 
*          SPI_SR寄存器操作(SPI_I2S_GetITStatus())跟着一个写SPI_CR1 
*          寄存器操作 (SPI_Cmd() 使能 SPI).  
  * @返回  没有
  */
void SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)
{
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_FLAG(SPI_I2S_FLAG));
    
    /* 清除选择的 SPI CRC 错误 (CRCERR) 标志 */
    SPIx->SR = (uint16_t)~SPI_I2S_FLAG;
}

/**
  * @简述  检查指定的的 SPI/I2S 中断是否发生了.
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  *            - 2 或 3 在 I2S 模式
  * @参数  SPI_I2S_IT: 待检查的 SPI/I2S 中断源. 
  *          这个参数可以是下面的值之一:
  *          SPI_I2S_IT_TXE:  传输缓冲为空中断.
  *          SPI_I2S_IT_RXNE: 接收缓冲不空中断.
  *          SPI_I2S_IT_OVR:  溢出中断.
  *          SPI_IT_MODF:     模式错误中断.
  *          SPI_IT_CRCERR:   CRC 校验错误中断.
  *          I2S_IT_UDR:      空栈读错误中断.
  * @返回  SPI_I2S_IT 的新状态 (SET 或 RESET).
  */
ITStatus SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT)
{
  ITStatus bitstatus = RESET;
  uint16_t itpos = 0, itmask = 0, enablestatus = 0;

  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_IT(SPI_I2S_IT));

  /* 得到 SPI/I2S 中断索引 */
  itpos = 0x01 << (SPI_I2S_IT & 0x0F);

  /* 得到 SPI/I2S 中断号 */
  itmask = SPI_I2S_IT >> 4;

  /* 设置中断号 */
  itmask = 0x01 << itmask;

  /* 取得 SPI_I2S_IT 使能状态位 */
  enablestatus = (SPIx->CR2 & itmask) ;

  /* 检查指定的 SPI/I2S 中断状态 */
  if (((SPIx->SR & itpos) != (uint16_t)RESET) && enablestatus)
  {
    /* 设置 SPI_I2S_IT */
    bitstatus = SET;
  }
  else
  {
    /* 复位 SPI_I2S_IT */
    bitstatus = RESET;
  }
  /* 返回 SPI_I2S_IT 状态 */
  return bitstatus;
}

/**
  * @简述  清除指定的 SPI CRC 错误 (CRCERR) 中断挂起位.
  * @参数  SPIx: x 可以是
  *            - 1, 2 或 3 在 SPI 模式 
  * @参数  SPI_I2S_IT: 清除指定的中断挂起位.
  *           这个函数只清除 CRCERR 中断挂起位.   
  * @note
*          - OVR (过速错误)中断挂起位可以被软件按顺序清除: 一个读  
*            SPI_DR寄存器操作 (SPI_I2S_ReceiveData()) 跟着一个读  
*            SPI_SR寄存器操作 (SPI_I2S_GetITStatus()).
*          - UDR (空栈读错误) 中断挂起位可以被一个读SPI_SR寄存器操
*            作清除(SPI_I2S_GetITStatus())。
*          - MODF (模式错误) 中断挂起位被软件按顺序清除:一个读/写 
*            SPI_SR寄存器操作(SPI_I2S_GetITStatus())跟着一个写SPI_CR1 
*            寄存器操作 (SPI_Cmd() 使能 SPI).  
  * @返回  没有
  */
void SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT)
{
  uint16_t itpos = 0;
  /* 检查参数 */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_IT(SPI_I2S_IT));

  /* 得到 SPI 中断索引 */
  itpos = 0x01 << (SPI_I2S_IT & 0x0F);

  /* 清除指定的 SPI CRC 错误 (CRCERR) 中断挂起位 */
  SPIx->SR = (uint16_t)~itpos;
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
