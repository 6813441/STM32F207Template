#ifndef __user_SysType_H
#define __user_SysType_H

#define MAX_TEMP_CH 4 // 最大温度通道
#define DEVICE_MAX 4  // 最大工位数

#define MAX_VOLSTATE 6 // 最大电压类型数

#define MOVAVER_MAXNUM 20      // 滑动平均滤波算法的滤波数为20个
#define TEMP_MOVAVER_MAXNUM 10 // 温度滑动平均滤波算法的滤波数为20个
#define MAX_REC_CMDSIZE 10     // 上位机接收数据命令长度，固定10个字，20个字节

#define MUL_VOLGRADE 10.0
#define MUL_CURRGRADE 100.0

#define ALLKB_WORD 1024

#define FALSE 0
#define TRUE 1
#define ENABLE_KB TRUE

#define SAMPLECNT 10
#define PASTE(x, y) x##y
#define RCU_GPIOx(x) PASTE(RCU_GPIO, x)
#define GPIOx(x) PASTE(GPIO, x)
#define GPIO_PIN_x(x) PASTE(GPIO_PIN_, x)




/******************************************************************************************************************************
 *
 *******************************************************************************************************************************/
union DATA_BYTE
{
    uint32_t data;
    struct
    {
        uint16_t lbyte;
        uint16_t hbyte;
    } bytes;
};


typedef enum
{
    CALIVG1 = 1,
    CALIVG2 = 2,
    CALIVD1 = 3,
    CALIVD2 = 4,
    CALIIG1,
    CALIIG2,
    CALIID1,
    CALIID2,
} CALITYPE_MODE;

struct KB_VALUE
{

    union DATA_BYTE KVal;
    union DATA_BYTE BVal;
};
/******************************************************************************************************************************
 *
 *******************************************************************************************************************************/
struct TIMER_REG
{
    uint8_t Second;
    uint8_t Minute;
    uint8_t Hour;
    uint8_t Day;
    uint8_t Momth;
    uint8_t Year;
	  uint8_t  LastSecond;
	  uint8_t  LastMinute;
};
/******************************************************************************************************************************
 *
 *******************************************************************************************************************************/
struct PID_REG
{
    float SetValue;
    float ActualValue;
    float errdata;
    float diffdata;
    float integral;
    float last_errdata;
    float llast_errdata;
    float KiIndex;
    float KpIndex, KdIndex;
    float Outvalue;
};

struct ETH_TYPE_REG
{
    union DATA_BYTE ETH_ReceiveLength;
    union DATA_BYTE ETH_Length;

    union DATA_BYTE Ssum_L;
    union DATA_BYTE Ssum_H;

    uint16_t Rec_Comand[MAX_REC_CMDSIZE];
    uint16_t Data[600];
};

struct AGEFLAG_BITS
{
    uint16_t PGEST : 1;  // 流程文件存在 program exist
    uint16_t PGST  : 1;  // program(flow) start    ;  1  start     ;   0  none
    uint16_t PGRN  : 1;  // program(flow) running  ;  1  run       ;   0  stop
    uint16_t rsvd  : 13;
};

union AGEFLAG_REG
{
    uint16_t all;
    struct AGEFLAG_BITS bit;
};

struct SYS_CTRL
{
    struct ETH_TYPE_REG Ethernet;
    struct PID_REG PIDControl[MAX_TEMP_CH];
    long FSD[MAX_TEMP_CH]; // 风机速度控制
    long HOT[MAX_TEMP_CH]; // 加热棒控制

    union DATA_BYTE V6SetData;
    union DATA_BYTE BoardTempChan[MAX_TEMP_CH];
    uint16_t ProgType;

    uint16_t EthConnectFlag;
    uint16_t EthConnectCnt;
	
//	  uint16_t VDVolFlag[2];   //VD1 VD2 发起标志
//	  uint16_t VGVolFlag[2];   //VG1 VG2 发起标志
		uint16_t StartMode;      //开始模式标志        当前模式初始化为10
	  uint8_t PlusOrMinusFlag[2]; // 10 为正 20 为负
	  uint8_t ADjustVolCnt;
	  int ADjustVol[2];
	  uint8_t PulseModeFlag;
//	  uint8_t ADjustVolSignFlag[2];
	
};
extern struct SYS_CTRL SysCtrl;

struct SYS_PROG
{
    union AGEFLAG_REG bflag;
    uint16_t SampBitValue[10];//存放采样码值
	  float  SampValue[10];
	  float  VOL_VALUE[4];
	  float  CURR_VALUE[4];
	  float  TEMP_VALUE[2];
	  float  TRANS_VOL_VALUE[4];
	  float  TRANS_CURR_VALUE[4];
	  float  TRANS_TEMP_VALUE[2];	
	  uint32_t TransVGValueFlag[2];
	  uint32_t TransIGValueFlag[2];
    uint16_t Ctrl_AGE_RUN[DEVICE_MAX];

    uint16_t CtrlAGE_RUN[2];                   //控制老化运行
    uint16_t Voltage_VD_AlarmCntUp[2];         // VD1 VD2电压报警信息
    uint16_t Voltage_VG_AlarmCntUp[2];         // VG1 VG2电压报警信息
    uint16_t Curr_ID_AlarmCntUp[2];            // ID1 ID2电压报警信息
    uint16_t Curr_IG_AlarmCntUp[2];            // IG1 IG2电压报警信息
		uint16_t TempAlarmCntUp[2];                // TC1 TC2温度报警信息
	  uint16_t ERRORFLAG;                        // 开启电压后出错停止标志位
		uint16_t PowerUpCnt;                       // 等待老化一级电源启动20秒启动不成功则进入出错停止
		uint16_t StopPowerCnt[2];
	  

};
extern struct SYS_PROG SysProg;

typedef enum
{
    MAX_GRADE = 10,      
    MAX_TEMPUKB_GRADE = 1,
} MAX_SYS_GRADE;
typedef enum
{
//	ConVG1 =1,
//	ConVG2 =2,
//	ConVD1 =3,
//	ConVD2 =4,
//	FeeVG1 =5,FeeVG2=6,FeeVD1=7,FeeVD2=8,FeeIG1=9,FeeIG2=10,FeeID1=11,FeeID2=12,
 	ConVG=1,ConVD=2,FeeVG=6,FeeVD=7,FeeIG_Z=8,FeeIG_F=9,FeeID=10,FeeTemp=19
} CAL_KB_TYPE;

struct CAL_REGS
{
	
	  struct KB_VALUE ConVG1[10];
	  struct KB_VALUE ConVG2[10];
	  struct KB_VALUE ConVD1[10];
	  struct KB_VALUE ConVD2[10];
	
	  struct KB_VALUE FeeVG1[10];
	  struct KB_VALUE FeeVG2[10];
	  struct KB_VALUE FeeVD1[10];
	  struct KB_VALUE FeeVD2[10];
	
	  struct KB_VALUE FeeIG1[5];
	  struct KB_VALUE FeeIG10[5];	
	  struct KB_VALUE FeeIG2[5];
	  struct KB_VALUE FeeIG20[5];	
	
	  struct KB_VALUE FeeID1[10];
	  struct KB_VALUE FeeID2[10];		
	
    struct KB_VALUE FeeTemp[18];   //温度回检+预留KB
	  /* 工位1--4组KB，50°一组 
		** 工位2--4组KB, 50°一组
		** 后10组预留KB ,留给硬件调试
		*/	
};
extern struct CAL_REGS Calib[1];
#endif
