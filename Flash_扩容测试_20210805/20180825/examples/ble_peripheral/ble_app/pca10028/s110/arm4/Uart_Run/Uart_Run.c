#include <Uart_Run.h>
#include <F_print.h>


uint8_t Uart_TX_String[20];  //串口输出字符串缓存

uint8_t Uart_RX_String[Uart_RX_String_Size];

uint32_t Uart_RX_len =0;

extern uint8_t UART_RUN;


/************************************************

串口字符串输出函数：


输入：*Uart_Dat  字符串指针++
输入： len  字符串长度

输出； 无

备注：

这个函数用于通过串口输出对应地址中指定长度的字符串

**************************************************/

void Uart_Put_String (uint8_t *Uart_Dat,uint8_t len)
{
	uint8_t i;
	
	if(UART_RUN==0)  //当串口未开启时，直接打开串口
	{
		
		Uart_Start_TC();
		
	}
	
	Uart_Close_T =0;   //发送一次 重置串口关闭计时时间

	#ifdef uart_start
	SEGGER_RTT_printf(0,"\r\nUARTTX:\r\n");
	
	for(i =0;i<len;i++)
	{
		
		SEGGER_RTT_printf(0," %x ",*Uart_Dat);
		Uart_Dat++;
	}
	
	SEGGER_RTT_printf(0,"\r\n");
			Uart_Dat-=i;
	
	#endif

	
	for(i=0;i<len;i++)
	{
		
			simple_uart_put(*Uart_Dat);
			Uart_Dat ++;

	}
	
	Uart_Close_T =0;

	
}


/***********************************

串口读取

**************************************/

 //while (NRF_UART0->EVENTS_RXDRDY != 1)
//  NRF_UART0->EVENTS_RXDRDY = 0;
//  return (uint8_t)NRF_UART0->RXD;//读出数据

uint32_t Get_String_t =0;  //数据抓取周期
void Uart_Get_String()  //获取 100ms内串口抓取的报文
{
	
	Get_String_t =0;
	Uart_RX_len =0;
	
	
	if(NRF_UART0->EVENTS_RXDRDY !=0)
	{

		Uart_Close_T =0;
			
		while((Get_String_t<6000)&&(Uart_RX_len<sizeof(Uart_RX_String)))
		{
			
			if(NRF_UART0->EVENTS_RXDRDY!=0)
			{
				
				NRF_UART0->EVENTS_RXDRDY =0;
				if(Uart_RX_len<sizeof(Uart_RX_String))  //当接收长度小于缓存区容量时允许继续接收
					Uart_RX_String[Uart_RX_len++]=NRF_UART0->RXD;  //将收到的数据放进缓存
				
			}
			
			Get_String_t ++;
			
//			if(Get_String_t>5000)  //当获取周期达到10000us(10ms) 即结束接收 
//				break;
			
		//	nrf_delay_us(1);
			
		}
		
		if(Uart_RX_len>0) //当获取到有效的包时
		{
			
	//		Uart_Put_String(Uart_RX_String,Uart_RX_len);
//			Uart_RX_len =0;
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nUART_RX_len:%x \r\n",Uart_RX_len);
			
			for (uint32_t i =0;i<Uart_RX_len;i++)
			{
				
				SEGGER_RTT_printf(0,"%x ",Uart_RX_String[i]);
				
				if((i%8==0)&&(i!=0))
					SEGGER_RTT_printf(0,"\r\n");
			}
			
			SEGGER_RTT_printf(0,"\r\n");
			
			#endif
			
			#ifdef  PS_Enable
			
			PS_Data_Cheak(Uart_RX_String,Uart_RX_len);			//	输出数据包
			
			#endif
			
			#ifdef NB_Enable   //当 使能NB 时
			
			if(NB_Run ==1)   //当 NB 模块被打开时将 NB 模块数据写入 
			{
					if((NB_Dat_Len+Uart_RX_len)<NB_Dat_Size)
					{
						for(uint32_t i =0;i<Uart_RX_len;i++)
						{
							
							NB_Dat[NB_Dat_Len+i] = Uart_RX_String[i];
							
						}
							NB_Dat_Len+=Uart_RX_len;  //增加 NB_Dat 值
						
					}
					else
					{
						
						#ifdef uart_start
						
							SEGGER_RTT_printf(0,"NB_Dat_Len_Full!\r\n");   // 提示 NB_Dat  长度溢出
						
						#endif
						
					}
			
			}
			
				#ifdef uart_start
				
					if(NB_Dat_Len>0)   //当 NB_Dat  缓存区有数据时
					{
						SEGGER_RTT_printf(0,"NB_Dat_Len: %x \r\n",NB_Dat_Len);
						
							for (uint32_t i =0;i<NB_Dat_Len;i++)
							{
								
								SEGGER_RTT_printf(0,"%x ",NB_Dat[i]);
								
								if((i%8==0)&&(i!=0))
									SEGGER_RTT_printf(0,"\r\n");
								
							}
						
					}
					
					SEGGER_RTT_printf(0,"\r\n");
					
					
					
				#endif
			
			
			#endif
			
			
		}
	}
}

/************************************

A71_串口数据解析

*************************************/

/*************************************

判断 数据包是否为合法的A71 串口报文

************************************/
uint8_t Get_A71_Str[100];  //获取到的A71 str 缓存
uint8_t A71_Str_Len =0;
uint8_t A71_Dat_Cheak()
{
	
	uint8_t len;  //有效数据包长度
	uint8_t i,it;
		
	A71_Str_Len =0;
	
	for(i =0;i<Uart_RX_len;i++)
	{
			len = Uart_RX_String[i+3];
		
			if((Uart_RX_String[i]==0x55)&&(Uart_RX_String[i+1]==0xAA)&&  //数据包头正确
				(Uart_RX_String[i+2]==0x00)&&																	//第一个长度字节暂时不加入运算
				(Uart_RX_String[i+3]<=(sizeof(Uart_RX_String)-i))&&						//判断字符串长度值是否合法
				(Uart_RX_String[i+Uart_RX_String[i+3]-1-1]==0xAA)&&(Uart_RX_String[i+Uart_RX_String[i+3]-1]==0x55)   //判断包尾是否合法
				)
			{
				#ifdef uart_start
				
			SEGGER_RTT_printf(0,"Get_A71_Str!\r\n");   //获得了一个A71 类型的数据包
				
				#endif
				
				for(it =0;it<len;it++)    //截取获取到的 A71 报文
				{
					
					Get_A71_Str[it] = Uart_RX_String[i+it];
				
				}
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Get_A71_Str_Len:%x\r\n",it);   //获得了一个A71 类型的数据包
				
				#endif
				
				
				return 1;
			}
	}
	
	return 0;

}




//A71 射频模块
/*********************************************

为保证数据传输稳定，在数据包发送前先发送5个 0x00 可用于降低干扰和唤醒模块

数据包格式: 0x55+0xAA 2 bit (用于通信开头)

长度： 2 bit  (用于存放数据长度)

数据类型：2 bit  (用于表示数据包类型)

数据来源： 2 bit  (用于区分数据包来源设备) 


数据包增量： 2 bit  (每次通信的报文编号，用于区分同一来源的不同数据包，或多个数据包的组合)

数据内容：  N bit   (有效的数据内容，仅需保证数据包总长度在最大数据内容)

数据校验码： 8 字节  (用于支持各种冗余算法，增加数据可靠性)

数据结尾： 0xAA+ 0X55 2 bit (用于通信结尾)

*********************************************/


//射频模块串口预装指令

//初始化 A71 射频模块 指令
const uint8_t  A71_Init[] = 
{
	0X55,0xAA,
	0x00,0x0C,
	0x04,0x08,
	0X10,0X01,
	0X00,0X00,
	0xAA,0x55
};

//设置 A71 射频模块 通信ID 指令
const uint8_t  A71_Set_ID[] = 
{
	0X55,0xAA,   //数据头
	0x00,0x11,	//长度
	0x04,0x01,  //指令码
	0x10,0x01,	//数据来源/目标
	0x00,0x00,  //数据包ID
	0x04,0x34,0x75,0xC5,0x8C,   //有效数据区 //默认四字节ID： 0X34,0X75,0XC5,0X8C
	0xAA,0x55   //数据尾
};


//设置 A71 射频模块 到接收模式，直到接收到数据

const uint8_t A71_RX_Mod[] = 
{
	0x55,0xAA,
	0X00,0X0C,
	0X04,0X06,
	0X10,0X01,
	0X00,0X00,
	0XAA,0X55
};

//用于串口发送模式的指令  （仅包含指令，实际以封包为准）

const uint8_t A71_TX_Mod[] = 
{
	0x55,0xAA,
	0X00,0X00,
	0X03,0X00,
};



//设置 A71 模块到掉电模式，直到唤醒

const uint8_t A71_Power_Down[] = 
{
	
	0x55,0xAA,
	0X00,0X0C,
	0X04,0X0D,
	0X10,0X01,
	0X00,0X00,
	0XAA,0X55
	
};

// 设置 A71 发送数据



//向 A71 模块发送数据
uint8_t Send_Buff[100];  //发送缓存

//在数据包发送前带 "A71_Send_Sp_Num"个 0X00，以保障通信可靠
const uint8_t A71_Send_Sp_Num =5;

/*****************

向 A71 模块写入初始化指令

*******************/

void A71_Send_Init ()
{
	
	uint8_t i;
	
	for(i =0;i<sizeof(Send_Buff);i++)   //清空缓存
 		Send_Buff[i] =0;
	
	for(i =0;i<sizeof(A71_Init);i++)
	{

		Send_Buff[i+A71_Send_Sp_Num] = A71_Init[i];   //偏移值表示发送数据包前面带了偏移值个数的0X00

	}
	
	Uart_Put_String(Send_Buff,sizeof(A71_Init)+A71_Send_Sp_Num);   //发送数据包到串口

}

/**************************

向 A71 模块写入 通信ID设置指令

*************************/

uint8_t A71_ID_Tab[4] = 
{
	
0x34,0x75,0xc5,0x8c
	
};  //写入ID缓存

void A71_Send_ID()
{
	
		uint8_t i;
		
		for(i =0;i<sizeof(Send_Buff);i++)
			Send_Buff[i] = 0;
		
		for(i =0;i<sizeof(A71_Set_ID);i++)
		{
			
			Send_Buff[i+A71_Send_Sp_Num] = A71_Set_ID[i];
			
		}
		
		//修改 ID 号到设置值
		
		Send_Buff[11+A71_Send_Sp_Num] = A71_ID_Tab[0];
		Send_Buff[12+A71_Send_Sp_Num] = A71_ID_Tab[1];
		Send_Buff[13+A71_Send_Sp_Num] = A71_ID_Tab[2];
		Send_Buff[14+A71_Send_Sp_Num] = A71_ID_Tab[3];
		
		Uart_Put_String(Send_Buff,sizeof(A71_Set_ID)+A71_Send_Sp_Num);   //发送数据包到串口
			
}

/**************************************************

向 A71 模块发送掉电指令 以关闭模块

（可通过串口发送数据唤醒模块）

*************************************************/

void A71_Send_PD()
{
	
	uint8_t i;
	
	for(i =0;i<sizeof(Send_Buff);i++)
		Send_Buff[i] = 0;
	
	for(i =0;i<sizeof(A71_Power_Down);i++)
	{
		
		Send_Buff[i+A71_Send_Sp_Num] = A71_Power_Down[i];
		
	}
	
	Uart_Put_String(Send_Buff,sizeof(A71_Set_ID)+A71_Send_Sp_Num);
	
}


/*******************************

发送接收模式指令
使 A71 模块进入接收模式 (维持时间由模块决定)

*******************************/

void A71_Send_RX()
{
	uint8_t i;
	
	for(i =0;i<sizeof(Send_Buff);i++)
		Send_Buff[i] =0;
	
	for(i =0;i<sizeof(A71_RX_Mod);i++)
	{
		
		Send_Buff[i+A71_Send_Sp_Num] = A71_RX_Mod[i];
		
	}
	
	Uart_Put_String(Send_Buff,sizeof(A71_Set_ID)+A71_Send_Sp_Num);//将数据包发送出去
	
	
}


/************************************************

向 A71 模块发送 数据包发送指令
以将指定数据通过模块发送到无线

*************************************************/

void A71_Data_TX(uint8_t	*s)
{
	uint8_t i;
		nrf_delay_ms(300);
	for(i =0;i<sizeof(Send_Buff);i++)
		Send_Buff[i] = 0;
	
	Send_Buff[0+A71_Send_Sp_Num] = 0X55;
	Send_Buff[1+A71_Send_Sp_Num] = 0XAA;
	Send_Buff[2+A71_Send_Sp_Num] = 0x00;
	Send_Buff[3+A71_Send_Sp_Num] = 77;   //数据包长度
	Send_Buff[4+A71_Send_Sp_Num] = 0X03;  //数据发送指令
	Send_Buff[5+A71_Send_Sp_Num] = 0x00;
	Send_Buff[6+A71_Send_Sp_Num] = 0x10;
	Send_Buff[7+A71_Send_Sp_Num] = 0x01;
	Send_Buff[8+A71_Send_Sp_Num] = 0x00;
	Send_Buff[9+A71_Send_Sp_Num] = 0x00;
	
	Send_Buff[10+A71_Send_Sp_Num] = 64;   //数据包长度
	
	for(i =11+A71_Send_Sp_Num;i<(75+A71_Send_Sp_Num);i++)
	{
		
		Send_Buff[i] = *s;
		s++;
	
	}
	
	Send_Buff[75+A71_Send_Sp_Num] = 0xAA;
	Send_Buff[76+A71_Send_Sp_Num] = 0x55;
	
	Uart_Put_String(Send_Buff,A71_Send_Sp_Num+77);
	
}




/*****************************************

下面是用于落实
A71_RF 模块功能的函数

****************************************/


/********************************

uint8_t A71_RF_RX_MODE()  

功能： 使A71_RF 模块 在接收模式

输入： 无
输出：成功： 1  失败：0

*******************************/

uint8_t A71_RF_RX_MODE()  
{
	uint8_t now;
	uint32_t run_t;  //运行时间 超时后自动结束
	uint8_t i;
	now =0;
	run_t =0;
	
	while(now<0xFF)   //直到RX运行设置完成
	{
		
		switch(now)
		{
			case 0:   //先唤醒设备
				
				for(i =0;i<sizeof(Send_Buff);i++)
				 Send_Buff[i] =0;
			
				Uart_Put_String(Send_Buff,20);   //发送20字节 0X00 用于唤醒设备
				nrf_delay_ms(100);
			
				now++;
				
			break;
			
			case 1:   //向设备发送进入接收模式的指令
				
				A71_Send_RX();
		
				Uart_RX_len =0;  //清空串口接收的缓存计数
	
				
				now++;
			
				#ifdef uart_start
			
				SEGGER_RTT_printf(0,"A71_Send_RX!\r\n");
				
				#endif
			
				break;
			
			case 2:  //等待数据
					
				if(Uart_RX_len>0)
				{
					
					if(A71_Dat_Cheak()==1)
					now++;
					else
					now=0xff;  //结束循环
					
				}
			
				break;
			
			case 3:  //判断应答控制字是否正确
			
				if (
					(Get_A71_Str[4]==(A71_RX_Mod[4]|0x80))&&
					(Get_A71_Str[5]==(A71_RX_Mod[5]|0X80))) 
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"A71_Get_Order_OK!\r\n");
					
					#endif
					now++;
				}
			
				break;
			
			case 4:
			
				if(Get_A71_Str[10]==0x01)  //当返回数据表示成功时
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"A71_Send_RX_MOD_OK!\r\n");
					
					#endif
					
					Uart_Close_T =0;   //成功设置为接收模式后 重置串口关闭计时时间
					
					return 1;
					
				}
				else if(Get_A71_Str[10]!=0x01)
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"A71_Send_RX_MOD_Error!\r\n");
					
					#endif
					
					return 0;
					
				}				
				now =0xFF;
			break;

			case 5:
			
			now =0xFF;			
			
				break;
			
			default:
			
				now = 0xFF;  //当 now值不合法时，结束循环
			
			break;
			
			
		}
		
		if((UART_RUN==1)&&(now==2))  //用于串口运行时接收数据
		{
			
			Uart_Get_String();
			
		}
		
		run_t++;
		if(run_t>500000)  //当运行超时后，结束运行
		{
			run_t =0;
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"A71_RF_RX_TimeOut!\r\n");
			
			#endif
			
			return 0;
		}
	
	}
	return 0;
}


/*************************************
通过设置模块进入接收模式，来完成数据包接收过

输入： RX_Dat_Num   单位时间内 获取数据包数量
输出： 1 成功获取到数据包  0：未获取到数据包

************************************/

uint8_t A71_RX_Dat_Tab[64];

uint8_t Need_Of_RX_Dat()
{
	
	uint32_t i;
	uint8_t now;
	uint32_t run_t;
	
	run_t =0;
	
	for(i =0;i<sizeof(A71_RX_Dat_Tab);i++)  //清空缓存
	{
		
		A71_RX_Dat_Tab[i] =0x00;
		
	}
		
	
	if(A71_RF_RX_MODE()==1)
	{
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Start_RXD_A71_DAT!\r\n");  //开始接收来自A71的数据
		
		#endif
		
		now =0;
		Uart_Close_T =0;   //发送一次 重置串口关闭计时时间
		
		while((now<0xFF)&&(UART_RUN==1))   //开始等待数据包
		{
			
				switch(now)
				{
					case 0:   
					
					if(UART_RUN==1)		//等待串口数据
						Uart_Get_String();
					
					if(Uart_RX_len>0)  //当收到串口包
						now++;
					
						break;
					
					case 1:
					
					if(A71_Dat_Cheak()==1)   //判断并抓取合法的串口数据包
					now++;
					else
					now=0;  //当无合法数据时，返回等待状态
					
						break;
					
					case 2:  //开始判断指令和长度
						
					if (
					(Get_A71_Str[4]==(A71_RX_Mod[4]|0x80))&&
					(Get_A71_Str[5]==(A71_RX_Mod[5]|0X80))&&
					(Get_A71_Str[3]==76))   // RF数据回应包长度为76字节
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"A71_Get_Dat_Order_OK!\r\n");
						
						#endif
						now++;
						
					}
					
						break;
					
					case 3:   //开始将有效数据写入缓存
					
						#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Get_Dat:\r\n");
					
						#endif
					
						for(i =0;i<64;i++)   //提取有效数据
						{
							A71_RX_Dat_Tab[i] = Get_A71_Str[i+10];
							
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"%x ",A71_RX_Dat_Tab[i]);
						
						if((i>0)&&(i%8==0))
						SEGGER_RTT_printf(0,"\r\n");
							
						#endif
						
						}
						
						#ifdef uart_start					
					
						SEGGER_RTT_printf(0,"\r\n");
					
						#endif

						now++;
						
						
						return 1;
						
					//	break;
						
					case 4:
						
					now =0xFF;
						break;
					
					case 5:
						
					now =0xFF;
						break;
					
					default:
						
					now = 0xFF;
					
					break;
					
				}
				

				
				run_t++;
				
			if(run_t>2500000)  //当接收超时时
			{
				run_t =0;
				
				#ifdef uart_start 
				
				SEGGER_RTT_printf(0,"Need_Of_RX_Dat_TimeOut!\r\n");
				
				#endif
				
				return 0;
			}
			
		}
		
	}
	
	return 0;
}


/**************************

uint8_t A71_TX_Dat(uint8_t *s)

完成一次完整的发送过程

**************************/

uint8_t A71_TX_Dat(uint8_t *s)
{
	uint8_t now;

	uint32_t run_t;
	
	now =0;
	run_t =0;
	
	while (now<0xFF)
	{
		
		switch(now)
		{
			
			case 0: 
			
			A71_Data_TX(s);   //向射频发送数据包
			
			now++;

				break;
			
			case 1:   //等待回应

			if(UART_RUN==1)		//等待串口数据
					Uart_Get_String();
				
			if(Uart_RX_len>0)  //当收到串口包
					now++;
				
					
			break;
			
			case 2:
				
			if(A71_Dat_Cheak()==1)   //判断并抓取合法的串口数据包
				now++;
		else 
			now =1;   //回到等待接收的位置
		
			break;

			case 3:
				
				if (
					(Get_A71_Str[4]==(A71_TX_Mod[4]|0x80))&&
					(Get_A71_Str[5]==(A71_TX_Mod[5]|0X80))&&
					(Get_A71_Str[3]==0x0D))   // RF数据回应包长度为13字节
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"A71_TX_Dat_Order_OK!\r\n");
						
						#endif
						now++;
						
					}
		
			break;

			case 4:   //等待第二条数据 (发送成功回应)
			
			if(UART_RUN==1)		//等待串口数据
				Uart_Get_String();
				
			if(Uart_RX_len>0)  //当收到串口包
					now++;
				
			
			break;

			case 5:
				
			if(A71_Dat_Cheak()==1)   //判断并抓取合法的串口数据包
				now++;
			else 
				now =4;  //数据包错误则返回
		
			break;
	
			case 6:
			
				if (
						(Get_A71_Str[4]==(A71_TX_Mod[4]|0X80))&&
						(Get_A71_Str[5]==0)&&				//当指令为发送成功回应
						(Get_A71_Str[3]==0x0D)		
					)
				{
					if(Get_A71_Str[10]==0X01)    //发送成功返回1 
					{
						#ifdef uart_start
							
						SEGGER_RTT_printf(0,"A71_TX_Succeed!\r\n");

						#endif
						
						return 1;
					}
					else if(Get_A71_Str[10]==0)
					{
						#ifdef uart_start
							
						SEGGER_RTT_printf(0,"A71_TX_Error!\r\n");

						#endif
												
					return 0;
					}
				}
				else
					now =4;
				
				
			break;
			
			
			default :
				now =0xFF;
				break;
			
		}
		
		
		run_t++;
		
		if(run_t>200000)  //当运行超时后，结束运行
		{
			run_t =0;
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"A71_RF_TX_TimeOut!\r\n");
			
			#endif
			
			return 0;
		}
		
	}
	return 0;
}


