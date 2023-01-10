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
#include "Devic_Init.h"  //�豸��ʼ��
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H"
#include <KEY_Run.h>
#include <NRF_WDT.H>
#include <Unix_Time.h>  //ʱ���ת��ͷ�ļ�
#include <BLE_Data.h>
#include <Act_PassWord.h>
#include "simple_uart.h"

#define Uart_Close_Time_Size 200   //Uart_Close ����

#define Uart_Close_Time 		5			//Uart_Close �ر�ʱ��

#ifdef NB_Enable


#define Uart_RX_String_Size  500

#else

#define Uart_RX_String_Size 20     //ָ��ͷ����

#endif

extern uint32_t Uart_Close_T;   //��ʱ�رմ�������

extern uint8_t A71_ID_Tab[];

void Uart_Put_String(uint8_t *Uart_Dat,uint8_t len);

void Uart_Get_String(void);  //��ȡ 10ms�ڴ���ץȡ�ı���

void Uart_Start_TC(void);    // ��������

void A71_Send_Init(void);   //��A71 ģ�鷢�ͳ�ʼ��ָ��

void A71_Send_ID(void);     // A71 ���� ID 

void A71_Send_PD(void);     //ʹ�豸�������ģʽ

void A71_Data_TX(uint8_t	*s);   //�������ݰ�������

uint8_t A71_RF_RX_MODE(void);     //�������ģʽ

uint8_t Need_Of_RX_Dat(void);   //����RF����

uint8_t A71_TX_Dat(uint8_t *s);    //�����ݷ��͵�RF



#endif

