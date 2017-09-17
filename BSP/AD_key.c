#include "AD_key.h"
/**********************************************************************
**电路介绍：2通道ADC按键，1个ADC通道下面接有8个按键，一共8*2=16个按键
**(ADC1)PB0脚接：----KEY_1----KEY_4----KEY_7----KEY_POINT----KEY_0----KEY_8----KEY_5----KEY_2----
**                  0.302v   0.611v   0.901v     1.19v      1.48V    1.82V    2.09V    2.4V
**(ADC2)PC5脚接：----KEY_3----KEY_6----KEY_9----KEY_CLR----KEY_OK----KEY_DOWN----KEY_UP----KEY_SET----
**                  0.302v   0.611v   0.901v     1.19v      1.48V    1.82V       2.09V     2.4V
**备注：当没有任何按键按下的时候，ADC引脚为低电平0V
**********************************************************************/

//这里存放两个通道得到dma值，每个通道采样16次，通过DMA填充到该buffer内，
u32 ADC_DMAStorageValue[16][2];
//这里存放两个通道分别16个值计算得出的平均值
u32 ADC_AveraValue[2];
//按键容差值

/**********************************************************************
** 函数名称         :KeyConfig
** 创建人           :liupcngcheng
** 创建日期         :2017-9-2
** 最新修改人       :
** 最近修改日期      :
** 功能描述         :初始化按键IO对应的外设的配置
** 入口参数         :none
** 返回参数         :none
** 备注/注意        :初始化/PB0/PC5/口为模拟输入并配置为ADC输入模式
** QQ              : 1240612852
***********************************************************************/
void KeyInit()
{
	
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;

	//RCC Config
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);   //APB2=16M   ADCClk=8M

	memset(ADC_DMAStorageValue, 32, 0);
	//GPIO Config
	/**********************************************************************/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = ADC_KEY_PORT1_Pin1;     //PC5
	GPIO_Init(ADC_KEY_PORT1, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = ADC_KEY_PORT2_PIN1;     //PB0
	GPIO_Init(ADC_KEY_PORT2, &GPIO_InitStruct);


	//ADC Config
	/**********************************************************************/
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_NbrOfChannel = 2;

	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	    //PB0	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_55Cycles5);    //PC5
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

	//DMA config
	/**********************************************************************/
	DMA_DeInit(DMA1_Channel1); 
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;					/*ADC地址*/ 
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&ADC_DMAStorageValue;			/*内存地址*/
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;							//外设为数据源 
	DMA_InitStruct.DMA_BufferSize = 32; 
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			 /*外设地址固定*/ 
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;					 /*内存地址递增*/ 
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	 //半字 
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;							 //循环传输 
	DMA_InitStruct.DMA_Priority = DMA_Priority_High; 
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel1, &DMA_InitStruct); 
	/* Enable DMA channel1 */ 
	DMA_Cmd(DMA1_Channel1, ENABLE);

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**********************************************************************
** 函数名称         :Get_ADCAveraValue
** 创建人           :liupcngcheng
** 创建日期         :2017年9月11日
** 最新修改人       :liupengcheng
** 最近修改日期     :2017年9月11日
** 功能描述         :将ADC_DMAStorageValue[2][16]里面得两个ADC值取平均值计算出来
** 入口参数         :u32* Return_ADCAvera_Value为指向全局变量ADC_AveraValue[2]的指针，返回获取的adc平均值
** 返回参数         :none
** 备注/注意        :输入指针参数为输出值
** QQ              : 1240612852
***********************************************************************/ 
void Get_ADCAveraValue(u32* Return_ADCAvera_Value)
{
	u8 i;
	u32 adc_sum1=0,adc_sum2=0;
	for (i = 0; i < 16; i++)
	{
		adc_sum1 += ADC_DMAStorageValue[i][0];
		adc_sum2 += ADC_DMAStorageValue[i][1];
	}
	Return_ADCAvera_Value[0] = adc_sum1 >> 4;
	
	Return_ADCAvera_Value[1] = adc_sum2 >> 4;
}


/**********************************************************************
** 函数名称         :GetKeyValue
** 创建人           :liupengcheng
** 创建日期         :2017/9/4 8:46
** 最新修改人       :liupengcheng
** 最近修改日期      :2017-9-12	
** 功能描述         :以ADKey_Typedef的结构返回当前按键
** 入口参数         :none
** 返回参数         :ADKey_Typedef类型枚举按键
** 备注/注意        :作为AD_Key.c和ui.c的接口
** QQ              : 1240612852
***********************************************************************/
ADKey_Typedef GetKeyValue(void)
{
	Get_ADCAveraValue(ADC_AveraValue);

	if (ADC_AveraValue[0] > (KEY_AD_GND + AD_KEY_TELORATE_VALUE))
	{
		//ADC第一个通道下的按键
		if (ADC_AveraValue[0] > (KEY_AD1_1 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_1 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_1;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_2 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_2 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_2;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_3 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_3 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_3;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_4 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_4 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_4;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_5 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_5 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_5;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_6 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_6 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_6;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_7 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_7 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_7;
		}
		else if (ADC_AveraValue[0] > (KEY_AD1_8 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[0] < (KEY_AD1_8 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD1_8;
		}
	}
	else if (ADC_AveraValue[1] > (KEY_AD_GND + AD_KEY_TELORATE_VALUE))
	{
		//ADC第二个通道下的按键
		if (ADC_AveraValue[1] > (KEY_AD2_1 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_1 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_1;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_2 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_2 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_2;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_3 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_3 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_3;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_4 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_4 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_4;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_5 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_5 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_5;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_6 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_6 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_6;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_7 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_7 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_7;
		}
		else if (ADC_AveraValue[1] > (KEY_AD2_8 - AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD2_8 + AD_KEY_TELORATE_VALUE))
		{
			return KEY_AD2_8;
		}
	}
	else if (ADC_AveraValue[0] < (KEY_AD_GND + AD_KEY_TELORATE_VALUE) && ADC_AveraValue[1] < (KEY_AD_GND + AD_KEY_TELORATE_VALUE))
	{
		return KEY_AD_GND;
	}
    return KEY_AD_GND;

	
}




