#ifndef SGM4588_H_
#define SGM4588_H_


/*
** ADD1---B2---PF10
** ADD2---B4---PC2
** ADD3---B3---PC3
** SW_EN_WDB---B1 ---PC0    ---温度板
** SW_EN_DYB---B53---PD11   ---电源板
*/
#define ADD1_PIN 				GPIO_PIN_10
#define ADD1_GPIO_Port 	GPIOF

#define ADD2_PIN 				GPIO_PIN_2	
#define ADD2_GPIO_Port 	GPIOC

#define ADD3_PIN 				GPIO_PIN_3
#define ADD3_GPIO_Port 	GPIOC


#define SW_EN_WDB_PIN 				GPIO_PIN_0
#define SW_EN_WDB_GPIO_Port 	GPIOC

#define SW_EN_DYB_PIN 				GPIO_PIN_11          
#define SW_EN_DYB_GPIO_Port 	GPIOD	 



#define ADD1_H 		  gpio_bit_write(ADD1_GPIO_Port,ADD1_PIN,SET) 
#define ADD1_L 		  gpio_bit_write(ADD1_GPIO_Port,ADD1_PIN,RESET) 
 
#define ADD2_H 			gpio_bit_write(ADD2_GPIO_Port,ADD2_PIN,SET)
#define ADD2_L 			gpio_bit_write(ADD2_GPIO_Port,ADD2_PIN,RESET) 

#define ADD3_H 	    gpio_bit_write(ADD3_GPIO_Port,ADD3_PIN,SET) 
#define ADD3_L    	gpio_bit_write(ADD3_GPIO_Port,ADD3_PIN,RESET) 

#define SW_EN_WDB_H 			gpio_bit_write(SW_EN_WDB_GPIO_Port,SW_EN_WDB_PIN,SET) 
#define SW_EN_WDB_L 			gpio_bit_write(SW_EN_WDB_GPIO_Port,SW_EN_WDB_PIN,RESET) 

#define SW_EN_DYB_H 			gpio_bit_write(SW_EN_DYB_GPIO_Port,SW_EN_DYB_PIN,SET) 
#define SW_EN_DYB_L 			gpio_bit_write(SW_EN_DYB_GPIO_Port,SW_EN_DYB_PIN,RESET) 


/*切电源板和温控板4588*/
#define WDB 				0
#define DYB 				1          

extern void SGM4588init(void);
extern void SelectSGM4588Bit(uint16_t ChanNo);
extern void Select_Channel(uint16_t Channel,uint16_t mode);
#endif

