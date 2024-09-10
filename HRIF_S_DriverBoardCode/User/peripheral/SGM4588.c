#include "main.h"

/****************************************
*  ��2.5 �� VREF                 0  0  0       0X00
*  ��1.25 �� VREF                0  0  1       0X01
*  ��0.625 �� VREF               0  1  0       0X02
*  0 to 2.5 �� VREF             1  0  0       0X04
*  0 to 1.25 �� VREF            1  1  0       0X06


*    case  ��2.5 �� VREF :    
      read_voltage=  (((read_data-32768)*AD8688_BENCHMARK_VOLTAGE*2500)/32768.0f)     ;break;
			
*    case  ��1.25 �� VREF:  
      read_voltage=  (((read_data-32768)*AD8688_BENCHMARK_VOLTAGE*1250)/32768.0f)     ;break;
			
*    case  ��0.625 �� VREF:  
      read_voltage=  (((read_data-32768)*AD8688_BENCHMARK_VOLTAGE*625)/32768.0f)      ;break;
			
*    case 0 to 2.5 �� VREF:         
      read_voltage=   ((read_data*AD8688_BENCHMARK_VOLTAGE*2500)/65536.0f)+0.5f        ;break;
			
*    case 0 to 1.25 �� VREF:        
      read_voltage=   ((read_data*AD8688_BENCHMARK_VOLTAGE*1250)/65536.0f)+0.5f        ;break;

*   �ڶ��׸ı������Χ�����ף�����ı������Χѡ��ڶ���
    �����ķ�Χ������������	Init_Ads8688();��������ڲ����õ�����Range_Info[8]��ֵ ������0X00��Ӧ�ľ�������� ��2.5 �� VREF ��0X01 ��Ӧ ��1.25 �� VREF
		 ���ݴ˸��ķ�Χ���ɣ���ע�����Ķ�ȡ�����ѹֵҲ��Ҫ��Ӧ���ģ�˫�����ǵ����Լ���ͬ�ķ�ΧӰ��������ȣ�

*   ADS8688.h�ļ��ж����#define first_ADS8688��#define second_ADS8688 ֻ����������һ������ѡ��ʹ����һ��
*****************************************/

/*
** ADD1---B2---PF10
** ADD2---B4---PC2
** ADD3---B3---PC3
** SW_EN_WDB---B1 ---PC0    --- �¶Ȱ�
** SW_EN_DYB---B53---PD11   --- ��Դ��
*/
void SGM4588init(void)
{
	rcu_periph_clock_enable(RCU_GPIOF);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);

	gpio_mode_set(ADD1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ADD1_PIN); // ADD1
	gpio_output_options_set(ADD1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ADD1_PIN);	
	gpio_af_set(ADD1_GPIO_Port,GPIO_AF_15,ADD1_PIN);
	
	gpio_mode_set(ADD2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ADD2_PIN); // ADD2
	gpio_output_options_set(ADD2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ADD2_PIN);	
	gpio_af_set(ADD2_GPIO_Port,GPIO_AF_15,ADD2_PIN);
	
	gpio_mode_set(ADD3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ADD3_PIN); // ADD3
	gpio_output_options_set(ADD3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ADD3_PIN);	
	gpio_af_set(ADD3_GPIO_Port,GPIO_AF_15,ADD3_PIN);
	
	gpio_mode_set(SW_EN_WDB_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SW_EN_WDB_PIN);
	gpio_output_options_set(SW_EN_WDB_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SW_EN_WDB_PIN);	
	gpio_af_set(SW_EN_WDB_GPIO_Port,GPIO_AF_15,SW_EN_WDB_PIN);
	
	gpio_mode_set(SW_EN_DYB_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SW_EN_DYB_PIN);
	gpio_output_options_set(SW_EN_DYB_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SW_EN_DYB_PIN);	
	gpio_af_set(SW_EN_DYB_GPIO_Port,GPIO_AF_15,SW_EN_DYB_PIN);

	ADD1_L;
	ADD2_L;
	ADD3_L; // 000

	SW_EN_WDB_L;
	SW_EN_DYB_L;
  	// ȫ������
}

/*ѡ��ͨ����ʹ��*/
void SelectSGM4588Bit(uint16_t ChanNo)
{
    switch (ChanNo)
	{
	// ADD3    ADD2    ADD1
	case 0:
		ADD3_L;
		ADD2_L;
		ADD1_L;
	  vTaskDelay(10);
		break; // 000  0
	case 1:
		ADD3_L;
		ADD2_L;
		ADD1_H;
  	vTaskDelay(10);
		break; // 001  1
	case 2:
		ADD3_L;
		ADD2_H;
		ADD1_L;
	  vTaskDelay(10);
		break; // 010  2
	case 3:
		ADD3_L;
		ADD2_H;
		ADD1_H;
  	vTaskDelay(10);
		break; // 011  3
	case 4:
		ADD3_H;
		ADD2_L;
		ADD1_L;
	  vTaskDelay(10);
		break; // 100  4
	case 5:
		ADD3_H;
		ADD2_L;
		ADD1_H;
  	vTaskDelay(10); 
		break; // 101  5
	case 6:
		ADD3_H;
		ADD2_H;
		ADD1_L;
  	vTaskDelay(10);
		break; // 110  6
	case 7:
		ADD3_H;
		ADD2_H;
		ADD1_H;
	  vTaskDelay(10); 
		break; // 111  7
	default:
		break;
	}
}


void Select_Channel(uint16_t Channel,uint16_t mode)
{
	if(mode == WDB){
		SelectSGM4588Bit(Channel);
		vTaskDelay(5);
		SW_EN_WDB_H;
		vTaskDelay(5);
	}
	
	else if (mode == DYB){
		SelectSGM4588Bit(Channel);
		vTaskDelay(1);
		SW_EN_DYB_H;
		vTaskDelay(5);
	}
//	SW_EN_WDB_L;  
//	SW_EN_DYB_L;      	//ʹ�ú�Ҫ���� ����������
}

