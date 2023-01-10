#include "uart.h"
#include "simple_uart.h"
#include <stdarg.h>
#include <stdio.h>



//void USART_Configuration(void)//串口初始化函数
//  {  

//    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
// //   nrf_gpio_cfg_output(ERROR_PIN); // ERROR_PIN configured as output.

//		}			

		
/************************************************************** 
* 函数名  : fputc()
* 描述    : 重定义putc函数，这样可以使用printf函数从串口1打印输出
* 输入    : None
* 输出    : None
* 返回    : None
* 作者    : 青风
* 创建日期: 2014.1.1
* 版本    : V1.00
*************************************************************/
int fputc(int ch, FILE *f)
{
		/* Place your implementation of fputc here */
		/* 发送一个字节的数据 */
		simple_uart_put((uint8_t)ch);
		/* 等待发送完成 */
		return ch;
}

/************************************************************** 
* 函数名  : fputc()
* 描述    : 重定义putc函数，这样可以使用printf函数从串口1获取数据
* 输入    : None
* 输出    : None
* 返回    : None
* 作者    : 青风
* 创建日期: 2014.1.1
* 版本    : V1.00
*************************************************************/
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		/* 返回接收到的数据 */
		return (int)simple_uart_get();
}
