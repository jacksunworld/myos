
#ifndef  BSP_PRESENT
#define  BSP_PRESENT
#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


#include "includes.h"


void         BSP_Init(void);

//void         BSP_IntDisAll               (void);

INT32U   BSP_CPU_ClkFreq             (void);

void GPIO_Configuration(void);
void EXTI_Config(void);
void NVIC_Configuration(void);
void USART_Configuration(void);
void RCC_Configuration(void);
void TIM2_Config(void);



#endif                                                          /* End of module include.                               */
