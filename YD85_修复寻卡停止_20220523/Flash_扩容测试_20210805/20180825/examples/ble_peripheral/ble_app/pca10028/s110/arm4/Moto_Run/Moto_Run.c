#include <MotO_Run.h>
#include <KEY_Run.h>
#include <mpr121.h>
#include <speak.h>
#include "Devic_Init.h"  //�豸��ʼ��
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H"
#include <KEY_Run.h>
#include <NRF_WDT.H>
#include <Unix_Time.h>  //ʱ���ת��ͷ�ļ�
#include <BLE_Data.h>
#include <Act_PassWord.h>

//#define Mot_reverse 1			//�����ת��־λ

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


extern uint8_t PassWord_Error_Num; //������������������ֵ

uint8_t LED_CL =0;				// LED ���״̬λ

//APP_TIMER_DEF (Read_My_Key);  //�����µ������ʱ������
void Lock_Open()  //����
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
		
		Lock_Mot_Init();  //�������ģ��
		
		if((MPR121_RUN==0)&&(nrf_gpio_pin_read(LEDB)==0))		//��������δ����ʱ����ͬʱ��һ�µ�
		{
			LED_CL =1;						//ȷ�� LED δ���� ����֮��ر�
			nrf_gpio_pin_set(LEDB);
		}

		#ifdef Mot_reverse			//�����ת

		MOT_A_L;
		
		MOT_B_H;
				
		#else
		
		MOT_A_H;
		
		MOT_B_L;
		
		
		#endif
		
		nrf_delay_ms(400);
		
		Lock_Mot_Default();  //�رյ��ģ��	
		Lock_State =1;
		
		if(LED_CL==1)		//��������δ����ʱ����ͬʱ��һ�µ�
			{
				LED_CL =0;
				nrf_gpio_pin_clear(LEDB);

			}

		
	//  input_speak(9);
		Moto_Timer_Init(); //������ʱ����		
	
		//nrf_delay_ms(500);	//mpr121_init();
		
		#ifdef Key_By_MPR121
		if(MPR121_RUN==1)
		{
		mpr121_Run(); //���ü���
		app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	
		}	
		else
			mpr121_init();
		#endif

	}
	
	
	Volt_Value = Batt_Value();  //�Ȳ�����ص�ѹ

	if(Volt_Value<10) //����������20%ʱ��ʾ������
	{
		nrf_delay_ms(500);
		input_speak(8);
	}
	
	PassWord_Error_Num =0;  //���Ź��� ���ô����������
	//mpr121_init();  //����ת��������ü���
}

extern uint8_t MPR121_RUN; //״̬
extern uint16_t MPR121_T;   //��ʱ��

#define Enable_Lock_Close  1    //ʹ�ܵ����λ

void Lock_Close()  //����
{
	if(Lock_State==1)
	{
		#ifdef Key_By_MPR121 
		if(MPR121_RUN==1)
		app_timer_stop(Read_My_Key);	
		#endif
		#ifdef Enable_Lock_Close
		
		Lock_Mot_Init();  //�������ģ��
		
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
				if(Devic_Static==true)  //�ڰ󶨺� ����ػ���״ֹ̬ͣ
				{
					MPR121_T = 9000;  //�ü���ֵ ʹ�豸�ر�led
				}
		
			
		#ifdef Key_By_MPR121
		if(MPR121_RUN==1)
			{
			mpr121_Run(); //���ü���
			app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	
				if((Devic_Static==true)&&(MPR121_RUN==true))  //�ڰ󶨺� ����ػ���״ֹ̬ͣ
				{
					MPR121_T = 80;  //�ü���ֵ ʹ�豸�ر�led
				}
			}	
			else
				mpr121_init();
		#endif
				
	}
//	mpr121_init();
}

