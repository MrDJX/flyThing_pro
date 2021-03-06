/*
 * ProtocolSender.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: guoxs
 */

#include "uart/ProtocolSender.h"
#include "uart/UartContext.h"
#include "utils/Log.h"
#include <stdio.h>

extern BYTE getCheckSum(const BYTE *pData, int len);

/**
 * 需要根据协议格式进行拼接，以下只是个模板
 */
bool sendProtocol(const UINT16 cmdID, const BYTE *pData, BYTE len) {
	if (len + DATA_PACKAGE_MIN_LEN > 256) {
		LOGE("sendProtocol data is too len !!!\n");
		return false;
	}

	BYTE dataBuf[256];
	int dataBufLen;
	dataBufLen = len+DATA_PACKAGE_MIN_LEN;
	dataBuf[0] = FRAME_HEAD1;
	dataBuf[1] = FRAME_HEAD2;			// 同步帧头

	dataBuf[2] = HIBYTE(cmdID);
	dataBuf[3] = LOBYTE(cmdID);		// 命令字节

//	dataBuf[4] = len;

	UINT frameLen = 4;

	// 数据
	for (int i = 0; i < len; ++i) {
		dataBuf[frameLen] = pData[i];
		frameLen++;
	}
#ifdef PRO_SUPPORT_CHECK_SUM
	// 校验码
	dataBuf[frameLen] = getCheckSum(dataBuf, frameLen);
	frameLen++;
#endif
#ifdef PRO_SUPPORT_FRAME_END
	dataBuf[frameLen]=FRAME_END1;
	frameLen++;
	dataBuf[frameLen]=FRAME_END2; // 同步帧尾
	frameLen++;
#endif
#ifdef DEBUG_PRO_DATA
		for (UINT i = 0; i < frameLen; ++i) {
			LOGD("tx:%x ", dataBuf[i]);
		}
		LOGD("\n");
#endif
	return UARTCONTEXT->send(dataBuf, frameLen);
}
