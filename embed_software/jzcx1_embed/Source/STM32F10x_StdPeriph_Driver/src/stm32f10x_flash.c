/**
  ******************************************************************************
  * @文件    stm32f10x_flash.c
  * @作者    MCD 应用程序组
  * @版本    V3.1.0
  * @日期    06/19/2009
  * @简述    这个文件 提供所有 FLASH 固件函数.
  * @翻译    ANSON/sweet1985 Email:airanson110@126.com
  * @日期    23/07/2009
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
#include "stm32f10x_flash.h"

/* 自用类型 --------------------------------------------------------------*/
/* 自用定义 --------------------------------------------------------------*/ 

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)

/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout             ((uint32_t)0x00000FFF)
#define ProgramTimeout           ((uint32_t)0x0000000F)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 -----------------------------------------------------------*/
/* 自用函数原型 -------------------------------------------------------*/
/* 自用函数 -----------------------------------------------------------*/
static void delay(void);
/**
  * @简述  设置代码延时值.
  * @参数  FLASH_Latency: 指定的 FLASH 延时值.
  *          这个参数可以是下面的值之一:
  *          FLASH_Latency_0: FLASH 0个延时周期
  *          FLASH_Latency_1: FLASH 1个延时周期
  *          FLASH_Latency_2: FLASH 2个延时周期
  * @返回  没有
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* 读 ACR 寄存器 */
  tmpreg = FLASH->ACR;  
  
  /* 设置延时值 */
  tmpreg &= ACR_LATENCY_Mask;
  tmpreg |= FLASH_Latency;
  
  /* 写 ACR 寄存器 */
  FLASH->ACR = tmpreg;
}

/**
  * @简述  使能或者失能 FLASH 半周期访问.
  * @参数  FLASH_HalfCycleAccess: 指定的 FLASH 半周期访问模式.
  *          这个参数可以是下面的值之一:
  *          FLASH_HalfCycleAccess_Enable:  半周期访问使能
  *          FLASH_HalfCycleAccess_Disable: 半周期访问失能
  * @返回  没有
  */
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
{
  /* 检查参数 */
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* 使能或者失能半周期访问 */
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/**
  * @简述  使能或者失能预取缓存.
  * @参数  FLASH_PrefetchBuffer: 指定的预取缓存状态.
  *          这个参数可以是下面的值之一:
  *          FLASH_PrefetchBuffer_Enable:  使能预取指缓存
  *          FLASH_PrefetchBuffer_Disable: 失能预取指缓存
  * @返回  没有
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* 检查参数 */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* 使能或者失能预取缓存 */
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

/**
  * @简述  解锁 FLASH 编写擦除控制器.
  * @参数  没有
  * @返回  没有
  */
void FLASH_Unlock(void)
{
  /* 授权 FPEC 存取 */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/**
  * @简述  锁定 FLASH 编写擦除控制器.
  * @参数  没有
  * @返回  没有
  */
void FLASH_Lock(void)
{
  /* 设置锁定位来锁定 FPEC 和 FCR */
  FLASH->CR |= CR_LOCK_Set;
}

/**
  * @简述  擦除一个 FLASH 页面.
  * @参数  Page_Address: 需要擦除的页.
  * @返回  FLASH 状态: 返回值可以是: FLASH_BUSY, FLASH_ERROR_PG,
  *                    FLASH_ERROR_WRP, FLASH_COMPLETE 或 FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_FLASH_ADDRESS(Page_Address));
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* 如果前一个操作已经完成 */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 如果擦除操作完成，失能 PER 位 */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  /* 返回擦除状态 */
  return status;
}

/**
  * @简述  擦除全部 FLASH 页面.
  * @参数  没有
  * @返回  FLASH 状态: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                    FLASH_COMPLETE 或 FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 如果前一个操作已经完成，擦除所有的页 */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 如果擦除操作完成，失能 MER 位 */
      FLASH->CR &= CR_MER_Reset;
    }
  }	   
  /* 返回擦除状态 */
  return status;
}

/**
  * @简述  擦除 FLASH 选择字节.
  * @参数  没有
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE 或 FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseOptionBytes(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* 如果前一个操作完成，擦除选项字节 */
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* 如果擦除操作完成，失能 OPTER 位 */
      FLASH->CR &= CR_OPTER_Reset;
       
      /* 使能字节编程操作选项 */
      FLASH->CR |= CR_OPTPG_Set;
      /* 使能读出访问 */
      OB->RDP= RDP_Key; 
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_TIMEOUT)
      {
        /* 如果编程操作完成，失能 OPTPG 位 */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* 失能 OPTPG 位 */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* 返回擦除状态 */
  return status;
}

/**
  * @简述  在指定地址编写一个字.
  * @参数  Address: 将要编程的地址.
  * @参数  Data: 指定被编程的数据.
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE 或 FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_FLASH_ADDRESS(Address));
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 如果前一个操作完成，编程新的第一个半字 */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = (uint16_t)Data;
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
 
    if(status == FLASH_COMPLETE)
    {
      /* 如果前一个操作完成，编程新的第二个半字 */
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;
    
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
        
      if(status != FLASH_TIMEOUT)
      {
        /* 失能 PG 位 */
        FLASH->CR &= CR_PG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* 失能 PG 位 */
        FLASH->CR &= CR_PG_Reset;
      }
     }
  }
  /* 返回编程状态 */
  return status;
}

/**
  * @简述  在特定地址编程一个半字.
  * @参数  Address: 被编程的地址.
  * @参数  Data: 指定被编程的半字数据.
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE 或 FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_FLASH_ADDRESS(Address));
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 如果前一个操作完成，编程新的数据 */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = Data;
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 如果编程操作完成，失能 PG 位 */
      FLASH->CR &= CR_PG_Reset;
    }
  } 
  /* 返回编程状态 */
  return status;
}

/**
  * @简述  在指定的选项字节数据地址上编写一个半字.
  * @参数  Address: 被编程的地址.
  *                 这个参数可以是 0x1FFFF804 或 0x1FFFF806. 
  * @参数  Data: 被编写的数据.
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE 或 FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_OB_DATA_ADDRESS(Address));
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    /* 使能字节编程操作选项 */
    FLASH->CR |= CR_OPTPG_Set; 
    *(__IO uint16_t*)Address = Data;
    
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 如果编程操作完成，失能 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* 返回选项字节数据的编程状态 */
  return status;
}

/**
  * @简述  对期望的页面写保护
  * @参数  FLASH_Pages: 待写保护页面的地址.
  *                 这个参数可以是:
  *                 STM32_低密度处理器: value between FLASH_WRProt_Pages0to3 和 FLASH_WRProt_Pages28to31  
  *                 STM32_中密度处理器: value between FLASH_WRProt_Pages0to3 和 FLASH_WRProt_Pages124to127
  *                 STM32_高密度处理器: value between FLASH_WRProt_Pages0to1 和 FLASH_WRProt_Pages60to61 或 FLASH_WRProt_Pages62to255
  *                 STM32_连通性处理器: value between FLASH_WRProt_Pages0to1 和 FLASH_WRProt_Pages60to61 或 FLASH_WRProt_Pages62to127    
  *                 FLASH_WRProt_AllPages
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE 或 FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
  uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* 检查参数 */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (uint32_t)(~FLASH_Pages);
  WRP0_Data = (uint16_t)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (uint16_t)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (uint16_t)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (uint16_t)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;
    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;
      
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;
      
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;
      
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;
     
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_TIMEOUT)
    {
      /* 如果编程操作完成，失能 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* 返回写保护操作状态 */
  return status;       
}

/**
  * @简述  使能或者失能读出保护.
  * @注解   如果在调用这个函数之前使用者已经编程了其它的选项字节，在这个
  *         函数擦除所有的选项字节以后他必须重新编程那些选项字节.
  * @参数  Newstate: 读出保护的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* 如果擦除操作完成，失能 OPTER 位 */
      FLASH->CR &= CR_OPTER_Reset;
      /* 使能字节编程操作选项 */
      FLASH->CR |= CR_OPTPG_Set; 
      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }
      /* 等待最后一个操作完成 */
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_TIMEOUT)
      {
        /* 如果编程操作完成，失能 OPTPG 位 */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_TIMEOUT)
      {
        /* 失能 OPTER 位 */
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* 返回写保护操作状态 */
  return status;      
}

/**
  * @简述  编写FLASH用户选择字节: IWDG_SW / RST_STOP / RST_STDBY.
  * @参数  OB_IWDG: 选择 IWDG 模式
  *          这个参数可以是下面的值之一:
  *          OB_IWDG_SW: 选择软件独立看门狗
  *          OB_IWDG_HW: 选择硬件独立看门狗
  * @参数  OB_STOP: 当进入 STOP 模式产生复位事件.
  *          这个参数可以是下面的值之一:
  *          OB_STOP_NoRST: 进入 STOP 模式不产生复位
  *          OB_STOP_RST:   进入 STOP 模式产生复位
  * @参数  OB_STDBY: 当进入 Standby 模式产生复位事件.
  *          这个参数可以是下面的值之一:
  *          OB_STDBY_NoRST: 进入 Standby 模式不产生复位
  *          OB_STDBY_RST:   进入 Standby 模式不产生复位
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* 检查参数 */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* 授权小信息块编程 */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* 等待最后一个操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* 使能字节编程操作选项 */
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->USER = OB_IWDG | (uint16_t)(OB_STOP | (uint16_t)(OB_STDBY | ((uint16_t)0xF8))); 
  
    /* 等待最后一个操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 如果编程操作完成，失能 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* 返回选项字节编程状态 */
  return status;
}

/**
  * @简述  返回 FLASH 用户选择字节的值.
  * @参数  没有
  * @返回  FLASH 用户选择字节的值:IWDG_SW(Bit0), RST_STOP(Bit1) 和 RST_STDBY(Bit2).
  */
uint32_t FLASH_GetUserOptionByte(void)
{
  /* 返回用户选项字节值 */
  return (uint32_t)(FLASH->OBR >> 2);
}

/**
  * @简述  返回 FLASH 写保护选择字节寄存器的值.
  * @参数  没有
  * @返回  FLASH 写保护选项字节寄存器值
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* 返回 FLASH 写保护寄存器值 */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @简述  检查 FLASH 读出保护设置与否.
  * @参数  没有
  * @返回  FLASH 读出保护的状态 (SET 或 RESET)
  */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if ((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @简述  检查 FLASH 预取指缓存设置与否.
  * @参数  没有
  * @返回  FLASH 预取缓冲区的状态 (SET 或 RESET).
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;
  
  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* 检查 FLASH 预取指缓存设置与否 (SET 或 RESET) */
  return bitstatus; 
}

/**
  * @简述  使能或者失能指定 FLASH 中断.
  * @参数  FLASH_IT: 待使能或者失能的指定 FLASH 中断源.
  *          这个参数可以是下面值的任意组合:
  *          FLASH_IT_ERROR: FLASH 错误中断源
  *          FLASH_IT_EOP:   FLASH 操作结束中断源
  * @参数  NewState: 指定的 FLASH 中断的新状态.
  *                  这个参数可以是: ENABLE 或 DISABLE.      
  * @返回  没有 
  */
void FLASH_ITConfig(uint16_t FLASH_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if(NewState != DISABLE)
  {
    /* 使能中断源 */
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* 失能中断源 */
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
}

/**
  * @简述  检查指定的 FLASH 标志位设置与否.
  * @参数  FLASH_FLAG: 需要检查的标志.
  *          这个参数可以是下面的值之一:
  *          FLASH_FLAG_BSY:      FLASH 忙标志           
  *          FLASH_FLAG_PGERR:    FLASH 程序错误标志       
  *          FLASH_FLAG_WRPRTERR: FLASH 页面写保护错误标志    
  *          FLASH_FLAG_EOP:      FLASH 操作结束标志          
  *          FLASH_FLAG_OPTERR:   FLASH 选项字节错误标志   
  * @返回  FLASH_FLAG 的新状态 (SET 或 RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint16_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* 返回 FLASH_FLAG 的新状态 (SET or RESET) */
  return bitstatus;
}

/**
  * @简述  清除 FLASH 待处理标志位.
  * @参数  FLASH_FLAG: 需要清除的标志.
  *          这个参数可以是下面值的任意组合:        
  *          FLASH_FLAG_PGERR:    FLASH 程序错误标志       
  *          FLASH_FLAG_WRPRTERR: FLASH W页面写保护错误标志      
  *          FLASH_FLAG_EOP:      FLASH 操作结束的标志          
  * @返回  没有
  */
void FLASH_ClearFlag(uint16_t FLASH_FLAG)
{
  /* 检查参数 */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* 清除标志 */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @简述  返回 FLASH 状态.
  * @参数  没有
  * @返回  FLASH 状态: 返回值可以是: FLASH_BUSY, FLASH_ERROR_PG,
  *                    FLASH_ERROR_WRP 或 FLASH_COMPLETE
  */
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* 返回 FLASH 状态 */
  return flashstatus;
}

/**
  * @简述  等待一个 Flash 操作来结束或一个 TIMEOUT 事件发生.
  * @参数  Timeout: FLASH 编程超时时间
  * @返回  FLASH Status: 返回值可以是: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE 或 FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* 检查 FLASH 状态 */
  status = FLASH_GetStatus();
  /* 等待 FLASH 操作完成或者发生超时 */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    delay();
    status = FLASH_GetStatus();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* 返回操作状态 */
  return status;
}

/**
  * @简述  插入一个延时.
  * @参数  没有
  * @返回  没有
  */
static void delay(void)
{
  __IO uint32_t i = 0;
  for(i = 0xFF; i != 0; i--)
  {
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
