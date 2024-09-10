/*
 * Copyright (c) 2023,杭可仪器第一研究所
 * All rights reserved.
 *
 * 文件名称：
 * 文件标识：见配置管理计划书
 * 摘 要：
 *
 * 当前版本：1.1
 * 作 者：
 * 完成日期：2024年4月8日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2024年4月8日
 */
#ifndef _main_H
#define _main_H

//#define USER_JLINK

#pragma anon_unions

/*标准库*/
#include "gd32f4xx.h"
#include "gd32f4xx_misc.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_exmc.h"
#include "gd32f4xx_syscfg.h"
#include "gd32f4xx_pmu.h"
#include "gd32f4xx_fmc.h"
//#include "gd32f4xx_adc.h"
//#include "gd32f4xx_timer.h"
/*c库*/
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"

/*FreeRTOS相关*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "portmacro.h"

#include "user_SysType.h"
#include "user_PC_Data.h"
/*主控板驱动*/
#include "user_cpld.h"
#include "user_delay.h"
#include "user_eth.h"
#include "user_Time.h"
#include "user_task.h"
#include "gd32f4xx_enet_eval.h"
#include "user_updata.h"

/*项目底板外设驱动*/

#include "user_all_gpio_init.h"
#include "AD8688.h"
#include "FPGA.h"
#include "SGM4588.h"
#include "SGM5349.h"
#include "Epwm.h"
#include "CaptureFrequence.h"
#include "ConRelay.h"
#include "GD485.h"
#include "AD7606.h"
//#include "user_timer.h"
/*SYSTEMVIEW相关*/
#ifdef USER_JLINK
#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW.h"
#endif

/*项目相关*/
#include "user_tcp_server.h"
#include "user_samp.h"
#include "user_alarm.h"
#include "user_ETHCommunication.h"
#include "user_programctrl.h"
#include "user_Cali.h"

#define AGING_REALITY TRUE
#define FeeTempKB     TRUE
#define CLRCon FALSE
#define CompensationIDKB TRUE
#endif
