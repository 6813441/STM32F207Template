/*
 * Copyright (c) 2024,����������һ�о���
 * All rights reserved.
 *
 * �ļ����ƣ�
 * �ļ���ʶ�������ù���ƻ���
 * ժ Ҫ��
 *
 * ��ǰ�汾��1.0
 * �� �ߣ�
 * ������ڣ�2024��4��25��
 *
 * ȡ���汾��1.0
 * ԭ���� ��
 * ������ڣ�2024��4��25��
 */
#include "main.h"

#include "api.h"
#define TransmitEthernetTxMessage(temp, num) netconn_write(remote_netconn, temp, (num * 2), NETCONN_COPY);
extern uint16_t SendData[16650];
extern uint16_t KbData[200];
extern struct TIMER_REG SysTime;
// ��λ������汾��
const uint8_t SoftVersionString_QDB[] = "I:_DSP;V2.2.2_20240521_JG_YZL_HRIFS"; // QDB:������ 
const CMD_Type CMD_Assemble[] = {

    //****************����������Э��-5***************/
    {"SENDPARA" , ReceiveAgingTestInfo}, // �����ϻ�����
		{"GETDATA"  , SendAgingTestData},    // �����ϻ�����
//		{"GETDATA"  , SendUpData},    // �����ϻ�����
    {"SENDSTATE", ReceiveAgingCommand},  // ����ϵͳ״̬
    {"SYSTIME"  , CalibSysTime},         // ϵͳʱ��У׼
		
    //****************У׼����Э��-2***************/
    {"CAL_CHGMODE"  , Cal_ChanMode},            //�л�ģʽ
    {"CAL_READVALUE", Cal_ChanReadData},        //����У׼ģʽ�²ɼ�����
    {"CAL_DRIVER_SETVALUE", Cal_ChanWriteData}, //����У׼ģʽ�µ�ѹ���������ѹ
    {"PCALIBRATION",   ReceiveCalibrationKB},   // ����KBֵ
    {"GETCALIBRATION", SendCalibrationKB},      // ����KBֵ		
		
    {"FANSETFANVALUE", ReceiveAgingFANInfo},    // ���շ����λֵ		
    //****************ͨ������Э��-1*****************/
    {"GETEDITION", GETEDITION},       // ��ȡ��λ���汾������
    {"ONLINEUPDATE1", ONLINEUPDATE1}, //
    {"ONLINEUPDATE2", ONLINEUPDATE2}, //
};
/******************************************************************
�������ƣ�
��������:��̫�����ݴ�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void EthernetInterfaceHandler(void)
{
    uint16_t i;
    uint32_t dSum, rSum;

    SysCtrl.Ethernet.ETH_ReceiveLength.bytes.lbyte = SysCtrl.Ethernet.Data[13];
    SysCtrl.Ethernet.ETH_ReceiveLength.bytes.hbyte = SysCtrl.Ethernet.Data[14];
    SysCtrl.Ethernet.ETH_ReceiveLength.data /= 2;
    if (SysCtrl.Ethernet.ETH_ReceiveLength.data > 600)
    {
        EthernetReceiveFailedString(0);
        return;
    }
    if (SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.data - 1] == 0x5E5D && SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.data - 2] == 0x5C5A)
    {
        dSum = Crs(SysCtrl.Ethernet.Data, SysCtrl.Ethernet.ETH_ReceiveLength.data - 4);
        rSum = SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.data - 4] + (SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.data - 3] << 16);
    }

    if (dSum == rSum)
    {
        SysCtrl.EthConnectCnt = 0;
        SysCtrl.EthConnectFlag = 0;
        for (i = 0; i < MAX_REC_CMDSIZE; i++)
        {
            SysCtrl.Ethernet.Rec_Comand[i] = SysCtrl.Ethernet.Data[2 + i];
        }
        PC_DATAHandler();
    }
    else
    {
        EthernetReceiveFailedString(0);
    }
    memset(SysCtrl.Ethernet.Data, 0, sizeof(SysCtrl.Ethernet.Data));
}
/******************************************************************
�������ƣ�
��������:�����յ�����������
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void PC_DATAHandler(void)
{
    bool verify_bool;
    uint16_t i, j = 0;
    uint16_t ptr[30];
    char *Command = CMD_Assemble[0].CMD_String;
    uint16_t CMD_Count = 0;

    for (i = 0; i < MAX_REC_CMDSIZE; i++)
    {
        ptr[j++] = (SysCtrl.Ethernet.Data[i + 2] & 0x00FF);
        ptr[j++] = ((SysCtrl.Ethernet.Data[i + 2] >> 8) & 0x00FF);
    }
    ptr[j++] = 0;
    ptr[j++] = 0;

    while (CMD_Count < PC_CMD_MAX)
    {

        verify_bool = TRUE;
        for (i = 0; Command[i]; i++)
        {
            if (Command[i] != ptr[i])
            {
                verify_bool = FALSE;
                break;
            }
        }
        if (ptr[i] != 0)
        {
            verify_bool = FALSE;
        }

        if (verify_bool == TRUE)
        {
            user_delayUs(100);
            (*CMD_Assemble[CMD_Count].Run_Function)((void *)(&SysCtrl.Ethernet.Data[15]));
        }
        CMD_Count++;
        if (CMD_Count < PC_CMD_MAX)
        {
            Command = CMD_Assemble[CMD_Count].CMD_String;
        }
        else
        {
        }
    }
}

/******************************************************************
�������ƣ�
��������:���ݽ��ճɹ���־���������λ��
���������
���������
����ֵ��
ע�⣺�ϴ���������Ϊ��ʱ����Ҫ�ϲ����׵�ַ,���ݳ��ȹ���ʱĬ�ϻָ�У���Ϊ0
           ParameterLenΪ0ʱ��ʾ���ϴ����ݣ���ʱSend����Ϊ����ֵ
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void EthernetReceiveSuccessString(uint32_t ParameterLen, uint16_t *Send)
{
    uint16_t temp[40], num = 0, i;
    uint32_t SUM;
    union DATA_BYTE TEMP;

    temp[num++] = 0xBBAA;
    temp[num++] = 0xDDCC;
    for (i = 0; i < MAX_REC_CMDSIZE; i++)
    {
        temp[num++] = SysCtrl.Ethernet.Rec_Comand[i];
    }
    temp[num++] = 0;
    temp[num++] = 0xc0a8;
    temp[num++] = (0x7b << 8) | IP_ADDR3;
    temp[num++] = 0x3388;
    TEMP.data = ParameterLen * 2 + (num * 2) + 12; // ��������+����֮ǰ�ĳ���+����֮���ȥ����ĳ���
    temp[num++] = TEMP.bytes.lbyte;
    temp[num++] = TEMP.bytes.hbyte;
    SUM = Crs(temp, num);
    TransmitEthernetTxMessage(temp, num);
    if (0 != ParameterLen)
    {
        if (ParameterLen <= 1460)
        {
            TransmitEthernetTxMessage(Send, ParameterLen);
            if (ParameterLen < 1000)
            {
                SUM += Crs(Send, ParameterLen);
            }
            else
            {
                SUM = 0;
            }
        }
        else
        {
            uint32_t Len = ParameterLen;

            for (i = 0; Len > 1460; i++)
            {
                TransmitEthernetTxMessage((Send + 1460 * i), 1460);
                Len -= 1460;
            }
            TransmitEthernetTxMessage((Send + ParameterLen - Len), Len);
            SUM = 0;
        }
    }
    num = 0;
    temp[num++] = SUM;
    temp[num++] = SUM >> 16;
    temp[num++] = 0x5C5A;
    temp[num++] = 0x5E5D;
    TransmitEthernetTxMessage(temp, num);
}

void EthernetReceiveSuccessString_Updata(uint32_t ParameterLen, uint16_t *Send)
{
    uint16_t temp[40], num = 0, i;
    uint32_t SUM;
	  
	  uint32_t PackageCnt=0;
    if (0 != ParameterLen)
    {
        if (ParameterLen <= 1460)
        {
            TransmitEthernetTxMessage(Send, ParameterLen);
            if (ParameterLen < 1000)
            {
                SUM += Crs(Send, ParameterLen);
            }
            else
            {
                SUM = 0;
            }
        }
        else
        {
            uint32_t Len = ParameterLen;
					  PackageCnt =  Len/1460;

            for (i = 0; Len > 1460; i++)
            {
                TransmitEthernetTxMessage((Send + 1460 * i), 1460);
                Len -= 1460;
            }
            TransmitEthernetTxMessage((Send + ParameterLen - Len), Len);
            SUM = 0;
        }
    }
    num = 0;
    temp[num++] = SUM;
    temp[num++] = SUM >> 16;
    temp[num++] = 0x5C5A;
    temp[num++] = 0x5E5D;
    TransmitEthernetTxMessage(temp, num);
}
/******************************************************************
�������ƣ�
��������:���ݽ���ʧ�ܱ�־���������λ��
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void EthernetReceiveFailedString(uint16_t Error)
{
    uint16_t temp[28], num = 0, i;
    uint32_t SUM;
    temp[num++] = 0xBBAA;
    temp[num++] = 0xDDCC;
    for (i = 0; i < MAX_REC_CMDSIZE; i++)
    {
        temp[num++] = SysCtrl.Ethernet.Rec_Comand[i];
    }
    temp[num++] = 0;
    temp[num++] = 0xc0a8;
    temp[num++] = (0x7b >> 8) | IP_ADDR3;
    temp[num++] = 0x1144;
    temp[num++] = 46;
    temp[num++] = 0x0000;
    temp[num++] = Error; // ����ĳɴ�����
    SUM = Crs(temp, num);
    temp[num++] = SUM;
    temp[num++] = SUM >> 16;
    temp[num++] = 0x5C5A;
    temp[num++] = 0x5E5D;
    TransmitEthernetTxMessage(temp, num);
}
/******************************************************************
�������ƣ�
��������:У��ͼ���
���������
���������
����ֵ��
ע�⣺У��� = ���� + ���ݣ�len�������㣬�����ǰ��ֽ���
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint32_t Crs(uint16_t *buf, uint32_t len)
{
    uint32_t i, sum = 0;
    uint16_t *ptr;

    if (len > 1536)
    {
        return 0;
    }
    ptr = buf;
    for (i = 0; i < len; i++)
    {
        sum += ((*(ptr + i)) & 0x00ff);
        sum += ((*(ptr + i)) >> 8);
    }

    return sum;
}
/******************************************************************
�������ƣ�
��������:У׼ϵͳʱ��
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void CalibSysTime(void *parameter)
{
    uint8_t *Adjust_Time = parameter;

    CalibRtcTime(&Adjust_Time[0]);

    EthernetReceiveSuccessString(0, 0);
}
/******************************************************************
�������ƣ�
��������:�����ϻ����Բ���
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void SendAgingTestData(void *parameter)
{
    uint16_t *sendptr, i;
    uint32_t length = 0;

    sendptr = (uint16_t *)(SendData);

    SysData.AgingTestData.DataStatus = DATA_VAL;

    *sendptr++ = SysData.AgingTestData.DataStatus;                 //55 AA  (2�ֽ�)

    *sendptr++ = SysData.AgingTestData.SampTime.Second + (SysData.AgingTestData.SampTime.Minute << 8);
    *sendptr++ = SysData.AgingTestData.SampTime.Hour + (SysData.AgingTestData.SampTime.Day << 8);
    *sendptr++ = SysData.AgingTestData.SampTime.Momth + ((SysData.AgingTestData.SampTime.Year % 100) << 8);  
    *sendptr++ = 21;                                               //ʵʱʱ�� �� �� ʱ �� �� �� ��8�ֽڣ�

    *sendptr++ = SysData.AgingTestData.ProRunTimeCnt.bytes.lbyte;
    *sendptr++ = SysData.AgingTestData.ProRunTimeCnt.bytes.hbyte;  //�ϻ�ʱ�� ��λ �루4�ֽڣ�

    *sendptr++ = SysData.AgingTestData.AgingStatus;                //�ϻ�״̬ ��2�ֽڣ�

		*sendptr++ = SysData.AgingTestData.BoardState;                 //���״̬ ��2�ֽڣ�  0:δ��� 1��� 2�쳣
		
    for (i = 0; i < 2; i++)                                        
    {
        *sendptr++ = SysData.AgingTestData.Station_VD_Voltage[i].bytes.lbyte;
        *sendptr++ = SysData.AgingTestData.Station_VD_Voltage[i].bytes.hbyte;
    }                                                              //2·VD��ѹ ��4*2�ֽڣ�VD1 VD2

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Voltage_VD_AlarmInformation[i];
    }                                                              //2·VD��ѹ������Ϣ

    for (i = 0; i < 2; i++)                                        
    {
        *sendptr++ = SysData.AgingTestData.Station_VG_Voltage[i].bytes.lbyte;
        *sendptr++ = SysData.AgingTestData.Station_VG_Voltage[i].bytes.hbyte;
    }                                                              //2·VG��ѹ ��4*2�ֽڣ�VG1 VG2

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Voltage_VG_AlarmInformation[i];
    }                                                              //2·VG��ѹ������Ϣ

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Station_ID_Curr[i].bytes.lbyte;
        *sendptr++ = SysData.AgingTestData.Station_ID_Curr[i].bytes.hbyte;
    }                                                              //2·ID���� ��4*2�ֽڣ�ID1 ID2

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Curr_ID_AlarmInformation[i];
    }                                                              //2·ID����������Ϣ		
		
    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Station_IG_Curr[i].bytes.lbyte;
        *sendptr++ = SysData.AgingTestData.Station_IG_Curr[i].bytes.hbyte;
    }                                                              //2·IG���� ��4*2�ֽڣ�IG1 IG2

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Curr_IG_AlarmInformation[i];
    }                                                              //2·IG����������Ϣ

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Station_TC_Temp[i].bytes.lbyte;
        *sendptr++ = SysData.AgingTestData.Station_TC_Temp[i].bytes.hbyte;
    }                                                              //2·���Ȱ��¶ȣ�TC1 TC2������Ŵ�10��

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Temp_TC_AlarmInformation[i];
    }                                                              //TC1 TC2�¶ȱ�����Ϣ	
		

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Rev_Word1[i].bytes.lbyte;
        *sendptr++ = SysData.AgingTestData.Rev_Word1[i].bytes.hbyte;
    }

    for (i = 0; i < 2; i++)
    {
        *sendptr++ = SysData.AgingTestData.Rev_Word2[i];
    }

    length = (uint32_t)(sendptr - (uint16_t *)(SendData)); // ���㵱ǰ���ݰ��ĳ���
    sendptr = (uint16_t *)(SendData);
    EthernetReceiveSuccessString(length, sendptr);
}
/******************************************************************
�������ƣ�
��������:�����ϻ�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ReceiveAgingTestInfo(void *parameter)
{
    uint16_t *recvptr = parameter, atemp = 0, itemp;
    uint32_t VolTemp;
	
    SysData.AgingTestParameters.AgingTestTime.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.AgingTestTime.bytes.hbyte = *recvptr++;  // �ϻ�ʱ��
	
    SysData.AgingTestParameters.AgingMode.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.AgingMode.bytes.hbyte = *recvptr++;      // ģʽѡ��		
	
    SysData.AgingTestParameters.SetCurr.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.SetCurr.bytes.hbyte = *recvptr++;        // ����������ģʽ��  ��λA��A�Ŵ�100��
	
    SysData.AgingTestParameters.SetPower.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.SetPower.bytes.hbyte = *recvptr++;       // ���ʣ��㹦��ģʽ����λW��W�Ŵ�100��

    SysData.AgingTestParameters.PulseWidth[0].bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.PulseWidth[0].bytes.hbyte = *recvptr++;  // �����ȣ�����ģʽ����λus   

    SysData.AgingTestParameters.DutyCycle[0].bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.DutyCycle[0].bytes.hbyte = *recvptr++;   // ռ�ձȣ�����ģʽ�� ��λ�����Ŵ�10��

    SysData.AgingTestParameters.SetVOL.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.SetVOL.bytes.hbyte = *recvptr++;         // ����ѹģʽ��       �յ�����0

    SysData.AgingTestParameters.FirstSetVTime.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.FirstSetVTime.bytes.hbyte = *recvptr++;  // 0��VD  1��VG

    SysData.AgingTestParameters.FirstStopVTime.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.FirstStopVTime.bytes.hbyte = *recvptr++;  // 0��VD  1��VG
		
    SysData.AgingTestParameters.Set_VD_Vol.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.Set_VD_Vol.bytes.hbyte = *recvptr++;     // VD��ѹ	��λV��V�Ŵ�100��
		
    SysData.AgingTestParameters.Set_VD_VolUp.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.Set_VD_VolUp.bytes.hbyte = *recvptr++;   // VD��ѹ����	��λV��V�Ŵ�100��
		
    SysData.AgingTestParameters.Set_VG_Vol.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.Set_VG_Vol.bytes.hbyte = *recvptr++;     // VG�����ѹ	��λV��V�Ŵ�100��
		
    SysData.AgingTestParameters.Set_VG_VolUp.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.Set_VG_VolUp.bytes.hbyte = *recvptr++;   // VG��ѹ����	��λV��V�Ŵ�100��

		SysData.AgingTestParameters.Set_ID_CurrUp.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.Set_ID_CurrUp.bytes.hbyte = *recvptr++;  // ID��������	��λmA��mA�Ŵ�100��
		
    SysData.AgingTestParameters.Set_IG_CurrUp.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.Set_IG_CurrUp.bytes.hbyte = *recvptr++;  // IG��������	��λmA��mA�Ŵ�100��

		SysData.AgingTestParameters.SetTemp.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.SetTemp.bytes.hbyte = *recvptr++;        // �¶�����	��λ�棬�Ŵ�10��
		
    SysData.AgingTestParameters.SetTempUp.bytes.lbyte = *recvptr++;
    SysData.AgingTestParameters.SetTempUp.bytes.hbyte = *recvptr++;      // �¶�����	��λ�棬�Ŵ�10��

    VD_PRE_L;
		vTaskDelay(10);
    AgingStateConfig(AGING_STATUS_READY);

    EthernetReceiveSuccessString(0, 0);
		
}
/******************************************************************
�������ƣ�
��������:�����ϻ�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ReceiveAgingCommand(void *parameter)
{
    uint16_t *recvptr = parameter;
    uint16_t Recetemp;

    Recetemp = *recvptr++;
    SysData.AgingCommond = (SetAgingMode)(Recetemp & 0xff);      //�ϻ����� (1�ֽ�)
	
    SysData.AgingFinishAutoEndFlag = ((Recetemp & 0xff00) >> 8); //�ϻ����Ҫ��Ҫ�Զ�ֹͣ�ϻ� (1�ֽ�)
	
    Recetemp = *recvptr++;
    SysData.AlarmingAutoEndFlag = (Recetemp & 0xff);             // �ϻ�����Ҫ��Ҫ�Զ�ֹͣ�ϻ�,0��ͣ��1ֹͣ (1�ֽ�)
	
    SysData.AgingFollowStatus = ((Recetemp & 0xff00) >> 8);      // �ϻ�����״̬,�ò���  Ĭ�� 0 (1�ֽ�)
	
    Recetemp = *recvptr++;
    SysData.ResumeAgingCycleCnt.bytes.lbyte = Recetemp; // �����ϻ�ѭ������ (4�ֽ�)
    Recetemp = *recvptr++;
    SysData.ResumeAgingCycleCnt.bytes.hbyte = Recetemp; // �����ϻ�ѭ������
	
    Recetemp = *recvptr++;
    SysData.AgingFinishBeepONFlag = (Recetemp & 0xff);       // �ϻ�����Ƿ���������,0���� 1�� (1�ֽ�)
	
    SysData.AlarmingBeepONFlag = ((Recetemp & 0xff00) >> 8); // �ϻ�����������Ҫ��Ҫ��,0���� (1�ֽ�)

    Recetemp = *recvptr++;
		SysData.DeviceVoltageDrop.bytes.lbyte  =  Recetemp;                  
		Recetemp = *recvptr++;
	  SysData.DeviceVoltageDrop.bytes.hbyte  =  Recetemp;      // ����ѹ�� ��4�ֽڣ�
		
    EthernetReceiveSuccessString(0, 0);
		
}

/******************************************************************
�������ƣ�
��������:������λ���·���У׼KB
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
#define Line(x, y)                                           \
    {                                                        \
       for (i = 0; i < y; i++)                               \
         {                                                   \
            Calib[0].x[i].KVal.bytes.lbyte = *recvptr++;     \
            Calib[0].x[i].KVal.bytes.hbyte = *recvptr++;     \
            Calib[0].x[i].BVal.bytes.lbyte = *recvptr++;     \
            Calib[0].x[i].BVal.bytes.hbyte = *recvptr++;     \
          }                                                  \
    }       /* y--->��GRADE ��λ����ѹ�����ĳ̿ػؼ쵵λ�� Calib[0]����ֻ��һ����λ*/   
		
void ReceiveCalibrationKB(void *parameter)
{
    uint16_t *recvptr = parameter, *kbptr, *dptr, *sptr, i, j;
    uint32_t u32_len;
    uint16_t CAL_TYPE;
    kbptr = (uint16_t *)(KbData);
    u32_len = ALLKB_WORD;
    CAL_TYPE = *recvptr++;
		if(CAL_TYPE!=0x13){
			recvptr += 2;	
		}
		else{}
    switch (CAL_TYPE)
    {
		case ConVG:	
			   Line(ConVG1, 8) 
		     Line(ConVG2, 8)
		     break;
		case ConVD:	
			   Line(ConVD1, 10) 
		     Line(ConVD2, 10)
	       break;
		case FeeVG:	
         Line(FeeVG1, 8)		
         Line(FeeVG2, 8)	
		     break;
		case FeeVD:	
         Line(FeeVD1, 10)
         Line(FeeVD2, 10)	
         break;		
		case FeeIG_Z:	
         Line(FeeIG1, 4)
         Line(FeeIG2, 4)	
         break;		
		case FeeIG_F:	
         Line(FeeIG10, 4)
         Line(FeeIG20, 4)	
         break;			
		case FeeID:	
         Line(FeeID1, 7)
         Line(FeeID2, 7)	
         break;			
    case FeeTemp:
			   Line(FeeTemp,18)
         break;
    default:
         break ;			
    }
	
    EthernetReceiveSuccessString(0, 0);

    // дflash
    kbptr = (uint16_t *)(Calib);
    dptr = (uint16_t *)(FLASH_KBDATA_BUFF);
    EraseNorFlashSector(dptr, 0);
    WriteNorFlashBlock(kbptr, dptr, sizeof(Calib)*2);
}
#undef Line
/******************************************************************
�������ƣ�
��������:���͸���λ��У׼��KB
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
#define Line(x, y)                                           \
    {                                                        \
				for (i = 0; i < y; i++)                              \
				{                                                    \
						*sendptr++ = Calib[0].x[i].KVal.bytes.lbyte;     \
						*sendptr++ = Calib[0].x[i].KVal.bytes.hbyte;     \
						*sendptr++ = Calib[0].x[i].BVal.bytes.lbyte;     \
						*sendptr++ = Calib[0].x[i].BVal.bytes.hbyte;     \
				}                                                    \
    }
void SendCalibrationKB(void *parameter)
{
    uint16_t *sendptr, *kbptr, i, j, u16_len;
    uint32_t length;
    uint16_t CAL_TYPE;
    u16_len = ALLKB_WORD;

    sendptr = (uint16_t *)(SendData);
    kbptr = (uint16_t *)(KbData);
	  CAL_TYPE = *(uint16_t *)parameter;
//    *sendptr++ = *(uint16_t *)parameter;
//		if(CAL_TYPE!=0x13){	  
			 *sendptr++ = CAL_TYPE;
//		}
//		else{
//		}
//    switch (*(uint16_t *)(SendData))
	switch (CAL_TYPE)
    {
		case ConVG:	
			   Line(ConVG1, 8) 
		     Line(ConVG2, 8)
		     break;
		case ConVD:	
			   Line(ConVD1, 10) 
		     Line(ConVD2, 10)
	       break;
		case FeeVG:	
         Line(FeeVG1, 8)		
         Line(FeeVG2, 8)	
		     break;
		case FeeVD:	
         Line(FeeVD1, 10)
         Line(FeeVD2, 10)	
         break;		
		case FeeIG_Z:	
         Line(FeeIG1, 4)
         Line(FeeIG2, 4)	
         break;		
		case FeeIG_F:	
         Line(FeeIG10, 4)
         Line(FeeIG20, 4)	
         break;			
		case FeeID:	
         Line(FeeID1, 7)
         Line(FeeID2, 7)	
         break;	
    case FeeTemp:
			   Line(FeeTemp,18)
         break;		
    default:
         break ;		
    }
    length = (uint32_t)(sendptr - (uint16_t *)(SendData)); // ���㵱ǰ���ݰ��ĳ���
    sendptr = (uint16_t *)(SendData);
    EthernetReceiveSuccessString(length, sendptr);
}
#undef Line
/******************************************************************
�������ƣ�
��������:��ȡ����汾��
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void GETEDITION(void *parameter)
{
    uint32_t parity;
    uint16_t i, j, len;
    uint16_t SoftVerSend[22];

    len = (sizeof(SoftVersionString_QDB) / 2); // 36

    for (i = 0, j = 0; i < len; i++)
    {
        SoftVerSend[i] = SoftVersionString_QDB[j++];
        SoftVerSend[i] += (SoftVersionString_QDB[j++] << 8);
    }
    parity = Crs(SoftVerSend, len);
    SoftVerSend[(len + 1)] = parity;
    SoftVerSend[(len + 2)] = (parity >> 16);

    EthernetReceiveSuccessString((len + 2), &SoftVerSend[0]);
}
/******************************************************************
�������ƣ�
��������:���߸��µ�һ�������ȡ�������ݳ���
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
// ���߸��������
static uint16_t pack_amount = 0;
void ONLINEUPDATE1(void *parameter)
{
    union DATA_BYTE updata_size;

    updata_size.bytes.lbyte = *((uint16_t *)parameter)++;
    updata_size.bytes.hbyte = *((uint16_t *)parameter)++;
    pack_amount = updata_size.data / 0x400 + 1;
    if (updata_size.data % 0x400 == 0)
        pack_amount--;
    // ��ȡ������
    Erase_Updata_Program_Area(); // ����������������
    Reset_Updata_Flag();         // �������±�־λ

    EthernetReceiveSuccessString(0, 0);
}
/******************************************************************
�������ƣ�
��������:���߸��µڶ���������ո��µ�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void ONLINEUPDATE2(void *parameter)
{
    uint16_t pack_num = *((uint16_t *)parameter - 3); // ��ȡ�����
    // ��ʼд��

    Program_Updata_Area(Updata_Program_Address, (uint32_t)parameter, 1024);  //�����ջ��������д��app flash��sizeΪ�ֽ�,updata_program_add����Ҫ��
    if (pack_amount - pack_num == 0) // ���һ���ѽ���
    {
        Set_Updata_Flag(); // ���±�־��λ
        reset_timer_handle = xTimerCreate("reset_timer",
                                          3000,
                                          pdTRUE, // �ظ�
                                          (void *)0,
                                          Reset_Callback); // ��λ��ʱ��
//        SEGGER_RTT_printf(0, "updata_program write ok,ready reset\n");
        xTimerStart(reset_timer_handle, 0); // ׼����λ
    }

    EthernetReceiveSuccessString(0, 0);
}

/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void Cal_ChanMode(void *parameter)
{
    uint16_t *recvptr = parameter;

    SysCtrl.ProgType = *recvptr++;   //ģʽѡ��
	
    // ��������
    EthernetReceiveSuccessString(0, 0);
}
/******************************************************************
�������ƣ�
��������: �ض�����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void Cal_ChanReadData(void *parameter)
{
	  float SendValueF=0,SendVlaueI=0;
	  uint32_t SendValue=0;
    uint16_t *recvptr = parameter;
    struct
    {
        uint16_t Mode;
        uint16_t ChanNo;
        uint32_t data;
    } __attribute__((packed)) SendData;
    SendData.Mode = *recvptr++;     //У׼��
    SendData.ChanNo = *recvptr++;   //��λ��

	if (SysCtrl.ProgType == Calibration)
  {
		if(SendData.ChanNo==1){
						 switch (SendData.Mode)
							{
							case 1://VG
									if(SysProg.TRANS_VOL_VALUE[0] < 0)//��
									{
										SendData.data = ((uint32_t)(SysProg.TRANS_VOL_VALUE[0]*(-10)));
										SendData.data|=0x80000000;
									}	
									else{
										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[0]) )*10; 
									}			
									break;
							case 2://VD
										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[2]) )*10; 
									break;
							case 3://IG
									if(SysProg.TRANS_CURR_VALUE[0] < 0)//��
									{
										SysProg.TRANS_CURR_VALUE[0] =SysProg.TRANS_CURR_VALUE[0]*100;
										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[0]*(-1))|0x80000000);									
									}	
									else{
										SysProg.TRANS_CURR_VALUE[0] =SysProg.TRANS_CURR_VALUE[0]*100;										
										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[0]) ); 										
									}		
									break;
							case 4://IG

									if(SysProg.TRANS_CURR_VALUE[0] < 0)//��
									{
										SysProg.TRANS_CURR_VALUE[0] =SysProg.TRANS_CURR_VALUE[0]*100;
										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[0]*(-1))|0x80000000);									
									}	
									else{
										SysProg.TRANS_CURR_VALUE[0] =SysProg.TRANS_CURR_VALUE[0]*100;										
										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[0]) ); 										
									}		
									break;									

							case 5://ID
										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[2]) )*100;
									break;
							default:
									break;
							}		  
		}
		else if(SendData.ChanNo==2)
		{
						 switch (SendData.Mode)
							{
							case 1://VG
									if(SysProg.TRANS_VOL_VALUE[1] < 0)//��
									{
										SendData.data = ((uint32_t)(SysProg.TRANS_VOL_VALUE[1]*(-10))|0x80000000);
									}	
									else{
										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[1]) )*10; 
									}			
									break;
							case 2://VD
										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[3]) )*10; 
									break;
							case 3://IG2
									if(SysProg.TRANS_CURR_VALUE[1] < 0)//��
									{
										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;										
										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[1]*(-1))|0x80000000);
									}	
									else{
										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;											
										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[1]) ); 
									}		
									break;
							case 4://IG2
									if(SysProg.TRANS_CURR_VALUE[1] < 0)//��
									{
										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;
										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[1]*(-1))|0x80000000);									
									}	
									else{
										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;										
										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[1]) ); 										
									}	
									break;									

							case 5://ID
										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[3]) )*100;
									break;
							default:
									break;
							}			
		
		}
 }
 else if (SysCtrl.ProgType == Measurement)  //����ģʽ
  {
		if(SendData.ChanNo==1){
						 switch (SendData.Mode)
							{
							case 1://VG
		SendValueF= SysProg.TRANS_VOL_VALUE[0];
		
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VG1FEE, 0);
	}
#endif				
		if(SysProg.TRANS_VOL_VALUE[0]<0){
			SendValueF = SendValueF*(-1);
			SendValue =(uint32_t)SendValueF*10;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendValue =(uint32_t)SendValueF*10;		
	    SendData.data =  SendValue;				
		}	
//									if(SysProg.TRANS_VOL_VALUE[0] < 0)//��
//									{
//										SendData.data = ((uint32_t)(SysProg.TRANS_VOL_VALUE[0]*(-10)));
//										SendData.data|=0x80000000;
//									}	
//									else{
//										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[0]) )*10; 
//									}												
									break;
							case 2://VD
								
	SendValueF =SysProg.TRANS_VOL_VALUE[2];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VD1FEE, 0);
	}
#endif	
  SendValue =(uint32_t)SendValueF*10;	
	SendData.data =  SendValue;							
//										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[2]) )*10; 
									break;
							case 3://IG z
								

		SendVlaueI= SysProg.TRANS_CURR_VALUE[0];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG1FEE, 0);
	}
#endif		
		if(SysProg.TRANS_CURR_VALUE[0]<0){
			SendVlaueI = SendVlaueI*(-1)*100;
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI*100;			
			SendValue =(uint32_t)SendVlaueI;		
	    SendData.data =  SendValue;				
		}		
//									if(SysProg.TRANS_CURR_VALUE[0] < 0)//��
//									{
//										SysProg.TRANS_CURR_VALUE[0] =SysProg.TRANS_CURR_VALUE[0]*100;
//										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[0]*(-1))|0x80000000);									
//									}	
//									else{
//										SysProg.TRANS_CURR_VALUE[0] =SysProg.TRANS_CURR_VALUE[0]*100;										
//										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[0]) ); 										
//									}		
									break;
							case 4://IG f
		SendVlaueI= SysProg.TRANS_CURR_VALUE[0];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG10FEE, 0);
	}
#endif		
		if(SysProg.TRANS_CURR_VALUE[0]<0){
			SendVlaueI = SendVlaueI*(-1)*100;
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI*100;			
			SendValue =(uint32_t)SendVlaueI;		
	    SendData.data =  SendValue;				
		}		
							
	
									break;									

							case 5://ID
								
							
		SendVlaueI= SysProg.TRANS_CURR_VALUE[2];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_ID1FEE, 0);
	}
#endif		
		if(SysProg.TRANS_CURR_VALUE[2]<0){
			SendVlaueI = SendVlaueI*(-1)*100;
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI*100;
			SendValue =(uint32_t)SendVlaueI;		
	    SendData.data =  SendValue;				
		}		
//										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[2]) )*100;
									break;
							default:
									break;
							}		  
		}
		else if(SendData.ChanNo==2)
		{
						 switch (SendData.Mode)
							{
							case 1://VG2
		SendValueF= SysProg.TRANS_VOL_VALUE[1];
		
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VG2FEE, 0);
	}
#endif				
		if(SysProg.TRANS_VOL_VALUE[1]<0){
			SendValueF = SendValueF*(-1);
			SendValue =(uint32_t)SendValueF*10;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendValue =(uint32_t)SendValueF*10;		
	    SendData.data =  SendValue;				
		}									
//									if(SysProg.TRANS_VOL_VALUE[1] < 0)//��
//									{
//										SendData.data = ((uint32_t)(SysProg.TRANS_VOL_VALUE[1]*(-10))|0x80000000);
//									}	
//									else{
//										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[1]) )*10; 
//									}			
									break;
							case 2://VD
	SendValueF =SysProg.TRANS_VOL_VALUE[3];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendValueF = FeedBackCalibration_v(SendValueF, VOL_VD2FEE, 0);
	}
#endif	
  SendValue =(uint32_t)SendValueF*10;	
	SendData.data =  SendValue;								
//										SendData.data =( ((uint32_t )SysProg.TRANS_VOL_VALUE[3]) )*10; 
									break;
							case 3://IG2 z
								
							
		SendVlaueI= SysProg.TRANS_CURR_VALUE[1];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG2FEE, 0);
	}
#endif		
		if(SysProg.TRANS_CURR_VALUE[1]<0){
			SendVlaueI = SendVlaueI*(-1)*100;
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI*100;			
			SendValue =(uint32_t)SendVlaueI;		
	    SendData.data =  SendValue;				
		}									
//									if(SysProg.TRANS_CURR_VALUE[1] < 0)//��
//									{
//										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;										
//										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[1]*(-1))|0x80000000);
//									}	
//									else{
//										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;											
//										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[1]) ); 
//									}		
									break;
							case 4://IG2 f
								
		SendVlaueI= SysProg.TRANS_CURR_VALUE[1];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_IG20FEE, 0);
	}
#endif		
		if(SysProg.TRANS_CURR_VALUE[1]<0){
			SendVlaueI = SendVlaueI*(-1)*100;
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI*100;			
			SendValue =(uint32_t)SendVlaueI;		
	    SendData.data =  SendValue;				
		}									
//									if(SysProg.TRANS_CURR_VALUE[1] < 0)//��
//									{
//										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;
//										SendData.data = ((uint32_t)(SysProg.TRANS_CURR_VALUE[1]*(-1))|0x80000000);									
//									}	
//									else{
//										SysProg.TRANS_CURR_VALUE[1] =SysProg.TRANS_CURR_VALUE[1]*100;										
//										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[1]) ); 										
//									}	
									break;									

							case 5://ID
								
		SendVlaueI= SysProg.TRANS_CURR_VALUE[3];
#if ENABLE_KB == TRUE
	if ((SysCtrl.ProgType == Measurement) || (SysCtrl.ProgType == NormalStation))
	{
		SendVlaueI = FeedBackCalibration_i(SendVlaueI, CURR_ID2FEE, 0);
	}
#endif		
		if(SysProg.TRANS_CURR_VALUE[3]<0){
			SendVlaueI = SendVlaueI*(-1)*100;
			SendValue =(uint32_t)SendVlaueI;
		  SendValue |=0x80000000;
	    SendData.data =  SendValue;	
		}
		else{
			SendVlaueI =SendVlaueI*100;			
			SendValue =(uint32_t)SendVlaueI;		
	    SendData.data =  SendValue;				
		}								
//										SendData.data =( ((uint32_t )SysProg.TRANS_CURR_VALUE[3]) )*100;
									break;
							default:
									break;
							}			
		
		}
 }
  EthernetReceiveSuccessString(sizeof(SendData) / 2, (void *)&SendData);
}
/******************************************************************
�������ƣ�
��������: У׼ģʽ�����·���ѹ ����ģʽ����Ҫ����KB����
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void Cal_ChanWriteData(void *parameter)
{
	  VD_PRE_H;
		ConCLR(RESET);          //��������
	  vTaskDelay(20);	
    uint16_t *recvptr = parameter;
    uint16_t Mode, ChanNo,SwitchMode;
//	  uint32_t SetData;
////    uint16_t unite;
    union DATA_BYTE SetData;
    float SetConVol;
    float SendVolData;
		uint16_t  SetDA=0;
		float SetVol=0;
		uint16_t SetDACnt=0;	

    Mode   = *recvptr++;   //У׼�� 2�ֽ�
    ChanNo = *recvptr++;   //�ò�������Ϊ0
	  SetData.bytes.lbyte= *recvptr++;
	  SetData.bytes.hbyte= *recvptr++;	
	  SwitchMode =Mode;


    if (SysCtrl.ProgType == Measurement)
    {
			  if(ChanNo==1){
				   switch (Mode)
						{
						 case 1: //VG
								SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
						    if(SetData.data>0x80000000){
								SetConVol =(-1)*SetConVol;
								}
								SendVolData = ErrorCompensation_v(SetConVol, VOL_VG1CON, 0);
								SendVolData = SendVolData / 1000.0;
								// ��DA
								SGM5349_DA( VG1_DA ,SendVolData ,DYB);	
								vTaskDelay(200);									
             break;
 						 case 2://VD
								SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;					 
								SendVolData = ErrorCompensation_v(SetConVol, VOL_VD1CON, 0);
								SendVolData = SendVolData / 1000.0;
								// ��DA
								SetVol =  (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
						    SetVol  =  SetVol/1000;
								SetDA  =  (SetVol/10.0)-1;
								SetDACnt = SetDA;
						    if(SetData.data==0){
									SGM5349_DA( VD1_DA ,0 ,DYB);
								}
								else{
									for(SetDA=0; SetDA<SetDACnt; SetDA++){
											SGM5349_DA( VD1_DA ,0+SetDA*10 ,DYB );
											vTaskDelay(100);
										}													
									SGM5349_DA( VD1_DA ,SendVolData ,DYB);											
								}
								vTaskDelay(200);		

							
             break;
             case 3:
             case 4:
             case 5:
							if(SetData.data!=0)
							{
 										if(SetData.data>0x80000000){
											SGM5349_DA( VG1_DA ,-4 ,DYB);
											SGM5349_DA( VG2_DA ,-4 ,DYB);																		
									    }
									   else{
											SendVolData =4;
											SGM5349_DA( VG1_DA ,SendVolData ,DYB);
											SGM5349_DA( VG2_DA ,SendVolData ,DYB);										 										 
										 }
											SGM5349_DA( VD1_DA ,4 ,DYB);
											SGM5349_DA( VD2_DA ,4 ,DYB);				
							}
							else
							{
									SGM5349_DA( VG1_DA ,0 ,DYB);
									SGM5349_DA( VG2_DA ,0 ,DYB);
									SGM5349_DA( VD1_DA ,0 ,DYB);
									SGM5349_DA( VD2_DA ,0 ,DYB);						
							}            
            break;				 
						default :
						 break ; 					 
					  } 
				}
				else if(ChanNo==2){
				   switch (SwitchMode)
						{					
						 case 1://VG
							SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
							if(SetData.data>0x80000000){
							SetConVol =(-1)*SetConVol;
							}						 
							SendVolData = ErrorCompensation_v(SetConVol, VOL_VG2CON, 0);
							SendVolData = SendVolData / 1000.0;
							// ��DA
							SGM5349_DA( VG2_DA ,SendVolData ,DYB);	
								vTaskDelay(200);								
						 break ;
             case 2://VD
							SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;					 
							SendVolData = ErrorCompensation_v(SetConVol, VOL_VD2CON, 0);
							SendVolData = SendVolData / 1000.0;

							SetVol =  (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
							SetVol = SetVol/1000;							
							SetDA  =  (SetVol/10.0)-1;
							SetDACnt = SetDA;
						    if(SetData.data==0){
									SGM5349_DA( VD2_DA ,0 ,DYB);
								}
								else{
									for(SetDA=0; SetDA<SetDACnt; SetDA++){
											SGM5349_DA( VD2_DA ,0+SetDA*10 ,DYB );
											vTaskDelay(100);
										}													
									SGM5349_DA( VD2_DA ,SendVolData ,DYB);									
								}	
								
								
								vTaskDelay(200);									
						 break ;							 
             case 3:							 
             case 4:					 
             case 5:
							if(SetData.data!=0)
							{
 										if(SetData.data>0x80000000){
											SGM5349_DA( VG1_DA ,-4 ,DYB);
											SGM5349_DA( VG2_DA ,-4 ,DYB);																		
									    }
									   else{
											SendVolData =4;
											SGM5349_DA( VG1_DA ,SendVolData ,DYB);
											SGM5349_DA( VG2_DA ,SendVolData ,DYB);										 										 
										 }
											SGM5349_DA( VD1_DA ,4 ,DYB);
											SGM5349_DA( VD2_DA ,4 ,DYB);					
							}
							else
							{
									SGM5349_DA( VG1_DA ,0 ,DYB);
									SGM5349_DA( VG2_DA ,0 ,DYB);
									SGM5349_DA( VD1_DA ,0 ,DYB);
									SGM5349_DA( VD2_DA ,0 ,DYB);						
							} 							 
						 break;	
						default :
						 break ; 							
						}							 
				
				}
		}
    else if (SysCtrl.ProgType == Calibration)
    {
			  if( ChanNo == 1 )
					{
							 switch (SwitchMode)
								{
								 case 1://VG
										SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
								    SendVolData = SetConVol;
										SendVolData = SendVolData / 1000.0;
										if (SetData.data & 0x80000000)
										{
												SendVolData = SendVolData * (-1.0);
										}
										// ��DA
										SGM5349_DA( VG1_DA ,SendVolData ,DYB);	
							    	vTaskDelay(200);											
								 break ;
								 case 2://VD
										SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
								    SendVolData = SetConVol;								 
										SendVolData = SendVolData / 1000.0;
										// ��DA
										SetVol =  (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
										SetVol = SetVol/1000;
										SetDA  =  (SetVol/10.0)-1;
										SetDACnt = SetDA;
						    if(SetData.data==0){
									SGM5349_DA( VD1_DA ,0 ,DYB);
								}
								else{
									for(SetDA=0; SetDA<SetDACnt; SetDA++){
											SGM5349_DA( VD1_DA ,0+SetDA*10 ,DYB );
											vTaskDelay(100);
										}													
									SGM5349_DA( VD1_DA ,SendVolData ,DYB);									
								}		
								vTaskDelay(200);									
								 break ;
								 case 3:
								 case 4:
								 case 5:
									if(SetData.data!=0)
									{ 
 										if(SetData.data>0x80000000){
											SGM5349_DA( VG1_DA ,-4 ,DYB);
											SGM5349_DA( VG2_DA ,-4 ,DYB);																		
									    }
									   else{
											SendVolData =4;
											SGM5349_DA( VG1_DA ,SendVolData ,DYB);
											SGM5349_DA( VG2_DA ,SendVolData ,DYB);										 										 
										 }
											SGM5349_DA( VD1_DA ,4 ,DYB);
											SGM5349_DA( VD2_DA ,4 ,DYB);				
									}
									else
									{
											SGM5349_DA( VG1_DA ,0 ,DYB);
											SGM5349_DA( VG2_DA ,0 ,DYB);
											SGM5349_DA( VD1_DA ,0 ,DYB);
											SGM5349_DA( VD2_DA ,0 ,DYB);						
									}            
							  	break;				 
								 default :
									break ; 
								} 
				}
				else if(ChanNo==2){
				   switch (SwitchMode)
						{					
						 case 1:
							SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
							SendVolData = SetConVol;						 
							SendVolData = SendVolData / 1000.0;
							if (SetData.data & 0x80000000)
							{
									SendVolData = SendVolData * (-1.0);
							}
							// ��DA
							SGM5349_DA( VG2_DA ,SendVolData ,DYB);	
								vTaskDelay(200);								
						 break ;
             case 2:
							SetConVol = (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
						  SendVolData = SetConVol;
							SendVolData = SendVolData / 1000.0;
							// ��DA
							SetVol =  (SetData.data & 0x7fffffff) / MUL_VOLGRADE;
							SetVol = SetVol/1000;							
							SetDA  =  (SetVol/10.0)-1;
							SetDACnt = SetDA;
						    if(SetData.data==0){
									SGM5349_DA( VD2_DA ,0 ,DYB);
								}
								else{
									for(SetDA=0; SetDA<SetDACnt; SetDA++){
											SGM5349_DA( VD2_DA ,0+SetDA*10 ,DYB );
											vTaskDelay(100);
										}													
									SGM5349_DA( VD2_DA ,SendVolData ,DYB);									
								}					
								vTaskDelay(200);									
						 break ;							 
             case 3:	 
             case 4:
             case 5:
							if(SetData.data!=0)
							{
 										if(SetData.data>0x80000000){
											SGM5349_DA( VG1_DA ,-4 ,DYB);
											SGM5349_DA( VG2_DA ,-4 ,DYB);																		
									    }
									   else{
											SendVolData =4;
											SGM5349_DA( VG1_DA ,SendVolData ,DYB);
											SGM5349_DA( VG2_DA ,SendVolData ,DYB);										 										 
										 }
											SGM5349_DA( VD1_DA ,4 ,DYB);
											SGM5349_DA( VD2_DA ,4 ,DYB);		
							}
							else
							{
									SGM5349_DA( VG1_DA ,0 ,DYB);
									SGM5349_DA( VG2_DA ,0 ,DYB);
									SGM5349_DA( VD1_DA ,0 ,DYB);
									SGM5349_DA( VD2_DA ,0 ,DYB);						
							}					 
						 break ;	
						 default :
						 break ; 							
						}							 
				
				}			
    }
    EthernetReceiveSuccessString(0, 0);
		ConCLR(SET);
}

void ReceiveAgingFANInfo(void *parameter)
 {  
  	uint32_t FSDGRADE=0;
//    uint32_t *recvptr = parameter;
//	  FSDGRADE = *recvptr++;
	 uint16_t *recvptr = parameter;
	 SysData.AgingTestParameters.SetFanGrade.bytes.lbyte =*recvptr++;
	 SysData.AgingTestParameters.SetFanGrade.bytes.hbyte =*recvptr++;	
   FSDGRADE  =SysData.AgingTestParameters.SetFanGrade.data;
//    SysData.AgingTestParameters.AgingTestTime.bytes.lbyte = *recvptr++;
//    SysData.AgingTestParameters.AgingTestTime.bytes.hbyte = *recvptr++;  // �ϻ�ʱ��
		FSDSortControl(FSDGRADE);
	 EthernetReceiveSuccessString(0, 0);
}
 

//���ͱ�����λ������
void SendUpData(void *parameter)
{
//#define Backdata_Program_Address (0x08000000 + 0x80000) // ����ԭ�ȵĳ������
    uint16_t *sendptr, i;
    uint32_t length = 0;	
	  uint16_t *sptr;		
		for( uint32_t PackCnt=0;PackCnt<192;PackCnt++ ){
			
			sendptr = (uint16_t *)(SendData);
			*sendptr++ = PackCnt+1;		
			 length = 0;
			 sptr = (uint16_t *)(Backdata_Program_Address+0x400*PackCnt);
			for (uint16_t i=(PackCnt*512);i<(512*PackCnt+512);i++) //0-1023 0x08080400 +0x3ff
			{
				*sendptr++ = *sptr++;
			}
			length = (uint32_t)(sendptr - (uint16_t *)(SendData));
			sendptr = (uint16_t *)(SendData);
			EthernetReceiveSuccessString_Updata(length, sendptr);			
		  vTaskDelay(1000);
		}
}

