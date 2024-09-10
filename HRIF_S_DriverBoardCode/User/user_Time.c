#include "main.h"

/******************************************************************
�������ƣ�
��������:RTCʵʱʱ�������
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/	
extern struct TIMER_REG  SysTime;
void Rtc_Time_Init(uint8_t time[6])
{
  rtc_deinit();
  rcu_periph_clock_disable(RCU_PMU);
  rcu_periph_clock_disable(RCU_RTC); // ����

  rcu_periph_clock_enable(RCU_PMU);
  rcu_periph_clock_enable(RCU_RTC);
  pmu_backup_write_enable(); // ����pmu��������
  // rcu_bkp_reset_enable();//���ñ�����

  rcu_osci_off(RCU_LXTAL);
  rcu_osci_on(RCU_LXTAL);
  rcu_osci_stab_wait(RCU_LXTAL);
  rcu_rtc_clock_config(RCU_RTCSRC_LXTAL); // 32k��������

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
  RCU_BDCTL |= RCU_BDCTL_RTCEN; // ����RTC

}
/******************************************************************
�������ƣ�
��������://У׼ʱ�� �ɵ͵��� �� �� ʱ ������
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void CalibRtcTime(uint8_t Adjust_Time[6])
{ 
    Rtc_Time_Init(&Adjust_Time[0]);
}
/******************************************************************
�������ƣ�
��������:��ȡʵʱʱ��
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
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
�������ƣ�
��������:BCDת��HEX��ʽ
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint8_t BCD_to_Hex(uint8_t bcd_num)
{
  return (bcd_num /16 ) * 10 + (bcd_num %16 );
}
/******************************************************************
�������ƣ�
��������:HEXת��BCD��ʽ
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint8_t Hex_to_BCD(uint8_t hex_num)
{
  return ((hex_num /10 ) * 16 + (hex_num % 10));
}
/******************************************************************
�������ƣ�
��������:����ʵʱʱ��
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
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
�������ƣ�
��������:����ʱ��Ƭ����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint16_t CalcDeltOfTime(void)
{
	uint16_t time_sum = 0, time_1, time_2;

	time_1 = SysTime.LastSecond; //ǰһ��ʱ��
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
�������ƣ�
��������:�ϻ�ʱ��ļ���
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
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
