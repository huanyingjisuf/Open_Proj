#include <BLE_Data.h>
#include <app_uart.h>
#include <nrf_error.h>
#include "Devic_Init.h"
#include <MotO_Run.h>
#include <F_print.h>
#include <Flash_RW.h>

//static  ble_nus_t  m_nus;     


#if YFA			//����ʶ�����ʱ


#define BLE_DAT_H1  0x5A
#define BLE_DAT_H2  0x5E

#define BLE_DAT_E1  0xA5
#define BLE_DAT_E2  0xEC

#elif DSA			// ��ҳ���ʱ


#define BLE_DAT_H1  0xED
#define BLE_DAT_H2  0x2C

#define BLE_DAT_E1  0xD3
#define BLE_DAT_E2  0xEB

#else				//��������ѡ��Ĭ��


#define BLE_DAT_H1  0x5E
#define BLE_DAT_H2  0xDC

#define BLE_DAT_E1  0xAE
#define BLE_DAT_E2  0x2B

#endif



/*****************************************************

����ļ����ڣ�����BLE����ͨ���������
���ڷ��ͺͻ�Ӧ ���ã���ʵ��ָ����ع��ܡ�

*****************************************************/

extern uint8_t BLE_LINK;  //BLE����״̬ʵʱ����
uint8_t BLE_Data[100];  // BLE���ݻ���
uint8_t BLE_Data_Len =0;  //���������ݳ���

#define data_len_min  0x10  //��С��Ч���ݳ���
uint16_t data_len;  //���ݳ���
uint8_t  IN_BLE_Dat_SW =0;
uint8_t irw; //���� Data_Input ѭ������

const uint32_t Version_Num = 0x20090709;			//�汾�� ��ʽ�� �� �� �� Сʱ

const uint32_t Authorization_time = 0x20090710;			//��Ȩ��ʼʱ�� ��ʽ�� �� �� �� Сʱ


//#define APP3_Op_Door  1					//���� �Ƿ���������� ����ָ��


/***********************************************
������ȡ����BLE���ݣ�׼����ʼ����
**********************************************/


void Data_Input(uint8_t *BLE_DAT_p,uint8_t Dat_Len)  //BLE��������
{
//	uint8_t it1,it2; //ǰ�����ֽڻ���
	
//	it1 = *BLE_DAT_p;
//	BLE_DAT_p++;
//	it2 = *BLE_DAT_p;  //ץȡǰ�����ֽ�
//	BLE_DAT_p--;   //ָ���λ 
//	
//	if((it1==0x5E)&&(it2==0xDC))
//	{
//		data_len =0;
//		BLE_Data_Len =0;  //���ְ�ͷʱ��ֱ�����֮ǰ����
//	}
	
		if(IN_BLE_Dat_SW==0)
		{
			IN_BLE_Dat_SW =1;
			for( irw =0;irw<Dat_Len;irw++)
			{
				BLE_Data[BLE_Data_Len] = *BLE_DAT_p; //����λ����Data_Len ����
				BLE_DAT_p++;
				if(BLE_Data_Len<(sizeof(BLE_Data)-2))  //�����ݻ��������пռ�ʱ��������������Ƶ�ַ
				BLE_Data_Len++;  //BLE���ݻ����ڵ����ݳ���
				else
					break; //���򵯳����ѭ��
			}
			BLE_DAT_SW();  //������ɺ��ʱ��������
		}
}

/***********************************************
�������ݹ��ܣ�������Ӧ���ܿ�ʼ���ֹ��ܡ�
***********************************************/
//�������ݰ���ʶ

/***********************************************
ָ�����ʽ

2�ֽڰ�ͷ+2�ֽڳ���+1�ֽ�ָ����+1�ֽڰ���ʶ
+1�ֽڰ����+4�ֽ��豸��ʶ+31�ֽڲ���+2�ֽ�У����+2�ֽڰ�β
***********************************************/
#define Sign_Add  5  //����ʶλ��

#define Command_Sign  0x01  //ָ���ʶ
#define Ack_Sign  0x02  //Ӧ���ʶ

//����ָ��ֵ
#define Command_Add 	0X04  //ָ�����ڱ����е�λ��
#define Mate_Command  0x43  //��ָ����
#define Time_Ref_Command 0x02  //����ʱ��ָ����
#define Get_Time_Command 0x03  //��ȡʱ��
#define APP_OpenLock_Command 0x04  //�������ָ����

#define Normally_Open_Door   0x05		//�ų���ָ��

#define Add_Card	0x10	 //�û��ֻ���������	���ָ��
#define Add_Information_Command 0x12 //��Ϣ����ָ����
#define Del_Information_Command	0x15  //������������Ϣָ����
#define Del_A_Information_Command 0x14 //ɾ������Ȩ��ָ��
#define Del_Mate_Command 0x30 //�����ָ��

#define More_Act_PassWord_Mode  0x50  //�޸�ʱЧ��̬����ģʽָ��  ֧��1������ͬʱ��Ч

#define Del_A_More_Act_PassWord 0x51  //�Ӷ��鶯̬������ɾ��������̬����

#ifdef APP3_Op_Door					//�����������APP ����ָ��ʱ

#define APP3_OpenLock_Command				0xA9				//������ APP ����ָ��

#endif

#define Read_OP_Door_note_Command 0x22  //ͬ�����ż�¼ָ��
#define Read_A_OP_Door_note_Command 0x45  //������ѯ���ż�¼ָ��
#define CL_OP_Door_note_Command 0x46  //��տ��ż�¼ָ��

void BLE_DAT_SW()
{
	uint16_t i,it,it1,it2; //ѭ����
	if ((BLE_Data_Len>=2)&&(data_len==0)) //������BLE����ͷʱ��ȷ��ͷ�Ƿ���ȷ�������������
	{
		if ((BLE_Data[0]==BLE_DAT_H1)&&(BLE_Data[1]==BLE_DAT_H2)) //�ж����ݰ�ͷ�����Ƿ���ȷ��ָ���
			//0x01��ʾ�յ�������ȷ��ָ����������������
		{
			;  //��ȷʱ�����������������������
		}
		else
		{
			BLE_Data_Len =0;  //��ͷ����ʱ��������յ�������
			data_len =0;  //���ݰ���������
			
			#ifdef uart_start
				SEGGER_RTT_printf(0,"BLE_Data_Error_0!\r\n");  //��ʾͷ����
			#endif
		} 
	}
	if ((BLE_Data_Len>=4)&&(data_len ==0)) //��ȡ���ݰ�����
	{
		if (BLE_Data[2]==0)
		{
		data_len = BLE_Data[3];  //��ȡ���ݰ�����
		}
		else
		{
			
//			data_len = BLE_Data[2];
//			data_len<<=8;
//			data_len+=BLE_Data[3];  //�ۼ�����			
			BLE_Data_Len =0;  //�����ݴ���255�ֽڣ�ֱ�Ӷ���
			
			data_len =0;
			#ifdef uart_start
				SEGGER_RTT_printf(0,"BLE_Data_Error_1!\r\n");  //��ʾ���ݰ�����ֵ����
			#endif
		}
		#ifdef uart_start
		SEGGER_RTT_printf(0,"data_len: %d\r\n",data_len);
		#endif
	//	BLE_Data_Len  =0;  //test		
		if (data_len<data_len_min)  //���յ�����ֵС����С���ȣ����������
		{
			BLE_Data_Len =0;
			data_len =0;
			#ifdef uart_start
			SEGGER_RTT_printf(0,"data_len Error_2!\r\n");  //�������ݳ���ֵ�����ʾ�����������
			#endif
		}
	}
		//�������ȱ�ʾ����Чʱ�Ĵ���
	if((BLE_Data_Len>=(data_len+6))&&(data_len>=data_len_min)) //�����ݰ������Ƚ�����ɺ�
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"BLE_Dat_get:\r\n");  //����һ�����ݰ�
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
		for(i =0;i<(data_len-2);i++)  //У����������Чֵ���
		{
			it +=BLE_Data[i+4];
		}
		if((it==it1)&&(BLE_Data[data_len+4]==BLE_DAT_E1)&&(BLE_Data[data_len+4+1]==BLE_DAT_E2))  //У��ͨ����
		{
			#ifdef uart_start
			SEGGER_RTT_printf (0,"Cheak_ok!\r\n");
			#endif
			BLE_Dat_Select();
		}
		else			//У������Ͽ�����
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Cheak_Error!\r\n");
			
			#endif
			Dis_Conner();			//У��ʧ�ܶϿ�����
		}
		
		//�������������ݣ���ʼ�µ����ݰ�����
		data_len =0;
		BLE_Data_Len =0;
	}
	IN_BLE_Dat_SW =0;
}

void BLE_Dat_Select()  //����ָ����ת�Ӻ���
{
//	uint32_t err_code;
		switch (BLE_Data[Command_Add])
		{
			case Mate_Command:  //��ָ����Ϊ��ָ��ʱ
				if (BLE_Data[Sign_Add]==Command_Sign) //������ʶΪָ���ʶʱ
					{
						Mate_Command_Run();
					}						
				else if (BLE_Data[Sign_Add]==Ack_Sign) //������ʶΪӦ���ʶʱ
					{
						
					}
					else
					{
						Dis_Conner();
					}
					
				break;
			case Time_Ref_Command:  //��ָ����Ϊ��ָ��ʱ
				if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
					{
						Time_Ref_Run();  //����ʱ�����ָ��
					}						
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
				
			case Get_Time_Command:  //��ָ����Ϊ��ָ��ʱ
				if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
					{
						Get_Time_Run();  //����ʱ�䷵��ָ��
					}						
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
							
			case APP_OpenLock_Command:  //��ָ����Ϊ����ָ��ʱ
				
			if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
					{
						APP_OpenLock_Run();  //����ʱ�䷵��ָ��
					}
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
				case Add_Information_Command:
					if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
						{
							Add_Information_Run();  //������Ϣ����ָ��
						}
					else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
						{
							
						}
						else
						{
							Dis_Conner();
						}				
							break;
				case Del_Information_Command:
					if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
						{
							Del_Information_Run();  //������Ϣɾ��ָ��
						}
					else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
						{
							
						}
						else
						{
							Dis_Conner();
						}				
							break;
					
				case Del_A_Information_Command:
				if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
				{
					if((BLE_Data[13]==0x01)||(BLE_Data[13]==0x02)||(BLE_Data[13]==0x03)||(BLE_Data[13]==0x04))  //��Ҫ�����Ȩ����ϢΪ�����TypeA/B��ʱ
					{
						Del_A_PassWord_Run();  //����ɾ������Ȩ��ָ��
					}
				}
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
				{
					
				}
				break;
				
				case Del_Mate_Command:
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))			//���
					{
						Del_Mate_Run();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}
					break;
					
					case Read_OP_Door_note_Command:				//��ָ���޸�Ϊ��ȡ���ؿ��ż�¼ ����
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 Read_OP_Door_note();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}
						break;
					
					
					case Read_A_OP_Door_note_Command:		//��ȡ��Ӧ��ŵ� ���ż�¼
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 TX_Door_note();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}
						break;
					
					case CL_OP_Door_note_Command:		//��� ���ż�¼
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 CL_Door_note();
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}
						break;
					
					
					
					case Add_Card:
						
					 if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))				//�ֶ�����
						{
							input_speak(22); //�ȷ�����������
							Manual_Add_Card();
						}				
						else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
						{
							
						}
						
						break;
						
					case Normally_Open_Door:  //��������ָ��
					
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 Normally_Open_Door_Run();		//���г�������ָ��
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}	
					
					break;
					
					case  More_Act_PassWord_Mode:   //�޸�ʱЧ��̬����ģʽָ��  ֧��1������ͬʱ��Ч (0X50)
					
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 More_Act_PassWord_Mode_Run();		//���г�������ָ��
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}
					
					break;
					
					case Del_A_More_Act_PassWord:			//ɾ������ʱЧ���� (0X51)
					
						
					if((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true))
					{
						 Del_A_More_Act_PassWord_Run();		//���г�������ָ��
					}
					else if((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true))//������ʶΪӦ���ʶʱ
					{
						
					}
					
					break;
					
					
					#ifdef APP3_Op_Door						//���������������ָ��
					
					case APP3_OpenLock_Command:
						
					if ((BLE_Data[Sign_Add]==Command_Sign)&&(Devic_Static==true)) //������ʶΪָ���ʶʱ
					{
						APP_OpenLock_Run();  //����ʱ�䷵��ָ��
					}
				else if ((BLE_Data[Sign_Add]==Ack_Sign)&&(Devic_Static==true)) //������ʶΪӦ���ʶʱ
					{
						
					}
					else
					{
						Dis_Conner();
					}
						break;
					#endif
				default :  //�������
								break;
		}
}

#define Mate_Dat_Len  40 //���ð󶨲�������  ���������ֽڰ󶨺ź�24�ֽڹ㲥���ƣ�
#define Mate_Com_Add	11
#define Mate_Ack_Len  28
extern uint8_t TSM12MC_Status;		//�ж� TSM12MC�Ƿ����

extern uint32_t id1,id2;  //���ڷ������豸ID��     
void Mate_Command_Run()
{
	
	uint32_t i,it,it1;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Mate_Command_Run!\r\n");
	#endif

	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x1E;  //����
	
	TX_Buff[4] = 0X43;  //ָ���� 
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;
	it1 &=0xff00;
	it1>>=8;
//	it1+=3;
	if(it1>127)  //�޸��ֽ����
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
	
	TX_Buff[11] = 0X01;  //��ʾ�󶨳ɹ�
	
	TX_Buff[12] = BLE_Data[11]; //�˺ű��
	TX_Buff[13] = BLE_Data[12];
	TX_Buff[14] = BLE_Data[13];
	TX_Buff[15] = BLE_Data[14];
	
	TX_Buff[16] = TSM12MC_Status;  //�����Ƿ�������� ��������״̬
	
	TX_Buff[17] = 0;  //�����Ƿ��ָ�� Ĭ��д0
	
	#ifdef PS_Enable  

	TX_Buff[17] = 1;  //ʹ�ܴ�ָ�ƺ�д1
		
	#endif
	
	//����
	it = Version_Num;
	
	TX_Buff[18] = it>>24&0xFF;  //Version_Num  �汾�� ��һλ �Ӹ�����

	TX_Buff[19] = it>>16&0XFF;  //

	TX_Buff[20] = it>>8&0XFF;  //

	TX_Buff[21] = it&0XFF;  //

	it = Authorization_time;  //Authorization_time ��Ȩʱ��  ��һλ �Ӹ�����

	TX_Buff[22] = it>>24&0XFF;  //��

	TX_Buff[23] = it>>16&0XFF;  //��

	TX_Buff[24] = it>>8&0XFF;  //��

	TX_Buff[25] = it&0xFF;   //ʱ
	
	TX_Buff[26] = 0;  //  �Ƿ�֧��ͬʱ��Ч���鶯̬���� 1 ��ʾ֧�� 0 ��ʾ��֧��
	
	TX_Buff[27] = 0;  // ����
	
	TX_Buff[28] = 0;  // ����
		
	TX_Buff[29] = 0;  // ����
		
	TX_Buff[30] = 0;  // ����
	
	TX_Buff[31] = 0;  // ����
	
	
	
	
	
	it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Mate_Ack_Len-4);i++)  //У����������Чֵ���
		{
			
			it +=TX_Buff[i+4];
		
		}
		
		if(Devic_Static==false) //���豸δ��ʱ ��������
		{
     
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Write_Devic_Num!\r\n");
			#endif
			
//			NRF_WDT_Feed();
			
			for(i =0;i<sizeof(Devic_Num);i++)
			Devic_Num[i] =0;
			
			for	(i =0;i<Mate_Dat_Len;i++)
			{
				Devic_Num[i] = BLE_Data[Mate_Com_Add+i];		//���ϰ�����λ��
			}
			
			Spi_Flash_Init();  //����SPI_FLASH ͨѶ
			Devic_static_read();  //(��ȡ�豸��״̬)
			Write_Devic_Num();  //д���豸����Ϣ
			SPI_Flash_Busy();
			Devic_static_read();  //(�����豸��״̬)
			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
				
				
			Spi_Flash_Init();  //����SPI_FLASH ͨѶ	

			if(BLE_Data[64]==1)    //��������������� ��̬����ģʽ
			{
				//���óɶ��鶯̬����ģʽ
				
				Ev_Cat_Dat(More_Act_PassWord_Num_Code,0xFFFF0000);  
				//�Ը�λ��Ϊ ��׼ ����λΪ0XFFFFʱΪ ���鶯̬����״̬ 
								
				TX_Buff[26] = 1;  //  �Ƿ�֧��ͬʱ��Ч���鶯̬���� 1 ��ʾ֧�� 0 ��ʾ��֧��
	
			}
			else if(BLE_Data[64]==0)
			{
				//���óɵ���ʱЧ��̬����ģʽ
				Ev_Cat_Dat(More_Act_PassWord_Num_Code,0);  
				//�Ը�λ��Ϊ ��׼ ����λΪ0XFFFFʱΪ ���鶯̬����״̬ 
				TX_Buff[26] = 0;  //  �Ƿ�֧��ͬʱ��Ч���鶯̬���� 1 ��ʾ֧�� 0 ��ʾ��֧��
	
			}
			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
			
			
				it1 =it;
				it1&=0xff00;
				it1>>=8;
				TX_Buff[32] = it1;
				it1 = it;
				it1&=0x00ff;
				TX_Buff[33] = it1;
				TX_Buff[34] = BLE_DAT_E1;
				TX_Buff[35] = BLE_DAT_E2;  //������������ڻ�Ӧ
					
			
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
			
			if(Devic_Static==true) //�󶨳ɹ�
			{
					for(i =0;i<20;i++)
					BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
				
					BLE_TX_Run(BLE_TX,20);   //���͵�һ����
				
					for(i =0;i<20;i++)
					BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�		
				
					BLE_TX_Run(BLE_TX,20);   //���͵�һ����				
					
					ref_BLE_Adv();     //���¹㲥����
					//input_speak(0);
					input_speak(3); //���������ʾ�󶨳ɹ�
					nrf_delay_ms(500);
			}
	//		read_Soctor(0);   //��ȡ����0����
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
		
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
}

#define Time_Ref_Ack_Len	22
uint8_t Update_Time_Flag =0;
extern uint32_t Board_Unix;
void Time_Ref_Run()
{
		uint32_t i,it,it1;
		uint64_t Input_Time;
		uint8_t TX_Buff[100];  //����������
		uint8_t BLE_TX[20]; //һ�η��͵�����
	//	uint32_t err_code;
		
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Time_Ref_Run!\r\n");
		#endif
		
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;
		
		TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
		TX_Buff[1] = BLE_DAT_H2;
		
		TX_Buff[2] = 0x00;
		TX_Buff[3] = 0x10;  //����
		
		TX_Buff[4] = 0X02;  //ָ����
		TX_Buff[5] = 0x02;  //����ʶ
		TX_Buff[6] = BLE_Data[6]; //���
		
		it1 = id1;			//�豸��ʶ
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
	
		TX_Buff[11] = 0X01;  //��ʾ�󶨳ɹ�
		
		TX_Buff[12] = 0; //��ʾ������Ч
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //�޹���
		TX_Buff[17] = 0;  //�޹���
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
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
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�		
		
			BLE_TX_Run(BLE_TX,20);				
}

#define Get_Time_Ack 30
#define Base_Volt_Value  380000
#define Max_Volt_Value  600000

uint8_t Batt_Value_T =100;  //ʵ��ֵ

uint8_t Batt_Value()  //������ȼ�
{
		uint32_t it1,it2;
		ADC_Value =nrf_adc_read();
//			Get_Time();  //��ʱ����ʱ��
		#ifdef uart_start
		SEGGER_RTT_printf(0,"ADC_Value: %d\r\n",ADC_Value);
		#endif
	
		ADC_Div = VDD_Value/1024;
		VCC_Value = ADC_Div*ADC_Value*(RES_d+1);
	//VCC_Value+=20000; //���������ܵ������

		VCC_Value_H = VCC_Value/100000;
		VCC_Value_L1 = VCC_Value/10000%10;
		VCC_Value_L2 = VCC_Value/1000%10;
		
#ifdef uart_start
	SEGGER_RTT_printf(0,"VCC_Value: %d.%d%dV\r\n",VCC_Value_H,VCC_Value_L1,VCC_Value_L2);
#endif
			//VCC_Value/=1000; //�������λ��
			if(VCC_Value>=Max_Volt_Value)  //��ֵ������ߵ�ѹֵ��ֱ�ӷ���100
				return 100;
			else if(VCC_Value<=(Base_Volt_Value+5000))
				return 0;
			it2= VCC_Value-Base_Volt_Value;  //��ȥ������ѹֵ
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

void Get_Time_Run()  //����ʱ���ȡָ��
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Get_Time_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x18;  //����
	
	TX_Buff[4] = 0X03;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	TX_Buff[11] = 0X01;  //��ʾ�ɹ���ȡʱ��
	
	TX_Buff[12] = YEAR; //���
	TX_Buff[13] = MON; //�·�
	TX_Buff[14] = DAY; //����
	TX_Buff[15] = HOUR; //ʱ��
	
	TX_Buff[16] = MIN;  //��
	TX_Buff[17] = SEC;  //��
	
	TX_Buff[18] = 0x02;  //���³�ʼ���� //ֱ�ӷ���0X02
	
	Read_Lock_Bon();
	if(Turn_Key) //Կ�״���ʱ
	TX_Buff[19] = 0x01;
	else 
	TX_Buff[19] = 0x02;
	
	if(Double_Key) //���ഥ��ʱ
	TX_Buff[20] = 0x01;
	else 
	TX_Buff[20] = 0x02;
	
	if(Latch_Bolt) //б�ഥ��ʱ
	TX_Buff[21] = 0x01;
	else 
	TX_Buff[21] = 0x02;
	
	TX_Buff[22] = 6; 							 //�㲥�ȼ�
	
	TX_Buff[23] = Batt_Value();  	//�����ȼ�
	
	it = 0; it1 =0;

//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

			for(i =0;i<(Get_Time_Ack-4);i++)  //У����������Чֵ���
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
			TX_Buff[27] = BLE_DAT_E2;  //������������ڻ�Ӧ					
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�		
		
			BLE_TX_Run(BLE_TX,20);				
}

extern uint8_t PassWord_Error_Num;  //��¼��������������
extern uint32_t PassWord_Error_Time;  //����������ʱ
#define PassWord_Error_Num_Max 6

void APP_OpenLock_Run()
{
	uint32_t i,it,it1;
	uint64_t Input_Time;		//ʱ��
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"App_OpenLock_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = BLE_Data[4];  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
	//����ʶ�������ָ�����ʱ��
	
	if(BLE_Data[11]>0)			//�ж�����Ƿ���Ч
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
	
	
	
	Spi_Flash_Init();  //����SPI_FLASH ͨѶ
	
	Devic_static_read();  //(��ȡ�豸��״̬)
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();
	
	
	#ifdef APP3_Op_Door 		//������ APP3 ����ָ��ʱ
	
	if(BLE_Data[4]==APP3_OpenLock_Command)    //��ָ���ǵ���������ָ��ʱ   //��ҪУ������Ƿ���ȷ
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
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�
//	nrf_delay_ms(1); //�ȴ��ȶ�
//	if(nrf_gpio_pin_read(5)==0)
//	TX_Buff[11] = 0x02;  //���ŷ���ʱ���迪��
//	else
	
	TX_Buff[11] = 0X01;  //��ʾ���ųɹ�
	TX_Buff[12] = Batt_Value();			//�������ֵ
	
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);

	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //�޹���
	TX_Buff[17] = 0;  //�޹���
	
	it = 0; it1 =0;
	
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);						
			
			
			if(TX_Buff[11]==0X01) //��������ʱ
			{
			
				if(MPR121_RUN==0)
				nrf_gpio_pin_set(LEDB);  //���ź��LED��
				Lock_Open();
				
			}
//			else
//			input_speak(17); //������ʾ�ѷ���
//		
		//	mpr121_Run_I(); 
//			mpr121_init_I();  //���¶�ȡһ�λ���ֵ
//			mpr121_init();
			TSM12_Init();
			nrf_gpio_pin_clear(LEDB);  //�ص�
			
		
			if(PassWord_Error_Num>=PassWord_Error_Num_Max)
				PassWord_Error_Time = 9000;// ��������
			
			
//			for(i =0;i<20;i++)
//			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
//		
//			BLE_TX_Run(BLE_TX,20);
//			for(i =0;i<20;i++)
//			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
//			BLE_TX_Run(BLE_TX,20);						
//			
}

/*
void APP_OpenLock_Run()
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"App_OpenLock_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = BLE_Data[4];  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
	Spi_Flash_Init();  //����SPI_FLASH ͨѶ
	
	Devic_static_read();  //(��ȡ�豸��״̬)
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();
	
	
	#ifdef APP3_Op_Door 		//������ APP3 ����ָ��ʱ
	
	if(BLE_Data[4]==APP3_OpenLock_Command)    //��ָ���ǵ���������ָ��ʱ   //��ҪУ������Ƿ���ȷ
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
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�
//	nrf_delay_ms(1); //�ȴ��ȶ�
//	if(nrf_gpio_pin_read(5)==0)
//	TX_Buff[11] = 0x02;  //���ŷ���ʱ���迪��
//	else
	
	TX_Buff[11] = 0X01;  //��ʾ���ųɹ�
	
//	nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);

	TX_Buff[12] = 0; //��ʾ������Ч
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //�޹���
	TX_Buff[17] = 0;  //�޹���
	
	it = 0; it1 =0;
	
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);						
			
			
			if(TX_Buff[11]==0X01) //��������ʱ
			{
			
				if(MPR121_RUN==0)
				nrf_gpio_pin_set(LEDB);  //���ź��LED��
				Lock_Open();
				
			}
//			else
//			input_speak(17); //������ʾ�ѷ���
//		
		//	mpr121_Run_I(); 
//			mpr121_init_I();  //���¶�ȡһ�λ���ֵ
//			mpr121_init();
			TSM12_Init();
			nrf_gpio_pin_clear(LEDB);  //�ص�
			
		
			if(PassWord_Error_Num>=PassWord_Error_Num_Max)
				PassWord_Error_Time = 9000;// ��������
			
			
//			for(i =0;i<20;i++)
//			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
//		
//			BLE_TX_Run(BLE_TX,20);
//			for(i =0;i<20;i++)
//			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
//			BLE_TX_Run(BLE_TX,20);						
//			
}
*/
uint8_t EX_BLE_ACK_TX[100];  //ͨ���ⲿ���BLE ACK TX ���� (��Ҫ���¼�����ݹ���)
void Add_Information_Run() //������Ϣ����ָ��
{
	uint8_t PassWord[50];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	uint32_t Start_Unix; //��Чʱ��
	uint32_t Stop_Unix;  //ʧЧʱ��
	uint16_t Card_ID;		//�����ȡ���Ŀ����
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Add_Information_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = 0X12;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
		TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
		
		TX_Buff[12] = 0; //��ʾ������Ч
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //�޹���
		TX_Buff[17] = 0;  //�޹���
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;
	
	
		for(i =0;i<sizeof(PassWord);i++)PassWord[i] =0x00; //��ջ�����
		
		if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x02)) //����Ա���� (��һ�ֽ�01��ʾ����Ա���ڶ��ֽ�02��ʾ����)
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_PassWord!\r\n");
			#endif
			
			PassWord[0] = 0x12; //��ʾ����Ա���� //�����ֽ�����
			for(i =1;i<18;i++)
			{
				PassWord[i] = BLE_Data[12+i];
			} //����������
	
			Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
		
			TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
			TX_Buff[12] = Add_Flash_PassWord(PassWord); //д�뱣��ID��
//			NRF_WDT_Feed();	
			if(TX_Buff[12]==0) //��д�뷵�ش���
			{
				TX_Buff[11] =0x02;
			}
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x02)) //�û����� (��һ�ֽ�02��ʾ��ͣ��ڶ��ֽ�02��ʾ����)
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_PassWord!\r\n");
			#endif
			
			PassWord[0] = 0x22; //��ʾ�û����� //�����ֽ�����
			for(i =1;i<18;i++)
			{
				PassWord[i] = BLE_Data[12+i];
			} //����������
	
			Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
			
			TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
			TX_Buff[12] = Add_Flash_PassWord(PassWord); //д�뱣��ID��
//			NRF_WDT_Feed();
			if(TX_Buff[12]==0) //��д�뷵�ش�����ʾ����ʧ��
			{
				TX_Buff[11] =0x02;
			}
		}
		else if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x03))  //����Ա Type A ��
		{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_TypeA_Card!\r\n");
			#endif
			
			Spi_Flash_Init();  //����Flash ͨѶ
			
			Start_Unix = 0;
			Stop_Unix =0;  //��ʼ�� Unix ʱ��
			
			for(i =0;i<4;i++)
			  PassWord[i] = BLE_Data[21+i];  //ʹ�� PassWord �ַ��� ���� ָ��������� Ȩ��ģʽ Ȩ�����ͺ�ʱ����������
			
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
			 
				if(Card_ID==0)  //�ж������Ƿ�д��ɹ� д��ʧ��ʱ�ı� TX_Buff[11] ��״̬
					TX_Buff[11] =0x02;
			
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x03))  //�û� Type A ��
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_TypeA_Card!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //��ʼ�� Unix ʱ��
			
			for(i =0;i<4;i++)
				PassWord[i] = BLE_Data[21+i];  //ʹ�� PassWord �ַ��� ���� ָ��������� Ȩ��ģʽ Ȩ�����ͺ�ʱ����������
						
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
			 
				if(Card_ID==0)  //�ж������Ƿ�д��ɹ� д��ʧ��ʱ�ı� TX_Buff[11] ��״̬
					TX_Buff[11] =0x02;
			
		}
		else if((BLE_Data[11]==0X01)&&(BLE_Data[12]==0x04))   //����Ա Type B ��
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_TypeB_Card!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //��ʼ�� Unix ʱ��
			
			for(i =0;i<8;i++)
				PassWord[i] = BLE_Data[21+i];  //ʹ�� PassWord �ַ��� ���� ָ��������� Ȩ��ģʽ Ȩ�����ͺ�ʱ����������
			
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
			 
				if(Card_ID==0)  //�ж������Ƿ�д��ɹ� д��ʧ��ʱ�ı� TX_Buff[11] ��״̬
					TX_Buff[11] =0x02;
			
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x04))  //�û� Type B ��
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_TypeB_Card!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //��ʼ�� Unix ʱ��
			
			for(i =0;i<8;i++)
				PassWord[i] = BLE_Data[21+i];  //ʹ�� PassWord �ַ��� ���� ָ��������� Ȩ��ģʽ Ȩ�����ͺ�ʱ����������
				
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
			 
				if(Card_ID==0)  //�ж������Ƿ�д��ɹ� д��ʧ��ʱ�ı� TX_Buff[11] ��״̬
					TX_Buff[11] =0x02;
			
		}
		
		#ifdef PS_Enable
		
		else if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x01)&&(F_print_Enable==1))			//����Աָ��
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_Admin_PS_Lmage!\r\n");
			#endif
			
			Spi_Flash_Init();
			
			Start_Unix = 0;
			Stop_Unix =0;  //��ʼ�� Unix ʱ��
			
		 Start_Unix = BLE_Data[13];				//������ʼʱ��ͽ���ʱ��
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

		Input_Ps_Lmage_Type =0x01;				//��ʼ��ָ��¼�����ֵ ����Ϊ����Աָ��
		Start_PS_Unix = Start_Unix;				//��Чʱ��
		Stop_PS_Unix = Stop_Unix;					//ʧЧʱ��
		
		for(i= 0;i<sizeof(TX_Buff);i++)				//�ⲿ�ֲ�ʹ�ô˺�����Ӧ�����Ӧ��ʹ�ü�ʱ�ⲿӦ��ķ�ʽ���
			EX_BLE_ACK_TX[i] = TX_Buff[i];
		
		}
		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x01)&&(F_print_Enable==1))			//�û�ָ��
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Add_User_PS_Lmage!\r\n");
			#endif
			
			
			 Start_Unix = BLE_Data[13];			//������ʼʱ��ͽ���ʱ��
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

			
			Input_Ps_Lmage_Type =0x02;				//��ʼ��ָ��¼�����ֵ ����Ϊ����Աָ��
			Start_PS_Unix = Start_Unix;				//��Чʱ��
			Stop_PS_Unix = Stop_Unix;					//ʧЧʱ�� 

			for(i= 0;i<sizeof(TX_Buff);i++)				//�ⲿ�ֲ�ʹ�ô˺�����Ӧ�����Ӧ��ʹ�ü�ʱ�ⲿӦ��ķ�ʽ���
				EX_BLE_ACK_TX[i] = TX_Buff[i];
				
		
		}
		#endif
		
		else
		{	//����ʧ�� //δ��չ����д�빦��
			TX_Buff[11] = 0x02; //��ʾ����ʧ��
			TX_Buff[12] = 0X00;
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Add_Information_Order_Warning!\r\n");  //������Ϣ�������
			
			#endif
	//		Spi_Flash_Init();
		}
	
			if(TX_Buff[11]==0X01&&(BLE_Data[12]==0x02)) // ����������ʾ�����ɹ�
		input_speak(9);
		else if(TX_Buff[11]==0x02)			// ʧ��ʱ��ʾ����ʧ��
			input_speak(10);
			
	
//����ȡ����У����
	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
		TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
		
		#ifdef PS_Enable
		if((BLE_Data[12]!=0x01)||(F_print_Enable==0))			//���ڷ�ָ��¼�������������ڵ�ǰ������Ӧ����������
		{
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
		}
		#else
		
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
		
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
		
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
		
		#endif
			

		
//			NRF_WDT_Feed();  //��֤��
//			read_Soctor(0);
//			NRF_WDT_Feed();
//			read_Soctor(1);
//			NRF_WDT_Feed();
//			read_Soctor(2);
//			NRF_WDT_Feed();

		
}

/***********************************

�ⲿ BLE Ӧ����

���룺 ACK_Type		Ӧ������(�ɹ���ʧ��)  0X01 ��ʾ�ɹ�  0X02  ��ʾʧ��

***********************************/

void EX_BLE_ACK_Run(uint8_t ACK_Type,uint8_t ACK_ID)
{
	
	uint16_t it,it1;
	
	uint8_t BLE_TX[20]; //һ�η��͵�����
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
		
		SEGGER_RTT_printf(0,"EX_BLE_ACK_BUFF_Error!\r\n");			//��ʾ���쳣
		
		#endif
		
		return;
		
	}
	if(ACK_Type==0X01)		//����ɹ�ʱ
	{
		EX_BLE_ACK_TX[11]=ACK_Type;
	}
	else if(ACK_Type==0x02)		//����ʧ��ʱ
	{
		EX_BLE_ACK_TX[11]=ACK_Type;
	}
	else
	{
		for(uint8_t i;i<sizeof(EX_BLE_ACK_TX);i++)
		{
			EX_BLE_ACK_TX[i] =0;		//��ջ���
		}
		return;		//��������ֵ��Ч��ֱ�ӽ���
		
	}
	
	if(BLE_LINK==0)		//������δ����ʱ���������ݣ����������
	{
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"BLE_Link_Error!\r\n");		//��ʾBLE���Ӵ���
		
		#endif
		for(uint8_t i;i<sizeof(EX_BLE_ACK_TX);i++)
		{
			EX_BLE_ACK_TX[i] =0;		//��ջ���
		}
		return;
		
	}
	
	EX_BLE_ACK_TX[12] = ACK_ID;			//��Ҫ���ص�ID
	
	//����ȡ����У����
	for(uint16_t i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
		EX_BLE_ACK_TX[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
		
		if(BLE_Data[12]==0x01)			//����ָ��¼����������������������ڵ�ǰ������Ӧ����������
		{
			for(uint8_t i =0;i<20;i++)
			BLE_TX[i] =EX_BLE_ACK_TX[i]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
			for(uint8_t i =0;i<20;i++)
			BLE_TX[i] = EX_BLE_ACK_TX[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
		}
		else
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"EX_BLE_ACK_BUFF_Type_Error!\r\n");		//��ʾӦ���Ŀ�����
			
			#endif
			
			return;		//��������
		}
		
}

void updata_Devic_Num()//�����豸������
{
	uint8_t read_tab [Devic_PAGE_Len];
	uint8_t tab[Devic_PAGE_Len];
	uint32_t i;
//	uint32_t dat;
	i =0;
	SpiFlash_Read_Data
	(read_tab,Devic_Num_BLOCK,
	Devic_Num_PAGE,Devic_PAGE_Len); //��ȡ����ȷ��������Ч��
	
	if(!((read_tab[0] == PAGE_Heart_H)&&
		(read_tab[1] == PAGE_Heart_L))) //�ж���ʼ������Ч�� ֻ�޸���Ч����
		{
			//����ʼ������Чʱ�����԰��������ı�
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_Num not Write!\r\nDevic_static_true!\r\n");
			
			#endif
		}
		else
		{  //����ʼ������Чʱ����ʼд������
			SPIFlash_Erase_Sector(Devic_Num_BLOCK,Devic_Num_Sector); //���豸�󶨺������������в���
			
			SPI_Flash_Busy(); //�ȴ��������
			
			tab[0] = PAGE_Heart_H;
			tab[1] = PAGE_Heart_L;
			for (i =0;i<Devic_Num_Len;i++)
			{
				tab[i+2] = Devic_Num[i];
			}
			SpiFlash_Write_Data(tab,Devic_Num_BLOCK,Devic_Num_PAGE,sizeof(tab));
			SPI_Flash_Busy(); //�ȴ�д�����
			
			SpiFlash_Read_Data (read_tab,Devic_Num_BLOCK,Devic_Num_PAGE,Devic_PAGE_Len);
			#ifdef uart_start 
			SEGGER_RTT_printf(0,"Devic_Num Write OK!\r\nDevic_Num:\r\n");
			for(i =0;i<sizeof(read_tab);i++)
			SEGGER_RTT_printf(0,"%x,",read_tab[i]);
			SEGGER_RTT_printf(0,"\r\n",read_tab[i]);
			
			#endif
		}
}


extern const uint8_t Act_PassWord_Secotr ;  //�洢��̬���� 

void Del_Information_Run()
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint32_t Block,Page;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	uint32_t Code_Num;  //ɾ����ʱ�Ļ���
		
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Del_Information_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = 0X15;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
	TX_Buff[12] = 0; //��ʾ������Ч
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //�޹���
	TX_Buff[17] = 0;  //�޹���
	it = 0; it1 =0;

	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
	TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
	
	if(BLE_Data[11]==0x01) //�������Ա��Ϣ
	{
		
		Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
		Del_Admin_PassWord();
		Devic_static_read();  //���� �豸����Ϣ(����ˢ�²��ְ�����)
		//�������Ա����ʱ����Ҫ���°������е�����У����
		//		���������ݣ�
		//���ֽ��˺ű��+24�ֽڹ㲥����ASIIC��+
		//���ֽ��豸У����+���ֽ�����У����+2�ֽ�¥����
		Devic_Num[32] = BLE_Data[12];
		Devic_Num[33] = BLE_Data[13];
		Devic_Num[34] = BLE_Data[14];
		Devic_Num[35] = BLE_Data[15]; //�滻����У����
		
		updata_Devic_Num();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_AD_TypeB_Data!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);
		
		if(Code_Num!=0)			//���й���ԱB��ʱ��չ���ԱB��
		{
			Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //��չ���Ա Type B ����Ϣ
			Erase_AD_TypeB_Sector();
		}
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_AD_TypeA_Data!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);
		if(Code_Num!=0)			//���й���Ա A ��ʱ��չ���ԱA������
		{
		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //��չ���Ա Type A ����Ϣ
		Erase_AD_TypeA_Sector();			//��� ����Ա A ����������
			
		}
		Ev_Cat_Dat(Nmg_Auth_Index_Code,0); //������Ա ���ݿ����к����
		
	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_Nmg_Auth_Index_Code!\r\n");
		
		#endif
		
	}
	else if(BLE_Data[11]==0x02) //����û�����
	{
		Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
		Del_User_PassWord();
		Ev_Cat_Dat(0x04,0);		//�����������
		Devic_static_read();  //���� �豸����Ϣ(����ˢ�²��ְ�����)
		//����������ʱ����ѽ���°������е��豸У���� (ͬʱ�����̬����)
		Devic_Num[28] = BLE_Data[12];
		Devic_Num[29] = BLE_Data[13];
		Devic_Num[30] = BLE_Data[14];
		Devic_Num[31] = BLE_Data[15]; //�滻�豸У����
		
		updata_Devic_Num();
		Ev_Cat_Dat(0x02,0);  //����̬�����������
		Ev_Cat_Dat(0x03,0);  //����ʷ��̬�����������
		
		for(i =0;i<sizeof(tab_dat);i++)
		tab_dat[i] =0x00;
		
		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,2); //���Ѿ������ʱЧ���븲��
		SPI_Flash_Busy();	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_TypeB_Data!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //����û� Type B ����Ϣ
		Erase_US_TypeB_Sector();
		}
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_TypeB_Data!\r\n");
		
		#endif
		
		
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //����û� Type A ����Ϣ
		Erase_US_TypeA_Sector();
		}
		
		Ev_Cat_Dat(CK_Auth_Index_Code,0); //���û� ���ݿ����к����
		
		Code_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);    //������ն����û�Կ�׵�ָ��
		
		Ev_Cat_Dat(More_Act_PassWord_Num_Code,Code_Num&0xFFFF0000);  //����û� Type B ����Ϣ
				
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_CK_Auth_Index_Code!\r\n");
		
		#endif
		
	}
		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep();
	
		for(i =0;i<20;i++)
		BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
	
		BLE_TX_Run(BLE_TX,20);
		for(i =0;i<20;i++)
		BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
		BLE_TX_Run(BLE_TX,20);	
	
	// 	input_speak(9);
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();
	
}

void Del_User_Key()		//����û�Կ��(�����ܿ��ŵ��û���Ϣ)
{
//	uint16_t i;
	uint32_t Code_Num;
//	uint32_t Block,Page;

	Del_User_PassWord();		//����û��Զ�������
	
	//	Ev_Cat_Dat(0x04,0);  //���������������
	
//	Ev_Cat_Dat(0x02,0);  //����̬�����������
//	Ev_Cat_Dat(0x03,0);  //����ʷ��̬�����������
	
//	for(i =0;i<sizeof(tab_dat);i++)			//��ջ���
//		tab_dat[i] =0x00;
//		
//		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
//		SPIFlash_Write_Tab(tab_dat,Block,Page,2); //���Ѿ������ʱЧ���븲��
//		SPI_Flash_Busy();	
//	
			Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //����û� Type B ����Ϣ
		Erase_US_TypeB_Sector();
		}
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_TypeB_Data!\r\n");
		
		#endif
		
		
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //����û� Type A ����Ϣ
		Erase_US_TypeA_Sector();
		}
		
		Ev_Cat_Dat(CK_Auth_Index_Code,0); //���û� ���ݿ����к����
		
		Code_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);    //������ն����û�Կ�׵�ָ��
		
		Ev_Cat_Dat(More_Act_PassWord_Num_Code,Code_Num&0xFFFF0000);  //����û� Type B ����Ϣ
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Del_US_CK_Auth_Index_Code!\r\n");
		
		#endif
	
	
}


/*********************************************

��ɾ����ʱ��ֻ���ֹ���Ա���û��������Ե��ſ�����

**********************************************/

void Del_A_PassWord_Run()  //ɾ�������������� ������
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	uint32_t Code_Num;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Del_A_PassWord_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = 0X14;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
	TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
	TX_Buff[12] = 0; //��ʾ������Ч
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //�޹���
	TX_Buff[17] = 0;  //�޹���
	it = 0; it1 =0;
	
	
	Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	
	if(BLE_Data[13]==0x02) //��ָ��Ϊɾ������ָ��ʱ ���������ɶ���SN����ƣ��ʲ��ÿ����Ƿ�Ϊ����Ա
	{
		
		if(Del_SN_PassWord(BLE_Data[12])==1)  //�ж�ɾ���������Ƿ�ɹ�ɾ��
		{
			
			TX_Buff[11] = 0x01;
			
		}
		else
		{
			
			TX_Buff[11]=0x02;
		}
		
	}
	else if ((BLE_Data[13]==0x03)&&(BLE_Data[11]==0x01))  // ��ָ��Ϊ ����Ա Type_A ����ʱ��
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nDel_AD_TypeA_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);  //�жϱ��������Ƿ����1 �������
		
		if(Code_Num!=0)
		{
			Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //��չ���Ա A ��
			Erase_AD_TypeA_Sector();
		}
		Code_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);
		
		if(Code_Num!=0)
		{
			Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //��չ���Ա B ��
			Erase_AD_TypeB_Sector();
		}
		
		TX_Buff[11] =0X01;
		
		
		
//		if(Del_AD_Type_A(BLE_Data[12]))  //�������ſ�
//			TX_Buff[11] =0X01;
//		else
//		TX_Buff[11]=0x02;
	
	}
	else if ((BLE_Data[13]==0x03)&&(BLE_Data[11]==0x02))  //��ָ��Ϊ �û� Type_A ��ʱ
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nDel_US_TypeA_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);  //�жϱ��������Ƿ����1 �������
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //����û� A ��
		Erase_US_TypeA_Sector();
		}

		Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //����û� B ��
		Erase_US_TypeB_Sector();
		}
		
		TX_Buff[11] =0X01;
		
//		if(Del_US_Type_A(BLE_Data[12]))   //�������ſ�
//			TX_Buff[11]=0x01;
//		else
//		TX_Buff[11]=0x02;
	}
	else if ((BLE_Data[13]==0x04)&&(BLE_Data[11]==0x01))  // ��ָ��Ϊ ����Ա Type_B ��ʱ
	{
		
		#ifdef  uart_start 
		
		SEGGER_RTT_printf(0,"\r\nDel_AD_TypeB_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);  //�жϱ��������Ƿ����1 �������
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //��չ���Ա A ��
		Erase_AD_TypeB_Sector();
		}
		Code_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //��չ���Ա A ��
		Erase_AD_TypeA_Sector();
		}
//		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //����û� A ��
//		Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //����û� B ��
//		
		TX_Buff[11] =0X01;
		
//		if(Del_AD_Type_B(BLE_Data[12]))  //�������ſ�
//			TX_Buff[11] =0X01;
//		else
//		TX_Buff[11]=0x02;
	}
	else if((BLE_Data[13]==0X04)&&(BLE_Data[11]==0X02))  //��ָ��Ϊ �û� Type_A ��ʱ
	{
		
		#ifdef  uart_start
		
		SEGGER_RTT_printf(0,"\r\nDel_US_TypeB_Card!\r\n");
		
		#endif
		
		Code_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);  //�жϱ��������Ƿ����1 �������
		
		if(Code_Num!=0)
		{
			
			Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //����û� B ��
			Erase_US_TypeB_Sector();
		
		}
		Code_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
		
		if(Code_Num!=0)
		{
		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //����û� A ��
		Erase_US_TypeA_Sector();
			
		}
		TX_Buff[11] =0X01;
		
//		if(Del_US_Type_B(BLE_Data[12]))  //�������ſ�
//			TX_Buff[11] =0X01;
//		else
//		TX_Buff[11]=0x02;
	}
	else if(BLE_Data[13]==0x01)			//���յ�ɾ��ָ��ָ��ʱ����ʼɾ��ָ��(������һ���������ֹ���Ա���û�)
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"\r\n Del_PS_Print!\r\n	");
		#endif
		if(Del_A_ID_PS_Print(BLE_Data[12])==1)
		{
					TX_Buff[11] =0X01;   //ɾ���ɹ�
		}
		else
		{
			TX_Buff[11] = 0x02;		//��֮��ʧ��
		}
	}
	
			
	
	for (i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
	TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ


	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();	
	
	for(i =0;i<20;i++)
	BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�

	BLE_TX_Run(BLE_TX,20);
	for(i =0;i<20;i++)
	BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
	BLE_TX_Run(BLE_TX,20);	
	  
	// 	input_speak(9);
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();	
}

void Del_Mate_Run() //����豸
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Del_Mate_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = 0X30;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
	TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
	TX_Buff[12] = 0; //��ʾ������Ч
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //�޹���
	TX_Buff[17] = 0;  //�޹���
	it = 0; it1 =0;
	
	
	Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	
//	if(BLE_Data[13]==0x02) //��ָ��Ϊɾ������ָ��ʱ
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
	//��ʼ����豸
	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
	TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ

//	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
//	Spi_Flash_Sleep();	

	for(i =0;i<20;i++)
	BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�

	BLE_TX_Run(BLE_TX,20);
	for(i =0;i<20;i++)
	BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
	BLE_TX_Run(BLE_TX,20);	

	  
	// 	input_speak(9);
	//��ʼ��ʼ��
 	SPIFlash_Erase_Sector(0,0);  //Ԥ�����һ������0 ���������״̬�����Ϣ
	CL_FlashWord_Num();  //���Flash Ŀ¼��Ϣ(�����������豸�������Ŀ¼��Ϣ)
	CL_Flash_PassWord(); //�����豸ʱ ���Flash ������Ϣ only Clean
	CL_Flash_OPEN_DOOR_NOTE();  //�����豸ʱ ���Flash ���ż�¼��Ϣ
	Erase_AD_TypeA_Sector();
	Erase_US_TypeA_Sector();
	Erase_AD_TypeB_Sector();
	Erase_US_TypeB_Sector();
	
	
	SPI_Flash_Busy();		
	Devic_static_read();
	
	
	
	input_speak(4);
	ref_BLE_Adv();  //���ú�ˢ��BLE�㲥����
	
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();	
}


const uint8_t R_ODoor_note_ACK_Len = 40;   //Ӧ���ż�¼���ݰ�����


uint16_t TX_Door_X;  //���ż�¼����
//uint32_t Note_Num_1 =0;
uint32_t Note_Out_Num =0;
static uint32_t Note_Num,Note_Num1;
void Read_OP_Door_note()  //��ȡ���ż�¼ָ�� ����ΪӦ�𱾵ؿ��ż�¼������
{
	
//	uint8_t PassWord[18];

	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;

	
//	uint16_t Note_R_Num;  //��ȡ����(�Ӻ���ǰ)
//	uint8_t key_Num;  //���ű��
//	uint8_t Key_Type;  //���ŷ�ʽ
//	uint32_t Unix_t;
	//
	Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	
	Note_Num = Get_Cat_Dat(OpenDoorNote_Num_Code); //��ȡ���ż�¼����
	
	#ifdef uart_start
	
	SEGGER_RTT_printf (0,"Read_Op_Note_Run!\r\n");
	SEGGER_RTT_printf (0,"Read_Op_Note_Num: %d!\r\n",Note_Num);
	
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
		TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
		TX_Buff[1] = BLE_DAT_H2;
		
		TX_Buff[2] = 0x00;
		TX_Buff[3] = 0x18;  //����
		
		TX_Buff[4] = Read_OP_Door_note_Command;  //ָ����
		TX_Buff[5] = 0x02;  //����ʶ
		TX_Buff[6] = BLE_Data[6]; //���
		
		it1 = id1;			//�豸��ʶ
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
		
		Note_Num1 = Note_Num;		//���ڼ����λ
		Note_Num1>>=8;
		
		TX_Buff[11] = Note_Num1&0xFF;  //���ż�¼����  Ӧ���ż�¼����
		TX_Buff[12] = Note_Num&0XFF; //���ż�¼����
		TX_Buff[13] = 0x00; //��ǰ�ڼ���
		TX_Buff[14] = 0x00; //��ǰ�ڼ���
		TX_Buff[15] = 0; //����Ȩ�ޱ��
		TX_Buff[16] = 0;  //����Ȩ������
		TX_Buff[17] = 0x00;  //����ʱ���
		TX_Buff[18] = 0x00;  //����ʱ���
		TX_Buff[19] = 0x00;  //����ʱ���
		TX_Buff[20] = 0x00;  //����ʱ���
		
		TX_Buff[21] = 0; //�޹���
		TX_Buff[22] = 0; //�޹���
		TX_Buff[23] = 0; //�޹���
		TX_Buff[24] = 0; //�޹���
		TX_Buff[25] = 0; //�޹���
		
		it = 0; it1 =0;
		//��ʼ����豸
		for(i =0;i<(R_ODoor_note_ACK_Len-4);i++)  //У����������Чֵ���
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
		TX_Buff[39] = BLE_DAT_E2;  //������������ڻ�Ӧ

		for(i =0;i<20;i++)
		BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�

		BLE_TX_Run(BLE_TX,20);
	//	nrf_delay_ms(100);
		for(i =0;i<20;i++)
		BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
		BLE_TX_Run(BLE_TX,20);	 
		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep();
		
}
	//	


extern uint8_t Read_Op_Tab[];

void TX_Door_note()  //��ȡ��Ӧ��ŵı��ؿ��ż�¼  0x42ָ��
{
//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
//	uint16_t Note_Num;
	
//	uint16_t Note_R_Num;  //��ȡ����(�Ӻ���ǰ)
	uint16_t key_Num;  //���ż�¼���
		
	//	Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	
		#ifdef uart_start
		SEGGER_RTT_printf (0,"TX_Door_Note_Run!\r\n");
		
		#endif
		
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;
		

		//	Read_Op_Note(&key_Num,&Key_Type,&Unix_t,Note_Num_1);
			
			TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
			TX_Buff[1] = BLE_DAT_H2;
			
			TX_Buff[2] = 0x00;
			TX_Buff[3] = 0x22;  //����
			
			TX_Buff[4] = Read_A_OP_Door_note_Command;  //ָ����
			TX_Buff[5] = 0x02;  //����ʶ
			TX_Buff[6] = BLE_Data[6]; //���
			
			it1 = id1;			//�豸��ʶ
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
			
			for(i =0;i<Open_Note_Num_Len;i++)			//����ջ���
				Read_Op_Tab[i] =0;
			
			//�����ݰ��л�ȡҪ��ȡ�Ŀ��ż�¼���
			key_Num = BLE_Data[11];
			
			key_Num*=10;
			
			key_Num += BLE_Data[12];    //���뿪�ż�¼���
			
			//��ȡ��Ӧ��ŵĿ��ż�¼
			
			if(Read_Op_Note(key_Num)==1)			//����ȡ����������Чʱ����������
			{
				
				for(i =1;i<Open_Note_Num_Len;i++)				//Open_Note_Num_Len = sizeof(Read_Op_Tab)
				{
					
					TX_Buff[i+10] = Read_Op_Tab[i];					//������Чʱ���ض�Ӧ����  ��Ϊ��1 ��ʼ��¼ ����ֻ+9
					
				}
				
			}
			else
			{
				for(i =1;i<Open_Note_Num_Len;i++)				//Open_Note_Num_Len = sizeof(Read_Op_Tab)
				{
					
					TX_Buff[i+10] = Read_Op_Tab[i];			//��������Чʱ����0 ��Ϊ��1 ��ʼ��¼ ����ֻ+9
				
				}
				
			}
			
			it = 0; it1 =0; 
			
			for(i =0;i<(R_ODoor_note_ACK_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[39] = BLE_DAT_E2;  //������������ڻ�Ӧ
			#ifdef uart_start
			SEGGER_RTT_printf (0,"TX_Door_Note_Run1!\r\n");
			#endif
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
			
		//	nrf_delay_ms(50);
			
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
		//	nrf_delay_ms(50);
			
			
				#ifdef uart_start
				
				for(i =0;i<40;i++)
					SEGGER_RTT_printf(0,"%x,",TX_Buff[i]);
				
				SEGGER_RTT_printf(0,"\r\n");
				
				#endif
		//	NRF_WDT_Feed();			//���ﲻ��ι��
			
//			Note_Out_Num++;
//			if((Note_Num_1==1)||(Note_Out_Num>50)||(BLE_LINK==false))
//			{	
//				TX_Door_X =0;
//				Note_Num_1 =0;
//				
//			//	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
//			//	Spi_Flash_Sleep();
//				#ifdef uart_start
//				SEGGER_RTT_printf(0,"TX_Door_Note_STOP!\r\n");
//				#endif
//			}
//			if(Note_Num_1>1)
//			Note_Num_1--;
			
//			if(Note_Out_Num>50)  //�ﵽ�ϴ����ƺ� �����ϴ�
//			{
//				//Note_Out_Num =0;
//				Note_Num_1 =0; 
//				TX_Door_X =0;
//			//	return 0;
//			}
//			
//			TX_Door_X =0;

		//	Note_Num_1 = 0XFFFF;//����
		//		return 0;
			//nrf_delay_ms(20);
		//	return 1;
				//return 0;
	//	
//	Note_Out_Num =0; //�ϴ���������
//	return 0;
}



/*****************************************

CL_Door_note

��ձ��ؿ��ż�¼ָ��

*****************************************/

void CL_Door_note(void)
{
	//	uint8_t PassWord[18];
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
//	uint16_t Note_Num;
	
//	uint16_t Note_R_Num;  //��ȡ����(�Ӻ���ǰ)
//	uint16_t key_Num;  //���ż�¼���
		
	//	Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	
		#ifdef uart_start
		SEGGER_RTT_printf (0,"TX_Door_Note_Run!\r\n");
		
		#endif
		
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;
		

		//	Read_Op_Note(&key_Num,&Key_Type,&Unix_t,Note_Num_1);
			
			TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
			TX_Buff[1] = BLE_DAT_H2;
			
			TX_Buff[2] = 0x00;
			TX_Buff[3] = 0x10;  //����
			
			TX_Buff[4] = CL_OP_Door_note_Command;  //ָ����
			TX_Buff[5] = 0x02;  //����ʶ
			TX_Buff[6] = BLE_Data[6]; //���
			
			it1 = id1;			//�豸��ʶ
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
			
			Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	
			
			Ev_Cat_Dat(OpenDoorNote_Num_Code,0);  //��տ��ż�¼����
			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
		
			
			TX_Buff[11] =1;		//����1 ���سɹ�
			TX_Buff[12] =0;		//����2 ����
			TX_Buff[13] =0;		//����3 ����
			TX_Buff[14] =0;		//����4 ����
			TX_Buff[15] =0;		//����5 ����
			TX_Buff[16] =0;		//����6 ����
			TX_Buff[17] =0;		//����7 ����
			
			it = 0; it1 =0; 
			
			for(i =0;i<(R_ODoor_note_ACK_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);
			
		//	nrf_delay_ms(50);
			
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
			BLE_TX_Run(BLE_TX,20);	
			
	
	
}


/*****************************************
�û�������������
(������ˢ��ʵ�ַ���)

*******************************************/

uint8_t Start_Manual_Add_Card =0;

uint32_t Manual_Add_Card_Start_Unix; //�ֶ����뿨����ʼʱ��
uint32_t Manual_Add_Card_Stop_Unix; //�ֶ����뿨�Ľ���ʱ��

void Manual_Add_Card()  //�ֶ�����
{
		
//		uint8_t PassWord[18];
		uint32_t it1;
//		uint64_t Input_Time;
	//	uint8_t TX_Buff[100];  //����������
//		uint8_t BLE_TX[20]; //һ�η��͵�����
//		uint32_t err_code;
		
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Aanual_Add_Card_Run!\r\n");
		#endif
		Start_Manual_Add_Card =0;  //��� ��־λ
	
//		for(i =0;i<sizeof(TX_Buff);i++)
//		TX_Buff[i] =0;

//		TX_Buff[0] = 0X5E;  //׼��Ӧ������
//		TX_Buff[1] = 0xDC;
//		
//		TX_Buff[2] = 0x00;
//		TX_Buff[3] = 0x10;  //����
//		
//		TX_Buff[4] = 0X10;  //ָ����
//		TX_Buff[5] = 0x02;  //����ʶ (Ӧ��02 ����01)
//		TX_Buff[6] = BLE_Data[6]; //���
		
		it1 = id1;			//�豸��ʶ
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
		
		if(BLE_Data[11]==0x01) //������Ϊ����Աʱ
		{
			
				if(BLE_Data[12]==0x03) //��ʾ����Ա Type A ��
				{
					
					Start_Manual_Add_Card = 1;
				
				}
				else
				if(BLE_Data[12]==0x04)  //��ʾ����Ա Type B ��
				{
					
					Start_Manual_Add_Card =2;
				
				}
				else
				{
					
					Start_Manual_Add_Card =0;
				
				}
		}
		else if(BLE_Data[11]==0x02) //������Ϊ���ʱ
		{
					if(BLE_Data[12]==0x03) //��ʾ����Ա Type A ��
					{
						
						Start_Manual_Add_Card = 3;
						
					}
					else
					if(BLE_Data[12]==0x04)  //��ʾ����Ա Type B ��
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
		
		Manual_Add_Card_Start_Unix|= BLE_Data[16];   //������������ʱЧ����ʼʱ��
		
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[17];
		
		Manual_Add_Card_Stop_Unix <<=8;
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[18];
		
		Manual_Add_Card_Stop_Unix <<=8;
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[19];
		
		Manual_Add_Card_Stop_Unix <<=8;
		
		Manual_Add_Card_Stop_Unix |= BLE_Data[20];   	//������������ʱЧ�Ľ���ʱ��
		
				Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
		
		
		//ͨ��ID���󣬿�����ʱ����ѯ�ж�
		
	//	if(Start_Manual_Add_Card>0) //��ֵ����λʱ
	//		app_timer_start(Timer_1,APP_TIMER_TICKS(Timer_1,APP_TIMER_PRESCALER),NULL);			//����500ms����Ķ�ʱ�����ʱ������
	//			
	//		for(i =0;i<sizeof(PassWord);i++)PassWord[i] =0x00; //��ջ�����
	//		
	//		if((BLE_Data[11]==0x01)&&(BLE_Data[12]==0x02)) //����Ա���� (��һ�ֽ�01��ʾ����Ա���ڶ��ֽ�02��ʾ����)
	//		{
	//			PassWord[0] = 0x12; //��ʾ����Ա���� //�����ֽ�����
	//			for(i =1;i<18;i++)
	//			{
	//				PassWord[i] = BLE_Data[12+i];
	//			} //����������
	//	
	//			Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	//		
	//			TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
	//			TX_Buff[12] = Add_Flash_PassWord(PassWord); //��ʾ������Ч
	//			NRF_WDT_Feed();	
	//			if(TX_Buff[12]==0) //��д�뷵�ش���
	//			{
	//				TX_Buff[11] =0x02;
	//			}
	//		}
	//		else if((BLE_Data[11]==0x02)&&(BLE_Data[12]==0x02))
	//		{
	//			PassWord[0] = 0x22; //��ʾ�û����� //�����ֽ�����
	//			for(i =1;i<18;i++)
	//			{
	//				PassWord[i] = BLE_Data[12+i];
	//			} //����������
	//	
	//			Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
	//			
	//			TX_Buff[11] = 0X01;  //��ʾ�����ɹ�
	//			TX_Buff[12] = Add_Flash_PassWord(PassWord); //��ʾ������Ч
	//			NRF_WDT_Feed(4);
	//			if(TX_Buff[12]==0) //��д�뷵�ش�����ʾ����ʧ��
	//			{
	//				TX_Buff[11] =0x02;
	//			}
	//		}
	//		else
	//		{	//����ʧ�� //δ��չ����д�빦��
	//			TX_Buff[11] = 0x02; //��ʾ����ʧ��
	//			#ifdef uart_start
	//			
	//			SEGGER_RTT_printf(0,"Add_Information_Full!\r\n");  //������Ϣ�������
	//			
	//			#endif
	//		}
	//	
	//	
	//	
	////����ȡ����У����
	//	for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
	//		TX_Buff[21] = 0x2b;  //������������ڻ�Ӧ
	//		

	//		
	//			for(i =0;i<20;i++)
	//			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
	//			BLE_TX_Run(BLE_TX,20);
	//			for(i =0;i<20;i++)
	//			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
	//			BLE_TX_Run(BLE_TX,20);
	//		
	////			NRF_WDT_Feed();  //��֤��
	////			read_Soctor(0);
	////			NRF_WDT_Feed();
	////			read_Soctor(1);
	////			NRF_WDT_Feed();
	////			read_Soctor(2);
	////			NRF_WDT_Feed();
	//			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	//			Spi_Flash_Sleep();
}

/*****************************************

���ڻ�Ӧ ��������ָ��

���룺 Ack_Now

0x01 �ɹ�
0x02 ʧ��

���룺 Card_ID 

��������� SN ��

���룺Card_Type

�����͡�
0X01 ����Ա A ��
0X02 �û� A ��
0X03 ����Ա B ��
0X04 �û� B ��


*****************************************/

#define Manual_Add_Card_Ack_Len 28

void Manual_Add_Card_ACK(uint8_t Ack_Now,uint16_t Card_ID,uint8_t Card_Type)  //�ֶ�������Ӧ����
{
		
//		uint8_t PassWord[50];
		uint32_t i,it,it1;
//		uint64_t Input_Time;
		uint8_t TX_Buff[100];  //����������
		uint8_t BLE_TX[20]; //һ�η��͵�����
//		uint32_t err_code;
		
		#ifdef uart_start
		SEGGER_RTT_printf (0,"Aanual_Add_Card_Run!\r\n");
		#endif
		Start_Manual_Add_Card =0;  //��� ��־λ
		for(i =0;i<sizeof(TX_Buff);i++)
		TX_Buff[i] =0;

		TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
		TX_Buff[1] = BLE_DAT_H2;
		
		TX_Buff[2] = 0x00;
		TX_Buff[3] = 0x16;  //����
		
		TX_Buff[4] = 0X10;  //ָ����
		TX_Buff[5] = 0x02;  //����ʶ (Ӧ��02 ����01)
		TX_Buff[6] = BLE_Data[6]; //���
		
		it1 = id1;			//�豸��ʶ
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
		
		TX_Buff[11] = Ack_Now;  //���뷵��״̬
		TX_Buff[12] = 0;					//ԭ���� ID ʧЧ
		
		TX_Buff[13] = Card_ID>>8&0xFF;		//����ID�Ÿ�λ
		TX_Buff[14] = Card_ID&0XFF;				//����ID�ŵ�λ
		
		if(Type_AB_UID[0]==0X01)	//��ƬΪA��ʱ
			TX_Buff[15] = 4;
		else if(Type_AB_UID[0]==0X02)
			TX_Buff[15] = 8;
		
		TX_Buff[16] = Type_AB_UID[1];		// ����Կ������ ��һλ
		TX_Buff[17] = Type_AB_UID[2];		// ����Կ������ �ڶ�λ
		TX_Buff[18] = Type_AB_UID[3];		// ����Կ������ ����λ
		TX_Buff[19] = Type_AB_UID[4];		// ����Կ������ ����λ
		TX_Buff[20] = Type_AB_UID[5];		// ����Կ������ ����λ
		TX_Buff[21] = Type_AB_UID[6];		// ����Կ������ ����λ
		TX_Buff[22] = Type_AB_UID[7];		// ����Կ������ ����λ
		TX_Buff[23] = Type_AB_UID[8];		// ����Կ������ �ڰ�λ
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Ack_Manual_Add_Card:%d\r\n",Start_Manual_Add_Card);
						if(Ack_Now==0x01)
							SEGGER_RTT_printf(0,"Card_ID: %d\r\n",Card_ID);
		#endif
		
	//����ȡ����У����
	
		for(i =0;i<(Manual_Add_Card_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[28] = BLE_DAT_E2;  //������������ڻ�Ӧ
			

			
				for(i =0;i<20;i++)
				BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
				BLE_TX_Run(BLE_TX,20);
				for(i =0;i<20;i++)
				BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�
				BLE_TX_Run(BLE_TX,20);
	//		
	////			NRF_WDT_Feed();  //��֤��
	////			read_Soctor(0);
	////			NRF_WDT_Feed();
	////			read_Soctor(1);
	////			NRF_WDT_Feed();
	////			read_Soctor(2);
	////			NRF_WDT_Feed();
	//			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	//			Spi_Flash_Sleep();
	
	
	nrf_delay_ms(1500);  //�����ӳ� Ԥ������ʱ��

}

void Normally_Open_Door_Run()
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Normally_Open_Door_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = 0X05;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
	TX_Buff[11] = 0X01;  //��ʾ���ųɹ�
	
	TX_Buff[12] = 0; //��ʾ������Ч
	TX_Buff[13] = 0;
	TX_Buff[14] = 0;
	TX_Buff[15] = 0;
	
	TX_Buff[16] = 0;  //�޹���
	TX_Buff[17] = 0;  //�޹���
	
	it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�		
		
			BLE_TX_Run(BLE_TX,20);		
			
			
			Lock_Mot_Init();  //�������ģ��
			MOT_A_H;
			
			MOT_B_L;
			
			nrf_delay_ms(400);
			
	//	Lock_State =1;
			input_speak(6);
			Lock_Mot_Default();
			
			nrf_delay_ms(900);  
			input_speak(9);  //�����ɹ�
			
}


////���� ���������������ݸ���
//void TSM12MC_Sen_Code_update_Run()
//{
//	
//	uint32_t i,it,it1;
//	uint8_t TX_Buff[100];  //����������
//	uint8_t BLE_TX[20]; //һ�η��͵�����
////	uint32_t err_code;
//	
//	#ifdef uart_start
//	SEGGER_RTT_printf (0,"TSM12MC_Sen_Code_update_Run!\r\n");
//	#endif
//	
//		for(i =0;i<sizeof(TX_Buff);i++)
//	TX_Buff[i] =0;
//	
//	TX_Buff[0] = 0X5E;  //׼��Ӧ������
//	TX_Buff[1] = 0xDC;
//	
//	TX_Buff[2] = 0x00;
//	TX_Buff[3] = 0x10;  //����
//	
//	TX_Buff[4] = 0X05;  //ָ����
//	TX_Buff[5] = 0x02;  //����ʶ
//	TX_Buff[6] = BLE_Data[6]; //���
//	
//	
//	it1 = id1;			//�豸��ʶ
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
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Normally_Open_Door_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = More_Act_PassWord_Mode;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
		Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
		
		if(BLE_Data[11]==1)    //��������������� ��̬����ģʽ
		{
			//���óɶ��鶯̬����ģʽ
			
			Ev_Cat_Dat(More_Act_PassWord_Num_Code,0xFFFF0000);  
			//�Ը�λ��Ϊ ��׼ ����λΪ0XFFFFʱΪ ���鶯̬����״̬ 
			
		}
		else if(BLE_Data[11]==0)
		{
			//���óɵ���ʱЧ��̬����ģʽ
			Ev_Cat_Dat(More_Act_PassWord_Num_Code,0);  
			//�Ը�λ��Ϊ ��׼ ����λΪ0XFFFFʱΪ ���鶯̬����״̬ 
			
		}
		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep();
		
	
		TX_Buff[11] = 0X01;  //��ʾ���óɹ�
		
		TX_Buff[12] = 0; //��ʾ������Ч
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //�޹���
		TX_Buff[17] = 0;  //�޹���
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�		
		
			BLE_TX_Run(BLE_TX,20);		
	
			input_speak(9);   //��ʾ�����ɹ�
}


void Del_A_More_Act_PassWord_Run()    //���� ɾ���������鶯̬����ָ��
{
	uint32_t i,it,it1;
//	uint64_t Input_Time;
	uint8_t TX_Buff[100];  //����������
	uint8_t BLE_TX[20]; //һ�η��͵�����
//	uint32_t err_code;
	
	uint8_t PassWord_Num[15];		//Ŀ�궯̬���뻺��
	
	#ifdef uart_start
	SEGGER_RTT_printf (0,"Normally_Open_Door_Run!\r\n");
	#endif
	
	for(i =0;i<sizeof(TX_Buff);i++)
	TX_Buff[i] =0;
	
	for(i =0;i<sizeof(PassWord_Num);i++)
		PassWord_Num[i] =0;
	
	TX_Buff[0] = BLE_DAT_H1;  //׼��Ӧ������
	TX_Buff[1] = BLE_DAT_H2;
	
	TX_Buff[2] = 0x00;
	TX_Buff[3] = 0x10;  //����
	
	TX_Buff[4] = Del_A_More_Act_PassWord;  //ָ����
	TX_Buff[5] = 0x02;  //����ʶ
	TX_Buff[6] = BLE_Data[6]; //���
	
	it1 = id1;			//�豸��ʶ
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
	
		Spi_Flash_Init();  //����SPI_FLASH ͨѶ	
		
		
		if(BLE_Data[11]<10)				//Ŀ�궯̬���볤���������9λ
		{
			PassWord_Num[0] = BLE_Data[11];
			
			for(i =0;i<BLE_Data[11];i++)
			{
				PassWord_Num[i+1] = BLE_Data[12+i];				//�������ݴ� BLE_Data[12]��ʼ
			}
			
			Dle_A_More_Act_PassWord_Verify(PassWord_Num);
			
			TX_Buff[11] = 0X01;  //��ʾɾ���ɹ�
		}
		else
			TX_Buff[11] = 0X00;  //��ʾɾ��ʧ��(����������Ч)
		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep();
		
	

		TX_Buff[12] = 0; //��ʾ������Ч
		TX_Buff[13] = 0;
		TX_Buff[14] = 0;
		TX_Buff[15] = 0;
		
		TX_Buff[16] = 0;  //�޹���
		TX_Buff[17] = 0;  //�޹���
		
		it = 0; it1 =0;
//		it1 = BLE_Data[data_len+2];
//		it1<<=8;
//		it2 = BLE_Data[data_len+3];
//		it1+=it2;

		for(i =0;i<(Time_Ref_Ack_Len-4);i++)  //У����������Чֵ���
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
			TX_Buff[21] = BLE_DAT_E2;  //������������ڻ�Ӧ
			
			
			
			for(i =0;i<20;i++)
			BLE_TX[i] =TX_Buff[i]; //��ֵ��һ�����ݰ�
		
			BLE_TX_Run(BLE_TX,20);
			for(i =0;i<20;i++)
			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�		
		
			BLE_TX_Run(BLE_TX,20);		
	
			input_speak(9);   //��ʾ�����ɹ�
}

