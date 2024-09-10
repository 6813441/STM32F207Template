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
    AGING_STATUS_WAIT                       = 0,        // 等待
    AGING_STATUS_READY                      = 1,        // 就绪
    AGING_STATUS_RUN                        = 2,        // 老化中
    AGING_STATUS_PAUSE                      = 3,        // 暂停
    AGING_STATUS_TESTING                    = 4,        // 正在检测,结温校准中
    AGING_STATUS_TEST_FINISH                = 5,        // 检测完成,结温校准完成
    AGING_STATUS_AGING_FINISH               = 6,        // 完成老化
    AGING_STATUS_ERROR_STOP                 = 7,        // 出错停止
    AGING_STATUS_ZERO_DRIFT_COLLECT_FINISH  = 8,        // 零漂采集完成
    AGING_STATUS_ZERO_DRIFT_COLLECTING      = 9,        // 零漂采集中
    AGING_STATUS_STATION_DETECTIONING       = 10,       // 工位检测中
    AGING_STATUS_MANUAL_STOP                = 11,       // 老化停止
    AGING_STATUS_CALIBRATIONING             = 12,       // 校准中
    AGING_STATUS_CONSTANT_TC_MODE_WARMING   = 13,       // 恒定TR模式升温中
    AGING_STATUS_ABNORMAL_POWER_START       = 15,       // 电源启动异常（下位机自行定±10V外）
    WAITING_TEMPERATURE_RISE                = 34,       // 等待温度到达设定温度(目前用于结温校准)
    STABLE_TEMPERATURE_ING                  = 35,       // (温度到达设定温度)温度稳定中(目前用于结温校准)
    WAITING_POWER_RISE                      = 37	
}AgingStatusCOM;

typedef enum
{
	  AGING_NULL        = 0,      // 老化开始
    AGING_START       = 1,      // 老化开始
    AGING_PAUSE       = 2,      // 老化暂停
    AGING_RECOVER     = 3,      // 老化恢复
    AGING_STOP        = 4,      // 老化停止	
	  AGING_REALITY_START  = 0x13,   // 实际老化开始		
}SetAgingMode;


typedef enum
{
  NormalStation = 2, //正常模式
  Measurement = 10,   //计量模式  
  Calibration = 11,   //校准模式 
            
} ProgType;

typedef enum
{
    TEMP_STANDBY_MODE =2,      //待机模式
    TEMP_HOT_MODE = 1,         //加温模式
    TEMP_COOL_MODE = 0,        //降温模式
}TempRunMode;

typedef enum
{
  aging_alarm_nomal = 0,         //老化报警正常模式      
  aging_alarm_volup = 72,        //老化报警电压超限  （超上限）    
  aging_alarm_voldown = 73,      //老化报警电压低于设定值  （超下限）    
  aging_alarm_curup = 17,        //老化报警电流超限  
	aging_alarm_curdown = 18,      //老化报警电流低于设定值
  aging_alarm_Tempup = 24, 	     //老化报警温度超过设定值
} aging_alarm_t;	


struct AGING_TEST_PARAMETERS                //当前结构体参数用来接收上位机发送的数据
{
  union DATA_BYTE AgingTestTime;            // 老化时间
	union DATA_BYTE AgingMode;                // 模式选择  0 恒流模式、 1 恒功率模式 、 2 脉冲模式 、3 恒压模式
	union DATA_BYTE SetCurr;                  // 电流（恒流模式）
	union DATA_BYTE SetPower;                 // 功率（恒功率模式）	
	union DATA_BYTE PulseWidth[2];            // 脉冲宽度 us                （脉冲模式）
  union DATA_BYTE DutyCycle[2];             // 占空比	  单位％，放大10倍  （脉冲模式）
  union DATA_BYTE SetVOL;                   // 0 （恒压模式）	
	union DATA_BYTE FirstSetVTime;            // 0先发VD 1先发VG
	union DATA_BYTE FirstStopVTime;           // 0先停VD 1先停VG  停止时例如 VG先停止，即VG1 VG2同时停止	
	
	union DATA_BYTE Set_VD_Vol;  	            // VD电压     单位V，V放大100倍
	union DATA_BYTE Set_VD_VolUp;  	          // VD电压上限	单位V，V放大100倍
	union DATA_BYTE Set_VG_Vol;  	            // VG起调电压	单位V，V放大100倍
	union DATA_BYTE Set_VG_VolUp;  	          // VG电压上限	单位V，V放大100倍	
	union DATA_BYTE Set_ID_CurrUp;  	        // ID电流上限	单位mA，mA放大100倍
	union DATA_BYTE Set_IG_CurrUp;  	        // IG电流上限	单位mA，mA放大100倍	
	
	union DATA_BYTE SetTemp;  	              // 设置温度	单位℃，放大10倍
	union DATA_BYTE SetTempUp;  	            // 设置温度上限	单位℃，放大10倍		
	
	union DATA_BYTE SetContrlFre[3];          // 频率
  union DATA_BYTE SetDutyCycle[3];          // 占空比
	uint8_t  SetVTime[MAX_VOLSTATE];          // 上电顺序
	
union DATA_BYTE SetFanGrade;
};

struct AGING_TEST_DATA                            //当前结构体参数是用来回传给上位机的数据
{   
    uint16_t DataStatus;           //老化数据标志状态位,0x0000:无效数据; 0x55AA:有效数据
    struct TIMER_REG SampTime;     //实时时间 秒 分 时 日 月 年
	  union DATA_BYTE ProRunTimeCnt; //老化时间
  	AgingStatusCOM AgingStatus;    //老化状态
  	uint16_t   BoardState;         //插板状态
	
    union DATA_BYTE Station_VD_Voltage[2];       //2路VD电压（V放大1000倍）VD1 VD2
    uint16_t Voltage_VD_AlarmInformation[2];     //VD1 VD2电压报警信息       	  

    union DATA_BYTE Station_VG_Voltage[2];       //2路VG电压（V放大1000倍）VG1 VG2
    uint16_t Voltage_VG_AlarmInformation[2];     //VG1 VG2电压报警信息 

  	union DATA_BYTE Station_ID_Curr[2];          //2路ID电流（A放大1000倍）ID1 ID2
    uint16_t Curr_ID_AlarmInformation[2];        //ID1 ID2电流报警信息

  	union DATA_BYTE Station_IG_Curr[2];          //2路IG电流（mA放大100倍）IG1 IG2
    uint16_t Curr_IG_AlarmInformation[2];	       //IG1 IG2电流报警信息
	
	  union DATA_BYTE Station_TC_Temp[2];          //2路加热板温度（TC1 TC2）（℃放大10）
    uint16_t Temp_TC_AlarmInformation[2];  	     //TC1 TC2温度报警信息
		

	
//	  union DATA_BYTE StationTemp[DEVICE_MAX];                   //工位电压（mV放大100）
//    uint16_t TempAlarmInformation[DEVICE_MAX];                 //电压报警信息
//	
  	union DATA_BYTE Rev_Word1[2];                     //预留字节段1
    uint16_t Rev_Word2[2];                            //预留字节段2
};  


struct SYS_DATA
{
    struct AGING_TEST_PARAMETERS    AgingTestParameters;//当前结构体参数用来接受上位机发送的数据
    struct AGING_TEST_DATA          AgingTestData;      //当前结构体参数是用来回传给上位机的数据
    SetAgingMode AgingCommond;            // 老化命令,1启动老化,2暂停老化,3继续老化,4停止老化
	  uint16_t AgingFinishAutoEndFlag;      // 老化完成要不要自动停止老化,0不停，1停止
    uint16_t AlarmingAutoEndFlag;         // 老化报警要不要自动停止老化,0不停，1停止
    uint16_t AgingFollowStatus;           // 老化后续状态,用不到  默认 0
    union DATA_BYTE ResumeAgingCycleCnt;  // 继续老化循环次数
    uint16_t AgingFinishBeepONFlag;       // 老化完成是否开启蜂鸣器,0不叫 1叫
    uint16_t AlarmingBeepONFlag;          // 老化报警蜂鸣器要不要叫,0不叫
		union DATA_BYTE DeviceVoltageDrop;    // 器件压降
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


