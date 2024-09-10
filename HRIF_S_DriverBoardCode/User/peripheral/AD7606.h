#ifndef __AD7606_H
#define __AD7606_H

#include "main.h"
/*******************************************************************************
 * Include files
 ******************************************************************************/

/* 
**  ADS8688 GPIO…Ë÷√
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

#define AD7606RESET_PORT GPIOC
#define AD7606RESET_PIN GPIO_PIN_12

#define AD7606CLK_PORT GPIOD
#define AD7606CLK_PIN GPIO_PIN_3

#define AD7606CS_PORT GPIOA
#define AD7606CS_PIN GPIO_PIN_15

#define AD7606CONV_PORT GPIOG
#define AD7606CONV_PIN GPIO_PIN_14

#define AD7606_SDO_PORT GPIOD
#define AD7606_SDO_PIN GPIO_PIN_2
//#define AD7606RESET_PORT GPIOG
//#define AD7606RESET_PIN GPIO_PIN_14

//#define AD7606CLK_PORT GPIOA
//#define AD7606CLK_PIN GPIO_PIN_15

//#define AD7606CS_PORT GPIOC
//#define AD7606CS_PIN GPIO_PIN_11

//#define AD7606CONV_PORT GPIOD
//#define AD7606CONV_PIN GPIO_PIN_7

//#define AD7606_SDO_PORT GPIOD
//#define AD7606_SDO_PIN GPIO_PIN_2


#define AD7606RESET_HIGH gpio_bit_write(AD7606RESET_PORT, AD7606RESET_PIN, SET)
#define AD7606RESET_LOW gpio_bit_write(AD7606RESET_PORT, AD7606RESET_PIN, RESET)

#define AD7606CLK_HIGH gpio_bit_write(AD7606CLK_PORT, AD7606CLK_PIN, SET)
#define AD7606CLK_LOW gpio_bit_write(AD7606CLK_PORT, AD7606CLK_PIN, RESET)

#define AD7606CS_HIGH gpio_bit_write(AD7606CS_PORT, AD7606CS_PIN, SET)
#define AD7606CS_LOW gpio_bit_write(AD7606CS_PORT, AD7606CS_PIN, RESET)

#define AD7606CONV_HIGH gpio_bit_write(AD7606CONV_PORT, AD7606CONV_PIN, SET)
#define AD7606CONV_LOW gpio_bit_write(AD7606CONV_PORT, AD7606CONV_PIN, RESET)

#define AD7606DO gpio_input_bit_get(AD7606_SDO_PORT, AD7606_SDO_PIN)



#define VOLTAGE_MINIMUM_SCALE (float)(0.00015258) // 5 / 32768// = 0.00015625

extern void Init_AD7606GPIO(void);
extern void Init_AD7606(void);
extern void AD7606Reset(void);
extern void AD7606ReadOneSample(uint16_t *iRdData);
extern float DigitalFilter_f32(float *dpoint, uint16_t num, uint16_t abandon);
extern float SortValue(float *dpoint, uint16_t num);
extern float DigitalFilter_f32ID(float *dpoint, uint16_t num, uint16_t abandon);

#define AD7606BRESET_PORT GPIOG
#define AD7606BRESET_PIN GPIO_PIN_13
#define AD7606BCLK_PORT GPIOC
#define AD7606BCLK_PIN GPIO_PIN_10
#define AD7606BCS_PORT GPIOC
#define AD7606BCS_PIN GPIO_PIN_12
#define AD7606BCONV_PORT GPIOD
#define AD7606BCONV_PIN GPIO_PIN_6
#define AD7606B_SDO_PORT GPIOD
#define AD7606B_SDO_PIN GPIO_PIN_3



/******************************************************************************
 ******************************************************************************/

#endif /* __AD7606_H__ */
/******************************************************************************
 ******************************************************************************/
