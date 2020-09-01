#pragma once
#include "uart/ProtocolSender.h"
static SSendProtocolData sSendProtocolData;
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXXPtr->setText("****") 在控件TextXXX上显示文字****
*mButton1Ptr->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBarPtr->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1Ptr->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1Ptr->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 “alt + /”  快捷键可以打开智能提示
*/
typedef struct {
	const char* mainText;
	const char* subText;
	const char* subText1;
} S_TEST_DATA;

static S_TEST_DATA sDataTestTab[] = {
	{ "帧头", 		"01 02", 		"01 02" 		},
	{ "命令字", 		"0a", 			"0a" 			},
	{ "序列号", 		"0a",			"0a"			},
	{ "数据位", 		"01 02 03 04", 	"01 02 03 04"	},
	{ "校验位", 		"0a", 		   	"0a" 			},
	{ "帧尾", 		"01 02", 		"01 02" 		}
};

typedef struct{
	BYTE power;
	BYTE instr;
	UINT16 freq;
	BYTE dutyCycle;
	UINT16 time;
	BYTE volume;
	BYTE mode;
	BYTE clrWarning;
}SET_PARAMETER;
static SET_PARAMETER sSET_PARAMETER;
/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[1] = {
	{0,  2000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
//	mTextView1Ptr->setText("123");
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {

}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {

}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

//	LOGD(" read data 0x%x 0x%x !!!\n", data.Frame_Head[0],data.Frame_Head[1]);
	char buf[16],buf1[16],buf2[16],buf3[16],buf4[16],buf5[16];
	snprintf(buf, sizeof(buf), "%02x %02x", data.Frame_Head[0],data.Frame_Head[1]);
	sDataTestTab[0].subText=buf;
	snprintf(buf1, sizeof(buf1), "%02x", data.Cmd_Byte);
	sDataTestTab[1].subText=buf1;
	snprintf(buf2, sizeof(buf2), "%02x", data.ID_Byte);
	sDataTestTab[2].subText=buf2;
	snprintf(buf3, sizeof(buf3), "%02x %02x %02x %02x", data.ProtocolData[0],\
											  data.ProtocolData[1],\
											  data.ProtocolData[2],\
											  data.ProtocolData[3]);
	sDataTestTab[3].subText=buf3;
	snprintf(buf4, sizeof(buf4), "%02x", data.Check_Byte);
	sDataTestTab[4].subText=buf4;
	snprintf(buf5, sizeof(buf5), "%02x %02x", data.Frame_End[0],data.Frame_End[1]);
	sDataTestTab[5].subText=buf5;
//	LOGD(" sDataTestTab %s !!!\n", sDataTestTab[0].subText);

	char abuf[16],abuf1[16],abuf2[16],abuf3[16],abuf4[16],abuf5[16];
	snprintf(abuf, sizeof(abuf), "%02x %02x", FRAME_HEAD1,FRAME_HEAD2);
	sDataTestTab[0].subText1=abuf;
	snprintf(abuf1, sizeof(abuf1), "%02x", (BYTE)(sSendProtocolData.CmdCtrlNum>>8));
	sDataTestTab[1].subText1=abuf1;
	snprintf(abuf2, sizeof(abuf2), "%02x", (BYTE)(sSendProtocolData.CmdCtrlNum<<8));
	sDataTestTab[2].subText1=abuf2;
	snprintf(abuf3, sizeof(abuf3), "%02x %02x %02x %02x", sSendProtocolData.SendProtocolData[0],\
														sSendProtocolData.SendProtocolData[1],\
														sSendProtocolData.SendProtocolData[2],\
														sSendProtocolData.SendProtocolData[3]);
	sDataTestTab[3].subText1=abuf3;
	snprintf(abuf4, sizeof(abuf4), "%02x", data.Check_Byte);
	sDataTestTab[4].subText1=abuf4;
	snprintf(abuf5, sizeof(abuf5), "%02x %02x", FRAME_END1,FRAME_END2);
	sDataTestTab[5].subText1=abuf5;
//	LOGD(" sDataTestTab %s !!!\n", sDataTestTab[5].subText1);

	mListView1Ptr->refreshListView();
}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */
static bool onUI_Timer(int id){
//	LOGD(" send data  %x !!!\n", *data);
	mListView1Ptr->refreshListView();
    return true;
}

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */
static bool onmainActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}
static int getListItemCount_ListView1(const ZKListView *pListView) {
    //LOGD("getListItemCount_ListView1 !\n");
    return (sizeof(sDataTestTab) / sizeof(S_TEST_DATA));
}

static void obtainListItemData_ListView1(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ListView1  !!!\n");
	ZKListView::ZKListSubItem* psubText = pListItem->findSubItemByID(ID_MAIN_SubItem1);
	ZKListView::ZKListSubItem* psubText1 = pListItem->findSubItemByID(ID_MAIN_SubItem2);
	psubText->setText(sDataTestTab[index].subText);
	psubText1->setText(sDataTestTab[index].subText1);
	pListItem->setText(sDataTestTab[index].mainText);

}
static bool onButtonClick_Button1(ZKButton *pButton) {
    LOGD(" ButtonClick Button1 !!!\n");
	BYTE data[4]={0};
	sSendProtocolData.CmdCtrlNum=OPEN_READY;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}
static bool onButtonClick_Button2(ZKButton *pButton) {
    LOGD(" ButtonClick Button2 !!!\n");
	BYTE data[4]={0};
//	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=CLOSE_READY;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_Button3(ZKButton *pButton) {
    LOGD(" ButtonClick Button3 !!!\n");
	BYTE data[4]={0};
	sSendProtocolData.CmdCtrlNum=OPEN_INDICATOR;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_Button4(ZKButton *pButton) {
    LOGD(" ButtonClick Button4 !!!\n");
	BYTE data[4]={0};
	sSendProtocolData.CmdCtrlNum=CLOSE_INDICATOR;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_setPowerButton(ZKButton *pButton) {
    LOGD(" ButtonClick setPowerButton !!!\n");
	BYTE data[4]={0,0,0,sSET_PARAMETER.power};
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=SET_POWER_PER;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}
static bool onButtonClick_setInstrButton(ZKButton *pButton) {
    LOGD(" ButtonClick setInstrButton !!!\n");
	BYTE data[4]={0,0,0,sSET_PARAMETER.instr};
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=SET_INDCATOR_LEVELS;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_setFreqButton(ZKButton *pButton) {
    LOGD(" ButtonClick setFreqButton !!!\n");
	BYTE data[4]={0,0,(BYTE)(sSET_PARAMETER.freq>>8),(BYTE)(sSET_PARAMETER.freq&0x00ff)};
//	LOGD("sSET_PARAMETER.freq %x,%x !!!\n",data[2],data[3]);
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=SET_FREQUENCY;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_setDutyButton(ZKButton *pButton) {
    LOGD(" ButtonClick setDutyButton !!!\n");
	BYTE data[4]={0,0,0,sSET_PARAMETER.dutyCycle};
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=SET_DUTY_CYCLE;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_setTimeButton(ZKButton *pButton) {
    LOGD(" ButtonClick setTimeButton !!!\n");
	BYTE data[4]={0,0,(BYTE)(sSET_PARAMETER.time>>8),(BYTE)(sSET_PARAMETER.time&0x00ff)};
//	LOGD("sSET_PARAMETER.freq %x,%x !!!\n",data[2],data[3]);
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=SET_TIMING_TIME;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_setVolumeButton(ZKButton *pButton) {
    LOGD(" ButtonClick setVolumeButton !!!\n");
    BYTE data[4]={0,0,0,sSET_PARAMETER.volume};
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=SET_SPEAKER_VOLUME;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}
static bool onButtonClick_readParmButton(ZKButton *pButton) {
    LOGD(" ButtonClick readParmButton !!!\n");
    BYTE data[4]={0};
//	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=READ_MACHINE_PARAMETER;
	sendProtocol(sSendProtocolData.CmdCtrlNum,data,PROTOCOL_DATA_LEN);
    return false;
}

static bool onButtonClick_clrWarningButton(ZKButton *pButton) {
    LOGD(" ButtonClick clrWarningButton !!!\n");
    return false;
}
static void onEditTextChanged_SetPower(const std::string &text) {
	if(atoi(text.c_str())>100){
		mSetPowerPtr->setTextColor(0xFF0000);
		sSET_PARAMETER.power=0;
	}
	else{
		mSetPowerPtr->setTextColor(0x000000);
		sSET_PARAMETER.power=atoi(text.c_str());
	}
    LOGD(" onEditTextChanged_ SetPower %d !!!\n", atoi(text.c_str()));
}
static void onEditTextChanged_setInstr(const std::string &text) {
    //LOGD(" onEditTextChanged_ setInstr %s !!!\n", text.c_str());
	if(atoi(text.c_str())>7){
		msetInstrPtr->setTextColor(0xFF0000);
		sSET_PARAMETER.instr=0;
	}
	else{
		msetInstrPtr->setTextColor(0x000000);
		sSET_PARAMETER.instr=atoi(text.c_str());
	}
    LOGD(" onEditTextChanged_ instr %d !!!\n", atoi(text.c_str()));
}

static void onEditTextChanged_setFreq(const std::string &text) {
    //LOGD(" onEditTextChanged_ setFreq %s !!!\n", text.c_str());
	if(atoi(text.c_str())>10000){
		msetFreqPtr->setTextColor(0xFF0000);
		sSET_PARAMETER.freq=0;
	}
	else{
		msetFreqPtr->setTextColor(0x000000);
		sSET_PARAMETER.freq=atoi(text.c_str());
	}
    LOGD(" onEditTextChanged_ setFreq %x !!!\n", atoi(text.c_str()));
}

static void onEditTextChanged_setDuty(const std::string &text) {
    //LOGD(" onEditTextChanged_ setDuty %s !!!\n", text.c_str());
	if(atoi(text.c_str())>100){
		msetDutyPtr->setTextColor(0xFF0000);
		sSET_PARAMETER.dutyCycle=0;
	}
	else{
		msetDutyPtr->setTextColor(0x000000);
		sSET_PARAMETER.dutyCycle=atoi(text.c_str());
	}
    LOGD(" onEditTextChanged_ setDuty %d !!!\n", atoi(text.c_str()));
}

static void onEditTextChanged_setTime(const std::string &text) {
    //LOGD(" onEditTextChanged_ setTime %s !!!\n", text.c_str());
	if(atoi(text.c_str())>65535){
		msetTimePtr->setTextColor(0xFF0000);
		sSET_PARAMETER.time=0;
	}
	else{
		msetTimePtr->setTextColor(0x000000);
		sSET_PARAMETER.time=atoi(text.c_str());
	}
}

static void onEditTextChanged_setVolume(const std::string &text) {
    //LOGD(" onEditTextChanged_ setVolume %s !!!\n", text.c_str());
	if(atoi(text.c_str())>100){
		msetVolumePtr->setTextColor(0xFF0000);
		sSET_PARAMETER.volume=0;
	}
	else{
		msetVolumePtr->setTextColor(0x000000);
		sSET_PARAMETER.volume=atoi(text.c_str());
	}
}

static bool onButtonClick_modeButtonV(ZKButton *pButton) {
    LOGD(" ButtonClick modeButtonV !!!\n");
    pButton->setSelected(!pButton->isSelected());
    return false;
}
static void onListItemClick_ListView1(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ListView1  !!!\n");
}

static bool onButtonClick_setModeButton(ZKButton *pButton) {
    LOGD(" ButtonClick setModeButton !!!\n");
    return false;
}
