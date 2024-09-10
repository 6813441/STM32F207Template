/*
 * Copyright (c) 2023,����������һ�о���
 * All rights reserved.
 *
 * �ļ����ƣ�
 * �ļ���ʶ�������ù���ƻ���
 * ժ Ҫ��
 *
 * ��ǰ�汾��1.1
 * �� �ߣ�
 * ������ڣ�2023��4��8��
 *
 * ȡ���汾��1.0
 * ԭ���� ��
 * ������ڣ�2023��4��8��
 */
#include "main.h"
//extern const uint8_t CONCRAL[MAX_VOLSTATE];
//extern const uint8_t CLR[MAX_VOLSTATE];
/******************************************************************
�������ƣ�
��������:���̿�������
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ProgCtrlRun(void)
{
	uint8_t i;
	/*�����PGEST�����ļ����ڡ�Ϊ1 ���롰 ��PGRN�������С�Ϊ0 ������ִ�У���������if*/
	if ((SysProg.bflag.bit.PGEST == 1) && (SysProg.bflag.bit.PGRN == 0))   
	{
		 SysProg.bflag.bit.PGST = 1;         //���á�PGST����ʼ��Ϊ1
//		for (i = 0; i < DEVICE_MAX; i++)
//		{
//			SysProg.Ctrl_AGE_RUN[i] = 1;       // DEVICE_MAX-->���λ��,   ���� �����ϻ����С�4��λ����־Ϊ1 
//		}
		for (i = 0; i < 2; i++)
		{
			SysProg.CtrlAGE_RUN[i] = 1;       // DEVICE_MAX-->���λ��,   ���� �����ϻ����С�4��λ����־Ϊ1 
		}		
	}
}
/******************************************************************
�������ƣ�
��������:���̿�����ͣ
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ProgCtrlPause(void)
{
	uint16_t i;
	/*�����PGRN�������С���־Ϊ1�������IF���*/
	if (SysProg.bflag.bit.PGRN == 1)
	{
		VD_PRE_H;
		ConCLR(RESET);          //��������
		AgingStateConfig(AGING_STATUS_PAUSE);  //�����ϻ�״̬���ú���   ��������Ϊ����ͣ��
	}
}
/******************************************************************
�������ƣ�
��������:���̿��ƻָ�
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ProgCtrlRecover(void)
{
	uint8_t i;
	/*���ϵͳ�ϻ�״̬������Ϊ�ϻ�����ͣ�� �� ϵͳ��PGRN�������С���־����Ϊ0  ��������ִ��if*/
	if ((SysData.AgingTestData.AgingStatus == AGING_STATUS_PAUSE) && (SysProg.bflag.bit.PGRN == 0))
	{
		if (SysProg.bflag.bit.PGEST == 1) //�����PGEST�����ļ����ڡ���־����Ϊ1
		{
			for (i = 0; i < 2; i++)
			{
//				SysProg.Ctrl_AGE_RUN[i] = 1;  //���ν���ϵͳ����.�����ϻ����С���λ��־����Ϊ1
				SysProg.CtrlAGE_RUN[i] = 1; 
			}
		}
		AgingStateConfig(AGING_STATUS_RUN);//�����ϻ�״̬���ú���������״̬Ϊ���ϻ��С�
	}
}
/******************************************************************
�������ƣ�
��������:���̿���ֹͣ
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ProgCtrlSTOP(void)
{
	uint8_t i;
	AgingStateConfig(AGING_STATUS_MANUAL_STOP);
}


/******************************************************************
�������ƣ�
��������:���̺����¶ȣ��ﵽ��ſ�ʼ�����ϻ� 23.10.18�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ProgCtrlPowerStart(void)
{
	AgingStateConfig(WAITING_POWER_RISE);
}

/******************************************************************
�������ƣ�
��������:�ϻ�״̬����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
extern uint8_t FSD_cnt;
void AgingStateConfig(AgingStatusCOM state)
{
	int InitiatingVolCheck=0;
	switch (state)
	{
	case AGING_STATUS_WAIT://���ȴ���
	{

		Fpga_Disable(0);
    /*TEMP_STANDBY_MODE =2-->����ģʽ*/
		StationTempCtrl(TEMP_STANDBY_MODE);   
	
  	SysProg.bflag.all = 0;
		ClearAgingVarParam();  //����ϻ��ṹ�����
		SysData.AgingTestData.ProRunTimeCnt.data = 0;//ϵͳ����.�ϻ���������.�ϻ�ʱ��.����
		memset(&SysData.AgingTestParameters, 0, sizeof(SysData.AgingTestParameters));

   /*ϵͳ����.����ģʽ����Ϊ����ģʽ*/
		SysCtrl.ProgType = NormalStation;  
   /*ϵͳ����.��̫�����ӱ�־�Լ�����������Ϊ0*/		
		SysCtrl.EthConnectFlag = 0;
		SysCtrl.EthConnectCnt = 0;
		SysCtrl.StartMode = 10;
		
	  SysCtrl.PlusOrMinusFlag[0]=0;
		SysCtrl.PlusOrMinusFlag[1]=0;
		SysCtrl.ADjustVolCnt =0;
		
		HRIF_S_Fpga_Init();		
	}

	break;
	
	/*AGING_STATUS_READY --> ����*/
	case AGING_STATUS_READY://��������                     
	{
	  Init_PID();    // ��ʼ���¿�PID
		SysCtrl.ProgType = NormalStation;  // ����ģʽΪ����ģʽ
//		ConCLR(RESET);
		ClearAgingVarParam();  //����ϻ��ṹ��
		SysData.AgingTestData.ProRunTimeCnt.data = 0;  //�ϻ�ʱ������
		SysProg.bflag.bit.PGEST = 1;                   //PGEST �����ļ����� 
		
		if(SysData.AgingTestParameters.AgingMode.data==PulseMode){
			VD_PRE_H;
	  	ConCLR(RESET);          //��������
			InitiatingVDVol_Test();
		}
	}
	break;
	
	/*AGING_STATUS_RUN-->�ϻ���*/
	case AGING_STATUS_RUN://���ϻ��С�
	{

  	/* ��ʼ����ѹ�Լ��̵��� */
		VD_PRE_H;
		ConCLR(RESET);          //��������
//		ConRelayInit();					//�̵�������														
		
		switch(SysData.AgingTestParameters.AgingMode.data)
		{
			
			case ConstantCurrentMode :  //������ģʽ��
			/*************************************************************************************************************************************************
			˵���� ����ģʽ�£�1.�����·�VD VG ����ֵ 2.��ȡID�ĵ���ֵ  3��ID�����õ���ֵ�Ƚ� 4. ���ڶ�Ӧ��ID1-VG1 ID2-VG2ʹ��ID=~���õ��� 
			*************************************************************************************************************************************************/
					vTaskDelay(20);
          InitiatingVolCheck =InitiatingVol();                            //�����ѹ
			    if(InitiatingVolCheck==SUCCESS){
						vTaskDelay(200);			
						ConCLR(SET); //                             ---����ʹ��	
						vTaskDelay(200);
					}
					else{
						vTaskDelay(20);
						ConCLR(SET); //                             ---����ʹ��		
						SysProg.ERRORFLAG = 0xfB;				
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;						
					}					
					SysCtrl.StartMode = ConstantCurrentMode;    // ģʽ����Ϊ����ģʽ
				break;
			
			case ConstantPowerMode :  //���㹦��ģʽ��
			/*************************************************************************************************************************************************
			˵���� �㹦��ģʽ��1.�����·�VD VG ����ֵ 2.���Iֵ I=P/U ����ת���ɺ���ģʽ ��
			��ȡIG�ĵ���ֵ  3��IG��Iֵ�Ƚ� 4. ���ڶ�Ӧ��IG1-VG1 IG2-VG2ʹ��IG=~I 
			*************************************************************************************************************************************************/
					vTaskDelay(20);	
          InitiatingVolCheck =InitiatingVol();                            //�����ѹ
			    if(InitiatingVolCheck==SUCCESS){ 
						ConCLR(SET); //                             ---����ʹ��	
						vTaskDelay(200);
					}
					else{
						vTaskDelay(20);
						ConCLR(SET); //                             ---����ʹ��		
						SysProg.ERRORFLAG = 0xfB;			
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;						
					}
					SysCtrl.StartMode = ConstantPowerMode;      // ģʽ����Ϊ�㹦��ģʽ
				break;
			
			case PulseMode :  //������ģʽ��
			/*************************************************************************************************************************************************
			˵���� ����ģʽ��1.�����·�VD VG ����ֵ  2. ����PWM�� 
			*************************************************************************************************************************************************/
					vTaskDelay(20);	
			    InitiatingVolCheck =InitiatingVol_Test();
          if(InitiatingVolCheck==SUCCESS){
						vTaskDelay(200);
						Fpga_Enable(0);                             // FPGA�������	
//					  SysCtrl.StartMode = PulseMode;              // ģʽ����Ϊ����ģʽ
				    HRIF_S_Fpga_USE();                          // ������
						vTaskDelay(200);
						ConCLR(RESET); //                             ---����ʹ��			
//						vTaskDelay(20);
//            VG1_CLR_H						
					}
					else{
						vTaskDelay(20);
						ConCLR(SET); //                             ---����ʹ��		
						SysProg.ERRORFLAG = 0xfB;				
						AgingStateConfig(AGING_STATUS_ERROR_STOP);
						return;						
					}
				  SysCtrl.StartMode = PulseMode;              // ģʽ����Ϊ����ģʽ		
				break;		
			
			case ConstantVoltageMode :  //����ѹģʽ��
			/*************************************************************************************************************************************************
			˵���� ��ѹģʽ��1.�����·�VD VG ����ֵ  2. ����PWM�� 
			*************************************************************************************************************************************************/  
					vTaskDelay(20);		
          InitiatingVolCheck = InitiatingVol();	//�����ѹ	
			    if(InitiatingVolCheck==SUCCESS){                                      
						vTaskDelay(200);
#if CLRCon==TRUE						
						ConCLR(SET); //                             ---����ʹ��			
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
					SysCtrl.StartMode = ConstantVoltageMode;    // ģʽ����Ϊ��ѹģʽ					
				break;  			
		}
//		vTaskDelay(500);
		SysProg.bflag.bit.PGRN = 1;
	}
	break;
	
  /*����ͣ��*/	
	case AGING_STATUS_PAUSE:   
	{
		StationTempCtrl(TEMP_STANDBY_MODE);    //����ƽ̨�¶ȿ��Ƴ������� ��������Ϊ����ģʽ
		ClearAgingVarParam();                  //��������ϻ��ṹ���������ṹ��
		SysProg.bflag.bit.PGRN = 0;            //��PGRN�������С���־����Ϊ0
	}
	break;
	
	/*������ϻ���*/
	case AGING_STATUS_AGING_FINISH:
	{
		StationTempCtrl(TEMP_STANDBY_MODE);
		HRIF_S_Fpga_Init();
		vTaskDelay(100);
		ConCLR(RESET); //�����ȹر�Ӳ������ʹ�ܣ��ſ����·�0������ᴥ��Ӳ������
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
	
	case AGING_STATUS_MANUAL_STOP:      //���ֶ�ֹͣ��
	{		

		StationTempCtrl(TEMP_STANDBY_MODE);
		HRIF_S_Fpga_Init();
		vTaskDelay(100);		
		ConCLR(RESET); //�����ȹر�Ӳ������ʹ�ܣ��ſ����·�0������ᴥ��Ӳ������
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
��������:StopVol
��������:ֹͣ��ѹ VD VG
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void StopVol(void)
{
	SysProg.StopPowerCnt[0] = 0;
	SysProg.StopPowerCnt[1] = 0;	
	if(SysData.AgingTestParameters.FirstStopVTime.data==0){      //����ͣVD��
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
	else if(SysData.AgingTestParameters.FirstStopVTime.data==1){ //����ͣVG��
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
��������:InitiatingVol
��������:�����ѹ VD VG
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/

int InitiatingVol_Test(void)
{
	float SetVol,SetProCurr,SetProTemp,ConstantCurr;      //�нӴ�����λ������ float����
	uint16_t Cnti=0,VGCnt=0,VDCnt=0;//,SetDACnt=0;//Time=0;
//  int SetDA=0 ;
//	uint16_t SignFlag[2]={0}; //  ��ӦVG1��2   10Ϊ�� 20Ϊ��
/*******************************************************�ȷ����������Լ������¶�*****************************************************************/	

	SetProCurr = (SysData.AgingTestParameters.Set_IG_CurrUp.data & 0x7fffffff)/100; //��λmA �����������õ�������
	SetProCurr = SetProCurr/1000;                                                   //ת����ʵ���·���ѹ ��λV
	SGM5349_DA( ICC_VG1_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- ������ѹ  
	SGM5349_DA( ICC_VG2_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- ������ѹ
	
	SetProCurr = (SysData.AgingTestParameters.Set_ID_CurrUp.data & 0x7fffffff)/100; //��λmA �����������õ�������
	SetProCurr = (SetProCurr/1000)*0.1;                                           //ת����ʵ���·���ѹ ��λV
	SGM5349_DA( ICC_VD1_P_DA ,SetProCurr ,DYB );   //1A  0.1V-- ������ѹ  
	SGM5349_DA( ICC_VD2_P_DA ,SetProCurr ,DYB );   //1000mA  0.1V-- ������ѹ
	
	SetProTemp = (SysData.AgingTestParameters.SetTempUp.data & 0x7fffffff)/10;      //��λ�� �����¶Ȳ����¶�����
	SGM5349_DA( TempProtect1 ,SetProTemp ,WDB );	
	SGM5349_DA( TempProtect2 ,SetProTemp ,WDB );

/*********************************************************��ǰ���������Դ�������ѹ*************************************************************/			

	if(SysData.AgingTestParameters.FirstSetVTime.data==0){  //���ȷ�VD��
		
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
	
	
	else if(SysData.AgingTestParameters.FirstSetVTime.data==1){ //���ȷ�VG��
		
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
��������:InitiatingVol
��������:�����ѹ VD VG
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/

int InitiatingVol(void)
{
	float SetVol,SetProCurr,SetProTemp,ConstantCurr;      //�нӴ�����λ������ float����
	uint16_t Cnti=0,VGCnt=0,VDCnt=0;//,SetDACnt=0;//Time=0;
//  int SetDA=0 ;
//	uint16_t SignFlag[2]={0}; //  ��ӦVG1��2   10Ϊ�� 20Ϊ��
/*******************************************************�ȷ����������Լ������¶�*****************************************************************/	

	SetProCurr = (SysData.AgingTestParameters.Set_IG_CurrUp.data & 0x7fffffff)/100; //��λmA �����������õ�������
	SetProCurr = SetProCurr/1000;                                                   //ת����ʵ���·���ѹ ��λV
	SGM5349_DA( ICC_VG1_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- ������ѹ  
	SGM5349_DA( ICC_VG2_P_DA ,SetProCurr ,DYB );   //300mA  0.3-- ������ѹ
	
	SetProCurr = (SysData.AgingTestParameters.Set_ID_CurrUp.data & 0x7fffffff)/100; //��λmA �����������õ�������
	SetProCurr = (SetProCurr/1000)*0.1;                                           //ת����ʵ���·���ѹ ��λV
	SGM5349_DA( ICC_VD1_P_DA ,SetProCurr ,DYB );   //1A  0.1V-- ������ѹ  
	SGM5349_DA( ICC_VD2_P_DA ,SetProCurr ,DYB );   //1000mA  0.1V-- ������ѹ
	
	SetProTemp = (SysData.AgingTestParameters.SetTempUp.data & 0x7fffffff)/10;      //��λ�� �����¶Ȳ����¶�����
	SGM5349_DA( TempProtect1 ,SetProTemp ,WDB );	
	SGM5349_DA( TempProtect2 ,SetProTemp ,WDB );

/*********************************************************��ǰ���������Դ�������ѹ*************************************************************/			

	if(SysData.AgingTestParameters.FirstSetVTime.data==0){  //���ȷ�VD��
		
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
	
	
	else if(SysData.AgingTestParameters.FirstSetVTime.data==1){ //���ȷ�VG��
		
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
��������:InitiatingVDVol
��������:�����ѹ 
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void InitiatingVDVol_Test()
{
	float SetVol=0;//,SendVolData=0;
  float SETVol[2]={0};
	int SetDA=0 ;
	uint16_t SetDACnt=0;
	SetVol = 110;//(SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;    //��λV   ��VD��ѹ
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
//					SendVolData = ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0);//�̿ص�ѹKB
//	        SendVolData =SendVolData/1000.0;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = ( ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0) )/1000.0; //Mv��ȥ ���ת����V
	      	SETVol[1] = ( ErrorCompensation_v(SETVol[1], VOL_VD2CON, 0) )/1000.0; //Mv��ȥ ���ת����V
#else
//					SendVolData = SetVol;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = SetVol; //Mv��ȥ ���ת����V
	      	SETVol[1]	=	SetVol;
#endif

//	 SGM5349_DA( VD1_DA ,SetVol ,DYB);
//	 SGM5349_DA( VD2_DA ,SetVol ,DYB);
	 SGM5349_DA( VD1_DA ,SETVol[0] ,DYB);
	 SGM5349_DA( VD2_DA ,SETVol[1] ,DYB);		
	vTaskDelay(200);
}

/******************************************************************
��������:InitiatingVDVol
��������:�����ѹ 
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void InitiatingVDVol()
{
	float SetVol=0;//,SendVolData=0;
  float SETVol[2]={0};
	int SetDA=0 ;
	uint16_t SetDACnt=0;
	SetVol = (SysData.AgingTestParameters.Set_VD_Vol.data & 0x7fffffff)/100.0;    //��λV   ��VD��ѹ
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
//					SendVolData = ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0);//�̿ص�ѹKB
//	        SendVolData =SendVolData/1000.0;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = ( ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0) )/1000.0; //Mv��ȥ ���ת����V
	      	SETVol[1] = ( ErrorCompensation_v(SETVol[1], VOL_VD2CON, 0) )/1000.0; //Mv��ȥ ���ת����V
#else
//					SendVolData = SetVol;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = SetVol; //Mv��ȥ ���ת����V
	      	SETVol[1]	=	SetVol;
#endif

//	 SGM5349_DA( VD1_DA ,SetVol ,DYB);
//	 SGM5349_DA( VD2_DA ,SetVol ,DYB);
	 SGM5349_DA( VD1_DA ,SETVol[0] ,DYB);
	 SGM5349_DA( VD2_DA ,SETVol[1] ,DYB);		
	vTaskDelay(200);
}


/******************************************************************
��������:InitiatingVGVol
��������:�����ѹ  VG
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void InitiatingVGVol()
{
	float SetVol=0;
	int SetDA=0 ;
	uint16_t SetDACnt=0;
	uint16_t SignFlag[2]={0}; //  ��ӦVG1��2   10Ϊ�� 20Ϊ��
	float SETVol[2]={0};
	

	if(SysData.AgingTestParameters.Set_VG_Vol.data>0x80000000){
		SetVol = (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;    //��λV   ��VG�����ѹ
		SignFlag[0] = 20;  //����ѹ ��Ҫ* ��-1��
	}
	else{
		SetVol = (SysData.AgingTestParameters.Set_VG_Vol.data & 0x7fffffff)/100.0;    //��λV   ��VG�����ѹ
		SignFlag[0] = 10;  //����ѹ ��Ҫ* ��1��									
	}

 if(SignFlag[0] == 20){	//Ϊ��--������Ҫ����-1��							 
//		 SGM5349_DA( VG1_DA ,(SETVol[0]*(-1)) ,DYB);
//		 SGM5349_DA( VG2_DA ,(SETVol[1]*(-1)) ,DYB);	
	SETVol[0] =SetVol*(-1000);
	SETVol[1] =SetVol*(-1000);		 
 }
 else if(SignFlag[0] == 10){//Ϊ��--��Ҫ*1
	SETVol[0] =SetVol*1000;
	SETVol[1] =SetVol*1000;		 
 } 
	

	
	
#if ENABLE_KB == TRUE
//					SendVolData = ErrorCompensation_v(SETVol[0], VOL_VD1CON, 0);//�̿ص�ѹKB
//	        SendVolData =SendVolData/1000.0;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = ( ErrorCompensation_v(SETVol[0], VOL_VG1CON, 0) )/1000.0; //Mv��ȥ ���ת����V
	      	SETVol[1] = ( ErrorCompensation_v(SETVol[1], VOL_VG2CON, 0) )/1000.0; //Mv��ȥ ���ת����V
#else
//					SendVolData = SetVol;
//		      SETVol[0] =SendVolData;
		      SETVol[0] = SetVol; //Mv��ȥ ���ת����V
	      	SETVol[1]	=	SetVol;
#endif		
	
	
	
 /* �����ж��������ֽ� */							 
// if(SignFlag[0] == 20){	//Ϊ��--������Ҫ����-1��							 
//		 SGM5349_DA( VG1_DA ,(SETVol[0]*(-1)) ,DYB);
//		 SGM5349_DA( VG2_DA ,(SETVol[1]*(-1)) ,DYB);							 
// }
// else if(SignFlag[0] == 10){//Ϊ��--��Ҫ*1
	
		 SGM5349_DA( VG1_DA ,SETVol[0] ,DYB);
		 SGM5349_DA( VG2_DA ,SETVol[1] ,DYB);		 
// } 
 
}


