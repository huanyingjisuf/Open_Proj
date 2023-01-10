#include <MPR121.h>
#include <Moto_Run.h>

#include <Devic_Init.h>


//I2C初始化


uint8_t i2c_statue;



uint8_t TOU_THRESH	= 0x02;
uint8_t	REL_THRESH	= 0x03; //设定工作触发值

uint8_t TOU_THRESH1	= 0x03;
uint8_t	REL_THRESH1 = 0x03;  //设定触发阈值


void I2C_GPIO_init()
{ 
// 	nrf_gpio_pin_pull_t NRF_GPIO_PIN_PULL_T; 
	nrf_gpio_cfg_output(I2C_SDA);
	nrf_gpio_cfg_output(I2C_SCL);
	nrf_gpio_cfg_output(I2C_EN);
	
	nrf_gpio_cfg_input(MPR_IRQ,NRF_GPIO_PIN_PULLUP);
	
	I2C_SDA_H; 
	I2C_SCL_H;
	
}



void I2C_delay(void)
{
	nrf_delay_us(10);
	
}


void I2C_START(void)
{
	I2C_SDA_H;
	I2C_SCL_H;
	I2C_delay();
	I2C_SDA_L;
	I2C_delay();
	I2C_SCL_L;
	I2C_delay();
	
}

void I2C_STOP(void)
{
	
	I2C_SDA_L;
	I2C_SCL_L;
	I2C_delay();
	I2C_SCL_H;
	I2C_delay();
	I2C_SDA_H;
	
}

void I2C_WAIT_ACK()
{
	uint8_t i;
	I2C_SCL_H;
	I2C_delay();
	while((nrf_gpio_pin_read(I2C_SDA)==1)&&(i<255))
		i++;
	I2C_SCL_L;
	I2C_delay();
}

void I2C_SEND_ACK(uint8_t ack)
{

	if(ack==1)
		I2C_SDA_L;
	else 
		I2C_SDA_H;
	
	I2C_SCL_H;
	I2C_delay();
	
	I2C_SCL_L;
	I2C_delay();
	
	I2C_SDA_H;
	I2C_delay();

}

void I2C_WRITE_BYTE(uint8_t dat)
{
	uint8_t i,temp;
	temp = dat;
	for (i =0;i<8;i++)
	{
		I2C_SCL_L;
		I2C_delay();
		if(temp&0x80)
			I2C_SDA_H;
		else
			I2C_SDA_L;
		
		temp <<= 1;
		I2C_delay();
		I2C_SCL_H;
	}
	I2C_SCL_L;
	I2C_delay();
	I2C_SDA_H;
	I2C_delay();
}

uint8_t I2C_READ_BYTE()
{
	uint8_t i,dat;
	I2C_SCL_L;
	I2C_delay();
	I2C_SDA_H;
	I2C_delay();
	for(i =0;i<8;i++)
	{
		I2C_SCL_H;
		I2C_delay();
		dat<<=1;
		dat |= nrf_gpio_pin_read(I2C_SDA);
		I2C_SCL_L;
		I2C_delay();
	}
	I2C_SDA_H;
	I2C_delay();
	
	return dat;
}

void write_i2c(uint8_t device,uint8_t addr,uint8_t dat)
{
	I2C_START();
  I2C_WRITE_BYTE(device);
	
	I2C_WAIT_ACK();
	I2C_WRITE_BYTE(addr);
	
	I2C_WAIT_ACK();
	I2C_WRITE_BYTE(dat);
	
	I2C_WAIT_ACK();
	I2C_STOP();
}

uint8_t read_i2c (uint8_t device,uint8_t addr)
{ 
	uint8_t dat;
	
	I2C_START();
	
	I2C_WRITE_BYTE (device);
	I2C_WAIT_ACK ();
	
	I2C_WRITE_BYTE (addr);
	I2C_WAIT_ACK();
	
	I2C_START();
	I2C_WRITE_BYTE(device+I2C_Read); 
	
	// 器件地址加 I2C_Read 表示读
	
	I2C_WAIT_ACK();
	
	dat = I2C_READ_BYTE();
	I2C_SEND_ACK(0);
	I2C_STOP();
	
	
	return dat;
	
}

uint8_t run1 =0; //触发后自带，重新初始化键盘一次

uint8_t mpr121_Baseline_Init[12];  //存放Init基线 值

void mpr121_init_I(void)
{
	
	#ifdef Key_By_MPR121	
	
	uint8_t i;
	
	run1 =0;
	write_i2c(I2C_ADD,0X80,0X63);  //soft REST
	// Section A 当值大于基线时
	// This group controls filtering when data is > baseline.
	write_i2c(I2C_ADD,MHD_R, 0x0F);  //最大半增量
	write_i2c(I2C_ADD,NHD_R, 0x0F);  //噪声半增量
	write_i2c(I2C_ADD,NCL_R, 0xff);  //噪声计数限制
	write_i2c(I2C_ADD,FDL_R, 0x0F);  //滤波器延迟数限制

	// Section B 当值小于基线时
	// This group controls filtering when data is < baseline.
	write_i2c(I2C_ADD,MHD_F, 0x0F);  //
	write_i2c(I2C_ADD,NHD_F, 0x0F); //
	write_i2c(I2C_ADD,NCL_F, 0xff); //
	write_i2c(I2C_ADD,FDL_F, 0x0F); //
	
	
	// Section C
	// This group sets touch and release thresholds for each electrode
	write_i2c(I2C_ADD,ELE0_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE0_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE1_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE1_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE2_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE2_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE3_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE3_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE4_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE4_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE5_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE5_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE6_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE6_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE7_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE7_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE8_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE8_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE9_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE9_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE10_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE10_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE11_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE11_R, REL_THRESH1);
	
  // Section D
  // Set the Filter Configuration 设置过滤 (扫描间隔)
  // Set ESI2
	
	write_i2c(I2C_ADD,0X5C,0XE0); //一级过滤和和充电电流。
	
  write_i2c(I2C_ADD,0x5D,0x26);  //0X5D
	write_i2c(I2C_ADD,0X5B,0X00); //防反跳 7次
  
//	
  // Section E
  // Electrode Configuration
  // Enable 6 Electrodes and set to run mode
  // Set ELE_CFG to 0x00 to return to standby mode

	write_i2c(I2C_ADD,ELE_CFG, 0xCC);	// Enables all 12 Electrodes
	
//  write_i2c(I2C_ADD,ELE_CFG, 0x06);		// Enable first 6 electrodes

//  Section F
	//Enable Auto Config and auto Reconfig
	write_i2c(I2C_ADD,ATO_CFG0, 0x00);  //   
	write_i2c(I2C_ADD,ATO_CFGU, 0xCF);  // // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   write_i2c(I2C_ADD,ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
	write_i2c(I2C_ADD,ATO_CFGT, 0x45);	// // Target = 0.9*USL = 0xB5 @3.3V
//	MPR121_RUN =0;
	
	nrf_delay_ms(1000);  //等待稳定
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"MPR121_Init_I_NUM:\r\n");
	
	#endif
	
	for(i =0;i<12;i++)
	{
		mpr121_Baseline_Init[i] = read_i2c(I2C_ADD,(i+0X1E));
		#ifdef uart_start
		SEGGER_RTT_printf(0," %x ",mpr121_Baseline_Init[i]);
		#endif
	}  //读取基线值到缓存
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\n");
	
	#endif
	
	#endif
				MPR121_RUN =0;
	
}


void mpr121_init(void)
{
	#ifdef Key_By_MPR121
	uint8_t i;
	run1 =0;
	write_i2c(I2C_ADD,0X80,0X63);  //soft REST
	// Section A 当值大于基线时
	// This group controls filtering when data is > baseline.
	write_i2c(I2C_ADD,MHD_R, 0x0F);  //最大半增量
	write_i2c(I2C_ADD,NHD_R, 0x0F);  //噪声半增量
	write_i2c(I2C_ADD,NCL_R, 0xff);  //噪声计数限制
	write_i2c(I2C_ADD,FDL_R, 0x0F);  //滤波器延迟数限制

	// Section B 当值小于基线时
	// This group controls filtering when data is < baseline.
	write_i2c(I2C_ADD,MHD_F, 0x0F);  //
	write_i2c(I2C_ADD,NHD_F, 0x0F); //
	write_i2c(I2C_ADD,NCL_F, 0xff); //
	write_i2c(I2C_ADD,FDL_F, 0x0F); //
	
	
	// Section C
	// This group sets touch and release thresholds for each electrode
	write_i2c(I2C_ADD,ELE0_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE0_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE1_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE1_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE2_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE2_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE3_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE3_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE4_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE4_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE5_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE5_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE6_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE6_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE7_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE7_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE8_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE8_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE9_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE9_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE10_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE10_R, REL_THRESH1);
	write_i2c(I2C_ADD,ELE11_T, TOU_THRESH1);
	write_i2c(I2C_ADD,ELE11_R, REL_THRESH1);
	
  // Section D
  // Set the Filter Configuration 设置过滤 (扫描间隔)
  // Set ESI2
	
	write_i2c(I2C_ADD,0X5C,0XE0); //一级过滤和和充电电流。
	
  write_i2c(I2C_ADD,0x5D,0x26);  //0X5D
	write_i2c(I2C_ADD,0X5B,0X00); //防反跳 7次
  
//	
  // Section E
  // Electrode Configuration
  // Enable 6 Electrodes and set to run mode
  // Set ELE_CFG to 0x00 to return to standby mode

	
		for(i =0;i<12;i++)  //将基线值写入
		{
			write_i2c(I2C_ADD,(i+0x1E),mpr121_Baseline_Init[i]);
		}
	
		
			write_i2c(I2C_ADD,ELE_CFG, 0x4C);	// Enables all 12 Electrodes
	
		
//  write_i2c(I2C_ADD,ELE_CFG, 0x06);		// Enable first 6 electrodes

//  Section F
	//Enable Auto Config and auto Reconfig
	write_i2c(I2C_ADD,ATO_CFG0, 0x00);  //   
	write_i2c(I2C_ADD,ATO_CFGU, 0xCF);  // // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   write_i2c(I2C_ADD,ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
	write_i2c(I2C_ADD,ATO_CFGT, 0x45);	// // Target = 0.9*USL = 0xB5 @3.3V
	
	
	nrf_delay_ms(10);
	
	#endif
	
}


uint8_t mpr121_Baseline[12];  //保存基线值
#define MPR121RUN_0X5C 0X30//一级过滤和和充电电流。
#define MPR121RUN_0X5D 0X2B//0X5D 充电周期，二级过滤次数 扫描周期

#define MPR121RUN_MHD_R  0X02//最大半增量
#define MPR121RUN_NHD_R  0X02
#define MPR121RUN_NCL_R  0XFF
#define MPR121RUN_FDL_R  0X01


#define MPR121RUN_MHD_F	0X02
#define MPR121RUN_NHD_F	0X02

#define MPR121RUN_NCL_F	0XFF
#define MPR121RUN_FDL_F	0X01

//#define	ATO_CFG0	0x7B  //AUTO-CONFIG Control Register 0

//#define	ATO_CFGU	0x7D  //AUTO-CONFIG USL Register
//#define	ATO_CFGL	0x7E  //AUTO-CONFIG LSL Register
//#define	ATO_CFGT	0x7F   //AUTO-CONFIG Target Level Register


#define MPR121RUN_ATO_CFG0_F  0xC3
#define MPR121RUN_ATO_CFG0_FI  0XC3

#define MPR121RUN_ATO_CFGU_F 	0XC4
#define MPR121RUN_ATO_CFGT_F 0XB0
#define MPR121RUN_ATO_CFGL_F  0X7F


void mpr121_Run_I(void)  //读取工作状态 (性能模式) 下的基线值
{
	#ifdef Key_By_MPR121
//	if(run1==0)
//	run1 =1;
	uint8_t i;
	
		for(i =0;i<sizeof(mpr121_Baseline);i++)
		{
		
			mpr121_Baseline[i] =0;  //初始化基线缓存值
		
		}
	
	
	write_i2c(I2C_ADD,0X80,0X63);  //soft REST
	// Section A 当值大于基线时
	// This group controls filtering when data is > baseline.
	write_i2c(I2C_ADD,MHD_R, MPR121RUN_MHD_R);  //最大半增量  0X2B
	write_i2c(I2C_ADD,NHD_R, MPR121RUN_NHD_R);  //噪声半增量		0X2C
	write_i2c(I2C_ADD,NCL_R, MPR121RUN_NCL_R);  //噪声计数限制  0X2D
	write_i2c(I2C_ADD,FDL_R, MPR121RUN_FDL_R);  //滤波器延迟数限制	0X2E

	// Section B 当值小于基线时
 	write_i2c(I2C_ADD,MHD_F, MPR121RUN_MHD_F);  // 0X2F
	write_i2c(I2C_ADD,NHD_F, MPR121RUN_NHD_F); //	0X30
	write_i2c(I2C_ADD,NCL_F, MPR121RUN_NCL_F); // 0X31
	write_i2c(I2C_ADD,FDL_F, MPR121RUN_FDL_F); // 0X32
	
	// Section C
	// This group sets touch and release thresholds for each electrode

	
	write_i2c(I2C_ADD,ELE0_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE0_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE1_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE1_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE2_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE2_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE3_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE3_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE4_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE4_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE5_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE5_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE6_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE6_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE7_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE7_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE8_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE8_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE9_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE9_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE10_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE10_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE11_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE11_R, REL_THRESH);


  // Section D
  // Set the Filter Configuration 设置过滤 (扫描间隔)
  // Set ESI2
	
	write_i2c(I2C_ADD,0X5C,MPR121RUN_0X5C); //一级过滤和和充电电流。
	
	write_i2c(I2C_ADD,FIL_CFG,MPR121RUN_0X5D);  //0X5D 充电周期，二级过滤次数 扫描周期
	write_i2c(I2C_ADD,0X5B,0X00); //防反跳 7次
	
	// Section E
	// Electrode Configuration
	// Enable 6 Electrodes and set to run mode
	// Set ELE_CFG to 0x00 to return to standby mode
	
	write_i2c(I2C_ADD,ELE_CFG, 0xCC);	// Enables all 12 Electrodes 并锁定基线
	
//	MPR121_RUN =1;
//  write_i2c(I2C_ADD,ELE_CFG, 0x06);		// Enable first 6 electrodes

//Section F
	//Enable Auto Config and auto Reconfig
	write_i2c(I2C_ADD,ATO_CFG0, MPR121RUN_ATO_CFG0_FI);  //   
	write_i2c(I2C_ADD,ATO_CFGU, MPR121RUN_ATO_CFGU_F);  // // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   write_i2c(I2C_ADD,ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
	write_i2c(I2C_ADD,ATO_CFGT, MPR121RUN_ATO_CFGT_F);	// // Target = 0.9*USL = 0xB5 @3.3V
	write_i2c(I2C_ADD,ATO_CFGL, MPR121RUN_ATO_CFGL_F);	// // Target = 0.9*USL = 0xB5 @3.3V

	nrf_delay_ms(500);  //等待稳定
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"MPR121_RUN_I_NUM:\r\n");
	
	#endif
	
	for(i =0;i<12;i++)
	{
		mpr121_Baseline[i] = read_i2c(I2C_ADD,(i+0X1E));
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0," %x ",mpr121_Baseline[i]);
		
		#endif
		
	}  //读取基线值到缓存
	
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\n");
		
		#endif
		
		#endif
	
}



void mpr121_Run(void)  //工作状态 (性能模式)
{
	#ifdef Key_By_MPR121
	uint8_t i;
	if(run1==0)
	run1 =1;
	write_i2c(I2C_ADD,0X80,0X63);  //soft REST
	
	// Section A 当值大于基线时
	// This group controls filtering when data is > baseline.
	// Section A 当值大于基线时
	// This group controls filtering when data is > baseline.
	write_i2c(I2C_ADD,MHD_R, MPR121RUN_MHD_R);  //最大半增量  0X2B
	write_i2c(I2C_ADD,NHD_R, MPR121RUN_NHD_R);  //噪声半增量		0X2C
	write_i2c(I2C_ADD,NCL_R, MPR121RUN_NCL_R);  //噪声计数限制  0X2D
	write_i2c(I2C_ADD,FDL_R, MPR121RUN_FDL_R);  //滤波器延迟数限制	0X2E

	// Section B 当值小于基线时
 	write_i2c(I2C_ADD,MHD_F, MPR121RUN_MHD_F);  // 0X2F
	write_i2c(I2C_ADD,NHD_F, MPR121RUN_NHD_F); //	0X30
	write_i2c(I2C_ADD,NCL_F, MPR121RUN_NCL_F); // 0X31
	write_i2c(I2C_ADD,FDL_F, MPR121RUN_FDL_F); // 0X32
	
	// Section C
	// This group sets touch and release thresholds for each electrode
	write_i2c(I2C_ADD,ELE0_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE0_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE1_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE1_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE2_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE2_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE3_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE3_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE4_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE4_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE5_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE5_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE6_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE6_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE7_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE7_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE8_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE8_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE9_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE9_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE10_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE10_R, REL_THRESH);
	write_i2c(I2C_ADD,ELE11_T, TOU_THRESH);
	write_i2c(I2C_ADD,ELE11_R, REL_THRESH);

  // Section D
  // Set the Filter Configuration 设置过滤 (扫描间隔)
  // Set ESI2
	
	write_i2c(I2C_ADD,0X5C,MPR121RUN_0X5C); //一级过滤和和充电电流。
	
	write_i2c(I2C_ADD,FIL_CFG,MPR121RUN_0X5D);  //0X5D 充电周期，二级过滤次数 扫描周期
	write_i2c(I2C_ADD,0X5B,0X01); //防反跳 7次
	
	// Section E
	// Electrode Configuration
	// Enable 6 Electrodes and set to run mode
	// Set ELE_CFG to 0x00 to return to standby mode
	
	
			for(i =0;i<12;i++)  //将基线值写入
		{
			write_i2c(I2C_ADD,(i+0x1E),mpr121_Baseline[i]);
		}
	
	write_i2c(I2C_ADD,ELE_CFG, 0x4C);	// Enables all 12 Electrodes 并锁定基线
	
	
	

//  write_i2c(I2C_ADD,ELE_CFG, 0x06);		// Enable first 6 electrodes

//Section F
	//Enable Auto Config and auto Reconfig
	write_i2c(I2C_ADD,ATO_CFG0, MPR121RUN_ATO_CFG0_F);  //   
	write_i2c(I2C_ADD,ATO_CFGU, MPR121RUN_ATO_CFGU_F);  // // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   write_i2c(I2C_ADD,ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
	write_i2c(I2C_ADD,ATO_CFGT, MPR121RUN_ATO_CFGT_F);	// // Target = 0.9*USL = 0xB5 @3.3V
	write_i2c(I2C_ADD,ATO_CFGL, MPR121RUN_ATO_CFGL_F);	// // Target = 0.9*USL = 0xB5 @3.3V

//	nrf_delay_ms(10);
					MPR121_RUN =1;
#endif

}

//#define	ATO_CFG0	0x7B
//#define	ATO_CFGU	0x7D
//#define	ATO_CFGL	0x7E
//#define	ATO_CFGT	0x7F

uint8_t key_flag =0;
void MSR_121_READ(uint8_t *keydat) //将获取到的数据，输出到dat0,dat1
{
	#ifdef Key_By_MPR121
	
	static uint8_t read_dat;
	read_dat =0;
	if(!nrf_gpio_pin_read(MPR_IRQ))
		{
			read_dat = read_i2c(I2C_ADD,0X00);
			*keydat =read_dat;
			keydat++;
		#ifdef uart_start
		SEGGER_RTT_printf(0,"ADD_0X00:%d \r\n",read_dat);
		#endif
		read_dat = read_i2c(I2C_ADD,0X01);
		*keydat = read_dat;
		#ifdef uart_start	
		SEGGER_RTT_printf(0,"ADD_0X01:%d \r\n",read_dat);
		#endif
		}
		
		#endif
}

extern uint16_t MPR_Value;  //获取的值
//APP_TIMER_DEF (Read_My_Key);  //声明新的软件定时器名称
uint8_t readkey_now =0;
uint8_t read_ir =0;  //重入计数

void MPR121_CL_Key()
{
	#ifdef Key_By_MPR121
		uint8_t key_dat[2];
		static uint8_t read_dat;

		read_dat = read_i2c(I2C_ADD,0X00);
		key_dat[0] =read_dat;
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"ADD_0X00:%d \r\n",read_dat);
		#endif
		read_dat = read_i2c(I2C_ADD,0X01);
		key_dat[1] = read_dat;
		#ifdef uart_start	
		SEGGER_RTT_printf(0,"ADD_0X01:%d \r\n",read_dat);
		#endif
		read_ir++;
		if(read_ir>=2) //当长时间未还原，则强制还原
		{
			read_ir =0;
			mpr121_Run();				
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Mpr121_Cl~\r\n"); //反馈强制清空
			#endif
		}
//		if((key_dat[0]==0)&&(key_dat[1]==0))
//		{
//			readkey_now =0;  //还原状态
//			read_ir =0;
//			if(run1==1)
//			{
//				nrf_delay_ms(10);
//				mpr121_Run();
//				run1++;  //只触发一次
//			}
//		}
readkey_now =0;
		
		#endif
}

uint8_t MSR_121_read_key()  //MSR_121_read_key
{
	
//	uint8_t key_dat[2];
//	uint8_t i,it,is,ir;  //循环变量

	uint8_t key_num; //key值缓存
	key_num = 0xFF; //初始key值高于全部key值
	
	#ifdef Key_By_MPR121
	
	if(!nrf_gpio_pin_read(MPR_IRQ)) //仅在有中断触发时读取
	{
		MSR_121_READ(key_dat);  //获取key值
	//	readkey_now =1;  //当中断进入过后，下次进入等待清空状态
		if(key_dat[0]==0x00)
		{
			it =0x01;
			is =0;
			for(i =0;i<4;i++)
			{
				if(key_dat[1]&it) is++;
				it<<=1;
			}
		}
		else if(key_dat[1]==0x00)
		{
			it =0x01;
			is =0;
			for(i =0;i<8;i++)
			{
				if (key_dat[0]&it) is++;
				it<<=1;
			}
		}
		if(is==1) //当触发按键合理时，尝试获取键值
		{		 
			MPR121_T =0;  //按键续时
			if(key_dat[0]==0x00)  //当0为空时
			{
				switch (key_dat[1]) //根据情况给出key值
				{
					case 8:
							key_num =12;
						//	run1 =1;
				#ifdef uart_start
					
				MPR_Value = read_i2c(I2C_ADD,0X51);
					SEGGER_RTT_printf(0,"MPR121_E8TTH: %d\r\n",MPR_Value);
				MPR_Value = read_i2c(I2C_ADD,0X52);
					SEGGER_RTT_printf(0,"MPR121_E8RTH: %d\r\n",MPR_Value);				
				MPR_Value = read_i2c(I2C_ADD,0X26);
				SEGGER_RTT_printf(0,"MPR121_8BASVALUE: %d\r\n",MPR_Value); //读取基线值	
				MPR_Value =	read_i2c(I2C_ADD,0x15);
				MPR_Value <<=8;
				MPR_Value |=	read_i2c(I2C_ADD,0x14);
				MPR_Value>>=2;    //只获取最高8字节
				SEGGER_RTT_printf(0,"MPR121_8VALUEKEY: %d\r\n",MPR_Value); //读取键值
				
				#endif
					
						break;
					case 4:
						  key_num =11;
					
				#ifdef uart_start			
				MPR_Value = read_i2c(I2C_ADD,0X51);
					SEGGER_RTT_printf(0,"MPR121_E8TTH: %d\r\n",MPR_Value);
				MPR_Value = read_i2c(I2C_ADD,0X52);
					SEGGER_RTT_printf(0,"MPR121_E8RTH: %d\r\n",MPR_Value);				
				MPR_Value = read_i2c(I2C_ADD,0X26);
				SEGGER_RTT_printf(0,"MPR121_8BASVALUE: %d\r\n",MPR_Value); //读取基线值	
				MPR_Value =	read_i2c(I2C_ADD,0x15);
				MPR_Value <<=8;
				MPR_Value |=	read_i2c(I2C_ADD,0x14);
				MPR_Value>>=2;    //只获取最高8字节
				SEGGER_RTT_printf(0,"MPR121_8VALUEKEY: %d\r\n",MPR_Value); //读取键值
				
				#endif
					
						break;
					case 2:
							key_num =10;
					
				#ifdef uart_start		
				MPR_Value = read_i2c(I2C_ADD,0X51);
					SEGGER_RTT_printf(0,"MPR121_E8TTH: %d\r\n",MPR_Value);
				MPR_Value = read_i2c(I2C_ADD,0X52);
					SEGGER_RTT_printf(0,"MPR121_E8RTH: %d\r\n",MPR_Value);				
				MPR_Value = read_i2c(I2C_ADD,0X26);
				SEGGER_RTT_printf(0,"MPR121_8BASVALUE: %d\r\n",MPR_Value); //读取基线值	
				MPR_Value =	read_i2c(I2C_ADD,0x15);
				MPR_Value <<=8;
				MPR_Value |=	read_i2c(I2C_ADD,0x14);
				MPR_Value>>=2;    //只获取最高8字节
				SEGGER_RTT_printf(0,"MPR121_8VALUEKEY: %d\r\n",MPR_Value); //读取键值
				
				#endif
					run1 =1;  //开始新的初始化周期
						break;
					case 1:
							key_num =9; //#

				#ifdef uart_start
					
				MPR_Value = read_i2c(I2C_ADD,0X51);
					SEGGER_RTT_printf(0,"MPR121_E8TTH: %d\r\n",MPR_Value);
				MPR_Value = read_i2c(I2C_ADD,0X52);
					SEGGER_RTT_printf(0,"MPR121_E8RTH: %d\r\n",MPR_Value);				
				MPR_Value = read_i2c(I2C_ADD,0X26);
				SEGGER_RTT_printf(0,"MPR121_8BASVALUE: %d\r\n",MPR_Value); //读取基线值	
				MPR_Value =	read_i2c(I2C_ADD,0x15);
				MPR_Value <<=8;
				MPR_Value |=	read_i2c(I2C_ADD,0x14);
				MPR_Value>>=2;    //只获取最高8字节
				SEGGER_RTT_printf(0,"MPR121_8VALUEKEY: %d\r\n",MPR_Value); //读取键值
				
				#endif
//				
				//	Lock_Open();
					break;
				}
			}
			else if (key_dat[1]==0x00) //当1为空时
			{
				switch (key_dat[0]) //根据情况给出key值
				{
					case 8: 
						key_num =4;
				#ifdef uart_start
				MPR_Value = read_i2c(I2C_ADD,0X47);
					SEGGER_RTT_printf(0,"MPR121_E3TTH: %d\r\n",MPR_Value);
					
				MPR_Value = read_i2c(I2C_ADD,0X48);
					SEGGER_RTT_printf(0,"MPR121_E3RTH: %d\r\n",MPR_Value);
						
				MPR_Value = read_i2c(I2C_ADD,0X21);
				SEGGER_RTT_printf(0,"MPR121_3BASVALUE: %d\r\n",MPR_Value); //读取基线值	
				MPR_Value =	read_i2c(I2C_ADD,0x0B);
				MPR_Value <<=8;
				MPR_Value |=	read_i2c(I2C_ADD,0x0A);
				MPR_Value>>=2;   //只获取最高8字节
				SEGGER_RTT_printf(0,"MPR121_3VALUEKEY: %d\r\n",MPR_Value); //读取键值
				#endif
						break;
					case 128:
						key_num =8;
						break;
					case 4:
						key_num =3;
						break;
					case 64:
						key_num =7;
						break;
					case 2:
						key_num =2;
						break;
					case 32:
						key_num =6;
						break;
					case 1:
						key_num = 1;
					break;
					case 16:
						key_num =5;
					break;
				}
			}
		}
		else
		{
			readkey_now =0; //清空过键盘，不进入等待状态
			mpr121_Run(); //获取键值后，键值不合理则初始化一次键盘
		}
		if(MPR121_RUN==0)  //当键盘未唤醒时，唤醒键盘
			mpr121_Run();
		if((is==1)&&(key_num<=12))
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"    Key_Num: %d\r\n",key_num);
			#endif
	
//			input_speak(15); //按键声
			
		}
//		if((key_dat[0]==0)&&(key_dat[1]==0))
//		{
////		if(run1==1)  //唤醒初期多次采样
////			{
////				nrf_delay_ms(800);
////				mpr121_Run();
////				run1++;  //只触发预设次数
////			}
//		}
	}
	#endif
	return key_num; 
	
}

uint8_t MPR121_Key_Run() //运行时的键盘读取过程
{
	#ifdef Key_By_MPR121
	uint8_t key_num;
	if(readkey_now==0)//正常读取
	{  
		key_num =MSR_121_read_key();
//			#ifdef uart_start
//			printf("  return  Key_Num: %d\r\n",key_num);
//			#endif
		if(key_num<=12) 
			return key_num; //当键值有效时返回键值
		else 
		{	
		
			mpr121_Run();
			return 0xff; //当键值无效时，返回0XFF表示键值无效
		
		}
	}
	else if(readkey_now ==1)
	{
		MPR121_CL_Key();
	}
	//else //出错重置
		readkey_now =0;
	
//		if((key_dat[0]==0)&&(key_dat[1]==0))
//		{
//			readkey_now =0;  //还原状态
//			read_ir =0;

//		}
	
	#endif
	
	return 0xFF;
	
}


const uint8_t key0[] =  //key0 键值
{8,128,4,64,2,32,1,16};  //1，2，4，5，7，8，*，0，
const uint8_t key1[] = //key1 键值
{8,4,2,1}; 						//3，6，9，#





//初始化TSM12 


void TSM12_Init()  //进入低功耗模式
{
	
	#ifdef Key_By_TSM12c
	
	TSM_EN_L;  //使能 TSM12 //已由硬件拉低
	
	nrf_delay_ms(200);
	
	// H/W Reset, enable software reset 
	
	write_i2c(I2C_ADD,CTRL2,0X0F);  //enable sleep mode
	
	nrf_delay_ms(200);
	
	//调节灵敏度到最高
	
	write_i2c(I2C_ADD,STY1,0XAA);  //设置最高灵敏度
	
	write_i2c(I2C_ADD,STY2,0XAA);  //设置最高灵敏度

	write_i2c(I2C_ADD,STY3,0XAA);  //设置最高灵敏度
	
	write_i2c(I2C_ADD,STY4,0XAA);  //设置最高灵敏度
	
	write_i2c(I2C_ADD,STY5,0XAA);  //设置最高灵敏度
	
	write_i2c(I2C_ADD,STY6,0X8A);  //设置最高灵敏度
	
	write_i2c(I2C_ADD,CTRL1,0X03);   //设置中断为低中高输出 
	
	write_i2c(I2C_ADD,Ref_Rest1,0X00);			//禁用自动参考值更新
	
	write_i2c(I2C_ADD,Ref_Rest2,0X00);			//禁用自动参考值更新
	
	write_i2c(I2C_ADD,CH_hold1,0x00);				//启用通道
	write_i2c(I2C_ADD,CH_hold2,0x00);				//启用通道
	
	write_i2c (I2C_ADD,CTRL2,0X07);  //进入低功耗模式	
	
	
	nrf_delay_ms(200);
	// reset of
	
	//write_i2c(I2C_ADD,CTRL2,0X03);  // TSM12C 进入正常模式
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TSM12M_Init!\r\n");
	
	#endif
	
	MPR121_RUN =0;
	
	#endif

	TSM_EN_H;  // TSM12 I2C disable

}

//TSM 12C 进入低功耗模式

void TSM12_Into_Sleep_Mode()
{

 
	
	#ifdef Key_By_TSM12c
	
	TSM_EN_H;  // 如果之前操作过 TSM_EN_L 必须拉高 
	
	nrf_delay_ms(300);
	
	TSM_EN_L;
	
	nrf_delay_ms(100);
	
  write_i2c (I2C_ADD,CTRL2,0X07);  //进入低功耗模式
	
	nrf_delay_ms(200);
	 TSM_EN_H;
#endif

}



// TSM12_Exit_Sleep_Mode 

uint8_t TSM12_RUN_Sen[6];			//缓存 TSM12_ 运行模式下的灵敏度

void TSM12_Exit_Sleep_Mode()  //进入运行模式
{
	
	#ifdef Key_By_TSM12c
	

	TSM_EN_L;  // i2c enable
	
	nrf_delay_ms(200);
	MPR121_RUN =1;
	
	
	write_i2c(I2C_ADD,CTRL2,0X0F);  //enable sleep mode
	
	nrf_delay_ms(200);
	
	//调节灵敏度到最高
	//标准灵敏度  3  高灵敏度  1 低灵敏度  4
	
//	write_i2c(I2C_ADD,STY1,0X33);  //设置最高灵敏度 
//	write_i2c(I2C_ADD,STY2,0X13);  //设置最高灵敏度
//	write_i2c(I2C_ADD,STY3,0X22);  //设置最高灵敏度
//	write_i2c(I2C_ADD,STY4,0X22);  //设置最高灵敏度
//	write_i2c(I2C_ADD,STY5,0X22);  //设置最高灵敏度
//	write_i2c(I2C_ADD,STY6,0X12);  //设置最高灵敏度

		write_i2c(I2C_ADD,STY1,TSM12_RUN_Sen[0]);  //设置最高灵敏度 
		write_i2c(I2C_ADD,STY2,TSM12_RUN_Sen[1]);  //设置最高灵敏度
		write_i2c(I2C_ADD,STY3,TSM12_RUN_Sen[2]);  //设置最高灵敏度
		write_i2c(I2C_ADD,STY4,TSM12_RUN_Sen[3]);  //设置最高灵敏度
		write_i2c(I2C_ADD,STY5,TSM12_RUN_Sen[4]);  //设置最高灵敏度
		write_i2c(I2C_ADD,STY6,TSM12_RUN_Sen[5]);  //设置最高灵敏度
//	
	write_i2c(I2C_ADD,CTRL1,0X01);   //设置中断为低中高输出 
	write_i2c(I2C_ADD,Ref_Rest1,0X00);			//禁用自动参考值更新
	write_i2c(I2C_ADD,Ref_Rest2,0X00);			//禁用自动参考值更新
	
	write_i2c(I2C_ADD,CH_hold1,0x00);				//启用通道
	write_i2c(I2C_ADD,CH_hold2,0x00);				//启用通道
	
	write_i2c(I2C_ADD,CTRL2,0X03);
	
	
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM12_Exit_Sleep!\r\n");
	
	#endif
	
//	 TSM_EN_H;
	
	#endif
}

uint8_t TSM12MC_Status =0;		//判断 TSM12MC是否存在

void TSM12_read_tast()  //TSM12 读取测试
{
	
	#ifdef Key_By_TSM12c
	
	TSM12MC_Status =0;
	
	uint8_t it,it1,it2,it3;
	
	TSM12_Exit_Sleep_Mode();
	
//	TSM_EN_L;  // i2c enable
	
	nrf_delay_ms(100);
	
	#ifdef uart_start

	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Read_TM_Tast\r\n");
	
	#endif
	
	it=	read_i2c(I2C_ADD,0X02);  //读取 02h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_02H:%x \r\n",it);  //默认值 10111011b （0xbb）
	
	#endif
	
	it=	read_i2c(I2C_ADD,0X03);  //读取 03h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_03H:%x \r\n",it);  //默认值 10111011b （0xbb）
	
	#endif
	
	it =	read_i2c(I2C_ADD,0X04);  //读取 04h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_04H:%x \r\n",it);  //默认值 10111011b （0xbb）
	
	#endif
	
	it =	read_i2c(I2C_ADD,0X05);  //读取 05h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_05H:%x \r\n",it);  //默认值 10111011b （0xbb）
	
	#endif
	
	it =	read_i2c(I2C_ADD,0X06);  //读取 06h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_06H:%x \r\n",it);  //默认值 10111011b （0xbb）
	
	#endif
	
	it =	read_i2c(I2C_ADD,0X07);  //读取 07h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_07H:%x \r\n",it);  //默认值 10111011b （0xbb）
	
	#endif
	
	it1 =	read_i2c(I2C_ADD,0X10);  //读取 10h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_10H:%x \r\n",it1);  //默认值 00000000b （0x00）
	
	#endif
	
	it2 =	read_i2c(I2C_ADD,0X11);  //读取 11h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_11H:%x \r\n",it2);  //默认值 00000000b （0x00）
	
	#endif
	
	#endif
	it3 =	read_i2c(I2C_ADD,0X12);  //读取 12h 寄存器的值 
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"TM_12H:%x \r\n",it3);  //默认值 00000000b （0x00）
	
	#endif
	
	if(!(it1||it2||it3))
	TSM12_Init();
	
	if(it3==0)			//it3 0X0C 寄存器等于0 时 表示 TSM12MC存在  存在 写1 不存在写0
	{
		
		TSM12MC_Status =1;
		
	}
	else
	{
		TSM12MC_Status =0;
	}
	
	TSM12_Init();
	
	TSM_EN_H;
	
	#endif
	

}

/**************************

获取 TSM12_ 键值

***************************/

#define key_num_delay_Timeout     1   //按键复位时间溢出(连续 扫描周期*key_delay_Timeout 未检测到按键触发则清零这个按键)
//#define key_num_out_delay_Timeout     10   //按键复位时间溢出(连续 扫描周期*key_delay_Timeout 未检测到按键触发则清零这个按键)
#define key_num_t1_Timeout  5				//按键触发消抖时间
#define key_num_out_Full		3   //连续触发延迟
uint8_t key_out_Time =0;
uint8_t Touch_key_Num =0;
uint8_t key_num_t[13] = 
{
	
0,0,0,0,
0,0,0,0,
0,0,0,0,0
	
};			//	当前触发的按键复位定时值

uint8_t key_num_t1[13] = 
{
	
0,0,0,0,
0,0,0,0,
0,0,0,0,0
	
};			//	当前触发的按键置位定时值



uint8_t key_num_is[13] = 
{
0,0,0,0,
0,0,0,0,
0,0,0,0,0
};			//	当前已经触发的按键  触发对应按键号位置值1

const uint8_t key_num_s[] = 			//寄存器对应位置"与"运算值
{
	0X03,0X0C,0X30,0XC0
};

const uint8_t key_num_tab1[] = 		//0X10H  寄存器 对应键值 
{
	3,2,1,8
};

const uint8_t key_num_tab2[] = 		//0X11H  寄存器 对应键值 
{
	9,10,11,12
};

const uint8_t key_num_tab3[] = 		//0X12H  寄存器 对应键值 
{
	7,4,5,6
};

#define sizeof_key_num_is  13
#define sizeof_key_num_tab1  4
#define sizeof_key_num_tab2  4
#define sizeof_key_num_tab3  4
#define sizeof_key_num_s 4

uint8_t TSM12_read_key_Num()
{
	
	uint8_t it1,it2,it3,i1,i2;
	uint8_t key_num =0;			//返回的键值
	uint8_t new_key_num =0;		//新增的按键个数
	
//	TSM_EN_L;  // i2c enable
//	
//	nrf_delay_ms(100);
//	
	new_key_num =0;
	key_num =0xff;
	#ifdef Key_By_TSM12c
	
		it1 =	read_i2c(I2C_ADD,0X10);  //读取 10h 寄存器的值 
	
//	#ifdef uart_start
//	
//	SEGGER_RTT_printf(0,"TM_10H:%x \r\n",it1);  //默认值 00000000b （0x00）
//	
//	#endif
		it2 =	read_i2c(I2C_ADD,0X11);  //读取 11h 寄存器的值 
//	
//	#ifdef uart_start
//	
//	SEGGER_RTT_printf(0,"TM_11H:%x \r\n",it2);  //默认值 00000000b （0x00）
//	
//	#endif
		it3 =	read_i2c(I2C_ADD,0X12);  //读取 12h 寄存器的值 
//	
//	#ifdef uart_start
//	
//	SEGGER_RTT_printf(0,"TM_12H:%x \r\n",it3);  //默认值 00000000b （0x00）
//	
//	#endif

	Touch_key_Num =0;		//清空触发计数
	for(uint8_t i =0;i<sizeof(key_num_is);i++)
	{
		if(key_num_is[i]==1)
			Touch_key_Num++;
	}
	
	
	for(i1 =0;i1<sizeof_key_num_is;i1++)			//清空未触发按键的定时值
	{

		if(key_num_is[i1]==0)			//依次判断按键是否触发
		{
			key_num_t[i1] =0;				//未触发按键的对应位置定时清零
			
		}
		else
		{
			key_num_t1[i1] =0;				//已触发按键的对应位置定时清零
			
		}
		
	}
	
	
	for(i1=0;i1<sizeof_key_num_tab1;i1++)			//每组四个按键 判断第一组
	{
				for(i2 =0;i2<sizeof_key_num_s;i2++)
				{
					if(it1&key_num_s[i2])				//10H 寄存器依次做与运算判断触发的值
					{
						//触发时
						if(key_num_is[key_num_tab1[i2]]==1)			//判断刚触发的按键当前是否为触发状态
						{
							//当对应按键触发时继续维持触发状态
							key_num_t[key_num_tab1[i2]] =0;			//清空定时状态
							key_num_t1[key_num_tab1[i2]] =0;			//清空定时状态
							
						}
						else if(Touch_key_Num==0)			//当前状态未触发时
						{
							
							key_num_t1[key_num_tab1[i2]]++;			//触发延迟消抖
							if(key_num_t1[key_num_tab1[i2]]>key_num_t1_Timeout)
							{
									key_num_is[key_num_tab1[i2]] =1;
									key_num = key_num_tab1[i2];				//对 key_num 赋值
									new_key_num++;			//新增触发按键+1
							}

							
						}
					}
					else
					{
						//未触发时
						if((key_num_is[key_num_tab1[i2]]==1))				//判断未触发的按键当前是否为触发状态
						{
							//当对应按键触发时，开始计时
							key_num_t[key_num_tab1[i2]]++;
							
							if(key_num_t[key_num_tab1[i2]]>key_num_delay_Timeout		//当未触发时间超时时
							)
							{
								key_num_t[key_num_tab1[i2]]=0;			//清空定时值
								key_num_is[key_num_tab1[i2]] =0;			//清空触发状态
								
							}
							
						}
						else
						{
							
							//当对应按键未触发时，不运作
						
						}
					}
					
				}
	}


		for(i1=0;i1<sizeof_key_num_tab2;i1++)			//每组四个按键 判断第二组
	{
				for(i2 =0;i2<sizeof_key_num_s;i2++)
				{
					if(it2&key_num_s[i2])				//10H 寄存器依次做与运算判断触发的值
					{
						//触发时
						if(key_num_is[key_num_tab2[i2]]==1)			//判断刚触发的按键当前是否为触发状态
						{
							//当对应按键触发时继续维持触发状态
							key_num_t[key_num_tab2[i2]] =0;			//清空定时状态
							key_num_t1[key_num_tab2[i2]] =0;			//清空定时状态
						}
						else	if(Touch_key_Num==0)		//当前状态未触发时
						{
						
							key_num_t1[key_num_tab2[i2]]++;			//触发延迟消抖
							if(key_num_t1[key_num_tab2[i2]]>key_num_t1_Timeout)
							{
									key_num_is[key_num_tab2[i2]] =1;
									key_num = key_num_tab2[i2];				//对 key_num 赋值
									new_key_num++;			//新增触发按键+1
							}
						}
					}
					else
					{
						//未触发时
						if((key_num_is[key_num_tab2[i2]]==1))				//判断未触发的按键当前是否为触发状态
						{
							//当对应按键触发时，开始计时
							key_num_t[key_num_tab2[i2]]++;
							
							if(key_num_t[key_num_tab2[i2]]>key_num_delay_Timeout		//当未触发时间超时时
							)
							{
								key_num_t[key_num_tab2[i2]]=0;			//清空定时值
								key_num_is[key_num_tab2[i2]] =0;			//清空触发状态
								
							}
							
						}
						else
						{
							//当对应按键未触发时，不运作
						}
					}
					
				}
	}

	
		for(i1=0;i1<sizeof_key_num_tab3;i1++)			//每组四个按键 判断第三组
	{
				for(i2 =0;i2<sizeof_key_num_s;i2++)
				{
					if(it3&key_num_s[i2])				//10H 寄存器依次做与运算判断触发的值
					{
						//触发时
						if(key_num_is[key_num_tab3[i2]]==1)			//判断刚触发的按键当前是否为触发状态
						{
							//当对应按键触发时继续维持触发状态
									key_num_t[key_num_tab3[i2]] =0;			//清空定时状态
									key_num_t1[key_num_tab3[i2]] =0;			//清空定时状态
						}
						else	if(Touch_key_Num==0)		//当前状态未触发时
						{
						
							key_num_t1[key_num_tab3[i2]]++;			//触发延迟消抖
							if(key_num_t1[key_num_tab3[i2]]>key_num_t1_Timeout)
							{
									key_num_is[key_num_tab3[i2]] =1;
									key_num = key_num_tab3[i2];				//对 key_num 赋值
									new_key_num++;			//新增触发按键+1
							}
						}
					}
					else
					{
						//未触发时
						if((key_num_is[key_num_tab3[i2]]==1))				//判断未触发的按键当前是否为触发状态
						{
							//当对应按键触发时，开始计时
							key_num_t[key_num_tab3[i2]]++;
							
							if(key_num_t[key_num_tab3[i2]]>key_num_delay_Timeout		//当未触发时间超时时
							)
							{
								key_num_t[key_num_tab3[i2]]=0;			//清空定时值
								key_num_is[key_num_tab3[i2]] =0;			//清空触发状态
							}
							
						}
						else
						{
							//当对应按键未触发时，不运作
						}
					}
					
				}
	}

		



	#endif
	
//	
	if(key_num!=0xff)  //触发一次清空唤醒时间
		MPR121_T =0;
	
	if(key_out_Time<key_num_out_Full)
	{
		key_num =0xFF;   //把过快的连续不同按键抹掉
	}
	
	if(key_num!=0XFF)			//触发一次 清空
	key_out_Time =0;
	else if(key_out_Time<key_num_out_Full)
	key_out_Time++;
	
	Touch_key_Num =0;		//清空触发计数
	for(uint8_t i =0;i<sizeof(key_num_is);i++)
	{
		if(key_num_is[i]==1)
			Touch_key_Num++;
	}
	
	if(Touch_key_Num>1)				//不允许一个以上有效触发
		key_num =0xFF;
	
	return key_num;

}






