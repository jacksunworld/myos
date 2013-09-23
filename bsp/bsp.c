/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2007-2008; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include <bsp.h>
#include <cpu.h>
#include "stm32f10x_lib.h"
#include "I2C_Driver.h"
static volatile ErrorStatus HSEStartUpStatus = SUCCESS;

#define LCD_PWR_ON      GPIO_ResetBits(GPIOG, GPIO_Pin_13)  	//LCD电源开
#define LCD_PWR_OFF     GPIO_SetBits(GPIOG, GPIO_Pin_13)  		//LCD电源关
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                /* -------------------- GPIOA PINS -------------------- */

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configures the different GPIO ports pins.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PD.08, PD.12 and PD.14 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure PE.00 and PE.01 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Configure PB.09 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* PD.09 used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* RIGHT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);

  /* LEFT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);

  /* DOWN Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource14);

  /* UP Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);

  /* SEL Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);

  /* KEY Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
}

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 void SysTick_Config(void)
{
  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
 
  /* SysTick interrupt each 100 Hz with HCLK equal to 72MHz */
  SysTick_SetReload(720000);

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}


void  BSP_Init (void)
{

	RCC_Configuration();	
	SysTick_Config();

	LCD_PWR_ON;

	/* Configure the GPIOs */
  	GPIO_Configuration();

		/* NVIC configuration */
  	NVIC_Configuration();
			//startup_stm32f10x_hd.s 文件zhong OS_CPU_SysTickHandler 中断函数的中断时间
	//即系统时钟(SYSCLK)
    OS_CPU_SysTickInit();                                       /* Initialize the SysTick.                              */

	
}

//管脚配置
//void GPIO_Configuration(void)
//{

//}

/*配置中断引脚*/
void EXTI_Config(void)
{

}

//中断配置
//void NVIC_Configuration(void)
//{

//}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                         OS CORTEX-M3 FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* Description : Get system tick clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Clock frequency (of system tick).
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    return (freq);
}

void NVIC_Configuration(void)
{   
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  	/* Set the Vector Table base location at 0x20000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  	/* Set the Vector Table base location at 0x08000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel ;		//配置TIM2中断，1号子优先
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	 NVIC_Init(&NVIC_InitStructure);
	/* Enable the USART1 Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;	 // USART1 全局中断 ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;		//配置USART2中断，0号子优先
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();												//复位时钟的相关设置

	RCC_HSEConfig(RCC_HSE_ON);									//允许外部高速晶振HSE 

	HSEStartUpStatus = RCC_WaitForHSEStartUp();				//等待HSE 工作正常

	if(HSEStartUpStatus == SUCCESS)							//HSE工作正常
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 						//HCLK = SYSCLK,AHB分频系数为0

		RCC_PCLK2Config(RCC_HCLK_Div1); 						//PCLK2 = HCLK, APB2/PCLK2 不分频

		RCC_PCLK1Config(RCC_HCLK_Div1);						//PCLK1 = HCLK APB1/PCLK1 分频系数/1

//		FLASH_SetLatency(FLASH_Latency_2);						//Flash 2 wait state
//
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//Enable Prefetch Buffer

		RCC_ADCCLKConfig(RCC_PCLK2_Div2);						//ADC_CLK =  PLCK2/2=12MHz,最大不超过14MHz

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_5);	//PLLCLK = 8MHz * 3 = 24 MHz,HSE作为PLL的输入时钟源

		RCC_PLLCmd(ENABLE);									//使能PLL 

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	//等待PLL工作稳定

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 			//选择PLL作SYSCLK时钟源

		while(RCC_GetSYSCLKSource() != 0x08);					//等等PLL作为SYSCLK时钟源稳定
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		

	}

}
/*********************************************************************
 * 函 数 名: TIM2_Config
 * 功能描述: 定时器2初始化
 * 函数说明: 根据所需要的中断频率
 * 调用函数: 
 * 全局变量:
 * 输　　入: 
 * 返　　回:
 **********************************************************************/
void TIM2_Config()
{	
}
void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	//激活GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	//激活GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 	//激活GPIOC clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 	//激活GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 	//激活GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 	//激活GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 	//激活GPIOG clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	 	//激活AFIO clock

  	/* Configure Pb.8-11 as output push-pull */
  	

//  	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//  	/* Configure USART1 Rx (PA.10) as input floating */
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	 //USART2 RX
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;			  		  	    //LCD复位	  先低再高
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;					//设置GPIO速度为10MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					//设置为输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);


}

void USART_Configuration(void)
{

}
