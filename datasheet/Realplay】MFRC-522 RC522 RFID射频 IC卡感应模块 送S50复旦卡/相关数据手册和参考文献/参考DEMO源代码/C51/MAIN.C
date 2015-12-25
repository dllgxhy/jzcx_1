#include "lpcreg.h"
#include "main.h"
#include "mfrc522.h"

unsigned char code data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data2[4]  = {0x12,0,0,0};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 


unsigned char g_ucTempbuf[20];                        

void main( )
{    
     unsigned char status;
     InitializeSystem( );
     PcdReset();
     PcdAntennaOff(); //关闭天线
     PcdAntennaOn();  //开启天线
     while ( 1 )
     {   
         status = PcdRequest(PICC_REQALL, g_ucTempbuf);       #define PICC_REQIDL 0x26    //寻天线区内未进入休眠状态
////////////////////////////////////////////////////////////////////////////////
         if (status != MI_OK)
         { 
              LED_GREEN = 1;
	      continue;
         }
/////////////////////////////////////////////////////////////////////
         LED_GREEN = 0;
         status = PcdAnticoll(g_ucTempbuf);
//////////////////////////////////////////////////////////////////////
         if (status != MI_OK)
         { 
             continue;    
         }
/////////////////////////////////////////////////////////////////////
         status = PcdSelect(g_ucTempbuf);
         if (status != MI_OK)
         {    
            continue;    
         }
////////////////////////////////////////////////////////////////////////////         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);
         if (status != MI_OK)
         {    
             continue;    
         }
/////////////////////////////////////////////////////////////////////////////         
         status = PcdWrite(1, data1);
         if (status != MI_OK)
         {    
            continue;    
         }
/////////////////////////////////////////////////////////////////////////////////         
         status = PcdValue(PICC_DECREMENT,1,data2);
         if (status != MI_OK)
         {    
           continue;    
         }
/////////////////////////////////////////////////////////////////////////////////         
         status = PcdBakValue(1, 2);
         if (status != MI_OK)
         {    
             continue;    
         }
/////////////////////////////////////////////////////////////////////////////         
         status = PcdRead(2, g_ucTempbuf);
         if (status != MI_OK)
         {    
            continue;   
         }
          PcdHalt();   //命令卡片进入休眠状态
    }
}


/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
    P0M1 = 0x0; P0M2 = 0x0;
    P1M1 = 0x0;	P1M2 = 0x0;                      
    P3M1 = 0x0; P3M2 = 0xFF; 
    P0 = 0xFF; P1 = 0xFF; P3 = 0xFF;
}
/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
    MF522_RST=1;
    _nop_();
    MF522_RST=0;
    _nop_();
    MF522_RST=1;
     _nop_();
    WriteRawRC(CommandReg,PCD_RESETPHASE);
    _nop_();
    
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
   
    return MI_OK;
}
/////////////////////////////////////////////////////////////////////
//功    能：写RC632寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr;

    MF522_SCK = 0;
    MF522_NSS = 0;
    ucAddr = ((Address<<1)&0x7E);

    for(i=8;i>0;i--)
    {
        MF522_SI = ((ucAddr&0x80)==0x80);
        MF522_SCK = 1;
        ucAddr <<= 1;
        MF522_SCK = 0;
    }

    for(i=8;i>0;i--)
    {
        MF522_SI = ((value&0x80)==0x80);
        MF522_SCK = 1;
        value <<= 1;
        MF522_SCK = 0;
    }
    MF522_NSS = 1;
    MF522_SCK = 1;
}
/////////////////////////////////////////////////////////////////////
//功    能：读RC632寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     MF522_SCK = 0;
     MF522_NSS = 0;
     ucAddr = ((Address<<1)&0x7E)|0x80;

     for(i=8;i>0;i--)
     {
         MF522_SI = ((ucAddr&0x80)==0x80);
         MF522_SCK = 1;
         ucAddr <<= 1;
         MF522_SCK = 0;
     }

     for(i=8;i>0;i--)
     {
         MF522_SCK = 1;
         ucResult <<= 1;
         ucResult|=(bit)MF522_SO;
         MF522_SCK = 0;
     }

     MF522_NSS = 1;
     MF522_SCK = 1;
     return ucResult;
}
   
  

