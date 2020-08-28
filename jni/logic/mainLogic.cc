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
} S_TEST_DATA;

static S_TEST_DATA sDataTestTab[] = {
	{ "帧头", 		"01 02" 		},
	{ "命令字", 		"0a" 			},
	{ "序列号", 		"0a"			},
	{ "数据位", 		"01 02 03 04" 	},
	{ "校验位", 		"0a" 			},
	{ "帧尾", 		"01 02" 		}
};

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
	BYTE data[4]={0x01,0x02,0x03,0x04};
	memcpy(&sSendProtocolData.SendProtocolData,&data,sizeof(data));
	sSendProtocolData.CmdCtrlNum=0x010a;
	sendProtocol(sSendProtocolData.CmdCtrlNum,sSendProtocolData.SendProtocolData,PROTOCOL_DATA_LEN);
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
static bool onButtonClick_Button1(ZKButton *pButton) {
    LOGD(" ButtonClick Button1 !!!\n");
    return false;
}

static void onEditTextChanged_EditText1(const std::string &text) {
    LOGD(" onEditTextChanged_ EditText1 %s !!!\n", text.c_str());

}
static int getListItemCount_ListView1(const ZKListView *pListView) {
    //LOGD("getListItemCount_ListView1 !\n");
    return (sizeof(sDataTestTab) / sizeof(S_TEST_DATA));
}

static void obtainListItemData_ListView1(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ListView1  !!!\n");
	ZKListView::ZKListSubItem* psubText = pListItem->findSubItemByID(ID_MAIN_SubItem1);
	psubText->setText(sDataTestTab[index].subText);
	pListItem->setText(sDataTestTab[index].mainText);
}

static void onListItemClick_ListView1(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ListView1  !!!\n");
}
