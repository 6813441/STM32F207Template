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
