#include "main.h"
uint16_t ad_retval1[10] = {0};
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
void Init_AD7606GPIO()
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);

    gpio_mode_set(AD7606RESET_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD7606RESET_PIN);
    gpio_output_options_set(AD7606RESET_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD7606RESET_PIN);
		gpio_af_set(AD7606RESET_PORT,GPIO_AF_15,AD7606RESET_PIN);
	
    gpio_mode_set(AD7606CLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD7606CLK_PIN);
    gpio_output_options_set(AD7606CLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD7606CLK_PIN);
		gpio_af_set(AD7606CLK_PORT,GPIO_AF_15,AD7606CLK_PIN);
	
    gpio_mode_set(AD7606CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD7606CS_PIN);
    gpio_output_options_set(AD7606CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD7606CS_PIN);
		gpio_af_set(AD7606CS_PORT,GPIO_AF_15,AD7606CS_PIN);
	
    gpio_mode_set(AD7606CONV_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD7606CONV_PIN);
    gpio_output_options_set(AD7606CONV_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD7606CONV_PIN);
		gpio_af_set(AD7606CONV_PORT,GPIO_AF_15,AD7606CONV_PIN);
		
    gpio_mode_set(AD7606_SDO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AD7606_SDO_PIN);
	
    // AD7606RESET_HIGH;
    AD7606RESET_LOW;

    // AD7606CLK_HIGH;
    AD7606CLK_LOW;

    // AD7606CS_HIGH;
    AD7606CS_LOW;

    // AD7606CONV_HIGH;
    AD7606CONV_LOW;
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
void AD7606Reset()
{
    uint16_t i;

    AD7606RESET_HIGH; // �ߵ�ƽʱ����С50ns
    i = 20;
    while (i--)
        ;
    AD7606RESET_LOW;
    i = 20;
    while (i--)
        ;
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
void Init_AD7606()
{
    AD7606RESET_LOW;
    AD7606CS_HIGH;
    AD7606CONV_HIGH;
    AD7606CLK_HIGH;
    AD7606Reset();
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
void AD7606ReadOneSample(uint16_t *iRdData)
{
    uint16_t i = 0, j = 0, k;
    uint16_t Temp = 0;

//    AD7606Reset();
    AD7606CONV_LOW;
    user_delayUs(100);
    AD7606CONV_HIGH;

    user_delayUs(5);
    AD7606CS_LOW;
    user_delayUs(5);
    for (i = 0; i < 128; i++) // 64  --- -4
    {
        j = i / 16;
        AD7606CLK_LOW;
        k = 20;
        while (k--)
            ;
        Temp = AD7606DO;
        iRdData[j] <<= 1;
        if (Temp)
        {
            iRdData[j] |= 1;
        }
        AD7606CLK_HIGH;

        k = 20;
        while (k--)
            ;
    }
    i = 20;
    while (i--)
        ;
    AD7606CS_HIGH;
    i = 20;
    while (i--)
        ;
}



/******************************************************************
�������ƣ�
��������:���� ������32λ
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
float SortValue(float *dpoint, uint16_t num)
{
    uint16_t aTemp, bTemp;
    float  tempcount;
    // ����
    for (aTemp = 0; aTemp < num; aTemp++)
    {
        for (bTemp = aTemp + 1; bTemp < num; bTemp++)
        {
            if (dpoint[aTemp] > dpoint[bTemp])
            {
                tempcount = dpoint[aTemp];
                dpoint[aTemp] = dpoint[bTemp];
                dpoint[bTemp] = tempcount;
            }
        }
    }

}



/******************************************************************
�������ƣ�
��������:ƽ���˲�����,������32λ
������������ݵ��׵�ַ�����ݳ��ȣ���ȥ���ݵĸ���,ǰabandon���ͺ�abandon��
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
float DigitalFilter_f32(float *dpoint, uint16_t num, uint16_t abandon)
{
    uint16_t aTemp, bTemp;
    float addtemp, tempcount, ctemp;
    // ����
    for (aTemp = 0; aTemp < num; aTemp++)
    {
        for (bTemp = aTemp + 1; bTemp < num; bTemp++)
        {
            if (dpoint[aTemp] > dpoint[bTemp])
            {
                tempcount = dpoint[aTemp];
                dpoint[aTemp] = dpoint[bTemp];
                dpoint[bTemp] = tempcount;
            }
        }
    }

    // �ۼ�
    addtemp = 0.0;
    for (aTemp = abandon; aTemp < num - abandon; aTemp++)
    {
        addtemp += dpoint[aTemp]; // ���
    }
    ctemp = num - (abandon * 2);
    addtemp = addtemp / ctemp;

    return (addtemp);
}




/******************************************************************
�������ƣ�
��������:ƽ���˲�����,������32λ
������������ݵ��׵�ַ�����ݳ��ȣ���ȥ���ݵĸ���,ǰabandon���ͺ�abandon��
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
float DigitalFilter_f32ID(float *dpoint, uint16_t num, uint16_t abandon)
{
    uint16_t aTemp, bTemp;
    float addtemp, tempcount, ctemp;
    // ����
//    for (aTemp = 0; aTemp < num; aTemp++)
//    {
//        for (bTemp = aTemp + 1; bTemp < num; bTemp++)
//        {
//            if (dpoint[aTemp] > dpoint[bTemp])
//            {
//                tempcount = dpoint[aTemp];
//                dpoint[aTemp] = dpoint[bTemp];
//                dpoint[bTemp] = tempcount;
//            }
//        }
//    }

    // �ۼ�
    addtemp = 0.0;
	  aTemp = 0;
    for (aTemp = abandon; aTemp < num - abandon; aTemp++)
    {
        addtemp += dpoint[aTemp]; // ���
    }
    ctemp = num - (abandon * 2);
    addtemp = addtemp / ctemp;

    return (addtemp);
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
void AD7606Test(void)
{
	uint16_t i, u16_sampcnt = 10, u16_abandon = 3;
  uint32_t atemp;	
	float ad_retval[10], btemp;;
	
	Init_AD7606GPIO(); // AD����
	Init_AD7606();
  while(1)
  {
    for (i = 0; i < u16_sampcnt; i++)
    {
        AD7606ReadOneSample(&ad_retval1[0]);
        atemp = ad_retval1[1];                //ͨ��1��0-7��
        ad_retval[i] = (float)(atemp);
    }     	 
		btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
    btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //�����ѹmV
		
		/*
		 0-32768֮�����ֵ����ֵ�������������ѹ��ֱ��*VOLTAGE_MINIMUM_SCALE
		 ����32768����65535-ȥ��ֵ��*VOLTAGE_MINIMUM_SCALE ������Ǹ���ѹ
		
		*/
  }

}
