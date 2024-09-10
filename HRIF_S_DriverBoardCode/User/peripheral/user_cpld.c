#include "user_cpld.h"

static void Cpld_GPIO_Init(void);
static uint16_t Cpld_Data[5];

#if USE_FSMC 

uint8_t Read_DIP_Switch(void)
{
  return *(uint8_t *)DIR_Switch_arr;
}

void CPLD_Ctrl(Latch_arr_t num, uint8_t data) 
{
  *(uint8_t *)(0x68000000 + num) = data;
}


void CPLD_SingleIo_Ctrl(CPLD_SingleCtrl ChanNo, uint8_t flag)
{
  volatile uint16_t bNo, bit;
	volatile uint16_t csbNo = 0;

	bNo = ChanNo / 8;
	bit = ChanNo % 8;

	if (flag == 1)
	{
		Cpld_Data[bNo] |= (0x01 << bit);
	}
	else
	{
		Cpld_Data[bNo] &= ~(0x01 << bit);
	}
	switch (bNo)
	{
	case ADDR_EN_DBUS:
		CPLD_Ctrl(EN_DBUS_ADDR, Cpld_Data[bNo]);
		break;
	case ADDR_CS_DBUS:
		CPLD_Ctrl(CS_DBUS_ADDR, Cpld_Data[bNo]);
		break;
	case ADDR_DATA_DBUS:
		CPLD_Ctrl(DATA_DBUS_ADDR, Cpld_Data[bNo]);
		break;
	case ADDR_PWR_DBUS:
		CPLD_Ctrl(PWR_CTLBUS_ADDR, Cpld_Data[bNo]);
		break;
	case ADDR_IO_DBUS:
		CPLD_Ctrl(IO_CTLBUS_ADDR, Cpld_Data[bNo]);
		break;
	default:
		break;
	}
} 

void CPLD_Init(void)
{
  Cpld_GPIO_Init();
	
  rcu_periph_clock_enable(RCU_EXMC);                                           // EXMC ʱ��ʹ��

  exmc_norsram_parameter_struct exmc_struct;                                //��EXMC NOR/SRAM��ʼ���ṹ��
	
  exmc_norsram_timing_parameter_struct nor_timing_init_struct;              //��EXMC NOR/SRAMʱ���ʼ���ṹ��
  nor_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;             //�첽����ģʽ��A��ʽ���롿
  nor_timing_init_struct.syn_data_latency = EXMC_DATALAT_2_CLK;             //�����ӳ١���һ��ͻ�����ʵ������ӳ�Ϊ2 EXMC_CLK��
  nor_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_DISABLE;   //ͬ��ʱ�ӷ�Ƶ�ȡ�EXMC_CLK���á�
  nor_timing_init_struct.bus_latency = 15;                                  //�����ӳ١�15��
  nor_timing_init_struct.asyn_data_setuptime = 50;                          //���ݽ���ʱ�䡾50��
  nor_timing_init_struct.asyn_address_holdtime = 15;                        //��ַ����ʱ�䡾15��
  nor_timing_init_struct.asyn_address_setuptime = 15;                       //��ַ����ʱ�䡾15��

  /*SRAM1*/
  exmc_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION0;   //ѡ��EXMC NOR/SRAM Region  ��bank0 NOR/SRAM region0��
  exmc_struct.write_mode = EXMC_ASYN_WRITE;                  //дģʽ���첽дģʽ��
  exmc_struct.extended_mode = ENABLE;                        //ʹ�ܻ��߽�����չģʽ��ʹ�ܡ�
  exmc_struct.asyn_wait = DISABLE;                           //ʹ�ܻ��߽����첽�ȴ����ܡ����ܡ�                      
  exmc_struct.nwait_signal = DISABLE;                        //��ͬ��ͻ��ģʽ�У�ʹ�ܻ��߽���NWAIT�źš����ܡ�
  exmc_struct.memory_write = ENABLE;                         //ʹ�ܻ��߽���д������ʹ�ܡ�
  exmc_struct.nwait_config = EXMC_NWAIT_CONFIG_DURING;       //����NWAIT�źš��ڵȴ�״̬�£�NWAIT�źŴ��ڻ״̬��
  exmc_struct.wrap_burst_mode = DISABLE;                     //ʹ�ܻ��߽��÷Ƕ������ģʽ�����ܡ�
  exmc_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;      //ָ��NWAIT�ļ��ԡ���ˮƽ�ǻ�Ծ��NWAIT��
  exmc_struct.burst_mode = DISABLE;                          //ʹ�ܻ��߽���ͻ��ģʽ�����ܡ�
  exmc_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;    //ָ���ⲿ�洢���������߿�ȡ�NOR���ݿ��Ϊ16λ��
  exmc_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;           //ָ���ⲿ�洢�������͡�SRAM,ROM��
  exmc_struct.address_data_mux = DISABLE;                    //������/��ַ�߸����Ƿ��á����ܡ�
  exmc_struct.read_write_timing = &nor_timing_init_struct;   //δ����չģʽʱ����ʱ�������дʱ��������������չģʽʱ����ʱ�����-->
  exmc_struct.write_timing = &nor_timing_init_struct;        //δ����չģʽʱ��дʱ�����-->
  exmc_norsram_init(&exmc_struct);                           //��ʼ��NOR/SRAM region
  exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION0);           //ʹ��bank0��EXMC NOR/SRAM region0

  /*SRAM2*/
  exmc_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION1;//ѡ��EXMC NOR/SRAM Region  ��bank0 NOR/SRAM region1��
  exmc_struct.write_mode = EXMC_ASYN_WRITE;               //дģʽ���첽дģʽ��
  exmc_struct.extended_mode = ENABLE;                     //ʹ�ܻ��߽�����չģʽ��ʹ�ܡ�
  exmc_struct.asyn_wait = DISABLE;                        //ʹ�ܻ��߽����첽�ȴ����ܡ����ܡ�
  exmc_struct.nwait_signal = DISABLE;                     //��ͬ��ͻ��ģʽ�У�ʹ�ܻ��߽���NWAIT�źš����ܡ�
  exmc_struct.memory_write = ENABLE;                      //ʹ�ܻ��߽���д������ʹ�ܡ�
  exmc_struct.nwait_config = EXMC_NWAIT_CONFIG_DURING;    //����NWAIT�źš��ڵȴ�״̬�£�NWAIT�źŴ��ڻ״̬��
  exmc_struct.wrap_burst_mode = DISABLE;                  //ʹ�ܻ��߽��÷Ƕ������ģʽ�����ܡ�
  exmc_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;   //ָ��NWAIT�ļ��ԡ���ˮƽ�ǻ�Ծ��NWAIT��
  exmc_struct.burst_mode = DISABLE;                       //ʹ�ܻ��߽���ͻ��ģʽ�����ܡ�
  exmc_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B; //ָ���ⲿ�洢���������߿�ȡ�NOR���ݿ��Ϊ16λ��
  exmc_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;        //ָ���ⲿ�洢�������͡�SRAM,ROM��
  exmc_struct.address_data_mux = DISABLE;                 //������/��ַ�߸����Ƿ��á����ܡ�
  exmc_struct.read_write_timing = &nor_timing_init_struct;
  exmc_struct.write_timing = &nor_timing_init_struct;
  exmc_norsram_init(&exmc_struct);                        //��ʼ��NOR/SRAM region
  exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION1);        //ʹ��bank0��EXMC NOR/SRAM region1

  /*SRAM3*/
  exmc_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION2;   //ѡ��EXMC NOR/SRAM Region  ��bank0 NOR/SRAM region2��
  exmc_struct.write_mode = EXMC_ASYN_WRITE;                  //дģʽ���첽дģʽ��
  exmc_struct.extended_mode = ENABLE;                        //ʹ�ܻ��߽�����չģʽ��ʹ�ܡ�
  exmc_struct.asyn_wait = DISABLE;                           //ʹ�ܻ��߽����첽�ȴ����ܡ����ܡ�
  exmc_struct.nwait_signal = DISABLE;                        //��ͬ��ͻ��ģʽ�У�ʹ�ܻ��߽���NWAIT�źš����ܡ�
  exmc_struct.memory_write = ENABLE;                         //ʹ�ܻ��߽���д������ʹ�ܡ�
  exmc_struct.nwait_config = EXMC_NWAIT_CONFIG_DURING;       //����NWAIT�źš��ڵȴ�״̬�£�NWAIT�źŴ��ڻ״̬��
  exmc_struct.wrap_burst_mode = DISABLE;                     //ʹ�ܻ��߽��÷Ƕ������ģʽ�����ܡ�
  exmc_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;      //ָ��NWAIT�ļ��ԡ���ˮƽ�ǻ�Ծ��NWAIT��
  exmc_struct.burst_mode = DISABLE;                          //ʹ�ܻ��߽���ͻ��ģʽ�����ܡ�
  exmc_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_8B;     //ָ���ⲿ�洢���������߿�ȡ�NOR���ݿ��Ϊ8λ��
  exmc_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;           //ָ���ⲿ�洢�������͡�SRAM,ROM��
  exmc_struct.address_data_mux = DISABLE;                    //������/��ַ�߸����Ƿ��á����ܡ�
  exmc_struct.read_write_timing = &nor_timing_init_struct;
  exmc_struct.write_timing = &nor_timing_init_struct;
  exmc_norsram_init(&exmc_struct);                           //��ʼ��NOR/SRAM region
  exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION2);           //ʹ��bank0��EXMC NOR/SRAM region2

  /*SRAM4*/
  exmc_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION3;   //ѡ��EXMC NOR/SRAM Region  ��bank0 NOR/SRAM region3��
  exmc_struct.write_mode = EXMC_ASYN_WRITE;                  //дģʽ���첽дģʽ��
  exmc_struct.extended_mode = ENABLE;                        //ʹ�ܻ��߽�����չģʽ��ʹ�ܡ�
  exmc_struct.asyn_wait = DISABLE;                           //ʹ�ܻ��߽����첽�ȴ����ܡ����ܡ�
  exmc_struct.nwait_signal = DISABLE;                        //��ͬ��ͻ��ģʽ�У�ʹ�ܻ��߽���NWAIT�źš����ܡ�
  exmc_struct.memory_write = ENABLE;                         //ʹ�ܻ��߽���д������ʹ�ܡ�
  exmc_struct.nwait_config = EXMC_NWAIT_CONFIG_DURING;       //����NWAIT�źš��ڵȴ�״̬�£�NWAIT�źŴ��ڻ״̬��
  exmc_struct.wrap_burst_mode = DISABLE;                     //ʹ�ܻ��߽��÷Ƕ������ģʽ�����ܡ�
  exmc_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;      //ָ��NWAIT�ļ��ԡ���ˮƽ�ǻ�Ծ��NWAIT��
  exmc_struct.burst_mode = DISABLE;                          //ʹ�ܻ��߽���ͻ��ģʽ�����ܡ�
  exmc_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;    //ָ���ⲿ�洢���������߿�ȡ�NOR���ݿ��Ϊ16λ��
  exmc_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;           //ָ���ⲿ�洢�������͡�SRAM,ROM��
  exmc_struct.address_data_mux = DISABLE;                    //������/��ַ�߸����Ƿ��á����ܡ�
  exmc_struct.read_write_timing = &nor_timing_init_struct;
  exmc_struct.write_timing = &nor_timing_init_struct;
  exmc_norsram_init(&exmc_struct);                           //��ʼ��NOR/SRAM region
  exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION3);           //ʹ��bank0��EXMC NOR/SRAM region3

 //  CPLD_Ctrl(FPGAIO_T4A_ADDR, 0x00);
  CPLD_Ctrl(FPGAIO_T4A_ADDR, 0x01);
  CPLD_Ctrl(EN_DBUS_ADDR, 0xff);
  // CPLD_Ctrl(EN_DBUS_ADDR, 0x00);
  CPLD_Ctrl(CS_DBUS_ADDR, 0xff);
  // CPLD_Ctrl(CS_DBUS_ADDR, 0x00);
  CPLD_Ctrl(DATA_DBUS_ADDR, 0xff);
  // CPLD_Ctrl(DATA_DBUS_ADDR, 0x00);
  CPLD_Ctrl(PWR_CTLBUS_ADDR, 0xff);
  //  CPLD_Ctrl(PWR_CTLBUS_ADDR, 0x00);
  CPLD_Ctrl(IO_CTLBUS_ADDR, 0xff);
  // CPLD_Ctrl(IO_CTLBUS_ADDR, 0x00);
  CPLD_Ctrl(FPGAIO_T4B_ADDR, 0xff);
  CPLD_Ctrl(FPGAIO_T6A_ADDR, 0xff);
  CPLD_Ctrl(FPGAIO_T6B_ADDR, 0xff);
  CPLD_Ctrl(FPGAIO_T12A_ADDR, 0xff);
  CPLD_Ctrl(FPGAIO_T17A_ADDR, 0xff);
  CPLD_Ctrl(FPGAIO_T18B_ADDR, 0xff);
}

static void Cpld_GPIO_Init(void)
{
  // GPIO AF12

  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);
  rcu_periph_clock_enable(RCU_GPIOD);
  rcu_periph_clock_enable(RCU_GPIOE);
  rcu_periph_clock_enable(RCU_GPIOF);
  rcu_periph_clock_enable(RCU_GPIOG);

// XD0-15
// PD
#if 1
  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0);

  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_1);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);

  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_8);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);

  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_9);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);

  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_10);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);

  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_14);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_14);

  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_15);
  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_15);

  // PE
  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_7);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_8);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_9);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_10);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_11);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_12);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_13);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_13);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_14);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_14);

  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_15);
  gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
  gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_15);

  // PF(XA0-4)
  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_0);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0);

  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_1);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);

  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_2);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);

  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_3);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);
  // address 5
  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_5);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
  // address 6
  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_12);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);
  // address 7
  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_13);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_13);
  // address 8
  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_14);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_14);

  // address 9
  gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_15);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_15);

  // address 10
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_0);
  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0);

  // address 11
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_1);
  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_1);
  // address 12
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_2);
  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);
  // address 13
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_3);
  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);
  // address 14
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_4);
  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4);
  // address 15
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_5);
  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
  // CTRL

  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4);
  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_4);

  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_5);

  gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
  gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);
  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_7);

  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_9);

  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_10);

  gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
  gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);
  gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_12);

#endif
}

void IP_Reset(void)
{
   CPLD_Ctrl(FPGAIO_T4A_ADDR, 0x00);
}

#endif
