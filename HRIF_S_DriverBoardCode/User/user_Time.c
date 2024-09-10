#include "main.h"

/******************************************************************
函数名称：
函数功能:RTC实时时间的配置
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/	
extern struct TIMER_REG  SysTime;
void Rtc_Time_Init(uint8_t time[6])
{
  rtc_deinit();
  rcu_periph_clock_disable(RCU_PMU);
  rcu_periph_clock_disable(RCU_RTC); // 重置

  rcu_periph_clock_enable(RCU_PMU);
  rcu_periph_clock_enable(RCU_RTC);
  pmu_backup_write_enable(); // 启动pmu备份区域
  // rcu_bkp_reset_enable();//重置备份域

  rcu_osci_off(RCU_LXTAL);
  rcu_osci_on(RCU_LXTAL);
  rcu_osci_stab_wait(RCU_LXTAL);
  rcu_rtc_clock_config(RCU_RTCSRC_LXTAL); // 32k晶振启动

  rtc_parameter_struct system_time_s;
  system_time_s.century = 21;
  system_time_s.year = Hex_to_BCD(time[5]);
  system_time_s.month = Hex_to_BCD(time[4]);
  system_time_s.date = Hex_to_BCD(time[3]);
  system_time_s.hour = Hex_to_BCD(time[2]);
  system_time_s.minute = Hex_to_BCD(time[1]);
  system_time_s.second = Hex_to_BCD(time[0]);
  system_time_s.day_of_week = 0x00;

  system_time_s.factor_asyn = 0;
  system_time_s.factor_syn = 0x7FFF;
  system_time_s.am_pm = RTC_AM;
  system_time_s.display_format = RTC_24HOUR;
  rtc_init(&system_time_s);
  RCU_BDCTL |= RCU_BDCTL_RTCEN; // 启动RTC

}
/******************************************************************
函数名称：
函数功能://校准时间 由低到高 秒 分 时 日月年
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void CalibRtcTime(uint8_t Adjust_Time[6])
{ 
    Rtc_Time_Init(&Adjust_Time[0]);
}
/******************************************************************
函数名称：
函数功能:获取实时时间
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Get_Time_Data(uint8_t *GetTime)
{
	rtc_parameter_struct curent_time_s = {0};
	rtc_current_time_get(&curent_time_s);
	*GetTime++ = BCD_to_Hex(curent_time_s.second);
	*GetTime++ = BCD_to_Hex(curent_time_s.minute);
	*GetTime++ = BCD_to_Hex(curent_time_s.hour);
	*GetTime++ = BCD_to_Hex(curent_time_s.date);
	*GetTime++ = BCD_to_Hex(curent_time_s.month);
	*GetTime++ = BCD_to_Hex(curent_time_s.year);
}
/******************************************************************
函数名称：
函数功能:BCD转成HEX格式
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint8_t BCD_to_Hex(uint8_t bcd_num)
{
  return (bcd_num /16 ) * 10 + (bcd_num %16 );
}
/******************************************************************
函数名称：
函数功能:HEX转成BCD格式
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint8_t Hex_to_BCD(uint8_t hex_num)
{
  return ((hex_num /10 ) * 16 + (hex_num % 10));
}
/******************************************************************
函数名称：
函数功能:拷贝实时时间
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void TimeCopy(uint8_t flag)
{
	if (flag == 0)
	{
		SysData.AgingTestData.SampTime.Second = SysTime.Second;
		SysData.AgingTestData.SampTime.Minute = SysTime.Minute;
		SysData.AgingTestData.SampTime.Hour = SysTime.Hour;
		SysData.AgingTestData.SampTime.Day = SysTime.Day;
		SysData.AgingTestData.SampTime.Momth = SysTime.Momth;
		SysData.AgingTestData.SampTime.Year = SysTime.Year;
	}
	else if (flag == 1)
	{
		SysTime.LastSecond = SysTime.Second;
		SysTime.LastMinute = SysTime.Minute;
		
	}
}
/******************************************************************
函数名称：
函数功能:计算时间片程序
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint16_t CalcDeltOfTime(void)
{
	uint16_t time_sum = 0, time_1, time_2;

	time_1 = SysTime.LastSecond; //前一次时间
	time_2 = SysTime.LastMinute;

	if (time_1 > SysTime.Second)
	{
		time_sum = 60 - time_1 + SysTime.Second;
		if (SysTime.Minute > time_2)
		{
			time_sum += (SysTime.Minute - time_2 - 1) * 60;
		}
		else
		{
			time_sum += (SysTime.Minute) * 60;
			// error
		}
	}
	else
	{
		time_sum = SysTime.Second - time_1;
		if (SysTime.Minute >= time_2)
		{
			time_sum += (SysTime.Minute - time_2) * 60;
		}
		else
		{
			time_sum += (SysTime.Minute) * 60;
			// error code
		}
	}

	return time_sum;
}
/******************************************************************
函数名称：
函数功能:老化时间的计算
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/	
void RunTime(void)
{
	uint16_t deltime_p = 0;

	deltime_p = CalcDeltOfTime();
	TimeCopy(1);

	if (SysData.AgingTestData.AgingStatus == AGING_STATUS_RUN)
	{
		if(deltime_p>=5)
		deltime_p = 0;
	}
	else
	{
		deltime_p = 0;
	}

	SysData.AgingTestData.ProRunTimeCnt.data += deltime_p;
}   
