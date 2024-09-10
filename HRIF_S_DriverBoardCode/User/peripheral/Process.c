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
    // 初始化定时器
    // 定时器时钟树来源
    /*    RCU_CFG1 寄存器 的24位
        0：如果 RCU_CFG0 寄存器的 APB1PSC/APB2PSC 位域的值为 0b0xx(CK_APBx = CK_AHB) 或 0b100(CK_APBx = CK_AHB/2) ，
        定 时 器 时 钟 等 于CK_AHB(CK_TIMERx = CK_AHB)，否则定时器时钟等于 APB 时钟的两倍
        (在 APB1域的定时器： CK_TIMERx = 2 x CK_APB1，在 APB2 域的定时器：CK_TIMERx = 2 x CK_APB2)。*/
    /*时钟频率120M/4*2/1 得到定时器时钟TIMER_CLK  两倍因素来源于上文写的除4则乘2 */
    rcu_periph_clock_enable(RCU_TIMER1); // 注意参数名前面有待RCU。。
    //	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
    timer_deinit(TIMER1);
    timer_internal_clock_config(TIMER1); // 选择内部时钟CK_TIMER(200M)

    /* TIMER1配置为0.1ms定时 */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);

    timer_initpara.prescaler = 5999;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 根据prescaler,clockdivision最终该定时器时钟评率为1M
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 触发方式设置根据边沿决定
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 计数向上
    timer_initpara.period = 4999;                       // 设置为0.5ms触发一次
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

    nvic_irq_enable(TIMER1_IRQn, 3, 0);
    timer_disable(TIMER1);
    timer_flag_clear(TIMER1, TIMER_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    // 创建新的信号量
    if (PID_semaphore == NULL)
    {
        vSemaphoreCreateBinary(PID_semaphore);
        xSemaphoreTake(PID_semaphore, 0);
    }
    // 创建线程

    xTaskCreate(Task_ControlTemperature,
                "TempCtrl_Thread",
                1024,
                NULL,
                15,
                &TempCtrl_handle);
	//SetAD5044(Temp_Ctr, 2, itemp, (((SysCtrl.TempLimit[itemp].data) * CALTEMPCON) / 5000 * 16384));
	/*for (uint8_t itemp = 0; itemp < 4; itemp++) // 电流保护优先开启
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
	//TODO:停止PID，并清除参数，停止加热
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
    // 温度接近设定温度控制
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

//    // 风机控制
//    FSD_cnt++;
//    if (FSD_cnt > 15)
//    {
//        for (itemp = 0; itemp < 4; itemp++)
//        {
//            if (SysCtrl.PT100Temp[itemp] > SysCtrl.SetTemp[itemp].data) // 温度大于设定温度三度，风机增速
//            {
//                if ((SysCtrl.PT100Temp[itemp] - SysCtrl.SetTemp[itemp].data > 30))
//                {
//                    if (SysCtrl.FSD[itemp] < 57000)
//                    {
//                        SysCtrl.FSD[itemp] += 2000;
//                    }
//                }
//            }
//            else if (SysCtrl.PT100Temp[itemp] < SysCtrl.SetTemp[itemp].data) // 温度小于设定温度三度，风机减速
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

//            if (SysCtrl.FSD[itemp] > 50000) // 设定风机上限
//            {
//                SysCtrl.FSD[itemp] = 50000;
//            }
//            else if (SysCtrl.FSD[itemp] <= 2000) // 风机下限
//            {
//                SysCtrl.FSD[itemp] = 0;
//            }
//        }
//        FSD_cnt = 0;
//        ePWM1Setup(SysCtrl.FSD[0], SysCtrl.FSD[1]);
//        ePWM2Setup(SysCtrl.FSD[2], SysCtrl.FSD[3]);
//    }
}
