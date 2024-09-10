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
#ifndef _user_Time_H
#define _user_Time_H

extern void Rtc_Time_Init(uint8_t time[6]);
extern void Get_Time_Data(uint8_t *GetTime);
extern void CalibRtcTime(uint8_t Adjust_Time[6]);
extern void TimeCopy(uint8_t flag);
extern void RunTime(void);
extern uint16_t CalcDeltOfTime(void);
extern uint8_t BCD_to_Hex(uint8_t bcd_num);
extern uint8_t Hex_to_BCD(uint8_t hex_num);
#endif

