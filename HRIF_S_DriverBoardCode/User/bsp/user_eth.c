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

#include <stdio.h>
// �ض���lwip�е�printf��ֹ����
int fputc(int ch, FILE *stream)
{
    // while((USART1->ISR & 0X40) == 0);
    // USART1->TDR = (uint8_t) ch;
    return ch;
}


extern uint8_t IP_ADDR3;
void Get_IP(void)
{
    IP_ADDR3=Read_DIP_Switch();
    if (IP_ADDR3 == 0)
        IP_ADDR3 = 1; // 未拨码默认1
}
