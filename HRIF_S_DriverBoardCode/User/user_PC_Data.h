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
#ifndef __user_PC_Data_H
#define __user_PC_Data_H
#include "main.h"

#define ConstantCurrentMode 0
#define ConstantPowerMode   1
#define PulseMode           2
#define ConstantVoltageMode 3

typedef enum
{
  DATA_NOVAL = 0x0000,
  DATA_VAL = 0x55AA
} DATA_state_t;

typedef enum
{
    AGING_STATUS_WAIT                       = 0,        // �ȴ�
    AGING_STATUS_READY                      = 1,        // ����
    AGING_STATUS_RUN                        = 2,        // �ϻ���
    AGING_STATUS_PAUSE                      = 3,        // ��ͣ
    AGING_STATUS_TESTING                    = 4,        // ���ڼ��,����У׼��
    AGING_STATUS_TEST_FINISH                = 5,        // ������,����У׼���
    AGING_STATUS_AGING_FINISH               = 6,        // ����ϻ�
    AGING_STATUS_ERROR_STOP                 = 7,        // ����ֹͣ
    AGING_STATUS_ZERO_DRIFT_COLLECT_FINISH  = 8,        // ��Ư�ɼ����
    AGING_STATUS_ZERO_DRIFT_COLLECTING      = 9,        // ��Ư�ɼ���
    AGING_STATUS_STATION_DETECTIONING       = 10,       // ��λ�����
    AGING_STATUS_MANUAL_STOP                = 11,       // �ϻ�ֹͣ
    AGING_STATUS_CALIBRATIONING             = 12,       // У׼��
    AGING_STATUS_CONSTANT_TC_MODE_WARMING   = 13,       // �㶨TRģʽ������
    AGING_STATUS_ABNORMAL_POWER_START       = 15,       // ��Դ�����쳣����λ�����ж���10V�⣩
    WAITING_TEMPERATURE_RISE                = 34,       // �ȴ��¶ȵ����趨�¶�(Ŀǰ���ڽ���У׼)
    STABLE_TEMPERATURE_ING                  = 35,       // (�¶ȵ����趨�¶�)�¶��ȶ���(Ŀǰ���ڽ���У׼)
    WAITING_POWER_RISE                      = 37	
}AgingStatusCOM;

typedef enum
{
	  AGING_NULL        = 0,      // �ϻ���ʼ
    AGING_START       = 1,      // �ϻ���ʼ
    AGING_PAUSE       = 2,      // �ϻ���ͣ
    AGING_RECOVER     = 3,      // �ϻ��ָ�
    AGING_STOP        = 4,      // �ϻ�ֹͣ	
	  AGING_REALITY_START  = 0x13,   // ʵ���ϻ���ʼ		
}SetAgingMode;


typedef enum
{
  NormalStation = 2, //����ģʽ
  Measurement = 10,   //����ģʽ  
  Calibration = 11,   //У׼ģʽ 
            
} ProgType;

typedef enum
{
    TEMP_STANDBY_MODE =2,      //����ģʽ
    TEMP_HOT_MODE = 1,         //����ģʽ
    TEMP_COOL_MODE = 0,        //����ģʽ
}TempRunMode;

typedef enum
{
  aging_alarm_nomal = 0,         //�ϻ���������ģʽ      
  aging_alarm_volup = 72,        //�ϻ�������ѹ����  �������ޣ�    
  aging_alarm_voldown = 73,      //�ϻ�������ѹ�����趨ֵ  �������ޣ�    
  aging_alarm_curup = 17,        //�ϻ�������������  
	aging_alarm_curdown = 18,      //�ϻ��������������趨ֵ
  aging_alarm_Tempup = 24, 	     //�ϻ������¶ȳ����趨ֵ
} aging_alarm_t;	


struct AGING_TEST_PARAMETERS                //��ǰ�ṹ���������������λ�����͵�����
{
  union DATA_BYTE AgingTestTime;            // �ϻ�ʱ��
	union DATA_BYTE AgingMode;                // ģʽѡ��  0 ����ģʽ�� 1 �㹦��ģʽ �� 2 ����ģʽ ��3 ��ѹģʽ
	union DATA_BYTE SetCurr;                  // ����������ģʽ��
	union DATA_BYTE SetPower;                 // ���ʣ��㹦��ģʽ��	
	union DATA_BYTE PulseWidth[2];            // ������ us                ������ģʽ��
  union DATA_BYTE DutyCycle[2];             // ռ�ձ�	  ��λ�����Ŵ�10��  ������ģʽ��
  union DATA_BYTE SetVOL;                   // 0 ����ѹģʽ��	
	union DATA_BYTE FirstSetVTime;            // 0�ȷ�VD 1�ȷ�VG
	union DATA_BYTE FirstStopVTime;           // 0��ͣVD 1��ͣVG  ֹͣʱ���� VG��ֹͣ����VG1 VG2ͬʱֹͣ	
	
	union DATA_BYTE Set_VD_Vol;  	            // VD��ѹ     ��λV��V�Ŵ�100��
	union DATA_BYTE Set_VD_VolUp;  	          // VD��ѹ����	��λV��V�Ŵ�100��
	union DATA_BYTE Set_VG_Vol;  	            // VG�����ѹ	��λV��V�Ŵ�100��
	union DATA_BYTE Set_VG_VolUp;  	          // VG��ѹ����	��λV��V�Ŵ�100��	
	union DATA_BYTE Set_ID_CurrUp;  	        // ID��������	��λmA��mA�Ŵ�100��
	union DATA_BYTE Set_IG_CurrUp;  	        // IG��������	��λmA��mA�Ŵ�100��	
	
	union DATA_BYTE SetTemp;  	              // �����¶�	��λ�棬�Ŵ�10��
	union DATA_BYTE SetTempUp;  	            // �����¶�����	��λ�棬�Ŵ�10��		
	
	union DATA_BYTE SetContrlFre[3];          // Ƶ��
  union DATA_BYTE SetDutyCycle[3];          // ռ�ձ�
	uint8_t  SetVTime[MAX_VOLSTATE];          // �ϵ�˳��
	
union DATA_BYTE SetFanGrade;
};

struct AGING_TEST_DATA                            //��ǰ�ṹ������������ش�����λ��������
{   
    uint16_t DataStatus;           //�ϻ����ݱ�־״̬λ,0x0000:��Ч����; 0x55AA:��Ч����
    struct TIMER_REG SampTime;     //ʵʱʱ�� �� �� ʱ �� �� ��
	  union DATA_BYTE ProRunTimeCnt; //�ϻ�ʱ��
  	AgingStatusCOM AgingStatus;    //�ϻ�״̬
  	uint16_t   BoardState;         //���״̬
	
    union DATA_BYTE Station_VD_Voltage[2];       //2·VD��ѹ��V�Ŵ�1000����VD1 VD2
    uint16_t Voltage_VD_AlarmInformation[2];     //VD1 VD2��ѹ������Ϣ       	  

    union DATA_BYTE Station_VG_Voltage[2];       //2·VG��ѹ��V�Ŵ�1000����VG1 VG2
    uint16_t Voltage_VG_AlarmInformation[2];     //VG1 VG2��ѹ������Ϣ 

  	union DATA_BYTE Station_ID_Curr[2];          //2·ID������A�Ŵ�1000����ID1 ID2
    uint16_t Curr_ID_AlarmInformation[2];        //ID1 ID2����������Ϣ

  	union DATA_BYTE Station_IG_Curr[2];          //2·IG������mA�Ŵ�100����IG1 IG2
    uint16_t Curr_IG_AlarmInformation[2];	       //IG1 IG2����������Ϣ
	
	  union DATA_BYTE Station_TC_Temp[2];          //2·���Ȱ��¶ȣ�TC1 TC2������Ŵ�10��
    uint16_t Temp_TC_AlarmInformation[2];  	     //TC1 TC2�¶ȱ�����Ϣ
		

	
//	  union DATA_BYTE StationTemp[DEVICE_MAX];                   //��λ��ѹ��mV�Ŵ�100��
//    uint16_t TempAlarmInformation[DEVICE_MAX];                 //��ѹ������Ϣ
//	
  	union DATA_BYTE Rev_Word1[2];                     //Ԥ���ֽڶ�1
    uint16_t Rev_Word2[2];                            //Ԥ���ֽڶ�2
};  


struct SYS_DATA
{
    struct AGING_TEST_PARAMETERS    AgingTestParameters;//��ǰ�ṹ���������������λ�����͵�����
    struct AGING_TEST_DATA          AgingTestData;      //��ǰ�ṹ������������ش�����λ��������
    SetAgingMode AgingCommond;            // �ϻ�����,1�����ϻ�,2��ͣ�ϻ�,3�����ϻ�,4ֹͣ�ϻ�
	  uint16_t AgingFinishAutoEndFlag;      // �ϻ����Ҫ��Ҫ�Զ�ֹͣ�ϻ�,0��ͣ��1ֹͣ
    uint16_t AlarmingAutoEndFlag;         // �ϻ�����Ҫ��Ҫ�Զ�ֹͣ�ϻ�,0��ͣ��1ֹͣ
    uint16_t AgingFollowStatus;           // �ϻ�����״̬,�ò���  Ĭ�� 0
    union DATA_BYTE ResumeAgingCycleCnt;  // �����ϻ�ѭ������
    uint16_t AgingFinishBeepONFlag;       // �ϻ�����Ƿ���������,0���� 1��
    uint16_t AlarmingBeepONFlag;          // �ϻ�����������Ҫ��Ҫ��,0����
		union DATA_BYTE DeviceVoltageDrop;    // ����ѹ��
};
extern  struct SYS_DATA  SysData; 


extern void ProgCtrlRun(void);
extern void ProgCtrlPause(void);
extern void ProgCtrlRecover(void);
extern void ProgCtrlSTOP(void);
extern void ProgCtrlJTempCaliStart(void);
extern void ProgCtrlJTempCaliManualStop(void);
extern void AgingStateConfig(AgingStatusCOM state); 
int InitiatingVol(void);
void InitiatingVDVol();
void InitiatingVGVol();
extern void StopVol(void);
extern void InitiatingVDVol_Test();
extern void ProgCtrlPowerStart(void);
int InitiatingVol_Test(void);

#endif


