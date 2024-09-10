#include "main.h"
static  uint16_t ad_retval1[10] = {0};
/******************************************************************
函数名称：BoardTest  --HRIF-S
函数功能: 单板测试
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/									
//void BoardTest(void)
//{
//	
//	float ADValue[8];
//	float TempValue[2];
//	
////	FSDSetup(48000,20000,30000,30000);  
////	vTaskDelay(20);
////	HOTSetup(30000,30000,30000,30000);
//	
//	
////	SGM5349_DA( VG1_DA ,-20 ,DYB);
////	SGM5349_DA( VG2_DA ,-20 ,DYB);
////	vTaskDelay(200);	
//	SGM5349_DA( VG1_DA ,20 ,DYB);
//	SGM5349_DA( VG2_DA ,20 ,DYB);
//	vTaskDelay(200);	
//	
//	SGM5349_DA( VD1_DA ,20 ,DYB );
//	SGM5349_DA( VD2_DA ,20 ,DYB ); 
//	
//	SGM5349_DA( ICC_VG1_P_DA ,2 ,DYB );   //300mA  0.3-- 保护电压  2a
//	SGM5349_DA( ICC_VG2_P_DA ,2 ,DYB );   //300mA  0.3-- 保护电压
//	
//	vTaskDelay(3000);
//	for(int i=0 ;i<8;i++)
//	{
//		ADValue[i]=Read_AD_Value(i, DYB);
//	}
//  
//	
///*保护测试--保护电压发起可与其他电压一起发，检测到程控电压发起后，保护使能打开void ConCLR(uint16_t mode) mode参数输入SET */	
////	ConCLR(SET); //                             ---保护使能

//  SGM5349_DA( TempProtect1 ,100 ,WDB );	
//	SGM5349_DA( TempProtect2 ,100 ,WDB );

//		
//	while(1){
//		
//  	vTaskDelay(1000);		
//		vTaskDelay(10000);
//		
//	}


//}	





/******************************************************************
函数名称：CheckStopVol  --HRIF-S
函数功能: 检查电压是否发起
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint16_t CheckStopVol(uint16_t ch)
{	
	
	float RealValue=0,SetValue=0;
	uint16_t SignFlag=0;          // 为10为正   为20为负
  
	switch(ch)
		{
				case VG1_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;       //单位V
				   if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
//					 SetValue =(-1)*SetValue;
						 SignFlag=20;
					 }
					 else{
//					 SetValue =SetValue;
						 SignFlag=10;
					 }

				   RealValue= ( Read_AD_Value(VG1_AD,DYB)*(-5) )/1000.0 ;                               //单位V
           if(SignFlag==20){
							 if(  (RealValue*(-1)) < (SetValue*0.1)   ){
							 }
							 else{
										return ERROR ;
							 }							 
					 
					 }
					 else if (SignFlag==10){ //为正
						 
							 if(  RealValue < (SetValue*0.1)   ){
							 }
							 else{
										return ERROR ;
							 }					 
					 
					 }

					break ;
				
				case VG2_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;     //单位V
				   if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
//					 SetValue =(-1)*SetValue;
						 SignFlag=20;
					 }
					 else{
//					 SetValue =SetValue;
						 SignFlag=10;
					 }				
				   RealValue= ( Read_AD_Value(VG2_AD,DYB)*(-5) )/1000.0 ;                               //单位V
           if(SignFlag==20){
							 if(  (RealValue*(-1)) < (SetValue*0.1)    ){
							 }
							 else{
										return ERROR ;
							 }							 
					 
					 }
					 else if (SignFlag==10){ //为正
						 
							 if(  RealValue < (SetValue*0.1)   ){
							 }
							 else{
										return ERROR ;
							 }					 
					 
					 }
					break ;
				
				case VD1_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;     //单位V
				   RealValue= ( Read_AD_Value(VD1_AD,DYB)/0.04 )/1000.0 ;                               //单位V
					 if(  RealValue < (SetValue*0.3)   ){
//								SysCtrl.VGVolFlag[0]=0;						 
					 }
 				   else{
//								SysCtrl.VGVolFlag[1]++;				 
								
					      return ERROR ;
					 }	
					break ;
				
				case VD2_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;     //单位V
				   RealValue= ( Read_AD_Value(VD2_AD,DYB)/0.04 )/1000.0 ;                               //单位V
					 if(  RealValue < (SetValue*0.3)   ){
										
//								SysCtrl.VGVolFlag[0]=0;		 
					 }
 				   else{
//								SysCtrl.VGVolFlag[1]++;				 						 
					      return ERROR ;
					 }					 
					break ;
		 }

   return SUCCESS ;
}




/******************************************************************
函数名称：CheckInitiatingVol  --HRIF-S
函数功能: 检查电压是否发起
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint16_t CheckInitiatingVol(uint16_t ch)
{	
	
	float RealValue=0,SetValue=0;
	uint16_t SignFlag=0;          // 为10为正   为20为负
  
	switch(ch)
		{
				case VG1_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;       //单位V
				   if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
//					 SetValue =(-1)*SetValue;
						 SignFlag=20;
					 }
					 else{
//					 SetValue =SetValue;
						 SignFlag=10;
					 }

				   RealValue= ( Read_AD_Value(VG1_AD,DYB)*(-5) )/1000.0 ;                               //单位V
           if(SignFlag==20){
							 if(  (RealValue*(-1)) > (SetValue*0.85)   ){
							 }
							 else{
										return ERROR ;
							 }							 
					 
					 }
					 else if (SignFlag==10){ //为正
						 
							 if(  RealValue > (SetValue*0.85)   ){
							 }
							 else{
										return ERROR ;
							 }					 
					 
					 }

					break ;
				
				case VG2_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;     //单位V
				   if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
//					 SetValue =(-1)*SetValue;
						 SignFlag=20;
					 }
					 else{
//					 SetValue =SetValue;
						 SignFlag=10;
					 }				
				   RealValue= ( Read_AD_Value(VG2_AD,DYB)*(-5) )/1000.0 ;                               //单位V
           if(SignFlag==20){
							 if(  (RealValue*(-1)) > (SetValue*0.85)    ){
							 }
							 else{
										return ERROR ;
							 }							 
					 
					 }
					 else if (SignFlag==10){ //为正
						 
							 if(  RealValue > (SetValue*0.85)   ){
							 }
							 else{
										return ERROR ;
							 }					 
					 
					 }
					break ;
				
				case VD1_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;     //单位V
				   RealValue= ( Read_AD_Value(VD1_AD,DYB)/0.04 )/1000.0 ;                          //单位V
           if(SetValue>=5){
						 if(  RealValue > (SetValue*0.85)   ){					 
						 }
						 else{
									return ERROR ;
						 }	
					 }
					 else{
						 if(  RealValue > (SetValue*0.5)   ){					 
						 }
						 else{
									return ERROR ;
						 }						 
					 
					 }
					break ;
				
				case VD2_AD :
					 SetValue= (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;     //单位V
				   RealValue= ( Read_AD_Value(VD2_AD,DYB)/0.04 )/1000.0 ;                               //单位V
//					 if(  RealValue > (SetValue*0.85)   ){
//					 }
// 				   else{		 						 
//					      return ERROR ;
//					 }	
           if(SetValue>=5){
						 if(  RealValue > (SetValue*0.85)   ){					 
						 }
						 else{
									return ERROR ;
						 }	
					 }
					 else{
						 if(  RealValue > (SetValue*0.5)   ){					 
						 }
						 else{
									return ERROR ;
						 }						 
					 
					 }				
					break ;
		 }

   return SUCCESS ;
}


									
/******************************************************************
函数名称：Read_AD_Value  --HRIF-S
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float ad_retvalID[2000];
float  Read_AD_Value(uint16_t ch,uint16_t Board)
{
//	  if(SysCtrl.StartMode==PulseMode){	
//			SysCtrl.PulseModeFlag=10;
//		}
//		if(SysCtrl.PulseModeFlag==10){}			
	uint16_t i, u16_sampcnt = 10, u16_abandon = 3;
  uint32_t atemp;	
	float ad_retval[10], btemp;	
//  float ad_retvalID[1000];
//	int i=0;
	volatile float WAD_Value;
	float SampTest[10]={0};
	float SampTest_Value;
	int DelayTime=0;
	/* 清除为0 */
	for(i=0;i<10;i++){
		SysProg.SampBitValue[i]=0;
	  SysProg.SampValue[i]   =0;
	}
	if (Board == DYB)
	{
//    SysProg.SampTempFlag =10;  //当温度采样标志为10 则不允许温度采样
			switch(ch)
		{
				case VG1_AD :
 					Select_Channel( VG1_AD, DYB );
				  vTaskDelay(10);
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[AD_DYB];                //通道1（0-7）
						if(atemp>=65534){
						  atemp=0;
						}					
						ad_retval[i] = (float)(atemp);
				} 
				 SortValue(ad_retval, u16_sampcnt);   //排序
				for(int SortI=0;SortI<10;SortI++){
 			    if(ad_retval[9]>=65400){
					   if( ad_retval[SortI]== 0 ){
						   ad_retval[SortI] = ad_retval[9];
						 }
					}
					    				
				}				
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);				
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV
			  	SW_EN_DYB_L;
				 
					break ;
				
				case VG2_AD :
					Select_Channel( VG2_AD, DYB );
				  vTaskDelay(10);
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[AD_DYB];                //通道1（0-7）
						if(atemp>=65534){
						  atemp=0;
						}					
						ad_retval[i] = (float)(atemp);
				} 
				 SortValue(ad_retval, u16_sampcnt);   //排序
				for(int SortI=0;SortI<10;SortI++){
 			    if(ad_retval[9]>=65400){
					   if( ad_retval[SortI]== 0 ){
						   ad_retval[SortI] = ad_retval[9];
						 }
					}
					    				
				}				
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV				
			  	SW_EN_DYB_L;

					break ;
				
				case VD1_AD :
					Select_Channel( VD1_AD, DYB );
				  vTaskDelay(10);
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[AD_DYB];                //通道1（0-7）
						if(atemp>=65500){
						  atemp=0;
						}					
						ad_retval[i] = (float)(atemp);
				}     	 
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value=btemp*1000;    //转化成mV			
				  SW_EN_DYB_L;	
					break ;
				
				case VD2_AD :
					Select_Channel( VD2_AD, DYB );
			  	vTaskDelay(10);			
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[AD_DYB];                //通道1（0-7）
						if(atemp>=65500){
						  atemp=0;
						}					
						ad_retval[i] = (float)(atemp);
				}     	 
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV				
			  	SW_EN_DYB_L;				
					break ;
				
				case ICC_VG1_AD :
					Select_Channel( ICC_VG1_AD, DYB );
				  vTaskDelay(10);
					for (i = 0; i < (u16_sampcnt); i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[AD_DYB];                //通道1（0-7）
					  ad_retval[i] = (float)(atemp);
				}     	 

				 SortValue(ad_retval, u16_sampcnt);   //排序
				//【当在正负0荡时】
				if(  (ad_retval[9]>=65400)&&(ad_retval[0]<=32) ){  
						for (i = 0; i < (u16_sampcnt-1); i++)
					  {
							AD7606ReadOneSample(&ad_retval1[0]);
							atemp = ad_retval1[AD_DYB];                //通道1（0-7）
							ad_retval[i] = (float)(atemp);
					  } 

						btemp = DigitalFilter_f32(ad_retval, u16_sampcnt-1, u16_abandon+1);
						
						if(btemp>32768){
							 btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
							 btemp = btemp*(-1);
						}
						else{
							 btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
						}
						WAD_Value =btemp*1000;    //转化成mV
			  	  SW_EN_DYB_L;
			
					  break ;						
				
				}
				
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
				
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV														  	
			  SW_EN_DYB_L;
			
					break ;					
				
				case ICC_VG2_AD :
					Select_Channel( ICC_VG2_AD, DYB );
				  vTaskDelay(10);	
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[AD_DYB];                //通道1（0-7）			
						ad_retval[i] = (float)(atemp);
				}     	 
				 SortValue(ad_retval, u16_sampcnt);   //排序
				//【当在正负0荡时】
				if(  (ad_retval[9]>=65400)&&(ad_retval[0]<=32) ){  
						for (i = 0; i < (u16_sampcnt-1); i++)
					  {
							AD7606ReadOneSample(&ad_retval1[0]);
							atemp = ad_retval1[AD_DYB];                //通道1（0-7）
							ad_retval[i] = (float)(atemp);
					  } 

						btemp = DigitalFilter_f32(ad_retval, u16_sampcnt-1, u16_abandon+1);
						
						if(btemp>32768){
							 btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
							 btemp = btemp*(-1);
						}
						else{
							 btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
						}
						WAD_Value =btemp*1000;    //转化成mV
			  	  SW_EN_DYB_L;
			
					  break ;						
				
				}
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);				
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV				
			  	SW_EN_DYB_L;	
				
					break ;
				
				
				
				
				
				case ICC_VD1_AD :			
			if(SysCtrl.StartMode!=PulseMode){					
							Select_Channel( ICC_VD1_AD, DYB );
							vTaskDelay(30);			
							for (i = 0; i < u16_sampcnt; i++)
						{
								AD7606ReadOneSample(&ad_retval1[0]);
								atemp = ad_retval1[AD_DYB];                //通道1（0-7）
								if(atemp>=32769){
									atemp=0;
								}					
								ad_retval[i] = (float)(atemp);
						}
						 SortValue(ad_retval, u16_sampcnt);   //排序		
						btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
						if(btemp>32768){
							 btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
							 btemp = btemp*(-1);
						}
						else{
							 btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
						}
						WAD_Value =btemp*1000;    //转化成mV				
							SW_EN_DYB_L;	
			 }
			else{
							Select_Channel( ICC_VD1_AD, DYB );
							vTaskDelay(30);
              DelayTime	 =MachineCycle();			 
							for (i = 0; i < 2000; i++)
						{
								AD7606ReadOneSample(&ad_retval1[0]);
								atemp = ad_retval1[AD_DYB];                //通道1（0-7）
								if(atemp>=32769){
									atemp=0;
								}					
								ad_retvalID[i] = (float)(atemp);
								user_delayUs(DelayTime);
						}		
						btemp = DigitalFilter_f32ID(ad_retvalID, 2000, 0);
						if(btemp>32768){
							 btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
							 btemp = btemp*(-1);
						}
						else{
							 btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
						}
						WAD_Value =btemp*1000;    //转化成mV				
							SW_EN_DYB_L;			
			
			}
					break ; 
			
			
			
			
				
				case ICC_VD2_AD :
					
			if(SysCtrl.StartMode!=PulseMode){
							 Select_Channel( ICC_VD2_AD, DYB );
							 vTaskDelay(10);
						for (i = 0; i < u16_sampcnt; i++)
					{
							AD7606ReadOneSample(&ad_retval1[0]);
							atemp = ad_retval1[AD_DYB];                //通道1（0-7）
							if(atemp>=32769){
								atemp=0;
							}			
							ad_retval[i] = (float)atemp;
							user_delayUs(10);
					}
					 vTaskDelay(10);
//					SortValue(ad_retvalID, u16_sampcnt);   //排序			
					btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
							if(btemp>32768){
								 btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
								 btemp = btemp*(-1);
							}
							else{
								 btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
							}
							WAD_Value =btemp*1000;    //转化成mV			
							SW_EN_DYB_L;						
			 }	
    else{
					 Select_Channel( ICC_VD2_AD, DYB );
				   vTaskDelay(10);
			              DelayTime	 =MachineCycle();	
						for (i = 0; i < 2000; i++)
					{
							AD7606ReadOneSample(&ad_retval1[0]);
							atemp = ad_retval1[AD_DYB];                //通道1（0-7）
							if(atemp>=32769){
								atemp=0;
							}			
							ad_retvalID[i] = (float)atemp;
							user_delayUs(DelayTime);
					}
					 vTaskDelay(10);
//					SortValue(ad_retvalID, u16_sampcnt);   //排序			
					btemp = DigitalFilter_f32ID(ad_retvalID, 2000, 0);
					if(btemp>32768){
						 btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
						 btemp = btemp*(-1);
					}
					else{
						 btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
					}
					WAD_Value =btemp*1000;    //转化成mV			
			  	SW_EN_DYB_L;			
		}			

			
					break ;				
		}

//    SysProg.SampTempFlag =0;  //当温度采样标志为10 则不允许温度采样	
	}
	else if (Board == WDB){
		
		switch(ch){
			
			case TC11:
					Select_Channel(  TC11 , WDB ); //
			    vTaskDelay(10);
//					for (i = 0; i < SAMPLECNT; i++)
//          {			
//						  SysProg.SampBitValue[i]= ReadAD_value(TOUT);
//						  if(SysProg.SampBitValue[i]<=32768){
//							SysProg.SampValue[i]  = (((32768-SysProg.SampBitValue[i])*AD8688_BENCHMARK_VOLTAGE*2500)/32768.0f)      ;
////							SysProg.SampValue[i]  = (-1)* SysProg.SampBitValue[i] ;
//						}
//						else{
//							SysProg.SampValue[i]  =(float)(((SysProg.SampBitValue[i]-32768)*AD8688_BENCHMARK_VOLTAGE*2500)/32768.0f)     ;
//						}	
//					}
//					WAD_Value = SortAndFilter(&SysProg.SampValue[0], SAMPLECNT);   //返回滤波后的AD值					
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[TOUT];                //通道1（0-7）
						if(atemp>=65534){
						  atemp=0;
						}					
						ad_retval[i] = (float)(atemp);
				}     	 
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV	
		    	SW_EN_WDB_L;
			break ;
				 
			case TC21:
			    Select_Channel( TC21, WDB ); //
			    vTaskDelay(10);
//					for (i = 0; i < SAMPLECNT; i++)
//          {			
//						  SysProg.SampBitValue[i]= ReadAD_value(TOUT);
//						  if(SysProg.SampBitValue[i]<=32768){
//							SysProg.SampValue[i]  = (((32768-SysProg.SampBitValue[i])*AD8688_BENCHMARK_VOLTAGE*2500)/32768.0f)      ;
////							SysProg.SampValue[i]  = (-1)* SysProg.SampBitValue[i] ;
//						}
//						else{
//							SysProg.SampValue[i]  =(float)(((SysProg.SampBitValue[i]-32768)*AD8688_BENCHMARK_VOLTAGE*2500)/32768.0f)     ;
//						}	
//					}
//					WAD_Value = SortAndFilter(&SysProg.SampValue[0], SAMPLECNT);   //返回滤波后的AD值				
					for (i = 0; i < u16_sampcnt; i++)
				{
						AD7606ReadOneSample(&ad_retval1[0]);
						atemp = ad_retval1[TOUT];                //通道1（0-7）
						if(atemp>=65534){
						  atemp=0;
						}					
						ad_retval[i] = (float)(atemp);
				}     	 
				btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
				if(btemp>32768){
				   btemp = (65535-btemp) * VOLTAGE_MINIMUM_SCALE;
				   btemp = btemp*(-1);
				}
				else{
		  	   btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV				
				}
				WAD_Value =btemp*1000;    //转化成mV						
		     	SW_EN_WDB_L;
			break ;			 
		
		}
				
	
	}
	
	return  WAD_Value ;
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
void SampTemp()
{	 
      float KBValueContinue;
	    uint32_t Kdata, Bdata;
			float AD_WDBValue[2]={0};
			for(int j=0 ;j<2;j++)
			{
				AD_WDBValue[j]=Read_AD_Value( j, WDB);
			}		
			SysProg.TEMP_VALUE[0] = AD_WDBValue[0];  //TC1 滤波后的值
			SysProg.TEMP_VALUE[1] = AD_WDBValue[1];	 //TC2 滤波后的值	

			CaliTempChanData();
			
//			SysProg.TRANS_TEMP_VALUE[0]=50;
//			SysProg.TRANS_TEMP_VALUE[1]=50;

			if (SysProg.TRANS_TEMP_VALUE[0]<270){
				KBValueContinue =  SysProg.TRANS_TEMP_VALUE[0]*10;
#if FeeTempKB==TRUE			
/*定义温度回检KB使用*/						
		KBValueContinue = FeedBackCalibration_Temp(KBValueContinue, Temp_1FEE);							
#endif
/*******************************************************************************************************/				
				SysData.AgingTestData.Station_TC_Temp[0].data = KBValueContinue;//SysProg.TRANS_TEMP_VALUE[0]*10;
			}
			else{
				SysData.AgingTestData.Station_TC_Temp[0].data = 999*10;
			}
			
			if (SysProg.TRANS_TEMP_VALUE[1]<270){
				KBValueContinue =  SysProg.TRANS_TEMP_VALUE[1]*10;
#if FeeTempKB==TRUE					
/*定义温度回检KB使用*/			
		KBValueContinue = FeedBackCalibration_Temp(KBValueContinue, Temp_2FEE);		
/*******************************************************************************************************/				
#endif				
				SysData.AgingTestData.Station_TC_Temp[1].data = KBValueContinue;//SysProg.TRANS_TEMP_VALUE[1]*10;
			}
			else{
				SysData.AgingTestData.Station_TC_Temp[1].data = 999*10;
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
void SampleALLChanData()
{
		float AD_DYBValue[8]={0};

if(SysCtrl.StartMode!=PulseMode){			
		AD_DYBValue[6]=Read_AD_Value( ICC_VD1_AD, DYB);
		AD_DYBValue[7]=Read_AD_Value( ICC_VD2_AD, DYB);	
		AD_DYBValue[4]=Read_AD_Value( ICC_VG1_AD, DYB);
		AD_DYBValue[5]=Read_AD_Value( ICC_VG2_AD, DYB);
		
		AD_DYBValue[0]=Read_AD_Value( VG1_AD, DYB);
		AD_DYBValue[1]=Read_AD_Value( VG2_AD, DYB);		
		AD_DYBValue[2]=Read_AD_Value( VD1_AD, DYB);
		AD_DYBValue[3]=Read_AD_Value( VD2_AD, DYB);
	
		SysProg.VOL_VALUE[0] = AD_DYBValue[0];  //VG1 滤波后的值
		SysProg.VOL_VALUE[1] = AD_DYBValue[1];  //VG2 滤波后的值
		SysProg.VOL_VALUE[2] = AD_DYBValue[2];  //VD1 滤波后的值
		SysProg.VOL_VALUE[3] = AD_DYBValue[3];	//VD2 滤波后的值
		
		SysProg.CURR_VALUE[0] = AD_DYBValue[4];  //IG1 滤波后的值
		SysProg.CURR_VALUE[1] = AD_DYBValue[5];  //IG2 滤波后的值
		SysProg.CURR_VALUE[2] = AD_DYBValue[6];  //ID1 滤波后的值
		SysProg.CURR_VALUE[3] = AD_DYBValue[7];	 //ID2 滤波后的值	
		return ;
}	
else{
	 if( SysCtrl.PulseModeFlag ==0 ){
			AD_DYBValue[6]=Read_AD_Value( ICC_VD1_AD, DYB);
		  SysCtrl.PulseModeFlag++;
		  AD_DYBValue[4]=Read_AD_Value( ICC_VG1_AD, DYB);
			AD_DYBValue[5]=Read_AD_Value( ICC_VG2_AD, DYB);		
			AD_DYBValue[0]=Read_AD_Value( VG1_AD, DYB);
			AD_DYBValue[1]=Read_AD_Value( VG2_AD, DYB);		
			AD_DYBValue[2]=Read_AD_Value( VD1_AD, DYB);
			AD_DYBValue[3]=Read_AD_Value( VD2_AD, DYB);
		 
			SysProg.VOL_VALUE[0] = AD_DYBValue[0];  //VG1 滤波后的值
			SysProg.VOL_VALUE[1] = AD_DYBValue[1];  //VG2 滤波后的值
			SysProg.VOL_VALUE[2] = AD_DYBValue[2];  //VD1 滤波后的值
			SysProg.VOL_VALUE[3] = AD_DYBValue[3];	//VD2 滤波后的值
			
			SysProg.CURR_VALUE[0] = AD_DYBValue[4];  //IG1 滤波后的值
			SysProg.CURR_VALUE[1] = AD_DYBValue[5];  //IG2 滤波后的值
			SysProg.CURR_VALUE[2] = AD_DYBValue[6];  //ID1 滤波后的值
//			SysProg.CURR_VALUE[3] = AD_DYBValue[7];	 //ID2 滤波后的值		 
			return ;		 
	 }
	 else if(SysCtrl.PulseModeFlag != 0 ){
			AD_DYBValue[7]=Read_AD_Value( ICC_VD2_AD, DYB);	
		  SysCtrl.PulseModeFlag=0;
		  AD_DYBValue[4]=Read_AD_Value( ICC_VG1_AD, DYB);
			AD_DYBValue[5]=Read_AD_Value( ICC_VG2_AD, DYB);		
			AD_DYBValue[0]=Read_AD_Value( VG1_AD, DYB);
			AD_DYBValue[1]=Read_AD_Value( VG2_AD, DYB);		
			AD_DYBValue[2]=Read_AD_Value( VD1_AD, DYB);
			AD_DYBValue[3]=Read_AD_Value( VD2_AD, DYB);	 
		 
			SysProg.VOL_VALUE[0] = AD_DYBValue[0];  //VG1 滤波后的值
			SysProg.VOL_VALUE[1] = AD_DYBValue[1];  //VG2 滤波后的值
			SysProg.VOL_VALUE[2] = AD_DYBValue[2];  //VD1 滤波后的值
			SysProg.VOL_VALUE[3] = AD_DYBValue[3];	//VD2 滤波后的值
			
			SysProg.CURR_VALUE[0] = AD_DYBValue[4];  //IG1 滤波后的值
			SysProg.CURR_VALUE[1] = AD_DYBValue[5];  //IG2 滤波后的值
//			SysProg.CURR_VALUE[2] = AD_DYBValue[6];  //ID1 滤波后的值
			SysProg.CURR_VALUE[3] = AD_DYBValue[7];	 //ID2 滤波后的值	 
		 	return ;
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
void SampleSingleChanData(uint16_t ChanNo)
{

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
void CaliALLData()
{
	CaliVolChanData();
	CaliCurrChanData();	

//	CaliTempChanData();
//	CaliBoardTempChanData();
	
	DataPackChanData();
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
void DataPackChanData()
{
  float SendValueF=0,SendVlaueI=0;
	uint32_t SendValue=0;
	float AdjuatV[4]={0};//VG1 VG2 VD1 VD2
	float AdjuatI[4]={0};//IG1 IG2 ID1 ID2
//	float ID_B=0;  //ID电流的补偿，VD每1V，ID补偿0.2mA
/***************************** 电压 ******************************/	
//【VG1】	

		SendValueF= SysProg.TRANS_VOL_VALUE[0];
		
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VG1FEE, 0);
	}
#endif	
	  SendValueF = SendValueF*10;	   //24.4.28改变上传倍率，再增大10倍
		if(SysProg.TRANS_VOL_VALUE[0]<0){
			SendValueF = SendValueF*(-1);
			SendValue =(uint32_t)SendValueF;
		  SendValue |=0x80000000;
	    SysData.AgingTestData.Station_VG_Voltage[0].data =  SendValue;	
		}
		else{
			SendValue =(uint32_t)SendValueF;		
	    SysData.AgingTestData.Station_VG_Voltage[0].data =  SendValue;				
		}
 

//【VG2】	
		SendValueF= SysProg.TRANS_VOL_VALUE[1];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VG2FEE, 0);
	}
#endif				  //VG1、VG2
	  SendValueF = SendValueF*10;	   //24.4.28改变上传倍率，再增大10倍                                                                       
		if(SysProg.TRANS_VOL_VALUE[1]<0){
			SendValueF = SendValueF*(-1);
			SendValue =(uint32_t)SendValueF;
		  SendValue |=0x80000000;
	    SysData.AgingTestData.Station_VG_Voltage[1].data =  SendValue;	
		}
		else{
			SendValue =(uint32_t)SendValueF;		
	    SysData.AgingTestData.Station_VG_Voltage[1].data =  SendValue;				
		}
		
		
//【VD1】
	SendValueF =SysProg.TRANS_VOL_VALUE[2];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VD1FEE, 0);
	}
#endif	
		  SendValueF = SendValueF*10;	   //24.4.28改变上传倍率，再增大10倍
  SendValue =(uint32_t)SendValueF;	
	SysData.AgingTestData.Station_VD_Voltage[0].data =  SendValue;//SysProg.TRANS_VOL_VALUE[2];

//【VD2】	
	SendValueF =SysProg.TRANS_VOL_VALUE[3];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VD2FEE, 0);
	}
#endif	
		  SendValueF = SendValueF*10;	   //24.4.28改变上传倍率，再增大10倍
  SendValue =(uint32_t)SendValueF;	
	SysData.AgingTestData.Station_VD_Voltage[1].data =  SendValue;//SysProg.TRANS_VOL_VALUE[3];//VD1 VD2
	

/***************************** 电流 ******************************/
//【IG1】
	if(SysProg.TRANS_CURR_VALUE[0] < 0)//负
	{
		
		SendVlaueI= SysProg.TRANS_CURR_VALUE[0];
	#if ENABLE_KB == TRUE
		if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
		{
			SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG10FEE, 0);
		}
	#endif	
	  SendVlaueI = SendVlaueI*10;	   //24.4.28改变上传倍率，再增大10倍		
		if(SysProg.TRANS_CURR_VALUE[0]<0){
			SendVlaueI = SendVlaueI*(-1);
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
//	    SysData.AgingTestData.Station_IG_Curr[0].data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI;			
			SendValue =(uint32_t)SendVlaueI;		
//	    SysData.AgingTestData.Station_IG_Curr[0].data =  SendValue;				
		}				
	}
	else{   //IGZ
					SendVlaueI= SysProg.TRANS_CURR_VALUE[0];
			#if ENABLE_KB == TRUE
				if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
				{
					SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG1FEE, 0);
				}
			#endif	
	  SendVlaueI = SendVlaueI*10;	   //24.4.28改变上传倍率，再增大10倍						
					if(SysProg.TRANS_CURR_VALUE[0]<0){
						SendVlaueI = SendVlaueI*(-1);
						SendValue =(uint32_t)SendVlaueI;
						SendValue |=0x80000000;
//						SysData.AgingTestData.Station_IG_Curr[0].data =  SendValue;	
					}
					else{
						SendVlaueI =SendVlaueI;			
						SendValue =(uint32_t)SendVlaueI;		
//						SysData.AgingTestData.Station_IG_Curr[0].data =  SendValue;				
					}		

	}	 
	  if((SendValue&0x7fffffff)<50){
		 SysData.AgingTestData.Station_IG_Curr[0].data =0;
		}
		else{
	   SysData.AgingTestData.Station_IG_Curr[0].data = SendValue;			
		}	
	
	
//【IG2】	
	if(SysProg.TRANS_CURR_VALUE[1] < 0)//负
	{
		
		SendVlaueI= SysProg.TRANS_CURR_VALUE[1];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG20FEE, 0);
	}
#endif		
		  SendVlaueI = SendVlaueI*10;	   //24.4.28改变上传倍率，再增大10倍		
		if(SysProg.TRANS_CURR_VALUE[1]<0){
			SendVlaueI = SendVlaueI*(-1);
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
//	    SysData.AgingTestData.Station_IG_Curr[1].data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI;			
			SendValue =(uint32_t)SendVlaueI;		
//	    SysData.AgingTestData.Station_IG_Curr[1].data =  SendValue;				
		}			
//		SendVlaueI= SysProg.TRANS_CURR_VALUE[1]*(-1);
//#if ENABLE_KB == TRUE
//	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
//	{
//		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG2FEE, 0);
//	}
//#endif		
//		SendValue =(uint32_t)SendVlaueI;
//		SendValue |=0x80000000;
//	  SysData.AgingTestData.Station_IG_Curr[1].data =  SendValue; 
	}
	else{ //IGZ
				SendVlaueI= SysProg.TRANS_CURR_VALUE[1];
		#if ENABLE_KB == TRUE
			if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
			{
				SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG2FEE, 0);
			}
		#endif		
				  SendVlaueI = SendVlaueI*10;	   //24.4.28改变上传倍率，再增大10倍		
				if(SysProg.TRANS_CURR_VALUE[1]<0){
					SendVlaueI = SendVlaueI*(-1);
					SendValue =(uint32_t)SendVlaueI;
					SendValue |=0x80000000;
//					SysData.AgingTestData.Station_IG_Curr[1].data =  SendValue;	
				}
				else{
					SendVlaueI =SendVlaueI;			
					SendValue =(uint32_t)SendVlaueI;		
//					SysData.AgingTestData.Station_IG_Curr[1].data =  SendValue;				
				}
				
//		SendVlaueI= SysProg.TRANS_CURR_VALUE[1];
//#if ENABLE_KB == TRUE
//	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
//	{
//		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG2FEE, 0);
//	}
//#endif		
//		SendValue =(uint32_t)SendVlaueI;
//	  SysData.AgingTestData.Station_IG_Curr[1].data = SendValue;//( ((uint32_t )SysProg.TRANS_CURR_VALUE[0]) ); 		
////	 SysData.AgingTestData.Station_IG_Curr[1].data = ( ((uint32_t )SysProg.TRANS_CURR_VALUE[1]) ); 
	}	
	  if((SendValue&0x7fffffff)<50){
		 SysData.AgingTestData.Station_IG_Curr[1].data =0;
		}
		else{
	   SysData.AgingTestData.Station_IG_Curr[1].data = SendValue;			
		}	
	
//【ID1】	

	
	
		SendVlaueI= SysProg.TRANS_CURR_VALUE[2];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_ID1FEE, 0);
	}
#endif

#if CompensationIDKB==TRUE
	  SendVlaueI = Compensation_ID_KB(SendVlaueI, ID_1Compensation);
#endif	
		  SendVlaueI = SendVlaueI*10;	   //24.4.28改变上传倍率，再增大10倍		
		if(SysProg.TRANS_CURR_VALUE[2]<0){
			SendVlaueI = SendVlaueI*(-1);
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    	
		}
		else{
			SendVlaueI =SendVlaueI;
			SendValue =(uint32_t)SendVlaueI;		
//	    SysData.AgingTestData.Station_ID_Curr[0].data =  SendValue;				
		}
	  if( ( SysProg.TRANS_CURR_VALUE[2] )<30){
		 SysData.AgingTestData.Station_ID_Curr[0].data =0;
		}
		else{
	   SysData.AgingTestData.Station_ID_Curr[0].data = SendValue;			
		}			
	
//【ID2】	
		
		SendVlaueI= SysProg.TRANS_CURR_VALUE[3];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_ID2FEE, 0);
	}
#endif	
#if CompensationIDKB==TRUE
	  SendVlaueI = Compensation_ID_KB(SendVlaueI, ID_2Compensation);
#endif		
		  SendVlaueI = SendVlaueI*10;	   //24.4.28改变上传倍率，再增大10倍		

		if(SysProg.TRANS_CURR_VALUE[3]<0){
			SendVlaueI = SendVlaueI*(-1);
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
		}
		else{
			SendVlaueI =SendVlaueI;			
			SendValue =(uint32_t)SendVlaueI;					
		}				
//	  if((SendValue&0x7fffffff)<500){
	  if( ( SysProg.TRANS_CURR_VALUE[3] )<30){
		 SysData.AgingTestData.Station_ID_Curr[1].data =0;
		}
		else{
	   SysData.AgingTestData.Station_ID_Curr[1].data = SendValue;			
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
void CaliVolChanData()
{	
	/*当前计算结果单位为Mv*/
   SysProg.TRANS_VOL_VALUE[0] = SysProg.VOL_VALUE[0]*(-5) ;
	 SysProg.TRANS_VOL_VALUE[1] = SysProg.VOL_VALUE[1]*(-5) ;//VG1 VG2计算的实际电压值
	 SysProg.TRANS_VOL_VALUE[2] = SysProg.VOL_VALUE[2]/0.04 ;
	 SysProg.TRANS_VOL_VALUE[3] = SysProg.VOL_VALUE[3]/0.04 ;//VD1 VD2计算的实际电压值		
	
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
void CaliCurrChanData()
{
   SysProg.TRANS_CURR_VALUE[0] = SysProg.CURR_VALUE[0] ;
	 SysProg.TRANS_CURR_VALUE[1] = SysProg.CURR_VALUE[1] ;   //IG1 IG2计算的实际电压值
	 SysProg.TRANS_CURR_VALUE[2] = SysProg.CURR_VALUE[2]*10 ;
	 SysProg.TRANS_CURR_VALUE[3] = SysProg.CURR_VALUE[3]*10 ;//ID1 ID2计算的实际电压值	

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
void CaliTempChanData()
{
	
	volatile float res, R, TEM;
	for(int i=0;i<2;i++){
		res = SysProg.TEMP_VALUE[i] / (50400.0) + 0.09804;
	  R = 5000 * res / (5 - res);
    TEM = (R - 100) / 0.39;		
	  SysProg.TEMP_VALUE[i] = TEM ;	
	}
	
   SysProg.TRANS_TEMP_VALUE[0] = SysProg.TEMP_VALUE[0] ;
	 SysProg.TRANS_TEMP_VALUE[1] = SysProg.TEMP_VALUE[1] ;   //VG1 VG2计算的实际电压值		
	
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
void CaliBoardTempChanData()
{

}
/******************************************************************
函数名称：SortAndFilter(float *dpoint,uint16_t num)
函数功能：
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float SortAndFilter(float *dpoint, uint16_t num)
{
	uint16_t ptemp, jtemp;
	float addtemp, tempcount;
	// 排序
	for (ptemp = 0; ptemp < num; ptemp++)
	{
		for (jtemp = ptemp + 1; jtemp < num; jtemp++)
		{
			if (dpoint[ptemp] > dpoint[jtemp])
			{
				tempcount = dpoint[ptemp];
				dpoint[ptemp] = dpoint[jtemp];
				dpoint[jtemp] = tempcount;
			}
		}
	}
	// 累加
	addtemp = 0;
	if (num <= 10) // 小于10的就去掉最大和最小值
	{
		for (ptemp = 1; ptemp < num - 1; ptemp++)
		{
			addtemp += dpoint[ptemp]; // 求和
		}
		addtemp = addtemp / (num - 2);
	}
	else if (num <= 30) // 如果是30个，就去掉前2和后2
	{
		for (ptemp = 2; ptemp < num - 2; ptemp++)
		{
			addtemp += dpoint[ptemp]; // 求和
		}
		addtemp = addtemp / (num - 4);
	}
	return (addtemp);
}
/******************************************************************
函数名称：SortAndFilter(uint32_t *dpoint,uint16_t num)
函数功能：
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint8_t AbsF(float DataA,float DataB,float DataD)
{
   float Data_S,Data_D,Data_A;
	
	Data_A=fabs(DataD)*0.8;
	Data_S=fabs(DataA);
	Data_D=fabs(DataB);
   if(Data_S>=Data_D)
   {
	  if((Data_S-Data_D)>=Data_A)
	  {
	     return 0;
	  }
	  else
      {
	     return 1;
	  }     
   }
   else
   {
      if((Data_D-Data_S)>=Data_A)
	  {
	     return 0;
	  }
	  else
      {
	     return 1;
	  } 
   }
}


/******************************************************************
函数名称：
函数功能：
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void SelectAgingModeSamp(void)
{
	 switch(SysCtrl.StartMode)//(SysData.AgingTestParameters.AgingMode.data)
		{
			case ConstantCurrentMode :  //恒流模式
					SampTemp();
					SampleALLChanData();
					CaliALLData();	
					RegulationMode();			
				break;
			
			case ConstantPowerMode :    //恒功率模式
					SampTemp();
					SampleALLChanData();
					CaliALLData();	
					RegulationMode();							
			break;
			
			case PulseMode :            //脉宽模式
					SampTemp();
					SampleALLChanData();
					CaliALLData();				
			break;		
			
			case ConstantVoltageMode :  //恒压模式
					SampTemp();
					SampleALLChanData();
					CaliALLData();
			break;			
		}

}


/******************************************************************
函数名称：
函数功能：
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void RegulationMode()
{
	float AdjustCur=0,AdjustPower=0;
  float DifferenceValue[2]={0};           //差值
 switch(SysCtrl.StartMode)
 {
	 
		case ConstantCurrentMode :  //【恒流模式】
	  	
		AdjustCur = (SysData.AgingTestParameters.SetCurr.data & 0x7fffffff)/100.0	;	   //单位A

/* VG1 */
		  if( AdjustCur >(SysData.AgingTestData.Station_ID_Curr[0].data/1000.0) ){
			  DifferenceValue[0]	= AdjustCur-(SysData.AgingTestData.Station_ID_Curr[0].data/1000.0);
				SysCtrl.PlusOrMinusFlag[0]=10;  // 为正需要加电压
				
			}
      else{
			  DifferenceValue[0]	= (SysData.AgingTestData.Station_ID_Curr[0].data/1000.0)-AdjustCur;
				SysCtrl.PlusOrMinusFlag[0]=20; // 为负需要降电压
			}
      RegulationVol(DifferenceValue[0],VG1_DA);
			
/* VG2 */
			if( AdjustCur >(SysData.AgingTestData.Station_ID_Curr[1].data/1000.0) ){
			  DifferenceValue[1]	= AdjustCur-(SysData.AgingTestData.Station_ID_Curr[1].data/1000.0);
				SysCtrl.PlusOrMinusFlag[1]=10;  // 为正需要加电压				
			}
      else{
			  DifferenceValue[1]	= (SysData.AgingTestData.Station_ID_Curr[1].data/1000.0)-AdjustCur;
				SysCtrl.PlusOrMinusFlag[1]=20; // 为负需要降电压
			}
      RegulationVol(DifferenceValue[1],VG2_DA);		
			
			break;
		
		case ConstantPowerMode :    //【恒功率模式】
			
		AdjustPower = (SysData.AgingTestParameters.SetPower.data & 0x7fffffff)/100.0	;	   //单位W
	  AdjustCur  =  AdjustPower / ((SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0);  //算出恒流值


/* VG1 */
		  if( AdjustCur >(SysData.AgingTestData.Station_ID_Curr[0].data/1000.0) ){
			  DifferenceValue[0]	= AdjustCur-(SysData.AgingTestData.Station_ID_Curr[0].data/1000.0);
				SysCtrl.PlusOrMinusFlag[0]=10;  // 为正需要加电压
				
			}
      else{
			  DifferenceValue[0]	= (SysData.AgingTestData.Station_ID_Curr[0].data/1000.0)-AdjustCur;
				SysCtrl.PlusOrMinusFlag[0]=20; // 为负需要降电压
			}
      RegulationVol(DifferenceValue[0],VG1_DA);
			
/* VG2 */
			if( AdjustCur >(SysData.AgingTestData.Station_ID_Curr[1].data/1000.0) ){
			  DifferenceValue[1]	= AdjustCur-(SysData.AgingTestData.Station_ID_Curr[1].data/1000.0);
				SysCtrl.PlusOrMinusFlag[1]=10;  // 为正需要加电压				
			}
      else{
			  DifferenceValue[1]	= (SysData.AgingTestData.Station_ID_Curr[1].data/1000.0)-AdjustCur;
				SysCtrl.PlusOrMinusFlag[1]=20; // 为负需要降电压
			}
      RegulationVol(DifferenceValue[1],VG2_DA);		
			
			break; 
		
		default :
			break ;
 }

}


/******************************************************************
函数名称：调节电压
函数功能：通过对VG电压的调节使得达到恒流或恒功率模式
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void RegulationVol(float DifferenceValue, uint16_t VolSwitch)
{
	uint16_t SignFlag=0;  // 符号标志位 10为正 20为负
	float SetVol=0,SetVol_VG1=0,SetVol_VG2=0;

  int SetVGVol=0;	
	
	
	if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
		SignFlag=20;  //为负 
		SetVGVol=(SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)*10;       //V放大1000倍
		SetVGVol =(-1)*SetVGVol;
	}
	else{
		SignFlag=10;	//为正	
		SetVGVol=(SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)*10;
	}

	
	if(	SysCtrl.ADjustVolCnt ==0){
		SysCtrl.ADjustVol[0] =SetVGVol;  //V放大1000倍
		SysCtrl.ADjustVol[1] =SetVGVol;			
		SysCtrl.ADjustVolCnt++;
	}
	else{
	
	}	

/* 
** VolSwitch 输入参数说明：
** VG1_DA       ， VG2_DA        --- 用来代表恒流的起调  	
*/	
	switch(VolSwitch)
  {
	  case VG1_DA:
			if(SysCtrl.PlusOrMinusFlag[0]==10){  // 为正需要加电压
				
			   if(DifferenceValue>0.5){
						SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]+50;									
						}

				 else if( (DifferenceValue>=0.1) && (DifferenceValue<=0.4) ){
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]+10;				 
				 }
				 else if( (DifferenceValue<0.1)&&(DifferenceValue>0.05) ){
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]+2;
			 			 
				 }
				 else if( (DifferenceValue<0.05)&&(DifferenceValue>0) ){
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]+1;			 			 
				 }				 
			 }
		 
			
			else if( SysCtrl.PlusOrMinusFlag[0]==20 ){// 为负需要降电压
			
			   if(DifferenceValue>0.5){
					  if(SignFlag==10)//为正
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]-50;
				 }
				 else if( (DifferenceValue>=0.1) && (DifferenceValue<=0.4) ){
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]-10;					 
				 }
				 else if( (DifferenceValue<0.1)&&(DifferenceValue>0.05) ){
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]-2;
			 			 
				 }
				 else if( (DifferenceValue<0.05)&&(DifferenceValue>0) ){
				    SysCtrl.ADjustVol[0] = SysCtrl.ADjustVol[0]-1;
			 			 
				 }		

			}

			SetVol = SysCtrl.ADjustVol[0]/1000.0;
			if( (SysCtrl.ADjustVol[0]<500)||(SysCtrl.ADjustVol[0]>(-500)) ){
					ConCLR(RESET);
					vTaskDelay(10);
			}
			else{
			    ConCLR(SET);
			}		
			
#if ENABLE_KB == TRUE

		      SetVol = ( ErrorCompensation_v(SetVol*1000, VOL_VG1CON, 0) )/1000; //Mv进去 最后转化成V
#else			
		    	SetVol = SetVol
#endif		
			SGM5349_DA( VG1_DA ,SetVol ,DYB );   //发调节VG1			
			break;
			
	  case VG2_DA:
		
			if(SysCtrl.PlusOrMinusFlag[1]==10){  // 为正需要加电压
				
			   if(DifferenceValue>0.5){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]+50;							
						}
		
				 else if( (DifferenceValue>=0.1) && (DifferenceValue<=0.4) ){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]+10;					 
				 }
				 else if( (DifferenceValue<0.1)&&(DifferenceValue>0.05) ){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]+2;
			 			 
				 }
				 else if( (DifferenceValue<0.05)&&(DifferenceValue>0) ){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]+1;
			 			 
				 }	 
		 
			}
			else if( SysCtrl.PlusOrMinusFlag[1]==20 ){// 为负需要降电压
			
			   if(DifferenceValue>0.5){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]-50;
				 }
				 else if( (DifferenceValue>=0.1) && (DifferenceValue<=0.4) ){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]-10;					 
				 }
				 else if( (DifferenceValue<0.1)&&(DifferenceValue>0.05) ){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]-2;
			 			 
				 }
				 else if( (DifferenceValue<0.05)&&(DifferenceValue>0) ){
				    SysCtrl.ADjustVol[1] = SysCtrl.ADjustVol[1]-1;
			 			 
				 }			

			}		
			
			SetVol = SysCtrl.ADjustVol[1]/1000.0;
			if( (SysCtrl.ADjustVol[1]<500)||(SysCtrl.ADjustVol[1]>(-500)) ){
					ConCLR(RESET);
					vTaskDelay(10);
			}
			else{
			    ConCLR(SET);
			}
#if ENABLE_KB == TRUE

		      SetVol = ( ErrorCompensation_v(SetVol*1000, VOL_VG2CON, 0) )/1000; //Mv进去 最后转化成V
#else			
		    	SetVol = SetVol
#endif					
			SGM5349_DA( VG2_DA ,SetVol ,DYB );   //发调节VG1				
			break;
				
    default :
      break;			
	}
  
	
	
  
}



/******************************************************************
函数名称：
函数功能：计算周期
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
int MachineCycle(void)
{
	uint32_t  TotalPeriod=0;
	int DelayTime=0;
	//  50*1000 /5
 	TotalPeriod = (SysData.AgingTestParameters.PulseWidth[0].data*1000)/(SysData.AgingTestParameters.DutyCycle[0].data);
	DelayTime = TotalPeriod/2000;
	return DelayTime;
}

