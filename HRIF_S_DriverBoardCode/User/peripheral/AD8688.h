#ifndef _AD8688_H_
#define _AD8688_H_

/* 
**  ADS8688 GPIO设置
**  SDI    --B43--PA15
**  RESET  --B42--PC12
**  AD_CS  --B41--PC11
**  AD_SCLK--B40--PD3
**  SDO    --B39--PD2

** IN+1---TOUT    0
** IN+2---AD_DYB  1
** IN+5---TEMP1   4
** IN+6---TEMP2   5

*/

/////////-------------------------- 屏蔽使用，如果是AD8688直接使用
//#define TOUT     1
//#define AD_DYB   2
//#define TEMP1    3
//#define TEMP2    4
//#define Test     8
////////---------------------------
#define TOUT     0
#define AD_DYB   1
#define TEMP1    4
#define TEMP2    5


#define AD8688_RST_PT C
#define AD8688_RST_PN 12

#define AD8688_SDI_PT G
#define AD8688_SDI_PN 14

#define AD8688_SCLK_PT  D
#define AD8688_SCLK_PN  3

#define AD8688_SDO_PT  D
#define AD8688_SDO_PN  2

#define AD8688_CS_PT  A
#define AD8688_CS_PN  15


#define AD8688_RST_PIN 			GPIO_PIN_x(AD8688_RST_PN)
#define AD8688_RST_PORT 	  GPIOx(AD8688_RST_PT)

#define AD8688_SDI_PIN 		  GPIO_PIN_x(AD8688_SDI_PN)
#define AD8688_SDI_PORT 	  GPIOx(AD8688_SDI_PT)

#define AD8688_SCLK_PIN 		GPIO_PIN_x(AD8688_SCLK_PN)
#define AD8688_SCLK_PORT 		GPIOx(AD8688_SCLK_PT)

#define AD8688_SDO_PIN 			GPIO_PIN_x(AD8688_SDO_PN)
#define AD8688_SDO_PORT 	  GPIOx(AD8688_SDO_PT)

#define AD8688_CS_PIN 			GPIO_PIN_x(AD8688_CS_PN)
#define AD8688_CS_PORT 			GPIOx(AD8688_CS_PT)


#define AD8688_RST_H gpio_bit_write(AD8688_RST_PORT, AD8688_RST_PIN, SET);
#define AD8688_RST_L gpio_bit_write(AD8688_RST_PORT, AD8688_RST_PIN, RESET);

#define AD8688_CS_H gpio_bit_write(AD8688_CS_PORT, AD8688_CS_PIN, SET);
#define AD8688_CS_L gpio_bit_write(AD8688_CS_PORT, AD8688_CS_PIN, RESET);

#define AD8688_SCLK_H gpio_bit_write(AD8688_SCLK_PORT, AD8688_SCLK_PIN, SET);
#define AD8688_SCLK_L gpio_bit_write(AD8688_SCLK_PORT, AD8688_SCLK_PIN, RESET);

#define AD8688_SDI_H gpio_bit_write(AD8688_SDI_PORT, AD8688_SDI_PIN, SET);
#define AD8688_SDI_L gpio_bit_write(AD8688_SDI_PORT, AD8688_SDI_PIN, RESET);

#define AD8688_SDO gpio_input_bit_get(AD8688_SDO_PORT, AD8688_SDO_PIN)


#define AD8688_BENCHMARK_VOLTAGE    (4.096f)//2.048f////基准电压

/******************************************************************************************************************************
 *                                                     GLOBAL FUNCTIONS
 *******************************************************************************************************************************/
extern void Init_AD8688Gpio(void);
extern void Init_AD8688(void);
extern void ADS8688_Reset(void);
extern void Write_AD8688(uint16_t data);
extern uint16_t Read_AD8688(void);
extern uint16_t ADS8688_READ_Register(uint16_t data);
extern uint8_t Comu_8688(uint8_t Data, uint8_t Ope);
extern unsigned int Read_8688(unsigned int Samp_Ch);
extern const unsigned int ADS8688_CH_TABLE[8];

extern int ReadAD_value(uint16_t Channel);
#endif
