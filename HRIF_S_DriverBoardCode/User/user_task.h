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
 * 完成日期：2023年4月8日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2023年4月8日
 */
#ifndef __user_task_H
#define __user_task_H

void TCPIP_Init(void);

extern TaskHandle_t AppTaskCreate_handle;
void AppTaskCreate(void *p);

extern uint8_t IP_ADDR3;
void Get_IP(void);

extern TaskHandle_t samp_task_handle;
void Samp_Task(void *p); // 采样线程
extern TaskHandle_t spare_task_handle;
void Spare_Task(void *p); // 备用线程，闪烁运行即可

//extern TimerHandle_t adjust_task_handle;
//void Adjust_Task(void *p); //调整线程，用于恒流恒功率脉冲等模式

extern TimerHandle_t reset_timer_handle;
void Reset_Callback(TimerHandle_t parameter);

#endif
