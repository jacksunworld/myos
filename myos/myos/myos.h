
#include <cpu.h>

#define OS_LOWEST_PRIO           7u   /* 最低优先级*//* Defines the lowest priority that can be assigned ...         */

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

#ifndef  OS_FALSE
#define  OS_FALSE                       0u
#endif

#ifndef  OS_TRUE
#define  OS_TRUE                        1u
#endif

/*
*********************************************************************************************************
*                                             ERROR CODES
*********************************************************************************************************
*/
#define OS_ERR_NONE                     0u

typedef  INT8U    OS_PRIO;

/* --------------------- 任务管理 ---------------------- *//* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    0u   /* 使用 OSTaskChangePrio() *//*     Include code for OSTaskChangePrio()                      */
#define OS_TASK_CREATE_EN         1u   /* 使用 OSTaskCreate() *//*     Include code for OSTaskCreate()                          */
#define OS_TASK_CREATE_EXT_EN     1u   /* 使用 OSTaskCreateExt() *//*     Include code for OSTaskCreateExt()                       */
#define OS_TASK_DEL_EN            1u   /* 使用 OSTaskDel() *//*     Include code for OSTaskDel()                             */


typedef struct os_tcb {
    OS_STK          *OSTCBStkPtr;           /* Pointer to current top of stack                         */
    
#if OS_TASK_CREATE_EXT_EN > 0u
    void            *OSTCBExtPtr;           /* Pointer to user definable data for TCB extension        */
    OS_STK          *OSTCBStkBottom;        /* Pointer to bottom of stack                              */
    INT32U           OSTCBStkSize;          /* Size of task stack (in number of stack elements)        */
    INT16U           OSTCBOpt;              /* Task options as passed by OSTaskCreateExt()             */
    INT16U           OSTCBId;               /* Task ID (0..65535)                                      */
#endif


    struct os_tcb   *OSTCBNext;             /* Pointer to next     TCB in the TCB list                 */
    struct os_tcb   *OSTCBPrev;             /* Pointer to previous TCB in the TCB list                 */

    INT32U           OSTCBDly;              /* Nbr ticks to delay task or, timeout waiting for event   */
    INT8U            OSTCBStat;             /* Task      status                                        */
    INT8U            OSTCBStatPend;         /* Task PEND status                                        */
    INT8U            OSTCBPrio;             /* Task priority (0 == highest)                            */
    INT32U           OSTCBCtxSwCtr;         /* Number of time the task was switched in                 */

}OS_TCB;

#define  OS_ASCII_NUL            (INT8U)0

#define  OS_PRIO_SELF                0xffu              /* Indicate SELF priority                      */

#if OS_TASK_STAT_EN > 0u
#define  OS_N_SYS_TASKS                 2u              /* Number of system tasks                      */
#else
#define  OS_N_SYS_TASKS                 1u
#endif

#define OS_MAX_TASKS             7u 

#define  OS_TASK_STAT_PRIO  (OS_LOWEST_PRIO - 1u)       /* Statistic task priority                     */
#define  OS_TASK_IDLE_PRIO  (OS_LOWEST_PRIO)            /* IDLE      task priority                     */

#if OS_LOWEST_PRIO <= 63u
#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 8u + 1u) /* Size of event table                         */
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 8u + 1u) /* Size of ready table  first version support 8 prio  */
#else
#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 16u + 1u)/* Size of event table                         */
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 16u + 1u)/* Size of ready table                         */
#endif

#define  OS_TASK_IDLE_ID            65535u              /* ID numbers for Idle, Stat and Timer tasks   */

#define  OS_TCB_RESERVED        ((OS_TCB *)1)

#define OS_TASK_IDLE_STK_SIZE   128u
#define  OS_TASK_OPT_STK_CHK       0x0001u  /* Enable stack checking for the task                      */
#define  OS_TASK_OPT_STK_CLR       0x0002u  /* Clear the stack when the task is create                 */

/*********************************************************************************************************
*                              TASK STATUS (Bit definition for OSTCBStat)
*********************************************************************************************************
*/
#define  OS_STAT_RDY                 0x00u  /* Ready to run                                            */
#define  OS_STAT_SEM                 0x01u  /* Pending on semaphore                                    */
#define  OS_STAT_MBOX                0x02u  /* Pending on mailbox                                      */
#define  OS_STAT_Q                   0x04u  /* Pending on queue                                        */
#define  OS_STAT_SUSPEND             0x08u  /* Task is suspended                                       */
#define  OS_STAT_MUTEX               0x10u  /* Pending on mutual exclusion semaphore                   */
#define  OS_STAT_FLAG                0x20u  /* Pending on event flag group                             */
#define  OS_STAT_MULTI               0x80u  /* Pending on multiple events                              */


#define  OS_STAT_PEND_ANY         (OS_STAT_SEM | OS_STAT_MBOX | OS_STAT_Q | OS_STAT_MUTEX | OS_STAT_FLAG)

/*
*********************************************************************************************************
*                           TASK PEND STATUS (Status codes for OSTCBStatPend)
*********************************************************************************************************
*/
#define  OS_STAT_PEND_OK                0u  /* Pending status OK, not pending, or pending complete     */
#define  OS_STAT_PEND_TO                1u  /* Pending timed out                                       */
#define  OS_STAT_PEND_ABORT             2u  /* Pending aborted                                         */



#define OS_ERR_MBOX_FULL               20u

#define OS_ERR_Q_FULL                  30u
#define OS_ERR_Q_EMPTY                 31u

#define OS_ERR_PRIO_EXIST              40u
#define OS_ERR_PRIO                    41u
#define OS_ERR_PRIO_INVALID            42u

#define OS_ERR_SCHED_LOCKED            50u
#define OS_ERR_SEM_OVF                 51u

#define OS_ERR_TASK_CREATE_ISR         60u
#define OS_ERR_TASK_DEL                61u
#define OS_ERR_TASK_DEL_IDLE           62u
#define OS_ERR_TASK_DEL_REQ            63u
#define OS_ERR_TASK_DEL_ISR            64u
#define OS_ERR_TASK_NAME_TOO_LONG      65u
#define OS_ERR_TASK_NO_MORE_TCB        66u
#define OS_ERR_TASK_NOT_EXIST          67u
#define OS_ERR_TASK_NOT_SUSPENDED      68u
#define OS_ERR_TASK_OPT                69u
#define OS_ERR_TASK_RESUME_PRIO        70u
#define OS_ERR_TASK_SUSPEND_IDLE       71u
#define OS_ERR_TASK_SUSPEND_PRIO       72u
#define OS_ERR_TASK_WAITING            73u



OS_EXT  INT32U            OSCtxSwCtr;               /* Counter of number of context switches           */

OS_EXT  INT8U             OSIntNesting;             /* Interrupt nesting level                         */

OS_EXT  INT8U             OSLockNesting;            /* Multitasking lock nesting level                 */

OS_EXT  INT8U             OSPrioCur;                /* Priority of current task                        */
OS_EXT  INT8U             OSPrioHighRdy;            /* Priority of highest priority task               */

OS_EXT  OS_PRIO           OSTaskRdy;                        /* Ready task byte                    */

OS_EXT  BOOLEAN           OSRunning;                       /* Flag indicating that kernel is running   */

OS_EXT  INT8U             OSTaskCtr;                       /* Number of tasks created                  */

OS_EXT  volatile  INT32U  OSIdleCtr;                                 /* Idle counter                   */

OS_EXT  OS_STK            OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];      /* Idle task stack                */

OS_EXT  OS_TCB           *OSTCBCur;                        /* Pointer to currently running TCB         */
OS_EXT  OS_TCB           *OSTCBFreeList;                   /* Pointer to list of free TCBs             */
OS_EXT  OS_TCB           *OSTCBHighRdy;                    /* Pointer to highest priority TCB R-to-R   */
OS_EXT  OS_TCB           *OSTCBList;                       /* Pointer to doubly linked list of TCBs    */
OS_EXT  OS_TCB           *OSTCBPrioTbl[OS_LOWEST_PRIO + 1u];    /* Table of pointers to created TCBs   */
OS_EXT  OS_TCB            OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];   /* Table of TCBs                  */

#if OS_LOWEST_PRIO <= 63u
typedef  INT8U    OS_PRIO;
#else
typedef  INT16U   OS_PRIO;
#endif

OS_STK       *OSTaskStkInit           (void           (*task)(void *p_arg),
                                       void            *p_arg,
                                       OS_STK          *ptos,
                                       INT16U           opt);


#define OS_TICKS_PER_SEC       1000u   /* 设定一秒节拍的数量 *//* Set the number of ticks in one second                        */
                      /* Idle counter                   */

void          OSInitHookBegin         (void);
void  OSInit (void);
void  OSStart (void);
void          OS_Sched                (void);
INT8U         OS_TCBInit              (INT8U            prio,
                                       OS_STK          *ptos,
                                       OS_STK          *pbos,
                                       INT16U           id,
                                       INT32U           stk_size,
                                       void            *pext,
                                       INT16U           opt);

INT8U         OSTaskCreateExt         (void           (*task)(void *p_arg),
                                       void            *p_arg,
                                       OS_STK          *ptos,
                                       INT8U            prio,
                                       INT16U           id,
                                       OS_STK          *pbos,
                                       INT32U           stk_size,
                                       void            *pext,
                                       INT16U           opt);
static  void  OS_InitMisc (void);

void          OSIntEnter              (void);
void          OSIntExit               (void);
void          OSTimeDly               (INT32U           ticks);
INT8U OS_CreatTask (void   (*task)(void *p_arg),
                     void    *p_arg,
                     OS_STK  *ptos,
                     INT8U    prio);
                     
void          OS_TaskReturn           (void);      
void          OSTaskReturnHook        (OS_TCB          *ptcb);
void  OSTimeTick (void);


               

