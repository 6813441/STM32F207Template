/*
 * Copyright (c) 2023,����������һ�о���
 * All rights reserved.
 *
 * �ļ����ƣ�
 * �ļ���ʶ�������ù���ƻ���
 * ժ Ҫ��
 *
 * ��ǰ�汾��1.1
 * �� �ߣ�
 * ������ڣ�2023��4��8��
 *
 * ȡ���汾��1.0
 * ԭ���� ��
 * ������ڣ�2023��4��8��
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

