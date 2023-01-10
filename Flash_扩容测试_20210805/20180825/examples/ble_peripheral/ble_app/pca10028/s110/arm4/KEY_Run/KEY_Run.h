#ifndef _KEY_Run_H_
#define _KEY_Run_H_

#include "mpr121.h" //�������õļ�������

#include "Moto_Run.h"  //���� �����������

#include "speak.h"  //������������  

#define  LEDB 	9
//#define  LEDB1 	12

#define  LEDR 	8  //LED��

/*********************************************
 
 KEY_Run �ļ����������������̣������������֮��Ĺ�ϵ
 Ӧ�����Լ���Ϊ��Ҫ�豸��������ϵ
************************************************/

APP_TIMER_DEF (Moto_Close_Timer);  //���ڿ���������Ƶĵ�� 
#define APP_TIMER_PRESCALER      0        

extern uint8_t Lock_State;

extern uint8_t Double_Key; //������־λ
extern uint8_t Turn_Key; //šԿ�ױ�־λ
extern uint8_t Latch_Bolt; //б���־λ

void Read_Lock_Bon(void);  //��ȡ��״̬

//void Moto_Timer_Hander(void);
void Moto_Timer_Init(void);
//void Moto_Timer_Hander(void);
void LED_BR_Init(void);

void LOCK_OPEN_Init(void);

#endif
