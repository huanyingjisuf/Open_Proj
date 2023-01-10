#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf51.h"
//#include "flash.h"
#include "nrf_delay.h"

#include <BL5372.h>
#include <Devic_Init.h>


void BL_I2C_GPIO_init()
{	
	
// 	nrf_gpio_pin_pull_t NRF_GPIO_PIN_PULL_T; 
	
	nrf_gpio_cfg_output(BLI2C_SDA);
	nrf_gpio_cfg_output(BLI2C_SCL);
//	nrf_gpio_cfg_input(BL_INTA,NRF_GPIO_PIN_PULLUP);
//	nrf_gpio_cfg_input(BL_INTB,NRF_GPIO_PIN_PULLUP);
	
	BLI2C_SDA_H;
	BLI2C_SCL_H;
}

void BL_I2C_GPIO_SLEEP()
{	
 //	nrf_gpio_pin_pull_t NRF_GPIO_PIN_PULL_T; 
	nrf_gpio_cfg_default(BLI2C_SDA);
	nrf_gpio_cfg_default(BLI2C_SCL);
//	nrf_gpio_cfg_default(BL_INTA);
//	nrf_gpio_cfg_default(BL_INTB);
	
	BLI2C_SDA_H;
	BLI2C_SCL_H;
}


void BLI2C_delay(void)
{
	nrf_delay_us(5);
}


void BLI2C_START(void)
{
	BLI2C_SDA_H;
	BLI2C_SCL_H;
	BLI2C_delay();
	BLI2C_SDA_L;
	BLI2C_delay();
	BLI2C_SCL_L;
	BLI2C_delay();
}

void BLI2C_STOP(void)
{
	BLI2C_SDA_L;
	BLI2C_SCL_L;
	BLI2C_delay();
	BLI2C_SCL_H;
	BLI2C_delay();
	BLI2C_SDA_H;
}

void BL_I2C_WAIT_ACK()
{
	uint8_t i;
	BLI2C_SCL_H;
	BLI2C_delay();
	while((nrf_gpio_pin_read(BLI2C_SDA)==1)&&(i<255))
		i++;
	BLI2C_SCL_L;
	BLI2C_delay();
}

void BL_I2C_SEND_ACK(uint8_t ack)
{

	if(ack==1)
		BLI2C_SDA_L;
	else 
		BLI2C_SDA_H;
	
	BLI2C_SCL_H;
	BLI2C_delay();
	
	BLI2C_SCL_L;
	BLI2C_delay();
	
	BLI2C_SDA_H;
	BLI2C_delay();

}

void BL_I2C_WRITE_BYTE(uint8_t dat)
{
	uint8_t i,temp;
	temp = dat;
	for (i =0;i<8;i++)
	{
		BLI2C_SCL_L;
		BLI2C_delay();
		if(temp&0x80)
			BLI2C_SDA_H;
		else
			BLI2C_SDA_L;
		
		temp <<= 1;
		BLI2C_delay();
		BLI2C_SCL_H;
	}
	BLI2C_SCL_L;
	BLI2C_delay();
	BLI2C_SDA_H;
	BLI2C_delay();
}

uint8_t BLI2C_READ_BYTE()
{
	uint8_t i,dat;
	BLI2C_SCL_L;
	BLI2C_delay();
	BLI2C_SDA_H;
	BLI2C_delay();
	for(i =0;i<8;i++)
	{
		BLI2C_SCL_H;
		BLI2C_delay();
		dat<<=1;
		dat |= nrf_gpio_pin_read(BLI2C_SDA);
		BLI2C_SCL_L;
		BLI2C_delay();
	}
	BLI2C_SDA_H;
	BLI2C_delay();
	
	return dat;
}

void BL_write_i2c(uint8_t device,uint8_t addr,uint8_t dat)
{
	BLI2C_START();
  BL_I2C_WRITE_BYTE(device);
	
	BL_I2C_WAIT_ACK();
	BL_I2C_WRITE_BYTE(addr);
	
	BL_I2C_WAIT_ACK();
	BL_I2C_WRITE_BYTE(dat);
	
	BL_I2C_WAIT_ACK();
	BLI2C_STOP();
}

uint8_t BL_read_i2c (uint8_t device,uint8_t addr)
{
	uint8_t dat;
	BLI2C_START();
	
	BL_I2C_WRITE_BYTE (device);
	BL_I2C_WAIT_ACK ();
	
	BL_I2C_WRITE_BYTE (addr);
	BL_I2C_WAIT_ACK();
	
	BLI2C_START();
	BL_I2C_WRITE_BYTE(device+0x01);
	BL_I2C_WAIT_ACK();
	
	dat = BLI2C_READ_BYTE();
	BL_I2C_SEND_ACK(0);
	BLI2C_STOP();
	
	return dat;
}

uint8_t CaculateWeekDay(int y,int m, int d)  //用年，月，日，算星期
{	
    if(m==1||m==2) 
		{
        m+=12;
        y--;
    }
    int iWeek; 
		iWeek=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
		
		#ifdef uart_start
		
		switch (iWeek)
		{
			case 0 : 
				SEGGER_RTT_printf (0,"Today is the Monday\n");
			break;
			case 1 : 
				SEGGER_RTT_printf (0,"Today is the Tuesday\n");
			break;
			case 2 : 
				SEGGER_RTT_printf (0,"Today is the Wednesday\n");
			break;
			case 3 : 
				SEGGER_RTT_printf (0,"Today is the Thurday\n");
			break;
			case 4 : 
				SEGGER_RTT_printf (0,"Today is the Firday\n");
			break;
			case 5 : 
				SEGGER_RTT_printf (0,"Today is the Saturday\n");
			break;
			case 6 : 
				SEGGER_RTT_printf (0,"Today is the Sunday\n");
			break;
		}
		#endif
		iWeek+=1;
		if(iWeek>6)iWeek =0;
		
		return iWeek;
} 


uint8_t BL_VALUE_READ[16];
uint8_t BL_VALUE_WRITE[16];

void bl_value_read() //获取BL时钟内数据
{
		BL_I2C_GPIO_init ();
	
	BL_VALUE_READ[BL_SEC] = BL_read_i2c (BL_I2C_ADD,REG_SEC);
	BL_VALUE_READ[BL_MINUTE] = BL_read_i2c (BL_I2C_ADD,REG_MINUTE);
	BL_VALUE_READ[BL_HOUR] = BL_read_i2c (BL_I2C_ADD,REG_HOUR);
	BL_VALUE_READ[BL_WEEK] = BL_read_i2c (BL_I2C_ADD,REG_WEEK);
	BL_VALUE_READ[BL_DAY] = BL_read_i2c (BL_I2C_ADD,REG_DAY);
	BL_VALUE_READ[BL_MONTH] = BL_read_i2c (BL_I2C_ADD,REG_MONTH);
	BL_VALUE_READ[BL_YEAR] = BL_read_i2c (BL_I2C_ADD,REG_YEAR);
	BL_VALUE_READ[BL_XSL] = BL_read_i2c (BL_I2C_ADD,REG_XSL);
	BL_VALUE_READ[BL_ALAA_MINUS] = BL_read_i2c (BL_I2C_ADD,REG_ALAA_MINUS);
	BL_VALUE_READ[BL_ALAA_HOUR] = BL_read_i2c (BL_I2C_ADD,REG_ALAA_HOUR);
	BL_VALUE_READ[BL_ALAA_DAY] = BL_read_i2c (BL_I2C_ADD,REG_ALAA_DAY);
	BL_VALUE_READ[BL_ALAB_MINUS] = BL_read_i2c (BL_I2C_ADD,REG_ALAB_MINUS);
	BL_VALUE_READ[BL_ALAB_HOUR] = BL_read_i2c (BL_I2C_ADD,REG_ALAB_HOUR);
	BL_VALUE_READ[BL_ALAB_DAY] = BL_read_i2c (BL_I2C_ADD,REG_ALAB_DAY);
	BL_VALUE_READ[BL_COMM1] = BL_read_i2c (BL_I2C_ADD,REG_COMM1);
	BL_VALUE_READ[BL_COMM2] = BL_read_i2c (BL_I2C_ADD,REG_COMM2);
	
		BL_I2C_GPIO_SLEEP();
}

void BL_write_i2c(uint8_t device,uint8_t addr,uint8_t dat);
void bl_value_write() // bl时钟寄存器写入
{
	BL_I2C_GPIO_init ();
	
	BL_write_i2c(BL_I2C_ADD,REG_SEC,BL_VALUE_WRITE[BL_SEC]);
	BL_write_i2c(BL_I2C_ADD,REG_MINUTE,BL_VALUE_WRITE[BL_MINUTE]);
	BL_write_i2c(BL_I2C_ADD,REG_HOUR,BL_VALUE_WRITE[BL_HOUR]);
	BL_write_i2c(BL_I2C_ADD,REG_WEEK,BL_VALUE_WRITE[BL_WEEK]);
	BL_write_i2c(BL_I2C_ADD,REG_DAY,BL_VALUE_WRITE[BL_DAY]);
	BL_write_i2c(BL_I2C_ADD,REG_MONTH,BL_VALUE_WRITE[BL_MONTH]);
	BL_write_i2c(BL_I2C_ADD,REG_YEAR,BL_VALUE_WRITE[BL_YEAR]);
	BL_write_i2c(BL_I2C_ADD,REG_XSL,BL_VALUE_WRITE[BL_XSL]);
	BL_write_i2c(BL_I2C_ADD,REG_ALAA_MINUS,BL_VALUE_WRITE[BL_ALAA_MINUS]);
	BL_write_i2c(BL_I2C_ADD,REG_ALAA_HOUR,BL_VALUE_WRITE[BL_ALAA_HOUR]);
	BL_write_i2c(BL_I2C_ADD,REG_ALAA_DAY,BL_VALUE_WRITE[BL_ALAA_DAY]);
	BL_write_i2c(BL_I2C_ADD,REG_ALAB_MINUS,BL_VALUE_WRITE[BL_ALAB_MINUS]);
	BL_write_i2c(BL_I2C_ADD,REG_ALAB_HOUR,BL_VALUE_WRITE[BL_ALAB_HOUR]);
	BL_write_i2c(BL_I2C_ADD,REG_ALAB_DAY,BL_VALUE_WRITE[BL_ALAB_DAY]);
	BL_write_i2c(BL_I2C_ADD,REG_COMM1,BL_VALUE_WRITE[BL_COMM1]);
	BL_write_i2c(BL_I2C_ADD,REG_COMM2,BL_VALUE_WRITE[BL_COMM2]);
	
	  
	BL_I2C_GPIO_SLEEP();	
}
