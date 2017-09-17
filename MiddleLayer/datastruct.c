#include "datastruct.h"

//main.c中使用
/********************************************************************************************************************************************************************/
UG_GUI GUI_Struct;


//AD_Key.c和ui.c之间的接口表
/********************************************************************************************************************************************************************/
/**********************************************************************
**AD_Key模块--按键为ADKey_Typedef
**ui模块--按键为AbstractKey_Typedef 
**该表为ui模块与ADKey模块按键键值的接口
**********************************************************************/
KeyValueTable_Typedef KeyValueTable_Struct[] =
{
	{ KEY_NONE, KEY_AD_GND},    
	{ KEY_SET,  KEY_AD2_8},    
	{ KEY_OK,   KEY_AD2_5},	   
	{ KEY_CLR,  KEY_AD2_4},	   
	{ KEY_UP,   KEY_AD2_7},	   
	{ KEY_DOWN, KEY_AD2_6},	   
	{ KEY_0,    KEY_AD1_5},	   
	{ KEY_1,    KEY_AD1_1},	   
	{ KEY_2,    KEY_AD1_8},	   
	{ KEY_3,    KEY_AD2_1},	   
	{ KEY_4,    KEY_AD1_2},	   
	{ KEY_5,    KEY_AD1_7},	   
	{ KEY_6,    KEY_AD2_2},	   
	{ KEY_7,    KEY_AD1_3},	   
	{ KEY_8,    KEY_AD1_6},	   
	{ KEY_9,    KEY_AD2_3},	   
	{ KEY_POINT,KEY_AD1_4},	   
};


/**********************************************************************
** 函数名称         :ADKey_2_AdstractKey   
** 创建人           :liupengcheng
** 创建日期         :2017/9/12 16:51
** 最新修改人       :liupengcheng
** 最近修改日期      :2017/9/12 16:51
** 功能描述         :通过查表的方式查出BSP层ADCKey对应应用层AbstractKey
** 入口参数         :
					--ADKey_Value:AD按键键值
** 返回参数         :
					--AbstractKey_Value
** 备注/注意        :该函数是衔接ADKey.c和UI.c键值的接口
** QQ              : 1240612852
***********************************************************************/
AbstractKey_Typedef ADKey_2_AdstractKey(ADKey_Typedef ADKey_Value)
{
	u8 i;
	for (i = 0; i < KEY_SUM; i++)
	{
		if (KeyValueTable_Struct[i].AD_Key_Enum == ADKey_Value)
		{
			return KeyValueTable_Struct[i].Abstracrt_Key_Enum;
		}
	}
	return KEY_NONE;
}

