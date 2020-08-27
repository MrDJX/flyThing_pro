/*
 * ProtocolSender.h
 *
 *  Created on: Sep 8, 2017
 *      Author: guoxs
 */

#ifndef _UART_PROTOCOL_SENDER_H_
#define _UART_PROTOCOL_SENDER_H_

#include "CommDef.h"

bool sendProtocol(const UINT16 cmdID,const BYTE *pData, BYTE len);
//发送数据结构体
typedef struct{
	//高8位：命令字 低8位：序列号
	UINT16  CmdCtrlNum;
	//协议数据
	BYTE SendProtocolData[PROTOCOL_DATA_LEN];
} SSendProtocolData;

#endif /* _UART_PROTOCOL_SENDER_H_ */
