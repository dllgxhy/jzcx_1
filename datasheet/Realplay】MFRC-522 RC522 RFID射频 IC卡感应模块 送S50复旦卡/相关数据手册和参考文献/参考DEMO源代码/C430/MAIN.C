#include  <msp430x41x.h>
#include "main.h"
#include "mfrc522.h"

#pragma memory = constseg(TAB)
unsigned char data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char data2[4]  = {0x12,0,0,0};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};                
#pragma memory=default 
            
unsigned char g_ucTempbuf[20];

void main(void)
{ 
     unsigned char status;
     InitializeSystem();
     PcdReset();
     PcdAntennaOff(); 
     PcdAntennaOn();  
     while ( 1 )
     {   
         status = PcdRequest(PICC_REQALL, g_ucTempbuf);
         if (status != MI_OK)
         {    
             LED_GREEN_OFF;
             continue;
         }
         LED_GREEN_ON;     //检测到有卡存在
         
         status = PcdAnticoll(g_ucTempbuf);
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdSelect(g_ucTempbuf);
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdWrite(1, data1);
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdValue(PICC_DECREMENT,1,data2);
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdBakValue(1, 2);
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdRead(2, g_ucTempbuf);
         if (status != MI_OK)
         {    continue;    }
         
         PcdHalt();
    }
}


/////////////////////////////////////////////////////////////////////
//功能：系统初始化
/////////////////////////////////////////////////////////////////////	
void InitializeSystem(void)
{
    WDTCTL = WDTPW + WDTHOLD;             //Stop watchdog timer
    _DINT();
     
    P1DIR = 0xE8;
    P1OUT = 0xFF;
      
    P6DIR = 0x08;
    P6OUT = 0xFF;
}    



