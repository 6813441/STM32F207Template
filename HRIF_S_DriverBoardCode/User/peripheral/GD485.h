/*
*  客户预留485通信 配置成USART即可
*  SCIRXD --  B41  -- PC11  USART2_Rx<--> AF7 USART3_Rx<--> AF8
*  SCITXD --  B44  -- PC10  USART2_Tx<--> AF7 USART3_Tx<--> AF8
*  485_EN --  B46  -- PG13
*/

#ifndef _GD485_H_
#define _GD485_H_


#define Communication_Port GPIOG
#define Communication_Pin GPIO_PIN_13

#define Communication_H() gpio_bit_write(Communication_Port, Communication_Pin, SET)
#define Communication_L() gpio_bit_write(Communication_Port, Communication_Pin, RESET)


extern void USART_Init(void);
extern int RS485_SendData(uint8_t *buf, uint32_t len);
extern void RS485Test(void);

#endif
