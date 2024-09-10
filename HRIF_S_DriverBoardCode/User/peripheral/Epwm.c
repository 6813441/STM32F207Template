#include "main.h"
#include "math.h"

/*
*  HRIF_S 发PWM波
   PWM3B -- B29  --PB9  --TIMER3_CH3
	 PWM4B -- B31  --PB7  --TIMER3_CH1
   PWM4A -- B32  --PB8  --TIMER3_CH2
	 PWM3A -- B34  --PB6  --TIMER3_CH0
	 
	 FSD1  -- B48                    --PC9   --TIMER2_CH3
	 FSD2  -- B47                    --PC8   --TIMER2_CH2
	 FSD3  -- B50  --USART5_RX_485   --PC7   --TIMER2_CH1
	 FSD4  -- B49  --USART5_TX_485   --PC6   --TIMER2_CH0
	 
*  HRIF_S 捕获PWM波	 
   FB_OUT3 -- B62 --PA11   --TIMER0_CH3
	 FB_OUT4 -- B61 --PB10   --TIMER1_CH2
	 FB_OUT1 -- B64 --PA10   --TIMER0_CH2
	 FA_OUT2 -- B63 --PA9    --TIMER0_CH1
	 
*  GPIC2008 发PWM波
   PWM3B -- B29  --PB9
	 PWM4B -- B31  --PB7
   PWM4A -- B32  --PB8
	 PWM3A -- B30  --PE0
	 
	 FSD1  -- B48
	 FSD2  -- B47
	 FSD3  -- B50
	 FSD4  -- B49
	 
*  GPIC2008 捕获PWM波	 
   FB_OUT3 -- B62
	 FB_OUT4 -- B61
	 FB_OUT1 -- B64
	 FA_OUT2 -- B63	 
*/


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
void PWM_GPIO_Init(void)
{
    // TIM2 GPIO Init
	  /*
	  FSD1  -- B48                    --PC9   --TIMER2_CH3
		FSD2  -- B47                    --PC8   --TIMER2_CH2
		FSD3  -- B50  --USART5_RX_485   --PC7   --TIMER2_CH1
		FSD4  -- B49  --USART5_TX_485   --PC6   --TIMER2_CH0
	  */
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_6);

    gpio_bit_set(GPIOC, GPIO_PIN_6); // 通道 ch 0

    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_7);

    gpio_bit_set(GPIOC, GPIO_PIN_7); // 通道 ch 1

    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_8);

    gpio_bit_set(GPIOC, GPIO_PIN_8); // 通道 ch 2

    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_9);

    gpio_bit_set(GPIOC, GPIO_PIN_9); // 通道 ch 3
		

    // TIM3 GPIO Init
		/* 
    PWM3B -- B29  --PB9  --TIMER3_CH3
		PWM4B -- B31  --PB7  --TIMER3_CH1
		PWM4A -- B32  --PB8  --TIMER3_CH2
		PWM3A -- B34  --PB6  --TIMER3_CH0
  	*/
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_6);

    gpio_bit_set(GPIOB, GPIO_PIN_6); //通道 ch 0

    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_7);

    gpio_bit_set(GPIOB, GPIO_PIN_7); //通道 ch 1

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_8);

    gpio_bit_set(GPIOB, GPIO_PIN_8); //通道 ch 2

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_9);

    gpio_bit_set(GPIOB, GPIO_PIN_9); //通道 ch 3

//    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
//    gpio_output_options_set(GPIOE, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
//    gpio_af_set(GPIOE, GPIO_AF_15, GPIO_PIN_0);

//    gpio_bit_set(GPIOE, GPIO_PIN_0); // ch 2
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
void Timer2Base_init()
{
    // 定时器时钟树来源
    /*    RCU_CFG1 寄存器 的24位
        0：如果 RCU_CFG0 寄存器的 APB1PSC/APB2PSC 位域的值为 0b0xx(CK_APBx
    = CK_AHB) 或 0b100(CK_APBx = CK_AHB/2) ， 定 时 器 时 钟 等 于

    CK_AHB(CK_TIMERx = CK_AHB)，否则定时器时钟等于 APB 时钟的两倍(在 APB1

    域的定时器： CK_TIMERx = 2 x CK_APB1，在 APB2 域的定时器：CK_TIMERx =
    2 x CK_APB2)。*/
    /*时钟频率120M/4*2/1 得到定时器时钟TIMER_CLK  两倍因素来源于上文写的除4则乘2 */

    // PWM 频率等于 TIMER_CLK/(prescaler+1)/(period+1)
    rcu_periph_clock_enable(RCU_TIMER2); // 注意参数名前面有待RCU。。
    //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
    timer_deinit(TIMER2);

    timer_internal_clock_config(TIMER2); // 选择内部时钟CK_TIMER(200M)

    /* TIMER1配置为0.1ms定时 */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);

    timer_initpara.prescaler = 5;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 根据prescaler,clockdivision最终该定时器时钟评率为1M
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 触发方式设置根据边沿决定
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 计数向上
    timer_initpara.period = 59999;                      // 设置为0.1ms触发一次
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &timer_initpara);
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：//out Fre=(60000000/(5+1))/60000=166HZ;
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/

void PWM_TIM2_Init()
{
    Timer2Base_init();

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_oc_parameter_struct timer_ocintpara;

    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER2, TIMER_CH_0, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 0); // 4修改占空比
    timer_channel_output_mode_config(TIMER2, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0); // 4修改占空比
    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_channel_output_config(TIMER2, TIMER_CH_2, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 0); // 4修改占空比
    timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_channel_output_config(TIMER2, TIMER_CH_3, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, 0); // 4修改占空比
    timer_channel_output_mode_config(TIMER2, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
    /* 使能TIMERx_CAR寄存器的影子寄存器 */
    timer_auto_reload_shadow_enable(TIMER2);

    /* 所有通道输出使能 */
    // timer_primary_output_config(TIMER2, ENABLE);

    // timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    timer_enable(TIMER2);
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
void Timer3Base_init()
{
    // 定时器时钟树来源
    /*    RCU_CFG1 寄存器 的24位
        0：如果 RCU_CFG0 寄存器的 APB1PSC/APB2PSC 位域的值为 0b0xx(CK_APBx
    = CK_AHB) 或 0b100(CK_APBx = CK_AHB/2) ， 定 时 器 时 钟 等 于

    CK_AHB(CK_TIMERx = CK_AHB)，否则定时器时钟等于 APB 时钟的两倍(在 APB1

    域的定时器： CK_TIMERx = 2 x CK_APB1，在 APB2 域的定时器：CK_TIMERx =
    2 x CK_APB2)。*/
    /*时钟频率120M/4*2/1 得到定时器时钟TIMER_CLK  两倍因素来源于上文写的除4则乘2 */
    rcu_periph_clock_enable(RCU_TIMER3); // 注意参数名前面有待RCU。。
    //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
    timer_deinit(TIMER3);

    timer_internal_clock_config(TIMER3); // 选择内部时钟CK_TIMER(200M)

    /* TIMER1配置为0.1ms定时 */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);

    timer_initpara.prescaler = 5;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 根据prescaler,clockdivision最终该定时器时钟评率为1M
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 触发方式设置根据边沿决定
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 计数向上
    timer_initpara.period = 59999;                      // 设置为0.1ms触发一次
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3, &timer_initpara);
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
void PWM_TIM3_Init()
{
    Timer3Base_init();
    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_oc_parameter_struct timer_ocintpara;

    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER3, TIMER_CH_0, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_0, 60000); // 4修改占空比
    timer_channel_output_mode_config(TIMER3, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_channel_output_config(TIMER3, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_1, 60000); // 4修改占空比
    timer_channel_output_mode_config(TIMER3, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_channel_output_config(TIMER3, TIMER_CH_2, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_2, 60000); // 4修改占空比
    timer_channel_output_mode_config(TIMER3, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

    /* TIMER的CH0配置为PWM输出:边触发,这样保证每0.1ms产生一次触发 */
    timer_channel_output_config(TIMER3, TIMER_CH_3, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_3, 60000); // 4修改占空比
    timer_channel_output_mode_config(TIMER3, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

    /* 使能TIMERx_CAR寄存器的影子寄存器 */
    timer_auto_reload_shadow_enable(TIMER3);

    /* 所有通道输出使能 */
//    timer_primary_output_config(TIMER3, ENABLE);

//    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
    timer_enable(TIMER3);
}

/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：    HRIF-S 只用FSD1 FSD2 HOT1 HOT2
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void FSDSetup(uint16_t FSD1, uint16_t FSD2,uint16_t FSD3, uint16_t FSD4)
{
		/*
	  FSD1  -- B48                    --PC9   --TIMER2_CH3  --- FSD1 <--> HOT1
		FSD2  -- B47                    --PC8   --TIMER2_CH2  --- FSD2 <--> HOT2
		FSD3  -- B50  --USART5_RX_485   --PC7   --TIMER2_CH1
		FSD4  -- B49  --USART5_TX_485   --PC6   --TIMER2_CH0
	  */
//    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, FSD4);
//    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, FSD3);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, FSD2);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, FSD1);
}

void FSDSortControl(uint16_t Gear)
{
	uint16_t FSD=0;
  switch(Gear)
	{
		case 0: FSD=0;
		break ;
		case 1: FSD=10000;
		break ;
		case 2: FSD=20000;
		break ;
		case 3: FSD=30000;
		break ;
		case 4: FSD=40000;
		break ;
		case 5: FSD=50000;
		break ;
		case 6: FSD=60000;
		break ;		
		default :
		break ;
	}
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, FSD);
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, FSD);


}
/******************************************************************
函数名称：HRIF-S 只用HOT1 HOT2
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void HOTSetup(uint16_t HOT1, uint16_t HOT2,uint16_t HOT3, uint16_t HOT4)
{
		/* 
    PWM3B -- B29  --PB9  --TIMER3_CH3  HOT2
		PWM4B -- B31  --PB7  --TIMER3_CH1  
		PWM4A -- B32  --PB8  --TIMER3_CH2  
		PWM3A -- B34  --PB6  --TIMER3_CH0  HOT1
  	*/
	  //CH3 对应第三路 
//    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_1, HOT3); 
//    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_2, HOT4);
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_3, HOT2);
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_0, HOT1); 
}
/******************************************************************
函数名称：PWM_Ctrl
函数功能:Pwm调速控制
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Init_PID(void)
{
    uint16_t itemp;
    for (itemp = 0; itemp < MAX_TEMP_CH; itemp++)
    {
        SysCtrl.PIDControl[itemp].SetValue = 0.0;
        SysCtrl.PIDControl[itemp].ActualValue = 0.0;
        SysCtrl.PIDControl[itemp].errdata = 0.0;
        SysCtrl.PIDControl[itemp].integral = 0;
        SysCtrl.PIDControl[itemp].last_errdata = 0.0;
        SysCtrl.PIDControl[itemp].llast_errdata = 0.0;
        SysCtrl.PIDControl[itemp].Outvalue = 0;

        SysCtrl.PIDControl[itemp].KpIndex = 2400;//1500;//1100;
        SysCtrl.PIDControl[itemp].KiIndex = 5.8; // 2.4
        SysCtrl.PIDControl[itemp].KdIndex = 0; //10

        SysCtrl.HOT[itemp] = 60000; // 加热棒上电默认关闭
        SysCtrl.FSD[itemp] = 2000;  // 风机上电默认关闭
    }
//		if(SysData.AgingTestParameters.SetTempUp.data >= 1200){
//		       for (itemp = 0; itemp < MAX_TEMP_CH; itemp++){
//					  SysCtrl.PIDControl[itemp].KpIndex = 1800;//(3800)//1500;//1100;
//						SysCtrl.PIDControl[itemp].KiIndex = 1.95;//(2.4+0.5+1.9);//(2.4+0.5);//6.8;//6.8; // 2.4
//					 }
//			
//		}
		
}

/******************************************************************
函数名称：PWM_Ctrl_1
函数功能:Pwm调速控制
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float PID_Caculate(struct PID_REG *PIDControl)
{
    if (!PIDControl->SetValue)
        return 0;
    PIDControl->errdata = PIDControl->SetValue - PIDControl->ActualValue;   //误差计算--比例控制 --P
    PIDControl->integral += PIDControl->errdata;                            //误差相加--积分控制 --I
    PIDControl->diffdata = (PIDControl->errdata - PIDControl->last_errdata) * 2;
    PIDControl->Outvalue = (PIDControl->KpIndex * PIDControl->errdata)+ (PIDControl->KiIndex * PIDControl->integral) + (PIDControl->KdIndex * PIDControl->diffdata) ;
    PIDControl->last_errdata = PIDControl->errdata;
    //		 //提前积累
    //		 if (PIDControl->errdata > -50 &&PIDControl->errdata <0)
    //			  PIDControl->integral +=  -PIDControl->errdata*20;
    //		 else if (PIDControl->errdata <-50)
    //			 PIDControl->integral=0;
    // 防止过饱和
    if ( PIDControl->integral > 100000 )
        PIDControl->integral = 100000;
    if ( PIDControl->Outvalue < 0 )
    {
        PIDControl->Outvalue = 0;
    }
    else if (PIDControl->Outvalue > 57000)
    {
        PIDControl->Outvalue = 57000;
    }
    else
    {
        // 当输出处于控制范围内不进行干涉
    }
    return PIDControl->Outvalue;
}

extern uint8_t FSD_cnt;
/******************************************************************
函数名称：
函数功能:平台温度控制程序 PID控温，降温等
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void StationTempCtrl(uint16_t Flag)
{
    uint16_t i, itemp;

    if (Flag == TEMP_HOT_MODE)
    {
        for (uint32_t i = 0; i < 2; i++)
        {
            // 温度接近设定温度控制
            if (0 == SysProg.CtrlAGE_RUN[i])
            {
                SysCtrl.HOT[i] = 60000;
            }
            else
            {
                if ( (SysData.AgingTestData.Station_TC_Temp[i].data > (SysData.AgingTestParameters.SetTemp.data -50)  ) )  //小于设置温度5°
                {
									  SysCtrl.PIDControl[i].SetValue    = SysData.AgingTestParameters.SetTemp.data;
                    SysCtrl.PIDControl[i].ActualValue = SysData.AgingTestData.Station_TC_Temp[i].data;
                    SysCtrl.HOT[i] = 60000 - PID_Caculate(&SysCtrl.PIDControl[i]);
//									SysCtrl.HOT[i] = PID_Caculate(&SysCtrl.PIDControl[i]);
                }
                else
                {
                    SysCtrl.HOT[i] = 0;
                }
            }
        }
				
        FSDSetup(SysCtrl.FSD[0], SysCtrl.FSD[1],SysCtrl.FSD[2], SysCtrl.FSD[3]);
        HOTSetup(SysCtrl.HOT[0], SysCtrl.HOT[1],SysCtrl.HOT[2], SysCtrl.HOT[3]);
        // 风机控制
        FSD_cnt++;
        if (FSD_cnt > 15)
        {
            for (itemp = 0; itemp < 2; itemp++)
            {
                if (SysData.AgingTestData.Station_TC_Temp[itemp].data > SysData.AgingTestParameters.SetTemp.data) // 温度大于设定温度三度，风机增速
                {
                    if ((SysData.AgingTestData.Station_TC_Temp[itemp].data - SysData.AgingTestParameters.SetTemp.data > 30))
                    {
                        SysCtrl.FSD[itemp] =30000;
                    }
										else if  ((SysData.AgingTestData.Station_TC_Temp[itemp].data - SysData.AgingTestParameters.SetTemp.data > 50))
                    {
                        SysCtrl.FSD[itemp] =50000;
                    }											
//										else
//										{   
//											   if(SysData.AgingTestParameters.SetTemp.data>1100){  //如果设置温度大于110° 则不开风扇
//									         	SysCtrl.FSD[itemp] =8000;
//										      }
//												 else {
//												    SysCtrl.FSD[itemp] =2000;
//												 }
//											  
//										}
                }
                else if (SysData.AgingTestData.Station_TC_Temp[itemp].data < SysData.AgingTestParameters.SetTemp.data) // 温度小于设定温度
                {
//                    if ( (SysData.AgingTestParameters.SetTemp.data - SysData.AgingTestData.Station_TC_Temp[itemp].data < 30) )  //低于设定温度3°
//                    {
											   if(SysData.AgingTestParameters.SetTemp.data>1100){  //如果设置温度大于110° 则不开风扇
									         	SysCtrl.FSD[itemp] =2000;
										      }
												 else {
												    SysCtrl.FSD[itemp] =8000;
												 }											
//                    }
//										else 
//										{
//											SysCtrl.FSD[itemp] =0;
//										}
                }
                else
                {
                }

                if (SysCtrl.FSD[itemp] > 50000) // 设定风机上限
                {
                    SysCtrl.FSD[itemp] = 50000;
                }
                else if (SysCtrl.FSD[itemp] <= 2000) // 风机下限
                {
                    SysCtrl.FSD[itemp] = 0;
                }
            }
            FSD_cnt = 0;
            FSDSetup(SysCtrl.FSD[0], SysCtrl.FSD[1],SysCtrl.FSD[2], SysCtrl.FSD[3]);;
        }
    }
    else if (Flag == TEMP_COOL_MODE)
    { // 风机全开，加热棒全关，冷却控制
			
		    for (i = 0; i < 2; i++)
        {
				  SysCtrl.HOT[i] = 60000;  //加热棒关
					if (SysData.AgingTestData.Station_TC_Temp[i].data> 500)
					{
            SysCtrl.FSD[i] = 60000;
					}
					else
					{
						SysCtrl.FSD[i] = 0;
            						
					}
				}
					
        FSDSetup(SysCtrl.FSD[0], SysCtrl.FSD[1],SysCtrl.FSD[2], SysCtrl.FSD[3]);
        HOTSetup(SysCtrl.HOT[0], SysCtrl.HOT[1],SysCtrl.HOT[2], SysCtrl.HOT[3]);

				
    }
    else if (Flag == TEMP_STANDBY_MODE)
    { // 待机温度控制 
        for (i = 0; i < 2; i++)
        {
            SysCtrl.HOT[i] = 60000;
//            SysCtrl.FSD[i] = 0;
					  SysCtrl.FSD[i] = 0;
        }

        FSDSetup(SysCtrl.FSD[0], SysCtrl.FSD[1],SysCtrl.FSD[2], SysCtrl.FSD[3]);;
        HOTSetup(SysCtrl.HOT[0], SysCtrl.HOT[1],SysCtrl.HOT[2], SysCtrl.HOT[3]);
    }
}