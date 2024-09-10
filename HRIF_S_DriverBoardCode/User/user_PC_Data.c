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
#include "main.h"
//extern const uint8_t CONCRAL[MAX_VOLSTATE];
//extern const uint8_t CLR[MAX_VOLSTATE];
/******************************************************************
函数名称：
函数功能:流程控制运行
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void ProgCtrlRun(void)
{
	uint8_t i;
	/*如果【PGEST流程文件存在】为1 ”与“ 【PGRN程序运行】为0 均在则执行，否则跳出if*/
	if ((SysProg.bflag.bit.PGEST == 1) && (SysProg.bflag.bit.PGRN == 0))   
	{
		 SysProg.bflag.bit.PGST = 1;         //设置【PGST程序开始】为1
//		for (i = 0; i < DEVICE_MAX; i++)
//		{
//			SysProg.Ctrl_AGE_RUN[i] = 1;       // DEVICE_MAX-->最大工位数,   设置 控制老化运行【4工位】标志为1 
//		}
		for (i = 0; i < 2; i++)
		{
			SysProg.CtrlAGE_RUN[i] = 1;       // DEVICE_MAX-->最大工位数,   设置 控制老化运行【4工位】标志为1 
		}		
	}
}
/******************************************************************
函数名称：
函数功能:流程控制暂停
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void ProgCtrlPause(void)
{
	uint16_t i;
	/*如果【PGRN程序运行】标志为1，则进入IF语句*/
	if (SysProg.bflag.bit.PGRN == 1)
	{
		VD_PRE_H;
		ConCLR(RESET);          //保护禁能
		AgingStateConfig(AGING_STATUS_PAUSE);  //调用老化状态配置函数   将其设置为【暂停】
	}
}
/******************************************************************
函数名称：
函数功能:流程控制恢复
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void ProgCtrlRecover(void)
{
	uint8_t i;
	/*如果系统老化状态被设置为老化“暂停” 与 系统【PGRN程序运行】标志设置为0  均满足则执行if*/
	if ((SysData.AgingTestData.AgingStatus == AGING_STATUS_PAUSE) && (SysProg.bflag.bit.PGRN == 0))
	{
		if (SysProg.bflag.bit.PGEST == 1) //如果【PGEST流程文件存在】标志设置为1
		{
			for (i = 0; i < 2; i++)
			{
//				SysProg.Ctrl_AGE_RUN[i] = 1;  //依次将【系统程序.控制老化运行】工位标志设置为1
				SysProg.CtrlAGE_RUN[i] = 1; 
			}
		}
		AgingStateConfig(AGING_STATUS_RUN);//调用老化状态配置函数，设置状态为【老化中】
	}
}
/******************************************************************
函数名称：
函数功能:流程控制停止
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void ProgCtrlSTOP(void)
{
	uint8_t i;
	AgingStateConfig(AGING_STATUS_MANUAL_STOP);
}


/******************************************************************
函数名称：
函数功能:流程烘箱温度，达到后才开始流程老化 23.10.18日添加
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void ProgCtrlPowerStart(void)
{
	AgingStateConfig(WAITING_POWER_RISE);
}

/******************************************************************
函数名称：
函数功能:老化状态配置
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
extern uint8_t FSD_cnt;
void AgingStateConfig(AgingStatusCOM state)
{
	int InitiatingVolCheck=0;
	switch (state)
	{
	case AGING_STATUS_WAIT://【等待】
	{

		Fpga_Disable(0);
    /*TEMP_STANDBY_MODE =2-->待机模式*/
		StationTempCtrl(TEMP_STANDBY_MODE);   
	
  	SysProg.bflag.all = 0;
		ClearAgingVarParam();  //清除老化结构体参数
		SysData.AgingTestData.ProRunTimeCnt.data = 0;//系统数据.老化测试数据.老化时间.数据
		memset(&SysData.AgingTestParameters, 0, sizeof(SysData.AgingTestParameters));

   /*系统控制.程序模式设置为正常模式*/
		SysCtrl.ProgType = NormalStation;  
   /*系统控制.以太网连接标志以及计数？设置为0*/		
		SysCtrl.EthConnectFlag = 0;
		SysCtrl.EthConnectCnt = 0;
		SysCtrl.StartMode = 10;
		
	  SysCtrl.PlusOrMinusFlag[0]=0;
		SysCtrl.PlusOrMinusFlag[1]=0;
		SysCtrl.ADjustVolCnt =0;
		
		HRIF_S_Fpga_Init();		
	}

	break;
	
	/*AGING_STATUS_READY --> 就绪*/
	case AGING_STATUS_READY://【就绪】                     
	{
	  Init_PID();    // 初始化温控PID
		SysCtrl.ProgType = NormalStation;  // 设置模式为正常模式
//		ConCLR(RESET);
		ClearAgingVarParam();  //清除老化结构体
		SysData.AgingTestData.ProRunTimeCnt.data = 0;  //老化时间清零
		SysProg.bflag.bit.PGEST = 1;                   //PGEST 流程文件存在 
		
		if(SysData.AgingTestParameters.AgingMode.data==PulseMode){
			VD_PRE_H;
	  	ConCLR(RESET);          //保护禁能
			InitiatingVDVol_Test();
		}
	}
	break;
	
	/*AGING_STATUS_RUN-->老化中*/
	case AGING_STATUS_RUN://【老化中】
	{

  	/* 初始化电压以及继电器 */
		VD_PRE_H;
		ConCLR(RESET);          //保护禁能
//		ConRelayInit();					//继电器拉高														
		
		switch(SysData.AgingTestParameters.AgingMode.data)
		{
			
			case ConstantCurrentMode :  //【恒流模式】
			/*************************************************************************************************************************************************
			说明： 恒流模式下，1.正常下发VD VG 设置值 2.读取ID的电流值  3，ID和设置电流值比较 4. 调节对应的ID1-VG1 ID2-VG2使得ID=~设置电流 
			*************************************************************************************************************************************************/
					vTaskDelay(20);
          InitiatingVolCheck =InitiatingVol();                            //发起电压
			    if(InitiatingVolCheck==SUCCESS){
						vTaskDelay(200);			
						ConCLR(SET); //                             ---保护使能	
						vTaskDelay(200);
					}
					else{
						vTaskDelay(20);
						ConCLR(SET); //                             ---保护使能		
						SysProg.ERRORFLAG = 0xfB;				
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;						
					}					
					SysCtrl.StartMode = ConstantCurrentMode;    // 模式设置为恒流模式
				break;
			
			case ConstantPowerMode :  //【恒功率模式】
			/*************************************************************************************************************************************************
			说明： 恒功率模式，1.正常下发VD VG 设置值 2.算出I值 I=P/U 后续转化成恒流模式 ；
			读取IG的电流值  3，IG和I值比较 4. 调节对应的IG1-VG1 IG2-VG2使得IG=~I 
			*************************************************************************************************************************************************/
					vTaskDelay(20);	
          InitiatingVolCheck =InitiatingVol();                            //发起电压
			    if(InitiatingVolCheck==SUCCESS){ 
						ConCLR(SET); //                             ---保护使能	
						vTaskDelay(200);
					}
					else{
						vTaskDelay(20);
						ConCLR(SET); //                             ---保护使能		
						SysProg.ERRORFLAG = 0xfB;			
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;						
					}
					SysCtrl.StartMode = ConstantPowerMode;      // 模式设置为恒功率模式
				break;
			
			case PulseMode :  //【脉宽模式】
			/*************************************************************************************************************************************************
			说明： 脉宽模式，1.正常下发VD VG 设置值  2. 发起PWM波 
			*************************************************************************************************************************************************/
					vTaskDelay(20);	
			    InitiatingVolCheck =InitiatingVol_Test();
          if(InitiatingVolCheck==SUCCESS){
						vTaskDelay(200);
						Fpga_Enable(0);                             // FPGA输出波形	
//					  SysCtrl.StartMode = PulseMode;              // 模式设置为脉宽模式
				    HRIF_S_Fpga_USE();                          // 发脉宽
						vTaskDelay(200);
						ConCLR(RESET); //                             ---保护使能			
//						vTaskDelay(20);
//            VG1_CLR_H						
					}
					else{
						vTaskDelay(20);
						ConCLR(SET); //                             ---保护使能		
						SysProg.ERRORFLAG = 0xfB;				
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;						
					}
				  SysCtrl.StartMode = PulseMode;              // 模式设置为脉宽模式		
				break;		
			
			case ConstantVoltageMode :  //【恒压模式】
			/*************************************************************************************************************************************************
			说明： 恒压模式，1.正常下发VD VG 设置值  2. 发起PWM波 
			*************************************************************************************************************************************************/  
					vTaskDelay(20);		
          InitiatingVolCheck = InitiatingVol();	//发起电压	
			    if(InitiatingVolCheck==SUCCESS){                                      
						vTaskDelay(200);
#if CLRCon==TRUE						
						ConCLR(SET); //                             ---保护使能			
#else						
						VD1_CLR_H 
#endif						
						HRIF_S_Fpga_USE();
					}
					else{
						vTaskDelay(20);
#if CLRCon==TRUE								
						ConCLR(SET);
#else						
						VD1_CLR_H 						
#endif								
						SysProg.ERRORFLAG = 0xfB;
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;
					}	
					SysCtrl.StartMode = ConstantVoltageMode;    // 模式设置为恒压模式					
				break;  			
		}
//		vTaskDelay(500);
		SysProg.bflag.bit.PGRN = 1;
	}
	break;
	
  /*【暂停】*/	
	case AGING_STATUS_PAUSE:   
	{
		StationTempCtrl(TEMP_STANDBY_MODE);    //调用平台温度控制程序函数， 将其设置为待机模式
		ClearAgingVarParam();                  //调用清除老化结构体参数清零结构体
		SysProg.bflag.bit.PGRN = 0;            //【PGRN程序运行】标志设置为0
	}
	break;
	
	/*【完成老化】*/
	case AGING_STATUS_AGING_FINISH:
	{
		StationTempCtrl(TEMP_STANDBY_MODE);
		HRIF_S_Fpga_Init();
		vTaskDelay(100);
		ConCLR(RESET); //必须先关闭硬件保护使能，才可以下发0，否则会触发硬件保护
	  vTaskDelay(20);	
//		VD_PRE_L;		
    StopVol();
	  vTaskDelay(20);		
//		VD_PRE_L;	
#if CLRCon==TRUE			
		ConCLR(SET);
#else						
						VD1_CLR_H 		
#endif		
		vTaskDelay(20);				
		Fpga_Disable(0);
		ClearAgingVarParam();
		vTaskDelay(5);
		SysProg.bflag.bit.PGRN = 0;
		memset(&SysData.AgingTestParameters, 0, sizeof(SysData.AgingTestParameters));
		SysProg.bflag.bit.PGEST = 0;		
	}
	break;
	
	case AGING_STATUS_ERROR_STOP:
	{

		StationTempCtrl(TEMP_STANDBY_MODE);		
		HRIF_S_Fpga_Init();
		vTaskDelay(100);				
		ConCLR(RESET); 
		vTaskDelay(20);		
//		VD_PRE_L;		
    StopVol();
	  vTaskDelay(20);		
//		VD_PRE_L;		
#if CLRCon==TRUE			
		ConCLR(SET);
#else						
						VD1_CLR_H 		
#endif		
		vTaskDelay(20);					
		Fpga_Disable(0);
		vTaskDelay(500);
//		ClearAgingVarParam();
//		StationTempCtrl(TEMP_COOL_MODE);
		ClearAgingVarParamError();
		SysProg.bflag.bit.PGRN = 0;

	}
	break;
	
	case AGING_STATUS_MANUAL_STOP:      //【手动停止】
	{		

		StationTempCtrl(TEMP_STANDBY_MODE);
		HRIF_S_Fpga_Init();
		vTaskDelay(100);		
		ConCLR(RESET); //必须先关闭硬件保护使能，才可以下发0，否则会触发硬件保护
	  vTaskDelay(20);	
//		VD_PRE_L;			
    StopVol();
	  vTaskDelay(20);		
//		VD_PRE_L;		
#if CLRCon==TRUE			
		ConCLR(SET);
#else						
						VD1_CLR_H 		
#endif		
		vTaskDelay(20);				
		Fpga_Disable(0);
		ClearAgingVarParam();
		vTaskDelay(5);
		SysProg.bflag.bit.PGRN = 0;
		memset(&SysData.AgingTestParameters, 0, sizeof(SysData.AgingTestParameters));
		SysProg.bflag.bit.PGEST = 0;

	}
	break;
	case WAITING_POWER_RISE :
		vTaskDelay(200);
		SysProg.PowerUpCnt++;
	break;
	default:
		break;
	}
	SysData.AgingTestData.AgingStatus = state;
}


/******************************************************************
函数名称:StopVol
函数功能:停止电压 VD VG
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void StopVol(void)
{
	SysProg.StopPowerCnt[0] = 0;
	SysProg.StopPowerCnt[1] = 0;	
	if(SysData.AgingTestParameters.FirstStopVTime.data==0){      //【先停VD】
	 SGM5349_DA( VD1_DA ,0 ,DYB);
	 SGM5349_DA( VD2_DA ,0 ,DYB);	
	 vTaskDelay(1000);
	 if(SysData.AgingTestParameters.AgingMode.data !=PulseMode){
			 while(1){
				 if( (CheckStopVol(VD1_DA)==SUCCESS) && (CheckStopVol(VD2_DA)==SUCCESS) )
				 { break ; }
				 else{
						SGM5349_DA( VD1_DA ,0 ,DYB);
						SGM5349_DA( VD2_DA ,0 ,DYB);	
						vTaskDelay(200);
						SysProg.StopPowerCnt[0] ++;		 
						if( SysProg.StopPowerCnt[0] > 50){  //5*4000=20 000 ms
							 break;
						}
					 
				 }
			 }
				 SGM5349_DA( VG1_DA ,0 ,DYB );
				 SGM5349_DA( VG2_DA ,0 ,DYB );	 
				 vTaskDelay(300);
				 while(1){
					 if( (CheckStopVol(VG1_DA)==SUCCESS) && (CheckStopVol(VG2_DA)==SUCCESS) )
					 { break ; }
					 else{
								SGM5349_DA( VG1_DA ,0 ,DYB );
								SGM5349_DA( VG2_DA ,0 ,DYB );				 
								vTaskDelay(200);
								SysProg.StopPowerCnt[1] ++;		 
								if( SysProg.StopPowerCnt[1] > 50){  //5*2000=10 000 ms
									 break;
								 }
					 }	 
				 
				 }			 			 
	 }
//	 while(1){
//	   if( (CheckStopVol(VD1_DA)==SUCCESS) && (CheckStopVol(VD1_DA)==SUCCESS) )
//	   { break ; }
//		 else{
//				SGM5349_DA( VD1_DA ,0 ,DYB);
//				SGM5349_DA( VD2_DA ,0 ,DYB);	
//				vTaskDelay(400);
//				SysProg.StopPowerCnt[0] ++;		 
//				if( SysProg.StopPowerCnt[0] > 50){  //5*4000=20 000 ms
//					 break;
//				}
//			 
//		 }
//	 }
	 SGM5349_DA( VG1_DA ,0 ,DYB );
	 SGM5349_DA( VG2_DA ,0 ,DYB );	 
	 vTaskDelay(300);
//	 while(1){
//	   if( (CheckStopVol(VG1_DA)==SUCCESS) && (CheckStopVol(VG2_DA)==SUCCESS) )
//	   { break ; }
//		 else{
//				  SGM5349_DA( VG1_DA ,0 ,DYB );
//				  SGM5349_DA( VG2_DA ,0 ,DYB );				 
//					vTaskDelay(200);
//					SysProg.StopPowerCnt[1] ++;		 
//					if( SysProg.StopPowerCnt[1] > 50){  //5*2000=10 000 ms
//						 break;
//					 }
//		 }	 
//	 
//	 }
	 
}
	else if(SysData.AgingTestParameters.FirstStopVTime.data==1){ //【先停VG】
	 SGM5349_DA( VG1_DA ,0 ,DYB );
	 SGM5349_DA( VG2_DA ,0 ,DYB );
	 vTaskDelay(300);		
	 if(SysData.AgingTestParameters.AgingMode.data !=PulseMode){
		 while(1){
			 if( (CheckStopVol(VG1_DA)==SUCCESS) && (CheckStopVol(VG2_DA)==SUCCESS) )
			 { break ;}
			 else{
						SGM5349_DA( VG1_DA ,0 ,DYB );
						SGM5349_DA( VG2_DA ,0 ,DYB );				 
						vTaskDelay(200);
						SysProg.StopPowerCnt[1] ++;		 
						if( SysProg.StopPowerCnt[1] > 50){  //5*2000=10 000 ms
							 break;
						 }
			 }		 
		 
		 }
			 SGM5349_DA( VD1_DA ,0 ,DYB);
			 SGM5349_DA( VD2_DA ,0 ,DYB);	
			 vTaskDelay(1000);
			 while(1){
				 if( (CheckStopVol(VD1_DA)==SUCCESS) && (CheckStopVol(VD2_DA)==SUCCESS) )
				 { break ;}
				 else{
						SGM5349_DA( VD1_DA ,0 ,DYB);
						SGM5349_DA( VD2_DA ,0 ,DYB);	
						vTaskDelay(2000);
						SysProg.StopPowerCnt[0] ++;		 
						if( SysProg.StopPowerCnt[0] > 5){  //5*2000=10 000 ms
							 break;
						}
					 
				 }
			 }		 
		 
	 }	 
//	 while(1){
//	   if( (CheckStopVol(VG1_DA)==SUCCESS) && (CheckStopVol(VG2_DA)==SUCCESS) )
//	   { break ;}
//		 else{
//				  SGM5349_DA( VG1_DA ,0 ,DYB );
//				  SGM5349_DA( VG2_DA ,0 ,DYB );				 
//					vTaskDelay(200);
//					SysProg.StopPowerCnt[1] ++;		 
//					if( SysProg.StopPowerCnt[1] > 50){  //5*2000=10 000 ms
//						 break;
//					 }
//		 }		 
//	 
//	 }	
		
	 SGM5349_DA( VD1_DA ,0 ,DYB);
	 SGM5349_DA( VD2_DA ,0 ,DYB);	
	 vTaskDelay(1000);
//	 while(1){
//	   if( (CheckStopVol(VD1_DA)==SUCCESS) && (CheckStopVol(VD1_DA)==SUCCESS) )
//	   { break ;}
//		 else{
//				SGM5349_DA( VD1_DA ,0 ,DYB);
//				SGM5349_DA( VD2_DA ,0 ,DYB);	
//				vTaskDelay(2000);
//				SysProg.StopPowerCnt[0] ++;		 
//				if( SysProg.StopPowerCnt[0] > 5){  //5*2000=10 000 ms
//					 break;
//				}
//			 
//		 }
//	 }
	
	}
//	 	 vTaskDelay(1000);		
}
	


/******************************************************************
函数名称:InitiatingVol
函数功能:发起电压 VD VG
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/

int InitiatingVol_Test(void)
{
	float SetVol,SetProCurr,SetProTemp,ConstantCurr;      //承接处理上位机数据 float变量
	uint16_t Cnti=0,VGCnt=0,VDCnt=0;//,SetDACnt=0;//Time=0;
//  int SetDA=0 ;
//	uint16_t SignFlag[2]={0}; //  对应VG1，2   10为正 20为负
/*******************************************************先发保护电流以及保护温度*****************************************************************/	

	SetProCurr = (SysData.AgingTestParameters.Set_IG_CurrUp.data & 0x7fffffff)/100; //单位mA 保护电流采用电流上限
	SetProCurr = SetProCurr/1000;                                                   //转化成实际下发电压 单位V
	SGM5349_DA( ICC_VG1_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- 保护电压  
	SGM5349_DA( ICC_VG2_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- 保护电压
	
	SetProCurr = (SysData.AgingTestParameters.Set_ID_CurrUp.data & 0x7fffffff)/100; //单位mA 保护电流采用电流上限
	SetProCurr = (SetProCurr/1000)*0.1;                                           //转化成实际下发电压 单位V
	SGM5349_DA( ICC_VD1_P_DA ,SetProCurr ,DYB );   //1A  0.1V-- 保护电压  
	SGM5349_DA( ICC_VD2_P_DA ,SetProCurr ,DYB );   //1000mA  0.1V-- 保护电压
	
	SetProTemp = (SysData.AgingTestParameters.SetTempUp.data & 0x7fffffff)/10;      //单位℃ 保护温度采用温度上限
	SGM5349_DA( TempProtect1 ,SetProTemp ,WDB );	
	SGM5349_DA( TempProtect2 ,SetProTemp ,WDB );

/*********************************************************当前发起二级电源，起调电压*************************************************************/			

	if(SysData.AgingTestParameters.FirstSetVTime.data==0){  //【先发VD】
		
		for(Cnti=0;Cnti<5;Cnti++){

			if( CheckInitiatingVol(VD1_DA)==SUCCESS && CheckInitiatingVol(VD2_DA)==SUCCESS ){
	    	InitiatingVGVol();
			  vTaskDelay(200);				
			  break;
			}	
	  	InitiatingVDVol_Test();
			vTaskDelay(200);			
		}
    if(Cnti==5){
//			if( CheckInitiatingVol(VD1_DA)!=SUCCESS || CheckInitiatingVol(VD2_DA)!=SUCCESS )
			if( CheckInitiatingVol(VD1_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VD_AlarmInformation[0] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			 }
			if( CheckInitiatingVol(VD2_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			 }				
     else{
	    	InitiatingVGVol();
			  vTaskDelay(200);		 
		 }			
		}
		
		for( VGCnt=0;VGCnt<4;VGCnt++){
		
			if( CheckInitiatingVol(VG1_DA)==SUCCESS && CheckInitiatingVol(VG2_DA)==SUCCESS ){				
					break;
				}	
			else{
				InitiatingVGVol();
				vTaskDelay(200);
			}
		
		}
    if(VGCnt==4){
//			if( CheckInitiatingVol(VG1_DA)!=SUCCESS || CheckInitiatingVol(VG2_DA)!=SUCCESS )
			if( CheckInitiatingVol(VG1_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VG_AlarmInformation[0] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}	
			if( CheckInitiatingVol(VG2_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}	

				
	}
	
}
	
	
	else if(SysData.AgingTestParameters.FirstSetVTime.data==1){ //【先发VG】
		
		for(Cnti=0;Cnti<5;Cnti++){
	  	InitiatingVGVol();
			vTaskDelay(200);
			if( CheckInitiatingVol(VG1_DA)==SUCCESS && CheckInitiatingVol(VG2_DA)==SUCCESS ){
//	    	InitiatingVDVol_Test();
			  vTaskDelay(200);				
			  break;
			}			
		}
    if(Cnti==5){
			if( CheckInitiatingVol(VG1_DA)!=SUCCESS || CheckInitiatingVol(VG2_DA)!=SUCCESS ){
			  	SysData.AgingTestData.Voltage_VG_AlarmInformation[0] = aging_alarm_voldown;
					SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}
     else{
//	    	InitiatingVDVol_Test();
			  vTaskDelay(200);		 
		 }			
		}

		for( VDCnt=0;VDCnt<4;VDCnt++){
		
			if( CheckInitiatingVol(VD1_DA)==SUCCESS && CheckInitiatingVol(VD2_DA)==SUCCESS ){				
					break;
				}	
			else{
				InitiatingVDVol_Test();
				vTaskDelay(200);
			}
		
		}
    if(VDCnt==4){
			if( CheckInitiatingVol(VD1_DA)!=SUCCESS || CheckInitiatingVol(VD2_DA)!=SUCCESS ){
			  	SysData.AgingTestData.Voltage_VD_AlarmInformation[0] = aging_alarm_voldown;
					SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}
		}			
	
				
	}


  return SUCCESS;

}




/******************************************************************
函数名称:InitiatingVol
函数功能:发起电压 VD VG
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/

int InitiatingVol(void)
{
	float SetVol,SetProCurr,SetProTemp,ConstantCurr;      //承接处理上位机数据 float变量
	uint16_t Cnti=0,VGCnt=0,VDCnt=0;//,SetDACnt=0;//Time=0;
//  int SetDA=0 ;
//	uint16_t SignFlag[2]={0}; //  对应VG1，2   10为正 20为负
/*******************************************************先发保护电流以及保护温度*****************************************************************/	

	SetProCurr = (SysData.AgingTestParameters.Set_IG_CurrUp.data & 0x7fffffff)/100; //单位mA 保护电流采用电流上限
	SetProCurr = SetProCurr/1000;                                                   //转化成实际下发电压 单位V
	SGM5349_DA( ICC_VG1_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- 保护电压  
	SGM5349_DA( ICC_VG2_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- 保护电压
	
	SetProCurr = (SysData.AgingTestParameters.Set_ID_CurrUp.data & 0x7fffffff)/100; //单位mA 保护电流采用电流上限
	SetProCurr = (SetProCurr/1000)*0.1;                                           //转化成实际下发电压 单位V
	SGM5349_DA( ICC_VD1_P_DA ,SetProCurr ,DYB );   //1A  0.1V-- 保护电压  
	SGM5349_DA( ICC_VD2_P_DA ,SetProCurr ,DYB );   //1000mA  0.1V-- 保护电压
	
	SetProTemp = (SysData.AgingTestParameters.SetTempUp.data & 0x7fffffff)/10;      //单位℃ 保护温度采用温度上限
	SGM5349_DA( TempProtect1 ,SetProTemp ,WDB );	
	SGM5349_DA( TempProtect2 ,SetProTemp ,WDB );

/*********************************************************当前发起二级电源，起调电压*************************************************************/			

	if(SysData.AgingTestParameters.FirstSetVTime.data==0){  //【先发VD】
		
		for(Cnti=0;Cnti<5;Cnti++){
	  	InitiatingVDVol();
			vTaskDelay(200);
			if( CheckInitiatingVol(VD1_DA)==SUCCESS && CheckInitiatingVol(VD2_DA)==SUCCESS ){
	    	InitiatingVGVol();
			  vTaskDelay(200);				
			  break;
			}			
		}
    if(Cnti==5){
//			if( CheckInitiatingVol(VD1_DA)!=SUCCESS || CheckInitiatingVol(VD2_DA)!=SUCCESS )
			if( CheckInitiatingVol(VD1_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VD_AlarmInformation[0] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			 }
			if( CheckInitiatingVol(VD2_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			 }				
     else{
	    	InitiatingVGVol();
			  vTaskDelay(200);		 
		 }			
		}
		
		for( VGCnt=0;VGCnt<4;VGCnt++){
		
			if( CheckInitiatingVol(VG1_DA)==SUCCESS && CheckInitiatingVol(VG2_DA)==SUCCESS ){				
					break;
				}	
			else{
				InitiatingVGVol();
				vTaskDelay(200);
			}
		
		}
    if(VGCnt==4){
//			if( CheckInitiatingVol(VG1_DA)!=SUCCESS || CheckInitiatingVol(VG2_DA)!=SUCCESS )
			if( CheckInitiatingVol(VG1_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VG_AlarmInformation[0] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}	
			if( CheckInitiatingVol(VG2_DA)!=SUCCESS)
				{
			  	SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;
//					SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}	

				
	}
	
}
	
	
	else if(SysData.AgingTestParameters.FirstSetVTime.data==1){ //【先发VG】
		
		for(Cnti=0;Cnti<5;Cnti++){
	  	InitiatingVGVol();
			vTaskDelay(200);
			if( CheckInitiatingVol(VG1_DA)==SUCCESS && CheckInitiatingVol(VG2_DA)==SUCCESS ){
	    	InitiatingVDVol();
			  vTaskDelay(200);				
			  break;
			}			
		}
    if(Cnti==5){
			if( CheckInitiatingVol(VG1_DA)!=SUCCESS || CheckInitiatingVol(VG2_DA)!=SUCCESS ){
			  	SysData.AgingTestData.Voltage_VG_AlarmInformation[0] = aging_alarm_voldown;
					SysData.AgingTestData.Voltage_VG_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}
     else{
	    	InitiatingVDVol();
			  vTaskDelay(200);		 
		 }			
		}

		for( VDCnt=0;VDCnt<4;VDCnt++){
		
			if( CheckInitiatingVol(VD1_DA)==SUCCESS && CheckInitiatingVol(VD2_DA)==SUCCESS ){				
					break;
				}	
			else{
				InitiatingVDVol();
				vTaskDelay(200);
			}
		
		}
    if(VDCnt==4){
			if( CheckInitiatingVol(VD1_DA)!=SUCCESS || CheckInitiatingVol(VD2_DA)!=SUCCESS ){
			  	SysData.AgingTestData.Voltage_VD_AlarmInformation[0] = aging_alarm_voldown;
					SysData.AgingTestData.Voltage_VD_AlarmInformation[1] = aging_alarm_voldown;	
          return ERROR;				
			}
		}			
	
				
	}


  return SUCCESS;

}

/******************************************************************
函数名称:InitiatingVDVol
函数功能:发起电压 
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void InitiatingVDVol_Test()
{
	float SetVol=0;//,SendVolData=0;
  float SETVol[2]={0};
	int SetDA=0 ;
	uint16_t SetDACnt=0;
	SetVol = 110;//(SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;    //单位V   发VD电压
	SETVol[0] =SetVol*1000;
	SETVol[1] =SetVol*1000;
	
	SetDA  =  (SetVol/10.0)-1;
	SetDACnt = SetDA;
	for(SetDA=0; SetDA<SetDACnt; SetDA++){
			SGM5349_DA( VD1_DA ,0+SetDA*10 ,DYB );
			SGM5349_DA( VD2_DA ,0+SetDA*10 ,DYB ); 
			vTaskDelay(10);
		}	

#if ENABLE_KB == TRUE
//					SendVolData = ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0);//程控电压KB
//	        SendVolData =SendVolData/1000.0;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = ( ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0) )/1000.0; //Mv进去 最后转化成V
	      	SETVol[1] = ( ErrorCompensation_v(SETVol[1], VOL_VD2CON, 0) )/1000.0; //Mv进去 最后转化成V
#else
//					SendVolData = SetVol;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = SetVol; //Mv进去 最后转化成V
	      	SETVol[1]	=	SetVol;
#endif

//	 SGM5349_DA( VD1_DA ,SetVol ,DYB);
//	 SGM5349_DA( VD2_DA ,SetVol ,DYB);
	 SGM5349_DA( VD1_DA ,SETVol[0] ,DYB);
	 SGM5349_DA( VD2_DA ,SETVol[1] ,DYB);		
	vTaskDelay(200);
}

/******************************************************************
函数名称:InitiatingVDVol
函数功能:发起电压 
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void InitiatingVDVol()
{
	float SetVol=0;//,SendVolData=0;
  float SETVol[2]={0};
	int SetDA=0 ;
	uint16_t SetDACnt=0;
	SetVol = (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;    //单位V   发VD电压
	SETVol[0] =SetVol*1000;
	SETVol[1] =SetVol*1000;
	
	SetDA  =  (SetVol/10.0)-1;
	SetDACnt = SetDA;
	for(SetDA=0; SetDA<SetDACnt; SetDA++){
			SGM5349_DA( VD1_DA ,0+SetDA*10 ,DYB );
			SGM5349_DA( VD2_DA ,0+SetDA*10 ,DYB ); 
			vTaskDelay(20);
		}	

#if ENABLE_KB == TRUE
//					SendVolData = ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0);//程控电压KB
//	        SendVolData =SendVolData/1000.0;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = ( ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0) )/1000.0; //Mv进去 最后转化成V
	      	SETVol[1] = ( ErrorCompensation_v(SETVol[1], VOL_VD2CON, 0) )/1000.0; //Mv进去 最后转化成V
#else
//					SendVolData = SetVol;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = SetVol; //Mv进去 最后转化成V
	      	SETVol[1]	=	SetVol;
#endif

//	 SGM5349_DA( VD1_DA ,SetVol ,DYB);
//	 SGM5349_DA( VD2_DA ,SetVol ,DYB);
	 SGM5349_DA( VD1_DA ,SETVol[0] ,DYB);
	 SGM5349_DA( VD2_DA ,SETVol[1] ,DYB);		
	vTaskDelay(200);
}


/******************************************************************
函数名称:InitiatingVGVol
函数功能:发起电压  VG
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void InitiatingVGVol()
{
	float SetVol=0;
	int SetDA=0 ;
	uint16_t SetDACnt=0;
	uint16_t SignFlag[2]={0}; //  对应VG1，2   10为正 20为负
	float SETVol[2]={0};
	

	if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
		SetVol = (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;    //单位V   发VG起调电压
		SignFlag[0] = 20;  //负电压 需要* （-1）
	}
	else{
		SetVol = (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;    //单位V   发VG起调电压
		SignFlag[0] = 10;  //正电压 需要* （1）									
	}

 if(SignFlag[0] == 20){	//为负--所以需要×（-1）							 
//		 SGM5349_DA( VG1_DA ,(SETVol[0]*(-1)) ,DYB);
//		 SGM5349_DA( VG2_DA ,(SETVol[1]*(-1)) ,DYB);	
	SETVol[0] =SetVol*(-1000);
	SETVol[1] =SetVol*(-1000);		 
 }
 else if(SignFlag[0] == 10){//为正--需要*1
	SETVol[0] =SetVol*1000;
	SETVol[1] =SetVol*1000;		 
 } 
	

	
	
#if ENABLE_KB == TRUE
//					SendVolData = ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0);//程控电压KB
//	        SendVolData =SendVolData/1000.0;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = ( ErrorCompensation_v(SETVol[0], VOL_VG1CON, 0) )/1000.0; //Mv进去 最后转化成V
	      	SETVol[1] = ( ErrorCompensation_v(SETVol[1], VOL_VG2CON, 0) )/1000.0; //Mv进去 最后转化成V
#else
//					SendVolData = SetVol;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = SetVol; //Mv进去 最后转化成V
	      	SETVol[1]	=	SetVol;
#endif		
	
	
	
 /* 进行判断正负并分解 */							 
// if(SignFlag[0] == 20){	//为负--所以需要×（-1）							 
//		 SGM5349_DA( VG1_DA ,(SETVol[0]*(-1)) ,DYB);
//		 SGM5349_DA( VG2_DA ,(SETVol[1]*(-1)) ,DYB);							 
// }
// else if(SignFlag[0] == 10){//为正--需要*1
	
		 SGM5349_DA( VG1_DA ,SETVol[0] ,DYB);
		 SGM5349_DA( VG2_DA ,SETVol[1] ,DYB);		 
// } 
 
}


