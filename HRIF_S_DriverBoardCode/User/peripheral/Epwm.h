#ifndef _EPWM_H_
#define _EPWM_H_
/******************************************************************************************************************************
 *		                                 GLOBAL FUNCTIONS
 *******************************************************************************************************************************/
extern void PWM_TIM2_Init(void);
extern void Timer2Base_init(void);
extern void PWM_TIM3_Init(void);
extern void Timer3Base_init(void);
extern void PWM_GPIO_Init(void);
extern float PID_Caculate(struct PID_REG * PIDControl);
extern void StationTempCtrl(uint16_t Flag);
extern void Init_PID(void);
extern void FSDSetup(uint16_t FSD1, uint16_t FSD2,uint16_t FSD3, uint16_t FSD4);
extern void HOTSetup(uint16_t HOT1, uint16_t HOT2,uint16_t HOT3, uint16_t HOT4);
extern void FSDSortControl(uint16_t Gear);
#endif
