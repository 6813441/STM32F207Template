#include "main.h"

const unsigned int ADS8688_CH_TABLE[8] = {0xc000, 0xc400, 0xc800, 0xcc00, 0xd000, 0xd400, 0xd800, 0xdc00};

/******************************************************************
函数名称：Init_AD8688Gpio
函数功能:Init_AD8688Gpio初始化
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Init_AD8688Gpio(void)
{
	rcu_periph_clock_enable(RCU_GPIOx(AD8688_RST_PT));
  rcu_periph_clock_enable(RCU_GPIOx(AD8688_SDI_PT));
  rcu_periph_clock_enable(RCU_GPIOx(AD8688_SCLK_PT));
  rcu_periph_clock_enable(RCU_GPIOx(AD8688_SDO_PT));
  rcu_periph_clock_enable(RCU_GPIOx(AD8688_CS_PT));
	
	gpio_mode_set(AD8688_CS_PORT,GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD8688_CS_PIN);
  gpio_output_options_set(AD8688_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD8688_CS_PIN);	
	gpio_af_set(AD8688_CS_PORT,GPIO_AF_15,AD8688_CS_PIN);

  gpio_mode_set(AD8688_SCLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD8688_SCLK_PIN);
  gpio_output_options_set(AD8688_SCLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD8688_SCLK_PIN);	
	gpio_af_set(AD8688_SCLK_PORT,GPIO_AF_15,AD8688_SCLK_PIN);
	

  gpio_mode_set(AD8688_SDI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD8688_SDI_PIN);
  gpio_output_options_set(AD8688_SDI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD8688_SDI_PIN);	
	gpio_af_set(AD8688_SDI_PORT,GPIO_AF_15,AD8688_SDI_PIN);
	

  gpio_mode_set(AD8688_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD8688_RST_PIN);
  gpio_output_options_set(AD8688_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD8688_RST_PIN);	
	gpio_af_set(AD8688_RST_PORT,GPIO_AF_15,AD8688_RST_PIN);	
	

  gpio_mode_set(AD8688_SDO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AD8688_SDO_PIN);
  gpio_output_options_set(AD8688_SDO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, AD8688_SDO_PIN);	
	gpio_af_set(AD8688_SDO_PORT,GPIO_AF_15,AD8688_SDO_PIN);		
		
}
/******************************************************************
函数名称：Init_AD8688
函数功能:Init_AD8688初始化
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Init_AD8688(void)
{
    AD8688_CS_H;
    AD8688_SCLK_H;
    AD8688_SDI_L;
    ADS8688_Reset();
    // 第一路-第八路采样范围
    Write_AD8688(0x0B00);
    Write_AD8688(0x0D00);
    Write_AD8688(0x0F00);
    Write_AD8688(0x1100);
    Write_AD8688(0x1300);
    Write_AD8688(0x1500);
    Write_AD8688(0x1700);
    Write_AD8688(0x1900);
}
/******************************************************************
函数名称：ADS8688_Reset
函数功能：8688重置
输入参数：
输出参数：
返回值：
注意：rst引脚拉低拉高，实现芯片重置
******************************************************************/
void ADS8688_Reset(void)
{
    AD8688_RST_H;
    user_delayUs(5000);
    AD8688_RST_L;
    user_delayUs(15000);
    AD8688_RST_H;
    user_delayUs(50000);
}
/******************************************************************
函数名称：Write_AD8688
函数功能：8688命令写入
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void Write_AD8688(uint16_t data)
{
    uint16_t i, j;
    AD8688_CS_H;
    user_delayUs(1000);
    AD8688_CS_L; // 片选信号拉低

    for (i = 0; i < 16; i++) // 写入16位命令
    {
        if ((data << i) & 0x8000)
        {
            AD8688_SDI_H; // din=1
        }
        else
        {
            AD8688_SDI_L; // din=0
        }

        AD8688_SCLK_H; // CLK=1
        j = 5;
        while (j--)
            ;
        AD8688_SCLK_L; // CLK =0
        j = 5;
        while (j--)
            ;
    }
    for (i = 0; i < 16; i++) // 后16个时钟为读取数据，写入命令是直接抛掉
    {
        AD8688_SCLK_H; // CLK=1
        j = 5;
        while (j--)
            ;
        AD8688_SCLK_L; // CLK =0
        j = 5;
        while (j--)
            ;
    }

    AD8688_CS_H; // 片选信号拉高
    AD8688_SCLK_H;
}

/******************************************************************
函数名称：Read_AD8688
函数功能：读取8688管脚
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
uint16_t Read_AD8688(void)
{
    uint16_t i, Result = 0, j;
    AD8688_CS_H;
    user_delayUs(2000);
    AD8688_CS_L; // 片选信号拉低

    for (i = 0; i < 16; i++) // 前16个时钟为写入命令，读取时直接抛掉
    {
        AD8688_SDI_L;  // 确保读取时，写入命令为0；
        AD8688_SCLK_H; // CLK=1
        j = 5;
        while (j--)
            ;
        AD8688_SCLK_L; // CLK =0
        j = 5;
        while (j--)
            ;
    }
    for (i = 0; i < 16; i++) // 读取十六位数据
    {
        AD8688_SCLK_H; // CLK =1
        j = 5;
        while (j--)
            ;
        Result <<= 1;
        if (AD8688_SDO == 1)
        {
            Result++;
        }
        AD8688_SCLK_L; // CLK =0
        j = 5;
        while (j--)
            ;
    }

    AD8688_CS_H; // 片选信号拉高
    AD8688_SCLK_H;
    return Result;
}
/******************************************************************
函数名称：ADS8688_READ_Register
函数功能：Samp8688;AUTORST模式扫描采集8路
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
uint16_t ADS8688_READ_Register(uint16_t data)
{
    uint16_t i, j, Result = 0;

    AD8688_CS_H;
    user_delayUs(2000);
    AD8688_CS_L;             // 片选信号拉低
    for (i = 0; i < 16; i++) // 写入16位命令
    {
        if ((data << i) & 0x8000)
        {
            AD8688_SDI_H; // din=1
        }
        else
        {
            AD8688_SDI_L; // din=0
        }
        AD8688_SCLK_H; // CLK=1
        j = 5;
        while (j--)
            ;
        AD8688_SCLK_L; // CLK =0
        j = 5;
        while (j--)
            ;
    }
    for (i = 0; i < 16; i++) // 读取十六位数据
    {
        AD8688_SCLK_H; // CLK =1
        j = 5;
        while (j--)
            ;

        Result <<= 1;
        if (AD8688_SDO == 1)
        {
            Result++;
        }
        AD8688_SCLK_L; // CLK =0
        j = 5;
        while (j--)
            ;
    }
    AD8688_CS_H; // 片选信号拉高
    AD8688_SCLK_H;
    return Result;
}
/******************************************************************
函数名称：Comu_8688
函数功能：Comu_8688
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
uint8_t Comu_8688(uint8_t Data, uint8_t Ope)
{
    volatile uint8_t i, Buf;
    if (Ope)
    {
        Buf = Data;
        for (i = 0; i < 8; i++)
        {
            //          user_delayUs(1);
            AD8688_SCLK_H;
            //          user_delayUs(1);
            if (Buf & 0x80)
            {
                AD8688_SDI_H;
            }
            else
            {
                AD8688_SDI_L;
            }
            //          user_delayUs(1);
            AD8688_SCLK_L;
            Buf <<= 1;
        }
        return 1;
    }
    else
    {
        Buf = 0;
        for (i = 0; i < 8; i++)
        {
            Buf <<= 1;
            if (AD8688_SDO)
                Buf |= 1;
            //              user_delayUs(1);

            AD8688_SCLK_H;
            //              user_delayUs(1);
            AD8688_SCLK_L;
            //              user_delayUs(1);
        }
        return Buf;
    }
}
/******************************************************************
函数名称：Comu_8688
函数功能：Comu_8688
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
unsigned int Read_8688(unsigned int Samp_Ch)
{
    uint8_t Buf;
    uint16_t Resault, data;
    // Init_Ads8688();
    //  user_delayUs(1);
    AD8688_CS_L;
    //  user_delayUs(1);

    data = (uint16_t)Samp_Ch;
    Buf = (uint8_t)((data >> 8) & 0xff);
    Comu_8688(Buf, 1);
    data = (uint16_t)Samp_Ch;
    Buf = (uint8_t)((data)&0xff);
    // Buf = (uint8_t)Samp_Ch;
    Comu_8688(Buf, 1);
    AD8688_CS_H;

    AD8688_CS_L;
    Comu_8688(0X00, 0);
    Comu_8688(0, 0);

    Resault = (Comu_8688(0x00, 0));
    Resault <<= 8;
    Resault |= (Comu_8688(0x00, 0));
    AD8688_CS_H;
    return (uint32_t)Resault;
}


/******************************************************************
函数名称：
函数功能:
输入参数：TOUT,AD_DYB,TEMP1,TEMP2
输出参数：
返回值：
注意：
******************************************************************/
int ReadAD_value(uint16_t Channel)
{
	uint32_t AD_Value=0;
		switch(Channel)
	{	
		case TOUT :   
			AD_Value= Read_8688(ADS8688_CH_TABLE[0]);
		break ;
		case AD_DYB : 
			AD_Value= Read_8688(ADS8688_CH_TABLE[1]);
		break ;
		case TEMP1 : 
			AD_Value= Read_8688(ADS8688_CH_TABLE[4]);
		break ;
		case TEMP2 : 
			AD_Value= Read_8688(ADS8688_CH_TABLE[5]);
//		case Test:
//			AD_Value= Read_8688(ADS8688_CH_TABLE[7]);
		break ;
	}
	return AD_Value;
}

