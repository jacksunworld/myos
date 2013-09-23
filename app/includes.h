/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INCLUDES_H
#define __INCLUDES_H
/* Includes ------------------------------------------------------------------*/
//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>
//#include <stdlib.h>
//#include <stdarg.h>

//#include <stm32f10x.h>
#include <stm32f10x_it.h>

#include <bsp.h>
#include <myos.h>




void RCC_Configuration(void);
void NVIC_Configuration(void);
//void SysTick_Config(void);
void GPIO_Configuration(void);
void USART1_Configuration(void);
void USART2_Configuration(void);
void SysTick_Config(void);
void Delay_Ms(u32  time) ;

//#include <tft.h>
//#include <usart.h>
//#include <delay.h>

//#include <GUI.h>
//#include <hzk.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __INCLUDES_H */
