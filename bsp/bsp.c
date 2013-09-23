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

#define LCD_PWR_ON      GPIO_ResetBits(GPIOG, GPIO_Pin_13)  	//LCD��Դ��
#define LCD_PWR_OFF     GPIO_SetBits(GPIOG, GPIO_Pin_13)  		//LCD��Դ��
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
			//startup_stm32f10x_hd.s �ļ�zhong OS_CPU_SysTickHandler �жϺ������ж�ʱ��
	//��ϵͳʱ��(SYSCLK)
    OS_CPU_SysTickInit();                                       /* Initialize the SysTick.                              */

	
}

//�ܽ�����
//void GPIO_Configuration(void)
//{

//}

/*�����ж�����*/
void EXTI_Config(void)
{

}

//�ж�����
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
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel ;		//����TIM2�жϣ�1��������
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	 NVIC_Init(&NVIC_InitStructure);
	/* Enable the USART1 Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;	 // USART1 ȫ���ж� ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;		//����USART2�жϣ�0��������
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
	RCC_DeInit();												//��λʱ�ӵ��������

	RCC_HSEConfig(RCC_HSE_ON);									//�����ⲿ���پ���HSE 

	HSEStartUpStatus = RCC_WaitForHSEStartUp();				//�ȴ�HSE ��������

	if(HSEStartUpStatus == SUCCESS)							//HSE��������
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 						//HCLK = SYSCLK,AHB��Ƶϵ��Ϊ0

		RCC_PCLK2Config(RCC_HCLK_Div1); 						//PCLK2 = HCLK, APB2/PCLK2 ����Ƶ

		RCC_PCLK1Config(RCC_HCLK_Div1);						//PCLK1 = HCLK APB1/PCLK1 ��Ƶϵ��/1

//		FLASH_SetLatency(FLASH_Latency_2);						//Flash 2 wait state
//
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//Enable Prefetch Buffer

		RCC_ADCCLKConfig(RCC_PCLK2_Div2);						//ADC_CLK =  PLCK2/2=12MHz,��󲻳���14MHz

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_5);	//PLLCLK = 8MHz * 3 = 24 MHz,HSE��ΪPLL������ʱ��Դ

		RCC_PLLCmd(ENABLE);									//ʹ��PLL 

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	//�ȴ�PLL�����ȶ�

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 			//ѡ��PLL��SYSCLKʱ��Դ

		while(RCC_GetSYSCLKSource() != 0x08);					//�ȵ�PLL��ΪSYSCLKʱ��Դ�ȶ�
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		

	}

}
/*********************************************************************
 * �� �� ��: TIM2_Config
 * ��������: ��ʱ��2��ʼ��
 * ����˵��: ��������Ҫ���ж�Ƶ��
 * ���ú���: 
 * ȫ�ֱ���:
 * �䡡����: 
 * ��������:
 **********************************************************************/
void TIM2_Config()
{	
}
void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	//����GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	//����GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 	//����GPIOC clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 	//����GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 	//����GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 	//����GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 	//����GPIOG clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	 	//����AFIO clock

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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;			  		  	    //LCD��λ	  �ȵ��ٸ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;					//����GPIO�ٶ�Ϊ10MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					//����Ϊ���
	GPIO_Init(GPIOG, &GPIO_InitStructure);


}

void USART_Configuration(void)
{

}
