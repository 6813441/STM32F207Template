/*
 * Copyright (c) 2023,杭可仪器第一研究所
 * All rights reserved.
 *
 * 文件名称：
 * 文件标识：见配置管理计划书
 * 摘 要：
 *
 * 当前版本：1.1
 * 作 者：
 * 完成日期：2023年4月8日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2023年4月8日
 */
#ifndef __updata_H
#define __updata_H
#include "main.h"

/*
中断向量表100个，向上圆整128 4字节所以占用512 所以起始地址为512的整数倍

GD32F4
块0扇区定义
扇区0				0x0800 0000 - 0x0800 3FFF			16KB			//启动程序
扇区1				0x0800 4000 - 0x0800 7FFF			16KB			//启动程序
扇区2				0x0800 8000 - 0x0800 BFFF			16KB			//启动程序

扇区3				0x0800 C000 - 0x0800 FFFF			16KB 			//长度（直接使用标志）

扇区4				0x0801 0000 - 0x0801 FFFF			64KB			//APP
扇区5				0x0802 0000 - 0x0803 FFFF			128KB			//APP							192K
扇区6				0x0804 0000 - 0x0805 FFFF			128KB			//UPDATA SAVE
扇区7				0x0806 0000 – 0x0807 FFFF			128KB			//UPDATA SAVE  		192K

扇区8				0x0808 0000 - 0x0809 FFFF			128KB			//UPDATA SAVE
扇区9				0x080A 0000 – 0x080B FFFF			128KB			//UPDATA SAVE
扇区10				0x080C 0000 - 0x080D FFFF			128KB			//UPDATA SAVE
扇区11				0x080E 0000 – 0x080F FFFF			128KB			//UPDATA SAVE

块1扇区定义
扇区12			0x0810 0000 - 0x0810 3FFF			16KB			//ADJUST
扇区13			0x0810 4000 - 0x0810 7FFF			16KB			//ADJUST
扇区14			0x0810 8000 - 0x0810 BFFF			16KB			//ADJUST
扇区15			0x0810 C000 - 0x0810 FFFF			16KB			//ADJUST
...

*/

/*
对于主存储闪存容量不多于3072KB的GD32F4xx，
包含8个16KB的扇区、2个64KB的扇区、14个128KB的扇区、4个256KB的扇区。
主存储闪存的每个扇区都可以单独擦除。
*/

#define App_Address (0x08000000 + 0x10000)            // App起始地址    0x08010000
#define Flag_Address (0x08000000 + 0xC000)            // 更新程序标志   0x0800C000  --> 扇区3
#define Updata_Program_Address (0x08000000 + 0x40000) // 更新程序放置   0x08040000  --> 扇区6

#define Backdata_Program_Address (0x08000000 + 0x80000) // 备份原先的程序放置

/**************这里三个宏定义没有用到****************/
#define App_Offset (0x10000)          // 向量表偏移
#define App_SIZE (0x2FFFF)            // 字节
#define Updata_Program_SIZE (0x2FFFF) // 字节,直接全部写入
/****************************************************/

#define App_SECTOR_1 CTL_SECTOR_NUMBER_4
#define App_SECTOR_2 CTL_SECTOR_NUMBER_5

#define Updata_Program_SECTOR_1 CTL_SECTOR_NUMBER_6
#define Updata_Program_SECTOR_2 CTL_SECTOR_NUMBER_7

#define Flag_SECTOR CTL_SECTOR_NUMBER_3

bool Check_Program(uint32_t buff_add, uint32_t updata_program_add, uint32_t size);
void Erase_Updata_Program_Area(void);
void Program_Updata_Area(uint32_t updata_program_add, uint32_t buff_add, const uint32_t size);
void Set_Updata_Flag(void);
void Reset_Updata_Flag(void);
#endif
