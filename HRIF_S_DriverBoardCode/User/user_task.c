/*
 * Copyright (c) 2024,杭可仪器第一研究所
 * All rights reserved.
 *
 * 文件名称：
 * 文件标识：见配置管理计划书
 * 摘 要：
 *
 * 当前版本：1.1
 * 作 者：
 * 完成日期：2024年4月25日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2024年4月25日
 */
#include "main.h"
uint8_t Set_time[6] = {1, 1, 1, 1, 1, 1};
uint8_t FSD_cnt;
TaskHandle_t AppTaskCreate_handle; // 任务句柄

/******************************************************************
函数名称：
函数功能:创建两个子任务并初始化相关外设和变量
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void AppTaskCreate(void *p)
{
	reset_timer_handle = xTimerCreate("Reset_Callback",
										3000,
										pdTRUE, // 重复
										(void *)0,
										Reset_Callback); // 复位定时器
	xTimerStart(reset_timer_handle, 0);

	memset(&SysCtrl, 0, sizeof(SysCtrl));
	memset(&SysProg, 0, sizeof(SysProg));
	memset(&SysData, 0, sizeof(SysData));

	All_GPIO_Simple_Init(); // 初始化所有管脚
	CPLD_Init();
	Fpga_Disable(0);
	user_delayMs(10);

	Get_IP(); // 获取IP
	VD_PRE_L;	
  VG_PRE_H;			
//	PRE_H;									
	ConCLR(SET);
 	ConRelayInit();			
//										
	SGM5349_Init(); // 初始化DA

//	Init_AD8688Gpio();
//	Init_AD8688(); // 初始化AD
	Init_AD7606GPIO(); // AD采样
	Init_AD7606();						
	SGM4588init(); // 初始化模拟开关
	Init_PID();    // 初始化温控PID
	PWM_GPIO_Init();
	/* HRIF-S  风机PWM设置0停止吹风，加热棒设置60000停止加热 */
	PWM_TIM2_Init(); // 风机控制
	PWM_TIM3_Init(); // 加热棒控制
	Rtc_Time_Init(&Set_time[0]); // RTC时间配置
//	USART_Init();                // 串口初始化
//	HRIF_S_Fpga_Init();
#if ENABLE_KB == TRUE
	ReadKBFromSRAM();
#endif

	enet_system_setup(); // 初始化以太网硬件部分
	TCPIP_Init();				 // LWIP初始化
	TCP_Server_Init();	 // 接收线程初始化
//	TCP_Listen_Init();
	
	//初始化状态不能超过3S
	xTaskCreate((TaskFunction_t)Samp_Task,
							(char *)"Samp_Task",
							(uint16_t)512,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)&samp_task_handle);

	xTaskCreate((TaskFunction_t)Spare_Task,
							(char *)"Spare_Task",
							(uint16_t)512,
							(void *)NULL,
							(UBaseType_t)4, // 优先级数值越大越优先
							(TaskHandle_t *)&spare_task_handle);

//	xTaskCreate((TaskFunction_t)Adjust_Task,
//							(char *)"Adjust_Task",
//							(uint16_t)512,
//							(void *)NULL,
//							(UBaseType_t)2, // 优先级数值越大越优先
//							(TaskHandle_t *)&adjust_task_handle);							

	// SEGGER_RTT_printf(0, "The system is start!!!\n"); // 系统启动成功

	xTimerDelete(reset_timer_handle, 0); // 删除复位定时器

	vTaskDelete(AppTaskCreate_handle); // 启动成功，删除监控定时器及AppTaskCreate任务 其他任务得以运行
	// SEGGER_RTT_printf(0, "create err\n");
	while (1)
		;
}

/******************************************************************
函数名称：
函数功能:创建采样和流程任务
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
TaskHandle_t samp_task_handle;
extern struct TIMER_REG  SysTime;
void Samp_Task(void *p) // 采样线程
{
/*************************
****当前为测试屏蔽****
//  HRIF_S_Fpga_USE();
//	BoardTest();
*************************/
	
	uint16_t Cnt, i;
	AgingStateConfig(AGING_STATUS_WAIT); // 初始化为等待状态，等待下发参数
#if AGING_REALITY == TRUE	
	while (TRUE)
	{
		
		if (SysCtrl.ProgType == NormalStation)
		{
			switch (SysData.AgingCommond)
			{
			case AGING_START:        // 启动
//				ProgCtrlRun();
			  ProgCtrlPowerStart();  // 先进入电源启动中（37）
			  if(	 SysProg.PowerUpCnt > 100 )
				{
					 AgingStateConfig(AGING_STATUS_ERROR_STOP);
				   vTaskDelay(2000);
					 SysData.AgingCommond = AGING_NULL;	
					 break;
				}		  
				break;
			case AGING_PAUSE: // 暂停
				ProgCtrlPause();
				SysData.AgingCommond = AGING_NULL;
				break;
			case AGING_RECOVER: // 继续
				ProgCtrlRecover();
				SysData.AgingCommond = AGING_NULL;
				break;
			case AGING_STOP: // 停止
				ProgCtrlSTOP();
				SysData.AgingCommond = AGING_NULL;
				break;
			case AGING_REALITY_START: //【老化实际开始】  收到19实际开始老化
				SysProg.PowerUpCnt = 0;
				ProgCtrlRun();
				if (SysProg.bflag.bit.PGST == 1)      //上位机给了老化状态【老化开始】会将PGST标志置1，下位机进入就绪状态
				{
					SysProg.bflag.bit.PGST = 0;		      //PGST标志置0	
					AgingStateConfig(AGING_STATUS_RUN); //老化运行
					SysData.AgingCommond = AGING_NULL;
					break;
				}
			  SysData.AgingCommond = AGING_NULL;    //老化命令清零			
        vTaskDelay(200);				
  			break ;
			default:
				break ;
			}

			// 不断检测老化状态并执行相关操作
			switch (SysData.AgingTestData.AgingStatus)
			{
			case AGING_STATUS_WAIT:   //【等待】

				break;
			case AGING_STATUS_READY:
				StationTempCtrl(TEMP_STANDBY_MODE);
//				if (SysProg.bflag.bit.PGST == 1)
//				{
//					SysProg.bflag.bit.PGST = 0;					
//					AgingStateConfig(AGING_STATUS_RUN);
//				}
				break;
			case AGING_STATUS_RUN:
						if(SysProg.ERRORFLAG== 0xfB){
						 AgingStateConfig(AGING_STATUS_ERROR_STOP);
						}
						Get_Time_Data(&SysTime.Second);
						RunTime();			
						TimeCopy(0);
						SelectAgingModeSamp();
						if (SysData.AgingTestData.ProRunTimeCnt.data >= SysData.AgingTestParameters.AgingTestTime.data)
						{
							AgingStateConfig(AGING_STATUS_AGING_FINISH); // 老化时间到,老化完成
						}
						StationTempCtrl(TEMP_HOT_MODE);
						
						AbnormalHandle();//保护异常处理
//				FSDSortControl(2);
				break;
			case AGING_STATUS_PAUSE:
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}				
				break;
			case AGING_STATUS_AGING_FINISH:
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}	
//        vTaskDelay(2000);				
//        VD_PRE_L;				
				break;
				
			case AGING_STATUS_ERROR_STOP:				
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}
//        vTaskDelay(2000);				
//        VD_PRE_L;							
				break;
			case AGING_STATUS_MANUAL_STOP:				
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}	
//        vTaskDelay(2000);				
//        VD_PRE_L;							
				break;
			default:
				break;
			}
		}
		vTaskDelay(200);
	}	
#else	
	while (TRUE)
	{

		
	
		if (SysCtrl.ProgType == NormalStation)
		{
			switch (SysData.AgingCommond)
			{
				
			case AGING_START: // 启动
				ProgCtrlRun();
				SysData.AgingCommond = AGING_NULL;			  
				break;
			case AGING_PAUSE: // 暂停
				ProgCtrlPause();
				SysData.AgingCommond = AGING_NULL;
				break;
			case AGING_RECOVER: // 继续
				ProgCtrlRecover();
				SysData.AgingCommond = AGING_NULL;
				break;
			case AGING_STOP: // 停止
				ProgCtrlSTOP();
				SysData.AgingCommond = AGING_NULL;
				break;
			default:
				break;
			}

			// 不断检测老化状态并执行相关操作
			switch (SysData.AgingTestData.AgingStatus)
			{
			case AGING_STATUS_WAIT:   //【等待】

				break;
			case AGING_STATUS_READY:
				StationTempCtrl(TEMP_STANDBY_MODE);
				if (SysProg.bflag.bit.PGST == 1)
				{
					SysProg.bflag.bit.PGST = 0;					
					AgingStateConfig(AGING_STATUS_RUN);
				}
				break;
			case AGING_STATUS_RUN:
				if(SysProg.ERRORFLAG== 0xfB){
				 AgingStateConfig(AGING_STATUS_ERROR_STOP);
				}
				Get_Time_Data(&SysTime.Second);
				RunTime();			
				TimeCopy(0);
			  SelectAgingModeSamp();
				if (SysData.AgingTestData.ProRunTimeCnt.data >= SysData.AgingTestParameters.AgingTestTime.data)
				{
					AgingStateConfig(AGING_STATUS_AGING_FINISH); // 老化时间到,老化完成
				}
				StationTempCtrl(TEMP_HOT_MODE);
				
				AbnormalHandle();//保护异常处理
				FSDSortControl(2);
				break;
			case AGING_STATUS_PAUSE:
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}				
				break;
			case AGING_STATUS_AGING_FINISH:
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}						
				break;
				
			case AGING_STATUS_ERROR_STOP:				
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}					
				break;
			case AGING_STATUS_MANUAL_STOP:				
				SampTemp();
		  	Cnt = 0;
				for (i = 0; i < 2; i++)
				{		
				 if(SysData.AgingTestData.Station_TC_Temp[i].data>= 500)
				 {
					 Cnt++;
				 }
				}	
        if(Cnt>0){
			  	StationTempCtrl(TEMP_COOL_MODE);
				}	
        else{
			  	StationTempCtrl(TEMP_STANDBY_MODE);
				}					
				break;
			default:
				break;
			}
		}
		vTaskDelay(200);
	}
#endif	
}
/******************************************************************
函数名称：
函数功能:创建用于校准与计量采样任务
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
extern TaskHandle_t TCP_Server_handle;
TaskHandle_t spare_task_handle;
void Spare_Task(void *p) // 备用线程,可以用于测试等，闪烁运行即可
{
	while (TRUE)
	{
		
		if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == Calibration))
		{
			SampleALLChanData();
			CaliALLData();
		}		
		
    LED2_TOGGLE;
	  ReConnectTask();    //网线断连重连
//		RS485Test();

		vTaskDelay(50);
	}
}
/******************************************************************
函数名称：
函数功能:创建用于以太网上电连不上的网络复位任务
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
TimerHandle_t reset_timer_handle;
extern struct netif gnetif;
extern char ethernetif_init(struct netif *netif);
void Reset_Callback(TimerHandle_t parameter)
{
	//	SEGGER_RTT_printf(0,"The tcpip init fault,try to restart . . .\n");
	NVIC_SystemReset();
	//enet_system_setup();
	//ethernetif_init(&gnetif);
}

/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
//TimerHandle_t adjust_task_handle;
//void Adjust_Task(void *p)
//{
//	
//	while (TRUE)
//	{		
//    
//	  vTaskDelay(100);
//	}
//	
//}

