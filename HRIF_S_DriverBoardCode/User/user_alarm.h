/*
 * Copyright (c) 2024,����������һ�о���
 * All rights reserved.
 *
 * �ļ����ƣ�
 * �ļ���ʶ�������ù���ƻ���
 * ժ Ҫ��
 *
 * ��ǰ�汾��1.1
 * �� �ߣ�
 * ������ڣ�2024��4��25��
 *
 * ȡ���汾��1.0
 * ԭ���� ��
 * ������ڣ�2024��4��25��
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
