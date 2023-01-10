#include <pca10028.h>

#ifdef NB_Enable

#include <NB_Mode1.h>


/**********************
void NB_Weak_Up()

用于唤醒 NB 模块 
同时开始接收数据
************************/

void NB_Weak_Up()
{
	
	#ifdef NB_Enable
	
	uint8_t Week_Up_Dat[10];
	
	for(uint8_t i;i<sizeof(Week_Up_Dat);i++)
	{
		
		Week_Up_Dat[i] = i;
	
	}
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"NB_Weak_Up!\r\n");
	
	#endif
	
	
	nrf_gpio_cfg_output(NB_WAKE_UP);   //唤醒NB模块
	
	nrf_gpio_pin_clear(NB_WAKE_UP);
	
	nrf_delay_ms(750);
	
	nrf_gpio_pin_set(NB_WAKE_UP);
	
	//nrf_delay_ms(300);
	nrf_gpio_cfg_default(NB_WAKE_UP);
	
	NB_Run =1;
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Wait_Dat!\r\n");
	
	#endif
	
	Uart_Put_String(Week_Up_Dat,sizeof(Week_Up_Dat));
	
	#endif

}

extern uint8_t NB_Dat[NB_Dat_Size]; 

extern uint32_t NB_Dat_Len;   //NB Dat 字符表 当前使用到的位置

uint8_t NB_Dat_Park[NB_Dat_Park_Num_Size][NB_Dat_Park_Size];

uint8_t NB_Dat_Park_Num =0;     // NB_Dat_Park  保存的数据条数

uint8_t NB_Dat_Park_Len =0;


void NB_Get_Dat()
{
	
	uint32_t i,i1;
	uint32_t len;   //长度临时缓存
	
	for(i =0;i<NB_Dat_Len;i++)
	{
		
		if((NB_Dat[i]==0x55)&&(NB_Dat[i+1]==0xAA))   // 当数据头正确时
		{
			
			len = NB_Dat[i+2];
			len<<=8;
			len+=NB_Dat[i+3];
			
			if(len<(NB_Dat_Park_Size-6))  //当长度字节在总长度限制内
			{
				
				if((i+len+6)<NB_Dat_Size)   //当字节长度后位置在有效空间内
				{
					
					if((NB_Dat[i+len+4]==0xAE)&&(NB_Dat[i+len+5]==0x2B)&&(NB_Dat_Park_Num<NB_Dat_Park_Num_Size)) 			//判断包尾是否正确,且NB_Dat_Park 还有空间
					{
						//当数据尾正确时，将数据写入缓存准备使用
						
						for(i1 =0;i1<(len+6);i1++)
							NB_Dat_Park[NB_Dat_Park_Num][i1] = NB_Dat[i+i1];
						
						NB_Dat_Park_Num++;
						
					}
					else if(NB_Dat_Park_Num_Size>=NB_Dat_Park_Num_Size)					//当 NB_Dat_Park 空间耗尽
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"NB_Dat_Park_Num_Size_Full!\r\n");					//提示空间满
						
						#endif
						
					}
					
				}
				
			}
			
		}
		
	}
	
	#ifdef uart_start
	
	if(NB_Dat_Park_Num_Size>0)   			//当获取到完整的数据包时
	{
		
		SEGGER_RTT_printf(0,"\r\n");
		
		SEGGER_RTT_printf(0,"NB_Dat_Park:\r\n");
		
		for(i =0;i<NB_Dat_Park_Num_Size;i++)
		{
			
			SEGGER_RTT_printf(0,"NB_Dat_Park %x:\r\n",i);					//输出 Park  
			
			for(i1 =0;i1<NB_Dat_Park_Size;i1++)    // 发送报文
			{
				
				SEGGER_RTT_printf(0,"%x",NB_Dat_Park[NB_Dat_Park_Num][i1]);
			
			}
			
		}
		
	}
	
	#endif
	
}








#endif
