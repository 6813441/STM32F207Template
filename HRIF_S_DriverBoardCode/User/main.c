/*
 * Copyright (c) 2024,杭可仪器第一研究所
 * All rights reserved.
 *
 * 文件名称：
 * 文件标识：见配置管理计划书
 * 摘 要：
 *
 * 当前版本：1.0
 * 作 者：
 * 完成日期：2024年3月25日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2024年3月25日
 */

#include "main.h"
__align(16) uint16_t SendData[16650] __attribute__((section("CCMRAM_SendData")));//打包存的地方
uint16_t KbData[200];

/******************************************************************
函数名称：
函数功能:最初的程序入口
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/ 
int main(void){	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);   //4位表示抢占优先级，0位表示次优先级
    
	#ifdef USER_JLINK		
	SEGGER_RTT_Init();
	DWT->CTRL=(1<<0);
	//SEGGER_SYSVIEW_Conf();
	//SEGGER_SYSVIEW_Start();//初始化rtt和开启systemview
	#endif 
	
	user_delayMs(1000);

	BaseType_t xReturn = pdPASS; // Define a creation information return value, the default is pdPASS
	xReturn=xTaskCreate((TaskFunction_t)AppTaskCreate,
							(char *)"AppTaskCreate",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)16,//删除前阻止其创建的任务执行
							(TaskHandle_t *)&AppTaskCreate_handle);
	// Start task scheduling
    if (pdPASS == xReturn)
        vTaskStartScheduler();
    else
        return -1;

    while (1); // Normally will not execute here
}
