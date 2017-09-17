#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H
/**********************************1、头文件************************************/
#include "AD_Key.h"
#include "ui.h"
#include "ugui.h"

/**********************************2、宏定义***********************************/
#define KEY_SUM   16             //按键总数=16

/**********************************3、类型定义***********************************/
////AD_Key.c和ui.c之间的接口表结构体类型
typedef struct
{
	AbstractKey_Typedef Abstracrt_Key_Enum;
	ADKey_Typedef		AD_Key_Enum;
}KeyValueTable_Typedef;


/**********************************4、变量声明**********************************/
extern UG_GUI GUI_Struct;
extern KeyValueTable_Typedef KeyValueTable_Struct[];
/**********************************5、函数声明*********************************/
AbstractKey_Typedef ADKey_2_AdstractKey(ADKey_Typedef ADKey_Value);


#endif





