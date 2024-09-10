/*
 * Copyright (c) 2024,杭可仪器第一研究所
 * All rights reserved.
 *
 * 文件名称：
 * 文件标识：见配置管理计划书
 * 摘 要：
 *
 * 当前版本：1.1
 * 作 者：
 * 完成日期：2024年4月25日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2024年4月25日
 */
#ifndef __alarm_H
#define __alarm_H

/*******************************************************************************
 * Include files
 ******************************************************************************/
#define WarnCompCnt 1
extern void AbnormalHandle(void);
extern void AbnormalCtrlHandle(void);
extern void Station1VolAbnormal(void);
extern void Station2VolAbnormal(void);

extern void Station1CurrAbnormal(void);
extern void Station2CurrAbnormal(void);

extern void StationTempAbnormal(void);
/******************************************************************************
 ******************************************************************************/
#endif
