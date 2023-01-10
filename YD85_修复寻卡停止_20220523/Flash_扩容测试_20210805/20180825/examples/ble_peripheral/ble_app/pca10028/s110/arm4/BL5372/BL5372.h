#ifndef _BL5372_H_
#define _BL5372_H_

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "bsp_btn_ble.h"
#include "nrf_delay.h"
#include "stdio.h"

/***********************************************************************
BL5372 .h soft I2C

*******************************************************f***************/

#define REG_SEC 					0X00  //��Ĵ��� BCD
#define REG_MINUTE 				0X10  //�ּĴ��� BCD
#define REG_HOUR 					0X20  //ʱ�Ĵ��� BCD
#define REG_WEEK 					0X30  //�ܼĴ��� BCD
#define REG_DAY 					0X40  //��Ĵ��� BCD
#define REG_MONTH 				0X50  //�¼Ĵ��� BCD
#define REG_YEAR 					0X60  //��Ĵ��� BCD
#define REG_XSL 					0X70  //ʱ�����
#define REG_ALAA_MINUS 		0X80  //�ֶ�ʱA
#define REG_ALAA_HOUR 		0X90  //ʱ��ʱA
#define REG_ALAA_DAY 			0XA0  //�춨ʱA
#define REG_ALAB_MINUS 		0XB0  //�ֶ�ʱB
#define REG_ALAB_HOUR 		0XC0  //ʱ��ʱB
#define REG_ALAB_DAY 			0XD0  //�춨ʱB
#define REG_COMM1 				0XE0  //����1
#define REG_COMM2 				0XF0  //����2


#define	BL_SEC     0
#define	BL_MINUTE  1
#define	BL_HOUR    2
#define	BL_WEEK    3
#define	BL_DAY     4
#define	BL_MONTH   5
#define	BL_YEAR    6
#define	BL_XSL     7
#define	BL_ALAA_MINUS   8
#define	BL_ALAA_HOUR    9
#define	BL_ALAA_DAY     10
#define	BL_ALAB_MINUS   11
#define	BL_ALAB_HOUR    12
#define	BL_ALAB_DAY     13
#define	BL_COMM1        14
#define	BL_COMM2        15


#define BLI2C_SDA 21
#define BLI2C_SCL 22

//#define BL_INTA 13
//#define BL_INTB 12

#define TIME_ERROR 0X10

#define BL_I2C_ADD 0X64
#define I2C_Read 0x01

#define BLI2C_SDA_H nrf_gpio_pin_set(BLI2C_SDA)
#define BLI2C_SDA_L nrf_gpio_pin_clear(BLI2C_SDA)

#define BLI2C_SCL_H nrf_gpio_pin_set(BLI2C_SCL)
#define BLI2C_SCL_L nrf_gpio_pin_clear(BLI2C_SCL)

#define BLI2C_SDA_R nrf_gpio_cfg_input(BLI2C_SDA,NRF_GPIO_PIN_PULLUP)
#define BLI2C_SDA_W nrf_gpio_cfg_output(BLI2C_SDA)


extern uint8_t BL_VALUE_READ[16];  //���ڴ洢ʵʱʱ�������ֽڵĸ���
extern uint8_t BL_VALUE_WRITE[16]; //д��ʵʱʱ�������ֽڵĻ�����

void  BL_I2C_GPIO_init(void);
void  BL_I2C_GPIO_SLEEP(void);

uint8_t BL_read_i2c (uint8_t device,uint8_t addr);
void BL_write_i2c(uint8_t device,uint8_t addr,uint8_t dat);
void bl_value_read(void);
void bl_value_write(void);

uint8_t CaculateWeekDay(int y,int m, int d); //�������ջ�ȡ����

#endif

