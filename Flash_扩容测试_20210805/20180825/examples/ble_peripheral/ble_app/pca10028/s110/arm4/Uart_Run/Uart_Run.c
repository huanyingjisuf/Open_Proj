#include <Uart_Run.h>
#include <F_print.h>


uint8_t Uart_TX_String[20];  //��������ַ�������

uint8_t Uart_RX_String[Uart_RX_String_Size];

uint32_t Uart_RX_len =0;

extern uint8_t UART_RUN;


/************************************************

�����ַ������������


���룺*Uart_Dat  �ַ���ָ��++
���룺 len  �ַ�������

����� ��

��ע��

�����������ͨ�����������Ӧ��ַ��ָ�����ȵ��ַ���

**************************************************/

void Uart_Put_String (uint8_t *Uart_Dat,uint8_t len)
{
	uint8_t i;
	
	if(UART_RUN==0)  //������δ����ʱ��ֱ�Ӵ򿪴���
	{
		
		Uart_Start_TC();
		
	}
	
	Uart_Close_T =0;   //����һ�� ���ô��ڹرռ�ʱʱ��

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

���ڶ�ȡ

**************************************/

 //while (NRF_UART0->EVENTS_RXDRDY != 1)
//  NRF_UART0->EVENTS_RXDRDY = 0;
//  return (uint8_t)NRF_UART0->RXD;//��������

uint32_t Get_String_t =0;  //����ץȡ����
void Uart_Get_String()  //��ȡ 100ms�ڴ���ץȡ�ı���
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
				if(Uart_RX_len<sizeof(Uart_RX_String))  //�����ճ���С�ڻ���������ʱ�����������
					Uart_RX_String[Uart_RX_len++]=NRF_UART0->RXD;  //���յ������ݷŽ�����
				
			}
			
			Get_String_t ++;
			
//			if(Get_String_t>5000)  //����ȡ���ڴﵽ10000us(10ms) ���������� 
//				break;
			
		//	nrf_delay_us(1);
			
		}
		
		if(Uart_RX_len>0) //����ȡ����Ч�İ�ʱ
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
			
			PS_Data_Cheak(Uart_RX_String,Uart_RX_len);			//	������ݰ�
			
			#endif
			
			#ifdef NB_Enable   //�� ʹ��NB ʱ
			
			if(NB_Run ==1)   //�� NB ģ�鱻��ʱ�� NB ģ������д�� 
			{
					if((NB_Dat_Len+Uart_RX_len)<NB_Dat_Size)
					{
						for(uint32_t i =0;i<Uart_RX_len;i++)
						{
							
							NB_Dat[NB_Dat_Len+i] = Uart_RX_String[i];
							
						}
							NB_Dat_Len+=Uart_RX_len;  //���� NB_Dat ֵ
						
					}
					else
					{
						
						#ifdef uart_start
						
							SEGGER_RTT_printf(0,"NB_Dat_Len_Full!\r\n");   // ��ʾ NB_Dat  �������
						
						#endif
						
					}
			
			}
			
				#ifdef uart_start
				
					if(NB_Dat_Len>0)   //�� NB_Dat  ������������ʱ
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

A71_�������ݽ���

*************************************/

/*************************************

�ж� ���ݰ��Ƿ�Ϊ�Ϸ���A71 ���ڱ���

************************************/
uint8_t Get_A71_Str[100];  //��ȡ����A71 str ����
uint8_t A71_Str_Len =0;
uint8_t A71_Dat_Cheak()
{
	
	uint8_t len;  //��Ч���ݰ�����
	uint8_t i,it;
		
	A71_Str_Len =0;
	
	for(i =0;i<Uart_RX_len;i++)
	{
			len = Uart_RX_String[i+3];
		
			if((Uart_RX_String[i]==0x55)&&(Uart_RX_String[i+1]==0xAA)&&  //���ݰ�ͷ��ȷ
				(Uart_RX_String[i+2]==0x00)&&																	//��һ�������ֽ���ʱ����������
				(Uart_RX_String[i+3]<=(sizeof(Uart_RX_String)-i))&&						//�ж��ַ�������ֵ�Ƿ�Ϸ�
				(Uart_RX_String[i+Uart_RX_String[i+3]-1-1]==0xAA)&&(Uart_RX_String[i+Uart_RX_String[i+3]-1]==0x55)   //�жϰ�β�Ƿ�Ϸ�
				)
			{
				#ifdef uart_start
				
			SEGGER_RTT_printf(0,"Get_A71_Str!\r\n");   //�����һ��A71 ���͵����ݰ�
				
				#endif
				
				for(it =0;it<len;it++)    //��ȡ��ȡ���� A71 ����
				{
					
					Get_A71_Str[it] = Uart_RX_String[i+it];
				
				}
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Get_A71_Str_Len:%x\r\n",it);   //�����һ��A71 ���͵����ݰ�
				
				#endif
				
				
				return 1;
			}
	}
	
	return 0;

}




//A71 ��Ƶģ��
/*********************************************

Ϊ��֤���ݴ����ȶ��������ݰ�����ǰ�ȷ���5�� 0x00 �����ڽ��͸��źͻ���ģ��

���ݰ���ʽ: 0x55+0xAA 2 bit (����ͨ�ſ�ͷ)

���ȣ� 2 bit  (���ڴ�����ݳ���)

�������ͣ�2 bit  (���ڱ�ʾ���ݰ�����)

������Դ�� 2 bit  (�����������ݰ���Դ�豸) 


���ݰ������� 2 bit  (ÿ��ͨ�ŵı��ı�ţ���������ͬһ��Դ�Ĳ�ͬ���ݰ����������ݰ������)

�������ݣ�  N bit   (��Ч���������ݣ����豣֤���ݰ��ܳ����������������)

����У���룺 8 �ֽ�  (����֧�ָ��������㷨���������ݿɿ���)

���ݽ�β�� 0xAA+ 0X55 2 bit (����ͨ�Ž�β)

*********************************************/


//��Ƶģ�鴮��Ԥװָ��

//��ʼ�� A71 ��Ƶģ�� ָ��
const uint8_t  A71_Init[] = 
{
	0X55,0xAA,
	0x00,0x0C,
	0x04,0x08,
	0X10,0X01,
	0X00,0X00,
	0xAA,0x55
};

//���� A71 ��Ƶģ�� ͨ��ID ָ��
const uint8_t  A71_Set_ID[] = 
{
	0X55,0xAA,   //����ͷ
	0x00,0x11,	//����
	0x04,0x01,  //ָ����
	0x10,0x01,	//������Դ/Ŀ��
	0x00,0x00,  //���ݰ�ID
	0x04,0x34,0x75,0xC5,0x8C,   //��Ч������ //Ĭ�����ֽ�ID�� 0X34,0X75,0XC5,0X8C
	0xAA,0x55   //����β
};


//���� A71 ��Ƶģ�� ������ģʽ��ֱ�����յ�����

const uint8_t A71_RX_Mod[] = 
{
	0x55,0xAA,
	0X00,0X0C,
	0X04,0X06,
	0X10,0X01,
	0X00,0X00,
	0XAA,0X55
};

//���ڴ��ڷ���ģʽ��ָ��  ��������ָ�ʵ���Է��Ϊ׼��

const uint8_t A71_TX_Mod[] = 
{
	0x55,0xAA,
	0X00,0X00,
	0X03,0X00,
};



//���� A71 ģ�鵽����ģʽ��ֱ������

const uint8_t A71_Power_Down[] = 
{
	
	0x55,0xAA,
	0X00,0X0C,
	0X04,0X0D,
	0X10,0X01,
	0X00,0X00,
	0XAA,0X55
	
};

// ���� A71 ��������



//�� A71 ģ�鷢������
uint8_t Send_Buff[100];  //���ͻ���

//�����ݰ�����ǰ�� "A71_Send_Sp_Num"�� 0X00���Ա���ͨ�ſɿ�
const uint8_t A71_Send_Sp_Num =5;

/*****************

�� A71 ģ��д���ʼ��ָ��

*******************/

void A71_Send_Init ()
{
	
	uint8_t i;
	
	for(i =0;i<sizeof(Send_Buff);i++)   //��ջ���
 		Send_Buff[i] =0;
	
	for(i =0;i<sizeof(A71_Init);i++)
	{

		Send_Buff[i+A71_Send_Sp_Num] = A71_Init[i];   //ƫ��ֵ��ʾ�������ݰ�ǰ�����ƫ��ֵ������0X00

	}
	
	Uart_Put_String(Send_Buff,sizeof(A71_Init)+A71_Send_Sp_Num);   //�������ݰ�������

}

/**************************

�� A71 ģ��д�� ͨ��ID����ָ��

*************************/

uint8_t A71_ID_Tab[4] = 
{
	
0x34,0x75,0xc5,0x8c
	
};  //д��ID����

void A71_Send_ID()
{
	
		uint8_t i;
		
		for(i =0;i<sizeof(Send_Buff);i++)
			Send_Buff[i] = 0;
		
		for(i =0;i<sizeof(A71_Set_ID);i++)
		{
			
			Send_Buff[i+A71_Send_Sp_Num] = A71_Set_ID[i];
			
		}
		
		//�޸� ID �ŵ�����ֵ
		
		Send_Buff[11+A71_Send_Sp_Num] = A71_ID_Tab[0];
		Send_Buff[12+A71_Send_Sp_Num] = A71_ID_Tab[1];
		Send_Buff[13+A71_Send_Sp_Num] = A71_ID_Tab[2];
		Send_Buff[14+A71_Send_Sp_Num] = A71_ID_Tab[3];
		
		Uart_Put_String(Send_Buff,sizeof(A71_Set_ID)+A71_Send_Sp_Num);   //�������ݰ�������
			
}

/**************************************************

�� A71 ģ�鷢�͵���ָ�� �Թر�ģ��

����ͨ�����ڷ������ݻ���ģ�飩

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

���ͽ���ģʽָ��
ʹ A71 ģ��������ģʽ (ά��ʱ����ģ�����)

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
	
	Uart_Put_String(Send_Buff,sizeof(A71_Set_ID)+A71_Send_Sp_Num);//�����ݰ����ͳ�ȥ
	
	
}


/************************************************

�� A71 ģ�鷢�� ���ݰ�����ָ��
�Խ�ָ������ͨ��ģ�鷢�͵�����

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
	Send_Buff[3+A71_Send_Sp_Num] = 77;   //���ݰ�����
	Send_Buff[4+A71_Send_Sp_Num] = 0X03;  //���ݷ���ָ��
	Send_Buff[5+A71_Send_Sp_Num] = 0x00;
	Send_Buff[6+A71_Send_Sp_Num] = 0x10;
	Send_Buff[7+A71_Send_Sp_Num] = 0x01;
	Send_Buff[8+A71_Send_Sp_Num] = 0x00;
	Send_Buff[9+A71_Send_Sp_Num] = 0x00;
	
	Send_Buff[10+A71_Send_Sp_Num] = 64;   //���ݰ�����
	
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

������������ʵ
A71_RF ģ�鹦�ܵĺ���

****************************************/


/********************************

uint8_t A71_RF_RX_MODE()  

���ܣ� ʹA71_RF ģ�� �ڽ���ģʽ

���룺 ��
������ɹ��� 1  ʧ�ܣ�0

*******************************/

uint8_t A71_RF_RX_MODE()  
{
	uint8_t now;
	uint32_t run_t;  //����ʱ�� ��ʱ���Զ�����
	uint8_t i;
	now =0;
	run_t =0;
	
	while(now<0xFF)   //ֱ��RX�����������
	{
		
		switch(now)
		{
			case 0:   //�Ȼ����豸
				
				for(i =0;i<sizeof(Send_Buff);i++)
				 Send_Buff[i] =0;
			
				Uart_Put_String(Send_Buff,20);   //����20�ֽ� 0X00 ���ڻ����豸
				nrf_delay_ms(100);
			
				now++;
				
			break;
			
			case 1:   //���豸���ͽ������ģʽ��ָ��
				
				A71_Send_RX();
		
				Uart_RX_len =0;  //��մ��ڽ��յĻ������
	
				
				now++;
			
				#ifdef uart_start
			
				SEGGER_RTT_printf(0,"A71_Send_RX!\r\n");
				
				#endif
			
				break;
			
			case 2:  //�ȴ�����
					
				if(Uart_RX_len>0)
				{
					
					if(A71_Dat_Cheak()==1)
					now++;
					else
					now=0xff;  //����ѭ��
					
				}
			
				break;
			
			case 3:  //�ж�Ӧ��������Ƿ���ȷ
			
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
			
				if(Get_A71_Str[10]==0x01)  //���������ݱ�ʾ�ɹ�ʱ
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"A71_Send_RX_MOD_OK!\r\n");
					
					#endif
					
					Uart_Close_T =0;   //�ɹ�����Ϊ����ģʽ�� ���ô��ڹرռ�ʱʱ��
					
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
			
				now = 0xFF;  //�� nowֵ���Ϸ�ʱ������ѭ��
			
			break;
			
			
		}
		
		if((UART_RUN==1)&&(now==2))  //���ڴ�������ʱ��������
		{
			
			Uart_Get_String();
			
		}
		
		run_t++;
		if(run_t>500000)  //�����г�ʱ�󣬽�������
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
ͨ������ģ��������ģʽ����������ݰ����չ�

���룺 RX_Dat_Num   ��λʱ���� ��ȡ���ݰ�����
����� 1 �ɹ���ȡ�����ݰ�  0��δ��ȡ�����ݰ�

************************************/

uint8_t A71_RX_Dat_Tab[64];

uint8_t Need_Of_RX_Dat()
{
	
	uint32_t i;
	uint8_t now;
	uint32_t run_t;
	
	run_t =0;
	
	for(i =0;i<sizeof(A71_RX_Dat_Tab);i++)  //��ջ���
	{
		
		A71_RX_Dat_Tab[i] =0x00;
		
	}
		
	
	if(A71_RF_RX_MODE()==1)
	{
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Start_RXD_A71_DAT!\r\n");  //��ʼ��������A71������
		
		#endif
		
		now =0;
		Uart_Close_T =0;   //����һ�� ���ô��ڹرռ�ʱʱ��
		
		while((now<0xFF)&&(UART_RUN==1))   //��ʼ�ȴ����ݰ�
		{
			
				switch(now)
				{
					case 0:   
					
					if(UART_RUN==1)		//�ȴ���������
						Uart_Get_String();
					
					if(Uart_RX_len>0)  //���յ����ڰ�
						now++;
					
						break;
					
					case 1:
					
					if(A71_Dat_Cheak()==1)   //�жϲ�ץȡ�Ϸ��Ĵ������ݰ�
					now++;
					else
					now=0;  //���޺Ϸ�����ʱ�����صȴ�״̬
					
						break;
					
					case 2:  //��ʼ�ж�ָ��ͳ���
						
					if (
					(Get_A71_Str[4]==(A71_RX_Mod[4]|0x80))&&
					(Get_A71_Str[5]==(A71_RX_Mod[5]|0X80))&&
					(Get_A71_Str[3]==76))   // RF���ݻ�Ӧ������Ϊ76�ֽ�
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"A71_Get_Dat_Order_OK!\r\n");
						
						#endif
						now++;
						
					}
					
						break;
					
					case 3:   //��ʼ����Ч����д�뻺��
					
						#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Get_Dat:\r\n");
					
						#endif
					
						for(i =0;i<64;i++)   //��ȡ��Ч����
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
				
			if(run_t>2500000)  //�����ճ�ʱʱ
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

���һ�������ķ��͹���

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
			
			A71_Data_TX(s);   //����Ƶ�������ݰ�
			
			now++;

				break;
			
			case 1:   //�ȴ���Ӧ

			if(UART_RUN==1)		//�ȴ���������
					Uart_Get_String();
				
			if(Uart_RX_len>0)  //���յ����ڰ�
					now++;
				
					
			break;
			
			case 2:
				
			if(A71_Dat_Cheak()==1)   //�жϲ�ץȡ�Ϸ��Ĵ������ݰ�
				now++;
		else 
			now =1;   //�ص��ȴ����յ�λ��
		
			break;

			case 3:
				
				if (
					(Get_A71_Str[4]==(A71_TX_Mod[4]|0x80))&&
					(Get_A71_Str[5]==(A71_TX_Mod[5]|0X80))&&
					(Get_A71_Str[3]==0x0D))   // RF���ݻ�Ӧ������Ϊ13�ֽ�
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"A71_TX_Dat_Order_OK!\r\n");
						
						#endif
						now++;
						
					}
		
			break;

			case 4:   //�ȴ��ڶ������� (���ͳɹ���Ӧ)
			
			if(UART_RUN==1)		//�ȴ���������
				Uart_Get_String();
				
			if(Uart_RX_len>0)  //���յ����ڰ�
					now++;
				
			
			break;

			case 5:
				
			if(A71_Dat_Cheak()==1)   //�жϲ�ץȡ�Ϸ��Ĵ������ݰ�
				now++;
			else 
				now =4;  //���ݰ������򷵻�
		
			break;
	
			case 6:
			
				if (
						(Get_A71_Str[4]==(A71_TX_Mod[4]|0X80))&&
						(Get_A71_Str[5]==0)&&				//��ָ��Ϊ���ͳɹ���Ӧ
						(Get_A71_Str[3]==0x0D)		
					)
				{
					if(Get_A71_Str[10]==0X01)    //���ͳɹ�����1 
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
		
		if(run_t>200000)  //�����г�ʱ�󣬽�������
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


