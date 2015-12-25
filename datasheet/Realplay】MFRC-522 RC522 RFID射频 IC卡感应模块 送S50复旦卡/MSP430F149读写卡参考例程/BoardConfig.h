#define uchar unsigned char
#define uint unsigned int 

//控制位的宏定义
#define Ctrl_Out  P3DIR |= BIT3 + BIT6 + BIT7;
#define Ctrl_0    P3OUT &= ~(BIT3 + BIT6 + BIT7)
#define SRCLK_1   P3OUT |= BIT7
#define SRCLK_0   P3OUT &= ~BIT7
#define SER_1     P3OUT |= BIT6
#define SER_0     P3OUT &= ~BIT6
#define RCLK_1    P3OUT |= BIT3
#define RCLK_0    P3OUT &= ~BIT3

//板上资源配置函数
void BoardConfig(uchar cmd)
{
    uchar i;
    
    Ctrl_Out;
    Ctrl_0;
    
    for(i = 0; i < 8; i++)
    {
        SRCLK_0;
        if(cmd & 0x80)  SER_1;
        else            SER_0;
        SRCLK_1;
        cmd <<= 1;
    }
    RCLK_1;
    _NOP();
    RCLK_0;
}
