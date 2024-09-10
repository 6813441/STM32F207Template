#include "main.h"

/******************************************************************
函数名称：  InitSGM5349Gpio(void)
函数功能:   SGM5349初始化
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/*
** CLR     ---B54---PD12
** LDAC    ---B6----PF11
** SCLK    ---B57---PG9
** DIN     ---B55---PB14
** SYNC_DYB --B18---PE2
** SYNC_WDB --B5----PE3
*/
void InitSGM5349Gpio(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOE);
	rcu_periph_clock_enable(RCU_GPIOF);
	rcu_periph_clock_enable(RCU_GPIOG);

	gpio_mode_set(SGM5349_LADC_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SGM5349_LADC_PIN); // LADC
	gpio_mode_set(SGM5349_CLR_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SGM5349_CLR_PIN);	 // CLR
	gpio_mode_set(SGM5349_DIN_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SGM5349_DIN_PIN);	 // DIN
	gpio_mode_set(SGM5349_SCLK_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SGM5349_SCLK_PIN); // SCLK

	gpio_mode_set(SGM5349_SYNC_DYB_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SGM5349_SYNC_DYB_PIN); 
	gpio_mode_set(SGM5349_SYNC_WDB_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SGM5349_SYNC_WDB_PIN); 
	

	
	gpio_output_options_set(SGM5349_LADC_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SGM5349_LADC_PIN); // LADC
	gpio_output_options_set(SGM5349_CLR_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SGM5349_CLR_PIN);	 // CLR
	gpio_output_options_set(SGM5349_DIN_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SGM5349_DIN_PIN);	 // DIN
	gpio_output_options_set(SGM5349_SCLK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SGM5349_SCLK_PIN); // SCLK

	gpio_output_options_set(SGM5349_SYNC_DYB_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SGM5349_SYNC_DYB_PIN); // SYNC1
	gpio_output_options_set(SGM5349_SYNC_WDB_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SGM5349_SYNC_WDB_PIN); // SYNC4_1

	SCLK_SGM5349_H; // 时钟
	DIN_SGM5349_H;	//
	LDAC_SGM5349_H;
	CLR_SGM5349_H;
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
void SGM5349_Write_Voltage(float voltage, SGM5349_frame_t *SGM5349_frame, SGM5349_type_t SGM5349_num)
{
	uint16_t data;

	switch (SGM5349_num)
	{
	case SYNC_DYB:
		data = (uint16_t)(((voltage / SGM5349_BENCHMARK_VOLTAGE1) * SGM5349_VOLTAGE_BIT));
		break;
	case SYNC_WDB:
		data = (uint16_t)(((voltage / SGM5349_BENCHMARK_VOLTAGE1) * SGM5349_VOLTAGE_BIT));
		break;
	default:
		break;
	}

	SGM5349_frame->v_data = data;
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
void SGM5349_Write_Channel(uint8_t ch, SGM5349_frame_t *SGM5349_frame) // 1-4
{
	SGM5349_frame->ch = ch;
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
void SGM5349_Send_Frame(SGM5349_frame_t SGM5349_frame, SGM5349_type_t SGM5349_num) // 哪一片输出
{
	uint16_t i;

	SCLK_SGM5349_H;
	
	SYNC_DYB_SGM5349_H;
	SYNC_WDB_SGM5349_H;

	user_delayUs(10);

	switch (SGM5349_num)
	{
	case SYNC_DYB:
		SYNC_DYB_SGM5349_L;
		break;
	case SYNC_WDB:
		SYNC_WDB_SGM5349_L;
		break;
	default:
		break;
	}
	user_delayUs(10);
	for (i = 0; i < SGM5349_FRAME_BIT; i++) // 下降沿读入
	{
		if ((SGM5349_frame.data & 0x80000000) == 0) // 32位
			DIN_SGM5349_L;
		else
			DIN_SGM5349_H;
		SGM5349_frame.data <<= 1;

		SCLK_SGM5349_L;
		user_delayUs(10);
		SCLK_SGM5349_H;
		user_delayUs(10);
	}
	user_delayUs(20);
	switch (SGM5349_num)
	{
	case SYNC_DYB:
		SYNC_DYB_SGM5349_H;
		break;
	case SYNC_WDB:
		SYNC_WDB_SGM5349_H;
		break;
	default:
		break;
	}
	user_delayUs(20);
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
void SGM5349_Output(float voltage, SGM5349_type_t SGM5349_num, uint8_t ch) // 4个通道 1-4
{
	if (voltage <= 0)
	{
		voltage = 0;
	}
	else
	{
		switch (SGM5349_num)
		{
		case SYNC_DYB:
			if (voltage >= SGM5349_BENCHMARK_VOLTAGE1)
				voltage = SGM5349_BENCHMARK_VOLTAGE1;
			break;
		case SYNC_WDB:
			if (voltage >= SGM5349_BENCHMARK_VOLTAGE1)
				voltage = SGM5349_BENCHMARK_VOLTAGE1;
			break;
		default:
			break;
		}
	}

	SGM5349_frame_t SGM5349_frame;
	SGM5349_frame.doncare1 = 0;
	SGM5349_frame.doncare2 = 0;
	
	SGM5349_frame.command = 0x03; // write and updata
	SGM5349_Write_Channel(ch, &SGM5349_frame);
	SGM5349_Write_Voltage(voltage, &SGM5349_frame, SGM5349_num);
	SGM5349_Send_Frame(SGM5349_frame, SGM5349_num);
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
void SGM5349_Init(void)
{
	InitSGM5349Gpio();
	uint16_t i, j;
	CLR_SGM5349_H;
	user_delayUs(5000);

	LDAC_SGM5349_L;
	
	SYNC_DYB_SGM5349_H;
	SYNC_WDB_SGM5349_H;
	

	SCLK_SGM5349_H;
	LDAC_SGM5349_L;


	SGM5349_Soft_Reset((SGM5349_type_t)SYNC_DYB);
	SGM5349_Soft_Reset((SGM5349_type_t)SYNC_WDB);


	user_delayMs(20);

//	for (j = 0; j < 8; j++)
//	{
//		SGM5349_Output(0, (SGM5349_type_t)SYNC_DYB, j); // SYNC1  
//		SGM5349_Output(0, (SGM5349_type_t)SYNC_WDB, j); // SYNC4_1  
//	}

 	SGM5349_DA( VG1_DA ,0 ,DYB );
	SGM5349_DA( VG2_DA ,0 ,DYB );
	
	SGM5349_DA( VD1_DA ,0 ,DYB );
	SGM5349_DA( VD2_DA ,0 ,DYB );
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
void SGM5349_Soft_Reset(SGM5349_type_t SGM5349_num)
{
	SGM5349_frame_t SGM5349_frame;
	SGM5349_frame.doncare1 = 0;
	SGM5349_frame.doncare2 = 0;

	SGM5349_frame.command = 0x07; // reset
	
//	SGM5349_Write_Channel(0, &SGM5349_frame);
//	SGM5349_Write_Voltage(0, &SGM5349_frame, SGM5349_num);
	
	
	if (SGM5349_num==SYNC_DYB){
		SGM5349_Write_Channel(0, &SGM5349_frame);
  	SGM5349_Write_Voltage(0, &SGM5349_frame, SGM5349_num);
		SGM5349_Send_Frame(SGM5349_frame, SGM5349_num);

	}
	else if (SGM5349_num==SYNC_WDB){
		SGM5349_Write_Channel(0, &SGM5349_frame);
  	SGM5349_Write_Voltage(0, &SGM5349_frame, SGM5349_num);
		SGM5349_Send_Frame(SGM5349_frame, SGM5349_num);
	}
	

}


/******************************************************************
函数名称：
函数功能:使用例子
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void SGM5349test(void)
{
	SGM5349_Init();
	while (1)
	{
		SGM5349_Output(0, (SGM5349_type_t)SYNC_DYB, VG1_DA);       // 基准5 
		SGM5349_Output(0, (SGM5349_type_t)SYNC_WDB, TempProtect1); // 基准5 
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
void SGM5349_DA( uint16_t ch ,float data ,uint16_t Board)
{
	float DAValue = 0;
	float R =0;
	if (Board==DYB){
				switch (ch)
				{
					case VG1_DA :
//						 data = (data+0.035)/1.0125 ;
//						 DAValue = 2.5 - data/12   ;//  VG1 = -12 * (DA - 2.5)  以V为单位  （VG1、VG2采用此公式）
						 DAValue =   data/10 + 2.5;
						 SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, VG1_DA);
						break ;
					
					case VG2_DA :
						 DAValue =   data/10 + 2.5;  // 2.5 - data/12   ;//  VG2 = -12 * (DA-2.5)  以V为单位  （VG1、VG2采用此公式）10* (DA-2.5)
						 SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, VG2_DA);
						break ; 
					
					case VD1_DA : 						
						DAValue = data*0.04   ;  //   VD1 = DA/0.04
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, VD1_DA);
						break ;
					
					case VD2_DA :               
						DAValue = data*0.04   ;  //   VD1 = DA/0.04
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, VD2_DA);
						break ;	
					
					case ICC_VG1_P_DA :         //  /* +-0.5 , I*R=VDA_Protect */   当前计算公式未写
						DAValue = data   ;
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, ICC_VG1_P_DA);
						break ;
					
					case ICC_VG2_P_DA :         //   当前计算公式未写
						DAValue = data   ;
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, ICC_VG2_P_DA);
						break ;

					case ICC_VD1_P_DA :         //  /* +-0.5 , I*R=VDA_Protect */   当前计算公式未写
						DAValue = data   ;
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, ICC_VD1_P_DA);
						break ;
					
					case ICC_VD2_P_DA :         //   当前计算公式未写
						DAValue = data   ;
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_DYB, ICC_VD2_P_DA);
						break ;
					
					default :
		        break ;
				}
	} 
  else if (Board==WDB){
	      switch (ch)
				{
				  case TempProtect1 :         
						
						R = 0.39*data +100 ;
					  DAValue = ((5*R)/(5000+R)-0.09804)*50.4 ;
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_WDB, TempProtect1);
						break ;
				  case TempProtect2 :         
					  R = 0.39*data +100 ;
					  DAValue = ((5*R)/(5000+R)-0.09804)*50.4 ;
						SGM5349_Output(DAValue, (SGM5349_type_t)SYNC_WDB, TempProtect2);
						break ;
					default :
		        break ;					
				}
	}	
	else {
		return ;
	}

}


