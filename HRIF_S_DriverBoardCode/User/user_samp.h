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
#ifndef __user_samp_H
#define __user_samp_H

#define VoltageCoefficient 0.3125




#define VG1_AD 0
#define VG2_AD 1
#define VD1_AD 2
#define VD2_AD 3
#define ICC_VG1_AD 4
#define ICC_VG2_AD 5
#define ICC_VD1_AD 6
#define ICC_VD2_AD 7

#define TC11 0
#define TC21 1




/*******************************************************************************
 * Include files
 ******************************************************************************/
extern void SampleALLChanData(void);
extern void SampleSingleChanData(uint16_t ChanNo);
extern void CaliALLData(void);
extern void DataPackChanData(void);

extern void CaliVolChanData();
extern void CaliCurrChanData();
extern void CaliTempChanData();
extern void CaliBoardTempChanData();

extern float SortAndFilter(float *dpoint, uint16_t num);
extern uint8_t AbsF(float DataA,float DataB,float DataD);


extern float Read_AD_Value(uint16_t ch,uint16_t Board);
//extern void BoardTest(void);
extern uint16_t CheckInitiatingVol(uint16_t ch);
extern uint16_t CheckStopVol(uint16_t ch);
extern void SampTemp();
extern void SelectAgingModeSamp(void);
extern void RegulationVol(float DifferenceValue, uint16_t VolSwitch);
extern void RegulationMode();
extern int MachineCycle(void);
//extern uint16_t CheckStopVol();
/******************************************************************************
 ******************************************************************************/
 
#endif
