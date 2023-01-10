#ifndef _Devic_Init_H_
#define _Devic_Init_H_
#include <time.h>  //ʱ��ĺ�
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
#include <Unix_Time.h>  //ʱ���ת��ͷ�ļ�
#include <NRF_WDT.H>
#include <NRF_WDT_RUN.H>
#include <F_print.h>


//ADC���ֵ
extern uint32_t ADC_Value;
extern uint32_t VCC_Value,VCC_Value_H,VCC_Value_L1,VCC_Value_L2; //������
#define VDD_Value 120000  //оƬADC�ο���ѹ*10000�� ��������
#define RES_d  10 //������ѹ����

extern uint16_t ADC_Div;  //ADC��С�ֱ���
extern uint16_t MPR_Del;  //�ڵ͹���ģʽ�£���ʱ�������̰� 
extern  uint16_t MPR_Value;  //��ȡ��ֵ

//extern uint8_t Init_Key_Num;

extern uint8_t TIME_STATE; //ʱ����Ч״̬

extern uint8_t Type_AB_UID[];  //AB��UID���ݻ���


#define Devic_PAGE_Len  40  //�豸������ʹ�ÿռ��С

#define tab_dat_size   1850

#define Read_Key_Time_Size		25  //��ʱ����������

#define Devic_Static_True_Mpr121_T_Size		(10*1000/Read_Key_Time_Size)					//�󶨺����廽��ʱ��

#define Devic_Static_False_Mpr121_T_Size		(15*1000/Read_Key_Time_Size)				//��ǰ�����廽��ʱ��


#define Timer1_Time_Size		100//time1 ��ʱ����

#define My_test_timer_Time_Size  30000   //My_Test_timer_Timer �ļ��ʱ��

#define Board_Timer_Size    My_test_timer_Time_Size/1000				// ����ʱ��У��ֵ (ͨ�� My_test_timer_Time_Size У��)

#define my_Op_Door_timer_Size   200  

#define Read_Card_Error_Num_Full   12					//   Read_Card_Error_Num  �����ֵֵ

void Dis_Conner(void);  //�Ͽ����� (main.c)

 
extern uint8_t Devic_Static;  //�豸��״̬ true �󶨣�false δ��
extern uint8_t Devic_Num[Devic_PAGE_Len];
extern uint8_t tab_dat[tab_dat_size];
extern uint8_t SEC,MIN,HOUR,DAY,MON,YEAR; //ʵʱʱ�� 24H

#ifdef NB_Enable

extern uint8_t NB_Dat[NB_Dat_Size];

extern uint32_t NB_Dat_Len;  // NB�յ���������

extern uint32_t NB_Dat_Len;   // NB ������ ���ڵ���������
extern uint8_t NB_Run;     //NB ����״̬


#endif

void  start_init(void);

void Get_Time(void);  // ��ȡ�ⲿʱ��

void Devic_static_read(void);

uint8_t BLE_TX_Run(uint8_t *s,uint8_t len);  //�������ݵ�BLE  ��main.c��

void ref_BLE_Adv(void);  //ˢ��BLE�㲥����

void CL_SKY1311_IRQ(void);

void sky1311_Init(void);


void Start_Key_Run(void);  //ֱ�ӻ��Ѽ��̵ĺ���

void Update_Time(uint64_t itime); //���µ�ʱ��д���ⲿʱ��

void bsp_event_handler_O(void);

void Devic_static_read(void);			// ��ȡ�豸����Ϣ(���°���Ϣ)
	
uint32_t CL_LED_Cheak_RC_Value(void);   //������ �ر�LED ʱ�� RC ֵ

//void read_key_run(void);

#endif

