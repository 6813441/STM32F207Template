#include "main.h"
//const float EPSINON=0.000001;
/******************************************************************
函数名称：
函数功能:异常处理
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void AbnormalHandle(void)
{
  Station1VolAbnormal();  //VD
	Station2VolAbnormal();  //VG
	
	Station1CurrAbnormal(); //ID
	Station2CurrAbnormal(); //IG

	StationTempAbnormal();
	AbnormalCtrlHandle();
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
void AbnormalCtrlHandle()
{
    uint16_t Cnt, i;

    Cnt = 0;
    for (i = 0; i < 2; i++)
    {
		if (SysProg.CtrlAGE_RUN[i] == 1)
        {
			if ((SysData.AgingTestData.Voltage_VD_AlarmInformation[i] != aging_alarm_nomal)
				||(SysData.AgingTestData.Curr_ID_AlarmInformation[i] != aging_alarm_nomal)
			    ||(SysData.AgingTestData.Voltage_VG_AlarmInformation[i] != aging_alarm_nomal)
				||(SysData.AgingTestData.Curr_IG_AlarmInformation[i] != aging_alarm_nomal)
			    ||(SysData.AgingTestData.Temp_TC_AlarmInformation[i]!= aging_alarm_nomal))
			{
				Cnt++;
			}
	    }
    }

    if (Cnt > 0)
    {
        AgingStateConfig(AGING_STATUS_ERROR_STOP);
    }
}

/******************************************************************
函数名称：VD1 
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Station1VolAbnormal()
{
	float vflct[2], lvwn, hvwn,Lvwn1;
  uint8_t itemp;
	if(SysData.AgingTestParameters.Set_VD_Vol.data!=0)
	{
		/* 	全部转化成正值  */
		lvwn = (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)*10;  //Mv放大100倍 然后*0.8		
    lvwn =lvwn*0.8;//mv
		Lvwn1 = ((SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)*10-5000);
		hvwn = (SysData.AgingTestParameters.Set_VD_VolUp.data& 0x7fffffff)*10;   //mv
	
		vflct[0] = (SysData.AgingTestData.Station_VD_Voltage[0].data & 0x7fffffff)/10;
		vflct[1] = (SysData.AgingTestData.Station_VD_Voltage[1].data & 0x7fffffff)/10;
			
		for(itemp=0;itemp<2;itemp++){
			
					if (vflct[itemp] > hvwn)
					{
						SysProg.Voltage_VD_AlarmCntUp[itemp]++;
						if (SysProg.Voltage_VD_AlarmCntUp[itemp] >= WarnCompCnt)
						{
							SysData.AgingTestData.Voltage_VD_AlarmInformation[itemp] = aging_alarm_volup;
						}
					}
					else
					{
						SysProg.Voltage_VD_AlarmCntUp[itemp] = 0;
					}
					if(vflct[itemp]<20000){
//						if ( (vflct[itemp] < lvwn) || ( vflct[itemp]<Lvwn1 ) )	
						if ( (vflct[itemp] < lvwn) )
						{
							SysProg.Voltage_VD_AlarmCntUp[itemp]++;
							if (SysProg.Voltage_VD_AlarmCntUp[itemp] >= WarnCompCnt)
							{
								SysData.AgingTestData.Voltage_VD_AlarmInformation[itemp] = aging_alarm_voldown;
							}
						}
						else
						{
							SysProg.Voltage_VD_AlarmCntUp[itemp] = 0;
						}
					}
          else{
						if ( (vflct[itemp] < Lvwn1) )
						{
							SysProg.Voltage_VD_AlarmCntUp[itemp]++;
							if (SysProg.Voltage_VD_AlarmCntUp[itemp] >= WarnCompCnt)
							{
								SysData.AgingTestData.Voltage_VD_AlarmInformation[itemp] = aging_alarm_voldown;
							}
						}
						else
						{
							SysProg.Voltage_VD_AlarmCntUp[itemp] = 0;
						}					
					
					}					
		
		}
	}

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
void Station2VolAbnormal()
{
	float vflct[2]={0}, lvwn, hvwn;
  uint8_t itemp;
	if(SysData.AgingTestParameters.Set_VG_Vol.data!=0)
	{
		/* 	全部转化成正值  */
		lvwn = ((SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100)*0.8*1000;  //放大100倍 然后*0.8		
		hvwn = (SysData.AgingTestParameters.Set_VG_VolUp.data& 0x7fffffff)*10;///100*1000;    mv
	
		vflct[0] = (SysData.AgingTestData.Station_VG_Voltage[0].data & 0x7fffffff)/10;// /1000;  mv
		vflct[1] = (SysData.AgingTestData.Station_VG_Voltage[1].data & 0x7fffffff)/10;// /1000;
		
			
		for(itemp=0;itemp<2;itemp++){
			
					if (vflct[itemp] > hvwn)
					{
						SysProg.Voltage_VG_AlarmCntUp[itemp]++;
						if (SysProg.Voltage_VG_AlarmCntUp[itemp] >= WarnCompCnt)
						{
							SysData.AgingTestData.Voltage_VG_AlarmInformation[itemp] = aging_alarm_volup;
						}
					}
					else
					{
						SysProg.Voltage_VG_AlarmCntUp[itemp] = 0;
					}
					if ( (vflct[itemp] < lvwn) )  //未验证
					{
						SysProg.Voltage_VG_AlarmCntUp[itemp]++;
						if (SysProg.Voltage_VG_AlarmCntUp[itemp] >= WarnCompCnt)
						{
							SysData.AgingTestData.Voltage_VG_AlarmInformation[itemp] = aging_alarm_voldown;
						}
					}
					else
					{
						SysProg.Voltage_VD_AlarmCntUp[itemp] = 0;
					}
		
		}
	}
}

/******************************************************************
函数名称：ID 电流
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Station1CurrAbnormal()
{
	float vflct[2], lvwn, hvwn;
	uint8_t itemp;

	vflct[0] = (SysData.AgingTestData.Station_ID_Curr[0].data & 0x7fffffff)/10;// /1000;
	vflct[1] = (SysData.AgingTestData.Station_ID_Curr[1].data & 0x7fffffff)/10;// /1000;    
	for (itemp = 0; itemp < 2; itemp++)
	{
		hvwn = (SysData.AgingTestParameters.Set_ID_CurrUp.data& 0x7fffffff)*10;              //单位A
  	hvwn = hvwn/1000; //MA
//		if(SysData.AgingTestData.Station_ID_Curr[itemp].data>0x80000000){
//			vflct[itemp] = (SysData.AgingTestData.Station_ID_Curr[itemp].data& 0x7fffffff);    //单位mA
////			vflct[itemp] = vflct[itemp]*(-1);
//		}
//		else{
//			vflct[itemp] = (SysData.AgingTestData.Station_ID_Curr[itemp].data& 0x7fffffff);
//		}

			if (vflct[itemp] > hvwn)
			{
				SysProg.Curr_ID_AlarmCntUp[itemp]++;
				if (SysProg.Curr_ID_AlarmCntUp[itemp] >= WarnCompCnt)
				{
					SysData.AgingTestData.Curr_ID_AlarmInformation[itemp] = aging_alarm_curup;
				}
			}
			else
			{
				SysProg.Curr_ID_AlarmCntUp[itemp] = 0;
			}
  }

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
void Station2CurrAbnormal()
{
    float vflct[2], lvwn, hvwn;
    uint8_t itemp;
	
	vflct[0] = (SysData.AgingTestData.Station_IG_Curr[0].data & 0x7fffffff)/10;// /1000;
	vflct[1] = (SysData.AgingTestData.Station_IG_Curr[1].data & 0x7fffffff)/10;// /1000;	
    
	for (itemp = 0; itemp < 2; itemp++)
	{
		hvwn = (SysData.AgingTestParameters.Set_IG_CurrUp.data& 0x7fffffff)*10;
  	hvwn = hvwn/1000; //MA	
////		if(SysData.AgingTestData.Station_IG_Curr[itemp].data>0x80000000){
////			vflct[itemp] = (SysData.AgingTestData.Station_IG_Curr[itemp].data& 0x7fffffff);    //单位mA
//////			vflct[itemp] = vflct[itemp]*(-1);
////		}
////		else{
////			vflct[itemp] = (SysData.AgingTestData.Station_IG_Curr[itemp].data& 0x7fffffff);
////		}
//////		vflct[itemp] = (SysData.AgingTestData.Station_IG_Curr[itemp].data & 0x7fffffff)/1000;

			if (vflct[itemp] > hvwn)
			{
				SysProg.Curr_IG_AlarmCntUp[itemp]++;
				if (SysProg.Curr_IG_AlarmCntUp[itemp] >= WarnCompCnt)
				{
					SysData.AgingTestData.Curr_IG_AlarmInformation[itemp] = aging_alarm_curup;
				}
			}
			else
			{
				SysProg.Curr_IG_AlarmCntUp[itemp] = 0;
			}
  }
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
void StationTempAbnormal()
{
    float vflct, lvwn, hvwn;
    uint8_t itemp;
    
	for (itemp = 0; itemp < 2; itemp++)
	{
		hvwn = SysData.AgingTestParameters.SetTempUp.data& 0x7fffffff;
//		if(SysData.AgingTestParameters.SetTempUp.data&0x80000000)
//		{
//			hvwn=hvwn*(-1.0);
//		}
//		else if(SysData.AgingTestParameters.SetTempUp.data==0)
//		{
//			hvwn=EPSINON;
//		}

		vflct = SysData.AgingTestData.Station_TC_Temp[itemp].data & 0x7fffffff;
//		if(SysData.AgingTestData.Temp_TC_AlarmInformation[itemp].data&0x80000000)
//		{
//			vflct=vflct*(-1.0);
//		}
//		if (SysProg.Ctrl_AGE_RUN[itemp] == 1)
//        {
			if (vflct > hvwn)
			{
				SysProg.TempAlarmCntUp[itemp]++;
				if (SysProg.TempAlarmCntUp[itemp] >= WarnCompCnt)
				{
					SysData.AgingTestData.Temp_TC_AlarmInformation[itemp] = aging_alarm_Tempup;
				}
			}
			else
			{
				SysProg.TempAlarmCntUp[itemp] = 0;
			}
						
	  }
//	}
} 
