#include <bsp.h>

#define  APP_TASK_START_STK_SIZE                         128
#define  APP_TASK_1_STK_SIZE							 128
#define  APP_TASK_2_STK_SIZE							 128
#define  APP_TASK_3_STK_SIZE							 128
#define  APP_START_PRIO								 	 1
#define  APP_TASK_1_PRIO								 2
#define  APP_TASK_2_PRIO								 3
#define  APP_TASK_3_PRIO								 4
static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK App_Task1Stk1[APP_TASK_1_STK_SIZE];
static  OS_STK App_Task2Stk2[APP_TASK_2_STK_SIZE];
static  OS_STK App_Task2Stk3[APP_TASK_3_STK_SIZE];

static  void App_TaskCreate(void);

static  void  App_TaskStart(void *p_arg);
static  void  App_Task1(void *p_arg);
static  void  App_Task2(void *p_arg);
//static  void  App_Task3(void *p_arg);

int taskstatus;
static u32 TimingDelay = 0; 

static  OS_STK App_Task1Stk1[APP_TASK_1_STK_SIZE];


/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}


void Delay_Ms(u32 nCount)
{

	OS_CPU_SR  cpu_sr = 0u;

	OS_ENTER_CRITICAL();
	TimingDelay = nCount;
	OS_EXIT_CRITICAL();
	//SysTick_CounterCmd(SysTick_Counter_Enable);				//允许SysTick Counter工作
	while(TimingDelay != 0)									//延时，debug，需要增加WatchDog
	{
	}
	//SysTick_CounterCmd(SysTick_Counter_Disable);				//禁止SysTick Counter工作
	//SysTick_CounterCmd(SysTick_Counter_Clear);					//清除SysTick Counter
}



int main(void)
{
  
    INT8U  os_err;
    OSInit();                                                  
    os_err = OS_CreatTask((void (*)(void *)) App_TaskStart,(void* )0,(OS_STK* )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],(INT8U) APP_START_PRIO);
    OSStart();
    return (os_err);
}



//创建新任务
static  void App_TaskCreate(void)
{
    INT8U  os_err;

    os_err = OS_CreatTask((void (*)(void *)) App_Task1,(void* )0,(OS_STK* )&App_Task1Stk1[APP_TASK_1_STK_SIZE - 1],(INT8U) APP_TASK_1_PRIO);
	os_err = OS_CreatTask((void (*)(void *)) App_Task2,(void* )0,(OS_STK* )&App_Task2Stk2[APP_TASK_2_STK_SIZE - 1],(INT8U) APP_TASK_2_PRIO);
//	os_err = OS_CreatTask((void (*)(void *)) App_Task3,(void* )0,(OS_STK* )&App_Task2Stk3[APP_TASK_3_STK_SIZE - 1],(INT8U) APP_TASK_3_PRIO);
    taskstatus=os_err;

}

static  void  App_Task1(void *p_arg)
{	

//	(void)p_arg;
	
//		OSTimeDly(5);
    while(1)
	{

//		LCD_blockClear_ILI9341DS(0, 0, 320 , 240, 0xF800);
		OSTimeDly(500);
	}
	
}

static  void  App_Task2(void *p_arg)
{	

//	(void)p_arg;
	
//		OSTimeDly(5);
    while(1)
	{

//		LCD_blockClear_ILI9341DS(0, 0, 320 , 240, 0x001F);
		OSTimeDly(100);
	}
	
}


static  void  App_TaskStart (void *p_arg)
{	 
    (void)p_arg;
	 
    BSP_Init();                                                 /* Initialize BSP functions.                            */	
	App_TaskCreate();
//	 while(1)
//	{
//
//		LCD_blockClear_ILI9341DS(0, 0, 320 , 240, 0x001F);
//		OSTimeDly(100);
//	}
	OSTaskDel(OS_PRIO_SELF);

}


