#ifndef _Devic_Init_H_
#define _Devic_Init_H_
#include <time.h>  //时间的宏
#include "ADC.H"

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
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.h"
#include "KEY_Run.h"
#include "Flash_RW.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include <Unix_Time.h>  //时间戳转换头文件
#include <NRF_WDT.H>
#include <NRF_WDT_RUN.H>
#include <F_print.h>


//ADC相关值
extern uint32_t ADC_Value;
extern uint32_t VCC_Value,VCC_Value_H,VCC_Value_L1,VCC_Value_L2; //运算结果
#define VDD_Value 120000  //芯片ADC参考电压*10000倍 方便运算
#define RES_d  10 //采样电压倍数

extern uint16_t ADC_Div;  //ADC最小分辨率
extern uint16_t MPR_Del;  //在低功耗模式下，定时重启键盘板 
extern  uint16_t MPR_Value;  //获取的值

//extern uint8_t Init_Key_Num;

extern uint8_t TIME_STATE; //时间有效状态

extern uint8_t Type_AB_UID[];  //AB卡UID数据缓存


#define Devic_PAGE_Len  40  //设备号扇区使用空间大小

#define tab_dat_size   1850

#define Read_Key_Time_Size		25  //定时读键盘周期

#define Devic_Static_True_Mpr121_T_Size		(10*1000/Read_Key_Time_Size)					//绑定后触摸板唤醒时间

#define Devic_Static_False_Mpr121_T_Size		(15*1000/Read_Key_Time_Size)				//绑定前触摸板唤醒时间


#define Timer1_Time_Size		100//time1 定时周期

#define My_test_timer_Time_Size  30000   //My_Test_timer_Timer 的间隔时间

#define Board_Timer_Size    My_test_timer_Time_Size/1000				// 板载时钟校验值 (通过 My_test_timer_Time_Size 校验)

#define my_Op_Door_timer_Size   200  

#define Read_Card_Error_Num_Full   12					//   Read_Card_Error_Num  溢出阈值值

void Dis_Conner(void);  //断开连接 (main.c)

 
extern uint8_t Devic_Static;  //设备绑定状态 true 绑定，false 未绑定
extern uint8_t Devic_Num[Devic_PAGE_Len];
extern uint8_t tab_dat[tab_dat_size];
extern uint8_t SEC,MIN,HOUR,DAY,MON,YEAR; //实时时间 24H

#ifdef NB_Enable

extern uint8_t NB_Dat[NB_Dat_Size];

extern uint32_t NB_Dat_Len;  // NB收到的数据量

extern uint32_t NB_Dat_Len;   // NB 缓存区 存在的数据数量
extern uint8_t NB_Run;     //NB 开启状态


#endif

void  start_init(void);

void Get_Time(void);  // 获取外部时钟

void Devic_static_read(void);

uint8_t BLE_TX_Run(uint8_t *s,uint8_t len);  //发送数据到BLE  （main.c）

void ref_BLE_Adv(void);  //刷新BLE广播名称

void CL_SKY1311_IRQ(void);

void sky1311_Init(void);


void Start_Key_Run(void);  //直接唤醒键盘的函数

void Update_Time(uint64_t itime); //将新的时间写入外部时钟

void bsp_event_handler_O(void);

void Devic_static_read(void);			// 获取设备绑定信息(更新绑定信息)
	
uint32_t CL_LED_Cheak_RC_Value(void);   //仅采样 关闭LED 时的 RC 值

//void read_key_run(void);

#endif

