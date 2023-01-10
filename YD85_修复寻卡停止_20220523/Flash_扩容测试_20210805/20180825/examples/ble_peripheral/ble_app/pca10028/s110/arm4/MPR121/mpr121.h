#ifndef _MPR121_H_
#define _MPR121_H_

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "bsp_btn_ble.h"
#include "nrf_delay.h"
#include "stdio.h"
#include "speak.h"

// MPR121 文件  已转为键盘实现的通用驱动函数，不再是专为 MPR121 服务的文件

//#define Key_By_MPR121  1  		//当键盘是通过 MPR121 实现的时
    
#define Key_By_TSM12c	 1 	  	//当键盘是通过 TSM12C 实现的时

//#define CN_TSM12C				1      //使用 国产 TSM12C 兼容芯片时

#ifdef Key_By_MPR121

#define I2C_SDA 11

#define I2C_SCL 10

#define MPR_IRQ 0

#endif


#ifdef Key_By_TSM12c

#define I2C_SDA 10

#define I2C_SCL 11

#define I2C_EN  12

#define MPR_IRQ 0

#endif

#ifdef Key_By_MPR121

// MPR121 Register Defines
#define MHD_R	0x2B  
#define NHD_R	0x2C
#define	NCL_R 	0x2D
#define	FDL_R	0x2E
#define	MHD_F	0x2F
#define	NHD_F	0x30
#define	NCL_F	0x31
#define	FDL_F	0x32
#define	ELE0_T	0x41
#define	ELE0_R	0x42
#define	ELE1_T	0x43
#define	ELE1_R	0x44
#define	ELE2_T	0x45
#define	ELE2_R	0x46
#define	ELE3_T	0x47
#define	ELE3_R	0x48
#define	ELE4_T	0x49
#define	ELE4_R	0x4A
#define	ELE5_T	0x4B
#define	ELE5_R	0x4C
#define	ELE6_T	0x4D
#define	ELE6_R	0x4E
#define	ELE7_T	0x4F
#define	ELE7_R	0x50
#define	ELE8_T	0x51
#define	ELE8_R	0x52
#define	ELE9_T	0x53
#define	ELE9_R	0x54
#define	ELE10_T	0x55
#define	ELE10_R	0x56
#define	ELE11_T	0x57
#define	ELE11_R	0x58
#define	FIL_CFG	0x5D
#define	ELE_CFG	0x5E
#define GPIO_CTRL0	0x73
#define	GPIO_CTRL1	0x74
#define GPIO_DATA	0x75
#define	GPIO_DIR	0x76
#define	GPIO_EN		0x77
#define	GPIO_SET	0x78
#define	GPIO_CLEAR	0x79
#define	GPIO_TOGGLE	0x7A
#define	ATO_CFG0	0x7B  //AUTO-CONFIG Control Register 0

#define	ATO_CFGU	0x7D  //AUTO-CONFIG USL Register
#define	ATO_CFGL	0x7E  //AUTO-CONFIG LSL Register
#define	ATO_CFGT	0x7F   //AUTO-CONFIG Target Level Register
#endif

#ifdef Key_By_TSM12c

//TSM12C 	寄存器

//Sensitivity 1  || addr 0x02	||	Reset Value	0xBB	（灵敏度调节寄存器1）
#define STY1	0X02	

//Sensitivity 2  || addr 0x03	||	Reset Value	0xBB	（灵敏度调节寄存器2）
#define STY2	0X03	

//Sensitivity 3  || addr 0x04	||	Reset Value	0xBB	（灵敏度调节寄存器3）
#define STY3	0X04	

//Sensitivity 4  || addr 0x05	||	Reset Value	0xBB	（灵敏度调节寄存器4）
#define STY4	0X05	

//Sensitivity 5  || addr 0x06	||	Reset Value	0xBB	（灵敏度调节寄存器5）
#define STY5	0X06	

//Sensitivity 6  || addr 0x07	||	Reset Value	0xBB	（灵敏度调节寄存器6）
#define STY6	0X07	

//CTRL	1		||	addr  0x08   ||  Reset  Value	0x22	(通用控制寄存器1)
#define CTRL1	0X08

//CTRL	2		||	addr  0x09   ||  Reset  Value	0000 01XXb	(通用控制寄存器2)
#define CTRL2	0X09

//Ref_rst1 1 || addr 0x0A		||  Reset		Value 0xFE	(通道参考复位控制寄存器1)
#define Ref_Rest1 0X0A

//Ref_rst1 2 || addr 0x0B		||  Reset		Value 0x0F	(通道参考复位控制寄存器2)
#define Ref_Rest2 0X0B

//Ch_hold 1 || addr	0x0C		||  Reset  Value  0xFE	(感应通道控制寄存器1)
#define CH_hold1	0x0C

//Ch_hold 2 || addr	0x0D		||  Reset  Value  0xFE	(感应通道控制寄存器2)
#define CH_hold2	0x0D

//Cal_hold 1	|| addr 0x0E		||  Reset  Value	0x00	(通道校准控制寄存器1)
#define Cal_hold1	0x0E

//Cal_hold 2	|| addr 0x0F		||  Reset  Value	0x00	(通道校准控制寄存器2)
#define Cal_hold2	0x0F

//Output 1  ||  addr 0x10			||  Reset  Value  0x00   (输出寄存器1)
#define Output1	0x10

//Output 2  ||  addr 0x11			||  Reset  Value  0x00   (输出寄存器2)
#define Output2	0x11

//Output 3  ||  addr 0x12			||  Reset  Value  0x00   (输出寄存器3)
#define Output3	0x12



#endif


////Global Constants

//#define TOU_THRESH	0x01
//#define	REL_THRESH	0x01

#ifdef Key_By_MPR121

#define I2C_ADD 		0XB4
#define I2C_Read 		0x01

#endif

#ifdef Key_By_TSM12c 

#define I2C_ADD		0xD0   // TSM12C ID_SEL  =0
#define I2C_Read	0x01   //暂时挂起

// 

#endif

#define I2C_SDA_H nrf_gpio_pin_set(I2C_SDA)
#define I2C_SDA_L nrf_gpio_pin_clear(I2C_SDA)
#define I2C_SCL_H nrf_gpio_pin_set(I2C_SCL)
#define I2C_SCL_L nrf_gpio_pin_clear(I2C_SCL)
#define I2C_SDA_R nrf_gpio_cfg_input(I2C_SDA,NRF_GPIO_PIN_PULLUP)
#define I2C_SDA_W nrf_gpio_cfg_output(I2C_SDA)

#ifdef Key_By_TSM12c

#define TSM_EN_L 	nrf_gpio_pin_clear(I2C_EN)

#define TSM_EN_H	nrf_gpio_pin_set(I2C_EN)

#endif

extern uint8_t MPR121_RUN; //状态
extern uint16_t MPR121_T;   //计时，

void I2C_GPIO_init(void);
void I2C_delay(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_WAIT_ACK(void);
void I2C_SEND_ACK(uint8_t ack);
void I2C_WRITE_BYTE (uint8_t dat);
uint8_t I2C_READ_BYTE(void);
void write_i2c(uint8_t device,uint8_t addr,uint8_t dat);
uint8_t read_i2c (uint8_t device,uint8_t addr);
void mpr121_init(void);
void mpr121_Run(void);
void MSR_IRQ_init(void);
void MSR_121_READ(uint8_t *keydat);
uint8_t MSR_121_read_key(void);

void mpr121_Run_I(void);  //读取工作状态 (性能模式) 下的基线值
void mpr121_init_I(void);		//读取工作状态 （低功耗模式） 下的基线值

void TSM12_Init (void);

void TSM12_read_tast(void);

void TSM12_Exit_Sleep_Mode(void);
uint8_t TSM12_read_key_Num(void);

uint8_t MPR121_Key_Run(void); //运行时的键盘读取过程

#endif

