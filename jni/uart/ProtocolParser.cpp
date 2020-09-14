/*
 * ProtocolParser.cpp
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */
#include <vector>
#include <string.h>
#include <system/Mutex.h>
#include "CommDef.h"
#include "uart/ProtocolParser.h"
#include "utils/Log.h"
#include "uart/ProtocolSender.h"

extern SSendProtocolData sSendProtocolData;
//发送协议长度
//BYTE PROTOCOL_DATA_LEN=4;

static Mutex sLock;
static std::vector<OnProtocolDataUpdateFun> sProtocolDataUpdateListenerList;

void registerProtocolDataUpdateListener(OnProtocolDataUpdateFun pListener) {
	Mutex::Autolock _l(sLock);
	LOGD("registerProtocolDataUpdateListener\n");
	if (pListener != NULL) {
		sProtocolDataUpdateListenerList.push_back(pListener);
	}
}

void unregisterProtocolDataUpdateListener(OnProtocolDataUpdateFun pListener) {
	Mutex::Autolock _l(sLock);
	LOGD("unregisterProtocolDataUpdateListener\n");
	if (pListener != NULL) {
		std::vector<OnProtocolDataUpdateFun>::iterator iter = sProtocolDataUpdateListenerList.begin();
		for (; iter != sProtocolDataUpdateListenerList.end(); iter++) {
			if ((*iter) == pListener) {
				sProtocolDataUpdateListenerList.erase(iter);
				return;
			}
		}
	}
}

static void notifyProtocolDataUpdate(const SProtocolData &data) {
	Mutex::Autolock _l(sLock);
	std::vector<OnProtocolDataUpdateFun>::const_iterator iter = sProtocolDataUpdateListenerList.begin();
	for (; iter != sProtocolDataUpdateListenerList.end(); iter++) {
		(*iter)(data);
	}
}

static SProtocolData sProtocolData;

SProtocolData& getProtocolData() {
	return sProtocolData;
}

/**
 * 获取校验码
 */
BYTE getCheckSum(const BYTE *pData, int len) {
	int sum = 0;
	for (int i = 2; i < len; ++i) {
		sum += pData[i];
	}

	return (BYTE) (sum&0xff);//sum%256
}

/**
 * 解析每一帧数据,缓存到结构体数组中
 */
static void procParse(const BYTE *pData, UINT len) {
	// CmdID
#ifdef DEBUG_procPars_DATA
		for (UINT i = 0; i < len; ++i) {
			LOGD("procParse:%x ", pData[i]);
		}
		LOGD("\n");
#endif
		sProtocolData.Frame_Head[0]    = pData[0];
		sProtocolData.Frame_Head[1]    = pData[1];
		sProtocolData.Cmd_Byte         = pData[2];
		sProtocolData.ID_Byte          = pData[3];
		sProtocolData.ProtocolData[0]  = pData[4];
		sProtocolData.ProtocolData[1]  = pData[5];
		sProtocolData.ProtocolData[2]  = pData[6];
		sProtocolData.ProtocolData[3]  = pData[7];
	if(sSendProtocolData.CmdCtrlNum==READ_MACHINE_PARAMETER){
		sProtocolData.ProtocolData[4]  = pData[8];
		sProtocolData.ProtocolData[5]  = pData[9];
		sProtocolData.ProtocolData[6]  = pData[10];
		sProtocolData.ProtocolData[7]  = pData[11];
		sProtocolData.ProtocolData[8]  = pData[12];
		sProtocolData.ProtocolData[9]  = pData[13];
		sProtocolData.ProtocolData[10]  = pData[14];
		sProtocolData.ProtocolData[11]  = pData[15];
		sProtocolData.Check_Byte       = pData[16];
		sProtocolData.Frame_End[0]     = pData[17];
		sProtocolData.Frame_End[1]     = pData[18];
	}
	else{
		sProtocolData.ProtocolData[4]  = 0;
		sProtocolData.ProtocolData[5]  = 0;
		sProtocolData.ProtocolData[6]  = 0;
		sProtocolData.ProtocolData[7]  = 0;
		sProtocolData.ProtocolData[8]  = 0;
		sProtocolData.ProtocolData[9]  = 0;
		sProtocolData.ProtocolData[10]  = 0;
		sProtocolData.ProtocolData[11]  = 0;
		sProtocolData.Check_Byte       = pData[8];
		sProtocolData.Frame_End[0]     = pData[9];
		sProtocolData.Frame_End[1]     = pData[10];
	}
	// 通知协议数据更新
	notifyProtocolDataUpdate(sProtocolData);
}

/**
 * 功能：解析协议
 * 参数：pData 协议数据，len 数据长度
 * 返回值：实际解析协议的长度
 */
int parseProtocol(const BYTE *pData, UINT len) {
	UINT remainLen = len;	// 剩余数据长度
	UINT dataLen=4;	// 数据包长度
	UINT frameLen;	// 帧长度

	/**
	 * 以下部分需要根据协议格式进行相应的修改，解析出每一帧的数据
	 */
	while (remainLen >= DATA_PACKAGE_MIN_LEN) {
		// 找到一帧数据的数据头
		while ((remainLen >= 2) && ((pData[0] != FRAME_HEAD1) || (pData[1] != FRAME_HEAD2))) {
			pData++;
			remainLen--;
			continue;
		}

		if (remainLen < DATA_PACKAGE_MIN_LEN) {
			break;
		}
		if(sSendProtocolData.CmdCtrlNum==READ_MACHINE_PARAMETER){
			dataLen = 12;
		}
		else{
			dataLen = 4;
		}
		frameLen = dataLen + DATA_PACKAGE_MIN_LEN -1;
		LOGD("frameLen %d ", frameLen);
		if (frameLen > remainLen) {
			// 数据内容不全
			break;
		}

		// 打印一帧数据，需要时在CommDef.h文件中打开DEBUG_PRO_DATA宏
#ifdef DEBUG_PRO_DATA
		for (UINT i = 0; i < frameLen; ++i) {
			LOGD("rx:%x ", pData[i]);
		}
		LOGD("\n");
#endif
#if (PRO_SUPPORT_CHECK_SUM_FLAG&PRO_SUPPORT_FRAME_END_FLAG)
		// 检测校验码,检测到帧尾
		if ((getCheckSum(pData, frameLen-3) \
				== pData[frameLen-3])\
				&&(pData[frameLen - 2] == FRAME_END1) \
				&&(pData[frameLen - 1] == FRAME_END2)) {
			// 解析一帧数据
			procParse(pData, frameLen);
		} else {
			LOGE("CheckSum FRAME_END error!!!!!!\n");
		}
#else
	#if (PRO_SUPPORT_CHECK_SUM_FLAG==1)
		if ((getCheckSum(pData, frameLen-(DATA_PACKAGE_MIN_LEN-PROTOCOL_DATA_LEN))\
				== pData[frameLen-(DATA_PACKAGE_MIN_LEN-PROTOCOL_DATA_LEN)])){
			// 解析一帧数据
			procParse(pData, frameLen);
		} else {
			LOGE("CheckSum error!!!!!!\n");
		}
	#elif(PRO_SUPPORT_FRAME_END_FLAG==1)
		if ((pData[frameLen - 2] == FRAME_END1) \
			&&(pData[frameLen - 1] == FRAME_END2)) {
			// 解析一帧数据
			procParse(pData, frameLen);
		} else {
			LOGE("FRAME_END error!!!!!!\n");
		}
	#else
		procParse(pData, frameLen);
	#endif
#endif
		pData += frameLen;
		remainLen -= frameLen;
	}

	return len - remainLen;
}
