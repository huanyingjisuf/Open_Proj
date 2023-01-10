#ifndef _KEY_Run_H_
#define _KEY_Run_H_

#include "mpr121.h" //声明调用的键盘驱动

#include "Moto_Run.h"  //声明 电机运行驱动

#include "speak.h"  //声明语音驱动  

#define  LEDB 	9
//#define  LEDB1 	12

#define  LEDR 	8  //LED灯

/*********************************************
 
 KEY_Run 文件，用于联动，键盘，电机，及语音之间的关系
 应用于以键盘为主要设备的联动关系
************************************************/

APP_TIMER_DEF (Moto_Close_Timer);  //用于开关锁体控制的电机 
#define APP_TIMER_PRESCALER      0        

extern uint8_t Lock_State;

extern uint8_t Double_Key; //反锁标志位
extern uint8_t Turn_Key; //拧钥匙标志位
extern uint8_t Latch_Bolt; //斜舌标志位

void Read_Lock_Bon(void);  //读取锁状态

//void Moto_Timer_Hander(void);
void Moto_Timer_Init(void);
//void Moto_Timer_Hander(void);
void LED_BR_Init(void);

void LOCK_OPEN_Init(void);

#endif
