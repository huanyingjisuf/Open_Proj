#include <F_print.h>
#include <Uart_Run.h>   // 串口


// 指纹头数据接口通信数据包
 
const uint8_t PS_Getlmage[] =  //发送指令，用于读取指纹图像
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X01,0X00,0X05
};

const uint8_t Null_Getlmage[] =  //应答指令，未读取到指纹(无指纹按下)
{
	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x03,0x02,0x00,0x0C
};

const uint8_t Err_Getlmage[] =  //应答指令，特征点太少，失败，丢弃本次按压数据
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X07,0X00,0X11
};

const uint8_t Be_Getlmage[] = //应答指令，获取到指纹 （有指纹按下）
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X00,0X00,0X0A
};

const uint8_t Get_Dat_Save_1[] = //生成特征，存储在缓存区 01
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X01,0X00,0X08
};

const uint8_t Get_Dat_Save_2[] = //生成特征，存储在缓存区 02
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X02,0X00,0X09
};

const uint8_t Get_Dat_Save_3[] = //生成特征，存储在缓存区 03
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X03,0X00,0X0A
};

const uint8_t Get_Dat_Save_4[] = // 生成特征，存储在缓存区 04
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X04,0X00,0X0B
};

const uint8_t Get_Dat_Save_5[] = // 生成特征，存储在缓存区 05
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X05,0X00,0X0C
};

const uint8_t Get_Dat_Save_6[] = // 生成特征，存储在缓存区  06
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X06,0X00,0X0D
};

const uint8_t Merge_Lmage_Dat[] = //合并 获取到的指纹特征数据
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X05,0X00,0X09
};

const uint8_t Ps_Save_ID[] =	//保存指纹到指定ID号
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X06,
	0X06,0X01,
	0X00,0X03,		//保存到  ID3  (可修改，从0开始)
	0X00,0X11
};
const uint8_t Se_Lmage_Dat[] =   //搜索本地是否存在类似指纹
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X08,
	0X04,0X01,
	0x00,0x00,
	0x00,0x64,
	0x00,0x72
};

const uint8_t Not_Se_Lmage[] = 	//本地未搜索到匹配指纹
{
	
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,
	0X00,0X07,
	0X09,
	0X00,0X01,
	0X00,0X01,
	0X00,0X19
	
};

const uint8_t Se_Lmarge[] = //搜索到指纹
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,							//包标识
	0X00,0X07,				//包长度
	0X00,						//确认码
	0X00,0X00,		//指纹 ID 号
	0X00,0X00,		//匹配度
	0X00,0X00    //校验码
};

const uint8_t PS_Del_A_Lmage[] =  //删除一个指纹指令
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,			//包标识
	0X00,0X07,			//包长度
	0X0C,						//指令
	0X00,0X07,		//要删除的  指纹 ID
	0X00,0X01,		
	0X00,0X1C			//校验码
};

const uint8_t PS_Del_A_Lmage_ACK[] =  //单个指纹删除成功应答
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,				//包标识
	0X00,0X03,
	0X00,				//确认码
	0X00,0X0A		//校验码
};

const uint8_t PS_Del_All_Lmage[] = // 删除所有指纹指令
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X03,		//包长度
	0x0D,						//指令码
	0x00,0x11				//校验码
};

const uint8_t PS_Del_Lmage_ACK[] = //删除所有指纹完成 应答
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0x07,
	0x00,0x03,
	0x00,
	0x00,0x0A
};

const uint8_t PS_Read_Lmage_Num[] = //读取保存的指纹数量
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X03,
	0X1D,			//指令
	0X00,0X21
};

const uint8_t PS_Lmage_Num_ACK[] = 	//应答的指纹数
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,
	0X00,0X05,		//		数据包长度
	0X00,					//		确认码
	0X00,0X06,		//   获取到的指纹数量
	0X00,0X12			//	校验码
};

const uint8_t PS_HandShake[]=  						//与指纹模块握手
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X03,			//长度
	0X35,						//指令码
	0X00,0X39				//校验码
};

const uint8_t PS_HandShake_ACK[]= 			//指纹模块握手应答
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,						//报标识
	0X00,0X03,			//	长度
	0x00,					//确认码
	0x00,0X0A			//	校验码
};

const uint8_t PS_PowDlow[] = 					//指纹休眠指令
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,					//包标识
	0X00,0X03,		//包长度
	0X33,					//指令码
	0X00,0X37			//校验码
};

const uint8_t PS_PowDlow_ACK[] = 			//指纹休眠指令应答
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,									//包标识
	0X00,0X03,						//包长度
	0X00,									//确认码
	0X00,0X0A							// 校验码
};

const uint8_t PS_LED_CFG[] = 		// 指纹头 LED 配置指令
{

	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,			//包标识
	0x00,0x07,			//包长度
	0x40,0x60,0x04,0x00,0x00,
	0x00,0xAC
	
};

const uint8_t PS_LED_POWER[] = 		//开启 指纹头 灯 电源
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X000,0X00,0X00,
	0X00,0XA8
	
};

const uint8_t  PS_LED_R1[]=    //指纹头 LED 红色 第一条指令
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X06,0X80,0X00,
	0X01,0X2E 
	
};

const uint8_t  PS_LED_R2[]=    //指纹头 LED 红色 第二条指令
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X04,0X01,0X00,
	0X00,0XAD
	
};

const uint8_t PS_LED_G1[]=		//指纹头 LED 绿色 第一条指令
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X07,0X80,0X00,
	0X01,0X2F
	
};

const uint8_t PS_LED_G2[]=		//指纹头 LED 绿色 第二条指令
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X04,0X04,0X00,
	0X00,0XB0
	
};

const uint8_t PS_LED_B1[]=			//指纹头 LED 蓝色 第一条指令
{
	
	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,0x40,0x60,0x08,0x80,0x00,
	0x01,0x30
	
};

const uint8_t PS_LED_B2[]=			//指纹头 LED 蓝色 第二条指令
{
	
	0xEF,01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,
	0x40,0x60,0x04,0x10,0x00,
	0x00,0xBC
	
};

const uint8_t PS_LED_W1[]=     //指纹头 LED 白色  第一条指令
{
	
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X09,0X80,0X00,
	0X01,0X31
	
};

const uint8_t PS_LED_W2[]=			//指纹头 LED 白色  第二条指令
{

	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,
	0x40,0x60,0x04,0x40,0x00,
	0x00,0xEC

};

const uint8_t PS_LED_Sleep[] = 		//使LED 驱动芯片进入  低功耗模式
{

	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,
	0x40,0x60,0x00,0x08,0x00,
	0x00,0xB0 

};



#define Controllable_Power  28

uint8_t F_Print_Run_Flag =0;			//指纹头运行标志位

uint8_t Controllable_Power_State =0;

void Controllable_Power_No() //开启指纹MCU电源
{
	
	nrf_delay_ms(100);
	nrf_gpio_cfg_output(Controllable_Power);
	nrf_gpio_pin_clear(Controllable_Power);
	Controllable_Power_State =1;
	
	if(Test_Ps_Mark==0)	
		PS_LED_Run(3);    //上电亮蓝灯
	
	
}


//关闭指纹MCU电源



void Controllable_Power_Off()
{

	nrf_gpio_pin_set(Controllable_Power);
	Controllable_Power_State =0;
  		F_Print_Run_Flag =0;
		Test_Ps_Mark =0;
	
}



//在指针中抓取指纹返回的数据包

#define Cheak_Initio_Len   6			//校验数据头长度

void PS_Data_Cheak(uint8_t *s,uint8_t len)			
{
	
	uint8_t i;
	uint8_t tab[50];
	uint8_t *s1;
	
	uint16_t Sum_Cheak,Sum_Cheak1;			//	校验和
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart PS_Ddata_Cheak!\r\n");
	
	SEGGER_RTT_printf(0,"len: %x\r\n",len);
	
	#endif
	
	Sum_Cheak =0;
	
	for(i =0;i<(len-Cheak_Initio_Len);i++)
	{
		s1 =s;					//起点
		s1+=i;
		
		for(uint8_t is =0;is<Cheak_Initio_Len;is++)
		{
			
			tab[is] = *s1;
			s1++;
		}
		if(
			(tab[0]==0xEF)&&(tab[1]==0x01)&&
			(tab[2]==0xFF)&&(tab[3]==0xFF)&&
			(tab[4]==0xFF)&&(tab[5]==0xFF)
			)
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Data_Cheak_OK!\r\n");
			
			#endif
			
			i = len;			//结束校验开头循环 开始下一步对比
			
			break;			//	校验成功结束循环
		}
		
		if(i>(len-Cheak_Initio_Len-1))
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"i: %d!\r\n",i);		//	输出i 的值
			
			SEGGER_RTT_printf(0,"Ps_Data_Cheak_Error!\r\n");			//	提示数据包校验错误
			
			#endif
			
			return;				//	校验错误时，结束程序
			
			
		}
		
	}
	
	//继续加入数据
			tab[6] = *s1;
			s1++;
			
			tab[7] = *s1;
			s1++;
			
			tab[8] = *s1;			//数据包长度
			s1++;
			
			
	
			if((tab[7]==0)&&(tab[8]<(sizeof(tab)-Cheak_Initio_Len-3)))		//抛掉高字节，限制字节长度
			{
				
				for(i =0;i<tab[8];i++)			//将剩下的数据载入到缓存
				{
					tab[9+i] = *s1;
					s1++;
				}
				//当所有数据都保存到缓存时
				
				Sum_Cheak =0;
				
				for(i =0;i<(tab[8]+1);i++)
				{
					
					Sum_Cheak +=tab[6+i];
					
				}
				
				Sum_Cheak1 = tab[Cheak_Initio_Len+1+tab[8]];
				Sum_Cheak1<<=8;
				Sum_Cheak1+=tab[Cheak_Initio_Len+2+tab[8]];			//将校验码左移
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Sum_Cheak1: %x. \r\n",Sum_Cheak1);
				
				#endif
				
				if(Sum_Cheak==Sum_Cheak1)     // 当校验和合理时
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Sum_Cheak_succeed!\r\n");			//	校验成功
					
					#endif
					
				}
				else
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Sum_Cheak_Error!\r\n");		//校验失败
					
					#endif
					
					return;		//	校验失败时结束操作
				}
				
				//当获取到合理的数据包时
						
				
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nPS_Data_Cheak:\r\n");			//输出获取到的指纹数据包
				
				for(i =0;i<(Cheak_Initio_Len+3+tab[8]);i++)		//	输出数据
				{
					
					SEGGER_RTT_printf(0," %x ",tab[i]);
					
				}
				
				SEGGER_RTT_printf(0,"\r\n");
				
				#endif
			
				PS_Input_Order_Run(tab,(Cheak_Initio_Len+3+tab[8]));			//将获取到的数据包载入指令判断
		
			
			}
			else
			{
				
				#ifdef uart_start
				
		
				SEGGER_RTT_printf(0,"PS_Data_Len_Error!\r\n");			//长度异常，结束本次判断
				
				#endif
				
				return;
				
			}
		
}

/*
指纹头返回数据指令判断
输入： *s  数据包指针
输入： len	数据包长度
*/

uint8_t Ps_Data_Tab[50];		//指纹头串口数据缓存

void PS_Input_Order_Run(uint8_t *s,uint8_t len)
{
	uint8_t tab[50];
	uint8_t i;
	
	#ifdef uart_start

	SEGGER_RTT_printf(0,"\r\nStart_PS_Input_Order_Run!\r\n"); //开始运行应答数据包指令判断
	
	#endif
	
	for(i =0;i<len;i++)			//将上级校验得到的数据包载入本地缓存
	{
		tab[i] = *s;
		s++;
	}
	
	if(tab[6]==0x07)			//判断包标识 是否为应答	
	{
		;
	}
	else
	{
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Tab_6_Error! Value: %x.\r\n",tab[6]);		//提示确认码异常
		
		#endif
		return;
	}
	if((tab[9]==0)&&(Test_Ps_Mark==1))		//当确认码合理且在测试指纹头是否存在的状态，进入这条程序
	{
		
		F_print_Enable =1;
		Test_Ps_Mark =0;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nF_printf_Enable_1!\r\n");		//检测到并使能第一种指纹模组
		
		#endif
		
		Controllable_Power_Off();		//检测到指纹模块即关闭电源
		
		return;
		
	}
	
	for(i =0;i<50;i++)
	{
		 Ps_Data_Tab[i] =0;		//清空缓存
	}
	
	for(i =0;i<len;i++)			//将上级校验得到的数据包载入本地缓存
	{
		Ps_Data_Tab[i] = tab[i];			//把数据放到外部缓存，方便其他函数调用
	}
	
	
	switch(tab[9])			//判断应答包确认码
	{
		
		case 0: 	//00H：表示指令执行完毕或 OK； 
			break;
		
		case 1:		//01H：表示数据包接收错误； 
			break;
		
		case 2:		//02H：表示传感器上没有手指； 
			break;
		
		case 3:		//03H：表示录入指纹图像失败； 
			break;
		
		case 4:		//04H：表示指纹图像太干、太淡而生不成特征； 
			break;
		
		case 5:		//05H：表示指纹图像太湿、太糊而生不成特征； 
			break;
		
		case 6:		//06H：表示指纹图像太乱而生不成特征； 
			break;
		
		case 7:		//07H：表示指纹图像正常，但特征点太少（或面积太小）而生不成特征； 
			break;
		
		case 8:		//08H：表示指纹不匹配； 
			break;
		
		case 9:		//09H：表示没搜索到指纹； 
			break;
		
		case 10:	//0aH：表示特征合并失败； 
			break;
		
		case 11:  //0bH：表示访问指纹库时地址序号超出指纹库范围； 
			break;
		
		case 12:	//0cH：表示从指纹库读模板出错或无效； 
			break;
		
		case 13:	//0dH：表示上传特征失败； 
			break;
		
		case 14:	//0eH：表示模块不能接收后续数据包； 
			break;
		
		case 15:	//0fH：表示上传图像失败；
			break;
		
		case 16:	//10H：表示删除模板失败； 
			break;
		
		case 17:	//11H：表示清空指纹库失败； 
			break;
		
		case 18:	//12H：表示不能进入低功耗状态； 
			break;
		
		case 19:	//13H：表示口令不正确； 
			break;
		
		case 20:	//14H：表示系统复位失败； 
			break;
		
		case 21:	//15H：表示缓冲区内没有有效原始图而生不成图像；
			break;
		
		case 22:	//16H：表示在线升级失败；
			break;
		
		case 23:	//17H：表示残留指纹或两次采集之间手指没有移动过； 
			break;
		
		case 24:	//18H：表示读写 FLASH 出错； 
				break;
		
		case 0XF0:	//f0H：有后续数据包的指令，正确接收后用 0xf0 应答； 
				break;
		
		case 0XF1:  //f1H：有后续数据包的指令，命令包用 0xf1 应答； 
				break;
		
		case 0XF2:	//f2H：表示烧写内部 FLASH 时，校验和错误；  
				break;
		
		case 0XF3:	//f3H：表示烧写内部 FLASH 时，包标识错误； 
				break;
		
		case 0XF4:	//f4H：表示烧写内部 FLASH 时，包长度错误； 
				break;
		
		case 0XF5:	//f5H：表示烧写内部 FLASH 时，代码长度太长； 
				break;
		
		case 0XF6:	//f6H：表示烧写内部 FLASH 时，烧写 FLASH 失败； 
				break;
		
		case 25:		//19H：随机数生成失败； 
				break;
		
		case 26:		//1aH：无效寄存器号； 
				break;
		
		case 27:		//1bH：寄存器设定内容错误号； 
				break;
		
		case 28:		//1cH：记事本页码指定错误； 
				break;
		
		case 29:		//1dH：端口操作失败； 
				break;
		
		case 30:		//1eH：自动注册（enroll）失败； 
				break;
		
		case 31:		//1fH：指纹库满； 
				break;
		
		case 32:		//20H：设备地址错误； 
				break;
		
		case 33:		//21H：密码有误； 
				break;
		
		case 34:		//22 H：指纹模板非空；
				break;
		
		case 35:		//23 H：指纹模板为空； 
				break;
		
		case 36:		//24 H：指纹库为空； 
				break;
		
		case 37:		//25 H：录入次数设置错误；
				break;
		
		case 38:		//26 H：超时；
				break;
		
		case 39:		//27 H：指纹已存在；
				break;
		
		case 40:		//28 H：指纹模板有关联； 
				break;
		
		case 41:		//29 H：传感器初始化失败；
				break;
		
		
		
	}
	
	
}

//录入指纹函数
/******************

输入： Ps_Lmage		指纹类型
输入： Start_Unix	开始时间
输入： Stop_Unix	结束时间


*******************/

uint8_t Input_Ps_Lmage_Type;
uint8_t Input_Ps_Source;
uint8_t Test_Ps_Mark =0;			//检测指纹头是否存在时的标志

uint32_t Start_PS_Unix;
uint32_t Stop_PS_Unix;
uint32_t F_print_Enable =0;			//在初始化时检测是否存在指纹头


#define Admin_Ps_Lmage	1
#define User_Ps_Lmage		2

void Input_PS_Lmage()
{
	
	if(F_print_Enable==0)		//当不存在指纹头时，清空指纹录入指令数据
	{
		
		Input_Ps_Lmage_Type =0;	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nF_print_Disable!\r\n");			//提示指纹头未连接
		
		#endif
		
	}
	

	
	if(Input_Ps_Lmage_Type!=0)			//当检测到指纹录入指令时
	{
		
		switch(Input_Ps_Lmage_Type)
		{
			case Admin_Ps_Lmage:		//管理员指纹
				
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Input_Admin_Ps_Lmage!\r\n");		//录入管理员指纹
			
			#endif
			
			if(Input_PS_Lmage_Run(Start_PS_Unix,Stop_PS_Unix,0)==0)
			{
				;
			}
			
				break;
			case User_Ps_Lmage:			//用户指纹
				
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Input_User_Ps_Lmage!\r\n");		//录入管理员指纹	
			
			#endif
			
			
			if(Input_PS_Lmage_Run(Start_PS_Unix,Stop_PS_Unix,0)==0)
			{
				;
			}
			
			
				break;
			
			default :
				Input_Ps_Lmage_Type =0;			//其他情况下清空
				Start_PS_Unix =0;
				Stop_PS_Unix =0;
			
				break;
			
		}
		
	}
}


/*************************************************************

uint8_t F_print_Goto_Sleep()

使指纹头进入睡眠状态

成功 返回1 失败 返回0

***************************************************************/

uint8_t F_Print_Goto_Sleep_Now =0;			//定义进入睡眠状态的状态机
uint32_t FP_Goto_Sleep_T =0;					//定义睡眠模式需要花费的时间
const uint32_t FP_Goto_Sleep_Out =50;				//定义超时  
uint8_t PS_Goto_Sleep_Num =0;				//待机尝试次数
uint8_t F_print_Goto_Sleep()
{
	uint8_t TX_BUFF[64];			//发送缓存
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"F_Print_Goto_Sleep_Now!\r\n");
	
	#endif
	
	switch (F_Print_Goto_Sleep_Now)
	{
		case 0:
		
		//PS_LED_Shutdown();			//先关闭 LED 模组
		
		for (uint32_t i =0;i<sizeof(PS_PowDlow);i++)			//在发送缓存区写入睡眠指令
		{
			TX_BUFF[i] = PS_PowDlow[i];
		}
		
		Uart_Put_String(TX_BUFF,sizeof(PS_PowDlow));				
		F_Print_Goto_Sleep_Now++;
		FP_Goto_Sleep_T =0;
		for(uint32_t i =0;i<sizeof(Ps_Data_Tab);i++)
		{
			Ps_Data_Tab[i] =0;
		}
		
			break;
		case 1: 
			
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
					(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)				//当收到的数据包为进入睡眠状态时操作成功
				)					//当获得的数据
			{
				
				F_Print_Goto_Sleep_Now =0;
				
				#ifdef uart_start
	
				SEGGER_RTT_printf(0,"F_Print_Goto_Sleep_OK!\r\n");
				
				#endif
				PS_Goto_Sleep_Num =0;			//次数清零
				return 1;
			}
			else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
			{
				F_Print_Goto_Sleep_Now =0;			//进入失败  重新发送
			}
			FP_Goto_Sleep_T++;
			
			if(FP_Goto_Sleep_T>FP_Goto_Sleep_Out)				//超时重新发送
			{
				FP_Goto_Sleep_T =0;
				F_Print_Goto_Sleep_Now =0;
				PS_Goto_Sleep_Num++;
			}
			break;
		
	}
	
	if(PS_Goto_Sleep_Num>=5)
	{
		PS_Goto_Sleep_Num =0;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_Goto_Sleep_Num_Out!\r\n");
		
		#endif
		return 1;
	}
	
	return 0;
}


/****************************************************************

运行指纹录入功能

返回： 是否录入成功 失败则返回0，成功则返回录入时获取到的指纹 ID

输入：Lmage_Start_Unix 起始时间
			Lmage_Stop_Unix  结束时间
			Ps_ID						 给新指纹分配的ID号


******************************************************************/

uint8_t Input_Ps_Lmage_Run_Now =0;
uint16_t Input_Ps_Lmage_ID;		//获取到的新指纹 ID
uint16_t input_ps_T =0;		//超时标志
const uint16_t input_ps_T_Out =30;		//超时周期数
	
uint8_t Input_PS_Lmage_Run(uint32_t Lmage_Start_Unix,uint32_t Lmage_Stop_Unix,uint32_t Ps_ID)
{
	
	uint8_t TX_BUFF[64];			//发送缓存
	uint32_t i;		//循环
	uint16_t ps_buff_chak;
//	uint16_t Sum_cheak;		//校验和数据
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Input_PS_now: %x!\r\n",Input_Ps_Lmage_Run_Now);
	
	#endif
	
	if((BLE_LINK==0)||(Lock_State==1))		//当蓝牙连接断开时或者锁未关闭时停止指纹录入过程
	{
		
	//	input_speak(10);		//提示操作失败
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_Input_Run_BLE_Disconnect!\r\n");
		
		#endif
		
		Input_Ps_Lmage_Run_Now =22;
		
	}
	
	
	switch(Input_Ps_Lmage_Run_Now)
	{
		case 0:
			
			Controllable_Power_No();	//给指纹头上电
			Input_Ps_Lmage_ID = Need_Of_Ps_Print_Data_IDLE_ID();
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Input_Ps_Lmage_ID: %x\r\n",Input_Ps_Lmage_ID);
		
				#endif
		
				if(Input_Ps_Lmage_ID<=PS_Print_Num)			//当指纹ID 合理时 允许继续
					Input_Ps_Lmage_Run_Now++;
				else
					Input_Ps_Lmage_Run_Now = 0xFF;
				
		break;
		
		case 1:		//发送读取指纹的指令
			for(i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
			
			break;
		
		case 2:			//确认指纹是否按下
		
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，回到上一步
			)					//当获得的数据
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage!\r\n");		//提示手指抬起
			
			#endif
			
			Input_Ps_Lmage_Run_Now --;
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//当收到的数据包为手指按下时，进入上一步
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage!\r\n\r\n");		//提示手指未抬起获取到指纹
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//收到其他包时推到上一步
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data!\r\n");		//提示其他报文的情况
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
		input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;
		
			break;
		
		case 3:				//保存录入的第一块指纹指纹 到模块的第一块缓存
			
				for(i =0;i<sizeof(Get_Dat_Save_1);i++)			//在发送缓存区写入保存指令
				{
					
					TX_BUFF[i] = Get_Dat_Save_1[i];
					
				}
		
			Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_1));
			
			for(i =0;i<50;i++)
			{
				 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			}
			
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nTX_Gen_Dat_Save_1!\r\n");
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
			
			input_ps_T =0;
			
			break;
	
		case 4:		//等待第一个数据的保存确认包
				
				if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为存储成功的应答时进入下一步
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_OK!\r\n");

					#endif
				//	nrf_delay_ms(500);
					input_speak(32);	//提示请再按一次指纹

					nrf_delay_ms(1000);
					PS_LED_Run(3);    //上电亮蓝灯

					Input_Ps_Lmage_Run_Now++;
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_Error!\r\n");
					
					#endif
					    
					Input_Ps_Lmage_Run_Now =1;		//其他数据包 开始重新录入上一个指纹数据
				}
				
		input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;
		
		
		break;
			
		case 5:		//开始准备录入第二次指纹等待手指放开
				
		for(i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		Input_Ps_Lmage_Run_Now++;
		
		input_ps_T =0;

			break;
				
		case 6: //确认指纹是否放开
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，回到上一步
			)					//当获得的数据
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage_1!\r\n");		//提示手指抬起
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;		//手指抬起时准备下一次录入指纹
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为手指按下时，进入下一步
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_1!\r\n\r\n");		//提示手指未抬起获取到指纹
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//收到其他包时推到上一步
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_1!\r\n");		//提示其他报文的情况退到上一步
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
			input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;
		
			break;
		//完成第一次录入
		case 7:		//开始第二次指纹录入
			
					for(i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		break;
		
		case 8:		//等待指纹按下
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，回到上一步
			)					//当获得的数据
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage!_2\r\n");		//提示手指抬起
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//当收到的数据包为手指按下时，进入上一步
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_2!\r\n\r\n");		//提示手指未抬起获取到指纹
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//收到其他包时推到上一步
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_2!\r\n");		//提示其他报文的情况
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
		input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;
		
		break;
		
		case 9:		//保存录入的指纹 到模块的第二块缓存
		
				for(i =0;i<sizeof(Get_Dat_Save_2);i++)			//在发送缓存区写入保存指令
				{
					
					TX_BUFF[i] = Get_Dat_Save_2[i];
					
				}
		
			Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_2));
			
			for(i =0;i<50;i++)
			{
				 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			}
			
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nTX_Gen_Dat_Save_2!\r\n");
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
			input_ps_T =0;
		break;
		
		case 10:
			
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为存储成功的应答时进入下一步
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_2_OK!\r\n"); //第二块指纹保存成功
					
					#endif
					//nrf_delay_ms(500);
					input_speak(32);	//提示请再按一次指纹
					nrf_delay_ms(1000);
						PS_LED_Run(3);    //上电亮蓝灯

					Input_Ps_Lmage_Run_Now++;
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_2_Error!\r\n");
					
					#endif
					
					Input_Ps_Lmage_Run_Now=7;		//其他数据包 开始重新录入上一个指纹数据
				}
			
		input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;

		
		break;
			
		case 11:	//等待指纹放开
		
					for(i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		break;
		
		case 12: //确认指纹是否放开
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，回到上一步
			)					//当获得的数据
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage_3!\r\n");		//提示手指抬起
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;		//手指抬起时准备下一次录入指纹
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为手指按下时，进入下一步
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_3!\r\n\r\n");		//提示手指未抬起获取到指纹
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//收到其他包时推到上一步
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_3!\r\n");		//提示其他报文的情况
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
			input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;

		break;
					
	//开始第三次指纹录入
					
				case 13:		//开始第三次指纹录入
			
					for(i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		break;
		
		case 14:		//等待指纹按下
		
					if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，回到上一步
			)					//当获得的数据
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage!_3\r\n");		//提示手指抬起
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//当收到的数据包为手指按下时，进入上一步
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_3!\r\n\r\n");		//提示手指未抬起获取到指纹
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//收到其他包时推到上一步
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_3!\r\n");		//提示其他报文的情况
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
			input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;

		break;
					
					
		
		case 15:		//保存数据到第三缓存区域
		
			for(i =0;i<sizeof(Get_Dat_Save_3);i++)			//在发送缓存区写入保存指令
				{
					
					TX_BUFF[i] = Get_Dat_Save_3[i];
					
				}
		
			Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_3));
			
			for(i =0;i<50;i++)
			{
				 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			}
			
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nTX_Gen_Dat_Save_3!\r\n");
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
			input_ps_T =0;
		break;
	
		case 16:		// 确定缓存数据保存
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为存储成功的应答时进入下一步
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_3_OK!\r\n");
					
					#endif
					Input_Ps_Lmage_Run_Now++;
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_3_Error!\r\n");
					
					#endif
					
					Input_Ps_Lmage_Run_Now =13;		//其他数据包 重新录入上一个指纹数据
				}
		
				
			input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;

		break;
				
		case 17:			//合并获得的指纹
		
			for(i =0;i<sizeof(Merge_Lmage_Dat);i++)			//合并指纹指令
			{
				
				TX_BUFF[i] = Merge_Lmage_Dat[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(Merge_Lmage_Dat));
		
		for(i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		
		break;
		
		case 18:			//确认合并指纹数据
			
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为存储成功的应答时进入下一步
					)
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nMerge_Lmage_Dat_OK!\r\n");
					
					#endif
					Input_Ps_Lmage_Run_Now++;
				
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nMerge_Lmage_Dat_Error!\r\n");
					
					#endif
					
					Input_Ps_Lmage_Run_Now =0;		//合并失败，重新录入指纹
				}
	
			input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;

		break;
				
		case 19:		//将指纹保存到指纹头
					
			for(i =0;i<sizeof(Ps_Save_ID);i++)			//保存指纹指令
			{
				TX_BUFF[i] = Ps_Save_ID[i];
			}
			
			TX_BUFF[12] = Input_Ps_Lmage_ID;		//在指令中放入需要保存的指纹 ID
			
			ps_buff_chak =0;		//定义校验和的值
			
			for(i =0;i<7;i++)			//计算校验和
			{
				
				ps_buff_chak+=TX_BUFF[i+6];
				
			}
			
			TX_BUFF[13] = ps_buff_chak%0xFF00>>8;		//载入发送的校验和
			TX_BUFF[14] = ps_buff_chak%0xFF;
			
			Uart_Put_String(TX_BUFF,sizeof(Ps_Save_ID));
			
			Input_Ps_Lmage_Run_Now++;
			input_ps_T =0;
		break;
	
		case 20:
		
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为存储成功的应答时进入下一步
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nPs_Save_ID_OK!\r\n");
					
					#endif
					Input_Ps_Lmage_Run_Now++;
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nPs_Save_ID_ERROR!\r\n");
					
					#endif
					
					Input_Ps_Lmage_Run_Now --;		//其他数据包 开始录入上一个指纹数据
				}
	
			input_ps_T++;		//记录持续时间
		
		if(input_ps_T>input_ps_T_Out)			//超时时返回上一步
			Input_Ps_Lmage_Run_Now--;

		break;
	
		case 21:
			
			
			if(Add_Flash_PS_Print(Input_Ps_Lmage_Type,Input_Ps_Lmage_ID,Start_PS_Unix,Stop_PS_Unix)==1)
			{
				
				input_speak(9);		//提示操作成功
				EX_BLE_ACK_Run(1,Input_Ps_Lmage_ID);			//应答成功
				Input_Ps_Lmage_Run_Now=22;
			}
			else
			{
				
				input_speak(10);
				EX_BLE_ACK_Run(2,0xFF);			//应答失败,发送无效ID
				Input_Ps_Lmage_Run_Now=22;
			}
			
			#ifdef uart_start
	    		
			SEGGER_RTT_printf(0,"Input_Ps_Lmage_ID: %x!\r\n",Input_Ps_Lmage_ID);
			
			#endif
			
			input_ps_T =0;

			
			break;
		
		case 22:
		
		if(F_print_Goto_Sleep()==0)				//等待进入睡眠状态
			{
				
				input_ps_T++;
				if(input_ps_T>input_ps_T_Out)			//当超时时
				{
		//			input_speak(10);		//提示操作失败
					Input_Ps_Lmage_Run_Now =0;		//清空状态机
					Input_Ps_Lmage_Type =0;				//初始化指纹录入相关值
					Start_PS_Unix =0;
					Stop_PS_Unix =0;				
			//		Controllable_Power_Off();
				F_Print_Run_Flag =1;
				}
			}
			else
			{
				Input_Ps_Lmage_Run_Now =0;		//清空状态机
				Input_Ps_Lmage_Type =0;				//初始化指纹录入相关值
				Start_PS_Unix =0;
				Stop_PS_Unix =0;
//				Controllable_Power_Off();
				F_Print_Run_Flag =1;
			}
		break;
			
		default :
			
			if(F_print_Goto_Sleep()==0)				//等待进入睡眠状态
			{
				input_ps_T++;
				if(input_ps_T>input_ps_T_Out)			//当超时时
				{
	//				input_speak(10);		//提示操作失败
					Input_Ps_Lmage_Run_Now =0;		//清空状态机
					Input_Ps_Lmage_Type =0;				//初始化指纹录入相关值
					Start_PS_Unix =0;
					Stop_PS_Unix =0;				
					//Controllable_Power_Off();
					F_Print_Run_Flag =1;
				}
			}
			else
			{
				
		//		input_speak(10);		//提示操作失败
				Input_Ps_Lmage_Run_Now =0;		//清空状态机
				Input_Ps_Lmage_Type =0;				//初始化指纹录入相关值
				Start_PS_Unix =0;
				Stop_PS_Unix =0;
				
				//Controllable_Power_Off();
				
				F_Print_Run_Flag =1;
			}
			
			return 1;   //返回异常状态
		
//		break;
		
		
	}
	return 0;
	
}



uint8_t CL_Ps_Print_Now =0;
uint8_t CL_PS_Num =0;
uint8_t CL_Ps_Print()		//清空指纹头数据
{
	uint8_t TX_BUFF[64];			//发送缓存
		
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Start_CL_Ps_Print!\r\n");
	
	#endif
	
	switch (CL_Ps_Print_Now)
	{
		case 0:
		for (uint32_t i =0;i<sizeof(PS_Del_All_Lmage);i++)			//在发送缓存区写入睡眠指令
		{
			TX_BUFF[i] = PS_PowDlow[i];
		}
		
		Uart_Put_String(TX_BUFF,sizeof(PS_PowDlow));				
		CL_Ps_Print_Now++;
		FP_Goto_Sleep_T =0;
		for(uint32_t i =0;i<sizeof(Ps_Data_Tab);i++)
		{
			Ps_Data_Tab[i] =0;
		}
			break;
		case 1: 
			
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
					(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)				//当收到的数据包为进入睡眠状态时操作成功
				)					//当获得的数据
			{
				
				CL_Ps_Print_Now =0;
				
				#ifdef uart_start
	
				SEGGER_RTT_printf(0,"F_Print_Goto_Sleep_OK!\r\n");
				
				#endif
				CL_PS_Num =0;			//清空成功后清空次数
				return 1;
			}
			else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
			{
				CL_Ps_Print_Now =0;			//进入失败  重新发送
				CL_PS_Num++;			//失败后加1
			}
			CL_Ps_Print_Now++;		//借用  goto sleep的延时
			
			if(FP_Goto_Sleep_T>FP_Goto_Sleep_Out)				//超时重新发送
			{
				FP_Goto_Sleep_T =0;
				CL_Ps_Print_Now =0;
				CL_PS_Num ++; //失败后次数加 1
			}
			break;
		
	}
	
		if(CL_PS_Num>5)			//失败五次后返回1 放弃这次重置
		{
			CL_PS_Num =0;
			return 1;
		}
	
	
	return 0;
	
}



//指纹头 LED 运行函数， 输入; LED_CR  LED 颜色  0 关 1 红 2 绿  3 蓝  4 白 

void PS_LED_Run(uint8_t LED_CR)
{
	uint8_t TX_BUFF[64];			//发送缓存
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Start_CL_Ps_Print!\r\n");
	
	#endif
	


		for (uint32_t i =0;i<sizeof(PS_LED_CFG);i++)			//在发送缓存区写入指纹头灯配置指令
		{
			
			TX_BUFF[i] = PS_LED_CFG[i];
		
		}
		
		Uart_Put_String(TX_BUFF,sizeof(PS_LED_CFG));				
		
		nrf_delay_ms(5);
		
			//配置电源模式
			
			for (uint32_t i =0;i<sizeof(PS_LED_POWER);i++)			//在发送缓存区写入指纹头灯配置指令
			{
				
				TX_BUFF[i] = PS_LED_POWER[i];
			
			}
			
			Uart_Put_String(TX_BUFF,sizeof(PS_LED_POWER));				
			
			nrf_delay_ms(5);
			
			if(LED_CR==1)		//红色
			{
				
				for (uint32_t i =0;i<sizeof(PS_LED_R1);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_R1[i];
				}
				
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_R1));
				nrf_delay_ms(5);
				
				for (uint32_t i =0;i<sizeof(PS_LED_R2);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_R2[i];
				}
				
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_R2));
				
			}
			else if(LED_CR==2)	//绿色
			{
				
				for (uint32_t i =0;i<sizeof(PS_LED_G1);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_G1[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_G1));
				nrf_delay_ms(5);
				for (uint32_t i =0;i<sizeof(PS_LED_G2);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_G2[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_G2));

			}
			else if(LED_CR==3)  //蓝色
			{
			for (uint32_t i =0;i<sizeof(PS_LED_B1);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_B1[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_B1));
				nrf_delay_ms(5);
				for (uint32_t i =0;i<sizeof(PS_LED_B2);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_B2[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_B2));
				

			}
			else if(LED_CR==4)   //白色
			{
				for (uint32_t i =0;i<sizeof(PS_LED_W1);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_W1[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_W1));
				nrf_delay_ms(10);
				for (uint32_t i =0;i<sizeof(PS_LED_W2);i++)			//在发送缓存区写入指纹头灯配置指令
				{
					TX_BUFF[i] = PS_LED_W2[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_W2));
				
			}
			
			
			FP_Goto_Sleep_T =0;
			for(uint32_t i =0;i<sizeof(Ps_Data_Tab);i++)			//清空指纹接收缓存
			{
				
				Ps_Data_Tab[i] =0;
			
			}

}

void PS_LED_Shutdown()			//指纹头 LED 模组进入 低功耗模式  (直接关闭指纹模组)
{
		uint8_t TX_BUFF[64];			//发送缓存
	
		for (uint32_t i =0;i<sizeof(PS_LED_Sleep);i++)			//在发送缓存区写入指纹头灯配置指令
			{
				TX_BUFF[i] = PS_LED_Sleep[i];
			}
			Uart_Put_String(TX_BUFF,sizeof(PS_LED_Sleep));
			
			nrf_delay_ms(100);		//等待 100 ms
			
}



