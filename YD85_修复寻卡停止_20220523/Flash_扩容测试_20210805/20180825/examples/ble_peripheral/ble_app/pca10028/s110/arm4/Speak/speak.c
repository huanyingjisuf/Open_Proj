#include <speak.h>
#include <KEY_Run.h>
#include "nrf_gpiote.h"

//#define speak_for_Been   1
#define speak_for_IC     1

//const uint16_t  Buzzer_Tab[] = //  蜂鸣器 DO-SI (1-7) 音阶 字符  加 “滴” (8)  0 无效
//{
//	0,756,665,558,716,637,568,506,150
//};



   
const uint16_t  Buzzer_Tab[] = //  蜂鸣器 DO-SI (1-7) 音阶 字符  加 “滴” (8)  0 无效
{
	
	0,300,250,200,716,637,568,506,230,250

};

#define DI_Delay   125
#define Buzz_Data		30			//		//配置 蜂鸣器 IO 口

#ifdef speak1
#define auart 29 //配置单线串口

#define auart_H nrf_gpio_pin_set(auart)
#define auart_L nrf_gpio_pin_clear(auart)
#endif

#ifdef speak2

#define auart_data  30
#define auart_rst  29

#define auart_data_H nrf_gpio_pin_set(auart_data)
#define auart_data_L nrf_gpio_pin_clear(auart_data)

#define auart_rst_H nrf_gpio_pin_set(auart_rst)
#define auart_rst_L nrf_gpio_pin_clear(auart_rst)


#endif

 

uint16_t Buzz_Data_T =0;
void timer2_init(void)			// 定时器 2 初始化
{
	
//		#ifdef speak_for_Been
//	
//		// 时钟 = 16M 
//		NRF_TIMER2->BITMODE = 0;   // (TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos); // 设置16bit模式
//		NRF_TIMER2->PRESCALER = 6;		//分频 2^9
//		NRF_TIMER2->SHORTS = 1<<18;			//(TIMER_SHORTS_COMPARE2_CLEAR_Enabled << TIMER_SHORTS_COMPARE2_CLEAR_Pos); //设置compare[2]事件产生时触发中断，在中断函数中清楚对应的compare

//		//2比较中断

//		NRF_TIMER2->MODE = 0;	//TIMER_MODE_MODE_Timer //计时模式
//		NRF_TIMER2->CC[0]= 247;  //0X7A12UL

//		//中断设置

//		NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE2_Enabled << TIMER_INTENSET_COMPARE2_Pos);
//	//	TIMER_INTENSET_COMPARE2_Disabled
//		
//	Buzz_Data_T =0;		//清空 BUZZ_Data_T 
//		

//    //开启中断
//    NVIC_ClearPendingIRQ(TIMER2_IRQn);
//    NVIC_SetPriority(TIMER2_IRQn, 1);
//    NVIC_ClearPendingIRQ(TIMER2_IRQn);
//    NVIC_EnableIRQ(TIMER2_IRQn);
//		
//		
//    //启动timer
//  //  NRF_TIMER2->TASKS_START = 1;
//		
//		#endif
		
}


void TIMER2_IRQHandler(void)  //定时器中断模式
{
	
//    if ((NRF_TIMER2->EVENTS_COMPARE[0] == 1)&& (NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE2_Msk))
//    {
//			NVIC_ClearPendingIRQ(TIMER2_IRQn);

//      NRF_TIMER2->EVENTS_COMPARE[0] = 0;
//      NRF_TIMER2->TASKS_CLEAR = 1; //	清空定时器
//			
//			nrf_gpio_pin_toggle(Buzz_Data);
//			if(Buzz_Data_T>0)
//			Buzz_Data_T++;
//			
//			if (Buzz_Data_T>DI_Delay)
//			{
//				Buzz_Data_T =0;

////				NRF_TIMER2->TASKS_STOP = 1;
////				nrf_gpio_pin_clear(Buzz_Data);
////				
////				NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE2_Disabled << TIMER_INTENSET_COMPARE2_Pos);
////				
////				NVIC_DisableIRQ(TIMER2_IRQn);		//关中断
//				
//			}
//			
//		}
//		else if(Buzz_Data_T==0)
//		{
//			Buzz_Data_T =0;
//		}

}


void auart_init()
{
	#ifdef speak_for_IC
	#ifdef speak1
	nrf_gpio_cfg_output(auart);
	auart_H;
	#endif
	
	#ifdef speak2
	
	nrf_gpio_cfg_output(auart_data);
	nrf_gpio_cfg_output(auart_rst);
	
	auart_data_L;
	auart_rst_L;
	
	#endif
	
	#endif
		
	#ifdef speak_for_Been
	
	
	nrf_gpio_cfg_output(Buzz_Data);
	nrf_gpio_pin_clear(Buzz_Data);
	
	
	#endif
	
	
}

void auart_sleep()
{

	#ifdef speak1
	nrf_gpio_cfg_default(auart);
	#endif
	
}

//void input_abus_byte(uint8_t byte)  //第一种语音芯片
//{
//	uint8_t i;
//	uint8_t it;
//	it =byte;
//	auart_L;
//	nrf_delay_ms (5);
//	
//	for(i =0;i<8;i++)
//	{
//		if(it&0x01)  //dat 1
//		{
//			auart_H;
//			nrf_delay_us(600);
//			auart_L;
//			nrf_delay_us(200);
//		}
//		else  //dat 0
//		{
//				auart_H;
//				nrf_delay_us(200);
//				auart_L;
//				nrf_delay_us(600);
//		}
//		it=it>>1;
//	}
//	auart_H;
//	nrf_delay_ms(5);
//}


APP_TIMER_DEF (LEDR_Close_Timer);  //用于自动恢复 LED R

static void LEDR_Close_Timer_Hander(void *params)
{
		nrf_gpio_pin_clear(LEDR);
		app_timer_stop(LEDR_Close_Timer);
}

void LEDR_Close_Init()
{

	uint32_t err_code;

	err_code =
					app_timer_create (&LEDR_Close_Timer,APP_TIMER_MODE_REPEATED,LEDR_Close_Timer_Hander);
	APP_ERROR_CHECK(err_code);
	err_code =	app_timer_start( LEDR_Close_Timer ,APP_TIMER_TICKS(200,APP_TIMER_PRESCALER),NULL );			//注册新的软件定时器任务
	APP_ERROR_CHECK(err_code);

}



void Buzzer_Error()				//失败提示音
{
	uint32_t i;


		for(i =0;i<DI_Delay;i++)
		{
		
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
		
		}
		
		nrf_delay_ms(70);
		for(i =0;i<DI_Delay;i++)
		{
		
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
		
		}
		
		nrf_delay_ms(70);
		
		for(i =0;i<DI_Delay;i++)
		{
		
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
		
		}
		

	
}

void Buzzer_Ok()				//成功提示音
{
	uint32_t i;
	
			for(i =0;i<170;i++)
		{
		
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[1]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[1]);
		
		}
		
		nrf_delay_ms(50);
			
		for(i =0;i<170;i++)
		{
		
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[2]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[2]);
		
		}
		nrf_delay_ms(50);
		for(i =0;i<170;i++)
		{
		
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[3]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[3]);
		
		}
		
//		for(i =0;i<100;i++)
//		{
//		
//			nrf_gpio_pin_set(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[4]);
//			
//			nrf_gpio_pin_clear(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[4]);
//		
//		}
//		
//		for(i =0;i<100;i++)
//		{
//		
//			nrf_gpio_pin_set(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[5]);
//			
//			nrf_gpio_pin_clear(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[5]);
//		
//		}
//		
//		for(i =0;i<100;i++)
//		{
//		
//			nrf_gpio_pin_set(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[6]);
//			
//			nrf_gpio_pin_clear(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[6]);
//		
//		}
//		
//		for(i =0;i<100;i++)
//		{
//		
//			nrf_gpio_pin_set(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[7]);
//			
//			nrf_gpio_pin_clear(LEDB);
//			
//			nrf_delay_us(Buzzer_Tab[7]);
//		
//		}
		
			
}

uint8_t Buzzer_Di_Run;

//
//void Buzzer_Di_p()				//提示音  滴
//{
//	
//		if(Buzz_Data_T==0)
//		{
//			Buzz_Data_T =1;
//						//开启中断
//			NVIC_ClearPendingIRQ(TIMER2_IRQn);
//			NVIC_SetPriority(TIMER2_IRQn, 1);
//			NVIC_ClearPendingIRQ(TIMER2_IRQn);
//			NVIC_EnableIRQ(TIMER2_IRQn);
//			
//			Buzz_Data_T =1;
//			//启动timer
//			NRF_TIMER2->TASKS_START = 1;
//			
//			NVIC_ClearPendingIRQ(TIMER2_IRQn);
//			NRF_TIMER2->TASKS_STOP = 1;
//			NVIC_DisableIRQ(TIMER2_IRQn);
//		//	nrf_delay_ms(10);
//		}
//}
//

void Buzzer_Di()				//提示音  滴
{
	
		uint16_t i;
		
		for(i =0;i<DI_Delay;i++)
		{
			nrf_gpio_pin_set(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
			
			nrf_gpio_pin_clear(Buzz_Data);
			
			nrf_delay_us(Buzzer_Tab[8]);
			

			
		}
}

void Buzzer_Cl()
{
	uint16_t i;
	
	for(i =0;i<180;i++)
	{
		nrf_gpio_pin_set(Buzz_Data);
		
		nrf_delay_us(Buzzer_Tab[9]);
		
		nrf_gpio_pin_clear(Buzz_Data);
		
		nrf_delay_us(Buzzer_Tab[9]);
		
	}
	
}

//蜂鸣器运行函数  根据载入数字反馈对应声音
void Buzzer_Run(uint8_t byte)
{
	
	while(1)
	{
	
		switch(byte)
		{
			case 28:   //再输错两次后锁定键盘
			

			Buzzer_Error();
			
			nrf_delay_ms(500);
			
			Buzzer_Error();
			
			
			break;
			
			case 29:		//滴
			
			Buzzer_Di();
			
				break;
			
			case 4:  // 重置成功
				
				Buzzer_Ok();
			
				break;
			
			case 7:		//已开锁
				
			Buzzer_Ok();

				break;
			case 6:		//已开锁
				
			Buzzer_Ok();

				break;
			
			
			case 3:				//绑定成功
				
			Buzzer_Ok();
			
			break;
			
			case 9:		//操作成功
				
				Buzzer_Ok();
			
				break;
			case 10:		//操作失败
				
				Buzzer_Error();
				
			break;
			
			case 5:		//密码错误
				
				nrf_delay_ms(300);
				Buzzer_Error();
				
			break;
			
			case 17:		//反锁
				
				nrf_delay_ms(300);
				Buzzer_Error();
				nrf_delay_ms(300);
				Buzzer_Error();
			
				
			break;
			
			
			case 22:			//请刷卡
			
					Buzzer_Di();
			
			break;
			
			case 8:
				Buzzer_Di();
				nrf_delay_ms(150);
				Buzzer_Di();
			break;
			
			case 16:  // 卡片到期
			Buzzer_Error();
			
				break;
			case 15:				//密码已过期
				nrf_delay_ms(200);
				Buzzer_Di();
				Buzzer_Di();
				Buzzer_Di();
				
				break;
			
			case 49:			//清空密码声音
			
				Buzzer_Cl();
			
			break;
			
			case 31:			//请按手指
				
				Buzzer_Di();
				nrf_delay_ms(200);
				Buzzer_Di();
			
			break;
			
			case 32:			//请再按手指
				
			Buzzer_Di();
			
			break;
			
			case 42:
			
			Buzzer_Error();
			
			break;
			
			case 20:				//非法操作(防拆）
			
			Buzzer_Error();	
			
			break;
		}
		
		nrf_gpio_pin_clear(Buzz_Data);		// 每次完成后 重新拉低一次  IO 
		return;
	
	}
}




void input_abus_byte(uint8_t byte)  //第二种语音芯片
{
	#ifdef speak_for_IC
	
#ifdef speak1
	uint8_t i;
	uint8_t it;
	
	if(byte==129)
		byte =29;
	it =byte;
//	auart_H;
//	nrf_delay_ms(50);
//////	
	auart_L;
//	nrf_delay_ms (4);
	
	nrf_delay_us(5000);
	
	auart_H;
	
	//nrf_delay_ms(1);
	nrf_delay_us(2000);
	
	for(i =0;i<8;i++)
	{
		if(it&0x80)  //dat 1
		{
			auart_L;
			nrf_delay_us(500);
			auart_H;
			nrf_delay_us(1500);
		}
		else  //dat 0
		{
				auart_L;
				nrf_delay_us(1500);
				auart_H;
				nrf_delay_us(500);
		}
		it=it<<1;
	}
	//auart_L;
	//nrf_delay_us(500);
	auart_H;
	
	#endif
	
	#ifdef speak2
	
		uint8_t i;
	//先复位芯片
	auart_init();
	
	nrf_delay_ms(10);
	auart_data_L;
	
	auart_rst_H;
	
	nrf_delay_us(200);
		
	auart_rst_L;
		
	nrf_delay_us(200);
	
	for(i=0;i<byte;i++)
	{
		auart_data_H;
		nrf_delay_us(350);
		
		auart_data_L;
		nrf_delay_us(350);
	}
	
	
	#endif
	
	#endif
	
	#ifdef speak_for_Been
	
	
	
	
	#endif
	
}

#define speak_busy 30 //定义语音忙碌
uint8_t speaki =0;//语音循环

void input_speak(uint8_t byte)  //连续输入语音
{
	#ifdef speak_for_IC
	
	#ifdef speak1  
		uint8_t i;
	
	auart_init();
	//input_abus_byte(0xF3);
//	nrf_delay_ms(100);

	input_abus_byte(0x00);
	nrf_delay_ms(20);
	
	i =0;
	speaki =0;
	
	nrf_gpio_cfg_input(speak_busy,NRF_GPIO_PIN_PULLUP);
	
	while(1)
	{
		switch(speaki)  //speak 循环
		{
			case 0:
			input_abus_byte(byte);				
			speaki++;			//撤销自动重发
			if(byte==129)
			{
//					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_clear(LEDB1);
//					nrf_gpio_pin_clear(LEDR);
				nrf_gpio_pin_set(LEDR);
					nrf_delay_ms(180);
			}
			else
				nrf_delay_ms(180);
//			if(byte==129)
//			{
//					nrf_gpio_pin_clear(LEDR);
//					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_set(LEDB1);
//				//byte = 29;  //灯只闪烁一次
//			}
				break;
			case 1:
			if(nrf_gpio_pin_read(speak_busy))
				i++;
			else
			{
				if(byte==129)
					{
								nrf_delay_ms(80);
					//			input_abus_byte(0x00);
//								nrf_delay_ms(120);
							//byte = 29;  //灯只闪烁一次
								
//								nrf_gpio_pin_clear(LEDR);
//								nrf_gpio_pin_set(LEDB);
//								nrf_gpio_pin_set(LEDB1);
						nrf_gpio_pin_clear(LEDR);
					}
				return;  //当语音芯片响应时结束循环
			}
				if(i==1)
				speaki =0;  //当语音芯片未响应时，再次发送语音指令
			else if(i==2)
				speaki =0;
			else if(i==3)
				speaki =0;
			else if(i>=4)  //当语音芯片第三次未响应，结束此次发送
			{
				speaki =0;  
					if(byte ==129)
						{
						//		nrf_delay_ms(80);
						//		input_abus_byte(0x00);
//								nrf_delay_ms(120);
							//byte = 29;  //灯只闪烁一次
								
//								nrf_gpio_pin_clear(LEDR);
//								nrf_gpio_pin_set(LEDB);
//								nrf_gpio_pin_set(LEDB1);
							nrf_gpio_pin_clear(LEDR);
						}
				return;
			}
				break;
			default:
				return;
		}
		
	}
//while(nrf_gpio_pin_read(speak_busy));
//	nrf_gpio_cfg_default(speak_busy);
//	nrf_delay_ms(10);
	//auart_sleep();
#endif


#ifdef speak2


switch(byte)  //输入语音地址重新映射
{
	case 29:		//滴
		input_abus_byte(28);
		break;
	
	case 6:   //门已开
		input_abus_byte(7);  //已开锁
		break;
	
	case 9:  //操作成功
		input_abus_byte(10);
			break;
	
	case 8:  //电量低
		input_abus_byte(9); //电量不足，请更换电池
			break;
	
	case 22:  //请刷卡
		input_abus_byte(22);
			break;
	
	case 3:  //绑定成功
		input_abus_byte(4);
			break;
	
	case 10:  //操作失败
		input_abus_byte(11);
			break;
	
	case 17: //已反锁
		input_abus_byte(17); //门已反锁
			break;
	
	case 4: //重置成功
		input_abus_byte(5);  //初始化成功
	break;
	
	case 16:		//卡片到期
		input_abus_byte(16);  //卡片到期
		break;
	
	case 28:  //您再输错密码两次后。。。。
		input_abus_byte(27);
		break;

	case 5:  //密码错误
		input_abus_byte(6);
	break;
	
	case 60: //欢迎回家
		
		input_abus_byte(8);
	break;
	
	default:
		
		input_abus_byte(byte);
		break;
}

#endif

#endif

while(Buzz_Data_T!=0);

if((byte==42)&&(F_print_Enable==1)&&
	(Controllable_Power_State==1)
	)   //提示指纹错误 亮一次红灯
{
	
	PS_LED_Run(1);
	#ifdef speak_for_Been
	if(byte==42)				//当蜂鸣器提示指纹错误时
	Buzzer_Run(byte);
	#endif
	nrf_delay_ms(800);
	
	PS_LED_Run(3);
	
}



#ifdef speak_for_Been 

if(byte!=42)				//不是指纹错误时
	Buzzer_Run(byte);

#endif

#ifdef uart_start

SEGGER_RTT_printf(0,"s:%d\r\n",byte);

#endif

if((byte==29)&&(nrf_gpio_pin_read(LEDR)==0))			//当输入滴 且 LEDR 未置位时
{
	
	nrf_gpio_pin_set(LEDR);
	LEDR_Close_Init();				//开启自动关闭定时器
	
}

if((byte==7)&&(F_print_Enable==1)&&
	(Controllable_Power_State==1)
	)   //指纹头开启时提示已开锁 亮绿灯
{
	PS_LED_Run(2);
	
//	nrf_delay_ms(800);
	
//	PS_LED_Run(3);
}



if(byte==29)
{
	
//	nrf_delay_ms(80);
	
	
//	input_abus_byte(0);
}

	if(byte==42)
		nrf_delay_ms(1200);			//当输入指纹错误时，额外等待1000ms


}










