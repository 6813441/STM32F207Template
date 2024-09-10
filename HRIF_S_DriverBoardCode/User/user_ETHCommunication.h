/*
 * Copyright (c) 2023,����������һ�о���
 * All rights reserved.
 *
 * �ļ����ƣ�
 * �ļ���ʶ�������ù����ƻ���
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
#ifndef _user_ETHCOMMUNICATION_H
#define _user_ETHCOMMUNICATION_H

extern struct netconn *remote_netconn;
/***************************************************************************
****************************************************************************/
#define PC_CMD_MAX (sizeof(CMD_Assemble) / sizeof(CMD_Type)) // TODO ��ǰ����ĸ���;��Ҫ���ݵ�ǰ��������ʵʱ�޸�

typedef struct CMD_TYPE
{
	char *CMD_String;					              // �����ַ���
	void (*Run_Function)(void *parameter);  // ��������(��������)
} CMD_Type;

extern void EthernetReceiveSuccess(uint32_t ParameterLen, uint16_t *Send);
extern void EthernetReceiveFailedString(uint16_t Error);
extern void EthernetInterfaceHandler(void);
extern void PC_DATAHandler(void);
extern uint32_t Crs(uint16_t *buf, uint32_t len);
extern void GetSoftWareVersion(void *parameter);
extern void CalibSysTime(void *parameter);

extern void SendAgingTestData(void *parameter);
extern void ReceiveAgingTestInfo(void *parameter);
extern void ReceiveAgingCommand(void *parameter);
extern void ReceiveCalibrationKB(void *parameter);
extern void SendCalibrationKB(void *parameter);
extern void GETEDITION(void *parameter);
extern void ONLINEUPDATE1(void *parameter);
extern void ONLINEUPDATE2(void *parameter);
extern void Cal_ChanMode(void *parameter);
extern void Cal_ChanReadData(void *parameter);
extern void Cal_ChanWriteData(void *parameter);
extern void ReceiveAgingFANInfo(void *parameter);

extern void EthernetReceiveSuccessString_Updata(uint32_t ParameterLen, uint16_t *Send);
extern void SendUpData(void *parameter);
/***************************************************************************
****************************************************************************/

#endif
