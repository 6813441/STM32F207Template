#include <stdio.h>
#include <stdint.h>
#ifndef _FPGA_H_
#define _FPGA_H_
typedef struct
{
	uint16_t Out_Type;
	uint16_t Hi_Impedance;
	uint16_t Group_N;
	uint16_t Out_Fre_L;
	uint16_t Out_Fre_H;
	uint16_t Deep_L;
	uint16_t Deep_H;
	uint16_t Sdram_Init_Count;
	uint16_t Sdram_Init_CountH;
} FPGA_INIT_STRUCT;

typedef struct
{
	uint16_t Signal_Type;
	uint16_t Fre_L;
	uint16_t Fre_H;
	uint16_t Phase_L;
	uint16_t Phase_H;
	uint16_t Amplitude;
	uint16_t Offset;
	uint16_t Data_Len;
	uint16_t duty_cycle; //????
} ANALOG_SIGNAL_INFO;

typedef struct
{
	uint16_t Signal_Type;
	uint16_t Deep_L;
	uint16_t Deep_H;
	uint16_t Phase_L;
	uint16_t Phase_H;
	uint16_t Amplitude;
	uint16_t Offset;
	uint16_t Dev;
} ANALOG_SEND_STRUCT;

typedef struct
{
	uint16_t Analog_Change;
	uint16_t Digital_Change;
} FPGA_STATE_CHANGE;


#define FPGA_CSBOARD_H gpio_bit_reset(FPGA_CE_PORT, FPGA_CE_PORT);
#define FPGA_CSBOARD_L gpio_bit_reset(FPGA_CE_PORT, FPGA_CE_PORT);

#define FPGA_CE_PORT GPIOD
#define FPGA_CE_PIN GPIO_PIN_7

#define FPGA_DC_PORT GPIOD
#define FPGA_DC_PIN GPIO_PIN_6

#define FPGA_RESET_PORT   GPIOE
#define FPGA_RESET_PIN    GPIO_PIN_1

#define FPGA_CS0_PORT   GPIOF
#define FPGA_CS0_PIN    GPIO_PIN_8

#define FPGA_CS1_PORT   GPIOB
#define FPGA_CS1_PIN    GPIO_PIN_1

#define FPGA_CS0_H gpio_bit_set(FPGA_CS0_PORT, FPGA_CS0_PIN);
#define FPGA_CS0_L gpio_bit_reset(FPGA_CS0_PORT, FPGA_CS0_PIN);

#define FPGA_RESET_H gpio_bit_set(FPGA_RESET_PORT, FPGA_RESET_PIN);
#define FPGA_RESET_L gpio_bit_reset(FPGA_RESET_PORT, FPGA_RESET_PIN);

#define FPGA_CS1_H gpio_bit_set(FPGA_CS1_PORT, FPGA_CS1_PIN);
#define FPGA_CS1_L gpio_bit_reset(FPGA_CS1_PORT, FPGA_CS1_PIN);


#define FPGA_DC_H gpio_bit_set(FPGA_DC_PORT, FPGA_DC_PIN);
#define FPGA_DC_L gpio_bit_reset(FPGA_DC_PORT, FPGA_DC_PIN);


#define FPGA_CE_H gpio_bit_set(FPGA_CE_PORT, FPGA_CE_PIN);
#define FPGA_CE_L gpio_bit_reset(FPGA_CE_PORT, FPGA_CE_PIN);


#define Data_test(x) *(uint8_t *)(0x60000000+((x)<<6))

#define ALL_SAME  (0x10)

#define ALL_CH_FRE  (0X40)

#define CH_FRE(x)  (ALL_CH_FRE+0x3*x)

#define ALL_CH_Ratio  (0X70)

#define CH_Ratio(x)  (ALL_CH_Ratio+0x3*x)

#define GROP_Ratio_BASE  (0XB0-0x3)

#define GROP_Ratio(x)  (GROP_Ratio_BASE+0x3*x)


#define GROP_FRE_BASE  (0XA0-0x3)

#define GROP_FRE(x)  (GROP_FRE_BASE+0x3*x)

#define CH_ENABLE0_7 (0x12)

#define CH_ENABLE8_11 (0x11)
#define FPGA_ANALOG_START {\
	FPGA_CS0_L;\
	FPGA_CS1_L;\
	Fpga_Write_Regist(0x03, 0x01); \
	FPGA_CS0_H;\
	FPGA_CS1_H;\
}

typedef union {
	uint32_t data;
  uint8_t Bytes[4];
}Word32b;

typedef struct
{
	enum {
	PWM_CH_ALL=0,
	PWM_CH_1,
  PWM_CH_2,
  PWM_CH_3,
  PWM_CH_4,		
  PWM_CH_5,		
  PWM_CH_6,		
  PWM_CH_7,		
  PWM_CH_8,		
  PWM_CH_9,		
  PWM_CH_10,		
  PWM_CH_11,		
  PWM_CH_12
	}CH;
  uint32_t Fre;
	float Duty_Ratio;
	uint32_t GR_SET;
}FPGA_PARA;

extern void Fpga_Send(FPGA_PARA *para);
extern void Fpga_Enable(uint32_t CH);
extern void Fpga_Disable(uint32_t CH);
extern void HRIF_S_Fpga_USE(void);
extern void HRIF_S_Fpga_Init(void);
#endif



