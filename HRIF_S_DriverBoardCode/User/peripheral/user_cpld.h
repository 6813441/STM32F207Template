#ifndef CPLD
#define CPLD
#include "main.h"

#define USE_FSMC 1
#define USE_IO (!USE_FSMC)

#if USE_FSMC

typedef enum
{
  EN_DBUS_ADDR = (0x0001),
  CS_DBUS_ADDR = (0x0002),
  DATA_DBUS_ADDR = (0x0003),
  PWR_CTLBUS_ADDR = (0x0004),
  IO_CTLBUS_ADDR = (0x0005),
  FPGAIO_T4A_ADDR = (0x0006),
  DBUS_SINGLEIO_ADDR = (0x0007),

  FPGAIO_T4B_ADDR = (0x0009),
  FPGAIO_T6A_ADDR = (0x000A),
  FPGAIO_T6B_ADDR = (0x000B),
  FPGAIO_T11B_ADDR = (0x000C),
  FPGAIO_T12A_ADDR = (0x000D),
  FPGAIO_T17A_ADDR = (0x000E),
  FPGAIO_T18B_ADDR = (0x000F)
} Latch_arr_t;

#define DIR_Switch_arr 0x68000008 // 拨码开关地址

typedef enum
{
    ADDR_EN_DBUS = 0,
    ADDR_CS_DBUS = 1,
    ADDR_DATA_DBUS = 2,
    ADDR_PWR_DBUS = 3,
    ADDR_IO_DBUS = 4,
} ADDR_Ctrl;

typedef enum
{
  CTRL_EN1 = 0,
  CTRL_EN2 = 1,
  CTRL_EN3 = 2,
  CTRL_EN4 = 3,
  CTRL_EN5 = 4,
  CTRL_EN6 = 5,
  CTRL_EN7 = 6,
  CTRL_EN8 = 7,

  CTRL_CS1 = 8,
  CTRL_CS2 = 9,
  CTRL_CS3 = 10,
  CTRL_CS4 = 11,
  CTRL_CS5 = 12,
  CTRL_CS6 = 13,
  CTRL_CS7 = 14,
  CTRL_CS8 = 15,
  CTRL_DATA1 = 16,
  CTRL_DATA2 = 17,
  CTRL_DATA3 = 18,
  CTRL_DATA4 = 19,
  CTRL_DATA5 = 20,
  CTRL_DATA6 = 21,
  CTRL_DATA7 = 22,
  CTRL_DATA8 = 23,
  CTRL_PWR1 = 24,
  CTRL_PWR2 = 25,
  CTRL_PWR3 = 26,
  CTRL_PWR4 = 27,
  CTRL_PWR5 = 28,
  CTRL_PWR6 = 29,
  CTRL_PWR7 = 30,
  CTRL_PWR8 = 31,
  CTRL_IO1 = 32,
  CTRL_IO2 = 33,
  CTRL_IO3 = 34,
  CTRL_IO4 = 35,
  CTRL_IO5 = 36,
  CTRL_IO6 = 37,
  CTRL_IO7 = 38,
  CTRL_IO8 = 39,

} CPLD_SingleCtrl;

extern void CPLD_Init(void);
extern uint8_t Read_DIP_Switch(void);
extern void CPLD_Ctrl(Latch_arr_t num, uint8_t data);
extern void CPLD_SingleIo_Ctrl(CPLD_SingleCtrl num, uint8_t flag);
extern void IP_Reset(void);
#endif

#endif
