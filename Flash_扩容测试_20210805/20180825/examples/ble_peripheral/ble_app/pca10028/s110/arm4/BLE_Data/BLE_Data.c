#include <BLE_Data.h>
#include <app_uart.h>
#include <nrf_error.h>
#include "Devic_Init.h"
#include <MotO_Run.h>
#include <F_print.h>
#include <Flash_RW.h>

//static  ble_nus_t  m_nus;     


#if YFA			//人脸识别成立时


#define BLE_DAT_H1  0x5A
#define BLE_DAT_H2  0x5E

#define BLE_DAT_E1  0xA5
#define BLE_DAT_E2  0xEC

#elif DSA			// 如家成立时


#define BLE_DAT_H1  0xED
#define BLE_DAT_H2  0x2C

#define BLE_DAT_E1  0xD3
#define BLE_DAT_E2  0xEB

#else				//均不成立选择默认


#define BLE_DAT_H1  0x5E
#define BLE_DAT_H2  0xDC

#define BLE_DAT_E1  0xAE
#define BLE_DAT_E2  0x2B

#endif



/*****************************************************

这个文件用于，处理BLE数据通信相关内容
关于发送和回应 配置，即实现指令相关功能。

*****************************************************/

extern uint8_t BLE_LINK;  //BLE连接状态实时更新
uint8_t BLE_Data[100];  // BLE数据缓存
uint8_t BLE_Data_Len =0;  //保留的数据长度

#define data_len_min  0x10  //最小有效数据长度
uint16_t data_len;  //数据长度
uint8_t  IN_BLE_Dat_SW =0;
uint8_t irw; //定义 Data_Input 循环函数

const uint32_t Version_Num = 0x20090709;			//版本号 格式： 年 月 日 小时

const uint32_t Authorization_time = 0x20090710;			//授权起始时间 格式： 年 月 日 小时


//#define APP3_Op_Door  1					//定义 是否允许第三方 开门指令


/***********************************************
保留获取到的BLE数据，准备开始分析
**********************************************/


void Data_Input(uint8_t *BLE_DAT_p,uint8_t Dat_Len)  //BLE数据输入
{
//	uint8_t it1,it2; //前两个字节缓存
	
//	it1 = *BLE_DAT_p;
//	BLE_DAT_p++;
//	it2 = *BLE_DAT_p;  //抓取前两个字节
//	BLE_DAT_p--;   //指针回位 
//	
//	if((it1==0x5E)&&(it2==0xDC))
//	{
//		data_len =0;
//		BLE_Data_Len =0;  //发现包头时，直接清空之前数据
//	}
	
		if(IN_BLE_Dat_SW==0)
		{
			IN_BLE_Dat_SW =1;
			for( irw =0;irw<Dat_Len;irw++)
			{
				BLE_Data[BLE_Data_Len] = *BLE_DAT_p; //存入位置由Data_Len 控制
				BLE_DAT_p++;
				if(BLE_Data_Len<(sizeof(BLE_Data)-2))  //当数据缓存区还有空间时，允许继续往后推地址
				BLE_Data_Len++;  //BLE数据缓存内的数据长度
				else
					break; //否则弹出这次循环
			}
			BLE_DAT_SW();  //输入完成后既时处理数据
		}
}

/***********************************************
分析数据功能，并按相应功能开始区分功能。
***********************************************/
//声明数据包标识

/***********************************************
指令包格式

2字节包头+2字节长度+1字节指令码+1字节包标识
+1字节包序号+4字节设备标识+31字节参数+2字节校验码+2字节包尾
***********************************************/
#define Sign_Add  5  //包标识位置

#define Command_Sign  0x01  //指令标识
#define Ack_Sign  0x02  //应答标识

//声明指令值
#define Command_Add 	0X04  //指令码在报文中的位置
#define Mate_Command  0x43  //绑定指令码
#define Time_Ref_Command 0x02  //更新时间指令码
#define Get_Time_Command 0x03  //获取时间
#define APP_OpenLock_Command 0x04  //软件开门指令码

#define Normally_Open_Door   0x05		//门常开指令

#define Add_Card	0x10	 //用户手机自助发卡	添加指纹
#define Add_Information_Command 0x12 //信息载入指令码
#define Del_Information_Command	0x15  //撤销已载入信息指令码
#define Del_A_Information_Command 0x14 //删除单个权限指令
#define Del_Mate_Command 0x30 //解除绑定指令

#define More_Act_PassWord_Mode  0x50  //修改时效动态密码模式指令  支持1个或多个同时生效

#define Del_A_More_Act_PassWord 0x51  //从多组动态密码中删除单个动态密码

#ifdef APP3_Op_Door					//当允许第三方APP 开门指令时

#define APP3_OpenLock_Command				0xA9				//第三方 APP 开门指令

#endif

#define Read_OP_Door_note_Command 0x22  //同步开门记录指令
#define Read_A_OP_Door_note_Command 0x45  //逐条查询开门记录指令
#define CL_OP_Door_note_Command 0x46  //清空开门记录指令

void BLE_DAT_SW()
{
	uint16_t i,it,it1,it2; //循环用
	if ((BLE_Data_Len>=2)&&(data_len==0)) //当载入BLE数据头时，确认头是否正确，如果错误则丢弃
	{
		if ((BLE_Data[0]==BLE_DAT_H1)&&(BLE_Data[1]==BLE_DAT_H2)) //判断数据包头数据是否正确的指令包
			//0x01表示收到的是正确的指令包，其他包不理会
		{
			;  //正确时不做处理，允许继续往后载入
		}
		else
		{
			BLE_Data_Len =0;  //当头错误时，则清除收到的数据
			data_len =0;  //数据包长度清零
			
			#ifdef uart_start
				SEGGER_RTT_printf(0,"BLE_Data_Error_0!\r\n");  //表示头错误
			#endif
		} 
	}
	if ((BLE_Data_Len>=4)&&(data_len ==0)) //获取数据包长度
	{
		if (BLE_Data[2]==0)
		{
		data_len = BLE_Data[3];  //提取数据包长度
		}
		else
		{
			
//			data_len = BLE_Data[2];
//			data_len<<=8;
//			data_len+=BLE_Data[3];  //累加总数			
			BLE_Data_Len =0;  //当内容大于255字节，直接丢弃
			
			data_len =0;
			#ifdef uart_start
				SEGGER_RTT_printf(0,"BLE_Data_Error_1!\r\n");  //表示数据包长度值错误
			#endif
		}
		#ifdef uart_start
		SEGGER_RTT_printf(0,"data_len: %d\r\n",data_len);
		#endif
	//	BLE_Data_Len  =0;  //test		
		if (data_len<data_len_min)  //当收到长度值小于最小长度，则丢弃这个包
		{
			BLE_Data_Len =0;
			data_len =0;
			#ifdef uart_start
			SEGGER_RTT_printf(0,"data_len Error_2!\r\n");  //反馈数据长度值错误表示这个包被丢弃
			#endif
		}
	}
		//接下来既表示包有效时的处理
	if((BLE_Data_Len>=(data_len+6))&&(data_len>=data_len_min)) //当数据包按长度接收完成后
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"BLE_Dat_get:\r\n");  //收完一个数据包
		for(i =0;i<BLE_Data_Len;i++)
		{
		SEGGER_RTT_printf (0,"0x%x",BLE_Data[i]);
		if(i<(BLE_Data_Len-1))
		SEGGER_RTT_printf (0,",");
		}
		SEGGER_RTT_printf (0,"\r\n");
		#endif
		
		it = 0;
		it1 = BLE_Data[data_len+2];
		it1<<=8;
		it2 = BLE_Data[data_len+3];
		it1+=it2;
		for(i =0;i<(data_len-2);i++)  //校验码除外的有效值相加
		{
			it +=BLE_Data[i+4];
		}
		if((it==it1)&&(BLE_Data[data_len+4]==BLE_DAT_E1)&&(BLE_Data[data_len+4+1]==BLE_DAT_E2))  //校验通过后
		{
			#ifdef uart_start
			SEGGER_RTT_printf (0,"Cheak_ok!\r\n");
			#endif
			BLE_Dat_Select();
		}
		else			//校验错误后断开连接
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Cheak_Error!\r\n");
			
			#endif
			Dis_Conner();			//校验失败断开连接
		}
		
		//处理完后清空内容，开始新的数据包处理
		data_len =0;
		BLE_Data_Len =0;
	}
	IN_BLE_Dat_SW =0;
}

void BLE_Dat_Select()  //根据指令跳转子函数
{
//	uint32_t err_code;
		switch (BLE_Data[Command_Add])
		{
			case Mate_Command:  //当指令码为绑定指令时
				if (BLE_Data[Sign_Add]==Command_Sign) //当包标识为指令标识时
					{
						Mate_Command_Run();
					}						
				else if (BLE_Data[Sign_Add]==Ack_Sign) //当包标识为应答标识时
					{
						
					}
					else
					{
						Dis_Conner();
					}
					
				break;
			case Time_Ref_Command:  //当指令码为绑定指令时
				if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
					{
						Time_Ref_Run();  //运行时间更新指令
					}						
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
				
			case Get_Time_Command:  //当指令码为绑定指令时
				if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
					{
						Get_Time_Run();  //运行时间返回指令
					}						
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
							
			case APP_OpenLock_Command:  //当指令码为开门指令时
				
			if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
					{
						APP_OpenLock_Run();  //运行时间返回指令
					}
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
				case Add_Information_Command:
					if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
						{
							Add_Information_Run();  //运行信息载入指令
						}
					else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
						{
							
						}
						else
						{
							Dis_Conner();
						}				
							break;
				case Del_Information_Command:
					if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
						{
							Del_Information_Run();  //运行信息删除指令
						}
					else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
						{
							
						}
						else
						{
							Dis_Conner();
						}				
							break;
					
				case Del_A_Information_Command:
				if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
				{
					if((BLE_Data[13]==0x01)||(BLE_Data[13]==0x02)||(BLE_Data[13]==0x03)||(BLE_Data[13]==0x04))  //当要处理的权限信息为密码或TypeA/B卡时
					{
						Del_A_PassWord_Run();  //运行删除密码权限指令
					}
				}
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
				{
					
				}
				break;
				
				case Del_Mate_Command:
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))			//解绑
					{
						Del_Mate_Run();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}
					break;
					
					case Read_OP_Door_note_Command:				//该指令修改为获取本地开门记录 数量
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 Read_OP_Door_note();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}
						break;
					
					
					case Read_A_OP_Door_note_Command:		//读取对应编号的 开门记录
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 TX_Door_note();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}
						break;
					
					case CL_OP_Door_note_Command:		//清空 开门记录
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 CL_Door_note();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}
						break;
					
					
					
					case Add_Card:
						
					 if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))				//手动发卡
						{
							input_speak(22); //先发送语音语音
							Manual_Add_Card();
						}				
						else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
						{
							
						}
						
						break;
						
					case Normally_Open_Door:  //常开门锁指令
					
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 Normally_Open_Door_Run();		//运行常开门锁指令
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}	
					
					break;
					
					case  More_Act_PassWord_Mode:   //修改时效动态密码模式指令  支持1个或多个同时生效 (0X50)
					
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 More_Act_PassWord_Mode_Run();		//运行常开门锁指令
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}
					
					break;
					
					case Del_A_More_Act_PassWord:			//删除单个时效密码 (0X51)
					
						
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 Del_A_More_Act_PassWord_Run();		//运行常开门锁指令
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//当包标识为应答标识时
					{
						
					}
					
					break;
					
					
					#ifdef APP3_Op_Door						//当允许第三方开门指令
					
					case APP3_OpenLock_Command:
						
					if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //当包标识为指令标识时
					{
						APP_OpenLock_Run();  //运行时间返回指令
					}
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //当包标识为应答标识时
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
					#endif
				default :  //其他情况
								break;
		}
}

#define Mate_Dat_Len  40 //设置绑定参数长度  （包含四字节绑定号和24字节广播名称）
#define Mate_Com_Add	11
#define Mate_Ack_Len  28
extern uint8_t TSM12MC_Status;		//判断 TSM12MC是否存在

extern uint32_t id1,id2;  //用于反馈的设备ID号     
void Mate_Command_Run()
{
	
	uint32_t i,it,it1;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Mate_Command_Run!\r\n");
	#endif

	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x1E;  //长度
	
	TX_Buff[4] = 0X43;  //指令码 
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127)  //修复字节溢出
		it1 = 0;
	
	TX_Buff[7] =it1;
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	TX_Buff[8] = it1;
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;
	TX_Buff[9] =it1;
	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	TX_Buff[10] = it1;
	
	TX_Buff[11] = 0X01;  //表示绑定成功
	
	TX_Buff[12] = BLE_Data[11]; //账号编号
	TX_Buff[13] = BLE_Data[12];
	TX_Buff[14] = BLE_Data[13];
	TX_Buff[15] = BLE_Data[14];
	
	TX_Buff[16] = TSM12MC_Status;  //返回是否包含密码 即按键板状态
	
	TX_Buff[17] = 0;  //返回是否带指纹 默认写0
	
	#ifdef PS_Enable  

	TX_Buff[17] = 1;  //使能带指纹后写1
		
	#endif
	
	//返回
	it = Version_Num;
	
	TX_Buff[18] = it>>24&0xFF;  //Version_Num  版本号 第一位 从高往低

	TX_Buff[19] = it>>16&0XFF;  //

	TX_Buff[20] = it>>8&0XFF;  //

	TX_Buff[21] = it&0XFF;  //

	it = Authorization_time;  //Authorization_time 授权时间  第一位 从高往低

	TX_Buff[22] = it>>24&0XFF;  //年

	TX_Buff[23] = it>>16&0XFF;  //月

	TX_Buff[24] = it>>8&0XFF;  //日

	TX_Buff[25] = it&0xFF;   //时
	
	TX_Buff[26] = 0;  //  是否支持同时生效多组动态密码 1 表示支持 0 表示不支持
	
	TX_Buff[27] = 0;  // 保留
	
	TX_Buff[28] = 0;  // 保留
		
	TX_Buff[29] = 0;  // 保留
		
	TX_Buff[30] = 0;  // 保留
	
	TX_Buff[31] = 0;  // 保留
	
	
	
	
	
	it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Mate_Ack_Len-4);i++)  //校验码除外的有效值相加
		{
			
			it +=TX_Buff[i+4];
		
		}
		
		if(Devic_Static==false) //当设备未绑定时 继续运行
		{
     
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Write_Devic_Num!\r\n");
			#endif
			
//			NRF_WDT_Feed();
			
			for(i =0;i<sizeof(Devic_Num);i++)
			Devic_Num[i] =0;
			
			for	(i =0;i<Mate_Dat_Len;i++)
			{
				Devic_Num[i] = BLE_Data[Mate_Com_Add+i];		//加上绑定数据位置
			}
			
			Spi_Flash_Init();  //开启SPI_FLASH 通讯
			Devic_static_read();  //(获取设备绑定状态)
			Write_Devic_Num();  //写入设备绑定信息
			SPI_Flash_Busy();
			Devic_static_read();  //(更新设备绑定状态)
			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
				
				
			Spi_Flash_Init();  //开启SPI_FLASH 通讯	

			if(BLE_Data[64]==1)    //根据输入参数设置 动态密码模式
			{
				//配置成多组动态密码模式
				
				Ev_Cat_Dat(More_Act_PassWord_Num_Code,0xFFFF0000);  
				//以高位两为 基准 高两位为0XFFFF时为 多组动态密码状态 
								
				TX_Buff[26] = 1;  //  是否支持同时生效多组动态密码 1 表示支持 0 表示不支持
	
			}
			else if(BLE_Data[64]==0)
			{
				//配置成单个时效动态密码模式
				Ev_Cat_Dat(More_Act_PassWord_Num_Code,0);  
				//以高位两为 基准 高两位为0XFFFF时为 多组动态密码状态 
				TX_Buff[26] = 0;  //  是否支持同时生效多组动态密码 1 表示支持 0 表示不支持
	
			}
			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
			
			
				it1 =it;
				it1&=0xff00;
				it1>>=8;
				TX_Buff[32] = it1;
				it1 = it;
				it1&=0x00ff;
				TX_Buff[33] = it1;
				TX_Buff[34] = BLE_DAT_E1;
				TX_Buff[35] = BLE_DAT_E2;  //打包好数据用于回应
					
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"TX_Buff_Dat:\r\n");
			
			for(i =0;i<40;i++)
			{
				SEGGER_RTT_printf(0,"%x,",TX_Buff[i]);
				if((i!=0)&&(i%20==0))
				{
					SEGGER_RTT_printf(0,"\r\n");
				}
			}
			SEGGER_RTT_printf(0,"\r\n");
			#endif
			
			if(Devic_Static==true) //绑定成功
			{
					for(i =0;i<20;i++)
					BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
				
					BLE_TX_Run(BLE_TX,20);   //发送第一个包
				
					for(i =0;i<20;i++)
					BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包		
				
					BLE_TX_Run(BLE_TX,20);   //发送第一个包				
					
					ref_BLE_Adv();     //更新广播名称
					//input_speak(0);
					input_speak(3); //最后语音提示绑定成功
					nrf_delay_ms(500);
			}
	//		read_Soctor(0);   //获取扇区0数据
	//		NRF_WDT_Feed();

		}
		else
		{
	//		input_speak(0);
		//	input_speak(10);
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_Static_true!\r\n");
			#endif
		}
		
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
}

#define Time_Ref_Ack_Len	22
uint8_t Update_Time_Flag =0;
extern uint32_t Board_Unix;
void Time_Ref_Run()
{
		uint32_t i,it,it1;
		uint64_t Input_Time;
		uint8_t TX_Buff[100];  //待发送数据
		uint8_t BLE_TX[20]; //一次发送的数据
	//	uint32_t err_code;
		
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Time_Ref_Run!\r\n");
		#endif
		
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;
		
		TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
		TX_Buff[1] = BLE_DAT_H2;
		
		TX_Buff[2] = 0x00;
		TX_Buff[3] = 0x10;  //长度
		
		TX_Buff[4] = 0X02;  //指令码
		TX_Buff[5] = 0x02;  //包标识
		TX_Buff[6] = BLE_Data[6]; //序号
		
		it1 = id1;			//设备标识
		it1 &=0xff00;
		it1>>=8;
	//	it1+=3;
		if(it1>127) it1 =0;
		
		if(BLE_Data[7]==it1)
		TX_Buff[7] =it1;
		else 
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
			#endif
			return;
		}
		
		it1 = id1;
		it1 &=0x00ff;
	//	it1+=5;
		if(BLE_Data[8]==it1)
		TX_Buff[8] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
		}
		
		it1 = id2;
		it1 &=0xff00;
		it1>>=8;
	//	it1+=9;

		if(BLE_Data[9]==it1)
		TX_Buff[9] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
		}	
		it1 = id2;
		it1 &=0x00ff;
		//it1+=6;
		if(BLE_Data[10]==it1)
		TX_Buff[10] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			
			return;
		}
	
		TX_Buff[11] = 0X01;  //表示绑定成功
		
		TX_Buff[12] = 0; //表示数据无效
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //无功能
		TX_Buff[17] = 0;  //无功能
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			Input_Time =0;
			Input_Time+= BLE_Data[11]*10000000000;
			Input_Time+= BLE_Data[12]*100000000;
			Input_Time+= BLE_Data[13]*1000000;
			Input_Time+= BLE_Data[14]*10000;
			Input_Time+= BLE_Data[15]*100;
			Input_Time+= BLE_Data[16];
			Update_Time(Input_Time);
			
				Update_Time_Flag =1;
				Get_Time();
				
				if(TIME_STATE==true)
				{
					Board_Unix = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);
				}
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包		
		
			BLE_TX_Run(BLE_TX,20);				
}

#define Get_Time_Ack 30
#define Base_Volt_Value  380000
#define Max_Volt_Value  600000

uint8_t Batt_Value_T =100;  //实际值

uint8_t Batt_Value()  //求电量等级
{
		uint32_t it1,it2;
		ADC_Value =nrf_adc_read();
//			Get_Time();  //定时更新时间
		#ifdef uart_start
		SEGGER_RTT_printf(0,"ADC_Value: %d\r\n",ADC_Value);
		#endif
	
		ADC_Div = VDD_Value/1024;
		VCC_Value = ADC_Div*ADC_Value*(RES_d+1);
	//VCC_Value+=20000; //矫正二极管导致误差

		VCC_Value_H = VCC_Value/100000;
		VCC_Value_L1 = VCC_Value/10000%10;
		VCC_Value_L2 = VCC_Value/1000%10;
		
#ifdef uart_start
	SEGGER_RTT_printf(0,"VCC_Value: %d.%d%dV\r\n",VCC_Value_H,VCC_Value_L1,VCC_Value_L2);
#endif
			//VCC_Value/=1000; //换算成三位数
			if(VCC_Value>=Max_Volt_Value)  //当值大于最高电压值，直接返回100
				return 100;
			else if(VCC_Value<=(Base_Volt_Value+5000))
				return 0;
			it2= VCC_Value-Base_Volt_Value;  //减去基础电压值
			it1 = Max_Volt_Value-Base_Volt_Value;
			it1/=100;
			it2 /=it1;
			
			if(it2>=Batt_Value_T)
				it2 = Batt_Value_T;
			else
				Batt_Value_T = it2;
		
				
			if(it2>100)
				return 0;
			else
			return it2;
}

void Get_Time_Run()  //运行时间获取指令
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Get_Time_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x18;  //长度
	
	TX_Buff[4] = 0X03;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	

	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	Get_Time();
	TX_Buff[11] = 0X01;  //表示成功获取时间
	
	TX_Buff[12] = YEAR; //年份
	TX_Buff[13] = MON; //月份
	TX_Buff[14] = DAY; //日期
	TX_Buff[15] = HOUR; //时间
	
	TX_Buff[16] = MIN;  //分
	TX_Buff[17] = SEC;  //秒
	
	TX_Buff[18] = 0x02;  //按下初始化键 //直接返回0X02
	
	Read_Lock_Bon();
	if(Turn_Key) //钥匙触发时
	TX_Buff[19] = 0x01;
	else 
	TX_Buff[19] = 0x02;
	
	if(Double_Key) //方舌触发时
	TX_Buff[20] = 0x01;
	else 
	TX_Buff[20] = 0x02;
	
	if(Latch_Bolt) //斜舌触发时
	TX_Buff[21] = 0x01;
	else 
	TX_Buff[21] = 0x02;
	
	TX_Buff[22] = 6; 							 //广播等级
	
	TX_Buff[23] = Batt_Value();  	//电量等级
	
	it = 0; it1 =0;

//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

			for(i =0;i<(Get_Time_Ack-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[24] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[25] = it1;
			TX_Buff[26] = BLE_DAT_E1;
			TX_Buff[27] = BLE_DAT_E2;  //打包好数据用于回应					
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包		
		
			BLE_TX_Run(BLE_TX,20);				
}

extern uint8_t PassWord_Error_Num;  //记录密码输入错误次数
extern uint32_t PassWord_Error_Time;  //错误锁定定时
#define PassWord_Error_Num_Max 6

void APP_OpenLock_Run()
{
	uint32_t i,it,it1;
	uint64_t Input_Time;		//时间
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"App_OpenLock_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = BLE_Data[4];  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	

	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
	//人脸识别机开门指令包含时间
	
	if(BLE_Data[11]>0)			//判断年份是否有效
	{
			Input_Time =0;
			Input_Time+= BLE_Data[11]*10000000000;
			Input_Time+= BLE_Data[12]*100000000;
			Input_Time+= BLE_Data[13]*1000000;
			Input_Time+= BLE_Data[14]*10000;
			Input_Time+= BLE_Data[15]*100;
			Input_Time+= BLE_Data[16];
			Update_Time(Input_Time);
			
			Update_Time_Flag =1;
			Get_Time();
			
			if(TIME_STATE==true)
			{
				Board_Unix = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);
			}
	}
	
	
	
	Spi_Flash_Init();  //开启SPI_FLASH 通讯
	
	Devic_static_read();  //(获取设备绑定状态)
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
	
	
	#ifdef APP3_Op_Door 		//当允许 APP3 开门指令时
	
	if(BLE_Data[4]==APP3_OpenLock_Command)    //当指令是第三方开门指令时   //需要校验客码是否正确
	{
		
		
	if(BLE_Data[11]==Devic_Num[28]);
		else
			return;
	
	if(BLE_Data[12]==Devic_Num[29]);
		else
			return;
	
	if(BLE_Data[13]==Devic_Num[30]);
		else
			return;
	
	if(BLE_Data[14]==Devic_Num[31]);
		else
			return;
	
	}
	
	#endif
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口
//	nrf_delay_ms(1); //等待稳定
//	if(nrf_gpio_pin_read(5)==0)
//	TX_Buff[11] = 0x02;  //当门反锁时不予开门
//	else
	
	TX_Buff[11] = 0X01;  //表示开门成功
	TX_Buff[12] = Batt_Value();			//载入电量值
	
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);

	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //无功能
	TX_Buff[17] = 0;  //无功能
	
	it = 0; it1 =0;
	
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);						
			
			
			if(TX_Buff[11]==0X01) //当允许开门时
			{
			
				if(MPR121_RUN==0)
				nrf_gpio_pin_set(LEDB);  //开门后打开LED灯
				Lock_Open();
				
			}
//			else
//			input_speak(17); //语音提示已反锁
//		
		//	mpr121_Run_I(); 
//			mpr121_init_I();  //重新读取一次基线值
//			mpr121_init();
			TSM12_Init();
			nrf_gpio_pin_clear(LEDB);  //关灯
			
		
			if(PassWord_Error_Num>=PassWord_Error_Num_Max)
				PassWord_Error_Time = 9000;// 解锁键盘
			
			
//			for(i =0;i<20;i++)
//			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
//		
//			BLE_TX_Run(BLE_TX,20);
//			for(i =0;i<20;i++)
//			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
//			BLE_TX_Run(BLE_TX,20);						
//			
}

/*
void APP_OpenLock_Run()
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"App_OpenLock_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = BLE_Data[4];  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	

	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
	Spi_Flash_Init();  //开启SPI_FLASH 通讯
	
	Devic_static_read();  //(获取设备绑定状态)
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
	
	
	#ifdef APP3_Op_Door 		//当允许 APP3 开门指令时
	
	if(BLE_Data[4]==APP3_OpenLock_Command)    //当指令是第三方开门指令时   //需要校验客码是否正确
	{
		
		
	if(BLE_Data[11]==Devic_Num[28]);
		else
			return;
	
	if(BLE_Data[12]==Devic_Num[29]);
		else
			return;
	
	if(BLE_Data[13]==Devic_Num[30]);
		else
			return;
	
	if(BLE_Data[14]==Devic_Num[31]);
		else
			return;
	
	}
	
	#endif
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口
//	nrf_delay_ms(1); //等待稳定
//	if(nrf_gpio_pin_read(5)==0)
//	TX_Buff[11] = 0x02;  //当门反锁时不予开门
//	else
	
	TX_Buff[11] = 0X01;  //表示开门成功
	
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);

	TX_Buff[12] = 0; //表示数据无效
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //无功能
	TX_Buff[17] = 0;  //无功能
	
	it = 0; it1 =0;
	
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);						
			
			
			if(TX_Buff[11]==0X01) //当允许开门时
			{
			
				if(MPR121_RUN==0)
				nrf_gpio_pin_set(LEDB);  //开门后打开LED灯
				Lock_Open();
				
			}
//			else
//			input_speak(17); //语音提示已反锁
//		
		//	mpr121_Run_I(); 
//			mpr121_init_I();  //重新读取一次基线值
//			mpr121_init();
			TSM12_Init();
			nrf_gpio_pin_clear(LEDB);  //关灯
			
		
			if(PassWord_Error_Num>=PassWord_Error_Num_Max)
				PassWord_Error_Time = 9000;// 解锁键盘
			
			
//			for(i =0;i<20;i++)
//			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
//		
//			BLE_TX_Run(BLE_TX,20);
//			for(i =0;i<20;i++)
//			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
//			BLE_TX_Run(BLE_TX,20);						
//			
}
*/
uint8_t EX_BLE_ACK_TX[100];  //通过外部完成BLE ACK TX 缓存 (主要针对录入数据过程)
void Add_Information_Run() //运行信息载入指令
{
	uint8_t PassWord[50];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	uint32_t Start_Unix; //有效时间
	uint32_t Stop_Unix;  //失效时间
	uint16_t Card_ID;		//缓存获取到的卡序号
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Add_Information_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = 0X12;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	

	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
		TX_Buff[11] = 0X01;  //表示操作成功
		
		TX_Buff[12] = 0; //表示数据无效
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //无功能
		TX_Buff[17] = 0;  //无功能
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;
	
	
		for(i =0;i<sizeof(PassWord);i++)PassWord[i] =0x00; //清空缓存区
		
		if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x02)) //管理员密码 (第一字节01表示管理员，第二字节02表示密码)
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_PassWord!\r\n");
			#endif
			
			PassWord[0] = 0x12; //表示管理员密码 //将两字节整合
			for(i =1;i<18;i++)
			{
				PassWord[i] = BLE_Data[12+i];
			} //将密码载入
	
			Spi_Flash_Init();  //开启SPI_FLASH 通讯	
		
			TX_Buff[11] = 0X01;  //表示操作成功
			TX_Buff[12] = Add_Flash_PassWord(PassWord); //写入保存ID号
//			NRF_WDT_Feed();	
			if(TX_Buff[12]==0) //当写入返回错误
			{
				TX_Buff[11] =0x02;
			}
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x02)) //用户密码 (第一字节02表示租客，第二字节02表示密码)
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_PassWord!\r\n");
			#endif
			
			PassWord[0] = 0x22; //表示用户密码 //将两字节整合
			for(i =1;i<18;i++)
			{
				PassWord[i] = BLE_Data[12+i];
			} //将密码载入
	
			Spi_Flash_Init();  //开启SPI_FLASH 通讯	
			
			TX_Buff[11] = 0X01;  //表示操作成功
			TX_Buff[12] = Add_Flash_PassWord(PassWord); //写入保存ID号
//			NRF_WDT_Feed();
			if(TX_Buff[12]==0) //当写入返回错误提示操作失败
			{
				TX_Buff[11] =0x02;
			}
		}
		else if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x03))  //管理员 Type A 卡
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_TypeA_Card!\r\n");
			#endif
			
			Spi_Flash_Init();  //开启Flash 通讯
			
			Start_Unix = 0;
			Stop_Unix =0;  //初始化 Unix 时间
			
			for(i =0;i<4;i++)
			  PassWord[i] = BLE_Data[21+i];  //使用 PassWord 字符表 保存 指令包参数中 权限模式 权限类型和时间戳后的数据
			
			 Start_Unix = BLE_Data[13];
			 Start_Unix <<=8;
			
       Start_Unix |= BLE_Data[14];
			 Start_Unix <<=8;
			
			 Start_Unix |= BLE_Data[15];
			 Start_Unix <<=8;
			 
			 Start_Unix |= BLE_Data[16];
			 
			 Stop_Unix = BLE_Data[17];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[18];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[19];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[20];
			 
			 TX_Buff[11] = 0x01;
			
			 TX_Buff[12] = 0;
				
			 Card_ID = Add_Flash_AD_TypeA_Data (PassWord,Start_Unix,Stop_Unix);
			 
			 TX_Buff[13] = Card_ID>>8&0xFF;
			 
			 TX_Buff[14] = Card_ID&0xFF;
			 
				if(Card_ID==0)  //判断数据是否写入成功 写入失败时改变 TX_Buff[11] 的状态
					TX_Buff[11] =0x02;
			
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x03))  //用户 Type A 卡
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_TypeA_Card!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //初始化 Unix 时间
			
			for(i =0;i<4;i++)
				PassWord[i] = BLE_Data[21+i];  //使用 PassWord 字符表 保存 指令包参数中 权限模式 权限类型和时间戳后的数据
						
			Start_Unix = BLE_Data[13];
			 Start_Unix <<=8;
			
       Start_Unix |= BLE_Data[14];
			 Start_Unix <<=8;
			
			 Start_Unix |= BLE_Data[15];
			 Start_Unix <<=8;
			 
			 Start_Unix |= BLE_Data[16];
				
			 Stop_Unix = BLE_Data[17];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[18];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[19];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[20];
			 
			 TX_Buff[11] = 0x01;
			 
			 TX_Buff[12] = 0;
				
			 Card_ID = Add_Flash_US_TypeA_Data (PassWord,Start_Unix,Stop_Unix);
			 
			 TX_Buff[13] = Card_ID>>8&0xFF;
			 
			 TX_Buff[14] = Card_ID&0xFF;
			 
				if(Card_ID==0)  //判断数据是否写入成功 写入失败时改变 TX_Buff[11] 的状态
					TX_Buff[11] =0x02;
			
		}
		else if((BLE_Data[11]==0X01)&&(BLE_Data[12]==0x04))   //管理员 Type B 卡
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_TypeB_Card!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //初始化 Unix 时间
			
			for(i =0;i<8;i++)
				PassWord[i] = BLE_Data[21+i];  //使用 PassWord 字符表 保存 指令包参数中 权限模式 权限类型和时间戳后的数据
			
			 Start_Unix = BLE_Data[13];
			 Start_Unix <<=8;
			
       Start_Unix |= BLE_Data[14];
			 Start_Unix <<=8;
			
			 Start_Unix |= BLE_Data[15];
			 Start_Unix <<=8;
			 
			 Start_Unix |= BLE_Data[16];
			 
			 
			 Stop_Unix = BLE_Data[17];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[18];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[19];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[20];
			 
			 TX_Buff[11] = 0x01;
			 
			 TX_Buff[12] = 0;
				
			 Card_ID = Add_Flash_AD_TypeB_Data (PassWord,Start_Unix,Stop_Unix);
			 
			 TX_Buff[13] = Card_ID>>8&0xFF;
			 
			 TX_Buff[14] = Card_ID&0xFF;
			 
				if(Card_ID==0)  //判断数据是否写入成功 写入失败时改变 TX_Buff[11] 的状态
					TX_Buff[11] =0x02;
			
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x04))  //用户 Type B 卡
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_TypeB_Card!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //初始化 Unix 时间
			
			for(i =0;i<8;i++)
				PassWord[i] = BLE_Data[21+i];  //使用 PassWord 字符表 保存 指令包参数中 权限模式 权限类型和时间戳后的数据
				
			 Start_Unix = BLE_Data[13];
			 Start_Unix <<=8;
			
       Start_Unix |= BLE_Data[14];
			 Start_Unix <<=8;
			
			 Start_Unix |= BLE_Data[15];
			 Start_Unix <<=8;
			 
			 Start_Unix |= BLE_Data[16];
			
			 Stop_Unix = BLE_Data[17];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[18];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[19];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[20];
			 
			 TX_Buff[11] = 0X01;
			 
			 TX_Buff[12] = 0;
				
			 Card_ID = Add_Flash_US_TypeB_Data (PassWord,Start_Unix,Stop_Unix);
			 
			 TX_Buff[13] = Card_ID>>8&0xFF;
			 
			 TX_Buff[14] = Card_ID&0xFF;
			 
				if(Card_ID==0)  //判断数据是否写入成功 写入失败时改变 TX_Buff[11] 的状态
					TX_Buff[11] =0x02;
			
		}
		
		#ifdef PS_Enable
		
		else if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x01)&&(F_print_Enable==1))			//管理员指纹
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_PS_Lmage!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //初始化 Unix 时间
			
		 Start_Unix = BLE_Data[13];				//载入起始时间和结束时间
		 Start_Unix <<=8;
		
		 Start_Unix |= BLE_Data[14];
		 Start_Unix <<=8;
		
		 Start_Unix |= BLE_Data[15];
		 Start_Unix <<=8;
		 
		 Start_Unix |= BLE_Data[16];
		
		 Stop_Unix = BLE_Data[17];
		 Stop_Unix <<=8;
		 
		 Stop_Unix |= BLE_Data[18];
		 Stop_Unix <<=8;
		 
		 Stop_Unix |= BLE_Data[19];
		 Stop_Unix <<=8;
		 
		 Stop_Unix |= BLE_Data[20];
		 
		 input_speak(31);

		Input_Ps_Lmage_Type =0x01;				//初始化指纹录入相关值 设置为管理员指纹
		Start_PS_Unix = Start_Unix;				//生效时间
		Stop_PS_Unix = Stop_Unix;					//失效时间
		
		for(i= 0;i<sizeof(TX_Buff);i++)				//这部分不使用此函数的应答包，应答使用即时外部应答的方式完成
			EX_BLE_ACK_TX[i] = TX_Buff[i];
		
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x01)&&(F_print_Enable==1))			//用户指纹
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_PS_Lmage!\r\n");
			#endif
			
			
			 Start_Unix = BLE_Data[13];			//载入起始时间和结束时间
			 Start_Unix <<=8;
			
       Start_Unix |= BLE_Data[14];
			 Start_Unix <<=8;
			
			 Start_Unix |= BLE_Data[15];
			 Start_Unix <<=8;
			 
			 Start_Unix |= BLE_Data[16];
			
			 Stop_Unix = BLE_Data[17];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[18];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[19];
			 Stop_Unix <<=8;
			 
			 Stop_Unix |= BLE_Data[20];
			 
				input_speak(31);

			
			Input_Ps_Lmage_Type =0x02;				//初始化指纹录入相关值 设置为管理员指纹
			Start_PS_Unix = Start_Unix;				//生效时间
			Stop_PS_Unix = Stop_Unix;					//失效时间 

			for(i= 0;i<sizeof(TX_Buff);i++)				//这部分不使用此函数的应答包，应答使用即时外部应答的方式完成
				EX_BLE_ACK_TX[i] = TX_Buff[i];
				
		
		}
		#endif
		
		else
		{	//操作失败 //未扩展其他写入功能
			TX_Buff[11] = 0x02; //表示操作失败
			TX_Buff[12] = 0X00;
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Add_Information_Order_Warning!\r\n");  //载入信息功能溢出
			
			#endif
	//		Spi_Flash_Init();
		}
	
			if(TX_Buff[11]==0X01&&(BLE_Data[12]==0x02)) // 载入密码提示操作成功
		input_speak(9);
		else if(TX_Buff[11]==0x02)			// 失败时提示操作失败
			input_speak(10);
			
	
//最后获取数据校验码
	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
		{
			it +=TX_Buff[i+4];
		}
		
		it1 =it;
		it1&=0xff00;
		it1>>=8;
		TX_Buff[18] = it1;
		it1 = it;
		it1&=0x00ff;
		TX_Buff[19] = it1;
		TX_Buff[20] = BLE_DAT_E1;
		TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
		
		#ifdef PS_Enable
		if((BLE_Data[12]!=0x01)||(F_print_Enable==0))			//仅在非指纹录入的情况下允许在当前函数内应答蓝牙报文
		{
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
		}
		#else
		
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
		
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
		
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
		
		#endif
			

		
//			NRF_WDT_Feed();  //验证用
//			read_Soctor(0);
//			NRF_WDT_Feed();
//			read_Soctor(1);
//			NRF_WDT_Feed();
//			read_Soctor(2);
//			NRF_WDT_Feed();

		
}

/***********************************

外部 BLE 应答发送

输入： ACK_Type		应答类型(成功或失败)  0X01 表示成功  0X02  表示失败

***********************************/

void EX_BLE_ACK_Run(uint8_t ACK_Type,uint8_t ACK_ID)
{
	
	uint16_t it,it1;
	
	uint8_t BLE_TX[20]; //一次发送的数据
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_EX_BLE_ACK_RUN!\r\n");
	
	#endif
	
	if((EX_BLE_ACK_TX[0]==BLE_DAT_H1)&&
		 (EX_BLE_ACK_TX[1]==BLE_DAT_H2))
	{
		;
	}
	else 
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"EX_BLE_ACK_BUFF_Error!\r\n");			//提示包异常
		
		#endif
		
		return;
		
	}
	if(ACK_Type==0X01)		//输入成功时
	{
		EX_BLE_ACK_TX[11]=ACK_Type;
	}
	else if(ACK_Type==0x02)		//输入失败时
	{
		EX_BLE_ACK_TX[11]=ACK_Type;
	}
	else
	{
		for(uint8_t i;i<sizeof(EX_BLE_ACK_TX);i++)
		{
			EX_BLE_ACK_TX[i] =0;		//清空缓存
		}
		return;		//输入其他值无效，直接结束
		
	}
	
	if(BLE_LINK==0)		//当蓝牙未连接时不发送数据，并输出错误
	{
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"BLE_Link_Error!\r\n");		//提示BLE连接错误
		
		#endif
		for(uint8_t i;i<sizeof(EX_BLE_ACK_TX);i++)
		{
			EX_BLE_ACK_TX[i] =0;		//清空缓存
		}
		return;
		
	}
	
	EX_BLE_ACK_TX[12] = ACK_ID;			//需要返回的ID
	
	//最后获取数据校验码
	for(uint16_t i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
		{
			it +=EX_BLE_ACK_TX[i+4];
		}
		
		it1 =it;
		it1&=0xff00;
		it1>>=8;
		EX_BLE_ACK_TX[18] = it1;
		it1 = it;
		it1&=0x00ff;
		EX_BLE_ACK_TX[19] = it1;
		EX_BLE_ACK_TX[20] = BLE_DAT_E1;
		EX_BLE_ACK_TX[21] = BLE_DAT_E2;  //打包好数据用于回应
		
		if(BLE_Data[12]==0x01)			//仅在指纹录入或其他特殊的情况下允许在当前函数内应答蓝牙报文
		{
			for(uint8_t i =0;i<20;i++)
			BLE_TX[i] =EX_BLE_ACK_TX[i]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
			for(uint8_t i =0;i<20;i++)
			BLE_TX[i] = EX_BLE_ACK_TX[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
		}
		else
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"EX_BLE_ACK_BUFF_Type_Error!\r\n");		//提示应答包目标错误
			
			#endif
			
			return;		//结束运行
		}
		
}

void updata_Devic_Num()//更新设备绑定数据
{
	uint8_t read_tab [Devic_PAGE_Len];
	uint8_t tab[Devic_PAGE_Len];
	uint32_t i;
//	uint32_t dat;
	i =0;
	SpiFlash_Read_Data
	(read_tab,Devic_Num_BLOCK,
	Devic_Num_PAGE,Devic_PAGE_Len); //获取数据确定数据有效性
	
	if(!((read_tab[0] == PAGE_Heart_H)&&
		(read_tab[1] == PAGE_Heart_L))) //判断起始数据有效性 只修改有效数据
		{
			//当起始数据无效时，不对绑定区做出改变
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_Num not Write!\r\nDevic_static_true!\r\n");
			
			#endif
		}
		else
		{  //当起始数据有效时，开始写入数据
			SPIFlash_Erase_Sector(Devic_Num_BLOCK,Devic_Num_Sector); //对设备绑定号所在扇区进行擦除
			
			SPI_Flash_Busy(); //等待擦除完成
			
			tab[0] = PAGE_Heart_H;
			tab[1] = PAGE_Heart_L;
			for (i =0;i<Devic_Num_Len;i++)
			{
				tab[i+2] = Devic_Num[i];
			}
			SpiFlash_Write_Data(tab,Devic_Num_BLOCK,Devic_Num_PAGE,sizeof(tab));
			SPI_Flash_Busy(); //等待写入完成
			
			SpiFlash_Read_Data (read_tab,Devic_Num_BLOCK,Devic_Num_PAGE,Devic_PAGE_Len);
			#ifdef uart_start 
			SEGGER_RTT_printf(0,"Devic_Num Write OK!\r\nDevic_Num:\r\n");
			for(i =0;i<sizeof(read_tab);i++)
			SEGGER_RTT_printf(0,"%x,",read_tab[i]);
			SEGGER_RTT_printf(0,"\r\n",read_tab[i]);
			
			#endif
		}
}


extern const uint8_t Act_PassWord_Secotr ;  //存储动态密码 

void Del_Information_Run()
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint32_t Block,Page;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	uint32_t Code_Num;  //删除卡时的缓存
		
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Del_Information_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = 0X15;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	
	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	TX_Buff[11] = 0X01;  //表示操作成功
	TX_Buff[12] = 0; //表示数据无效
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //无功能
	TX_Buff[17] = 0;  //无功能
	it = 0; it1 =0;

	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
	{
		it +=TX_Buff[i+4];
	}
	it1 =it;
	it1&=0xff00;
	it1>>=8;
	TX_Buff[18] = it1;
	it1 = it;
	it1&=0x00ff;
	TX_Buff[19] = it1;
	TX_Buff[20] = BLE_DAT_E1;
	TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
	
	if(BLE_Data[11]==0x01) //清除管理员信息
	{
		
		Spi_Flash_Init();  //开启SPI_FLASH 通讯	
		Del_Admin_PassWord();
		Devic_static_read();  //更新 设备码信息(用于刷新部分绑定数据)
		//清除管理员密码时，需要更新绑定数据中的主体校验码
		//		绑定数据内容：
		//四字节账号编号+24字节广播名称ASIIC码+
		//四字节设备校验码+四字节主体校验码+2字节楼层编号
		Devic_Num[32] = BLE_Data[12];
		Devic_Num[33] = BLE_Data[13];
		Devic_Num[34] = BLE_Data[14];
		Devic_Num[35] = BLE_Data[15]; //替换主体校验码
		
		updata_Devic_Num();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_AD_TypeB_Data!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);
		
		if(Code_Num!=0)			//当有管理员B卡时清空管理员B卡
		{
			Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //清空管理员 Type B 卡信息
			Erase_AD_TypeB_Sector();
		}
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_AD_TypeA_Data!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);
		if(Code_Num!=0)			//当有管理员 A 卡时清空管理员A卡数据
		{
		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //清空管理员 Type A 卡信息
		Erase_AD_TypeA_Sector();			//清空 管理员 A 卡扇区数据
			
		}
		Ev_Cat_Dat(Nmg_Auth_Index_Code,0); //将管理员 数据卡序列号清空
		
	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_Nmg_Auth_Index_Code!\r\n");
		
		#endif
		
	}
	else if(BLE_Data[11]==0x02) //清除用户密码
	{
		Spi_Flash_Init();  //开启SPI_FLASH 通讯	
		Del_User_PassWord();
		Ev_Cat_Dat(0x04,0);		//清空永久密码
		Devic_static_read();  //更新 设备码信息(用于刷新部分绑定数据)
		//清除租客密码时，需呀更新绑定数据中的设备校验码 (同时清除动态密码)
		Devic_Num[28] = BLE_Data[12];
		Devic_Num[29] = BLE_Data[13];
		Devic_Num[30] = BLE_Data[14];
		Devic_Num[31] = BLE_Data[15]; //替换设备校验码
		
		updata_Devic_Num();
		Ev_Cat_Dat(0x02,0);  //将动态密码数量清除
		Ev_Cat_Dat(0x03,0);  //将历史动态密码数量清除
		
		for(i =0;i<sizeof(tab_dat);i++)
		tab_dat[i] =0x00;
		
		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,2); //将已经保存的时效密码覆盖
		SPI_Flash_Busy();	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_TypeB_Data!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //清空用户 Type B 卡信息
		Erase_US_TypeB_Sector();
		}
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_TypeB_Data!\r\n");
		
		#endif
		
		
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //清空用户 Type A 卡信息
		Erase_US_TypeA_Sector();
		}
		
		Ev_Cat_Dat(CK_Auth_Index_Code,0); //将用户 数据卡序列号清空
		
		Code_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);    //增加清空多组用户钥匙的指令
		
		Ev_Cat_Dat(More_Act_PassWord_Num_Code,Code_Num&0xFFFF0000);  //清空用户 Type B 卡信息
				
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_CK_Auth_Index_Code!\r\n");
		
		#endif
		
	}
		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep();
	
		for(i =0;i<20;i++)
		BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
	
		BLE_TX_Run(BLE_TX,20);
		for(i =0;i<20;i++)
		BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
		BLE_TX_Run(BLE_TX,20);	
	
	// 	input_speak(9);
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
	
}

void Del_User_Key()		//清空用户钥匙(所有能开门的用户信息)
{
//	uint16_t i;
	uint32_t Code_Num;
//	uint32_t Block,Page;

	Del_User_PassWord();		//清空用户自定义密码
	
	//	Ev_Cat_Dat(0x04,0);  //清空永久密码数量
	
//	Ev_Cat_Dat(0x02,0);  //将动态密码数量清除
//	Ev_Cat_Dat(0x03,0);  //将历史动态密码数量清除
	
//	for(i =0;i<sizeof(tab_dat);i++)			//清空缓存
//		tab_dat[i] =0x00;
//		
//		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
//		SPIFlash_Write_Tab(tab_dat,Block,Page,2); //将已经保存的时效密码覆盖
//		SPI_Flash_Busy();	
//	
			Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //清空用户 Type B 卡信息
		Erase_US_TypeB_Sector();
		}
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_TypeB_Data!\r\n");
		
		#endif
		
		
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //清空用户 Type A 卡信息
		Erase_US_TypeA_Sector();
		}
		
		Ev_Cat_Dat(CK_Auth_Index_Code,0); //将用户 数据卡序列号清空
		
		Code_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);    //增加清空多组用户钥匙的指令
		
		Ev_Cat_Dat(More_Act_PassWord_Num_Code,Code_Num&0xFFFF0000);  //清空用户 Type B 卡信息
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_CK_Auth_Index_Code!\r\n");
		
		#endif
	
	
}


/*********************************************

当删除卡时，只区分管理员与用户卡，不对单张卡处理

**********************************************/

void Del_A_PassWord_Run()  //删除单个密码数据 或卡数据
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	uint32_t Code_Num;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Del_A_PassWord_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = 0X14;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	
	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
	TX_Buff[11] = 0X01;  //表示操作成功
	TX_Buff[12] = 0; //表示数据无效
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //无功能
	TX_Buff[17] = 0;  //无功能
	it = 0; it1 =0;
	
	
	Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	
	if(BLE_Data[13]==0x02) //当指令为删除密码指令时 由于密码由独立SN码控制，故不用考虑是否为管理员
	{
		
		if(Del_SN_PassWord(BLE_Data[12])==1)  //判断删除的密码是否成功删除
		{
			
			TX_Buff[11] = 0x01;
			
		}
		else
		{
			
			TX_Buff[11]=0x02;
		}
		
	}
	else if ((BLE_Data[13]==0x03)&&(BLE_Data[11]==0x01))  // 当指令为 管理员 Type_A 卡的时候
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nDel_AD_TypeA_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);  //判断本地数据是否大于1 否则不清空
		
		if(Code_Num!=0)
		{
			Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //清空管理员 A 卡
			Erase_AD_TypeA_Sector();
		}
		Code_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);
		
		if(Code_Num!=0)
		{
			Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //清空管理员 B 卡
			Erase_AD_TypeB_Sector();
		}
		
		TX_Buff[11] =0X01;
		
		
		
//		if(Del_AD_Type_A(BLE_Data[12]))  //不处理单张卡
//			TX_Buff[11] =0X01;
//		else
//		TX_Buff[11]=0x02;
	
	}
	else if ((BLE_Data[13]==0x03)&&(BLE_Data[11]==0x02))  //当指令为 用户 Type_A 卡时
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nDel_US_TypeA_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);  //判断本地数据是否大于1 否则不清空
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //清空用户 A 卡
		Erase_US_TypeA_Sector();
		}

		Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //清空用户 B 卡
		Erase_US_TypeB_Sector();
		}
		
		TX_Buff[11] =0X01;
		
//		if(Del_US_Type_A(BLE_Data[12]))   //不处理单张卡
//			TX_Buff[11]=0x01;
//		else
//		TX_Buff[11]=0x02;
	}
	else if ((BLE_Data[13]==0x04)&&(BLE_Data[11]==0x01))  // 当指令为 管理员 Type_B 卡时
	{
		
		#ifdef  uart_start 
		
		SEGGER_RTT_printf(0,"\r\nDel_AD_TypeB_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);  //判断本地数据是否大于1 否则不清空
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //清空管理员 A 卡
		Erase_AD_TypeB_Sector();
		}
		Code_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //清空管理员 A 卡
		Erase_AD_TypeA_Sector();
		}
//		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //清空用户 A 卡
//		Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //清空用户 B 卡
//		
		TX_Buff[11] =0X01;
		
//		if(Del_AD_Type_B(BLE_Data[12]))  //不处理单张卡
//			TX_Buff[11] =0X01;
//		else
//		TX_Buff[11]=0x02;
	}
	else if((BLE_Data[13]==0X04)&&(BLE_Data[11]==0X02))  //当指令为 用户 Type_A 卡时
	{
		
		#ifdef  uart_start
		
		SEGGER_RTT_printf(0,"\r\nDel_US_TypeB_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);  //判断本地数据是否大于1 否则不清空
		
		if(Code_Num!=0)
		{
			
			Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //清空用户 B 卡
			Erase_US_TypeB_Sector();
		
		}
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //清空用户 A 卡
		Erase_US_TypeA_Sector();
			
		}
		TX_Buff[11] =0X01;
		
//		if(Del_US_Type_B(BLE_Data[12]))  //不处理单张卡
//			TX_Buff[11] =0X01;
//		else
//		TX_Buff[11]=0x02;
	}
	else if(BLE_Data[13]==0x01)			//当收到删除指纹指令时，开始删除指纹(与密码一样，不区分管理员与用户)
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"\r\n Del_PS_Print!\r\n	");
		#endif
		if(Del_A_ID_PS_Print(BLE_Data[12])==1)
		{
					TX_Buff[11] =0X01;   //删除成功
		}
		else
		{
			TX_Buff[11] = 0x02;		//反之则失败
		}
	}
	
			
	
	for (i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
	{
		
		it +=TX_Buff[i+4];
		
	}
	
	it1 =it;
	it1&=0xff00;
	it1>>=8;
	TX_Buff[18] = it1;
	it1 = it;
	it1&=0x00ff;
	TX_Buff[19] = it1;
	TX_Buff[20] = BLE_DAT_E1;
	TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应


	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();	
	
	for(i =0;i<20;i++)
	BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包

	BLE_TX_Run(BLE_TX,20);
	for(i =0;i<20;i++)
	BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
	BLE_TX_Run(BLE_TX,20);	
	  
	// 	input_speak(9);
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();	
}

void Del_Mate_Run() //解绑设备
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Del_Mate_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = 0X30;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
		
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	
	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
	TX_Buff[11] = 0X01;  //表示操作成功
	TX_Buff[12] = 0; //表示数据无效
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //无功能
	TX_Buff[17] = 0;  //无功能
	it = 0; it1 =0;
	
	
	Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	
//	if(BLE_Data[13]==0x02) //当指令为删除密码指令时
//	{
//		if(Del_A_PassWord(BLE_Data[12])==1)
//		{
//			TX_Buff[11] = 0x01;
//		}
//		else
//		{
//			TX_Buff[0x02]=0x02;
//		}
//	}
	//开始解绑设备
	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
	{
		it +=TX_Buff[i+4];
	}
	it1 =it;
	it1&=0xff00;
	it1>>=8;
	TX_Buff[18] = it1;
	it1 = it;
	it1&=0x00ff;
	TX_Buff[19] = it1;
	TX_Buff[20] = BLE_DAT_E1;
	TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应

//	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
//	Spi_Flash_Sleep();	

	for(i =0;i<20;i++)
	BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包

	BLE_TX_Run(BLE_TX,20);
	for(i =0;i<20;i++)
	BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
	BLE_TX_Run(BLE_TX,20);	

	  
	// 	input_speak(9);
	//开始初始化
 	SPIFlash_Erase_Sector(0,0);  //预先清空一次扇区0 以清空密码状态与绑定信息
	CL_FlashWord_Num();  //清除Flash 目录信息(由于是重置设备，故清除目录信息)
	CL_Flash_PassWord(); //重置设备时 清除Flash 密码信息 only Clean
	CL_Flash_OPEN_DOOR_NOTE();  //重置设备时 清除Flash 开门记录信息
	Erase_AD_TypeA_Sector();
	Erase_US_TypeA_Sector();
	Erase_AD_TypeB_Sector();
	Erase_US_TypeB_Sector();
	
	
	SPI_Flash_Busy();		
	Devic_static_read();
	
	
	
	input_speak(4);
	ref_BLE_Adv();  //重置后刷新BLE广播名称
	
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();	
}


const uint8_t R_ODoor_note_ACK_Len = 40;   //应答开门记录数据包长度


uint16_t TX_Door_X;  //开门记录上限
//uint32_t Note_Num_1 =0;
uint32_t Note_Out_Num =0;
static uint32_t Note_Num,Note_Num1;
void Read_OP_Door_note()  //读取开门记录指令 （改为应答本地开门记录数量）
{
	
//	uint8_t PassWord[18];

	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;

	
//	uint16_t Note_R_Num;  //读取条数(从后往前)
//	uint8_t key_Num;  //开门编号
//	uint8_t Key_Type;  //开门方式
//	uint32_t Unix_t;
	//
	Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	
	Note_Num = Get_Cat_Dat(OpenDoorNote_Num_Code); //获取开门记录数量
	
	#ifdef uart_start
	
	SEGGER_RTT_printf (0,"Read_Op_Note_Run!\r\n");
	SEGGER_RTT_printf (0,"Read_Op_Note_Num: %d!\r\n",Note_Num);
	
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
		TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
		TX_Buff[1] = BLE_DAT_H2;
		
		TX_Buff[2] = 0x00;
		TX_Buff[3] = 0x18;  //长度
		
		TX_Buff[4] = Read_OP_Door_note_Command;  //指令码
		TX_Buff[5] = 0x02;  //包标识
		TX_Buff[6] = BLE_Data[6]; //序号
		
		it1 = id1;			//设备标识
		it1 &=0xff00;
		it1>>=8;
	//	it1+=3;
		if(it1>127) it1 =0;
		if(BLE_Data[7]==it1)
		TX_Buff[7] =it1;
		else 
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
			#endif
			return;
		}
		it1 = id1;
		it1 &=0x00ff;
	//	it1+=5;
		if(BLE_Data[8]==it1)
		TX_Buff[8] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
		}
		it1 = id2;
		it1 &=0xff00;
		it1>>=8;
	//	it1+=9;

		if(BLE_Data[9]==it1)
		TX_Buff[9] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
		}	
		it1 = id2;
		it1 &=0x00ff;
		//it1+=6;
		
		if(BLE_Data[10]==it1)
		TX_Buff[10] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			
			return;
		}
		
		Note_Num1 = Note_Num;		//用于计算高位
		Note_Num1>>=8;
		
		TX_Buff[11] = Note_Num1&0xFF;  //开门记录总数  应答开门记录总数
		TX_Buff[12] = Note_Num&0XFF; //开门记录总数
		TX_Buff[13] = 0x00; //当前第几条
		TX_Buff[14] = 0x00; //当前第几条
		TX_Buff[15] = 0; //开门权限编号
		TX_Buff[16] = 0;  //开门权限类型
		TX_Buff[17] = 0x00;  //开门时间戳
		TX_Buff[18] = 0x00;  //开门时间戳
		TX_Buff[19] = 0x00;  //开门时间戳
		TX_Buff[20] = 0x00;  //开门时间戳
		
		TX_Buff[21] = 0; //无功能
		TX_Buff[22] = 0; //无功能
		TX_Buff[23] = 0; //无功能
		TX_Buff[24] = 0; //无功能
		TX_Buff[25] = 0; //无功能
		
		it = 0; it1 =0;
		//开始解绑设备
		for(i =0;i<(R_ODoor_note_ACK_Len-4);i++)  //校验码除外的有效值相加
		{
			it +=TX_Buff[i+4];
		}
		it1 =it;
		it1&=0xff00;
		it1>>=8;
		TX_Buff[36] = it1;
		it1 = it;
		it1&=0x00ff;
		TX_Buff[37] = it1;
		TX_Buff[38] = BLE_DAT_E1;
		TX_Buff[39] = BLE_DAT_E2;  //打包好数据用于回应

		for(i =0;i<20;i++)
		BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包

		BLE_TX_Run(BLE_TX,20);
	//	nrf_delay_ms(100);
		for(i =0;i<20;i++)
		BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
		BLE_TX_Run(BLE_TX,20);	 
		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep();
		
}
	//	


extern uint8_t Read_Op_Tab[];

void TX_Door_note()  //读取对应编号的本地开门记录  0x42指令
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
//	uint16_t Note_Num;
	
//	uint16_t Note_R_Num;  //读取条数(从后往前)
	uint16_t key_Num;  //开门记录编号
		
	//	Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	
		#ifdef uart_start
		SEGGER_RTT_printf (0,"TX_Door_Note_Run!\r\n");
		
		#endif
		
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;
		

		//	Read_Op_Note(&key_Num,&Key_Type,&Unix_t,Note_Num_1);
			
			TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
			TX_Buff[1] = BLE_DAT_H2;
			
			TX_Buff[2] = 0x00;
			TX_Buff[3] = 0x22;  //长度
			
			TX_Buff[4] = Read_A_OP_Door_note_Command;  //指令码
			TX_Buff[5] = 0x02;  //包标识
			TX_Buff[6] = BLE_Data[6]; //序号
			
			it1 = id1;			//设备标识
			it1 &=0xff00;
			it1>>=8;
		//	it1+=3;
			if(it1>127) it1 =0;
			if(BLE_Data[7]==it1)
			TX_Buff[7] =it1;
			else 
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
				#endif
		//		Note_Num_1 = 0;
			}
			it1 = id1;
			it1 &=0x00ff;
		//	it1+=5;
			if(BLE_Data[8]==it1)
			TX_Buff[8] = it1;
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
				#endif
		//		Note_Num_1 = 0;
			}
			it1 = id2;
			it1 &=0xff00;
			it1>>=8;
		//	it1+=9;

			if(BLE_Data[9]==it1)
			TX_Buff[9] = it1;
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
				#endif
	//			Note_Num_1 = 0;
			}
			it1 = id2;
			it1 &=0x00ff;
			//it1+=6;
			
			if(BLE_Data[10]==it1)
			TX_Buff[10] = it1;
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
				#endif
				
//				Note_Num_1 = 0;
			}
			
			for(i =0;i<Open_Note_Num_Len;i++)			//先清空缓存
				Read_Op_Tab[i] =0;
			
			//从数据包中获取要读取的开门记录编号
			key_Num = BLE_Data[11];
			
			key_Num*=10;
			
			key_Num += BLE_Data[12];    //载入开门记录编号
			
			//读取对应编号的开门记录
			
			if(Read_Op_Note(key_Num)==1)			//当获取到的数据有效时把数据载入
			{
				
				for(i =1;i<Open_Note_Num_Len;i++)				//Open_Note_Num_Len = sizeof(Read_Op_Tab)
				{
					
					TX_Buff[i+10] = Read_Op_Tab[i];					//数据有效时返回对应数据  因为从1 开始记录 所以只+9
					
				}
				
			}
			else
			{
				for(i =1;i<Open_Note_Num_Len;i++)				//Open_Note_Num_Len = sizeof(Read_Op_Tab)
				{
					
					TX_Buff[i+10] = Read_Op_Tab[i];			//数据区无效时返回0 因为从1 开始记录 所以只+9
				
				}
				
			}
			
			it = 0; it1 =0; 
			
			for(i =0;i<(R_ODoor_note_ACK_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[36] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[37] = it1;
			TX_Buff[38] = BLE_DAT_E1;
			TX_Buff[39] = BLE_DAT_E2;  //打包好数据用于回应
			#ifdef uart_start
			SEGGER_RTT_printf (0,"TX_Door_Note_Run1!\r\n");
			#endif
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
			
		//	nrf_delay_ms(50);
			
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
		//	nrf_delay_ms(50);
			
			
				#ifdef uart_start
				
				for(i =0;i<40;i++)
					SEGGER_RTT_printf(0,"%x,",TX_Buff[i]);
				
				SEGGER_RTT_printf(0,"\r\n");
				
				#endif
		//	NRF_WDT_Feed();			//这里不能喂狗
			
//			Note_Out_Num++;
//			if((Note_Num_1==1)||(Note_Out_Num>50)||(BLE_LINK==false))
//			{	
//				TX_Door_X =0;
//				Note_Num_1 =0;
//				
//			//	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
//			//	Spi_Flash_Sleep();
//				#ifdef uart_start
//				SEGGER_RTT_printf(0,"TX_Door_Note_STOP!\r\n");
//				#endif
//			}
//			if(Note_Num_1>1)
//			Note_Num_1--;
			
//			if(Note_Out_Num>50)  //达到上传限制后 结束上传
//			{
//				//Note_Out_Num =0;
//				Note_Num_1 =0; 
//				TX_Door_X =0;
//			//	return 0;
//			}
//			
//			TX_Door_X =0;

		//	Note_Num_1 = 0XFFFF;//锁定
		//		return 0;
			//nrf_delay_ms(20);
		//	return 1;
				//return 0;
	//	
//	Note_Out_Num =0; //上传次数限制
//	return 0;
}



/*****************************************

CL_Door_note

清空本地开门记录指令

*****************************************/

void CL_Door_note(void)
{
	//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
//	uint16_t Note_Num;
	
//	uint16_t Note_R_Num;  //读取条数(从后往前)
//	uint16_t key_Num;  //开门记录编号
		
	//	Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	
		#ifdef uart_start
		SEGGER_RTT_printf (0,"TX_Door_Note_Run!\r\n");
		
		#endif
		
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;
		

		//	Read_Op_Note(&key_Num,&Key_Type,&Unix_t,Note_Num_1);
			
			TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
			TX_Buff[1] = BLE_DAT_H2;
			
			TX_Buff[2] = 0x00;
			TX_Buff[3] = 0x10;  //长度
			
			TX_Buff[4] = CL_OP_Door_note_Command;  //指令码
			TX_Buff[5] = 0x02;  //包标识
			TX_Buff[6] = BLE_Data[6]; //序号
			
			it1 = id1;			//设备标识
			it1 &=0xff00;
			it1>>=8;
		//	it1+=3;
			if(it1>127) it1 =0;
			if(BLE_Data[7]==it1)
			TX_Buff[7] =it1;
			else 
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
				#endif
		//		Note_Num_1 = 0;
			}
			it1 = id1;
			it1 &=0x00ff;
		//	it1+=5;
			if(BLE_Data[8]==it1)
			TX_Buff[8] = it1;
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
				#endif
		//		Note_Num_1 = 0;
			}
			it1 = id2;
			it1 &=0xff00;
			it1>>=8;
		//	it1+=9;

			if(BLE_Data[9]==it1)
			TX_Buff[9] = it1;
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
				#endif
	//			Note_Num_1 = 0;
			}
			it1 = id2;
			it1 &=0x00ff;
			//it1+=6;
			
			if(BLE_Data[10]==it1)
			TX_Buff[10] = it1;
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
				#endif
				
//				Note_Num_1 = 0;
			}
			
			Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	
			
			Ev_Cat_Dat(OpenDoorNote_Num_Code,0);  //清空开门记录数量
			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
		
			
			TX_Buff[11] =1;		//参数1 返回成功
			TX_Buff[12] =0;		//参数2 保留
			TX_Buff[13] =0;		//参数3 保留
			TX_Buff[14] =0;		//参数4 保留
			TX_Buff[15] =0;		//参数5 保留
			TX_Buff[16] =0;		//参数6 保留
			TX_Buff[17] =0;		//参数7 保留
			
			it = 0; it1 =0; 
			
			for(i =0;i<(R_ODoor_note_ACK_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);
			
		//	nrf_delay_ms(50);
			
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
			BLE_TX_Run(BLE_TX,20);	
			
	
	
}


/*****************************************
用户自助发卡操作
(在门上刷卡实现发卡)

*******************************************/

uint8_t Start_Manual_Add_Card =0;

uint32_t Manual_Add_Card_Start_Unix; //手动载入卡的起始时间
uint32_t Manual_Add_Card_Stop_Unix; //手动载入卡的结束时间

void Manual_Add_Card()  //手动发卡
{
		
//		uint8_t PassWord[18];
		uint32_t it1;
//		uint64_t Input_Time;
	//	uint8_t TX_Buff[100];  //待发送数据
//		uint8_t BLE_TX[20]; //一次发送的数据
//		uint32_t err_code;
		
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Aanual_Add_Card_Run!\r\n");
		#endif
		Start_Manual_Add_Card =0;  //清空 标志位
	
//		for(i =0;i<sizeof(TX_Buff);i++)
//		TX_Buff[i] =0;

//		TX_Buff[0] = 0X5E;  //准备应答数据
//		TX_Buff[1] = 0xDC;
//		
//		TX_Buff[2] = 0x00;
//		TX_Buff[3] = 0x10;  //长度
//		
//		TX_Buff[4] = 0X10;  //指令码
//		TX_Buff[5] = 0x02;  //包标识 (应答02 发送01)
//		TX_Buff[6] = BLE_Data[6]; //序号
		
		it1 = id1;			//设备标识
		it1 &=0xff00;
		it1>>=8;
	//	it1+=3;
		if(it1>127) it1 =0;
//		if(BLE_Data[7]==it1)
//		TX_Buff[7] =it1;
//		else 
//		{
//			#ifdef uart_start
//			SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
//			#endif
//			return;
//		}
		
		it1 = id1;
		it1 &=0x00ff;
	//	it1+=5;
		if(BLE_Data[8]==it1);
	//	TX_Buff[8] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
		}
		
		it1 = id2;
		it1 &=0xff00;
		it1>>=8;
	//	it1+=9;

		if(BLE_Data[9]==it1);
	//	TX_Buff[9] = it1;
		else
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
			
		}
		
		it1 = id2;
		it1 &=0x00ff;
		//it1+=6;
		

		if(BLE_Data[10]==it1);
		//TX_Buff[10] = it1;
		else
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			
			return;
			
		}
		
		//Start_Marnual_Add_Card =1;
		
		if(BLE_Data[11]==0x01) //当属性为管理员时
		{
			
				if(BLE_Data[12]==0x03) //表示管理员 Type A 卡
				{
					
					Start_Manual_Add_Card = 1;
				
				}
				else
				if(BLE_Data[12]==0x04)  //表示管理员 Type B 卡
				{
					
					Start_Manual_Add_Card =2;
				
				}
				else
				{
					
					Start_Manual_Add_Card =0;
				
				}
		}
		else if(BLE_Data[11]==0x02) //当属性为租客时
		{
					if(BLE_Data[12]==0x03) //表示管理员 Type A 卡
					{
						
						Start_Manual_Add_Card = 3;
						
					}
					else
					if(BLE_Data[12]==0x04)  //表示管理员 Type B 卡
					{
						
						Start_Manual_Add_Card =4;
					
					}
					else
					{
						
						Start_Manual_Add_Card =0;
					
					}
		}
		else 
			Start_Manual_Add_Card =0;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Start_Manual_Add_Card:%d\r\n",Start_Manual_Add_Card);
		
		#endif
		
		Manual_Add_Card_Start_Unix =0;
		Manual_Add_Card_Stop_Unix =0;
		
		Manual_Add_Card_Start_Unix|= BLE_Data[13];
		
		Manual_Add_Card_Start_Unix <<=8;
		
		Manual_Add_Card_Start_Unix|= BLE_Data[14];
		
		Manual_Add_Card_Start_Unix <<=8;
		
		Manual_Add_Card_Start_Unix|= BLE_Data[15];
		
		Manual_Add_Card_Start_Unix <<=8;
		
		Manual_Add_Card_Start_Unix|= BLE_Data[16];   //载入自助发卡时效的起始时间
		
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[17];
		
		Manual_Add_Card_Stop_Unix <<=8;
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[18];
		
		Manual_Add_Card_Stop_Unix <<=8;
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[19];
		
		Manual_Add_Card_Stop_Unix <<=8;
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[20];   	//载入自助发卡时效的结束时间
		
				Start_Key_Run();  //直接唤醒键盘的函数
		
		
		//通过ID检测后，开启定时器轮询判断
		
	//	if(Start_Manual_Add_Card>0) //当值被置位时
	//		app_timer_start(Timer_1,APP_TIMER_TICKS(Timer_1,APP_TIMER_PRESCALER),NULL);			//开启500ms间隔的定时软件定时器任务
	//			
	//		for(i =0;i<sizeof(PassWord);i++)PassWord[i] =0x00; //清空缓存区
	//		
	//		if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x02)) //管理员密码 (第一字节01表示管理员，第二字节02表示密码)
	//		{
	//			PassWord[0] = 0x12; //表示管理员密码 //将两字节整合
	//			for(i =1;i<18;i++)
	//			{
	//				PassWord[i] = BLE_Data[12+i];
	//			} //将密码载入
	//	
	//			Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	//		
	//			TX_Buff[11] = 0X01;  //表示操作成功
	//			TX_Buff[12] = Add_Flash_PassWord(PassWord); //表示数据无效
	//			NRF_WDT_Feed();	
	//			if(TX_Buff[12]==0) //当写入返回错误
	//			{
	//				TX_Buff[11] =0x02;
	//			}
	//		}
	//		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x02))
	//		{
	//			PassWord[0] = 0x22; //表示用户密码 //将两字节整合
	//			for(i =1;i<18;i++)
	//			{
	//				PassWord[i] = BLE_Data[12+i];
	//			} //将密码载入
	//	
	//			Spi_Flash_Init();  //开启SPI_FLASH 通讯	
	//			
	//			TX_Buff[11] = 0X01;  //表示操作成功
	//			TX_Buff[12] = Add_Flash_PassWord(PassWord); //表示数据无效
	//			NRF_WDT_Feed(4);
	//			if(TX_Buff[12]==0) //当写入返回错误提示操作失败
	//			{
	//				TX_Buff[11] =0x02;
	//			}
	//		}
	//		else
	//		{	//操作失败 //未扩展其他写入功能
	//			TX_Buff[11] = 0x02; //表示操作失败
	//			#ifdef uart_start
	//			
	//			SEGGER_RTT_printf(0,"Add_Information_Full!\r\n");  //载入信息功能溢出
	//			
	//			#endif
	//		}
	//	
	//	
	//	
	////最后获取数据校验码
	//	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
	//		{
	//			it +=TX_Buff[i+4];
	//		}
	//		
	//		it1 =it;
	//		it1&=0xff00;
	//		it1>>=8;
	//		TX_Buff[18] = it1;
	//		it1 = it;
	//		it1&=0x00ff;
	//		TX_Buff[19] = it1;
	//		TX_Buff[20] = 0xae;
	//		TX_Buff[21] = 0x2b;  //打包好数据用于回应
	//		

	//		
	//			for(i =0;i<20;i++)
	//			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
	//			BLE_TX_Run(BLE_TX,20);
	//			for(i =0;i<20;i++)
	//			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
	//			BLE_TX_Run(BLE_TX,20);
	//		
	////			NRF_WDT_Feed();  //验证用
	////			read_Soctor(0);
	////			NRF_WDT_Feed();
	////			read_Soctor(1);
	////			NRF_WDT_Feed();
	////			read_Soctor(2);
	////			NRF_WDT_Feed();
	//			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	//			Spi_Flash_Sleep();
}

/*****************************************

用于回应 自助发卡指令

输入： Ack_Now

0x01 成功
0x02 失败

输入： Card_ID 

保存的数据 SN 号

输入：Card_Type

卡类型　
0X01 管理员 A 卡
0X02 用户 A 卡
0X03 管理员 B 卡
0X04 用户 B 卡


*****************************************/

#define Manual_Add_Card_Ack_Len 28

void Manual_Add_Card_ACK(uint8_t Ack_Now,uint16_t Card_ID,uint8_t Card_Type)  //手动发卡回应报文
{
		
//		uint8_t PassWord[50];
		uint32_t i,it,it1;
//		uint64_t Input_Time;
		uint8_t TX_Buff[100];  //待发送数据
		uint8_t BLE_TX[20]; //一次发送的数据
//		uint32_t err_code;
		
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Aanual_Add_Card_Run!\r\n");
		#endif
		Start_Manual_Add_Card =0;  //清空 标志位
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;

		TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
		TX_Buff[1] = BLE_DAT_H2;
		
		TX_Buff[2] = 0x00;
		TX_Buff[3] = 0x16;  //长度
		
		TX_Buff[4] = 0X10;  //指令码
		TX_Buff[5] = 0x02;  //包标识 (应答02 发送01)
		TX_Buff[6] = BLE_Data[6]; //序号
		
		it1 = id1;			//设备标识
		it1 &=0xff00;
		it1>>=8;
	//	it1+=3;
		if(it1>127) it1 =0;
		if(BLE_Data[7]==it1)
		TX_Buff[7] =it1;
		else 
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
			#endif
			return;
		}
		
		it1 = id1;
		it1 &=0x00ff;
	//	it1+=5;
		if(BLE_Data[8]==it1)
		TX_Buff[8] = it1;
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
		}
		
		it1 = id2;
		it1 &=0xff00;
		it1>>=8;
	//	it1+=9;

		if(BLE_Data[9]==it1)
		TX_Buff[9] = it1;
		else
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			return;
			
		}
		
		it1 = id2;
		it1 &=0x00ff;
		//it1+=6;
		

		if(BLE_Data[10]==it1)
		TX_Buff[10] = it1;
		else
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
			#endif
			
			return;
			
		}
		
		TX_Buff[11] = Ack_Now;  //输入返回状态
		TX_Buff[12] = 0;					//原返回 ID 失效
		
		TX_Buff[13] = Card_ID>>8&0xFF;		//载入ID号高位
		TX_Buff[14] = Card_ID&0XFF;				//载入ID号低位
		
		if(Type_AB_UID[0]==0X01)	//卡片为A卡时
			TX_Buff[15] = 4;
		else if(Type_AB_UID[0]==0X02)
			TX_Buff[15] = 8;
		
		TX_Buff[16] = Type_AB_UID[1];		// 返回钥匙详情 第一位
		TX_Buff[17] = Type_AB_UID[2];		// 返回钥匙详情 第二位
		TX_Buff[18] = Type_AB_UID[3];		// 返回钥匙详情 第三位
		TX_Buff[19] = Type_AB_UID[4];		// 返回钥匙详情 第四位
		TX_Buff[20] = Type_AB_UID[5];		// 返回钥匙详情 第五位
		TX_Buff[21] = Type_AB_UID[6];		// 返回钥匙详情 第六位
		TX_Buff[22] = Type_AB_UID[7];		// 返回钥匙详情 第七位
		TX_Buff[23] = Type_AB_UID[8];		// 返回钥匙详情 第八位
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Ack_Manual_Add_Card:%d\r\n",Start_Manual_Add_Card);
						if(Ack_Now==0x01)
							SEGGER_RTT_printf(0,"Card_ID: %d\r\n",Card_ID);
		#endif
		
	//最后获取数据校验码
	
		for(i =0;i<(Manual_Add_Card_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[25] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[26] = it1;
			TX_Buff[27] = BLE_DAT_E1;
			TX_Buff[28] = BLE_DAT_E2;  //打包好数据用于回应
			

			
				for(i =0;i<20;i++)
				BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
				BLE_TX_Run(BLE_TX,20);
				for(i =0;i<20;i++)
				BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包
				BLE_TX_Run(BLE_TX,20);
	//		
	////			NRF_WDT_Feed();  //验证用
	////			read_Soctor(0);
	////			NRF_WDT_Feed();
	////			read_Soctor(1);
	////			NRF_WDT_Feed();
	////			read_Soctor(2);
	////			NRF_WDT_Feed();
	//			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	//			Spi_Flash_Sleep();
	
	
	nrf_delay_ms(1500);  //运行延迟 预留语音时间

}

void Normally_Open_Door_Run()
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Normally_Open_Door_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = 0X05;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
	TX_Buff[11] = 0X01;  //表示开门成功
	
	TX_Buff[12] = 0; //表示数据无效
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //无功能
	TX_Buff[17] = 0;  //无功能
	
	it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包		
		
			BLE_TX_Run(BLE_TX,20);		
			
			
			Lock_Mot_Init();  //启动电机模块
			MOT_A_H;
			
			MOT_B_L;
			
			nrf_delay_ms(400);
			
	//	Lock_State =1;
			input_speak(6);
			Lock_Mot_Default();
			
			nrf_delay_ms(900);  
			input_speak(9);  //操作成功
			
}


////运行 触摸板灵敏度数据更新
//void TSM12MC_Sen_Code_update_Run()
//{
//	
//	uint32_t i,it,it1;
//	uint8_t TX_Buff[100];  //待发送数据
//	uint8_t BLE_TX[20]; //一次发送的数据
////	uint32_t err_code;
//	
//	#ifdef uart_start
//	SEGGER_RTT_printf (0,"TSM12MC_Sen_Code_update_Run!\r\n");
//	#endif
//	
//		for(i =0;i<sizeof(TX_Buff);i++)
//	TX_Buff[i] =0;
//	
//	TX_Buff[0] = 0X5E;  //准备应答数据
//	TX_Buff[1] = 0xDC;
//	
//	TX_Buff[2] = 0x00;
//	TX_Buff[3] = 0x10;  //长度
//	
//	TX_Buff[4] = 0X05;  //指令码
//	TX_Buff[5] = 0x02;  //包标识
//	TX_Buff[6] = BLE_Data[6]; //序号
//	
//	
//	it1 = id1;			//设备标识
//	it1 &=0xff00;
//	it1>>=8;
////	it1+=3;
//	if(it1>127) it1 =0;
//	
//	if(BLE_Data[7]==it1)
//	TX_Buff[7] =it1;
//	else 
//	{
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
//		#endif
//		return;
//	}
//	
//	it1 = id1;
//	it1 &=0x00ff;
////	it1+=5;
//	if(BLE_Data[8]==it1)
//	TX_Buff[8] = it1;
//	else
//	{
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
//		#endif
//		return;
//	}
//	
//	it1 = id2;
//	it1 &=0xff00;
//	it1>>=8;
////	it1+=9;

//	if(BLE_Data[9]==it1)
//	TX_Buff[9] = it1;
//	else
//	{
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
//		#endif
//		return;
//	}	
//	it1 = id2;
//	it1 &=0x00ff;
//	//it1+=6;
//	if(BLE_Data[10]==it1)
//	TX_Buff[10] = it1;
//	else
//	{
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
//		#endif
//		
//		return;
//	}
//	
//	
//	
//}


void More_Act_PassWord_Mode_Run()
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Normally_Open_Door_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = More_Act_PassWord_Mode;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
		Spi_Flash_Init();  //开启SPI_FLASH 通讯	
		
		if(BLE_Data[11]==1)    //根据输入参数设置 动态密码模式
		{
			//配置成多组动态密码模式
			
			Ev_Cat_Dat(More_Act_PassWord_Num_Code,0xFFFF0000);  
			//以高位两为 基准 高两位为0XFFFF时为 多组动态密码状态 
			
		}
		else if(BLE_Data[11]==0)
		{
			//配置成单个时效动态密码模式
			Ev_Cat_Dat(More_Act_PassWord_Num_Code,0);  
			//以高位两为 基准 高两位为0XFFFF时为 多组动态密码状态 
			
		}
		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep();
		
	
		TX_Buff[11] = 0X01;  //表示设置成功
		
		TX_Buff[12] = 0; //表示数据无效
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //无功能
		TX_Buff[17] = 0;  //无功能
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包		
		
			BLE_TX_Run(BLE_TX,20);		
	
			input_speak(9);   //提示操作成功
}


void Del_A_More_Act_PassWord_Run()    //允许 删除单个多组动态密码指令
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //待发送数据
	uint8_t BLE_TX[20]; //一次发送的数据
//	uint32_t err_code;
	
	uint8_t PassWord_Num[15];		//目标动态密码缓存
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Normally_Open_Door_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	for(i =0;i<sizeof(PassWord_Num);i++)
		PassWord_Num[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //准备应答数据
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //长度
	
	TX_Buff[4] = Del_A_More_Act_PassWord;  //指令码
	TX_Buff[5] = 0x02;  //包标识
	TX_Buff[6] = BLE_Data[6]; //序号
	
	it1 = id1;			//设备标识
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127) it1 =0;
	
	if(BLE_Data[7]==it1)
	TX_Buff[7] =it1;
	else 
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error!\r\n");
		#endif
		return;
	}
	
	it1 = id1;
	it1 &=0x00ff;
//	it1+=5;
	if(BLE_Data[8]==it1)
	TX_Buff[8] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}
	
	it1 = id2;
	it1 &=0xff00;
	it1>>=8;
//	it1+=9;

	if(BLE_Data[9]==it1)
	TX_Buff[9] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		return;
	}	
	it1 = id2;
	it1 &=0x00ff;
	//it1+=6;
	if(BLE_Data[10]==it1)
	TX_Buff[10] = it1;
	else
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Devic_ID_Error\r\n");
		#endif
		
		return;
	}
	
		Spi_Flash_Init();  //开启SPI_FLASH 通讯	
		
		
		if(BLE_Data[11]<10)				//目标动态密码长度最大允许9位
		{
			PassWord_Num[0] = BLE_Data[11];
			
			for(i =0;i<BLE_Data[11];i++)
			{
				PassWord_Num[i+1] = BLE_Data[12+i];				//密码内容从 BLE_Data[12]开始
			}
			
			Dle_A_More_Act_PassWord_Verify(PassWord_Num);
			
			TX_Buff[11] = 0X01;  //表示删除成功
		}
		else
			TX_Buff[11] = 0X00;  //表示删除失败(载入数据无效)
		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep();
		
	

		TX_Buff[12] = 0; //表示数据无效
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //无功能
		TX_Buff[17] = 0;  //无功能
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //校验码除外的有效值相加
			{
				it +=TX_Buff[i+4];
			}
			
			it1 =it;
			it1&=0xff00;
			it1>>=8;
			TX_Buff[18] = it1;
			it1 = it;
			it1&=0x00ff;
			TX_Buff[19] = it1;
			TX_Buff[20] = BLE_DAT_E1;
			TX_Buff[21] = BLE_DAT_E2;  //打包好数据用于回应
			
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //赋值第一个数据包
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包		
		
			BLE_TX_Run(BLE_TX,20);		
	
			input_speak(9);   //提示操作成功
}

