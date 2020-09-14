/*
 * ProtocolData.h
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */

#ifndef _UART_PROTOCOL_DATA_H_
#define _UART_PROTOCOL_DATA_H_

#include <string>
#include "CommDef.h"

/******************** CmdID ***********************/
#define CMDID_POWER							0x0
/**************************************************/

/******************** 错误码 ***********************/
#define ERROR_CODE_CMDID			1
/**************************************************/
//RX接收读取数据缓存帧结构体
typedef struct {
	BYTE Frame_Head[2];
	BYTE Cmd_Byte;
	BYTE ID_Byte;
	BYTE ProtocolData[12];
	BYTE Check_Byte;
	BYTE Frame_End[2];
} SProtocolData;

#endif /* _UART_PROTOCOL_DATA_H_ */
