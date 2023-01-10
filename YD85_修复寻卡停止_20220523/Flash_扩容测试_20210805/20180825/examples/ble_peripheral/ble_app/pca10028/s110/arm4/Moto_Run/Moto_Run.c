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

//#define Mot_reverse 1			//电机反转标志位

void Lock_Mot_Init()
{
	nrf_gpio_cfg_output(MOT_A);
	nrf_gpio_cfg_output(MOT_B);	
}

void Lock_Mot_Default()
{

	MOT_A_L;
	MOT_B_L;
	
	nrf_gpio_cfg_default(MOT_A);
	nrf_gpio_cfg_default(MOT_B);

}


extern uint8_t PassWord_Error_Num; //声明密码输入错误次数值

uint8_t LED_CL =0;				// LED 清空状态位

//APP_TIMER_DEF (Read_My_Key);  //声明新的软件定时器名称
void Lock_Open()  //开锁
{
	uint8_t Volt_Value;
	if(Lock_State==0)
	{
		
		#ifdef Key_By_MPR121
		
		if(MPR121_RUN==1)
		app_timer_stop(Read_My_Key);
		#endif
		
		input_speak(6);
		
	//	nrf_delay_ms(1000);
		
		Lock_Mot_Init();  //启动电机模块
		
		if((MPR121_RUN==0)&&(nrf_gpio_pin_read(LEDB)==0))		//当触摸板未唤醒时开锁同时开一下灯
		{
			LED_CL =1;						//确认 LED 未开启 用于之后关闭
			nrf_gpio_pin_set(LEDB);
		}

		#ifdef Mot_reverse			//电机反转

		MOT_A_L;
		
		MOT_B_H;
				
		#else
		
		MOT_A_H;
		
		MOT_B_L;
		
		
		#endif
		
		nrf_delay_ms(400);
		
		Lock_Mot_Default();  //关闭电机模块	
		Lock_State =1;
		
		if(LED_CL==1)		//当触摸板未唤醒时开锁同时开一下灯
			{
				LED_CL =0;
				nrf_gpio_pin_clear(LEDB);

			}

		
	//  input_speak(9);
		Moto_Timer_Init(); //开启定时关锁		
	
		//nrf_delay_ms(500);	//mpr121_init();
		
		#ifdef Key_By_MPR121
		if(MPR121_RUN==1)
		{
		mpr121_Run(); //重置键盘
		app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	
		}	
		else
			mpr121_init();
		#endif

	}
	
	
	Volt_Value = Batt_Value();  //先测量电池电压

	if(Volt_Value<10) //当电量低于20%时提示电量低
	{
		nrf_delay_ms(500);
		input_speak(8);
	}
	
	PassWord_Error_Num =0;  //开门过后 重置错误密码次数
	//mpr121_init();  //在运转电机后，重置键盘
}

extern uint8_t MPR121_RUN; //状态
extern uint16_t MPR121_T;   //计时，

#define Enable_Lock_Close  1    //使能电机回位

void Lock_Close()  //关锁
{
	if(Lock_State==1)
	{
		#ifdef Key_By_MPR121 
		if(MPR121_RUN==1)
		app_timer_stop(Read_My_Key);	
		#endif
		#ifdef Enable_Lock_Close
		
		Lock_Mot_Init();  //启动电机模块
		
		#ifdef Mot_reverse
		
			MOT_A_H;		
			MOT_B_L;
				
		
		#else
		
			MOT_A_L;
			
			MOT_B_H;
		
		#endif	
			nrf_delay_ms(500);
			Lock_Mot_Default();
			
		#endif
		
			Lock_State =0;
			 nrf_delay_ms(500);	//mpr121_init();
				if(Devic_Static==true)  //在绑定后 电机关唤醒状态停止
				{
					MPR121_T = 9000;  //置计数值 使设备关闭led
				}
		
			
		#ifdef Key_By_MPR121
		if(MPR121_RUN==1)
			{
			mpr121_Run(); //重置键盘
			app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	
				if((Devic_Static==true)&&(MPR121_RUN==true))  //在绑定后 电机关唤醒状态停止
				{
					MPR121_T = 80;  //置计数值 使设备关闭led
				}
			}	
			else
				mpr121_init();
		#endif
				
	}
//	mpr121_init();
}

