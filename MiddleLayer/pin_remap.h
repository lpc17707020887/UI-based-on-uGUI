#ifndef _PIN_REMAP_H
#define _PIN_REMAP_H

//引用范围：AD_key.c
////////*********************************************************/ 
//按键的原理是ADC按键，这里使用了两个ADC引脚做输入，
//每个ADC引脚下面接了8个按键，一共16个按键
//一个是PC5 ,一个是PB0
#define ADC_KEY_PORT1		 GPIOC               
#define ADC_KEY_PORT1_Pin1   GPIO_Pin_5

#define ADC_KEY_PORT2		  GPIOB
#define ADC_KEY_PORT2_PIN1    GPIO_Pin_0
/*******************************************************//////////

//引用范围
////////////*******************************************************/
// 0，开背光，1：关背光
#define LCD_BL_PORT		GPIOE 	 
#define LCD_BL_PIN		GPIO_Pin_5   
// 1：数据，0：命令
#define LCD_RS_PORT		GPIOB 	 
#define LCD_RS_PIN		GPIO_Pin_12   
// LCD控制选择，低电平使能
#define LCD_CS_PORT		GPIOB    
#define LCD_CS_PIN      GPIO_Pin_14    
// LCD复位， 低电平复位 
#define LCD_RST_PORT	GPIOB    
#define LCD_RST_PIN    GPIO_Pin_5     
/*********************************************************///////////


#endif





