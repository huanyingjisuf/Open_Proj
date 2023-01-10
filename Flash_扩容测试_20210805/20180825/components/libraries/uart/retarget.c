/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef NRF_LOG_USES_RTT

#include <stdio.h>
#include <stdint.h>
#include "app_uart.h"
#include "nordic_common.h"
#include "nrf_error.h"

#include "uart.h"
#include "simple_uart.h"  //定义外设串口  //printf 重定向 外设

#if !defined(__ICCARM__)
struct __FILE 
{
    int handle;
};
#endif

FILE __stdout;
FILE __stdin;


#if defined(__CC_ARM) ||  defined(__ICCARM__)
//int fgetc(FILE * p_file)
//{
//	
////    uint8_t input;
////    while (app_uart_get(&input) == NRF_ERROR_NOT_FOUND)
////    {
////        // No implementation needed.
////    }
////    return input;
//	
//		//		/* 等待串口1输入数据 */
//		/* 返回接收到的数据 */
//		return (int)simple_uart_get();
//}


//int fputc(int ch, FILE * p_file)
//{
//	
////    UNUSED_PARAMETER(p_file);

////    UNUSED_VARIABLE(app_uart_put((uint8_t)ch));
////    return ch;
//	//		/* Place your implementation of fputc here */
//		/* 发送一个字节的数据 */
//		simple_uart_put((uint8_t)ch);
//		/* 等待发送完成 */
//		return ch;
//}
#elif defined(__GNUC__)


int _write(int file, const char * p_char, int len)
{
    int i;

    UNUSED_PARAMETER(file);

    for (i = 0; i < len; i++)
    {
        UNUSED_VARIABLE(app_uart_put(*p_char++));
    }

    return len;
}


int _read(int file, char * p_char, int len)
{
    UNUSED_PARAMETER(file);
    while (app_uart_get((uint8_t *)p_char) == NRF_ERROR_NOT_FOUND)
    {
        // No implementation needed.
    }

    return 1;
}
#endif

#endif // #ifndef NRF_LOG_USES_RTT
