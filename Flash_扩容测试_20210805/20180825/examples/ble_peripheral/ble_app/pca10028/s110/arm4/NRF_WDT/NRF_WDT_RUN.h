#ifndef _NRF_WDT_RUN_H_
#define _NRF_WDT_RUN_H_


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


void NRF_WDT_Init(void);  //初始化看门狗
void NRF_WDT_Feed(void);  //喂狗

#endif
