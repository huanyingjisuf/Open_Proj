#ifndef __speak_H
#define __speak_H


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
#include "Devic_Init.h"


#include <MotO_Run.h>
#include <KEY_Run.h>
#include <mpr121.h>
#include <speak.h>
#include "Devic_Init.h"  //设备初始化
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H"
#include <KEY_Run.h>
#include <NRF_WDT.H>
#include <Unix_Time.h>  //时间戳转换头文件
#include <BLE_Data.h>
#include <Act_PassWord.h>
//#define speak1 1

#define speak2 1



void auart_init(void);
void auart_sleep(void);
void input_abus_byte(uint8_t byte);
void input_speak(uint8_t byte);
void timer2_init(void);



#endif
