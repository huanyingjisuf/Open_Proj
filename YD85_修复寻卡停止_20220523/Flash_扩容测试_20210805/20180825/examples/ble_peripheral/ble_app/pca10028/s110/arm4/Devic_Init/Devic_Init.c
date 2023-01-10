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

//uint8_t BL_VALUE_CLOCK[16]; // 时钟内容解析
//uint8_t CLOCK_VALUE[16]; // 时钟内容解析本地缓存
uint8_t dati,datii; //运算缓冲
uint8_t TIME_STATE =0; //时间有效状态
uint8_t SEC,MIN,HOUR,DAY,MON,YEAR; //实时时间 24H
uint8_t MPR121_ERR; // MPR121 ERROR flag
uint8_t Flash_ERR;  //Flash_ERROR flag
uint8_t flash_test_tab[10];  

/*********************************
函数名 :Get_Time
输入： 无
输出： 无
功能： 当外部时钟数据有效时，获取外部时钟
当外部时钟数据无效时，不获取。
******************************/
void Get_Time()  // 获取外部时钟
{
	
		bl_value_read(); // 获取时间
	
		//时间有效，更新本地时间
		TIME_STATE =true;  //时钟有效
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
			SEGGER_RTT_printf(0,"Time: %d:%d:%d:%d:%d:%d\r\n",YEAR,MON,DAY,HOUR,MIN,SEC); //输出内嵌时间到串口
			#endif
		}
		else
		{
			#ifdef uart_start //提示时钟错误
			SEGGER_RTT_printf(0,"Time_ERROR!\r\n");
			#endif
		}
	//}
}


//将外部数据


void Update_Time(uint64_t itime) //将新的时间写入外部时钟
{
	for(uint8_t i =0;i<sizeof(BL_VALUE_WRITE);i++)
	{
		BL_VALUE_WRITE[i] =0;			//清空缓存
		
	}

	BL_VALUE_WRITE[BL_SEC] = itime%100; //提取秒
	BL_VALUE_WRITE[BL_MINUTE] = itime/100%100; //提取分
	BL_VALUE_WRITE[BL_HOUR] = itime/10000%100; //提取时
	BL_VALUE_WRITE[BL_DAY] = itime/1000000%100; //提取日期
	BL_VALUE_WRITE[BL_MONTH] = itime/100000000%100;//提取月份
	BL_VALUE_WRITE[BL_YEAR] = itime/10000000000%100; //提取年
	BL_VALUE_WRITE[BL_WEEK] = CaculateWeekDay(
	BL_VALUE_WRITE[BL_YEAR],BL_VALUE_WRITE[BL_MONTH],
	BL_VALUE_WRITE[BL_DAY]
	);  //运算获得星期
	
	BL_VALUE_WRITE[BL_SEC] += BL_VALUE_WRITE[BL_SEC]/10*6;
	BL_VALUE_WRITE[BL_MINUTE] += BL_VALUE_WRITE[BL_MINUTE]/10*6;
	BL_VALUE_WRITE[BL_HOUR] += BL_VALUE_WRITE[BL_HOUR]/10*6;
	BL_VALUE_WRITE[BL_DAY] += BL_VALUE_WRITE[BL_DAY]/10*6;
	BL_VALUE_WRITE[BL_MONTH] += BL_VALUE_WRITE[BL_MONTH]/10*6;
	BL_VALUE_WRITE[BL_YEAR] += BL_VALUE_WRITE[BL_YEAR]/10*6;
		
	

	BL_VALUE_WRITE[BL_COMM2] = 0X28;  //关闭32k输出，24小时值
	
	bl_value_write();
	

	
}

static uint8_t flash_test[] = {"NRF_51822"};
extern uint32_t Board_Unix;

void  start_init() //上电自检 
{
	
//	uint8_t read_dat =0; // MPR121 读取数据缓冲
//	uint8_t init_times;  //初始化重试次数	
	uint8_t Volt_Value;  //采集电源电压
//	init_times =0;
//	uint8_t con;
//	uint8_t PassWord[18] = {"111111111111111111"};
//			uint32_t Sector;
	//Update_Time(181207115900);  //载入时间 (测试)
	//NRF_WDT_Feed();
	Get_Time();  //获取时钟数据
//	NRF_WDT_Feed();
	
	if(TIME_STATE==true)
	{
		Board_Unix = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);
	}
	I2C_GPIO_init();  //键盘IO初始化

	Read_Lock_Bon();  //刷新锁体状态
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
	}while(read_dat == 0xff); //判断寄存器值，是否与预设一致。
	
	#endif
	


	#ifdef uart_start
	#ifdef Key_By_MPR121
	read_dat = read_i2c(I2C_ADD,0x5C);
	SEGGER_RTT_printf(0,"MSC_0X5C:%d \r\n",read_dat);
	read_dat = read_i2c(I2C_ADD,FIL_CFG);
	SEGGER_RTT_printf(0,"MSC_0X5D:%d \r\n",read_dat);
	read_dat = read_i2c(I2C_ADD,0x5E);
	SEGGER_RTT_printf(0,"MSC_0X5E:%d \r\n",read_dat);
	if(MPR121_ERR == false)   // 输出 MPR121 状态
		SEGGER_RTT_printf(0,"MPR121_ERROR!\r\n");
	else
		SEGGER_RTT_printf(0,"MPR121_OK!\r\n");
	#endif
	#endif
	

	Spi_Flash_Init();  //开启SPI_FLASH 通讯
	
	SPIFlash_Erase_Sector(63,15); //对最后一个区块的最后一个扇区进行擦除
	nrf_delay_ms(100); //扇删除耗时100ms
	SPI_Flash_Busy();
	SpiFlash_Write_Data(flash_test,63,255,sizeof(flash_test));  //对最后一个区块的最后一个扇区进行块写入测试
	nrf_delay_ms(10); //页写入等待10ms
	SPI_Flash_Busy();
	SpiFlash_Read_Data(flash_test_tab,63,255,sizeof(flash_test_tab));

		if(flash_test_tab[0]=='N'&&flash_test_tab[1]=='R'&& //判断写入数据与预设是否一致
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
	
	TSM12_Init();   //初始化 TSM12
	
	TSM12_read_tast();  //读取 TSM12 测试

	
	//SPI_FLASH 写入参考
	//SpiFlash_Read_Data(FLASH_Buf,FLASH_BLOCK_NUMBLE ,FLASH_PAGE_NUMBLE ,10); //页读取
  //SpiFlash_Read_Data(输出字符表,块地址 ,页地址 ,10); //页读取 (页地址0-256)
	//	
  //	SpiFlash_Write_Data(输入字符表,块地址,页地址,长度);//页写入 (页地址0-256)
	//ble_nus_string_send(&m_nus,FLASH_Buf,sizeof(FLASH_Buf)); //5秒发送一次数据给主机

//	SPIFlash_Erase_Sector(0,0);  //预先清空一次扇区0 以清空密码状态（测试）

//	SPI_Flash_Busy();
	Devic_static_read();  //(获取设备绑定状态)
	if(Devic_Static==false)
	{
		CL_FlashWord_Num();  //未绑定的设备，重置设备目录
	}
//	CL_FlashWord_Num();
//	CL_Flash_OPEN_DOOR_NOTE();  //清除开门记录 only Clean (test)
//	SPI_Flash_Busy();
//	for(uint32_t i =0;i<50;i++)
//	{
//	ADD_Op_Door_Note(01,02,(i*86400));
//	NRF_WDT_Feed();
//	}
	
//	Write_Devic_Num();  //写入设备绑定信息(test)
//		SPI_Flash_Busy();
//	Devic_static_read();  //(获取设备绑定状态)
//	Ev_Cat_Dat(OpenDoorNote_Num_Code,5);
//	CL_FlashWord_Num();  //清除Flash 目录信息 (test)
////	
//	CL_Flash_PassWord(); //清除Flash 密码信息 only Clean (test)
//CL_Flash_OPEN_DOOR_NOTE();  //清除开门记录 only Clean (test)
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
//	Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1);  //先擦除数据
//	SPIFlash_Erase_Sector(Block,Sector);
//		SPI_Flash_Busy();	
//	Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1);
//	SPIFlash_Write_Tab(tab_dat,Block,Page,2000); //写入新数据数据
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

		auart_init();						//初始化语音gpio
	//input_abus_byte(0XEd);  //调节语音音量
	LED_BR_Init();  				//初始化LED
//	input_abus_byte(11);
//标志门锁定 后关锁一次
	Lock_State =1;
	Lock_Close();	       //关闭锁体
	adc_init(2,adc,0);  //开启电压采集 
	nrf_delay_ms(10);
	Volt_Value = Batt_Value();

	if(Volt_Value<10) //当电量低于20%时提示电量低
	{
		nrf_delay_ms(500);
		input_speak(0);
		input_speak(8);
	}
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
	
//	if(MPR121_ERR==true) //当键盘正常时，开始读取基线值
//	{
//		mpr121_Run_I();
//		mpr121_init_I();  //重新初始化键盘进入低功耗模式
//		mpr121_init();		
	TSM12_Init();
	//}
}

/*

*/

//判断设备是否已绑定 以第一个扇区内容判断设备是否已绑定
/******************************************************

绑定数据内容：

四字节账号编号+24字节广播名称ASIIC码+

四字节设备校验码+四字节主体校验码+2字节楼层编号

******************************************************/

uint8_t Devic_Static;
uint8_t Devic_Num[Devic_PAGE_Len];    //设备绑定号
//绑定： true 未绑定： false
void Devic_static_read()
{
  uint8_t i;
	uint8_t FLASH_tab[Devic_PAGE_Len];
		
	SpiFlash_Read_Data (FLASH_tab,Devic_Num_BLOCK,Devic_Num_PAGE,Devic_PAGE_Len);  //获取数据用于判断

	//0扇区用于保存绑定号
	if(FLASH_tab[0] == 0x55&&FLASH_tab[1] == 0xAA) //判断起始字节
	{
		for(i =0;i<sizeof(Devic_Num);i++)
			Devic_Num[i] =0;
			
    for(i =0;i<(Devic_PAGE_Len-2);i++) //获取绑定数据 (去除数据头)
      {
        Devic_Num[i] = FLASH_tab[i+2];
      }
			Devic_Static = true;  //设置状态为已绑定
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
      Devic_Static = false;  //设置状态为未绑定
			
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Devic_Static false!\r\n");
		
		#endif
  }
}

//获取授权状态后，当设备状态为未绑定，按#号键可直接驱动电机开门

////独立控制串口使能失能
//void Uart_Start() //开启串口
//{
//	NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled 
//	<< UART_ENABLE_ENABLE_Pos);
//	NRF_UART0->TASKS_STARTRX =1;
//	NRF_UART0->TASKS_STARTTX =1;
//}

//void Uart_Stop() //关闭串口
//{
//	NRF_UART0-> TASKS_STOPTX = 1;
//	NRF_UART0-> TASKS_STOPRX = 1;
//	NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Disabled<<
//	UART_ENABLE_ENABLE_Pos);
//}
