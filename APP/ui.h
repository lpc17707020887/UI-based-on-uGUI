#ifndef _UI_H
#define _UI_H
/****************************1、头文件******************************************/
#include "stddef.h"
#include "AD_key.h"
#include "SPI_lcd.h"
#include "ugui.h"
#include "stdio.h"


/****************************2、宏定义******************************************/
#define KEY_SUM 16       //设备按键数--16
#define NUM_KEY_SUM 11   //数字按键个数--包括小数点按键
/**********************************************************************
**把11个数字按键的处理封装成PROCESS_ShortNumKey_Of_Page1函数，
**当前情况有16-11=5类按键处理函数，分类如下，实现在ui.c中
**1、上键处理函数 2、下键处理函数 3、OK键处理函数 4、CLR键处理函数 5、数字键处理函数（包括小数点键）
**********************************************************************/
#define PROCESS_KEY_FUNCTION_SUM   KEY_SUM-NUM_KEY_SUM+1           

#define PAGE_SUM 19    //设备界面数--19
#define LONG_KEY_JUDGE_TIMES  50        //长按键判断次数----连续5次扫描到同一个按键按下则为长按键
//////供LCDUpdateMain和KeyScanMain之间邮箱通信使用
#define LcdUpdateMain_ID      0x99     //LCDUpdateMain模块ID号 
#define KeyScanMain_ID		  0x88	   //KeyScanMain模块ID号		


/******************************3、类型定义****************************************/
//当前按键枚举
//备注：本按键枚举类型的顺序绝对不能修改，顺序影响按键响应函数的指针链接
typedef enum
{
	KEY_NONE=0,        //-//
	KEY_SET=1,		   //功	 			
	KEY_OK=2,            //能
	KEY_CLR=3,           //按
	KEY_UP=4,            //键
	KEY_DOWN=5,          //-// 



	KEY_0=6,			   //-//	 
	KEY_1=7,			   // 	
	KEY_2=8,			   //
	KEY_3=9,			   //数
	KEY_4=10,			   //字
	KEY_5=11,			   //按
	KEY_6=12,			   //键
	KEY_7=13,             //
	KEY_8=14,             //
	KEY_9=15,             //
	KEY_POINT,         //-//
}AbstractKey_Typedef;


//当前界面枚举
//本界面枚举类型的顺序绝对不能修改，顺序影响按键响应函数的指针链接
typedef enum
{
	PageInit=0,					// 上电初始化页面
	PageDefault,				// 默认显示页码
	PageRGZS,					// 人工置数页码
	PageCodeEnt,				// 密码输入页码
	PageCodeChange,				// 密码修改页码
	PageMainMenu,				// 主菜单页码
	PageDTUSet,
	PageSensorMotherConfig,		// 传感器母菜单页码
	PageSensorSonPara,			// 传感器子菜单1页码
	PageTestSend,				// 发送测试报文页码
	PageParaBak,				// 恢复出厂默认页码
	PageDateTimeSet,			// 日期和时间设置
		
	PageDTUServerSet,			// DTU移动服务参数设置
	PageDTURunModeSet,			// DTU运行参数设置
	PageDTUSMSSet,				// DTU SMS短信参数设置
	PageDTUChan1Set,			// DTU通道1设置
	PageDTUChan2Set,			// DTU通道2设置
	PageDTUChan3Set,			// DTU通道3设置
	PageDTUChan4Set,		    // DTU通道4设置
}
CurrentPageTypedef;

//当前行数枚举
typedef enum
{
	COLUME1=0,
	COLUME2,
	COLUME3,
	COLUME4,
	COLUME5,
	COLUME6,
	COLUME7,
	COLUME8,
	COLUME9,
	COLUME10,
}
CurrentColumeTypedef;


typedef struct 
{
	u8 SenderID;       //寄件模块ID
	u8 RecieverID;     //收件模块ID 
	u8 data;           //寄件数据
}
KeyValueBox_Typedef;

/******************************4、变量声明****************************************/




/*******************************5、函数声明***************************************/
void BoxConfig(void);
void LcdUpdateMain(void);
void KeyScanMain(void);

#endif






