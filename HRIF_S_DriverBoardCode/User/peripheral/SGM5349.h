#ifndef _SGM5349_H_
#define _SGM5349_H_
/******************************************************************************
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 * Include files
 ******************************************************************************/
//ADC/DAC共用时钟  数据线
typedef union
{
	uint32_t data;
  struct
  {
        uint32_t doncare2:4;//后输入的低位
        uint32_t v_data  :16;
        uint32_t ch :4;
        uint32_t command :4;
        uint32_t doncare1:4;//前输入的高位
  };

}SGM5349_frame_t;

/*
** CLR     ---B54---PD12
** LDAC    ---B6----PF11
** SCLK    ---B57---PG9
** DIN     ---B55---PB14
** SYNC_DYB --B18---PE2
** SYNC_WDB --B5----PE3
*/

#define SGM5349_SCLK_PIN 				GPIO_PIN_9
#define SGM5349_SCLK_GPIO_Port 	GPIOG

#define SGM5349_LADC_PIN 			  GPIO_PIN_11
#define SGM5349_LADC_GPIO_Port 	GPIOF

#define SGM5349_CLR_PIN					GPIO_PIN_12
#define SGM5349_CLR_GPIO_Port 	GPIOD

#define SGM5349_DIN_PIN					GPIO_PIN_14
#define SGM5349_DIN_GPIO_Port 	GPIOB

#define SGM5349_SYNC_DYB_PIN 				GPIO_PIN_2  
#define SGM5349_SYNC_DYB_GPIO_Port 	GPIOE

#define SGM5349_SYNC_WDB_PIN 				GPIO_PIN_3   
#define SGM5349_SYNC_WDB_GPIO_Port 	GPIOE




/******************************************************************
函数更改历史记录：
SYNC4-1 PA5
SYNC1 PD11
LDAC  PF11
CLR   PD12LDAC_SGM5349_H
DIN   PB14
SCLK  PG9

******************************************************************/
#define SCLK_SGM5349_H      gpio_bit_write(SGM5349_SCLK_GPIO_Port,SGM5349_SCLK_PIN,SET)
#define SCLK_SGM5349_L      gpio_bit_write(SGM5349_SCLK_GPIO_Port,SGM5349_SCLK_PIN,RESET) 

#define DIN_SGM5349_H       gpio_bit_write(SGM5349_DIN_GPIO_Port,SGM5349_DIN_PIN,SET)
#define DIN_SGM5349_L       gpio_bit_write(SGM5349_DIN_GPIO_Port,SGM5349_DIN_PIN,RESET) 

#define LDAC_SGM5349_H      gpio_bit_write(SGM5349_LADC_GPIO_Port,SGM5349_LADC_PIN,SET)
#define LDAC_SGM5349_L      gpio_bit_write(SGM5349_LADC_GPIO_Port,SGM5349_LADC_PIN,RESET) 

#define CLR_SGM5349_H       gpio_bit_write(SGM5349_CLR_GPIO_Port,SGM5349_CLR_PIN,SET)
#define CLR_SGM5349_L       gpio_bit_write(SGM5349_CLR_GPIO_Port,SGM5349_CLR_PIN,RESET) 


#define SYNC_DYB_SGM5349_H       gpio_bit_write(SGM5349_SYNC_DYB_GPIO_Port,SGM5349_SYNC_DYB_PIN,SET)
#define SYNC_DYB_SGM5349_L       gpio_bit_write(SGM5349_SYNC_DYB_GPIO_Port,SGM5349_SYNC_DYB_PIN,RESET) 

#define SYNC_WDB_SGM5349_H       gpio_bit_write(SGM5349_SYNC_WDB_GPIO_Port,SGM5349_SYNC_WDB_PIN,SET)
#define SYNC_WDB_SGM5349_L       gpio_bit_write(SGM5349_SYNC_WDB_GPIO_Port,SGM5349_SYNC_WDB_PIN,RESET) 


#define SGM5349_BENCHMARK_VOLTAGE1    5//基准电压   

#define SGM5349_VOLTAGE_BIT        65535//位数
#define SGM5349_FRAME_BIT          32//数据帧长


typedef enum
{
	SYNC_DYB =0,
	SYNC_WDB =1
}SGM5349_type_t;




#define DYB  1
#define WDB  2

/*DYB*/
#define VG1_DA         0   //VoutA
#define VG2_DA         1   //VoutB
#define VD1_DA         2   //VoutC
#define VD2_DA         3   //VoutD
#define ICC_VG1_P_DA   4   //VoutE
#define ICC_VG2_P_DA   5   //VoutF
#define ICC_VD1_P_DA   6   //VoutG
#define ICC_VD2_P_DA   7   //VoutH

/*WDB*/
#define TempProtect1   1   //VTSET1 VoutB 
#define TempProtect2   3   //VTSET2 VoutD 
//#define TempProtect3   5   //VTSET3 VoutF 当前只用TempProtect1，TempProtect2
//#define TempProtect4   7   //VTSET4 VoutH 


extern void InitSGM5349Gpio(void);
extern void SGM5349_Init(void);
extern void SGM5349_Output(float voltage,SGM5349_type_t SGM5349_num,uint8_t ch);
extern void SGM5349_Soft_Reset(SGM5349_type_t SGM5349_num);
void SGM5349test(void);

extern void SGM5349_DA( uint16_t ch ,float data ,uint16_t Board);
/******************************************************************************
 ******************************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */
/******************************************************************************
 ******************************************************************************/
#endif /* USERHEADERFILE_SGM5349_H_ */
