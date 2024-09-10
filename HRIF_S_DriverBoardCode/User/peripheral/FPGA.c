/********************************************************
 * @file:
 * @brief: 模拟信号板驱动程序
 * @details:
 * @Author: ShadowFilter
 * @Date: 2023-04-13 08:52:54
 * @version:
 * @LastEditors: ShadowFilter
 * @LastEditTime: 2023-04-17 14:36:36
 * @FilePath: \GD32F407_templete\User\hardware\Src\FPGA.c
 * @Description:
 **********************************************************/
#include "main.h"
//__IO osThreadId Task_Id_Read;
//__IO float Task_Priority_Get;
#define FPGA_Read_Analog_Data(data, len) FPGA_Read_REG_Datas(0x28, data, len);
#define FPGA_Send_Analog_Data(data, len) FPGA_Write_REG_Datas(0x28, data, len);

#define _TIMEOUT_ 1

#ifdef _TIMEOUT_ // 超时处理
#define TimeOutHanle                     \
	TimeOut++;                             \
	vTaskDelay(2);                         \
	{                                      \
		if (TimeOut > MAX_ANALOGCONFIG_TIME) \
			goto RET;                          \
	}
#define MAX_ANALOGCONFIG_TIME 200
#else
#define TimeOutHanle
#endif
#define Delay_T user_delayUs(20)
/********************************************************
 * @brief
 * @param {FPGA_PARA} *para :
 *         GR_SET;  当组非0时，CH参数无效，GR_SET为几就设置第几组
 *                  1组1-4通道 2组5-8 通道 3组9-12通道
 *         CH;   设置通道 0 表示全局 1-12表示通道1-12
 *         Fre;  设置频率 (1-2000000)
 *         Duty_Ratio; 设置占空比 (0-1)
 * @return {void}
 * @warning
 **********************************************************/
void Fpga_Send(FPGA_PARA *para)
{
	Word32b Period;
	Word32b H_Period;
	uint32_t ReadBackJudge;
	uint16_t CH_OFFSET = para->CH;
	uint32_t GR_FRESET_OFFSET = 0; // 用于补偿设置组频率寄存器地址
	uint32_t GR_RATSET_OFFSET = 0; // 用于补偿设置组占空寄存器地址
	
	uint32_t READ11,READ12;
#ifdef _TIMEOUT_
	uint32_t TimeOut = 0;
#endif
	if (!para->Fre)
		return;
	Period.data = 50000000 / para->Fre;
	if (para->Duty_Ratio > 1 || para->Duty_Ratio < 0)
		return;
	H_Period.data = Period.data * para->Duty_Ratio;
	if (para->GR_SET)
	{
		if (para->GR_SET >3)
			return;
		GR_FRESET_OFFSET = GROP_FRE_BASE - ALL_CH_FRE;
		GR_RATSET_OFFSET = GROP_Ratio_BASE - ALL_CH_Ratio;
		CH_OFFSET = para->GR_SET;
	}
	do
	{
		Data_test(GR_FRESET_OFFSET + CH_FRE(CH_OFFSET)) = Period.Bytes[2];
		Delay_T;
		ReadBackJudge = Data_test(GR_FRESET_OFFSET + CH_FRE(CH_OFFSET));
		TimeOutHanle
	} while (ReadBackJudge != Period.Bytes[2]);
	do
	{
		Data_test(GR_FRESET_OFFSET + CH_FRE(CH_OFFSET) + 1) = Period.Bytes[1];
		Delay_T;
		ReadBackJudge = Data_test(GR_FRESET_OFFSET + CH_FRE(CH_OFFSET) + 1);
		TimeOutHanle
	} while (ReadBackJudge != Period.Bytes[1]);
	do
	{
		Data_test(GR_FRESET_OFFSET + CH_FRE(CH_OFFSET) + 2) = Period.Bytes[0];
		Delay_T;
		ReadBackJudge = Data_test(GR_FRESET_OFFSET + CH_FRE(CH_OFFSET) + 2);
		TimeOutHanle
	} while (ReadBackJudge != Period.Bytes[0]);

	do
	{
		Data_test(GR_RATSET_OFFSET + CH_Ratio(CH_OFFSET)) = H_Period.Bytes[2];
		Delay_T;
		ReadBackJudge = Data_test(GR_RATSET_OFFSET + CH_Ratio(CH_OFFSET));
		TimeOutHanle
	} while (ReadBackJudge != H_Period.Bytes[2]);
	do
	{
		READ11 = Data_test(0x11);
		READ12 = Data_test(0x12);
		Data_test(GR_RATSET_OFFSET + CH_Ratio(CH_OFFSET) + 1) = H_Period.Bytes[1];
		Delay_T;
		ReadBackJudge = Data_test(GR_RATSET_OFFSET + CH_Ratio(CH_OFFSET) + 1);
		TimeOutHanle
	} while (ReadBackJudge != H_Period.Bytes[1]);
	do
	{
		Data_test(GR_RATSET_OFFSET + CH_Ratio(CH_OFFSET) + 2) = H_Period.Bytes[0];
		Delay_T;
		ReadBackJudge = Data_test(GR_RATSET_OFFSET + CH_Ratio(CH_OFFSET) + 2);
		TimeOutHanle
	} while (ReadBackJudge != H_Period.Bytes[0]);

	if (!(para->CH + para->GR_SET))
	{
		do
		{
			Data_test(ALL_SAME) = 0x11;
			Delay_T;
			ReadBackJudge = Data_test(ALL_SAME);
			TimeOutHanle
		} while (ReadBackJudge != 0x11);
	}
	else
	{
		if (!para->GR_SET)
		{
			do
			{
				Data_test(ALL_SAME) = 0x0;
				Delay_T;
				ReadBackJudge = Data_test(ALL_SAME);
				TimeOutHanle
			} while (ReadBackJudge != 0x0);
		}
		else
		{
			do
			{
				Data_test(ALL_SAME) = 0x01;
				Delay_T;
				ReadBackJudge = Data_test(ALL_SAME);
				TimeOutHanle
			} while (ReadBackJudge != 0x01);
		}
	}
RET:
	return;
}
/********************************************************
 * @brief 使能FPGA
 * @param {uint32_t} CH 0 表示全部，1-12分别为通道1到通道12
 * @return {void}
 * @warning
 **********************************************************/
void Fpga_Enable(uint32_t CH)
{
	uint32_t ReadBackJudge;
#ifdef _TIMEOUT_
	uint32_t TimeOut = 0;
#endif
	if (!CH) // 全部使能
	{
		do
		{
			Data_test(CH_ENABLE8_11) = 0x0F;
			Delay_T;
			ReadBackJudge = Data_test(CH_ENABLE8_11);
			TimeOutHanle
		} while (ReadBackJudge != 0x0F);
		do
		{
			Data_test(CH_ENABLE0_7) = 0xFF;
			Delay_T;
			ReadBackJudge = Data_test(CH_ENABLE0_7);
			TimeOutHanle
		} while (ReadBackJudge != 0xFF);
	RET:
		return ;
	}
	if (CH < 9)
	{
		uint32_t buff;
		buff = Data_test(CH_ENABLE0_7);
		buff |= 0x1 << (CH - 1);
		do
		{
			Data_test(CH_ENABLE0_7) = buff;
			Delay_T;
			ReadBackJudge = Data_test(CH_ENABLE0_7);
			TimeOutHanle
		} while (ReadBackJudge != buff);
	}
	else if (CH < 13)
	{
		uint32_t buff;
		buff = Data_test(CH_ENABLE8_11);
		buff |= 0x1 << (CH - 9);
		do
		{
			Data_test(CH_ENABLE8_11) = buff;
			Delay_T;
			ReadBackJudge = Data_test(CH_ENABLE8_11);
			TimeOutHanle
		} while (ReadBackJudge != buff);
	}
	return;
}
/********************************************************
 * @brief 失能FPGA
 * @param {uint32_t} CH 0 表示全部，1-12分别为通道1到通道12
 * @return {void}
 * @warning
 **********************************************************/
void Fpga_Disable(uint32_t CH)
{
	uint32_t ReadBackJudge;
#ifdef _TIMEOUT_
	uint32_t TimeOut = 0;
#endif
	if (!CH) // 全部使能
	{
		do
		{
			Data_test(CH_ENABLE8_11) = 0x00;
			ReadBackJudge = Data_test(CH_ENABLE8_11);
			TimeOutHanle
		} while (ReadBackJudge != 0x00);
		do
		{
			Data_test(CH_ENABLE0_7) = 0x00;
			ReadBackJudge = Data_test(CH_ENABLE0_7);
			TimeOutHanle
		} while (ReadBackJudge != 0x00);
	RET:
		return;
	}
	if (CH < 9)
	{
		uint32_t buff;
		buff = Data_test(CH_ENABLE0_7);
		buff &= ~(0x1 << (CH - 1));
		do
		{
			Data_test(CH_ENABLE0_7) = buff;
			ReadBackJudge = Data_test(CH_ENABLE0_7);
			TimeOutHanle
		} while (ReadBackJudge != buff);
	}
	else if (CH < 13)
	{
		uint32_t buff;
		buff = Data_test(CH_ENABLE8_11);
		buff &= ~(0x1 << (CH - 9));
		do
		{
			Data_test(CH_ENABLE8_11) = buff;
			ReadBackJudge = Data_test(CH_ENABLE8_11);
			TimeOutHanle
		} while (ReadBackJudge != buff);
	}
	return;
}


/********************************************************
 * @brief HRIF_S项目使能FPGA ，当前只用前两个通道
 * @param {uint32_t} CH 0 表示全部，1-2分别为通道1到通道2
 * @return {void}
 * @warning
 **********************************************************/
void HRIF_S_Fpga_ENABLE(uint32_t CH)
{
 switch(CH)
 {
	 case 0 :
		 Fpga_Enable(1);
	   Fpga_Enable(2);
		 break;
	 case 1 :
		 Fpga_Enable(1);
		 break; 
	 case 2 :
	   Fpga_Enable(2);
		 break;	 
	 default:
		 break ;
 }

}


/********************************************************
 * @brief HRIF_S项目发pwm波 ，当前只用前两个通道 在脉冲模式下使能---4个通道
 * @param 
 * @return 
 * @warning
 **********************************************************/
void HRIF_S_Fpga_USE(void)
{
	
	uint32_t Frequcy[2]={0};
	float Duty[2]={0};
	uint32_t TotalPeriod=0;
	

	
	FPGA_PARA Fpag_Data;  // Fpag_Data 结构体初始化

	Fpga_Disable(0);
	
	/* 只发一个参数， */
	
	Duty[0] = ( SysData.AgingTestParameters.DutyCycle[0].data/1000.0 );  // /1000.0
	TotalPeriod = (SysData.AgingTestParameters.PulseWidth[0].data*1000)/(SysData.AgingTestParameters.DutyCycle[0].data);//总周期
	Frequcy[0] = 1000000/TotalPeriod;
//  Frequcy[0] = (( SysData.AgingTestParameters.PulseWidth[0].data )/Duty[0])*1000;	
//	Frequcy[0] = ( (( SysData.AgingTestParameters.PulseWidth[0].data )*1000000)/Duty[0] );
	
	Duty[1] =Duty[0];
	Frequcy[1] =Frequcy[0];
	
//	Duty[1] = ( (SysData.AgingTestParameters.DutyCycle[1].data/10)/100 );
//	Frequcy[1] = ( (( SysData.AgingTestParameters.PulseWidth[0].data )*1000000)/Duty[1] );	
	
	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 1;    //0;
	Fpag_Data.Fre = Frequcy[0];//SysData.AgingTestParameters.SetContrlFre[i].data;   //频率？
	Fpag_Data.Duty_Ratio = Duty[0];//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.2
	Fpga_Send(&Fpag_Data);

	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 2;//0;
	Fpag_Data.Fre = Frequcy[1];//SysData.AgingTestParameters.SetContrlFre[i].data;   //频率？
	Fpag_Data.Duty_Ratio = Duty[1];//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.8
	Fpga_Send(&Fpag_Data);
	
	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 3;    //0;
	Fpag_Data.Fre = Frequcy[0];//SysData.AgingTestParameters.SetContrlFre[i].data;   //频率？
	Fpag_Data.Duty_Ratio = Duty[0];//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.2
	Fpga_Send(&Fpag_Data);

	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 4;//0;
	Fpag_Data.Fre = Frequcy[1];//SysData.AgingTestParameters.SetContrlFre[i].data;   //频率？
	Fpag_Data.Duty_Ratio = Duty[1];//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.8
	Fpga_Send(&Fpag_Data);	
	
	vTaskDelay(500);	
	
//	Fpga_Disable(0);
	Fpga_Enable(0); // FPGA输出波形	

}


/********************************************************
 * @brief HRIF_S项目发pwm波 ，当前只用前两个通道 均发0的占空比
 * @param 
 * @return 
 * @warning
 **********************************************************/
void HRIF_S_Fpga_Init(void)
{
	
	FPGA_PARA Fpag_Data;  // Fpag_Data 结构体初始化
	Fpga_Disable(0);		 
	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 1;    //0;
	Fpag_Data.Fre = 100000;//SysData.AgingTestParameters.PulseWidth[0].data;   //频率？
	Fpag_Data.Duty_Ratio = 0;//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.2
	Fpga_Send(&Fpag_Data);

	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 2;//0;
	Fpag_Data.Fre = 100000;//SysData.AgingTestParameters.SetContrlFre[i].data;   //频率？
	Fpag_Data.Duty_Ratio = 0;//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.8
	Fpga_Send(&Fpag_Data);

	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 3;    //0;
	Fpag_Data.Fre = 100000;//SysData.AgingTestParameters.PulseWidth[0].data;   //频率？
	Fpag_Data.Duty_Ratio = 0;//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.2
	Fpga_Send(&Fpag_Data);

	Fpag_Data.GR_SET = 0;//(i + 1);
	Fpag_Data.CH = 4;//0;
	Fpag_Data.Fre = 100000;//SysData.AgingTestParameters.SetContrlFre[i].data;   //频率？
	Fpag_Data.Duty_Ratio = 0;//(float)(SysData.AgingTestParameters.SetDutyCycle[i].data / 1000.0);   //占空比？0.8
	Fpga_Send(&Fpag_Data);
	
	vTaskDelay(500);	
	
//	Fpga_Disable(0);
//	Fpga_Enable(0); // FPGA输出波形	
  Fpga_Enable(0);
}