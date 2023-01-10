#include <KEY_Run.h>




/************************************
键盘功能联动

**************************************/

//APP_TIMER_DEF (Moto_Close_Timer);  //用于开关锁体控制的电机 

#define LOCK_OPEN  7// 门锁开锁反馈GPIO
#define LOCK_Close_Time 90  // 自动关锁时间 n/30 秒
uint8_t Lock_State =0;  //锁状态  1即开，0即关
uint32_t Lock_N;
uint8_t LO =0;  //开锁体后，开启标志

void LED_BR_Init() //初始化LEDgpio
{
	nrf_gpio_cfg_output(LEDB);
//	nrf_gpio_cfg_output(LEDB1);
	nrf_gpio_cfg_output(LEDR);
	
	//nrf_gpio_pin_set(LEDB);
}

uint8_t Double_Key; //反锁标志位
uint8_t Turn_Key; //拧钥匙标志位
uint8_t Latch_Bolt; //斜舌标志位

void Read_Lock_Bon()  //读取锁状态
{
	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入
	//nrf_gpio_cfg_input(6,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入
	nrf_gpio_cfg_input(7,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入
	
//	nrf_gpio_pin_read(5); //反锁
//	nrf_gpio_pin_read(6); //拧钥匙
//	nrf_gpio_pin_read(7); //斜舌
	if(nrf_gpio_pin_read(5)==0)
		Double_Key =1;
	else Double_Key =0;
	
	if(nrf_gpio_pin_read(6)==0)
		Turn_Key =1;
	else Turn_Key =0;
	
	if(nrf_gpio_pin_read(7)==0)
		Latch_Bolt =1;
	else Latch_Bolt =0;
	
	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);  //配置GPIO为输入 平时反锁位置长期下拉 
//	nrf_gpio_cfg_input(6,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入
//	nrf_gpio_cfg_input(7,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入
	
}

void LOCK_OPEN_Init()
{
	
	nrf_gpio_cfg_input(LOCK_OPEN,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入

}

static void Moto_Timer_Hander(void *params)
{
	
	if(Lock_State)
	{
		if(nrf_gpio_pin_read(LOCK_OPEN)==0) //当锁体开启过后 标志置1，
		{
			LO =1;
		}
		
		if((LO==1)&&(nrf_gpio_pin_read(LOCK_OPEN)==1)) //锁体回位，恢复关闭状态
		{
			Lock_N =LOCK_Close_Time;
			//input_speak(29);
		}  //直接满足条件
		
		Lock_N ++;
		if(Lock_N>=LOCK_Close_Time)
		{
			
			Lock_N =0;
			LO =0;
			Lock_Close();
			app_timer_stop(Moto_Close_Timer);
		}
	}
	else
	{
		app_timer_stop(Moto_Close_Timer);
		Lock_N =0;
		LO =0;
	}
}


void Moto_Timer_Init()  //初始化电机定时器
{
	  uint32_t err_code;
		err_code =
					app_timer_create (&Moto_Close_Timer,APP_TIMER_MODE_REPEATED,Moto_Timer_Hander);
	APP_ERROR_CHECK(err_code);
	err_code =	app_timer_start( Moto_Close_Timer ,APP_TIMER_TICKS(50,APP_TIMER_PRESCALER),NULL );			//注册新的软件定时器任务
	APP_ERROR_CHECK(err_code);
}

