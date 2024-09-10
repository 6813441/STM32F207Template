#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
xSemaphoreHandle PID_semaphore = NULL;
TaskHandle_t TempCtrl_handle;
extern uint16_t ptemp;
void Task_ControlTemperature(void *p);
static void TempCtrl(void);
double SortAndFilter_f(double *dpoint, uint16_t num);
void GlideFilterTEM(uint16_t ChanNo);

void Task_ControlTemperature_Init(void)
{
    // ��ʼ����ʱ��
    // ��ʱ��ʱ������Դ
    /*    RCU_CFG1 �Ĵ��� ��24λ
        0����� RCU_CFG0 �Ĵ����� APB1PSC/APB2PSC λ���ֵΪ 0b0xx(CK_APBx = CK_AHB) �� 0b100(CK_APBx = CK_AHB/2) ��
        �� ʱ �� ʱ �� �� ��CK_AHB(CK_TIMERx = CK_AHB)������ʱ��ʱ�ӵ��� APB ʱ�ӵ�����
        (�� APB1��Ķ�ʱ���� CK_TIMERx = 2 x CK_APB1���� APB2 ��Ķ�ʱ����CK_TIMERx = 2 x CK_APB2)��*/
    /*ʱ��Ƶ��120M/4*2/1 �õ���ʱ��ʱ��TIMER_CLK  ����������Դ������д�ĳ�4���2 */
    rcu_periph_clock_enable(RCU_TIMER1); // ע�������ǰ���д�RCU����
    //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
    timer_deinit(TIMER1);
    timer_internal_clock_config(TIMER1); // ѡ���ڲ�ʱ��CK_TIMER(200M)

    /* TIMER1����Ϊ0.1ms��ʱ */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);

    timer_initpara.prescaler = 5999;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // ����prescaler,clockdivision���ոö�ʱ��ʱ������Ϊ1M
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // ������ʽ���ø��ݱ��ؾ���
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // ��������
    timer_initpara.period = 4999;                       // ����Ϊ0.5ms����һ��
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

    nvic_irq_enable(TIMER1_IRQn, 3, 0);
    timer_disable(TIMER1);
    timer_flag_clear(TIMER1, TIMER_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    // �����µ��ź���
    if (PID_semaphore == NULL)
    {
        vSemaphoreCreateBinary(PID_semaphore);
        xSemaphoreTake(PID_semaphore, 0);
    }
    // �����߳�

    xTaskCreate(Task_ControlTemperature,
                "TempCtrl_Thread",
                1024,
                NULL,
                15,
                &TempCtrl_handle);
	//SetAD5044(Temp_Ctr, 2, itemp, (((SysCtrl.TempLimit[itemp].data) * CALTEMPCON) / 5000 * 16384));
	/*for (uint8_t itemp = 0; itemp < 4; itemp++) // �����������ȿ���
	{	
	   SetAD5044(Temp_Ctr, 2, itemp, (((2.5) * CALTEMPCON) / 5000 * 16384));
	}*/
}
void TIMER1_IRQHandler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (SET == timer_flag_get(TIMER1, TIMER_FLAG_UP))
    {
        xSemaphoreGiveFromISR(PID_semaphore, &xHigherPriorityTaskWoken);
    }

    timer_flag_clear(TIMER1, TIMER_FLAG_UP);

    /* switch tasks if necessary */
    if (pdFALSE != xHigherPriorityTaskWoken)
    {
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
}
void TempCtrlStop(void)
{
	timer_disable(TIMER1);
	//TODO:ֹͣPID�������������ֹͣ����
}
void Task_ControlTemperature(void *p)
{
    timer_enable(TIMER1);
	
	SysCtrl.HOT[0]=30000;
	SysCtrl.HOT[1]=60000;
	SysCtrl.HOT[2]=60000;
	SysCtrl.HOT[3]=60000;	
    for (;;)
    {
        if (pdTRUE == xSemaphoreTake(PID_semaphore, portMAX_DELAY))
        {
            
        }
    }
}
extern uint16_t FSD_cnt;
static void TempCtrl(void)
{
   for (uint32_t i =0;i<MAX_TEMP_CH;i++){
    // �¶Ƚӽ��趨�¶ȿ���
    if ((SysData.AgingTestData.StationTemp[i].data > SysData.AgingTestParameters.SetStationTemp[i].data*0.85) && (0 == SysProg.Ctrl_AGE_RUN[i]))
    {
		SysCtrl.PIDControl[i].SetValue = SysData.AgingTestParameters.SetStationTemp[i].data;
        SysCtrl.HOT[i] = 60000-PID_Caculate(&SysCtrl.PIDControl[i]);
    }
    else
    {
        if (0 == SysProg.Ctrl_AGE_RUN[i])
        {
            SysCtrl.HOT[i] = 0;
        }
        else
        {
            SysCtrl.HOT[i] = 60000;
        }
    }
	}
    ePWM1Setup(SysCtrl.FSD[0], SysCtrl.FSD[1]);
    ePWM2Setup(SysCtrl.FSD[2], SysCtrl.FSD[3]);
    ePWM4Setup(SysCtrl.HOT[2], SysCtrl.HOT[3]);
    ePWM3Setup(SysCtrl.HOT[0], SysCtrl.HOT[1]);

//    // �������
//    FSD_cnt++;
//    if (FSD_cnt > 15)
//    {
//        for (itemp = 0; itemp < 4; itemp++)
//        {
//            if (SysCtrl.PT100Temp[itemp] > SysCtrl.SetTemp[itemp].data) // �¶ȴ����趨�¶����ȣ��������
//            {
//                if ((SysCtrl.PT100Temp[itemp] - SysCtrl.SetTemp[itemp].data > 30))
//                {
//                    if (SysCtrl.FSD[itemp] < 57000)
//                    {
//                        SysCtrl.FSD[itemp] += 2000;
//                    }
//                }
//            }
//            else if (SysCtrl.PT100Temp[itemp] < SysCtrl.SetTemp[itemp].data) // �¶�С���趨�¶����ȣ��������
//            {
//                if ((SysCtrl.SetTemp[itemp].data - SysCtrl.PT100Temp[itemp] > 30))
//                {
//                    if (SysCtrl.FSD[itemp] > 2000)
//                    {
//                        SysCtrl.FSD[itemp] -= 2000;
//                    }
//                }
//            }
//            else
//            {
//            }

//            if (SysCtrl.FSD[itemp] > 50000) // �趨�������
//            {
//                SysCtrl.FSD[itemp] = 50000;
//            }
//            else if (SysCtrl.FSD[itemp] <= 2000) // �������
//            {
//                SysCtrl.FSD[itemp] = 0;
//            }
//        }
//        FSD_cnt = 0;
//        ePWM1Setup(SysCtrl.FSD[0], SysCtrl.FSD[1]);
//        ePWM2Setup(SysCtrl.FSD[2], SysCtrl.FSD[3]);
//    }
}
