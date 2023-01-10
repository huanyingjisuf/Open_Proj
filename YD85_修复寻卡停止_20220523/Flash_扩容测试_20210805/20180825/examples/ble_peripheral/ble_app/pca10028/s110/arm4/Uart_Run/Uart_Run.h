#ifndef _Uart_Run_H_
#define _Uart_Run_H_

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "nrf_delay.h"
#include "mpr121.h"
#include "w25q32.h"
#include "BL5372.h"
#include "Moto_Run.h"
#include "Devic_Init.h"  //设备初始化
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H"
#include <KEY_Run.h>
#include <NRF_WDT.H>
#include <Unix_Time.h>  //时间戳转换头文件
#include <BLE_Data.h>
#include <Act_PassWord.h>
#include "simple_uart.h"

#define Uart_Close_Time_Size 200   //Uart_Close 周期

#define Uart_Close_Time 		5			//Uart_Close 关闭时间

#ifdef NB_Enable


#define Uart_RX_String_Size  500

#else

#define Uart_RX_String_Size 20     //指纹头报文

#endif

extern uint32_t Uart_Close_T;   //定时关闭串口周期

extern uint8_t A71_ID_Tab[];

void Uart_Put_String(uint8_t *Uart_Dat,uint8_t len);

void Uart_Get_String(void);  //获取 10ms内串口抓取的报文

void Uart_Start_TC(void);    // 开启串口

void A71_Send_Init(void);   //向A71 模块发送初始化指令

void A71_Send_ID(void);     // A71 设置 ID 

void A71_Send_PD(void);     //使设备进入掉电模式

void A71_Data_TX(uint8_t	*s);   //发送数据包到无线

uint8_t A71_RF_RX_MODE(void);     //进入接收模式

uint8_t Need_Of_RX_Dat(void);   //接收RF数据

uint8_t A71_TX_Dat(uint8_t *s);    //将数据发送到RF



#endif

