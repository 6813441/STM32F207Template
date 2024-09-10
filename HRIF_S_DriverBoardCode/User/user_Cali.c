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
/******************************************************************
函数名称：
函数功能: 校准挡位选择
输入参数：data 输入电压值  Flag 电压类型
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint16_t CalibGrade_Selet(float data, uint16_t Flag)
{
    uint16_t grade = 0;
    grade = Selet_Grade(Flag, data);    //选择档位
    return grade;
}
/******************************************************************
函数名称：
函数功能: 选择/计算档位
输入参数：Mode 电压类型  data 输入电压值
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint16_t Selet_Grade(uint16_t Mode, float data)
{
    int SeletVal[MAX_GRADE] = {0};
    uint16_t grade = 0;

    switch (Mode)
    {
    case VOL_VG1CON:  //VG1 程控
        SeletVal[0] = VOL_VG1VoltGrade0;
        SeletVal[1] = VOL_VG1VoltGrade1;
        SeletVal[2] = VOL_VG1VoltGrade2;
        SeletVal[3] = VOL_VG1VoltGrade3;
        SeletVal[4] = VOL_VG1VoltGrade4;
        SeletVal[5] = VOL_VG1VoltGrade5;
        SeletVal[6] = VOL_VG1VoltGrade6;
        SeletVal[7] = VOL_VG1VoltGrade7;
        SeletVal[8] = VOL_VG1VoltGrade8;
        SeletVal[9] = VOL_VG1VoltGrade9;		
        break;
    case VOL_VG2CON: //VG2 程控
        SeletVal[0] = VOL_VG2VoltGrade0;
        SeletVal[1] = VOL_VG2VoltGrade1;
        SeletVal[2] = VOL_VG2VoltGrade2;
        SeletVal[3] = VOL_VG2VoltGrade3;
        SeletVal[4] = VOL_VG2VoltGrade4;
        SeletVal[5] = VOL_VG2VoltGrade5;
        SeletVal[6] = VOL_VG2VoltGrade6;
        SeletVal[7] = VOL_VG2VoltGrade7;
        SeletVal[8] = VOL_VG2VoltGrade8;
        SeletVal[9] = VOL_VG2VoltGrade9;	
        break;
    case VOL_VD1CON: //VD1 程控
        SeletVal[0] = VOL_VD1VoltGrade0;
        SeletVal[1] = VOL_VD1VoltGrade1;
        SeletVal[2] = VOL_VD1VoltGrade2;
        SeletVal[3] = VOL_VD1VoltGrade3;
        SeletVal[4] = VOL_VD1VoltGrade4;
        SeletVal[5] = VOL_VD1VoltGrade5;
        SeletVal[6] = VOL_VD1VoltGrade6;
        SeletVal[7] = VOL_VD1VoltGrade7;
        SeletVal[8] = VOL_VD1VoltGrade8;
        SeletVal[9] = VOL_VD1VoltGrade9;	
        break;
    case VOL_VD2CON://VD2 程控
        SeletVal[0] = VOL_VD2VoltGrade0;
        SeletVal[1] = VOL_VD2VoltGrade1;
        SeletVal[2] = VOL_VD2VoltGrade2;
        SeletVal[3] = VOL_VD2VoltGrade3;
        SeletVal[4] = VOL_VD2VoltGrade4;
        SeletVal[5] = VOL_VD2VoltGrade5;
        SeletVal[6] = VOL_VD2VoltGrade6;
        SeletVal[7] = VOL_VD2VoltGrade7;
        SeletVal[8] = VOL_VD2VoltGrade8;
        SeletVal[9] = VOL_VD2VoltGrade9;
        break;
    case VOL_VG1FEE://VG1 回检
        SeletVal[0] = VOL_VG1VoltGrade0;
        SeletVal[1] = VOL_VG1VoltGrade1;
        SeletVal[2] = VOL_VG1VoltGrade2;
        SeletVal[3] = VOL_VG1VoltGrade3;
        SeletVal[4] = VOL_VG1VoltGrade4;
        SeletVal[5] = VOL_VG1VoltGrade5;
        SeletVal[6] = VOL_VG1VoltGrade6;
        SeletVal[7] = VOL_VG1VoltGrade7;
        SeletVal[8] = VOL_VG1VoltGrade8;
        SeletVal[9] = VOL_VG1VoltGrade9;
        break;
    case VOL_VG2FEE://VG2 回检
        SeletVal[0] = VOL_VG2VoltGrade0;
        SeletVal[1] = VOL_VG2VoltGrade1;
        SeletVal[2] = VOL_VG2VoltGrade2;
        SeletVal[3] = VOL_VG2VoltGrade3;
        SeletVal[4] = VOL_VG2VoltGrade4;
        SeletVal[5] = VOL_VG2VoltGrade5;
        SeletVal[6] = VOL_VG2VoltGrade6;
        SeletVal[7] = VOL_VG2VoltGrade7;
        SeletVal[8] = VOL_VG2VoltGrade8;
        SeletVal[9] = VOL_VG2VoltGrade9;	
        break;
    case VOL_VD1FEE://VD1 回检
        SeletVal[0] = VOL_VD1VoltGrade0;
        SeletVal[1] = VOL_VD1VoltGrade1;
        SeletVal[2] = VOL_VD1VoltGrade2;
        SeletVal[3] = VOL_VD1VoltGrade3;
        SeletVal[4] = VOL_VD1VoltGrade4;
        SeletVal[5] = VOL_VD1VoltGrade5;
        SeletVal[6] = VOL_VD1VoltGrade6;
        SeletVal[7] = VOL_VD1VoltGrade7;
        SeletVal[8] = VOL_VD1VoltGrade8;
        SeletVal[9] = VOL_VD1VoltGrade9;	
        break;
    case VOL_VD2FEE://VD2 回检
        SeletVal[0] = VOL_VD2VoltGrade0;
        SeletVal[1] = VOL_VD2VoltGrade1;
        SeletVal[2] = VOL_VD2VoltGrade2;
        SeletVal[3] = VOL_VD2VoltGrade3;
        SeletVal[4] = VOL_VD2VoltGrade4;
        SeletVal[5] = VOL_VD2VoltGrade5;
        SeletVal[6] = VOL_VD2VoltGrade6;
        SeletVal[7] = VOL_VD2VoltGrade7;
        SeletVal[8] = VOL_VD2VoltGrade8;
        SeletVal[9] = VOL_VD2VoltGrade9;
        break;

    case CURR_IG1FEE://IG1电流回检
        SeletVal[0] = CURR_IG1Grade0;
        SeletVal[1] = CURR_IG1Grade1;
        SeletVal[2] = CURR_IG1Grade2;
        SeletVal[3] = CURR_IG1Grade3;

        break;		
    case CURR_IG10FEE://IG1负电流回检
        SeletVal[0] = CURR_IG10Grade0;
        SeletVal[1] = CURR_IG10Grade1;
        SeletVal[2] = CURR_IG10Grade2;
        SeletVal[3] = CURR_IG10Grade3;
        break;
    case CURR_IG2FEE://IG2正电流回检
        SeletVal[0] = CURR_IG2Grade0;
        SeletVal[1] = CURR_IG2Grade1;
        SeletVal[2] = CURR_IG2Grade2;
        SeletVal[3] = CURR_IG2Grade3;
        break;
    case CURR_IG20FEE://IG2负电流回检
        SeletVal[0] = CURR_IG20Grade0;
        SeletVal[1] = CURR_IG20Grade1;
        SeletVal[2] = CURR_IG20Grade2;
        SeletVal[3] = CURR_IG20Grade3;
        break;		
    case CURR_ID1FEE://ID1电流回检
        SeletVal[0] = CURR_ID1Grade0;
        SeletVal[1] = CURR_ID1Grade1;
        SeletVal[2] = CURR_ID1Grade2;
        SeletVal[3] = CURR_ID1Grade3;
        SeletVal[4] = CURR_ID1Grade4;
        SeletVal[5] = CURR_ID1Grade5;
        SeletVal[6] = CURR_ID1Grade6;
        SeletVal[7] = CURR_ID1Grade7;
        SeletVal[8] = CURR_ID1Grade8;
        SeletVal[9] = CURR_ID1Grade9;
        break;
    case CURR_ID2FEE://ID2电流回检
        SeletVal[0] = CURR_ID2Grade0;
        SeletVal[1] = CURR_ID2Grade1;
        SeletVal[2] = CURR_ID2Grade2;
        SeletVal[3] = CURR_ID2Grade3;
        SeletVal[4] = CURR_ID2Grade4;
        SeletVal[5] = CURR_ID2Grade5;
        SeletVal[6] = CURR_ID2Grade6;
        SeletVal[7] = CURR_ID2Grade7;
        SeletVal[8] = CURR_ID2Grade8;
        SeletVal[9] = CURR_ID2Grade9;
        break;
    case Temp_1FEE://Temp1 温度回检
        SeletVal[0] = Temp1Grade0;
        SeletVal[1] = Temp1Grade1;
        SeletVal[2] = Temp1Grade2;
        SeletVal[3] = Temp1Grade3;
        SeletVal[4] = Temp1Grade4;
        SeletVal[5] = Temp1Grade5;
        SeletVal[6] = Temp1Grade6;
        SeletVal[7] = Temp1Grade7;
        SeletVal[8] = Temp1Grade8;
        SeletVal[9] = Temp1Grade9;
        break;		
    case Temp_2FEE://Temp2 温度回检
        SeletVal[0] = Temp2Grade0;
        SeletVal[1] = Temp2Grade1;
        SeletVal[2] = Temp2Grade2;
        SeletVal[3] = Temp2Grade3;
        SeletVal[4] = Temp2Grade4;
        SeletVal[5] = Temp2Grade5;
        SeletVal[6] = Temp2Grade6;
        SeletVal[7] = Temp2Grade7;
        SeletVal[8] = Temp2Grade8;
        SeletVal[9] = Temp2Grade9;
        break;			
		
    default:
        break;
    }
    if (data <= SeletVal[0])
    {
        grade = 1;
    }
    else if (data <= SeletVal[1])
    {
        grade = 2;
    }
    else if (data <= SeletVal[2])
    {
        grade = 3;
    }
    else if (data <= SeletVal[3])
    {
        grade = 4;
    }
    else if (data <= SeletVal[4])
    {
        grade = 5;
    }
    else if (data <= SeletVal[5])
    {
        grade = 6;
    }
    else if (data <= SeletVal[6])
    {
        grade = 7;
    }
    else if (data <= SeletVal[7])
    {
        grade = 8;
    }
    else if (data <= SeletVal[8])
    {
        grade = 9;
    }		
    else if (data <= SeletVal[9])
    {
        grade = 10;
    }				
    else
    {
        grade = 0;
    }
    return grade;
}


/******************************************************************
函数名称：
函数功能: 选择/计算档位
输入参数：Mode 电压类型  data 输入电压值
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint16_t SeletID_Grade(float data ,uint16_t Mode)
{
    int SeletVal[MAX_GRADE] = {0};
    uint16_t grade = 0;
		Mode = ID_Compensation;
    switch (Mode)
    {
    case ID_Compensation:  
        SeletVal[0] = COMP_IDGrade0;
        SeletVal[1] = COMP_IDGrade1;
        SeletVal[2] = COMP_IDGrade2;
        SeletVal[3] = COMP_IDGrade3;
        SeletVal[4] = COMP_IDGrade4;
        SeletVal[5] = COMP_IDGrade5;
        SeletVal[6] = COMP_IDGrade6;
        SeletVal[7] = COMP_IDGrade7;
        SeletVal[8] = COMP_IDGrade8;
        SeletVal[9] = COMP_IDGrade9;		
        break;
		 default:
        break;
	}
    if (data <= SeletVal[0])
    {
        grade = 1;
    }
    else if (data <= SeletVal[1])
    {
        grade = 2;
    }
    else if (data <= SeletVal[2])
    {
        grade = 3;
    }
    else if (data <= SeletVal[3])
    {
        grade = 4;
    }
    else if (data <= SeletVal[4])
    {
        grade = 5;
    }
    else if (data <= SeletVal[5])
    {
        grade = 6;
    }
    else if (data <= SeletVal[6])
    {
        grade = 7;
    }
    else if (data <= SeletVal[7])
    {
        grade = 8;
    }
    else if (data <= SeletVal[8])
    {
        grade = 9;
    }		
    else if (data <= SeletVal[9])
    {
        grade = 10;
    }				
    else
    {
        grade = 0;
    }
    return grade;		
}		
		
/******************************************************************
函数名称：
函数功能:  【程控电压KB】
输入参数：  vol 浮点型   flag 电压类型  ChanNo工位--HRIF工位1当前不用
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float ErrorCompensation_v(float vol, uint16_t flag, uint16_t ChanNo) // 1正压 0负压
{
    float tempdata;
    uint32_t Kdata, Bdata;
    uint16_t grade;
    grade = CalibGrade_Selet(vol, flag);        //校准档位选择  进入要是mV单位

    switch (flag)
    {
    case VOL_VG1CON:
        Kdata = Calib[0].ConVG1[grade - 1].KVal.data;
        Bdata = Calib[0].ConVG1[grade - 1].BVal.data;
        break;
    case VOL_VG2CON:
        Kdata = Calib[0].ConVG2[grade - 1].KVal.data;
        Bdata = Calib[0].ConVG2[grade - 1].BVal.data;
        break;
    case VOL_VD1CON:
        Kdata = Calib[0].ConVD1[grade - 1].KVal.data;
        Bdata = Calib[0].ConVD1[grade - 1].BVal.data;
        break;
    case VOL_VD2CON:
        Kdata = Calib[0].ConVD2[grade - 1].KVal.data;
        Bdata = Calib[0].ConVD2[grade - 1].BVal.data;
        break;
    default:
        Kdata = 1000000;
        Bdata = 0;
        break;
    }

    if (Kdata > 1500000 || Kdata < 700000) // check that the KB_data is legal
    {
        Kdata = 1000000;
        Bdata = 0;
    }
    else
    {
    }

    tempdata = vol * (Kdata / 1000000.0);

    if (Bdata & 0x80000000)
    {
        tempdata -= (Bdata & 0x7fffffff) / 1000.0;
    }
    else
    {
        tempdata += Bdata / 1000.0;
    }

    return tempdata;
}
/******************************************************************
函数名称：
函数功能://【回检电流KB】
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float FeedBackCalibration_i(float data, uint16_t flag, uint16_t ChanNo) // 1正压 0负压
{
    uint16_t grade;
    float buff;
    uint32_t Kdata, Bdata;

    grade = CalibGrade_Selet(data, flag);

    switch (flag)
    {
    case CURR_IG1FEE:
        Kdata = Calib[0].FeeIG1[grade - 1].KVal.data;
        Bdata = Calib[0].FeeIG1[grade - 1].BVal.data;
        break;
    case CURR_IG10FEE:
        Kdata = Calib[0].FeeIG10[grade - 1].KVal.data;
        Bdata = Calib[0].FeeIG10[grade - 1].BVal.data;
        break;		
    case CURR_IG2FEE:
        Kdata = Calib[0].FeeIG2[grade - 1].KVal.data;
        Bdata = Calib[0].FeeIG2[grade - 1].BVal.data;
        break;
    case CURR_IG20FEE:
        Kdata = Calib[0].FeeIG20[grade - 1].KVal.data;
        Bdata = Calib[0].FeeIG20[grade - 1].BVal.data;
        break;		
    case CURR_ID1FEE:
        Kdata = Calib[0].FeeID1[grade - 1].KVal.data;
        Bdata = Calib[0].FeeID1[grade - 1].BVal.data;
        break;
    case CURR_ID2FEE:
        Kdata = Calib[0].FeeID2[grade - 1].KVal.data;
        Bdata = Calib[0].FeeID2[grade - 1].BVal.data;
        break;
    default:
        Kdata = 1000000;
        Bdata = 0;
        break;
    }

    if (Kdata > 1500000 || Kdata < 700000)
    {
        Kdata = 1000000;
        Bdata = 0;
    }
    else
    {
    }

    buff = data * Kdata / 1000000.0;

    if (Bdata & 0x80000000)
    {
        buff -= (Bdata & 0x7fffffff) / 1000.0;
    }
    else
    {
        buff += Bdata / 1000.0;
    }

    return buff;
}
/******************************************************************
函数名称：【回检电压KB】
函数功能://FeedBackCalibration_v(float data,uint16_t flag,uint16_t ChanNo)
输入参数：
输出参数：
返回值：
注意： //1正压 0负压
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float FeedBackCalibration_v(float data, uint16_t flag, uint16_t ChanNo)
{
    uint16_t grade;
    float buff;
    uint32_t Kdata, Bdata;

    grade = CalibGrade_Selet(data, flag);

    switch (flag)
    {
    case VOL_VG1FEE:
        Kdata = Calib[0].FeeVG1[grade - 1].KVal.data;
        Bdata = Calib[0].FeeVG1[grade - 1].BVal.data;
        break;
    case VOL_VG2FEE:
        Kdata = Calib[0].FeeVG2[grade - 1].KVal.data;
        Bdata = Calib[0].FeeVG2[grade - 1].BVal.data;
        break;
    case VOL_VD1FEE:
        Kdata = Calib[0].FeeVD1[grade - 1].KVal.data;
        Bdata = Calib[0].FeeVD1[grade - 1].BVal.data;
        break;
    case VOL_VD2FEE:
        Kdata = Calib[0].FeeVD2[grade - 1].KVal.data;
        Bdata = Calib[0].FeeVD2[grade - 1].BVal.data;
        break;
    default:
        Kdata = 1000000;
        Bdata = 0;
        break;
    }

    if (Kdata > 1500000 || Kdata < 700000) // check that the KB_data is legal
    {
        Kdata = 1000000;
        Bdata = 0;
    }
    else
    {
    }

    buff = data * Kdata / 1000000.0;

    if (Bdata & 0x80000000)
    {
        buff -= (Bdata & 0x7fffffff) / 1000.0;
    }
    else
    {
        buff += Bdata / 1000.0;
    }

    return buff;
}


/******************************************************************
函数名称：【回检电压KB】
函数功能://FeedBackCalibration_v(float data,uint16_t flag,uint16_t ChanNo)
输入参数：
输出参数：
返回值：
注意： //1正压 0负压
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float FeedBackCalibration_Temp(float data, uint16_t flag)
{
    uint16_t grade;
    float buff;
    uint32_t Kdata, Bdata;

    grade = CalibGrade_Selet(data, flag);

    switch (flag)
    {
    case Temp_1FEE:
        Kdata = Calib[0].FeeTemp[grade - 1].KVal.data;
        Bdata = Calib[0].FeeTemp[grade - 1].BVal.data;
        break;
    case Temp_2FEE:
        Kdata = Calib[0].FeeTemp[(grade - 1)+4].KVal.data;
        Bdata = Calib[0].FeeTemp[(grade - 1)+4].BVal.data;
        break;
    default:
        Kdata = 1000000;
        Bdata = 0;
        break;
    }

    if (Kdata > 1500000 || Kdata < 700000) // check that the KB_data is legal
    {
        Kdata = 1000000;
        Bdata = 0;
    }
    else
    {
    }

    buff = data * Kdata / 1000000.0;

    if (Bdata & 0x80000000)
    {
        buff -= (Bdata & 0x7fffffff) / 1000.0;
    }
    else
    {
        buff += Bdata / 1000.0;
    }

    return buff;
}


/******************************************************************
函数名称：【补偿ID B值】
函数功能:
输入参数：
输出参数：
返回值：
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float Compensation_ID_KB(float data, uint16_t flag)
{
	 /*  ID1 ID2补偿值存放于温度KB中，硬件采样不稳定导致 */
    uint16_t grade;
    float buff;
    uint32_t Kdata, Bdata;

    grade = SeletID_Grade(data, flag);

    switch (flag)
    {
    case ID_1Compensation:
        Kdata = Calib[0].FeeTemp[((grade - 1)+8)].KVal.data;
        Bdata = Calib[0].FeeTemp[((grade - 1)+8)].BVal.data;
        break;
    case ID_2Compensation:
        Kdata = Calib[0].FeeTemp[(grade - 1)+13].KVal.data;
        Bdata = Calib[0].FeeTemp[(grade - 1)+13].BVal.data;
        break;
    default:
        Kdata = 1000000;
        Bdata = 0;
        break;
    }
		if(Kdata==1000000)	{                     //K值为1 才调用B值
				if( (Bdata & 0x7fffffff)> 500 ){      //B值大于+0.5，-0.5时候取0 
						Bdata=0;
				}		
				if (Bdata & 0x80000000)
				{
					buff =	SysData.AgingTestData.Station_VD_Voltage[1].data/10000.0 ;
					buff =  buff*(Bdata & 0x7fffffff) / 1000.0;
					buff = (-1)*buff;
					buff = buff+data;
				}	
				else
				{
					buff =	SysData.AgingTestData.Station_VD_Voltage[1].data/10000.0 ;
					buff =  buff*(Bdata & 0x7fffffff) / 1000.0;
					buff = buff+data;
				}
		}
		else{
				buff = data;
		}
    return buff;
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
extern uint16_t KbData[200];
void ReadNorFlashWord(uint16_t *source, uint16_t *dest, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        *dest++ = *source++;
    }
}
/******************************************************************
函数名称：
函数功能:擦除用于存储KB的flash
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void EraseNorFlashSector(uint16_t *address, uint16_t SectorNum)
{
    fmc_unlock();
    fmc_sector_erase(FLASH_KBDATA_SECTOR);
    fmc_lock();
}
/******************************************************************
函数名称：
函数功能:按半字写入KB到flash
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void WriteNorFlashBlock(uint16_t *source, uint16_t *dest, uint32_t len)
{
    fmc_unlock();
    for (int i = 0; i < len; i++) // 写入
    fmc_halfword_program((uint32_t)(dest + i), *(uint16_t *)(source + i));
    fmc_lock();
}
/******************************************************************
函数名称：
函数功能:从flash中读取KB并给结构体赋值
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/

void ReadKBFromSRAM(void)
{
    struct CAL_REGS *sptr;
    sptr = (struct CAL_REGS *)(FLASH_KBDATA_BUFF);
//		for (uint16_t i=0;i<4;i++)
		Calib[0] = sptr[0];
}

#undef Line
