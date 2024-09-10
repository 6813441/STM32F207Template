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
#ifndef _user_Cali_H_
#define _user_Cali_H_


/*
中断向量表100个，向上圆整128 4字节所以占用512 所以起始地址为512的整数倍

GD32F4
块0扇区定义
扇区0				0x0800 0000 - 0x0800 3FFF			16KB			//启动程序
扇区1				0x0800 4000 - 0x0800 7FFF			16KB			//启动程序
扇区2				0x0800 8000 - 0x0800 BFFF			16KB			//启动程序

扇区3				0x0800 C000 - 0x0800 FFFF			16KB 			//长度（直接使用标志）

扇区4				0x0801 0000 - 0x0801 FFFF			64KB			//APP
扇区5				0x0802 0000 - 0x0803 FFFF			128KB			//APP							192K
扇区6				0x0804 0000 - 0x0805 FFFF			128KB			//UPDATA SAVE
扇区7				0x0806 0000 – 0x0807 FFFF			128KB			//UPDATA SAVE  		192K
	
扇区8				0x0808 0000 - 0x0809 FFFF			128KB			//UPDATA SAVE
扇区9				0x080A 0000 – 0x080B FFFF			128KB			//UPDATA SAVE
扇区10			0x080C 0000 - 0x080D FFFF			128KB			//UPDATA SAVE
扇区11			0x080E 0000 – 0x080F FFFF			128KB			//UPDATA SAVE

块1扇区定义
扇区12			0x0810 0000 - 0x0810 3FFF			16KB			//ADJUST
扇区13			0x0810 4000 - 0x0810 7FFF			16KB			//ADJUST
扇区14			0x0810 8000 - 0x0810 BFFF			16KB			//ADJUST
扇区15			0x0810 C000 - 0x0810 FFFF			16KB			//ADJUST
...

*/

#define FLASH_KBDATA_SECTOR CTL_SECTOR_NUMBER_10
#define FLASH_KBDATA_BUFF   0x080C0000
/*******************************************************************************
 * Include files
 ******************************************************************************/

#define VOL_VG1CON   0xA1
#define VOL_VG2CON   0xA2
#define VOL_VD1CON   0xA3
#define VOL_VD2CON   0xA4

#define VOL_VG1FEE   0xB1
#define VOL_VG2FEE   0xB2
#define VOL_VD1FEE   0xB3
#define VOL_VD2FEE   0xB4

#define CURR_IG1FEE   0xC1
#define CURR_IG10FEE  0xC2
#define CURR_IG2FEE   0xC3
#define CURR_IG20FEE  0xC4
#define CURR_ID1FEE   0xC5
#define CURR_ID2FEE   0xC6

#define Temp_1FEE    0xD1
#define Temp_2FEE    0xD2


#define ID_1Compensation    0xD1
#define ID_2Compensation    0xD2
#define ID_Compensation     0xD3

typedef enum  //VG1 档位
{
    VOL_VG1VoltGrade0 = (((int)-15000) * 1),
    VOL_VG1VoltGrade1 = (((int)-10000) * 1),
    VOL_VG1VoltGrade2 = (((int)-5000) * 1), 
    VOL_VG1VoltGrade3 = (((int)0) * 1),
    VOL_VG1VoltGrade4 = (((int)5000) * 1),
    VOL_VG1VoltGrade5 = (((int)10000) * 1),
    VOL_VG1VoltGrade6 = (((int)15000) * 1),
    VOL_VG1VoltGrade7 = (((int)25000) * 1),  
    VOL_VG1VoltGrade8 = (((int)0) * 1),
    VOL_VG1VoltGrade9 = (((int)0) * 1),	
} VOL_VG1VOLTGRADE;

typedef enum //VG2 档位
{
    VOL_VG2VoltGrade0 = (((int)-15000) * 1),
    VOL_VG2VoltGrade1 = (((int)-10000) * 1),
    VOL_VG2VoltGrade2 = (((int)-5000) * 1),
    VOL_VG2VoltGrade3 = (((int)0) * 1),
    VOL_VG2VoltGrade4 = (((int)5000) * 1),
    VOL_VG2VoltGrade5 = (((int)10000) * 1),
    VOL_VG2VoltGrade6 = (((int)15000) * 1),
    VOL_VG2VoltGrade7 = (((int)25000) * 1),
    VOL_VG2VoltGrade8 = (((int)0) * 1),
    VOL_VG2VoltGrade9 = (((int)0) * 1),	
} VOL_VG2VOLTGRADE;

typedef enum//VD1 档位
{
    VOL_VD1VoltGrade0 = (((int)10000) * 1),
    VOL_VD1VoltGrade1 = (((int)20000) * 1),
    VOL_VD1VoltGrade2 = (((int)30000) * 1),
    VOL_VD1VoltGrade3 = (((int)40000) * 1),
    VOL_VD1VoltGrade4 = (((int)50000) * 1),
    VOL_VD1VoltGrade5 = (((int)60000) * 1),
    VOL_VD1VoltGrade6 = (((int)70000) * 1),
    VOL_VD1VoltGrade7 = (((int)80000) * 1),
    VOL_VD1VoltGrade8 = (((int)90000) * 1),
    VOL_VD1VoltGrade9 = (((int)105000) * 1),		
} VOL_VD1VOLTGRADE;

typedef enum//VD2 档位
{
    VOL_VD2VoltGrade0 = (((int)10000) * 1),
    VOL_VD2VoltGrade1 = (((int)20000) * 1),
    VOL_VD2VoltGrade2 = (((int)30000) * 1),
    VOL_VD2VoltGrade3 = (((int)40000) * 1),
    VOL_VD2VoltGrade4 = (((int)50000) * 1),
    VOL_VD2VoltGrade5 = (((int)60000) * 1),
    VOL_VD2VoltGrade6 = (((int)70000) * 1),
    VOL_VD2VoltGrade7 = (((int)80000) * 1),
    VOL_VD2VoltGrade8 = (((int)90000) * 1),
    VOL_VD2VoltGrade9 = (((int)105000) * 1),	
} VOL_VD2VOLTGRADE;
 
typedef enum //IG1 正电流 后面一个1代表正
{

    CURR_IG1Grade0 = (((int)50) * 1),
    CURR_IG1Grade1 = (((int)100) * 1),
    CURR_IG1Grade2 = (((int)500) * 1),
    CURR_IG1Grade3 = (((int)1500) * 1),
} CURR_IG1GRADE;

typedef enum //IG1 负电流 后面一个0代表负
{
    CURR_IG10Grade0 = (((int)-500) * 1),
    CURR_IG10Grade1 = (((int)-100) * 1),
    CURR_IG10Grade2 = (((int)-50) * 1),
    CURR_IG10Grade3 = (((int)0) * 1),
} CURR_IG10GRADE;

typedef enum //IG2 电流 
{
    CURR_IG2Grade0 = (((int)50) * 1),
    CURR_IG2Grade1 = (((int)100) * 1),
    CURR_IG2Grade2 = (((int)500) * 1),
    CURR_IG2Grade3 = (((int)1500) * 1),	
} CURR_IG2GRADE;

typedef enum //IG2 负电流 后面一个0代表负
{
    CURR_IG20Grade0 = (((int)-500) * 1),
    CURR_IG20Grade1 = (((int)-100) * 1),
    CURR_IG20Grade2 = (((int)-50) * 1),
    CURR_IG20Grade3 = (((int)0) * 1),
} CURR_IG20GRADE;


typedef enum //ID1 电流 
{
    CURR_ID1Grade0 = (((int)100) * 1),
    CURR_ID1Grade1 = (((int)200) * 1),
    CURR_ID1Grade2 = (((int)1000) * 1),
    CURR_ID1Grade3 = (((int)2000) * 1),
    CURR_ID1Grade4 = (((int)3000) * 1),
    CURR_ID1Grade5 = (((int)4000) * 1),
    CURR_ID1Grade6 = (((int)5500) * 1),
    CURR_ID1Grade7 = (((int)0) * 1),
    CURR_ID1Grade8 = (((int)0) * 1),
    CURR_ID1Grade9 = (((int)0) * 1),	
} CURR_ID1GRADE;


typedef enum //ID2 电流 
{
    CURR_ID2Grade0 = (((int)100) * 1),
    CURR_ID2Grade1 = (((int)200) * 1),
    CURR_ID2Grade2 = (((int)1000) * 1),
    CURR_ID2Grade3 = (((int)2000) * 1),
    CURR_ID2Grade4 = (((int)3000) * 1),
    CURR_ID2Grade5 = (((int)4000) * 1),
    CURR_ID2Grade6 = (((int)5500) * 1),
    CURR_ID2Grade7 = (((int)0) * 1),
    CURR_ID2Grade8 = (((int)0) * 1),
    CURR_ID2Grade9 = (((int)0) * 1),	
} CURR_ID2GRADE;



typedef enum //Temp1 温度 
{
    Temp1Grade0 = (((int)570) * 1),
    Temp1Grade1 = (((int)1070) * 1),
    Temp1Grade2 = (((int)1570) * 1),
    Temp1Grade3 = (((int)9500) * 1),
    Temp1Grade4 = (((int)0) * 1),
    Temp1Grade5 = (((int)0) * 1),
		Temp1Grade6 = (((int)0) * 1),
    Temp1Grade7 = (((int)0) * 1),
    Temp1Grade8 = (((int)0) * 1),
    Temp1Grade9 = (((int)0) * 1),	
} Temp1_GRADE;

typedef enum //Temp2 温度 
{
    Temp2Grade0 = (((int)570) * 1),
    Temp2Grade1 = (((int)1070) * 1),
    Temp2Grade2 = (((int)1570) * 1),
    Temp2Grade3 = (((int)9500) * 1),
    Temp2Grade4 = (((int)0) * 1),
    Temp2Grade5 = (((int)0) * 1),
		Temp2Grade6 = (((int)0) * 1),
    Temp2Grade7 = (((int)0) * 1),
    Temp2Grade8 = (((int)0) * 1),
    Temp2Grade9 = (((int)0) * 1),	
} Temp2_GRADE;


typedef enum //ID1 电流  CompensationIDKB
{
    COMP_IDGrade0 = (((int)1000) * 1),
    COMP_IDGrade1 = (((int)2000) * 1),
    COMP_IDGrade2 = (((int)3000) * 1),
    COMP_IDGrade3 = (((int)4000) * 1),
    COMP_IDGrade4 = (((int)5500) * 1),
    COMP_IDGrade5 = (((int)0) * 1),
    COMP_IDGrade6 = (((int)0) * 1),
    COMP_IDGrade7 = (((int)0) * 1),
    COMP_IDGrade8 = (((int)0) * 1),
    COMP_IDGrade9 = (((int)0) * 1),	
} COMP_IDGRADE;

//typedef enum //ID2 电流  CompensationIDKB
//{
//    COMP_ID2Grade0 = (((int)1000) * 1),
//    COMP_ID2Grade1 = (((int)2000) * 1),
//    COMP_ID2Grade2 = (((int)3000) * 1),
//    COMP_ID2Grade3 = (((int)4000) * 1),
//    COMP_ID2Grade4 = (((int)5500) * 1),
//    COMP_ID2Grade5 = (((int)0) * 1),
//    COMP_ID2Grade6 = (((int)0) * 1),
//    COMP_ID2Grade7 = (((int)0) * 1),
//    COMP_ID2Grade8 = (((int)0) * 1),
//    COMP_ID2Grade9 = (((int)0) * 1),	
//} COMP_ID2GRADE;


extern uint16_t CalibGrade_Selet(float data, uint16_t Flag);
extern uint16_t Selet_Grade(uint16_t Mode, float data);
extern float ErrorCompensation_v(float vol, uint16_t flag, uint16_t ChanNo);   
extern float FeedBackCalibration_i(float data, uint16_t flag, uint16_t ChanNo); 
extern float FeedBackCalibration_v(float data, uint16_t flag, uint16_t ChanNo); 
extern void ReadNorFlashWord(uint16_t *source, uint16_t *dest, uint32_t len);
extern void EraseNorFlashSector(uint16_t *address, uint16_t SectorNum);
extern void WriteNorFlashBlock(uint16_t *source, uint16_t *dest, uint32_t len);
extern void ReadKBFromSRAM(void);
extern float FeedBackCalibration_Temp(float data, uint16_t flag);
extern uint16_t SeletID_Grade(float data ,uint16_t Mode);
extern float Compensation_ID_KB(float data, uint16_t flag);
/******************************************************************************
 ******************************************************************************/
#endif /* __CALIBRATION_H__ */
/******************************************************************************
 ******************************************************************************/
