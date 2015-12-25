#include <msp430.h>
#define uchar unsigned char
/*******************************************
函数名称：InitUART
功    能：初始化UART端口
参    数：无
返回值  ：无
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
}
/*******************************************
函数名称：Send1Char
功    能：向PC机发送一个字符
参    数：sendchar--要发送的字符
返回值  ：无
********************************************/
void Send1Char(uchar sendchar)
{
      while (!(IFG1 & UTXIFG0));    //等待发送寄存器为空         
      TXBUF0 = sendchar; 
   
}
/*******************************************
函数名称：Get1Char
功    能：接收一个来自PC机的字符
参    数：无
返回值  ：接收到的字符
********************************************/
uchar Get1Char(void)
{
    while (!(IFG1 & URXIFG0));    //等待接收到字符        
    return  RXBUF0;
}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串并换行指令
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            Send1Char(*ptr++);                     // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';                              //发送换行指令
}
/*******************************************
函数名称：PutSting0
功    能：向PC机发送字符串，无换行
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString0(uchar *ptr)
{
      while(*ptr != '\0')
      {
            Send1Char(*ptr++);                     // 发送数据
      }
}
/********************************************/
void DisplayConfigMenu(void)
{
    PutString("  东方神锐电子科技——RFID-RC522射频模块  ");
    PutString("  网店地址：http://east-rain.taobao.com  ");
    PutString("  Choose Read or Write operation:  ");
    PutString("  A-- Auto to find card  ");
    PutString("  F-- Find card  ");
    PutString("  *-- any other key to redispaly the menu  ");
}

/*******************************************
函数名称：tochar
功    能：处理16进制函数
参    数：id
返回值  ：无
********************************************/
void tochar(uchar id)
{
    switch(id) 
    {
    case 0x00:PutString0("00");break;
    case 0x01:PutString0("01");break;
    case 0x02:PutString0("02");break;
    case 0x03:PutString0("03");break;
    case 0x04:PutString0("04");break;
    case 0x05:PutString0("05");break;
    case 0x06:PutString0("06");break;
    case 0x07:PutString0("07");break;
    case 0x08:PutString0("08");break;
    case 0x09:PutString0("09");break;
    case 0x0a:PutString0("0a");break;
    case 0x0b:PutString0("0b");break;
    case 0x0c:PutString0("0c");break;
    case 0x0d:PutString0("0d");break;
    case 0x0e:PutString0("0e");break;
    case 0x0f:PutString0("0f");break;
    case 0x10:PutString0("10");break;
    case 0x11:PutString0("11");break;
    case 0x12:PutString0("12");break;
    case 0x13:PutString0("13");break;
    case 0x14:PutString0("14");break;
    case 0x15:PutString0("15");break;
    case 0x16:PutString0("16");break;
    case 0x17:PutString0("17");break;
    case 0x18:PutString0("18");break;
    case 0x19:PutString0("19");break;
    case 0x1a:PutString0("1a");break;
    case 0x1b:PutString0("1b");break;
    case 0x1c:PutString0("1c");break;
    case 0x1d:PutString0("1d");break;
    case 0x1e:PutString0("1e");break;
    case 0x1f:PutString0("1f");break;
    case 0x20:PutString0("20");break;
    case 0x21:PutString0("21");break;
    case 0x22:PutString0("22");break;
    case 0x23:PutString0("23");break;
    case 0x24:PutString0("24");break;
    case 0x25:PutString0("25");break;
    case 0x26:PutString0("26");break;
    case 0x27:PutString0("27");break;
    case 0x28:PutString0("28");break;
    case 0x29:PutString0("29");break;
    case 0x2a:PutString0("2a");break;
    case 0x2b:PutString0("2b");break;
    case 0x2c:PutString0("2c");break;
    case 0x2d:PutString0("2d");break;
    case 0x2e:PutString0("2e");break;
    case 0x2f:PutString0("2f");break;
    case 0x30:PutString0("30");break;
    case 0x31:PutString0("31");break;
    case 0x32:PutString0("32");break;
    case 0x33:PutString0("33");break;
    case 0x34:PutString0("34");break;
    case 0x35:PutString0("35");break;
    case 0x36:PutString0("36");break;
    case 0x37:PutString0("37");break;
    case 0x38:PutString0("38");break;
    case 0x39:PutString0("39");break;
    case 0x3a:PutString0("3a");break;
    case 0x3b:PutString0("3b");break;
    case 0x3c:PutString0("3c");break;
    case 0x3d:PutString0("3d");break;
    case 0x3e:PutString0("3e");break;
    case 0x3f:PutString0("3f");break;
    case 0x40:PutString0("40");break;
    case 0x41:PutString0("41");break;
    case 0x42:PutString0("42");break;
    case 0x43:PutString0("43");break;
    case 0x44:PutString0("44");break;
    case 0x45:PutString0("45");break;
    case 0x46:PutString0("46");break;
    case 0x47:PutString0("47");break;
    case 0x48:PutString0("48");break;
    case 0x49:PutString0("49");break;
    case 0x4a:PutString0("4a");break;
    case 0x4b:PutString0("4b");break;
    case 0x4c:PutString0("4c");break;
    case 0x4d:PutString0("4d");break;
    case 0x4e:PutString0("4e");break;
    case 0x4f:PutString0("4f");break;
    case 0x50:PutString0("50");break;
    case 0x51:PutString0("51");break;
    case 0x52:PutString0("52");break;
    case 0x53:PutString0("53");break;
    case 0x54:PutString0("54");break;
    case 0x55:PutString0("55");break;
    case 0x56:PutString0("56");break;
    case 0x57:PutString0("57");break;
    case 0x58:PutString0("58");break;
    case 0x59:PutString0("59");break;
    case 0x5a:PutString0("5a");break;
    case 0x5b:PutString0("5b");break;
    case 0x5c:PutString0("5c");break;
    case 0x5d:PutString0("5d");break;
    case 0x5e:PutString0("5e");break;
    case 0x5f:PutString0("5f");break;
    case 0x60:PutString0("60");break;
    case 0x61:PutString0("61");break;
    case 0x62:PutString0("62");break;
    case 0x63:PutString0("63");break;
    case 0x64:PutString0("64");break;
    case 0x65:PutString0("65");break;
    case 0x66:PutString0("66");break;
    case 0x67:PutString0("67");break;
    case 0x68:PutString0("68");break;
    case 0x69:PutString0("69");break;
    case 0x6a:PutString0("6a");break;
    case 0x6b:PutString0("6b");break;
    case 0x6c:PutString0("6c");break;
    case 0x6d:PutString0("6d");break;
    case 0x6e:PutString0("6e");break;
    case 0x6f:PutString0("6f");break;
    case 0x70:PutString0("70");break;
    case 0x71:PutString0("71");break;
    case 0x72:PutString0("72");break;
    case 0x73:PutString0("73");break;
    case 0x74:PutString0("74");break;
    case 0x75:PutString0("75");break;
    case 0x76:PutString0("76");break;
    case 0x77:PutString0("77");break;
    case 0x78:PutString0("78");break;
    case 0x79:PutString0("79");break;
    case 0x7a:PutString0("7a");break;
    case 0x7b:PutString0("7b");break;
    case 0x7c:PutString0("7c");break;
    case 0x7d:PutString0("7d");break;
    case 0x7e:PutString0("7e");break;
    case 0x7f:PutString0("7f");break;
    case 0x80:PutString0("80");break;
    case 0x81:PutString0("81");break;
    case 0x82:PutString0("82");break;
    case 0x83:PutString0("83");break;
    case 0x84:PutString0("84");break;
    case 0x85:PutString0("85");break;
    case 0x86:PutString0("86");break;
    case 0x87:PutString0("87");break;
    case 0x88:PutString0("88");break;
    case 0x89:PutString0("89");break;
    case 0x8a:PutString0("8a");break;
    case 0x8b:PutString0("8b");break;
    case 0x8c:PutString0("8c");break;
    case 0x8d:PutString0("8d");break;
    case 0x8e:PutString0("8e");break;
    case 0x8f:PutString0("8f");break;
    case 0x90:PutString0("90");break;
    case 0x91:PutString0("91");break;
    case 0x92:PutString0("92");break;
    case 0x93:PutString0("93");break;
    case 0x94:PutString0("94");break;
    case 0x95:PutString0("95");break;
    case 0x96:PutString0("96");break;
    case 0x97:PutString0("97");break;
    case 0x98:PutString0("98");break;
    case 0x99:PutString0("99");break;
    case 0x9a:PutString0("9a");break;
    case 0x9b:PutString0("9b");break;
    case 0x9c:PutString0("9c");break;
    case 0x9d:PutString0("9d");break;
    case 0x9e:PutString0("9e");break;
    case 0x9f:PutString0("9f");break;
    case 0xa0:PutString0("a0");break;
    case 0xa1:PutString0("a1");break;
    case 0xa2:PutString0("a2");break;
    case 0xa3:PutString0("a3");break;
    case 0xa4:PutString0("a4");break;
    case 0xa5:PutString0("a5");break;
    case 0xa6:PutString0("a6");break;
    case 0xa7:PutString0("a7");break;
    case 0xa8:PutString0("a8");break;
    case 0xa9:PutString0("a9");break;
    case 0xaa:PutString0("aa");break;
    case 0xab:PutString0("ab");break;
    case 0xac:PutString0("ac");break;
    case 0xad:PutString0("ad");break;
    case 0xae:PutString0("ae");break;
    case 0xaf:PutString0("af");break;
    case 0xb0:PutString0("b0");break;
    case 0xb1:PutString0("b1");break;
    case 0xb2:PutString0("b2");break;
    case 0xb3:PutString0("b3");break;
    case 0xb4:PutString0("b4");break;
    case 0xb5:PutString0("b5");break;
    case 0xb6:PutString0("b6");break;
    case 0xb7:PutString0("b7");break;
    case 0xb8:PutString0("b8");break;
    case 0xb9:PutString0("b9");break;
    case 0xba:PutString0("ba");break;
    case 0xbb:PutString0("bb");break;
    case 0xbc:PutString0("bc");break;
    case 0xbd:PutString0("bd");break;
    case 0xbe:PutString0("be");break;
    case 0xbf:PutString0("bf");break;
    case 0xc0:PutString0("c0");break;
    case 0xc1:PutString0("c1");break;
    case 0xc2:PutString0("c2");break;
    case 0xc3:PutString0("c3");break;
    case 0xc4:PutString0("c4");break;
    case 0xc5:PutString0("c5");break;
    case 0xc6:PutString0("c6");break;
    case 0xc7:PutString0("c7");break;
    case 0xc8:PutString0("c8");break;
    case 0xc9:PutString0("c9");break;
    case 0xca:PutString0("ca");break;
    case 0xcb:PutString0("cb");break;
    case 0xcc:PutString0("cc");break;
    case 0xcd:PutString0("cd");break;
    case 0xce:PutString0("ce");break;
    case 0xcf:PutString0("cf");break;
    case 0xd0:PutString0("d0");break;
    case 0xd1:PutString0("d1");break;
    case 0xd2:PutString0("d2");break;
    case 0xd3:PutString0("d3");break;
    case 0xd4:PutString0("d4");break;
    case 0xd5:PutString0("d5");break;
    case 0xd6:PutString0("d6");break;
    case 0xd7:PutString0("d7");break;
    case 0xd8:PutString0("d8");break;
    case 0xd9:PutString0("d9");break;
    case 0xda:PutString0("da");break;
    case 0xdb:PutString0("db");break;
    case 0xdc:PutString0("dc");break;
    case 0xdd:PutString0("dd");break;
    case 0xde:PutString0("de");break;
    case 0xdf:PutString0("df");break;
    case 0xe0:PutString0("e0");break;
    case 0xe1:PutString0("e1");break;
    case 0xe2:PutString0("e2");break;
    case 0xe3:PutString0("e3");break;
    case 0xe4:PutString0("e4");break;
    case 0xe5:PutString0("e5");break;
    case 0xe6:PutString0("e6");break;
    case 0xe7:PutString0("e7");break;
    case 0xe8:PutString0("e8");break;
    case 0xe9:PutString0("e9");break;
    case 0xea:PutString0("ea");break;
    case 0xeb:PutString0("eb");break;
    case 0xec:PutString0("ec");break;
    case 0xed:PutString0("ed");break;
    case 0xee:PutString0("ee");break;
    case 0xef:PutString0("ef");break;
    case 0xf0:PutString0("f0");break;
    case 0xf1:PutString0("f1");break;
    case 0xf2:PutString0("f2");break;
    case 0xf3:PutString0("f3");break;
    case 0xf4:PutString0("f4");break;
    case 0xf5:PutString0("f5");break;
    case 0xf6:PutString0("f6");break;
    case 0xf7:PutString0("f7");break;
    case 0xf8:PutString0("f8");break;
    case 0xf9:PutString0("f9");break;
    case 0xfa:PutString0("fa");break;
    case 0xfb:PutString0("fb");break;
    case 0xfc:PutString0("fc");break;
    case 0xfd:PutString0("fd");break;
    case 0xfe:PutString0("fe");break;
    case 0xff:PutString0("ff");break;
    default:
              DisplayConfigMenu();  
    }
}
