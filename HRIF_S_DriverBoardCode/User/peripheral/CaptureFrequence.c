#include "main.h"

/******************************************************************
函数名称：void getSysFreq(void)
函数功能: 获取系统频率函数
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void getSysFreq(void)
{
  int i = 0;
  int sys = 0;
  int ahb = 0;
  int apb1 = 0;
  int apb2 = 0;
 	
  i = rcu_system_clock_source_get();	//当前所用的系统时钟  
  sys = rcu_clock_freq_get(CK_SYS);
  ahb = rcu_clock_freq_get(CK_AHB);
  apb1 = rcu_clock_freq_get(CK_APB1);
  apb2 = rcu_clock_freq_get(CK_APB2);
 
}

/******************************************************************
函数名称：void FB_OUT_GPIOInit(void)
函数功能: 捕获频率GPIO初始化
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void FB_OUT_GPIOInit(void)
{
/* 
** FB_OUT3---B62---PA11 ---TIMER0_CH3--AF1
** FB_OUT1---B64---PA10 ---TIMER0_CH2--AF1
** FB_OUT4---B61---PB10 ---TIMER1_CH2--AF1
** FB_OUT2---B63---PA9  ---TIMER0_CH1--AF1
*/
	rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
	
	/* FB_OUT4 TIMER1_CH2*/
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_10);	
	
	/* FB_OUT1 TIMER0_CH2*/
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);	
	
	/* FB_OUT2 TIMER0_CH1*/
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);	

	/* FB_OUT3 TIMER0_CH3*/
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11);
}

/******************************************************************
函数名称：void FB_OUT_TimerInit(void)
函数功能: 捕获频率时钟初始化
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void FB_OUT_TimerInit(uint16_t FB_OUT)
{
	FB_OUT_GPIOInit();
	
	timer_ic_parameter_struct timer_icinitpara;
  timer_parameter_struct timer_initpara;
	switch(FB_OUT)
	{
	  case FB_OUT1:  /* TIMER0_CH2 */
							rcu_periph_clock_enable(RCU_TIMER0);
							rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
							timer_deinit(TIMER0);
							
							timer_initpara.prescaler = (120) - 1;             //psc
							timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
							timer_initpara.counterdirection = TIMER_COUNTER_UP;
							timer_initpara.period = 65535;                        //arr
							timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
							timer_initpara.repetitioncounter = 0;
							timer_init(TIMER0, &timer_initpara);
							
							timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;              //输入捕获上升沿
							timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
							timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
							timer_icinitpara.icfilter = 0x0;
						//	timer_input_capture_config(TIMER0, TIMER_CH_0, &timer_icinitpara);   //配置TIMERx输入捕获参数  
//							timer_input_capture_config(TIMER0, TIMER_CH_1, &timer_icinitpara);
							timer_input_capture_config(TIMER0, TIMER_CH_2, &timer_icinitpara);
//							timer_input_capture_config(TIMER0, TIMER_CH_3, &timer_icinitpara);
							
							timer_auto_reload_shadow_enable(TIMER0);
							timer_interrupt_flag_clear(TIMER0, TIMER_INT_CH2);
							timer_interrupt_enable(TIMER0, TIMER_INT_CH2);
							timer_enable(TIMER0);
							nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
							
							nvic_irq_enable(TIMER0_Channel_IRQn, 5, 0);
							nvic_irq_disable(TIMER1_IRQn);
		break;
	  case FB_OUT2:
		break;
	  case FB_OUT3:
		break;
	  case FB_OUT4:
		break;		
	}
	
	

}
/******************************************************************
函数名称：void TIMERXX_IRQHandler(void)  TIMER0_Channel_IRQHandler
函数功能: 捕获频率中断配置
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void TIMER0_Channel_IRQHandler(void)
{
  
	
	
}