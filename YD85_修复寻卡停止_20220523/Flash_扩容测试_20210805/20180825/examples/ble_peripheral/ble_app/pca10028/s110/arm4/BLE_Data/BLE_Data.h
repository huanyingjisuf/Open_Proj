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
#include "Devic_Init.h"  //�豸��ʼ��
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H"
#include <KEY_Run.h>
#include <NRF_WDT.H>


extern uint8_t BLE_Data[];  // BLE���ݻ���
extern uint8_t BLE_Data_Len;  //���������ݳ���

void Data_Input(uint8_t *BLE_DAT_p,uint8_t Dat_Len);  //��������������(�ְ��������)
void BLE_DAT_SW(void);  //��������������
void BLE_Dat_Select(void);  //����ָ����ת�Ӻ���
void Mate_Command_Run(void);   //���а�ָ��
void Time_Ref_Run(void);  //����ʱ�����ָ��
void Get_Time_Run(void);  //����ʱ�䷵��ָ��
uint8_t Batt_Value(void);  //������ȼ�
void APP_OpenLock_Run(void);  //�����������
void Add_Information_Run(void);  //����������Ϣָ��

void EX_BLE_ACK_Run(uint8_t ACK_Type,uint8_t ACK_ID);			//�ⲿ BLE Ӧ����

void Del_Information_Run(void);  //�������������Ϣָ��
void Del_A_PassWord_Run(void);  //ɾ��������������
void Del_Mate_Run(void); //����豸
void Read_OP_Door_note(void);  //��ȡ���ż�¼ָ��
void TX_Door_note(void);  //��ȡ���ż�¼ָ�� һ����������
void Manual_Add_Card(void);  //�ֶ�����

void Manual_Add_Card_ACK(uint8_t Ack_Now,uint16_t Card_ID,uint8_t Card_Type);  //�ֶ�������Ӧ����

void Normally_Open_Door_Run(void);			//�����Ż�Ӧ����

void Del_User_Key(void);    //ɾ���û�Կ��

void CL_Door_note(void);		//

void More_Act_PassWord_Mode_Run(void);  // �޸�

void Del_A_More_Act_PassWord_Run(void);    //���� ɾ���������鶯̬����ָ��

#endif
