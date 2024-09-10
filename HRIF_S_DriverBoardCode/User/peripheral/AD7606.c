#include "main.h"
uint16_t ad_retval1[10] = {0};
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
void AD7606Reset()
{
    uint16_t i;

    AD7606RESET_HIGH; // 高电平时间最小50ns
    i = 20;
    while (i--)
        ;
    AD7606RESET_LOW;
    i = 20;
    while (i--)
        ;
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
void Init_AD7606()
{
    AD7606RESET_LOW;
    AD7606CS_HIGH;
    AD7606CONV_HIGH;
    AD7606CLK_HIGH;
    AD7606Reset();
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
函数名称：
函数功能:排序 浮点数32位
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float SortValue(float *dpoint, uint16_t num)
{
    uint16_t aTemp, bTemp;
    float  tempcount;
    // 排序
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
函数名称：
函数功能:平均滤波函数,浮点数32位
输入参数：数据的首地址，数据长度，舍去数据的个数,前abandon个和后abandon个
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float DigitalFilter_f32(float *dpoint, uint16_t num, uint16_t abandon)
{
    uint16_t aTemp, bTemp;
    float addtemp, tempcount, ctemp;
    // 排序
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

    // 累加
    addtemp = 0.0;
    for (aTemp = abandon; aTemp < num - abandon; aTemp++)
    {
        addtemp += dpoint[aTemp]; // 求和
    }
    ctemp = num - (abandon * 2);
    addtemp = addtemp / ctemp;

    return (addtemp);
}




/******************************************************************
函数名称：
函数功能:平均滤波函数,浮点数32位
输入参数：数据的首地址，数据长度，舍去数据的个数,前abandon个和后abandon个
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
float DigitalFilter_f32ID(float *dpoint, uint16_t num, uint16_t abandon)
{
    uint16_t aTemp, bTemp;
    float addtemp, tempcount, ctemp;
    // 排序
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

    // 累加
    addtemp = 0.0;
	  aTemp = 0;
    for (aTemp = abandon; aTemp < num - abandon; aTemp++)
    {
        addtemp += dpoint[aTemp]; // 求和
    }
    ctemp = num - (abandon * 2);
    addtemp = addtemp / ctemp;

    return (addtemp);
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
void AD7606Test(void)
{
	uint16_t i, u16_sampcnt = 10, u16_abandon = 3;
  uint32_t atemp;	
	float ad_retval[10], btemp;;
	
	Init_AD7606GPIO(); // AD采样
	Init_AD7606();
  while(1)
  {
    for (i = 0; i < u16_sampcnt; i++)
    {
        AD7606ReadOneSample(&ad_retval1[0]);
        atemp = ad_retval1[1];                //通道1（0-7）
        ad_retval[i] = (float)(atemp);
    }     	 
		btemp = DigitalFilter_f32(ad_retval, u16_sampcnt, u16_abandon);
    btemp = btemp * VOLTAGE_MINIMUM_SCALE;   //算出电压mV
		
		/*
		 0-32768之间的码值是正值，算出的是正电压，直接*VOLTAGE_MINIMUM_SCALE
		 大于32768的用65535-去该值再*VOLTAGE_MINIMUM_SCALE 算出的是负电压
		
		*/
  }

}
