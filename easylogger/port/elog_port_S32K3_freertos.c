/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for S32K3 platform.
 * Created on: 2015-04-28
 */
 
#include "elog.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"

#if defined (CFG_EASY_LOGGER_TERMINAL_UART)
#include "Lpuart_Uart_Ip.h"
#endif

/********************************************************************************************************
 *                                  Private Variable Definitions                                        *
 *******************************************************************************************************/
static SemaphoreHandle_t output_lock;

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;
    output_lock = xSemaphoreCreateMutex();
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
#if defined (CFG_EASY_LOGGER_TERMINAL_UART)
    for(unsigned int i = 0; i < size; i++)
    {
        while((CFG_EASY_LOGGER_TERMINAL_UART->STAT & LPUART_STAT_TC_MASK)>>LPUART_STAT_TC_SHIFT==0);
        CFG_EASY_LOGGER_TERMINAL_UART->DATA = log[i];
    }
#else
    /* output to terminal */
    printf("%.*s", size, log);
#endif
    //TODO output to flash
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    //__disable_irq();
    xSemaphoreTake(output_lock, portMAX_DELAY);
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    //__enable_irq();
    xSemaphoreGive(output_lock);
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    static char cur_system_time[16] = "";
    snprintf(cur_system_time, 16, "%lu", xTaskGetTickCount());
    return cur_system_time;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    return "pid:1008";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    return "tid:24";
}
