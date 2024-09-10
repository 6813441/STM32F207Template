/*
 * Copyright (c) 2023,����������һ�о���
 * All rights reserved.
 *
 * �ļ����ƣ�
 * �ļ���ʶ�������ù���ƻ���
 * ժ Ҫ��
 *
 * ��ǰ�汾��1.1
 * �� �ߣ�
 * ������ڣ�2023��4��8��
 *
 * ȡ���汾��1.0
 * ԭ���� ��
 * ������ڣ�2023��4��8��
 */
#include "main.h"

/******************************************************************
�������ƣ�
��������:����ϻ��ṹ�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
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
	/* ���ϵͳ������Ϣ */
	for(i=0 ; i<2 ; i++){
		SysProg.Voltage_VD_AlarmCntUp[i] = 0;// VD1 VD2��ѹ������Ϣ
		SysProg.Voltage_VG_AlarmCntUp[i] = 0;// VG1 VG2��ѹ������Ϣ
		
		SysProg.Curr_ID_AlarmCntUp[i] = 0;   // ID1 ID2��ѹ������Ϣ
		SysProg.Curr_IG_AlarmCntUp[i] = 0;	 // IG1 IG2��ѹ������Ϣ
		
		SysProg.TempAlarmCntUp[i] = 0;       // TC1 TC2�¶ȱ�����Ϣ
	}
	  SysProg.ERRORFLAG =0;    
	  SysProg.PowerUpCnt =0;   	
	/* ����ش���λ�����ϻ����� */
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
�������ƣ�
��������:����ϻ��ṹ�����������ֹͣ�ã�
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
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
	/* ���ϵͳ������Ϣ */
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
