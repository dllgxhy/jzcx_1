#ifndef _BLE_H_
#define _BLE_H_

#include "stdint.h"
#include "string.h"
#include "stdio.h"

///////////////////BLE传输数据的数据格式/////////////////////
//1  2  3


//定义BLE传输数据结构体，每次传输的数据都包括
typedef struct T_BLE_DATA
{
	uint8_t a;
}ble_Data;


void Init_Ble();


#endif
