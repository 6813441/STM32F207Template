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
#include "main.h"


/*
扇区擦除
FMC的扇区擦除功能使得主存储闪存的扇区内容初始化为高电平。每一扇区都可以被独立擦除，
而不影响其他扇区内容。FMC扇区擦除操作步骤如下：
1. 确保FMC_CTL寄存器不处于锁定状态；
2. 检查FMC_STAT寄存器的BUSY位来判定闪存是否正处于擦写访问状态，若BUSY位为1，
   则需等待该操作结束，BUSY位变为0；
3. 置位FMC_CTL寄存器的SER位；
4. 将待擦除扇区号写到FMC_CTL寄存器SN位；
5. 通过将FMC_CTL寄存器的START位置1来发送扇区擦除命令到FMC；
6. 等待擦除指令执行完毕，FMC_STAT寄存器的BUSY位清0；
7. 如果需要，使用DBUS读操作验证该扇区是否擦除成功。
*/

//1.1. 擦除升级区，准备写入
void Erase_Updata_Program_Area(void)
{
	fmc_unlock();                                //解锁FMC主操作
	fmc_sector_erase(Updata_Program_SECTOR_1);   //扇区6
	fmc_sector_erase(Updata_Program_SECTOR_2);   //扇区7
	fmc_lock();                                  //锁定FMC主操作
}

//1.2. 擦除更新标志位
void Reset_Updata_Flag(void)  
{
	fmc_unlock();                               //解锁FMC主操作
	fmc_sector_erase(Flag_SECTOR);              //扇区3(存放标志位)
	fmc_lock();                                 //锁定FMC主操作
}

static uint32_t updata_shift = 0; // 偏移量
// 检查更新是否完全写入，true为已正确写入，字节为单位
bool Check_Program(uint32_t buff_add, uint32_t updata_program_add, uint32_t size)
{
	if (0 == memcmp((uint8_t *)buff_add, (uint8_t *)updata_program_add, size))
		return true;
	else
		return false;
}


//2.1. 将接收缓存的数据写入app flash，size为字节,updata_program_add不需要动
void Program_Updata_Area(uint32_t updata_program_add, uint32_t buff_add, const uint32_t size) 
	{ /*传输的updata_program_add（在线更新程序地址）为扇区6 buff_add为上位机传来的数据 size为1024字节*/
	fmc_unlock();                  //解锁FMC主操作
	for (int i = 0; i < size; i++) // 写入
		fmc_byte_program(updata_program_add + updata_shift + i, *(uint8_t *)(buff_add + i));  //在相应的地址上编程一个字节
	if (true == Check_Program(updata_program_add + updata_shift, buff_add, size)) // 校验
		SEGGER_RTT_printf(0, "write right!!\n");
	else
		SEGGER_RTT_printf(0, "write error!!\n");
	updata_shift += size;          //计算写入后的偏移量
	fmc_lock();                    //锁定FMC主操作
}

//2.2. 更新置位
void Set_Updata_Flag(void)
{
	fmc_unlock();
	fmc_word_program(Flag_Address, 0x00000000);  //在相应的地址编程一个字
	fmc_lock();
}


