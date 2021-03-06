/*
 * CommDef.h
 *
 *  Created on: 2016-2-15
 *      Author: guoxs
 */

#ifndef _UART_COMMDEF_H_
#define _UART_COMMDEF_H_

#include <stdint.h>

#ifndef BYTE
typedef unsigned char	BYTE;
#endif
#ifndef UINT
typedef unsigned int	UINT;
#endif
#ifndef UINT16
typedef unsigned short  UINT16;
#endif

#ifndef MAKEWORD
#define MAKEWORD(low, high)		(((BYTE)(low)) | (((BYTE)(high)) << 8))
#endif

#ifndef LOBYTE
#define LOBYTE(l)           ((BYTE)(l))
#endif

#ifndef HIBYTE
#define HIBYTE(l)           ((BYTE)(l >> 8))
#endif

#ifndef TABLESIZE
#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))
#endif


// 需要打印协议数据时，打开以下宏
#define DEBUG_PRO_DATA
#define DEBUG_procPars_DATA
// 支持checksum校验，打开以下宏
#define PRO_SUPPORT_CHECK_SUM
//定义帧尾校验，打开以下宏
#define PRO_SUPPORT_FRAME_END

/* SynchFrame CmdID  DataLen Data CheckSum (可选) SynchFrameEnd*/
/*     2Byte  2Byte   1Byte	N Byte  1Byte  2Byte*/
// 有CheckSum情况下最小长度: 2 + 2 + 1 + 1 +2 = 8
// 无CheckSum情况下最小长度: 2 + 2 + 1 +2 = 6

#ifdef PRO_SUPPORT_CHECK_SUM
	#define PRO_SUPPORT_CHECK_SUM_FLAG  1
#else
	#define PRO_SUPPORT_CHECK_SUM_FLAG  0
#endif

#ifdef PRO_SUPPORT_FRAME_END
	#define PRO_SUPPORT_FRAME_END_FLAG  1
#else
	#define PRO_SUPPORT_FRAME_END_FLAG  0
#endif

#if (PRO_SUPPORT_CHECK_SUM_FLAG&PRO_SUPPORT_FRAME_END_FLAG)
	#define DATA_PACKAGE_MIN_LEN		8
#else
	#if (PRO_SUPPORT_CHECK_SUM_FLAG==1)
		#define DATA_PACKAGE_MIN_LEN		6
	#elif(PRO_SUPPORT_FRAME_END_FLAG==1)
		#define DATA_PACKAGE_MIN_LEN		7
	#else
		#define DATA_PACKAGE_MIN_LEN		5
	#endif
#endif

// 同步帧头
#define FRAME_HEAD1				0XAA
#define FRAME_HEAD2				0X54
//同步帧尾
#define FRAME_END1  			0XAA
#define FRAME_END2 	 			0X55

//协议发送的命令字宏
#define OPEN_READY				0X0100
#define CLOSE_READY 			0X0200
#define OPEN_INDICATOR 			0X0300
#define CLOSE_INDICATOR 		0X0400
#define SET_POWER_PER			0X0500
#define SET_INDCATOR_LEVELS 	0X0600
#define SET_FREQUENCY			0X0700
#define SET_DUTY_CYCLE			0X0800
#define SET_TIMING_TIME			0X0900
#define SET_SPEAKER_VOLUME		0X0A00
#define SET_MODE				0X0B00
#define READ_MACHINE_PARAMETER  0X0C00
#define CLEAR_WARNING			0X0D00

#define PROTOCOL_DATA_LEN 		4

#endif /* _UART_COMMDEF_H_ */

