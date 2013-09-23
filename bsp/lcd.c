#include<lcd.h>


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  vu16 LCD_REG;
  vu16 LCD_RAM;
} LCD_TypeDef;

u8 iferaseBG1624=1;

/* Note: LCD /CS is CE4 - Bank 4 of NOR/SRAM Bank 1~4 */
#define LCD_BASE        ((u32)(0x60000000 | 0x0C000000))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

void LCD_ILI9341C_CMD(u16 LCD_Reg)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
}
void LCD_ILI9341C_Parameter(u16 LCD_RegValue)
{

  LCD->LCD_RAM = LCD_RegValue;
}

void LCD_CtrlWrite(u16 LCD_Reg)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
}
void LCD_DataWrite(u16 LCD_RegValue)
{

  LCD->LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;

/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 4 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  p.FSMC_AddressSetupTime = 0;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 1 ;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;

  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

  /* BANK 4 (of NOR/SRAM Bank 1~4) is enabled */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

void LCD9341_SetCursor(u16 x1, u16 y1, u16 x2, u16 y2)
{ 
  	LCD_CtrlWrite(0x2A);
	LCD_DataWrite(x1 >> 8);
	LCD_DataWrite(x1 & 0xFF);
	LCD_DataWrite(x2 >> 8);
	LCD_DataWrite(x2 & 0xFF);

	LCD_CtrlWrite(0x2B);
	LCD_DataWrite(y1 >> 8);
	LCD_DataWrite(y1 & 0xFF);
	LCD_DataWrite(y2 >> 8);
	LCD_DataWrite(y2 & 0xFF);
}

void LCD_blockClear_ILI9341DS(u16 x1, u16 y1, u16 x2, u16 y2, u16 data)
{
	u16 LCD_y,LCD_x;	

	LCD9341_SetCursor(x1,y1,x2,y2);	
	LCD_CtrlWrite(0x2C);

 	for(LCD_y=y1;LCD_y<=y2;LCD_y++)
	{
		for(LCD_x=x1;LCD_x<=x2;LCD_x++)
		{
			LCD->LCD_RAM = data;
		}
	}
}

#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800

/*******************************************************************************
* Function Name  : STM3210E_LCD_Init
* Description    : Initializes the LCD.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STM3210E_LCD_Init(void)
{ 
  vu16 dummy = 0;
/* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();
/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
  Delay_Ms(5); /* delay 5 ms */


//LCDRESETON;
//Delay(1);       // Delay 1ms 
//LCDRESETOFF; 
//Delay(20);       // Delay 20ms // This delay time is necessary 
//LCDRESETON; 
//Delay(120);       // Delay 120 ms 

  //************* Start Initial Sequence **********//HSD 
LCD_ILI9341C_CMD(0xEF);  
LCD_ILI9341C_Parameter (0x03); 
LCD_ILI9341C_Parameter (0x80); 
LCD_ILI9341C_Parameter (0x02); 
 
LCD_ILI9341C_CMD(0xCF);  
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0xAA); 
LCD_ILI9341C_Parameter (0XB0); 

LCD_ILI9341C_CMD(0xED);  
LCD_ILI9341C_Parameter (0x67); 
LCD_ILI9341C_Parameter (0x03); 
LCD_ILI9341C_Parameter (0X12); 
LCD_ILI9341C_Parameter (0X81); 

LCD_ILI9341C_CMD(0xE8);  
LCD_ILI9341C_Parameter (0x85); 
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x7A); 

LCD_ILI9341C_CMD(0xF6);  
LCD_ILI9341C_Parameter (0x01); 
LCD_ILI9341C_Parameter (0x30); 
LCD_ILI9341C_Parameter (0x00); 

LCD_ILI9341C_CMD(0xCB);  
LCD_ILI9341C_Parameter (0x39); 
LCD_ILI9341C_Parameter (0x2C); 
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x34); 
LCD_ILI9341C_Parameter (0x02); 
 
LCD_ILI9341C_CMD(0xF7);  
LCD_ILI9341C_Parameter (0x20); 
 
LCD_ILI9341C_CMD(0xEA);  
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x00); 

LCD_ILI9341C_CMD(0xC0);    //Power control 
LCD_ILI9341C_Parameter (0x21);   //VRH[5:0] 
 
LCD_ILI9341C_CMD(0xC1);    //Power control 
LCD_ILI9341C_Parameter (0x11);   //SAP[2:0];BT[3:0] 

LCD_ILI9341C_CMD(0xC5);    //VCM control 
LCD_ILI9341C_Parameter (0x20); //24//22
LCD_ILI9341C_Parameter (0x3C); 

LCD_ILI9341C_CMD(0xC7);    //VCM control2 
LCD_ILI9341C_Parameter (0xC1); //B0//194

LCD_ILI9341C_CMD(0x3A);  
LCD_ILI9341C_Parameter (0x55); 	//设置为 16位色
 
LCD_ILI9341C_CMD(0x36);    // Memory Access Control 
LCD_ILI9341C_Parameter (0xA8); 

LCD_ILI9341C_CMD(0xB1);  
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x12); 
LCD_ILI9341C_CMD(0xB4);  
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_CMD(0xB6);  
LCD_ILI9341C_Parameter (0x0A); 
LCD_ILI9341C_Parameter (0x82); //02 background black,82,background white

LCD_ILI9341C_CMD(0xF2);    // 3Gamma Function Disable 
LCD_ILI9341C_Parameter (0x00); 
 
LCD_ILI9341C_CMD(0x26);    //Gamma curve selected 
LCD_ILI9341C_Parameter (0x01); 

LCD_ILI9341C_CMD(0xE0);    //Set Gamma 
LCD_ILI9341C_Parameter (0x0F); 
LCD_ILI9341C_Parameter (0x17); 
LCD_ILI9341C_Parameter (0x16); 
LCD_ILI9341C_Parameter (0x0C); 
LCD_ILI9341C_Parameter (0x10); 
LCD_ILI9341C_Parameter (0x09); 
LCD_ILI9341C_Parameter (0x45); 
LCD_ILI9341C_Parameter (0xA9); 
LCD_ILI9341C_Parameter (0x33); 
LCD_ILI9341C_Parameter (0x08); 
LCD_ILI9341C_Parameter (0x0D); 
LCD_ILI9341C_Parameter (0x03); 
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x00); 
 
LCD_ILI9341C_CMD(0XE1);    //Set Gamma 
LCD_ILI9341C_Parameter (0x00); 
LCD_ILI9341C_Parameter (0x28); 
LCD_ILI9341C_Parameter (0x29); 
LCD_ILI9341C_Parameter (0x03); 
LCD_ILI9341C_Parameter (0x0F); 
LCD_ILI9341C_Parameter (0x06); 
LCD_ILI9341C_Parameter (0x3A); 
LCD_ILI9341C_Parameter (0x56); 
LCD_ILI9341C_Parameter (0x4C); 
LCD_ILI9341C_Parameter (0x07); 
LCD_ILI9341C_Parameter (0x12); 
LCD_ILI9341C_Parameter (0x0C); 
LCD_ILI9341C_Parameter (0x3F); 
LCD_ILI9341C_Parameter (0x3F); 
LCD_ILI9341C_Parameter (0x0F); 
 
LCD_ILI9341C_CMD(0x11);    //Exit Sleep 
Delay_Ms(120); 

LCD_ILI9341C_CMD(0x29);    //Display on 

LCD_ILI9341C_CMD(0x2C); //GRAM start writing 
//
//LCD_ILI9341C_CMD(0x2A); //GRAM start writing 
//LCD_ILI9341C_Parameter (0x00); 
//
//LCD_ILI9341C_CMD(0x2B); //GRAM start writing 
//LCD_ILI9341C_Parameter (0x00); 
// bgrnum=sizeof (Genwell_logo_16bit)/sizeof(Genwell_logo_16bit[0]);
LCD_blockClear_ILI9341DS(0, 0, 320 , 240, Red);
//LCD_ClearAll_ILI9341DS(Black);

}

void LCD_ExitSleep_ILI9341(void) 
{ 
	LCD_ILI9341C_CMD(0x11);   // Sleep out 
	Delay_Ms(120); 
	LCD_ILI9341C_CMD(0x29);   // Display on 
} 

void LCD_EnterSleep_ILI9341(void) 
{ 
	LCD_ILI9341C_CMD(0x28);   // Display off 
	Delay_Ms(20); 
	LCD_ILI9341C_CMD(0x10);   // Enter Sleep mode 
} 

void ScreenOFF(void)
{
//        I2C_WriteByte(126,0xee, 0);
        I2C_WriteByte(0,0xee, 1);
	    I2C_WriteByte(0,0xee, 2);
	    
	     LCD_EnterSleep_ILI9341();

	return;
}


void  ScreenON(void)
{
//    LCD_blockClear_ILI9341DS(0,0,ScreenWide,ScreenHigh-32,Black);
        LCD_ExitSleep_ILI9341();
 //唤醒屏幕
    I2C_WriteByte(126,0xee, 0);
    I2C_WriteByte(30,0xee, 1);
	I2C_WriteByte(30,0xee, 2);
	return;
}
