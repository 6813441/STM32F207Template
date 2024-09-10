#ifndef _CONRELAY_H_
#define _CONRELAY_H_

/* 
*  VG2_MOS_SET_MCU  ---  IO_CTL7    ---�̵���
*  VG1_MOS_SET_MCU  ---  IO_CTL5    ---�̵���
*/


#define VG2_MOS_SET_MCU_H      CPLD_SingleIo_Ctrl(CTRL_IO7,1);

#define VG2_MOS_SET_MCU_L      CPLD_SingleIo_Ctrl(CTRL_IO7,0);

#define VG1_MOS_SET_MCU_H      CPLD_SingleIo_Ctrl(CTRL_IO5,1);

#define VG1_MOS_SET_MCU_L      CPLD_SingleIo_Ctrl(CTRL_IO5,0);


/* 
VD1_CLR  --   IO_CTL8  
VD2_CLR  --   IO_CTL6
VG1_CLR  --   IO_CTL4
VG2_CLR  --   IO_CTL2
*/
#define VG_PRE_H   VG2_CLR_H
#define VG_PRE_L   VG2_CLR_L

#define VD_PRE_H   VD2_CLR_H
#define VD_PRE_L   VD2_CLR_L

#define VD1_CLR_H  CPLD_SingleIo_Ctrl(CTRL_IO8,1);
#define VD2_CLR_H  CPLD_SingleIo_Ctrl(CTRL_IO6,1);
#define VG1_CLR_H  CPLD_SingleIo_Ctrl(CTRL_IO4,1);
#define VG2_CLR_H  CPLD_SingleIo_Ctrl(CTRL_IO2,1);


#define VD1_CLR_L  CPLD_SingleIo_Ctrl(CTRL_IO8,0);
#define VD2_CLR_L  CPLD_SingleIo_Ctrl(CTRL_IO6,0);
#define VG1_CLR_L  CPLD_SingleIo_Ctrl(CTRL_IO4,0);
#define VG2_CLR_L  CPLD_SingleIo_Ctrl(CTRL_IO2,0);


//#define PRE_H  CPLD_SingleIo_Ctrl(CTRL_PWR8 ,1);
//#define PRE_L  CPLD_SingleIo_Ctrl(CTRL_PWR8 ,0);


extern void ConCLR(uint16_t mode);
extern void ConRelayInit(void);
#endif




