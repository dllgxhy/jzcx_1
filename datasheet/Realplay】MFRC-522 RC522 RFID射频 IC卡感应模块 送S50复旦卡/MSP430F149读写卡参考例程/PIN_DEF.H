
//***************************************************************************//
//                                                                           //
//                       引脚功能定义                                        //
//                                                                           //
//***************************************************************************//

#define LED_RED                BIT0  // P2.0 红灯亮起
#define LED_GRE                BIT0  // P2.0 绿灯亮起
#define RF_LPCTL               BIT3  // P2.3 射频卡休眠控制 ------RST   
#define RF_SS                  BIT7  // p2.7 射频卡从机选择(SS)---SDA
#define RF_SCLK                BIT6  // p2.6 射频卡数据时钟输出(SCLK)
#define RF_DATA_OUT            BIT5  // p2.5 射频卡数据输出(MOSI)
#define RF_DATA_IN             BIT1  // p2.1 射频模块输入(MISO) 

//****************************************************************************
//                                                                           *
//                           操作宏定义                                      *
//                                                                           *
//****************************************************************************

//------------------------------- RC522操作宏 ------------------------------//
//#define    RF_POWER_ON             P8OUT &=~RF_PCTL                      // 射频模块上电
//#define    RF_POWER_OFF            P8OUT |= RF_PCTL                      // 射频模块下电
#define    RF_POWER_ON             _NOP()                                // 射频模块上电
#define    RF_POWER_OFF            _NOP()                                // 射频模块下电
#define    RED_LED_ON              P2OUT &=~LED_RED                      // 红灯亮起
#define    RED_LED_OFF             P2OUT |= LED_RED                      // 红灯熄灭
#define    GRE_LED_ON              P2OUT &=~LED_GRE                      // 绿灯亮起
#define    GRE_LED_OFF             P2OUT |= LED_GRE                      // 绿灯熄灭

#define    RF_SLEEP                P2OUT &=~RF_LPCTL                     // 射频模块唤醒
#define    RF_WAKEUP               P2OUT |= RF_LPCTL                     // 射频模块休眠
#define    SCLK_HIGH               P2OUT |= RF_SCLK                      // 数据时钟置高
#define    SCLK_LOW                P2OUT &=~RF_SCLK                      // 数据时钟置低
#define    DATA_OUT_HIGH           P2OUT |= RF_DATA_OUT                  // 数据置高
#define    DATA_OUT_LOW            P2OUT &=~RF_DATA_OUT                  // 数据置低
#define    SS_HIGH                 P2OUT |= RF_SS                        // 从机选择无效
#define    SS_LOW                  P2OUT &=~RF_SS                        // 从机选择有效
#define    DATA_IN                 P2IN  &  RF_DATA_IN                   // 数据输入

#define    NSS522_1                P2OUT |= RF_SS                        // 从机选择无效       
#define    NSS522_0                P2OUT &=~RF_SS                        // 从机选择有效
#define    SCK522_1                P2OUT |= RF_SCLK                      // 数据时钟置高
#define    SCK522_0                P2OUT &=~RF_SCLK                      // 数据时钟置低
#define    SI522_1                 P2OUT |= RF_DATA_OUT                  // 数据置高        
#define    SI522_0                 P2OUT &=~RF_DATA_OUT                  // 数据置低        
#define    SO522                 ((P2IN  &  RF_DATA_IN)>>1)              // 数据输入
#define    RST522_1                P2OUT |= RF_LPCTL
#define    RST522_0                P2OUT &=~RF_LPCTL


//------------------------------ 操作结果  ---------------------------------------//
#define    SUCCESS                    0
#define    FAILURE                    1
#define    CRC_ERROR                  2
