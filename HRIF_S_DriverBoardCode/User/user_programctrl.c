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
#include "main.h"

/******************************************************************
函数名称：
函数功能:清除老化结构体参数
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
extern uint8_t FSD_cnt;
void ClearAgingVarParam()
{
	uint16_t i, j;
  FSD_cnt=0;
	SysCtrl.StartMode = 10;
	SysCtrl.PlusOrMinusFlag[0]=0;
	SysCtrl.PlusOrMinusFlag[1]=0;
	SysCtrl.ADjustVolCnt =0;
//	SysCtrl.ADjustVolSignFlag[0] =0;
//	SysCtrl.ADjustVolSignFlag[1] =0;	
	vTaskDelay(200);
	/* 清除系统报警信息 */
	for(i=0 ; i<2 ; i++){
		SysProg.Voltage_VD_AlarmCntUp[i] = 0;// VD1 VD2电压报警信息
		SysProg.Voltage_VG_AlarmCntUp[i] = 0;// VG1 VG2电压报警信息
		
		SysProg.Curr_ID_AlarmCntUp[i] = 0;   // ID1 ID2电压报警信息
		SysProg.Curr_IG_AlarmCntUp[i] = 0;	 // IG1 IG2电压报警信息
		
		SysProg.TempAlarmCntUp[i] = 0;       // TC1 TC2温度报警信息
	}
	  SysProg.ERRORFLAG =0;    
	  SysProg.PowerUpCnt =0;   	
	/* 清除回传上位机的老化数据 */
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Station_VD_Voltage[i].data = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Voltage_VD_AlarmInformation[i] = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Station_VG_Voltage[i].data = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Voltage_VG_AlarmInformation[i] = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Station_ID_Curr[i].data = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Curr_ID_AlarmInformation[i] = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Station_IG_Curr[i].data = 0;
	}	
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Curr_IG_AlarmInformation[i] = 0;
	}
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Station_TC_Temp[i].data = 0;
	}		
	for (i = 0; i < 2; i++)
	{
		SysData.AgingTestData.Temp_TC_AlarmInformation[i] = 0;
	}		
	
}

/******************************************************************
函数名称：
函数功能:清除老化结构体参数（出错停止用）
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void ClearAgingVarParamError(void)
{
	uint16_t i, j;
  FSD_cnt=0;
	SysCtrl.StartMode = 10;
	SysCtrl.PlusOrMinusFlag[0]=0;
	SysCtrl.PlusOrMinusFlag[1]=0;
	SysCtrl.ADjustVolCnt =0;
	vTaskDelay(200);
	/* 清除系统报警信息 */
	for(i=0 ; i<2 ; i++){
		if(SysProg.Voltage_VD_AlarmCntUp[i] == 0){
		 SysData.AgingTestData.Station_VD_Voltage[i].data = 0;
		}
		if(SysProg.Voltage_VG_AlarmCntUp[i] == 0){
	   SysData.AgingTestData.Station_VG_Voltage[i].data = 0;
		}
		if(SysProg.Curr_ID_AlarmCntUp[i] == 0){
		 SysData.AgingTestData.Station_ID_Curr[i].data = 0;
		}
		if(SysProg.Curr_IG_AlarmCntUp[i] == 0){
	   SysData.AgingTestData.Station_IG_Curr[i].data = 0;
		}
		if(SysProg.TempAlarmCntUp[i] == 0){
	   SysData.AgingTestData.Station_TC_Temp[i].data = 0;
		}		

	}

}
