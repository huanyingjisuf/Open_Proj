#include <KEY_Run.h>




/************************************
���̹�������

**************************************/

//APP_TIMER_DEF (Moto_Close_Timer);  //���ڿ���������Ƶĵ�� 

#define LOCK_OPEN  7// ������������GPIO
#define LOCK_Close_Time 90  // �Զ�����ʱ�� n/30 ��
uint8_t Lock_State =0;  //��״̬  1������0����
uint32_t Lock_N;
uint8_t LO =0;  //������󣬿�����־

void LED_BR_Init() //��ʼ��LEDgpio
{
	nrf_gpio_cfg_output(LEDB);
//	nrf_gpio_cfg_output(LEDB1);
	nrf_gpio_cfg_output(LEDR);
	
	//nrf_gpio_pin_set(LEDB);
}

uint8_t Double_Key; //������־λ
uint8_t Turn_Key; //šԿ�ױ�־λ
uint8_t Latch_Bolt; //б���־λ

void Read_Lock_Bon()  //��ȡ��״̬
{
	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����
	//nrf_gpio_cfg_input(6,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����
	nrf_gpio_cfg_input(7,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����
	
//	nrf_gpio_pin_read(5); //����
//	nrf_gpio_pin_read(6); //šԿ��
//	nrf_gpio_pin_read(7); //б��
	if(nrf_gpio_pin_read(5)==0)
		Double_Key =1;
	else Double_Key =0;
	
	if(nrf_gpio_pin_read(6)==0)
		Turn_Key =1;
	else Turn_Key =0;
	
	if(nrf_gpio_pin_read(7)==0)
		Latch_Bolt =1;
	else Latch_Bolt =0;
	
	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);  //����GPIOΪ���� ƽʱ����λ�ó������� 
//	nrf_gpio_cfg_input(6,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����
//	nrf_gpio_cfg_input(7,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����
	
}

void LOCK_OPEN_Init()
{
	
	nrf_gpio_cfg_input(LOCK_OPEN,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����

}

static void Moto_Timer_Hander(void *params)
{
	
	if(Lock_State)
	{
		if(nrf_gpio_pin_read(LOCK_OPEN)==0) //�����忪������ ��־��1��
		{
			LO =1;
		}
		
		if((LO==1)&&(nrf_gpio_pin_read(LOCK_OPEN)==1)) //�����λ���ָ��ر�״̬
		{
			Lock_N =LOCK_Close_Time;
			//input_speak(29);
		}  //ֱ����������
		
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


void Moto_Timer_Init()  //��ʼ�������ʱ��
{
	  uint32_t err_code;
		err_code =
					app_timer_create (&Moto_Close_Timer,APP_TIMER_MODE_REPEATED,Moto_Timer_Hander);
	APP_ERROR_CHECK(err_code);
	err_code =	app_timer_start( Moto_Close_Timer ,APP_TIMER_TICKS(50,APP_TIMER_PRESCALER),NULL );			//ע���µ������ʱ������
	APP_ERROR_CHECK(err_code);
}

