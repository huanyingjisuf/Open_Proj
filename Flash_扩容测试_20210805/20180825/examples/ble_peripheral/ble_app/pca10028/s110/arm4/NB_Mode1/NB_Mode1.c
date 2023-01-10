#include <pca10028.h>

#ifdef NB_Enable

#include <NB_Mode1.h>


/**********************
void NB_Weak_Up()

���ڻ��� NB ģ�� 
ͬʱ��ʼ��������
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
	
	
	nrf_gpio_cfg_output(NB_WAKE_UP);   //����NBģ��
	
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

extern uint32_t NB_Dat_Len;   //NB Dat �ַ��� ��ǰʹ�õ���λ��

uint8_t NB_Dat_Park[NB_Dat_Park_Num_Size][NB_Dat_Park_Size];

uint8_t NB_Dat_Park_Num =0;     // NB_Dat_Park  �������������

uint8_t NB_Dat_Park_Len =0;


void NB_Get_Dat()
{
	
	uint32_t i,i1;
	uint32_t len;   //������ʱ����
	
	for(i =0;i<NB_Dat_Len;i++)
	{
		
		if((NB_Dat[i]==0x55)&&(NB_Dat[i+1]==0xAA))   // ������ͷ��ȷʱ
		{
			
			len = NB_Dat[i+2];
			len<<=8;
			len+=NB_Dat[i+3];
			
			if(len<(NB_Dat_Park_Size-6))  //�������ֽ����ܳ���������
			{
				
				if((i+len+6)<NB_Dat_Size)   //���ֽڳ��Ⱥ�λ������Ч�ռ���
				{
					
					if((NB_Dat[i+len+4]==0xAE)&&(NB_Dat[i+len+5]==0x2B)&&(NB_Dat_Park_Num<NB_Dat_Park_Num_Size)) 			//�жϰ�β�Ƿ���ȷ,��NB_Dat_Park ���пռ�
					{
						//������β��ȷʱ��������д�뻺��׼��ʹ��
						
						for(i1 =0;i1<(len+6);i1++)
							NB_Dat_Park[NB_Dat_Park_Num][i1] = NB_Dat[i+i1];
						
						NB_Dat_Park_Num++;
						
					}
					else if(NB_Dat_Park_Num_Size>=NB_Dat_Park_Num_Size)					//�� NB_Dat_Park �ռ�ľ�
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"NB_Dat_Park_Num_Size_Full!\r\n");					//��ʾ�ռ���
						
						#endif
						
					}
					
				}
				
			}
			
		}
		
	}
	
	#ifdef uart_start
	
	if(NB_Dat_Park_Num_Size>0)   			//����ȡ�����������ݰ�ʱ
	{
		
		SEGGER_RTT_printf(0,"\r\n");
		
		SEGGER_RTT_printf(0,"NB_Dat_Park:\r\n");
		
		for(i =0;i<NB_Dat_Park_Num_Size;i++)
		{
			
			SEGGER_RTT_printf(0,"NB_Dat_Park %x:\r\n",i);					//��� Park  
			
			for(i1 =0;i1<NB_Dat_Park_Size;i1++)    // ���ͱ���
			{
				
				SEGGER_RTT_printf(0,"%x",NB_Dat_Park[NB_Dat_Park_Num][i1]);
			
			}
			
		}
		
	}
	
	#endif
	
}








#endif
