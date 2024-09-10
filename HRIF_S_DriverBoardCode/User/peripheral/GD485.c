#include "main.h"
uint16_t ReceiveCount=0;
uint8_t  ReceiveBuffer[10]={0};
/******************************************************************
�������ƣ����ڳ�ʼ��
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void USART_Init(void)
{	
/*
*  �ͻ�Ԥ��485ͨ�� ���ó�USART����
*  SCIRXD --  B41  -- PC11  USART2_Rx<--> AF7   ��ɫ
*  SCITXD --  B44  -- PC10  USART2_Tx<--> AF7   ��ɫ
*  485_EN --  B46  -- PG13
*/
	rcu_periph_clock_enable(RCU_GPIOG);
	Communication_L();	
	gpio_mode_set(Communication_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, Communication_Pin);
	gpio_output_options_set(Communication_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Communication_Pin);
	gpio_af_set(Communication_Port, GPIO_AF_15, Communication_Pin);
	

	rcu_periph_clock_enable(RCU_GPIOC);	
	
	/* connect port to USARTx_Tx */
	gpio_af_set(GPIOC, GPIO_AF_7, GPIO_PIN_10);

	/* connect port to USARTx_Rx */
	gpio_af_set(GPIOC, GPIO_AF_7, GPIO_PIN_11);		
	
	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

	/* USART2 configure */
	rcu_periph_clock_enable(RCU_USART2);
	usart_deinit(USART2);
	usart_baudrate_set(USART2, 9600U);
	usart_parity_config(USART2, USART_PM_NONE);   // ��У��
	usart_word_length_set(USART2, USART_WL_8BIT); // 8λ
	usart_stop_bit_set(USART2, USART_STB_1BIT);   // 1λ
	
	usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE); //����RTS
	usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE); //��Ӳ������������

	usart_receive_config(USART2, USART_RECEIVE_ENABLE);        //ʹ�ܽ���
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);      //ʹ�ܷ���
	usart_interrupt_enable(USART2, USART_INT_RT);
	usart_enable(USART2);
	
	nvic_irq_enable(USART2_IRQn, 5, 0);
	usart_interrupt_enable(USART2, USART_INT_RBNE);         /* ʹ��USART2�����ǿ��ж� */  
	usart_interrupt_enable(USART2, USART_INT_IDLE);         /* ʹ��USART2�����ж� */

}

/******************************************************************
�������ƣ�485��������
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
int RS485_SendData(uint8_t *buf, uint32_t len)
{   
	Communication_H();	// ����Ϊ����ģʽ
	vTaskDelay(200);
    while(len--)
	{
		while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));	// �ȴ�������Ϊ��
		usart_data_transmit(USART2, *(buf++) );  		// ��������
    }
	while(RESET == usart_flag_get(USART2, USART_FLAG_TC));	// ��������ж�
		
	ReceiveCount = 0;	
		
	Communication_L();		// ����Ϊ����ģʽ
	vTaskDelay(200);
		
	return 0;
}


/******************************************************************
�������ƣ�USART�жϽ�������
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void USART2_IRQHandler(void)
{   
  if((RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)) && (RESET != usart_flag_get(USART2, USART_FLAG_RBNE)))// ���յ�����
	 {	
		  ReceiveBuffer[ReceiveCount++] = usart_data_receive(USART2);
		  usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE); //���жϱ�־
	 }
	else if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE)) {
	   usart_interrupt_flag_clear(USART2, USART_INT_FLAG_IDLE);
		 usart_data_receive(USART2); /* ���������ɱ�־λ */
	}
}

/******************************************************************
�������ƣ�USART�жϽ�������
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void RS485Test(void)
{
	uint8_t send_buf[3] ={0x01,0x00,0x02};
	RS485_SendData(send_buf,3);
}
