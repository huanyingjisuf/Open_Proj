#ifndef BLE_DATA_H
#define BLE_DATA_H

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


extern uint8_t BLE_Data[];  // BLE数据缓存
extern uint8_t BLE_Data_Len;  //保留的数据长度

void Data_Input(uint8_t *BLE_DAT_p,uint8_t Dat_Len);  //待处理数据载入(分包数据组合)
void BLE_DAT_SW(void);  //分析待处理数据
void BLE_Dat_Select(void);  //根据指令跳转子函数
void Mate_Command_Run(void);   //运行绑定指令
void Time_Ref_Run(void);  //运行时间更新指令
void Get_Time_Run(void);  //运行时间返回指令
uint8_t Batt_Value(void);  //求电量等级
void APP_OpenLock_Run(void);  //运行软件开门
void Add_Information_Run(void);  //运行载入信息指令

void EX_BLE_ACK_Run(uint8_t ACK_Type,uint8_t ACK_ID);			//外部 BLE 应答发送

void Del_Information_Run(void);  //销毁已载入的信息指令
void Del_A_PassWord_Run(void);  //删除单个密码数据
void Del_Mate_Run(void); //解绑设备
void Read_OP_Door_note(void);  //读取开门记录指令
void TX_Door_note(void);  //读取开门记录指令 一个个·返回
void Manual_Add_Card(void);  //手动发卡

void Manual_Add_Card_ACK(uint8_t Ack_Now,uint16_t Card_ID,uint8_t Card_Type);  //手动发卡回应报文

void Normally_Open_Door_Run(void);			//常开门回应报文

void Del_User_Key(void);    //删除用户钥匙

void CL_Door_note(void);		//

void More_Act_PassWord_Mode_Run(void);  // 修改

void Del_A_More_Act_PassWord_Run(void);    //允许 删除单个多组动态密码指令

#endif
