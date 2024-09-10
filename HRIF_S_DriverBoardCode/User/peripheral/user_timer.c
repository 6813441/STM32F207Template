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
void time1_init(void)
{
	//nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);//库函数bug
  rcu_periph_clock_enable(RCU_TIMER1); // 注意参数名前面有待RCU。。
  //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
  timer_deinit(TIMER1);

  timer_internal_clock_config(TIMER1); // 选择内部时钟CK_TIMER(200M)

  /* TIMER1配置为0.1ms定时 */
  timer_parameter_struct timer_initpara;
  timer_struct_para_init(&timer_initpara);

  timer_initpara.prescaler = 1;
  timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 根据prescaler,clockdivision最终该定时器时钟评率为1M
  timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 触发方式设置根据边沿决定
  timer_initpara.counterdirection = TIMER_COUNTER_UP; // 计数向上
  timer_initpara.period = 59999;                        // 设置为0.1ms触发一次
  timer_initpara.repetitioncounter = 0;
  timer_init(TIMER1, &timer_initpara);
	
  timer_auto_reload_shadow_enable(TIMER1);
	
  timer_interrupt_enable(TIMER1,TIMER_INT_UP);
	
  nvic_irq_enable(TIMER1_IRQn,5,0);
	
  timer_disable(TIMER1);
  //timer_enable(TIMER1);
  
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
void TIMER1_IRQHandler(void)
{
   if(SET==timer_interrupt_flag_get(TIMER1,TIMER_INT_UP))
   {
	   LED1_TOGGLE;
        
      timer_interrupt_flag_clear(TIMER1,TIMER_INT_UP);
   }
   
}

  

