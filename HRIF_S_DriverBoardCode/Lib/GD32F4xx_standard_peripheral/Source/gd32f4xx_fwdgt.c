/*!
    \file    gd32f4xx_fwdgt.c
    \brief   FWDGT driver

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

/*
   独立看门狗定时器（FWDGT）
*独立看门狗定时器（FWDGT）有独立时钟源（IRC32K）。因此就算是主时钟失效了，它仍然
 能保持工作状态，这非常适合于需要独立环境且对计时精度要求不高的场合。
*当内部向下计数器的计数值达到0，独立看门狗会产生一个复位。使能独立看门狗的寄存器写
保护功能可以避免寄存器的值被意外的配置篡改。
   主要特征
* 独立运行的12位向下计数器；
* 如果看门狗定时器被使能，那么当向下计数器的值达到0时产生系统复位；
* 独立时钟源，独立看门狗定时器在主时钟故障（例如待机和深度睡眠模式下）时仍能工作；
* 独立看门狗定时器硬件控制位，可以用来控制是否在上电时自动启动独立看门狗定时器；
* 可以配置独立看门狗定时器在调试模式下选择停止还是继续工作。

*/

#include "gd32f4xx_fwdgt.h"

/* write value to FWDGT_CTL_CMD bit field */
#define CTL_CMD(regval)             (BITS(0,15) & ((uint32_t)(regval) << 0))
/* write value to FWDGT_RLD_RLD bit field */
#define RLD_RLD(regval)             (BITS(0,11) & ((uint32_t)(regval) << 0))

/*!
    \brief    enable write access to FWDGT_PSC and FWDGT_RLD
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fwdgt_write_enable(void)
{
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
}

/*!
    \brief    disable write access to FWDGT_PSC and FWDGT_RLD
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fwdgt_write_disable(void)
{
    FWDGT_CTL = FWDGT_WRITEACCESS_DISABLE;
}

/*!
    \brief    start the free watchdog timer counter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fwdgt_enable(void)
{
    FWDGT_CTL = FWDGT_KEY_ENABLE;
}

/*!
    \brief    reload the counter of FWDGT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fwdgt_counter_reload(void)
{
    FWDGT_CTL = FWDGT_KEY_RELOAD;
}

/*!
    \brief    configure counter reload value, and prescaler divider value
    \param[in]  reload_value: specify reload value(0x0000 - 0x0FFF)
    \param[in]  prescaler_div: FWDGT prescaler value
                only one parameter can be selected which is shown as below:
      \arg        FWDGT_PSC_DIV4: FWDGT prescaler set to 4
      \arg        FWDGT_PSC_DIV8: FWDGT prescaler set to 8
      \arg        FWDGT_PSC_DIV16: FWDGT prescaler set to 16
      \arg        FWDGT_PSC_DIV32: FWDGT prescaler set to 32
      \arg        FWDGT_PSC_DIV64: FWDGT prescaler set to 64
      \arg        FWDGT_PSC_DIV128: FWDGT prescaler set to 128
      \arg        FWDGT_PSC_DIV256: FWDGT prescaler set to 256
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus fwdgt_config(uint16_t reload_value, uint8_t prescaler_div)
{
    uint32_t timeout = FWDGT_PSC_TIMEOUT;
    uint32_t flag_status = RESET;
  
    /* enable write access to FWDGT_PSC,and FWDGT_RLD */
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
  
    /* wait until the PUD flag to be reset */
    do{
       flag_status = FWDGT_STAT & FWDGT_STAT_PUD;
    }while((--timeout > 0U) && ((uint32_t)RESET != flag_status));
    
    if ((uint32_t)RESET != flag_status){
        return ERROR;
    }

    /* configure FWDGT */
    FWDGT_PSC = (uint32_t)prescaler_div;

    timeout = FWDGT_RLD_TIMEOUT;
    /* wait until the RUD flag to be reset */
    do{
       flag_status = FWDGT_STAT & FWDGT_STAT_RUD;
    }while((--timeout > 0U) && ((uint32_t)RESET != flag_status));
   
    if ((uint32_t)RESET != flag_status){
        return ERROR;
    }
    
    FWDGT_RLD = RLD_RLD(reload_value);
    
    /* reload the counter */
    FWDGT_CTL = FWDGT_KEY_RELOAD;

    return SUCCESS;
}

/*!
    \brief    get flag state of FWDGT
    \param[in]  flag: flag to get 
                only one parameter can be selected which is shown as below:
      \arg        FWDGT_STAT_PUD: a write operation to FWDGT_PSC register is on going
      \arg        FWDGT_STAT_RUD: a write operation to FWDGT_RLD register is on going
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus fwdgt_flag_get(uint16_t flag)
{
    if(RESET != (FWDGT_STAT & flag)){
        return SET;
  }

    return RESET;
}
