#include <F_print.h>
#include <Uart_Run.h>   // ����


// ָ��ͷ���ݽӿ�ͨ�����ݰ�
 
const uint8_t PS_Getlmage[] =  //����ָ����ڶ�ȡָ��ͼ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X01,0X00,0X05
};

const uint8_t Null_Getlmage[] =  //Ӧ��ָ�δ��ȡ��ָ��(��ָ�ư���)
{
	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x03,0x02,0x00,0x0C
};

const uint8_t Err_Getlmage[] =  //Ӧ��ָ�������̫�٣�ʧ�ܣ��������ΰ�ѹ����
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X07,0X00,0X11
};

const uint8_t Be_Getlmage[] = //Ӧ��ָ���ȡ��ָ�� ����ָ�ư��£�
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X00,0X00,0X0A
};

const uint8_t Get_Dat_Save_1[] = //�����������洢�ڻ����� 01
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X01,0X00,0X08
};

const uint8_t Get_Dat_Save_2[] = //�����������洢�ڻ����� 02
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X02,0X00,0X09
};

const uint8_t Get_Dat_Save_3[] = //�����������洢�ڻ����� 03
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X03,0X00,0X0A
};

const uint8_t Get_Dat_Save_4[] = // �����������洢�ڻ����� 04
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X04,0X00,0X0B
};

const uint8_t Get_Dat_Save_5[] = // �����������洢�ڻ����� 05
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X05,0X00,0X0C
};

const uint8_t Get_Dat_Save_6[] = // �����������洢�ڻ�����  06
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X06,0X00,0X0D
};

const uint8_t Merge_Lmage_Dat[] = //�ϲ� ��ȡ����ָ����������
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X05,0X00,0X09
};

const uint8_t Ps_Save_ID[] =	//����ָ�Ƶ�ָ��ID��
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X06,
	0X06,0X01,
	0X00,0X03,		//���浽  ID3  (���޸ģ���0��ʼ)
	0X00,0X11
};
const uint8_t Se_Lmage_Dat[] =   //���������Ƿ��������ָ��
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X08,
	0X04,0X01,
	0x00,0x00,
	0x00,0x64,
	0x00,0x72
};

const uint8_t Not_Se_Lmage[] = 	//����δ������ƥ��ָ��
{
	
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,
	0X00,0X07,
	0X09,
	0X00,0X01,
	0X00,0X01,
	0X00,0X19
	
};

const uint8_t Se_Lmarge[] = //������ָ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,							//����ʶ
	0X00,0X07,				//������
	0X00,						//ȷ����
	0X00,0X00,		//ָ�� ID ��
	0X00,0X00,		//ƥ���
	0X00,0X00    //У����
};

const uint8_t PS_Del_A_Lmage[] =  //ɾ��һ��ָ��ָ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,			//����ʶ
	0X00,0X07,			//������
	0X0C,						//ָ��
	0X00,0X07,		//Ҫɾ����  ָ�� ID
	0X00,0X01,		
	0X00,0X1C			//У����
};

const uint8_t PS_Del_A_Lmage_ACK[] =  //����ָ��ɾ���ɹ�Ӧ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,				//����ʶ
	0X00,0X03,
	0X00,				//ȷ����
	0X00,0X0A		//У����
};

const uint8_t PS_Del_All_Lmage[] = // ɾ������ָ��ָ��
{
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X03,		//������
	0x0D,						//ָ����
	0x00,0x11				//У����
};

const uint8_t PS_Del_Lmage_ACK[] = //ɾ������ָ����� Ӧ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0x07,
	0x00,0x03,
	0x00,
	0x00,0x0A
};

const uint8_t PS_Read_Lmage_Num[] = //��ȡ�����ָ������
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X03,
	0X1D,			//ָ��
	0X00,0X21
};

const uint8_t PS_Lmage_Num_ACK[] = 	//Ӧ���ָ����
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,
	0X00,0X05,		//		���ݰ�����
	0X00,					//		ȷ����
	0X00,0X06,		//   ��ȡ����ָ������
	0X00,0X12			//	У����
};

const uint8_t PS_HandShake[]=  						//��ָ��ģ������
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X03,			//����
	0X35,						//ָ����
	0X00,0X39				//У����
};

const uint8_t PS_HandShake_ACK[]= 			//ָ��ģ������Ӧ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,						//����ʶ
	0X00,0X03,			//	����
	0x00,					//ȷ����
	0x00,0X0A			//	У����
};

const uint8_t PS_PowDlow[] = 					//ָ������ָ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,					//����ʶ
	0X00,0X03,		//������
	0X33,					//ָ����
	0X00,0X37			//У����
};

const uint8_t PS_PowDlow_ACK[] = 			//ָ������ָ��Ӧ��
{
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X07,									//����ʶ
	0X00,0X03,						//������
	0X00,									//ȷ����
	0X00,0X0A							// У����
};

const uint8_t PS_LED_CFG[] = 		// ָ��ͷ LED ����ָ��
{

	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,			//����ʶ
	0x00,0x07,			//������
	0x40,0x60,0x04,0x00,0x00,
	0x00,0xAC
	
};

const uint8_t PS_LED_POWER[] = 		//���� ָ��ͷ �� ��Դ
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X000,0X00,0X00,
	0X00,0XA8
	
};

const uint8_t  PS_LED_R1[]=    //ָ��ͷ LED ��ɫ ��һ��ָ��
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X06,0X80,0X00,
	0X01,0X2E 
	
};

const uint8_t  PS_LED_R2[]=    //ָ��ͷ LED ��ɫ �ڶ���ָ��
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X04,0X01,0X00,
	0X00,0XAD
	
};

const uint8_t PS_LED_G1[]=		//ָ��ͷ LED ��ɫ ��һ��ָ��
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X07,0X80,0X00,
	0X01,0X2F
	
};

const uint8_t PS_LED_G2[]=		//ָ��ͷ LED ��ɫ �ڶ���ָ��
{
	
	0XEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X04,0X04,0X00,
	0X00,0XB0
	
};

const uint8_t PS_LED_B1[]=			//ָ��ͷ LED ��ɫ ��һ��ָ��
{
	
	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,0x40,0x60,0x08,0x80,0x00,
	0x01,0x30
	
};

const uint8_t PS_LED_B2[]=			//ָ��ͷ LED ��ɫ �ڶ���ָ��
{
	
	0xEF,01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,
	0x40,0x60,0x04,0x10,0x00,
	0x00,0xBC
	
};

const uint8_t PS_LED_W1[]=     //ָ��ͷ LED ��ɫ  ��һ��ָ��
{
	
	0xEF,0X01,0XFF,0XFF,0XFF,0XFF,
	0X01,
	0X00,0X07,
	0X40,0X60,0X09,0X80,0X00,
	0X01,0X31
	
};

const uint8_t PS_LED_W2[]=			//ָ��ͷ LED ��ɫ  �ڶ���ָ��
{

	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,
	0x40,0x60,0x04,0x40,0x00,
	0x00,0xEC

};

const uint8_t PS_LED_Sleep[] = 		//ʹLED ����оƬ����  �͹���ģʽ
{

	0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
	0x01,
	0x00,0x07,
	0x40,0x60,0x00,0x08,0x00,
	0x00,0xB0 

};



#define Controllable_Power  28

uint8_t F_Print_Run_Flag =0;			//ָ��ͷ���б�־λ

uint8_t Controllable_Power_State =0;

void Controllable_Power_No() //����ָ��MCU��Դ
{
	
	nrf_delay_ms(100);
	nrf_gpio_cfg_output(Controllable_Power);
	nrf_gpio_pin_clear(Controllable_Power);
	Controllable_Power_State =1;
	
	if(Test_Ps_Mark==0)	
		PS_LED_Run(3);    //�ϵ�������
	
	
}


//�ر�ָ��MCU��Դ



void Controllable_Power_Off()
{

	nrf_gpio_pin_set(Controllable_Power);
	Controllable_Power_State =0;
  		F_Print_Run_Flag =0;
		Test_Ps_Mark =0;
	
}



//��ָ����ץȡָ�Ʒ��ص����ݰ�

#define Cheak_Initio_Len   6			//У������ͷ����

void PS_Data_Cheak(uint8_t *s,uint8_t len)			
{
	
	uint8_t i;
	uint8_t tab[50];
	uint8_t *s1;
	
	uint16_t Sum_Cheak,Sum_Cheak1;			//	У���
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart PS_Ddata_Cheak!\r\n");
	
	SEGGER_RTT_printf(0,"len: %x\r\n",len);
	
	#endif
	
	Sum_Cheak =0;
	
	for(i =0;i<(len-Cheak_Initio_Len);i++)
	{
		s1 =s;					//���
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
			
			i = len;			//����У�鿪ͷѭ�� ��ʼ��һ���Ա�
			
			break;			//	У��ɹ�����ѭ��
		}
		
		if(i>(len-Cheak_Initio_Len-1))
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"i: %d!\r\n",i);		//	���i ��ֵ
			
			SEGGER_RTT_printf(0,"Ps_Data_Cheak_Error!\r\n");			//	��ʾ���ݰ�У�����
			
			#endif
			
			return;				//	У�����ʱ����������
			
			
		}
		
	}
	
	//������������
			tab[6] = *s1;
			s1++;
			
			tab[7] = *s1;
			s1++;
			
			tab[8] = *s1;			//���ݰ�����
			s1++;
			
			
	
			if((tab[7]==0)&&(tab[8]<(sizeof(tab)-Cheak_Initio_Len-3)))		//�׵����ֽڣ������ֽڳ���
			{
				
				for(i =0;i<tab[8];i++)			//��ʣ�µ��������뵽����
				{
					tab[9+i] = *s1;
					s1++;
				}
				//���������ݶ����浽����ʱ
				
				Sum_Cheak =0;
				
				for(i =0;i<(tab[8]+1);i++)
				{
					
					Sum_Cheak +=tab[6+i];
					
				}
				
				Sum_Cheak1 = tab[Cheak_Initio_Len+1+tab[8]];
				Sum_Cheak1<<=8;
				Sum_Cheak1+=tab[Cheak_Initio_Len+2+tab[8]];			//��У��������
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Sum_Cheak1: %x. \r\n",Sum_Cheak1);
				
				#endif
				
				if(Sum_Cheak==Sum_Cheak1)     // ��У��ͺ���ʱ
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Sum_Cheak_succeed!\r\n");			//	У��ɹ�
					
					#endif
					
				}
				else
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Sum_Cheak_Error!\r\n");		//У��ʧ��
					
					#endif
					
					return;		//	У��ʧ��ʱ��������
				}
				
				//����ȡ����������ݰ�ʱ
						
				
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nPS_Data_Cheak:\r\n");			//�����ȡ����ָ�����ݰ�
				
				for(i =0;i<(Cheak_Initio_Len+3+tab[8]);i++)		//	�������
				{
					
					SEGGER_RTT_printf(0," %x ",tab[i]);
					
				}
				
				SEGGER_RTT_printf(0,"\r\n");
				
				#endif
			
				PS_Input_Order_Run(tab,(Cheak_Initio_Len+3+tab[8]));			//����ȡ�������ݰ�����ָ���ж�
		
			
			}
			else
			{
				
				#ifdef uart_start
				
		
				SEGGER_RTT_printf(0,"PS_Data_Len_Error!\r\n");			//�����쳣�����������ж�
				
				#endif
				
				return;
				
			}
		
}

/*
ָ��ͷ��������ָ���ж�
���룺 *s  ���ݰ�ָ��
���룺 len	���ݰ�����
*/

uint8_t Ps_Data_Tab[50];		//ָ��ͷ�������ݻ���

void PS_Input_Order_Run(uint8_t *s,uint8_t len)
{
	uint8_t tab[50];
	uint8_t i;
	
	#ifdef uart_start

	SEGGER_RTT_printf(0,"\r\nStart_PS_Input_Order_Run!\r\n"); //��ʼ����Ӧ�����ݰ�ָ���ж�
	
	#endif
	
	for(i =0;i<len;i++)			//���ϼ�У��õ������ݰ����뱾�ػ���
	{
		tab[i] = *s;
		s++;
	}
	
	if(tab[6]==0x07)			//�жϰ���ʶ �Ƿ�ΪӦ��	
	{
		;
	}
	else
	{
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Tab_6_Error! Value: %x.\r\n",tab[6]);		//��ʾȷ�����쳣
		
		#endif
		return;
	}
	if((tab[9]==0)&&(Test_Ps_Mark==1))		//��ȷ����������ڲ���ָ��ͷ�Ƿ���ڵ�״̬��������������
	{
		
		F_print_Enable =1;
		Test_Ps_Mark =0;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nF_printf_Enable_1!\r\n");		//��⵽��ʹ�ܵ�һ��ָ��ģ��
		
		#endif
		
		Controllable_Power_Off();		//��⵽ָ��ģ�鼴�رյ�Դ
		
		return;
		
	}
	
	for(i =0;i<50;i++)
	{
		 Ps_Data_Tab[i] =0;		//��ջ���
	}
	
	for(i =0;i<len;i++)			//���ϼ�У��õ������ݰ����뱾�ػ���
	{
		Ps_Data_Tab[i] = tab[i];			//�����ݷŵ��ⲿ���棬����������������
	}
	
	
	switch(tab[9])			//�ж�Ӧ���ȷ����
	{
		
		case 0: 	//00H����ʾָ��ִ����ϻ� OK�� 
			break;
		
		case 1:		//01H����ʾ���ݰ����մ��� 
			break;
		
		case 2:		//02H����ʾ��������û����ָ�� 
			break;
		
		case 3:		//03H����ʾ¼��ָ��ͼ��ʧ�ܣ� 
			break;
		
		case 4:		//04H����ʾָ��ͼ��̫�ɡ�̫���������������� 
			break;
		
		case 5:		//05H����ʾָ��ͼ��̫ʪ��̫���������������� 
			break;
		
		case 6:		//06H����ʾָ��ͼ��̫�Ҷ������������� 
			break;
		
		case 7:		//07H����ʾָ��ͼ����������������̫�٣������̫С���������������� 
			break;
		
		case 8:		//08H����ʾָ�Ʋ�ƥ�䣻 
			break;
		
		case 9:		//09H����ʾû������ָ�ƣ� 
			break;
		
		case 10:	//0aH����ʾ�����ϲ�ʧ�ܣ� 
			break;
		
		case 11:  //0bH����ʾ����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ�� 
			break;
		
		case 12:	//0cH����ʾ��ָ�ƿ��ģ��������Ч�� 
			break;
		
		case 13:	//0dH����ʾ�ϴ�����ʧ�ܣ� 
			break;
		
		case 14:	//0eH����ʾģ�鲻�ܽ��պ������ݰ��� 
			break;
		
		case 15:	//0fH����ʾ�ϴ�ͼ��ʧ�ܣ�
			break;
		
		case 16:	//10H����ʾɾ��ģ��ʧ�ܣ� 
			break;
		
		case 17:	//11H����ʾ���ָ�ƿ�ʧ�ܣ� 
			break;
		
		case 18:	//12H����ʾ���ܽ���͹���״̬�� 
			break;
		
		case 19:	//13H����ʾ�����ȷ�� 
			break;
		
		case 20:	//14H����ʾϵͳ��λʧ�ܣ� 
			break;
		
		case 21:	//15H����ʾ��������û����Чԭʼͼ��������ͼ��
			break;
		
		case 22:	//16H����ʾ��������ʧ�ܣ�
			break;
		
		case 23:	//17H����ʾ����ָ�ƻ����βɼ�֮����ָû���ƶ����� 
			break;
		
		case 24:	//18H����ʾ��д FLASH ���� 
				break;
		
		case 0XF0:	//f0H���к������ݰ���ָ���ȷ���պ��� 0xf0 Ӧ�� 
				break;
		
		case 0XF1:  //f1H���к������ݰ���ָ�������� 0xf1 Ӧ�� 
				break;
		
		case 0XF2:	//f2H����ʾ��д�ڲ� FLASH ʱ��У��ʹ���  
				break;
		
		case 0XF3:	//f3H����ʾ��д�ڲ� FLASH ʱ������ʶ���� 
				break;
		
		case 0XF4:	//f4H����ʾ��д�ڲ� FLASH ʱ�������ȴ��� 
				break;
		
		case 0XF5:	//f5H����ʾ��д�ڲ� FLASH ʱ�����볤��̫���� 
				break;
		
		case 0XF6:	//f6H����ʾ��д�ڲ� FLASH ʱ����д FLASH ʧ�ܣ� 
				break;
		
		case 25:		//19H�����������ʧ�ܣ� 
				break;
		
		case 26:		//1aH����Ч�Ĵ����ţ� 
				break;
		
		case 27:		//1bH���Ĵ����趨���ݴ���ţ� 
				break;
		
		case 28:		//1cH�����±�ҳ��ָ������ 
				break;
		
		case 29:		//1dH���˿ڲ���ʧ�ܣ� 
				break;
		
		case 30:		//1eH���Զ�ע�ᣨenroll��ʧ�ܣ� 
				break;
		
		case 31:		//1fH��ָ�ƿ����� 
				break;
		
		case 32:		//20H���豸��ַ���� 
				break;
		
		case 33:		//21H���������� 
				break;
		
		case 34:		//22 H��ָ��ģ��ǿգ�
				break;
		
		case 35:		//23 H��ָ��ģ��Ϊ�գ� 
				break;
		
		case 36:		//24 H��ָ�ƿ�Ϊ�գ� 
				break;
		
		case 37:		//25 H��¼��������ô���
				break;
		
		case 38:		//26 H����ʱ��
				break;
		
		case 39:		//27 H��ָ���Ѵ��ڣ�
				break;
		
		case 40:		//28 H��ָ��ģ���й����� 
				break;
		
		case 41:		//29 H����������ʼ��ʧ�ܣ�
				break;
		
		
		
	}
	
	
}

//¼��ָ�ƺ���
/******************

���룺 Ps_Lmage		ָ������
���룺 Start_Unix	��ʼʱ��
���룺 Stop_Unix	����ʱ��


*******************/

uint8_t Input_Ps_Lmage_Type;
uint8_t Input_Ps_Source;
uint8_t Test_Ps_Mark =0;			//���ָ��ͷ�Ƿ����ʱ�ı�־

uint32_t Start_PS_Unix;
uint32_t Stop_PS_Unix;
uint32_t F_print_Enable =0;			//�ڳ�ʼ��ʱ����Ƿ����ָ��ͷ


#define Admin_Ps_Lmage	1
#define User_Ps_Lmage		2

void Input_PS_Lmage()
{
	
	if(F_print_Enable==0)		//��������ָ��ͷʱ�����ָ��¼��ָ������
	{
		
		Input_Ps_Lmage_Type =0;	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nF_print_Disable!\r\n");			//��ʾָ��ͷδ����
		
		#endif
		
	}
	

	
	if(Input_Ps_Lmage_Type!=0)			//����⵽ָ��¼��ָ��ʱ
	{
		
		switch(Input_Ps_Lmage_Type)
		{
			case Admin_Ps_Lmage:		//����Աָ��
				
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Input_Admin_Ps_Lmage!\r\n");		//¼�����Աָ��
			
			#endif
			
			if(Input_PS_Lmage_Run(Start_PS_Unix,Stop_PS_Unix,0)==0)
			{
				;
			}
			
				break;
			case User_Ps_Lmage:			//�û�ָ��
				
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Input_User_Ps_Lmage!\r\n");		//¼�����Աָ��	
			
			#endif
			
			
			if(Input_PS_Lmage_Run(Start_PS_Unix,Stop_PS_Unix,0)==0)
			{
				;
			}
			
			
				break;
			
			default :
				Input_Ps_Lmage_Type =0;			//������������
				Start_PS_Unix =0;
				Stop_PS_Unix =0;
			
				break;
			
		}
		
	}
}


/*************************************************************

uint8_t F_print_Goto_Sleep()

ʹָ��ͷ����˯��״̬

�ɹ� ����1 ʧ�� ����0

***************************************************************/

uint8_t F_Print_Goto_Sleep_Now =0;			//�������˯��״̬��״̬��
uint32_t FP_Goto_Sleep_T =0;					//����˯��ģʽ��Ҫ���ѵ�ʱ��
const uint32_t FP_Goto_Sleep_Out =50;				//���峬ʱ  
uint8_t PS_Goto_Sleep_Num =0;				//�������Դ���
uint8_t F_print_Goto_Sleep()
{
	uint8_t TX_BUFF[64];			//���ͻ���
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"F_Print_Goto_Sleep_Now!\r\n");
	
	#endif
	
	switch (F_Print_Goto_Sleep_Now)
	{
		case 0:
		
		//PS_LED_Shutdown();			//�ȹر� LED ģ��
		
		for (uint32_t i =0;i<sizeof(PS_PowDlow);i++)			//�ڷ��ͻ�����д��˯��ָ��
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
					(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)				//���յ������ݰ�Ϊ����˯��״̬ʱ�����ɹ�
				)					//����õ�����
			{
				
				F_Print_Goto_Sleep_Now =0;
				
				#ifdef uart_start
	
				SEGGER_RTT_printf(0,"F_Print_Goto_Sleep_OK!\r\n");
				
				#endif
				PS_Goto_Sleep_Num =0;			//��������
				return 1;
			}
			else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
			{
				F_Print_Goto_Sleep_Now =0;			//����ʧ��  ���·���
			}
			FP_Goto_Sleep_T++;
			
			if(FP_Goto_Sleep_T>FP_Goto_Sleep_Out)				//��ʱ���·���
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

����ָ��¼�빦��

���أ� �Ƿ�¼��ɹ� ʧ���򷵻�0���ɹ��򷵻�¼��ʱ��ȡ����ָ�� ID

���룺Lmage_Start_Unix ��ʼʱ��
			Lmage_Stop_Unix  ����ʱ��
			Ps_ID						 ����ָ�Ʒ����ID��


******************************************************************/

uint8_t Input_Ps_Lmage_Run_Now =0;
uint16_t Input_Ps_Lmage_ID;		//��ȡ������ָ�� ID
uint16_t input_ps_T =0;		//��ʱ��־
const uint16_t input_ps_T_Out =30;		//��ʱ������
	
uint8_t Input_PS_Lmage_Run(uint32_t Lmage_Start_Unix,uint32_t Lmage_Stop_Unix,uint32_t Ps_ID)
{
	
	uint8_t TX_BUFF[64];			//���ͻ���
	uint32_t i;		//ѭ��
	uint16_t ps_buff_chak;
//	uint16_t Sum_cheak;		//У�������
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Input_PS_now: %x!\r\n",Input_Ps_Lmage_Run_Now);
	
	#endif
	
	if((BLE_LINK==0)||(Lock_State==1))		//���������ӶϿ�ʱ������δ�ر�ʱָֹͣ��¼�����
	{
		
	//	input_speak(10);		//��ʾ����ʧ��
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_Input_Run_BLE_Disconnect!\r\n");
		
		#endif
		
		Input_Ps_Lmage_Run_Now =22;
		
	}
	
	
	switch(Input_Ps_Lmage_Run_Now)
	{
		case 0:
			
			Controllable_Power_No();	//��ָ��ͷ�ϵ�
			Input_Ps_Lmage_ID = Need_Of_Ps_Print_Data_IDLE_ID();
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Input_Ps_Lmage_ID: %x\r\n",Input_Ps_Lmage_ID);
		
				#endif
		
				if(Input_Ps_Lmage_ID<=PS_Print_Num)			//��ָ��ID ����ʱ �������
					Input_Ps_Lmage_Run_Now++;
				else
					Input_Ps_Lmage_Run_Now = 0xFF;
				
		break;
		
		case 1:		//���Ͷ�ȡָ�Ƶ�ָ��
			for(i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
			
			break;
		
		case 2:			//ȷ��ָ���Ƿ���
		
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ���ص���һ��
			)					//����õ�����
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage!\r\n");		//��ʾ��ָ̧��
			
			#endif
			
			Input_Ps_Lmage_Run_Now --;
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//���յ������ݰ�Ϊ��ָ����ʱ��������һ��
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage!\r\n\r\n");		//��ʾ��ָδ̧���ȡ��ָ��
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//�յ�������ʱ�Ƶ���һ��
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data!\r\n");		//��ʾ�������ĵ����
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
		input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;
		
			break;
		
		case 3:				//����¼��ĵ�һ��ָ��ָ�� ��ģ��ĵ�һ�黺��
			
				for(i =0;i<sizeof(Get_Dat_Save_1);i++)			//�ڷ��ͻ�����д�뱣��ָ��
				{
					
					TX_BUFF[i] = Get_Dat_Save_1[i];
					
				}
		
			Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_1));
			
			for(i =0;i<50;i++)
			{
				 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			}
			
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nTX_Gen_Dat_Save_1!\r\n");
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
			
			input_ps_T =0;
			
			break;
	
		case 4:		//�ȴ���һ�����ݵı���ȷ�ϰ�
				
				if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ�洢�ɹ���Ӧ��ʱ������һ��
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_OK!\r\n");

					#endif
				//	nrf_delay_ms(500);
					input_speak(32);	//��ʾ���ٰ�һ��ָ��

					nrf_delay_ms(1000);
					PS_LED_Run(3);    //�ϵ�������

					Input_Ps_Lmage_Run_Now++;
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_Error!\r\n");
					
					#endif
					    
					Input_Ps_Lmage_Run_Now =1;		//�������ݰ� ��ʼ����¼����һ��ָ������
				}
				
		input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;
		
		
		break;
			
		case 5:		//��ʼ׼��¼��ڶ���ָ�Ƶȴ���ָ�ſ�
				
		for(i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		Input_Ps_Lmage_Run_Now++;
		
		input_ps_T =0;

			break;
				
		case 6: //ȷ��ָ���Ƿ�ſ�
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ���ص���һ��
			)					//����õ�����
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage_1!\r\n");		//��ʾ��ָ̧��
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;		//��ָ̧��ʱ׼����һ��¼��ָ��
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ��ָ����ʱ��������һ��
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_1!\r\n\r\n");		//��ʾ��ָδ̧���ȡ��ָ��
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//�յ�������ʱ�Ƶ���һ��
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_1!\r\n");		//��ʾ�������ĵ�����˵���һ��
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
			input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;
		
			break;
		//��ɵ�һ��¼��
		case 7:		//��ʼ�ڶ���ָ��¼��
			
					for(i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		break;
		
		case 8:		//�ȴ�ָ�ư���
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ���ص���һ��
			)					//����õ�����
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage!_2\r\n");		//��ʾ��ָ̧��
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//���յ������ݰ�Ϊ��ָ����ʱ��������һ��
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_2!\r\n\r\n");		//��ʾ��ָδ̧���ȡ��ָ��
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//�յ�������ʱ�Ƶ���һ��
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_2!\r\n");		//��ʾ�������ĵ����
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
		input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;
		
		break;
		
		case 9:		//����¼���ָ�� ��ģ��ĵڶ��黺��
		
				for(i =0;i<sizeof(Get_Dat_Save_2);i++)			//�ڷ��ͻ�����д�뱣��ָ��
				{
					
					TX_BUFF[i] = Get_Dat_Save_2[i];
					
				}
		
			Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_2));
			
			for(i =0;i<50;i++)
			{
				 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			}
			
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nTX_Gen_Dat_Save_2!\r\n");
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
			input_ps_T =0;
		break;
		
		case 10:
			
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ�洢�ɹ���Ӧ��ʱ������һ��
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_2_OK!\r\n"); //�ڶ���ָ�Ʊ���ɹ�
					
					#endif
					//nrf_delay_ms(500);
					input_speak(32);	//��ʾ���ٰ�һ��ָ��
					nrf_delay_ms(1000);
						PS_LED_Run(3);    //�ϵ�������

					Input_Ps_Lmage_Run_Now++;
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_2_Error!\r\n");
					
					#endif
					
					Input_Ps_Lmage_Run_Now=7;		//�������ݰ� ��ʼ����¼����һ��ָ������
				}
			
		input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;

		
		break;
			
		case 11:	//�ȴ�ָ�Ʒſ�
		
					for(i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		break;
		
		case 12: //ȷ��ָ���Ƿ�ſ�
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ���ص���һ��
			)					//����õ�����
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage_3!\r\n");		//��ʾ��ָ̧��
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;		//��ָ̧��ʱ׼����һ��¼��ָ��
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ��ָ����ʱ��������һ��
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_3!\r\n\r\n");		//��ʾ��ָδ̧���ȡ��ָ��
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//�յ�������ʱ�Ƶ���һ��
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_3!\r\n");		//��ʾ�������ĵ����
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
			input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;

		break;
					
	//��ʼ������ָ��¼��
					
				case 13:		//��ʼ������ָ��¼��
			
					for(i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		break;
		
		case 14:		//�ȴ�ָ�ư���
		
					if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ���ص���һ��
			)					//����õ�����
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Null_Getlmage!_3\r\n");		//��ʾ��ָ̧��
			
			#endif
			
			Input_Ps_Lmage_Run_Now--;
			
			
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//���յ������ݰ�Ϊ��ָ����ʱ��������һ��
					)
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nPS_Be_Getlmage_3!\r\n\r\n");		//��ʾ��ָδ̧���ȡ��ָ��
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
		
		}
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//�յ�������ʱ�Ƶ���һ��
					)
					{
						
						#ifdef uart_start
			
						SEGGER_RTT_printf(0,"PS_Other_Getlmage_Data_3!\r\n");		//��ʾ�������ĵ����
						
						#endif
						
						Input_Ps_Lmage_Run_Now--;
					}
		
			input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;

		break;
					
					
		
		case 15:		//�������ݵ�������������
		
			for(i =0;i<sizeof(Get_Dat_Save_3);i++)			//�ڷ��ͻ�����д�뱣��ָ��
				{
					
					TX_BUFF[i] = Get_Dat_Save_3[i];
					
				}
		
			Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_3));
			
			for(i =0;i<50;i++)
			{
				 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			}
			
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nTX_Gen_Dat_Save_3!\r\n");
			
			#endif
			
			Input_Ps_Lmage_Run_Now++;
			input_ps_T =0;
		break;
	
		case 16:		// ȷ���������ݱ���
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ�洢�ɹ���Ӧ��ʱ������һ��
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
					
					Input_Ps_Lmage_Run_Now =13;		//�������ݰ� ����¼����һ��ָ������
				}
		
				
			input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;

		break;
				
		case 17:			//�ϲ���õ�ָ��
		
			for(i =0;i<sizeof(Merge_Lmage_Dat);i++)			//�ϲ�ָ��ָ��
			{
				
				TX_BUFF[i] = Merge_Lmage_Dat[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(Merge_Lmage_Dat));
		
		for(i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		
		}
		
		Input_Ps_Lmage_Run_Now++;
			
		input_ps_T =0;
		
		break;
		
		case 18:			//ȷ�Ϻϲ�ָ������
			
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ�洢�ɹ���Ӧ��ʱ������һ��
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
					
					Input_Ps_Lmage_Run_Now =0;		//�ϲ�ʧ�ܣ�����¼��ָ��
				}
	
			input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;

		break;
				
		case 19:		//��ָ�Ʊ��浽ָ��ͷ
					
			for(i =0;i<sizeof(Ps_Save_ID);i++)			//����ָ��ָ��
			{
				TX_BUFF[i] = Ps_Save_ID[i];
			}
			
			TX_BUFF[12] = Input_Ps_Lmage_ID;		//��ָ���з�����Ҫ�����ָ�� ID
			
			ps_buff_chak =0;		//����У��͵�ֵ
			
			for(i =0;i<7;i++)			//����У���
			{
				
				ps_buff_chak+=TX_BUFF[i+6];
				
			}
			
			TX_BUFF[13] = ps_buff_chak%0xFF00>>8;		//���뷢�͵�У���
			TX_BUFF[14] = ps_buff_chak%0xFF;
			
			Uart_Put_String(TX_BUFF,sizeof(Ps_Save_ID));
			
			Input_Ps_Lmage_Run_Now++;
			input_ps_T =0;
		break;
	
		case 20:
		
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ�洢�ɹ���Ӧ��ʱ������һ��
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
					
					Input_Ps_Lmage_Run_Now --;		//�������ݰ� ��ʼ¼����һ��ָ������
				}
	
			input_ps_T++;		//��¼����ʱ��
		
		if(input_ps_T>input_ps_T_Out)			//��ʱʱ������һ��
			Input_Ps_Lmage_Run_Now--;

		break;
	
		case 21:
			
			
			if(Add_Flash_PS_Print(Input_Ps_Lmage_Type,Input_Ps_Lmage_ID,Start_PS_Unix,Stop_PS_Unix)==1)
			{
				
				input_speak(9);		//��ʾ�����ɹ�
				EX_BLE_ACK_Run(1,Input_Ps_Lmage_ID);			//Ӧ��ɹ�
				Input_Ps_Lmage_Run_Now=22;
			}
			else
			{
				
				input_speak(10);
				EX_BLE_ACK_Run(2,0xFF);			//Ӧ��ʧ��,������ЧID
				Input_Ps_Lmage_Run_Now=22;
			}
			
			#ifdef uart_start
	    		
			SEGGER_RTT_printf(0,"Input_Ps_Lmage_ID: %x!\r\n",Input_Ps_Lmage_ID);
			
			#endif
			
			input_ps_T =0;

			
			break;
		
		case 22:
		
		if(F_print_Goto_Sleep()==0)				//�ȴ�����˯��״̬
			{
				
				input_ps_T++;
				if(input_ps_T>input_ps_T_Out)			//����ʱʱ
				{
		//			input_speak(10);		//��ʾ����ʧ��
					Input_Ps_Lmage_Run_Now =0;		//���״̬��
					Input_Ps_Lmage_Type =0;				//��ʼ��ָ��¼�����ֵ
					Start_PS_Unix =0;
					Stop_PS_Unix =0;				
			//		Controllable_Power_Off();
				F_Print_Run_Flag =1;
				}
			}
			else
			{
				Input_Ps_Lmage_Run_Now =0;		//���״̬��
				Input_Ps_Lmage_Type =0;				//��ʼ��ָ��¼�����ֵ
				Start_PS_Unix =0;
				Stop_PS_Unix =0;
//				Controllable_Power_Off();
				F_Print_Run_Flag =1;
			}
		break;
			
		default :
			
			if(F_print_Goto_Sleep()==0)				//�ȴ�����˯��״̬
			{
				input_ps_T++;
				if(input_ps_T>input_ps_T_Out)			//����ʱʱ
				{
	//				input_speak(10);		//��ʾ����ʧ��
					Input_Ps_Lmage_Run_Now =0;		//���״̬��
					Input_Ps_Lmage_Type =0;				//��ʼ��ָ��¼�����ֵ
					Start_PS_Unix =0;
					Stop_PS_Unix =0;				
					//Controllable_Power_Off();
					F_Print_Run_Flag =1;
				}
			}
			else
			{
				
		//		input_speak(10);		//��ʾ����ʧ��
				Input_Ps_Lmage_Run_Now =0;		//���״̬��
				Input_Ps_Lmage_Type =0;				//��ʼ��ָ��¼�����ֵ
				Start_PS_Unix =0;
				Stop_PS_Unix =0;
				
				//Controllable_Power_Off();
				
				F_Print_Run_Flag =1;
			}
			
			return 1;   //�����쳣״̬
		
//		break;
		
		
	}
	return 0;
	
}



uint8_t CL_Ps_Print_Now =0;
uint8_t CL_PS_Num =0;
uint8_t CL_Ps_Print()		//���ָ��ͷ����
{
	uint8_t TX_BUFF[64];			//���ͻ���
		
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Start_CL_Ps_Print!\r\n");
	
	#endif
	
	switch (CL_Ps_Print_Now)
	{
		case 0:
		for (uint32_t i =0;i<sizeof(PS_Del_All_Lmage);i++)			//�ڷ��ͻ�����д��˯��ָ��
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
					(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)				//���յ������ݰ�Ϊ����˯��״̬ʱ�����ɹ�
				)					//����õ�����
			{
				
				CL_Ps_Print_Now =0;
				
				#ifdef uart_start
	
				SEGGER_RTT_printf(0,"F_Print_Goto_Sleep_OK!\r\n");
				
				#endif
				CL_PS_Num =0;			//��ճɹ�����մ���
				return 1;
			}
			else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
			{
				CL_Ps_Print_Now =0;			//����ʧ��  ���·���
				CL_PS_Num++;			//ʧ�ܺ��1
			}
			CL_Ps_Print_Now++;		//����  goto sleep����ʱ
			
			if(FP_Goto_Sleep_T>FP_Goto_Sleep_Out)				//��ʱ���·���
			{
				FP_Goto_Sleep_T =0;
				CL_Ps_Print_Now =0;
				CL_PS_Num ++; //ʧ�ܺ������ 1
			}
			break;
		
	}
	
		if(CL_PS_Num>5)			//ʧ����κ󷵻�1 �����������
		{
			CL_PS_Num =0;
			return 1;
		}
	
	
	return 0;
	
}



//ָ��ͷ LED ���к����� ����; LED_CR  LED ��ɫ  0 �� 1 �� 2 ��  3 ��  4 �� 

void PS_LED_Run(uint8_t LED_CR)
{
	uint8_t TX_BUFF[64];			//���ͻ���
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Start_CL_Ps_Print!\r\n");
	
	#endif
	


		for (uint32_t i =0;i<sizeof(PS_LED_CFG);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
		{
			
			TX_BUFF[i] = PS_LED_CFG[i];
		
		}
		
		Uart_Put_String(TX_BUFF,sizeof(PS_LED_CFG));				
		
		nrf_delay_ms(5);
		
			//���õ�Դģʽ
			
			for (uint32_t i =0;i<sizeof(PS_LED_POWER);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
			{
				
				TX_BUFF[i] = PS_LED_POWER[i];
			
			}
			
			Uart_Put_String(TX_BUFF,sizeof(PS_LED_POWER));				
			
			nrf_delay_ms(5);
			
			if(LED_CR==1)		//��ɫ
			{
				
				for (uint32_t i =0;i<sizeof(PS_LED_R1);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_R1[i];
				}
				
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_R1));
				nrf_delay_ms(5);
				
				for (uint32_t i =0;i<sizeof(PS_LED_R2);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_R2[i];
				}
				
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_R2));
				
			}
			else if(LED_CR==2)	//��ɫ
			{
				
				for (uint32_t i =0;i<sizeof(PS_LED_G1);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_G1[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_G1));
				nrf_delay_ms(5);
				for (uint32_t i =0;i<sizeof(PS_LED_G2);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_G2[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_G2));

			}
			else if(LED_CR==3)  //��ɫ
			{
			for (uint32_t i =0;i<sizeof(PS_LED_B1);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_B1[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_B1));
				nrf_delay_ms(5);
				for (uint32_t i =0;i<sizeof(PS_LED_B2);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_B2[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_B2));
				

			}
			else if(LED_CR==4)   //��ɫ
			{
				for (uint32_t i =0;i<sizeof(PS_LED_W1);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_W1[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_W1));
				nrf_delay_ms(10);
				for (uint32_t i =0;i<sizeof(PS_LED_W2);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
				{
					TX_BUFF[i] = PS_LED_W2[i];
				}
				Uart_Put_String(TX_BUFF,sizeof(PS_LED_W2));
				
			}
			
			
			FP_Goto_Sleep_T =0;
			for(uint32_t i =0;i<sizeof(Ps_Data_Tab);i++)			//���ָ�ƽ��ջ���
			{
				
				Ps_Data_Tab[i] =0;
			
			}

}

void PS_LED_Shutdown()			//ָ��ͷ LED ģ����� �͹���ģʽ  (ֱ�ӹر�ָ��ģ��)
{
		uint8_t TX_BUFF[64];			//���ͻ���
	
		for (uint32_t i =0;i<sizeof(PS_LED_Sleep);i++)			//�ڷ��ͻ�����д��ָ��ͷ������ָ��
			{
				TX_BUFF[i] = PS_LED_Sleep[i];
			}
			Uart_Put_String(TX_BUFF,sizeof(PS_LED_Sleep));
			
			nrf_delay_ms(100);		//�ȴ� 100 ms
			
}



