#include "Devic_Init.h"
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
#include <BLE_Data.h>

//uint8_t BL_VALUE_CLOCK[16]; // ʱ�����ݽ���
//uint8_t CLOCK_VALUE[16]; // ʱ�����ݽ������ػ���
uint8_t dati,datii; //���㻺��
uint8_t TIME_STATE =0; //ʱ����Ч״̬
uint8_t SEC,MIN,HOUR,DAY,MON,YEAR; //ʵʱʱ�� 24H
uint8_t MPR121_ERR; // MPR121 ERROR flag
uint8_t Flash_ERR;  //Flash_ERROR flag
uint8_t flash_test_tab[10];  

/*********************************
������ :Get_Time
���룺 ��
����� ��
���ܣ� ���ⲿʱ��������Чʱ����ȡ�ⲿʱ��
���ⲿʱ��������Чʱ������ȡ��
******************************/
void Get_Time()  // ��ȡ�ⲿʱ��
{
	
		bl_value_read(); // ��ȡʱ��
	
		//ʱ����Ч�����±���ʱ��
		TIME_STATE =true;  //ʱ����Ч
		dati = ((BL_VALUE_READ[BL_SEC]&0xf0)>>4)*10;
		datii = BL_VALUE_READ[BL_SEC]&0x0f;
		SEC = dati+datii;
		
		dati = ((BL_VALUE_READ[BL_MINUTE]&0xf0)>>4)*10;
		datii = BL_VALUE_READ[BL_MINUTE]&0x0f;
		MIN = dati+datii;
		
		dati = ((BL_VALUE_READ[BL_HOUR]&0xf0)>>4)*10;
		datii = BL_VALUE_READ[BL_HOUR]&0x0f;
		HOUR = dati+datii;
		
		dati = ((BL_VALUE_READ[BL_DAY]&0xf0)>>4)*10;
		datii = BL_VALUE_READ[BL_DAY]&0x0f;
		DAY = dati+datii;
		
		dati = ((BL_VALUE_READ[BL_MONTH]&0xf0)>>4)*10; 
		datii = BL_VALUE_READ[BL_MONTH]&0x0f;
		MON = dati+datii;
		
		dati = ((BL_VALUE_READ[BL_YEAR]&0xf0)>>4)*10;
		datii = BL_VALUE_READ[BL_YEAR]&0x0f;
		YEAR = dati+datii;
		if((MON==0)||(YEAR<18)||(YEAR>50)||(MON>12))
			TIME_STATE = false;
		if(TIME_STATE==true)
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Time: %d:%d:%d:%d:%d:%d\r\n",YEAR,MON,DAY,HOUR,MIN,SEC); //�����Ƕʱ�䵽����
			#endif
		}
		else
		{
			#ifdef uart_start //��ʾʱ�Ӵ���
			SEGGER_RTT_printf(0,"Time_ERROR!\r\n");
			#endif
		}
	//}
}


//���ⲿ����


void Update_Time(uint64_t itime) //���µ�ʱ��д���ⲿʱ��
{
	for(uint8_t i =0;i<sizeof(BL_VALUE_WRITE);i++)
	{
		BL_VALUE_WRITE[i] =0;			//��ջ���
		
	}

	BL_VALUE_WRITE[BL_SEC] = itime%100; //��ȡ��
	BL_VALUE_WRITE[BL_MINUTE] = itime/100%100; //��ȡ��
	BL_VALUE_WRITE[BL_HOUR] = itime/10000%100; //��ȡʱ
	BL_VALUE_WRITE[BL_DAY] = itime/1000000%100; //��ȡ����
	BL_VALUE_WRITE[BL_MONTH] = itime/100000000%100;//��ȡ�·�
	BL_VALUE_WRITE[BL_YEAR] = itime/10000000000%100; //��ȡ��
	BL_VALUE_WRITE[BL_WEEK] = CaculateWeekDay(
	BL_VALUE_WRITE[BL_YEAR],BL_VALUE_WRITE[BL_MONTH],
	BL_VALUE_WRITE[BL_DAY]
	);  //����������
	
	BL_VALUE_WRITE[BL_SEC] += BL_VALUE_WRITE[BL_SEC]/10*6;
	BL_VALUE_WRITE[BL_MINUTE] += BL_VALUE_WRITE[BL_MINUTE]/10*6;
	BL_VALUE_WRITE[BL_HOUR] += BL_VALUE_WRITE[BL_HOUR]/10*6;
	BL_VALUE_WRITE[BL_DAY] += BL_VALUE_WRITE[BL_DAY]/10*6;
	BL_VALUE_WRITE[BL_MONTH] += BL_VALUE_WRITE[BL_MONTH]/10*6;
	BL_VALUE_WRITE[BL_YEAR] += BL_VALUE_WRITE[BL_YEAR]/10*6;
		
	

	BL_VALUE_WRITE[BL_COMM2] = 0X28;  //�ر�32k�����24Сʱֵ
	
	bl_value_write();
	

	
}

static uint8_t flash_test[] = {"NRF_51822"};
extern uint32_t Board_Unix;

void  start_init() //�ϵ��Լ� 
{
	
//	uint8_t read_dat =0; // MPR121 ��ȡ���ݻ���
//	uint8_t init_times;  //��ʼ�����Դ���	
	uint8_t Volt_Value;  //�ɼ���Դ��ѹ
//	init_times =0;
//	uint8_t con;
//	uint8_t PassWord[18] = {"111111111111111111"};
//			uint32_t Sector;
	//Update_Time(181207115900);  //����ʱ�� (����)
	//NRF_WDT_Feed();
	Get_Time();  //��ȡʱ������
//	NRF_WDT_Feed();
	
	if(TIME_STATE==true)
	{
		Board_Unix = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);
	}
	I2C_GPIO_init();  //����IO��ʼ��

	Read_Lock_Bon();  //ˢ������״̬
	#ifdef Key_By_MPR121
	MPR121_ERR = true;
	do
	{
		mpr121_init();
		read_dat = read_i2c(I2C_ADD,ELE_CFG);
		init_times++;
		if(init_times>5)
		{
			init_times =0;
			MPR121_ERR = false;
			break;
		}
	}while(read_dat == 0xff); //�жϼĴ���ֵ���Ƿ���Ԥ��һ�¡�
	
	#endif
	


	#ifdef uart_start
	#ifdef Key_By_MPR121
	read_dat = read_i2c(I2C_ADD,0x5C);
	SEGGER_RTT_printf(0,"MSC_0X5C:%d \r\n",read_dat);
	read_dat = read_i2c(I2C_ADD,FIL_CFG);
	SEGGER_RTT_printf(0,"MSC_0X5D:%d \r\n",read_dat);
	read_dat = read_i2c(I2C_ADD,0x5E);
	SEGGER_RTT_printf(0,"MSC_0X5E:%d \r\n",read_dat);
	if(MPR121_ERR == false)   // ��� MPR121 ״̬
		SEGGER_RTT_printf(0,"MPR121_ERROR!\r\n");
	else
		SEGGER_RTT_printf(0,"MPR121_OK!\r\n");
	#endif
	#endif
	

	Spi_Flash_Init();  //����SPI_FLASH ͨѶ
	
	SPIFlash_Erase_Sector(63,15); //�����һ����������һ���������в���
	nrf_delay_ms(100); //��ɾ����ʱ100ms
	SPI_Flash_Busy();
	SpiFlash_Write_Data(flash_test,63,255,sizeof(flash_test));  //�����һ����������һ���������п�д�����
	nrf_delay_ms(10); //ҳд��ȴ�10ms
	SPI_Flash_Busy();
	SpiFlash_Read_Data(flash_test_tab,63,255,sizeof(flash_test_tab));

		if(flash_test_tab[0]=='N'&&flash_test_tab[1]=='R'&& //�ж�д��������Ԥ���Ƿ�һ��
			 flash_test_tab[2]=='F'&&flash_test_tab[3]=='_'&&
			 flash_test_tab[4]=='5'&&flash_test_tab[5]=='1'&&
			 flash_test_tab[6]=='8'&&flash_test_tab[7]=='2'&&
			 flash_test_tab[8]=='2')
			{
				Flash_ERR = false;
			}
			else
				Flash_ERR = true;
				
	#ifdef uart_start
	if(Flash_ERR)
		SEGGER_RTT_printf(0,"Flash_Error!\r\n");
	else
		SEGGER_RTT_printf(0,"Flash_OK!\r\n");
	
	SEGGER_RTT_printf(0,"Init_Ok!\r\n");
	#endif
	
	TSM12MC_Sen_Read();
	
	TSM12_Init();   //��ʼ�� TSM12
	
	TSM12_read_tast();  //��ȡ TSM12 ����

	
	//SPI_FLASH д��ο�
	//SpiFlash_Read_Data(FLASH_Buf,FLASH_BLOCK_NUMBLE ,FLASH_PAGE_NUMBLE ,10); //ҳ��ȡ
  //SpiFlash_Read_Data(����ַ���,���ַ ,ҳ��ַ ,10); //ҳ��ȡ (ҳ��ַ0-256)
	//	
  //	SpiFlash_Write_Data(�����ַ���,���ַ,ҳ��ַ,����);//ҳд�� (ҳ��ַ0-256)
	//ble_nus_string_send(&m_nus,FLASH_Buf,sizeof(FLASH_Buf)); //5�뷢��һ�����ݸ�����

//	SPIFlash_Erase_Sector(0,0);  //Ԥ�����һ������0 ���������״̬�����ԣ�

//	SPI_Flash_Busy();
	Devic_static_read();  //(��ȡ�豸��״̬)
	if(Devic_Static==false)
	{
		CL_FlashWord_Num();  //δ�󶨵��豸�������豸Ŀ¼
	}
//	CL_FlashWord_Num();
//	CL_Flash_OPEN_DOOR_NOTE();  //������ż�¼ only Clean (test)
//	SPI_Flash_Busy();
//	for(uint32_t i =0;i<50;i++)
//	{
//	ADD_Op_Door_Note(01,02,(i*86400));
//	NRF_WDT_Feed();
//	}
	
//	Write_Devic_Num();  //д���豸����Ϣ(test)
//		SPI_Flash_Busy();
//	Devic_static_read();  //(��ȡ�豸��״̬)
//	Ev_Cat_Dat(OpenDoorNote_Num_Code,5);
//	CL_FlashWord_Num();  //���Flash Ŀ¼��Ϣ (test)
////	
//	CL_Flash_PassWord(); //���Flash ������Ϣ only Clean (test)
//CL_Flash_OPEN_DOOR_NOTE();  //������ż�¼ only Clean (test)
//	for(uint8_t it =0;it<18;it++) PassWord[it] =0;
//	PassWord[0] = 0x22;
//	PassWord[5] =0xf0;
//	PassWord[6] =0xf0;
//	PassWord[7] =0xf0;
//	PassWord[8] =0x22;
//	
//	for(uint8_t i=0;i<200;i++)
//	{
//		if(i <10)PassWord[0] = 0x22;
//		else PassWord[0] = 0x12;
//		PassWord[9] =i;
//		PassWord[10] =i;
//		PassWord[11] =i;
//		PassWord[12] =i;
//		PassWord[13] =i;
//		PassWord[14] =i;
//		
//		Add_Flash_PassWord(PassWord);
//		NRF_WDT_Feed();
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"i:%d\r\n",i);
//		#endif
//	}
//	Del_SN_PassWord(0x80);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0x89);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0XC6);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0XBF);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0x66);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0x64);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0x60);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0x15);
//			NRF_WDT_Feed();
//	Del_SN_PassWord(0x18);
//		NRF_WDT_Feed();
//	Del_SN_PassWord(0x51);
//		NRF_WDT_Feed();
//	Del_SN_PassWord(0x8c);
//		NRF_WDT_Feed();
//	Del_SN_PassWord(0x90);
//		NRF_WDT_Feed();
//	Del_SN_PassWord(0x01);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x02);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x03);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x04);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x05);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x06);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x07);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x08);
//	NRF_WDT_Feed();
//	Del_SN_PassWord(0x09);
//	NRF_WDT_Feed();
	
	
//	for(int32_t i =130;i>70;i--)
//	{
//		Del_SN_PassWord(i);
//			NRF_WDT_Feed();
//	}
//	Add_Flash_PassWord(PassWord);
//	for(uint8_t i =0;i<11;i++)
//	{
//	for(uint8_t it =0;it<18;it++) PassWord[it] =i;
//	Del_Flash_PassWord(PassWord);
//	}
//	for(uint8_t i=130;i>1;i--)
//	{
//		PassWord[9] =i;
//		PassWord[10] =i;
//		PassWord[11] =i;
//		PassWord[12] =i;
//		PassWord[13] =i;
//		PassWord[14] =i;
//		
//		Del_Flash_PassWord(PassWord);
//		NRF_WDT_Feed();
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"i:%d\r\n",i);
//		#endif
//	}
//Del_SN_PassWord(111);
//Del_User_PassWord();
//Del_Admin_PassWord();

//	for(uint32_t i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0x01;
//	Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1);  //�Ȳ�������
//	SPIFlash_Erase_Sector(Block,Sector);
//		SPI_Flash_Busy();	
//	Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1);
//	SPIFlash_Write_Tab(tab_dat,Block,Page,2000); //д������������
//	SPI_Flash_Busy();	
//		read_Soctor(1);
//		NRF_WDT_Feed();
//		read_Soctor(Open_Note_Secotr1);
//		NRF_WDT_Feed();
//		read_Soctor(Open_Note_Secotr2);
//		NRF_WDT_Feed();
//		read_Soctor(Open_Note_Secotr3);
//		NRF_WDT_Feed();
//		read_Soctor(Open_Note_Secotr4);
//		NRF_WDT_Feed();
		
//		read_Soctor(3);
//		NRF_WDT_Feed();
//		read_Soctor(4);
//		NRF_WDT_Feed();

		auart_init();						//��ʼ������gpio
	//input_abus_byte(0XEd);  //������������
	LED_BR_Init();  				//��ʼ��LED
//	input_abus_byte(11);
//��־������ �����һ��
	Lock_State =1;
	Lock_Close();	       //�ر�����
	adc_init(2,adc,0);  //������ѹ�ɼ� 
	nrf_delay_ms(10);
	Volt_Value = Batt_Value();

	if(Volt_Value<10) //����������20%ʱ��ʾ������
	{
		nrf_delay_ms(500);
		input_speak(0);
		input_speak(8);
	}
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();
	
//	if(MPR121_ERR==true) //����������ʱ����ʼ��ȡ����ֵ
//	{
//		mpr121_Run_I();
//		mpr121_init_I();  //���³�ʼ�����̽���͹���ģʽ
//		mpr121_init();		
	TSM12_Init();
	//}
}

/*

*/

//�ж��豸�Ƿ��Ѱ� �Ե�һ�����������ж��豸�Ƿ��Ѱ�
/******************************************************

���������ݣ�

���ֽ��˺ű��+24�ֽڹ㲥����ASIIC��+

���ֽ��豸У����+���ֽ�����У����+2�ֽ�¥����

******************************************************/

uint8_t Devic_Static;
uint8_t Devic_Num[Devic_PAGE_Len];    //�豸�󶨺�
//�󶨣� true δ�󶨣� false
void Devic_static_read()
{
  uint8_t i;
	uint8_t FLASH_tab[Devic_PAGE_Len];
		
	SpiFlash_Read_Data (FLASH_tab,Devic_Num_BLOCK,Devic_Num_PAGE,Devic_PAGE_Len);  //��ȡ���������ж�

	//0�������ڱ���󶨺�
	if(FLASH_tab[0] == 0x55&&FLASH_tab[1] == 0xAA) //�ж���ʼ�ֽ�
	{
		for(i =0;i<sizeof(Devic_Num);i++)
			Devic_Num[i] =0;
			
    for(i =0;i<(Devic_PAGE_Len-2);i++) //��ȡ������ (ȥ������ͷ)
      {
        Devic_Num[i] = FLASH_tab[i+2];
      }
			Devic_Static = true;  //����״̬Ϊ�Ѱ�
			#ifdef uart_start
				
			SEGGER_RTT_printf (0,"Devic_static true!\r\n");
			SEGGER_RTT_printf (0,"Devic_Num:\r\n");
			
			for(i =0;i<sizeof(Devic_Num);i++)	
			{
				
				SEGGER_RTT_printf(0,"%x,",Devic_Num[i]);
				if((i%10==0)&&(i!=0))
				{
					SEGGER_RTT_printf(0,"\r\n");
				}
			}
			SEGGER_RTT_printf(0,"\r\n");
			
		#endif
	}
	else
  {
      Devic_Static = false;  //����״̬Ϊδ��
			
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Devic_Static false!\r\n");
		
		#endif
  }
}

//��ȡ��Ȩ״̬�󣬵��豸״̬Ϊδ�󶨣���#�ż���ֱ�������������

////�������ƴ���ʹ��ʧ��
//void Uart_Start() //��������
//{
//	NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled 
//	<< UART_ENABLE_ENABLE_Pos);
//	NRF_UART0->TASKS_STARTRX =1;
//	NRF_UART0->TASKS_STARTTX =1;
//}

//void Uart_Stop() //�رմ���
//{
//	NRF_UART0-> TASKS_STOPTX = 1;
//	NRF_UART0-> TASKS_STOPRX = 1;
//	NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Disabled<<
//	UART_ENABLE_ENABLE_Pos);
//}
