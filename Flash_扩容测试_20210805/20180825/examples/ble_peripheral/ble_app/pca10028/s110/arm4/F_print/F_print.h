#ifndef __F_print_H_
#define __F_print_H_
#include "Devic_Init.h"  //设备初始化
// 指纹头数据接口通信数据包

/*
数据包格式：

发送包：

数据包分三类：
包标识 = 01: 命令包
包标识 = 02: 数据包，且有后续包 
包标识 = 08: 最后一个数据包，即结束包

所有的数据包都要加包头： 0xEF01.

校验和是从包标识至校验和之间所有字节之和，超出 2 字节的进位忽略。 

包长度 = 包长度至校验和（指令、参数或数据）的总字节数，包含校验和，但不包含包长 度本身的字节数。 

01 命令包格式： 

0XEF01 + 0XFFFFFFFF + 1字节包标识(0x01) + 2字节包长度 
+ 1字节指令 + N字节参数 + 2byte 校验包

02 数据包格式：
0XEF01 + 0XFFFFFFFF + 1字节包标识(0X02) + 
2字节包长度 + N 字节数据 + 2字节校验包

08 数据包格式：
0XEF01 + 0XFFFFFFFF + 1字节包标识(0X08) + 
2字节包长度 + N 字节数据 + 2字节校验包

数据包不能单独进入执行流程，必须跟在指令包或应答包后面。 

下传或上传的数据包格式相同。 

包长度 = 包长度至校验和（指令、参数或数据）的总字节数，包含校验和，但不包含包长 度本身的字节数。 

校验和是从包标识至校验和之间所有字节之和，超出 2 字节的进位忽略。 

芯片地址在没有生成之前为缺省的 0xffffffff，一旦上位机通过指令生成了芯片地址，则所有 的数据包都必须按照生成的地址收发。芯片将拒绝地址错误的数据包。 

对于多字节的高字节在前低字节在后（如 2bytes 的 00 06 表示 0006，而不是 0600）。 

指令应答：

应答是将有关命令执行情况与结果上报给上位机，应答包含有参数，并可跟后续数据包。
上位机只 有在收到 SOC 的应答包后才能确认 SOC 收包情况与指令执行情况。 

应答包格式：

0XEF01 + 0XFFFFFFFF + 包标识(0x07) + 
2 字节包长度 + 确认码 (1字节) + 返回参数（N字节）+ 2字节校验包

 确认码定义： 
00H：表示指令执行完毕或 OK； 
01H：表示数据包接收错误； 
02H：表示传感器上没有手指； 
03H：表示录入指纹图像失败； 
04H：表示指纹图像太干、太淡而生不成特征； 
05H：表示指纹图像太湿、太糊而生不成特征； 
06H：表示指纹图像太乱而生不成特征； 
07H：表示指纹图像正常，但特征点太少（或面积太小）而生不成特征； 
08H：表示指纹不匹配； 
09H：表示没搜索到指纹； 
0aH：表示特征合并失败； 
0bH：表示访问指纹库时地址序号超出指纹库范围； 
0cH：表示从指纹库读模板出错或无效； 
0dH：表示上传特征失败； 
0eH：表示模块不能接收后续数据包； 
0fH：表示上传图像失败； 
10H：表示删除模板失败； 
11H：表示清空指纹库失败； 
12H：表示不能进入低功耗状态； 
13H：表示口令不正确； 
14H：表示系统复位失败； 
15H：表示缓冲区内没有有效原始图而生不成图像； 
16H：表示在线升级失败； 
17H：表示残留指纹或两次采集之间手指没有移动过； 
18H：表示读写 FLASH 出错； 
f0H：有后续数据包的指令，正确接收后用 0xf0 应答； 
f1H：有后续数据包的指令，命令包用 0xf1 应答； 
f2H：表示烧写内部 FLASH 时，校验和错误；  
f3H：表示烧写内部 FLASH 时，包标识错误；  
f4H：表示烧写内部 FLASH 时，包长度错误； 
f5H：表示烧写内部 FLASH 时，代码长度太长； 
f6H：表示烧写内部 FLASH 时，烧写 FLASH 失败； 
19H：随机数生成失败； 
1aH：无效寄存器号； 
1bH：寄存器设定内容错误号； 
1cH：记事本页码指定错误； 
1dH：端口操作失败； 
1eH：自动注册（enroll）失败； 
1fH：指纹库满； 
20H：设备地址错误； 
21H：密码有误； 
22 H：指纹模板非空； 
23 H：指纹模板为空； 
24 H：指纹库为空； 
25 H：录入次数设置错误； 
26 H：超时； 
27 H：指纹已存在； 
28 H：指纹模板有关联； 
29 H：传感器初始化失败； 
2AH—efH：Reserved。 


*/

#define F_Print_IRQ			27

extern uint8_t Input_Ps_Lmage_Type;			//录入指纹类型(管理员/用户)
extern uint8_t Input_Ps_Source;					//录入指纹来源（来自蓝牙或其他）
extern uint8_t Test_Ps_Mark;						//检测指纹头标志
extern uint32_t Start_PS_Unix;					//录入指纹生效时间
extern uint32_t Stop_PS_Unix;						//录入指纹失效时间
extern uint32_t F_print_Enable;			//在初始化时检测是否存在指纹头
extern uint8_t Input_Ps_Lmage_Run_Now;			//录入指纹运行的状态机

#define Admin_Ps_Lmage	1
#define User_Ps_Lmage		2

extern uint8_t BLE_LINK;  //BLE连接状态实时更新

extern const uint8_t PS_Getlmage[12];  //发送指令，用于读取指纹图像

extern const uint8_t Null_Getlmage[12];  //应答指令，未读取到指纹(无指纹按下)

extern const uint8_t Err_Getlmage[12];  //应答指令，特征点太少，失败，丢弃本次按压数据

extern const uint8_t Be_Getlmage[12]; //应答指令，获取到指纹 （有指纹按下）

extern const uint8_t Get_Dat_Save_1[13]; //生成特征，存储在缓存区 01

extern const uint8_t Get_Dat_Save_2[13]; //生成特征，存储在缓存区 02

extern const uint8_t Get_Dat_Save_3[13]; //生成特征，存储在缓存区 03

extern const uint8_t Get_Dat_Save_4[13]; // 生成特征，存储在缓存区 04
	
extern const uint8_t Get_Dat_Save_5[13]; // 生成特征，存储在缓存区 05

extern const uint8_t Get_Dat_Save_6[13]; // 生成特征，存储在缓存区  06

extern const uint8_t Merge_Lmage_Dat[12]; //合并 获取到的指纹特征数据

extern const uint8_t Ps_Save_ID[15];	//保存指纹到指定ID号

extern const uint8_t Se_Lmage_Dat[17];					   //搜索本地是否存在类似指纹

extern const uint8_t Not_Se_Lmage[16]; 	//本地未搜索到匹配指纹

extern const uint8_t Se_Lmarge[16];			//搜索到指纹

extern const uint8_t PS_Del_A_Lmage[16];  //删除一个指纹指令

extern const uint8_t PS_Del_A_Lmage_ACK[12];  //单个指纹删除成功应答

extern const uint8_t PS_Del_All_Lmage[12];	 // 删除所有指纹指令

extern const uint8_t PS_Del_Lmage_ACK[12];		 //删除所有指纹完成 应答

extern const uint8_t PS_Read_Lmage_Num[12];		 //读取保存的指纹数量

extern const uint8_t PS_Lmage_Num_ACK[14];		 	//应答的指纹数

extern const uint8_t PS_HandShake[12];						//与指纹模块握手

extern const uint8_t PS_HandShake_ACK[12]; 			//指纹模块握手应答

extern const uint8_t PS_PowDlow[12]; 					//指纹休眠指令

extern const uint8_t PS_PowDlow_ACK[12]; 			//指纹休眠指令应答

extern uint8_t Ps_Data_Tab[50];								//指纹头串口数据缓存

extern const uint8_t PS_LED_CFG[16];					// 指纹头 LED 配置指令

extern const uint8_t PS_LED_POWER[16]; 		//开启 指纹头 灯 电源

extern const uint8_t  PS_LED_R1[16];								//指纹头 LED 红色 第一条指令

extern const uint8_t  PS_LED_R2[16];								//指纹头 LED 红色 第二条指令

extern const uint8_t PS_LED_G1[16];		//指纹头 LED 绿色 第一条指令

extern const uint8_t PS_LED_G2[16];		//指纹头 LED 绿色 第二条指令

extern const uint8_t PS_LED_B1[16];			//指纹头 LED 蓝色 第一条指令

extern const uint8_t PS_LED_B2[16];			//指纹头 LED 蓝色 第二条指令

extern const uint8_t PS_LED_W1[16];     //指纹头 LED 白色  第一条指令

extern const uint8_t PS_LED_W2[16];     //指纹头 LED 白色  第二条指令

extern const uint8_t PS_LED_Sleep[16];		//使LED 驱动芯片进入  低功耗模式

extern uint8_t Controllable_Power_State;		//指纹头电源状态

extern uint8_t UART_RUN;   //用于表示串口开启状态

extern uint8_t F_Print_Run_Flag;				//指纹头运行标志位

extern uint16_t CL_PS_ID;			//设定 要删除的 指纹 ID  
extern uint16_t CL_PS_Run;				//设定 删除指纹头运行

extern uint8_t CL_PS_Print_Now;


extern const uint8_t CL_PS_Run_Del_Nor_Mode;					//删除 指纹ID 功能挂起
extern const uint8_t CL_PS_Run_Del_Single_Mode;				//删除单个 指纹ID 的模式
extern const uint8_t CL_PS_Run_Del_All_Mode;					//删除所有 指纹ID 的模式
extern const uint8_t CL_PS_Run_Del_Batch_Mode;				//批量删除 指纹ID 的模式




void Controllable_Power_No(void); //开启指纹MCU电源
void Controllable_Power_Off(void);		//关闭指纹MCU电源



void Input_PS_Lmage(void);					//录入指纹

void PS_Data_Cheak(uint8_t *s,uint8_t len);			//在串口包中获取指纹数据包
void PS_Input_Order_Run(uint8_t *s,uint8_t len);			//指纹头返回数据指令判断
uint8_t Input_PS_Lmage_Run(uint32_t Lmage_Start_Unix,uint32_t Lmage_Stop_Unix,uint32_t Ps_ID);			//运行指纹录入功能

uint8_t F_print_Goto_Sleep(void);			//关闭指纹模块电源

uint8_t CL_Ps_Print(void);		//清空指纹头数据


void PS_LED_Run(uint8_t LED_CR);					// 指纹头 LED 设置

void PS_LED_Shutdown(void);								 // 

#endif
