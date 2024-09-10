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
#ifndef __all_gpio_H
#define __all_gpio_H
#include "main.h"

#define LED1_ON gpio_bit_write(GPIOA, GPIO_PIN_0, RESET)
#define LED1_OFF gpio_bit_write(GPIOA, GPIO_PIN_0, SET)
#define LED1_TOGGLE (gpio_output_bit_get(GPIOA, GPIO_PIN_0)==SET)?LED1_ON:LED1_OFF
#define LED2_ON gpio_bit_write(GPIOA, GPIO_PIN_3, RESET)
#define LED2_OFF gpio_bit_write(GPIOA, GPIO_PIN_3, SET)
#define LED2_TOGGLE (gpio_output_bit_get(GPIOA, GPIO_PIN_3)==SET)?LED2_ON:LED2_OFF




void All_GPIO_Simple_Init(void);

#endif
