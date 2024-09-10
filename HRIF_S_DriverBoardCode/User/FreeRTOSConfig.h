/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "gd32f4xx.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#ifdef __ICCARM__
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif


#if 1//如果使用systemview
#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW_Freertos.h"
#define INCLUUDE_xTaskGetIdleTaskHandler 1
#define INCLUUDE_pxTaskGetStackStart 1
#endif



// 调度模式 1抢占 0协作
#define configUSE_PREEMPTION			1

// 0通用方法 1硬件方法 硬件方法强制限定最大优先级为32
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

//#define configUSE_IDLE_HOOK				1
//#define configUSE_TICK_HOOK				1

// 低功耗tickless模式 1开启 0关闭（一直运行）
#define configUSE_TICKLESS_IDLE                 0

// 内核时钟频率 
#define configCPU_CLOCK_HZ				( SystemCoreClock )

// 一秒内进行调度的次数 1ms调度一次
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )

// 配置1的最大优先级
#define configMAX_PRIORITIES			( 32 )

// 设置空闲任务栈大小
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 )

// 设置任务名字符长度
#define configMAX_TASK_NAME_LEN                 (30)

// 系统节拍器数据类型 1uint16_t 0uint32_t
#define configUSE_16_BIT_TICKS                  0

//空闲任务时直接执行同优先级任务 1开启 0关闭
#define configIDLE_SHOULD_YIELD                 1

//开启任务通知功能 1开启
#define configUSE_TASK_NOTIFICATIONS            0//systemview调用

//通知数组
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3

// 使用互斥信号量 1开启 0关闭
#define configUSE_MUTEXES                       1

// 使用递归互斥信号量 1开启 0关闭
#define configUSE_RECURSIVE_MUTEXES             1

// 使用计数信号量 1开起 0关闭
#define configUSE_COUNTING_SEMAPHORES           1

#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */

// 可以注册的信号量和消息队列个数
#define configQUEUE_REGISTRY_SIZE               10

// 使用队列设置
#define configUSE_QUEUE_SETS                    0


#define configUSE_TIME_SLICING                  0


#define configUSE_NEWLIB_REENTRANT              0


#define configENABLE_BACKWARD_COMPATIBILITY     1

// 进程本地存储指针大小
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

// 堆栈深度类型
#define configSTACK_DEPTH_TYPE                  uint16_t


#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* Memory allocation related definitions.
内存分配相关定义  仅使用动态分配*/

// 提供静态分配 1开 0关
#define configSUPPORT_STATIC_ALLOCATION             0

// 提供动态分配 1开 0关
#define configSUPPORT_DYNAMIC_ALLOCATION            1

// 总堆大小 50k
#define configTOTAL_HEAP_SIZE                       ((size_t)(50*1024))

//应用程序分配堆 1开启 0关闭 由系统自动分配
#define configAPPLICATION_ALLOCATED_HEAP            0

//栈从堆中分配
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   1


/* Hook function related definitions. 
钩子函数相关*/

// 使用空闲任务钩子
#define configUSE_IDLE_HOOK                     0

// 使用时间片钩子
#define configUSE_TICK_HOOK                     0

// 溢出检测钩子
#define configCHECK_FOR_STACK_OVERFLOW          0

// 内存申请失败钩子
#define configUSE_MALLOC_FAILED_HOOK            0

// 使用守护进程启动钩子
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. 
运行时间和任务状态收集相关函数*/

// 启用运行时间统计功能
#define configGENERATE_RUN_TIME_STATS           0

// 启用可视化跟踪
#define configUSE_TRACE_FACILITY                0

// 与可视化追踪同为1的时候会编译
/*prvWriteNameToBuffer
  vTaskList
  vTaskGetRunTimeStats
*/
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. 
与协程相关*/

// 启用协程序 ，需添加croutine.c
#define configUSE_CO_ROUTINES                   0

// 有效优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions.
软件定时器相关 */

// 启用软件定时器
#define configUSE_TIMERS                        1

// 软件优先级 31(基本放在最后)
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES-1)

// 设置定时器队列长度 10
#define configTIMER_QUEUE_LENGTH                10

// 软件定时器栈大小
#define configTIMER_TASK_STACK_DEPTH            (configMINIMAL_STACK_SIZE*2)

/* Interrupt nesting behaviour configuration. 中断配置*/
// 空闲任务相同优先级的任务优先执行
#define configIDLE_SHOULD_YIELD			1

//堆栈溢出检测方法，需用户提供钩子函数
#define configCHECK_FOR_STACK_OVERFLOW	0

//获取任务标签函数启用
#define configUSE_APPLICATION_TASK_TAG	1



/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

/* Optional functions - most linkers will remove unused functions anyway.
可选函数 可以选择使用的函数 */

//任务优先级设置
#define INCLUDE_vTaskPrioritySet                1

//任务优先级获取
#define INCLUDE_uxTaskPriorityGet               1

//任务删除
#define INCLUDE_vTaskDelete                     1

//任务挂起
#define INCLUDE_vTaskSuspend                    1

//任务从中断中恢复
#define INCLUDE_xResumeFromISR                  1

//delay直到
#define INCLUDE_vTaskDelayUntil                 1

//delay
#define INCLUDE_vTaskDelay                      1

//获取任务调度状态
#define INCLUDE_xTaskGetSchedulerState          1

//获取当前任务句柄
#define INCLUDE_xTaskGetCurrentTaskHandle       1

//获取栈区高水mark
#define INCLUDE_uxTaskGetStackHighWaterMark     0

//获取空闲任务句柄 暂时屏蔽
//#define INCLUDE_xTaskGetIdleTaskHandle          0

//获取状态
#define INCLUDE_eTaskGetState                   0

//中断事件组设置位
#define INCLUDE_xEventGroupSetBitFromISR        1

//定时器延迟回调？
#define INCLUDE_xTimerPendFunctionCall          0

//任务中止延时 返回用户自定义的数据类型
#define INCLUDE_xTaskAbortDelay                 0

//获取任务句柄
#define INCLUDE_xTaskGetHandle                  1

//任务从中断中恢复
#define INCLUDE_xTaskResumeFromISR              1

//任务清除资源
#define INCLUDE_vTaskCleanUpResources	          1



/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
//配置FreeRTOS用到的SysTick中断和PendSV中断的优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
//允许freertos控制的最大的优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	3

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

