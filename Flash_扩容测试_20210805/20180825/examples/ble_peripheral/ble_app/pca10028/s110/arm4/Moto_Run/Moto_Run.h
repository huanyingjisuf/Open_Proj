#ifndef __MOTO_RUN_H
#define __MOTO_RUN_H

/******************************************************
锁体运行状态监测与控制

*******************************************************/
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


#define MOT_A 3  //定义锁电机驱动GPIO
#define MOT_B 4

#define MOT_A_H nrf_gpio_pin_set(MOT_A)

#define MOT_A_L nrf_gpio_pin_clear(MOT_A)

#define MOT_B_H nrf_gpio_pin_set(MOT_B)

#define MOT_B_L nrf_gpio_pin_clear(MOT_B)



void Lock_Open(void);
void Lock_Close(void);


void Lock_Mot_Init(void);

void Lock_Mot_Default(void);

#endif
