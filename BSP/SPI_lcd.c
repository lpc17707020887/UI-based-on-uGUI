#include "SPI_lcd.h"

/**********************************************************************
**LCD硬件描述：LCD为spi接口，引脚定义如下所示
**LCD_BL		GPIOE, GPIO_Pin_5	 // 0，关闭，1：打开
**LCD_RS		GPIOB, GPIO_Pin_12	 // 1：数据，0：命令
**LCD_CS		GPIOB, GPIO_Pin_14   // LCD控制选择，低电平使能
**LCD_RST		GPIOB, GPIO_Pin_5    // LCD复位，	低电平复位
**LCD_MOSI      GPIOB, GPIO_Pin_15    
**LCD_SCLK      GPIOB, GPIO_Pin_13
**LCD像素为64（行）*132（列）
**
**
**显存为132*8字节，
**********************************************************************/
u8 g_Display_Buffer[8][132] = {0x00};
/**********************************************************************
**0列--------------------------------------------------------------132列
**0000000000000000000000000000000000000000000000000000000000000000000000
**xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
**1010101010101010101010101010101010101010101010101010101010101010101010
**0101010101010101010101010101010101010101010101010101010101010101010101
**
**
**
**
**********************************************************************/

void Delay(unsigned int tDly)
{
	while (tDly > 0)
	{
		tDly--;
	}
}



/**********************************************************************
**函数名称   ：   SdCmd
**创建人     ：   guoyu
**创建日期   ：   2013/05/28
**功能描述   ：   命令写入函数
**入口参数   ：   None
**返回参数   ：   None
**备注/注意  ：   None
**********************************************************************/
void SdCmd(uint8_t Command)
{
	GPIO_ResetBits(LCD_RS_PORT,LCD_RS_PIN);
	GPIO_ResetBits(LCD_CS_PORT,LCD_CS_PIN);
	Delay(1);

	SPI_I2S_SendData(SPI2, (uint16_t)Command);

	Delay(5);
	GPIO_SetBits(LCD_CS_PORT,LCD_CS_PIN);
}

/**********************************************************************
**函数名称   ：   SdData
**创建人     ：   guoyu
**创建日期   ：   2013/05/28
**功能描述   ：   显示数据写入函数
**入口参数   ：   None
**返回参数   ：   None
**备注/注意  ：   None
**********************************************************************/
void SdData(uint8_t DData)
{
	GPIO_SetBits(LCD_RS_PORT, LCD_RS_PIN);
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN);
	Delay(1);

	SPI_I2S_SendData(SPI2, (uint16_t)DData);

	Delay(5);
	GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN);
}

void initLCDM(void)
{
	SdCmd(0xaf); // 开显示
	SdCmd(0x40); // 设置显示起始行=0
	SdCmd(0xa0); // RAM列地址与列驱动同顺序
				 //SdCmd(0xa1);                                      // RAM列地址与列驱动逆顺序  LM6063C
	SdCmd(0xa6); // 正向显示
	SdCmd(0xa4); // 显示全亮功能关闭
	SdCmd(0xa2); // LCD偏压比为1/9
				 //SdCmd(0xc0);                                      // 行驱动方向为正向 LM6063C
	SdCmd(0xc8); // 行驱动方向为反向
	SdCmd(0x2f); // 启用内部LCD驱动电源
	SdCmd(0x25); // 电阻比
	SdCmd(0xf8);
	SdCmd(0x00); // 倍压设置为4X
				 //SdCmd(0xf8);
				 //SdCmd(0x01);                                             // 倍压设置为5X     LM6060C
	SdCmd(0x81);
	SdCmd(DEFAULT_CONTRAS); // 设置对比度
}

/**********************************************************************
**函数名称   :   ClearRAM
**创建人     :   guoyu
**创建日期   :   2013/05/28
**功能描述   :   清屏函数
**入口参数   :   None
**返回参数   :   None
**备注/注意  :   None
**********************************************************************/
void ClearRAM(void)
{
	unsigned char i, j;

	for (i = 0; i < 8; i++) // 循环写8页
	{
		SdCmd(i | 0xb0);		  // 页地址设置
		SdCmd(0x10);			  // 列地址高4位
		SdCmd(0x00);			  // 列地址低4位
		for (j = 0; j < 132; j++) // 循环写132单元
		{
			SdData(0x00);			  // 数据设置为0
		}
	}
}



/**********************************************************************
**函数名称   ：   Lcd_Init
**创建人     ：   guoyu
**创建日期   ：   2013/05/28
**功能描述   ：   Lcd模块初始化，配置硬件SPI
**入口参数   ：   None
**返回参数   ：   None
**备注/注意  ：   None
**********************************************************************/
void LcdInit(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_SetBits(LCD_RST_PORT,LCD_RST_PIN);
	GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;         //SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        //SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  // SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;		    //CRCPolynomial;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);

	GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);
	GPIO_SetBits(LCD_CS_PORT,LCD_CS_PIN);

	initLCDM();
	ClearRAM();
}

/**********************************************************************
** 函数名称         :FLUSHScreenRAM
** 创建人           :liupcngcheng
** 创建日期         :2017-9-8
** 最新修改人       :liupengcheng
** 最近修改日期      :2017-9-8
** 功能描述         : 将显存上的数据推送到LCD上显示
** 入口参数         : none
** 返回参数         : none
** 备注/注意        : 所有对
** QQ              : 1240612852
***********************************************************************/
void UPDATE_ScreenRAM()
{
	unsigned char i, j;

	for (i = 0; i < 8; i++) // 循环写8页
	{
		SdCmd(i | 0xb0);		  // 页地址设置
		SdCmd(0x10);			  // 列地址高4位
		SdCmd(0x00);			  // 列地址低4位
		for (j = 0; j < 132; j++) // 循环写132单元
		{
			SdData(g_Display_Buffer[i][j]);			  
		}
	}
}

/**********************************************************************
** 函数名称         :LCDDrawDot
** 创建人           :liupcngcheng
** 创建日期         :2017-9-8
** 最新修改人       :liupengcheng
** 最近修改日期     :2017-9-8
** 功能描述         :在132（列）*64（行）的LCD二值屏上面打点
** 入口参数         :
						--x：X轴坐标，原点为0
						--y：y轴坐标，原点为0
						--color：颜色  例0xffffffff为白色
** 返回参数         :none
** 备注/注意        :作为接口衔接ugui图像库
** QQ              : 1240612852
***********************************************************************/ 
void LCDDrawDot(s16 x, s16 y, u32 color)
{
	u8 row_byte,row_bit;
	row_byte = y / 8;
	row_bit = y % 8;
	if (color == 0x00FFFFFF)   //白色
	{
		g_Display_Buffer[row_byte][x] &= (~(0x01 << row_bit));
	}
	else if(color == 0x00000000)                       //黑色
	{
		g_Display_Buffer[row_byte][x] |= (0x01 << row_bit);
	}
}

