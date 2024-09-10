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
/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void time1_init(void)
{
	//nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);//�⺯��bug
  rcu_periph_clock_enable(RCU_TIMER1); // ע�������ǰ���д�RCU����
  //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
  timer_deinit(TIMER1);

  timer_internal_clock_config(TIMER1); // ѡ���ڲ�ʱ��CK_TIMER(200M)

  /* TIMER1����Ϊ0.1ms��ʱ */
  timer_parameter_struct timer_initpara;
  timer_struct_para_init(&timer_initpara);

  timer_initpara.prescaler = 1;
  timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // ����prescaler,clockdivision���ոö�ʱ��ʱ������Ϊ1M
  timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // ������ʽ���ø��ݱ��ؾ���
  timer_initpara.counterdirection = TIMER_COUNTER_UP; // ��������
  timer_initpara.period = 59999;                        // ����Ϊ0.1ms����һ��
  timer_initpara.repetitioncounter = 0;
  timer_init(TIMER1, &timer_initpara);
	
  timer_auto_reload_shadow_enable(TIMER1);
	
  timer_interrupt_enable(TIMER1,TIMER_INT_UP);
	
  nvic_irq_enable(TIMER1_IRQn,5,0);
	
  timer_disable(TIMER1);
  //timer_enable(TIMER1);
  
}
/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void TIMER1_IRQHandler(void)
{
   if(SET==timer_interrupt_flag_get(TIMER1,TIMER_INT_UP))
   {
	   LED1_TOGGLE;
        
      timer_interrupt_flag_clear(TIMER1,TIMER_INT_UP);
   }
   
}

  

