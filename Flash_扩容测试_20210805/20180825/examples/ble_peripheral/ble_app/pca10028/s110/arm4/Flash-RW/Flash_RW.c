#include <Flash_RW.h>
#include <BLE_Data.h>



//��Ƭ�洢������Χ�� 440 ��Ϊֹ���µĹ���������445�ſ�ʼ

const uint16_t TSM12MC_Sen_Sector_Num =447;		//���屣�水�������ȵ�����

//Ŀ¼�� ��������

/*
��ע��  
Ŀ¼������������ͬʱʹ�ã����ǰ�����˳������д�뱣��ͬһ������ 
��ƽ��Flash��дĥ���Դﵽ��������۲�д����
д����һ������ʱ����Ҫʹ����������Ч��  ���������㣩
*/


const uint8_t Cat_Dat_Sector1 = 10;						//�趨Ŀ¼��������ʼ��ַ

const uint8_t Cat_Dat_Sector_Num =10;				// �趨 Ŀ¼ ���ĵ� ��������

const uint32_t Cat_Dat_len =256;				// �趨 Ŀ¼ ���ݵ��ܳ���



const uint8_t PassWord_Len = 18;  //�����������ݳ���

const uint8_t Open_Note_Num_Len = 26; //�������ż�¼����

const uint8_t PassWord_Secotr1 = 2;  //����������������

const uint8_t PassWord_Secotr2 = 3;  //����������������

const uint8_t PassWord_SN_Secotr = 4;  //�����Ŵ洢���� (ÿ���Զ�������ı��)

const uint8_t Act_PassWord_Secotr = 5;  //�洢��̬����

const uint8_t History_PassWord_Secotr = 6;          //��ʷ��̬����

const uint8_t Per_Act_PassWord_Secotr = 7;				//�������ö�̬����

const uint16_t More_Act_PassWord_Secotr = 445;				//���������̬�����Ŀ¼��Ŀǰ����һ��������

const uint16_t More_Act_PassWord_Len = 17;					//���������̬�������ݳ���

const uint8_t More_Act_PassWord_MAX_Num = 100;					//���������̬����������

const uint16_t AD_TypeA_Data_Secotr1 = 25;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint16_t AD_TypeB_Data_Secotr1 = 130;  //����B�����ݵ�����1 (Ŀǰ����1������)

const uint16_t US_TypeA_Data_Secotr1 = 235;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint16_t US_TypeB_Data_Secotr1 = 340;  //����B�����ݵ�����1 (Ŀǰ����1������)

// �Ϸ�ÿ������ͷ ��� 105 ������Ϊ�˱�֤���ݰ�ȫ����Ҫ�����������������100������

const uint8_t TypeA_Data_Len = 14;  	//һ��A�����ݵĳ���

const uint8_t TypeB_Data_Len = 18;		//һ��B�����ݵĳ���

const uint8_t A_Sector_TypeA_Data_Num = 100;			//һ����������� A �� ��������

const uint8_t A_Sector_TypeB_Data_Num = 100;			// һ����������� B �� ��������

const uint8_t AD_TypeA_Data_Sector_Num =5;		// AD_TypeA_Data  ��������ʹ�õ���������

const uint8_t US_TypeA_Data_Sector_Num =5;		// US_TypeA_Data  ��������ʹ�õ���������

const uint8_t AD_TypeB_Data_Sector_Num =5;		// AD_TypeB_Data  ��������ʹ�õ���������

const uint8_t US_TypeB_Data_Sector_Num =5;		// US_TypeB_Data  ��������ʹ�õ���������


extern u8 Op_Door_Buff[Open_Note_Num_Len];				//���ڻ��濪�ż�¼���� ��д�뻺��


/*(�洢1����̬�����n��һ������)
��ʼλ�ô洢���һ����Ч��ʱЧ�����ö�̬���� 

֮������ȫ���ڴ洢һ���������ڷ�ֹһ��������ο���

��ʽ�� ��һ������

*/

const uint8_t Flash_Word_Num =15;  //���� Flash Ŀ¼��������

const uint8_t OpenDoorNote_Num_Code =0x11; 		//���ż�¼��ֵ



//0x03;  //���� ��ʧЧ��̬�����¼
//0X01;  //������������ ����

const uint8_t Act_PassWord_Num_Code =0x02;// ��̬����洢Ŀ¼���

const uint8_t Failure_Act_PassWord_Num_Code = 0X03;				//Sec_dat[i*7+2] = 0x03;  //���� ��ʧЧ��̬�����¼

const uint8_t Per_Act_PassWord_Num_Code =0x04;// ���ö�̬����洢Ŀ¼���

const uint8_t More_Act_PassWord_Num_Code = 0x05;			//����ʱЧ��̬����洢Ŀ¼

const uint8_t AD_TypeA_Data_Num_Code = 0X12; // ����ԱA����Ϣ�洢Ŀ¼���

const uint8_t AD_TypeB_Data_Num_Code = 0X13; // ����ԱB����Ϣ�洢Ŀ¼���

const uint8_t US_TypeA_Data_Num_Code = 0X14; // �û�A����Ϣ�洢Ŀ¼���

const uint8_t US_TypeB_Data_Num_Code = 0X15; // �û�B����Ϣ�洢Ŀ¼���

const uint8_t CK_Auth_Index_Code = 0x16;  //�����豸�������ݿ����к� @ck_auth_index

const uint8_t Nmg_Auth_Index_Code = 0x17;  //�����豸�������ݿ����к� @ nmg_auth_index

const uint8_t PS_Print_Num_Code = 0x18;			//���屣�����Чָ������



const uint16_t Open_Note_Secotr1 = 700;
const uint16_t Open_Note_Secotr2 = 701;
const uint16_t Open_Note_Secotr3 = 702;
const uint16_t Open_Note_Secotr4 = 703; 

//�ĸ����ż�¼ר������


uint8_t Read_Op_Tab[Open_Note_Num_Len];

const uint16_t Open_Note_Size =240;  //���忪�ż�¼����
const uint16_t A_Sector_Open_Note_Size = 60;

const uint8_t Max_PassWord_Num = 200;  //�����������
const uint8_t A_Sector_PassWord_Num = 100;  //���嵥�����������������ɵ���������



const uint8_t PS_Print_Len = 13;		//0x55+2�ֽ�ָ������+2�ֽ�ָ��ID+4�ֽ���ʼʵ��+4�ֽ�ʧЧʵ��

const uint8_t PS_Print_Num =100;			//���浽ָ������

const uint16_t PS_Print_Soctor1 = 446;  //����ָ����������1



//const uint8_t PS_Print_Secotr1 = 


//void Spi_Flash_Write_Data
//(Flash_TAB,FLASH_BLOCK_NUM,FLASH_PAGE_NUM,sizeof(FLASH_TAB))
//Flash_TAB for input *p,FLASH_BLOCK_NUM <= 63 (0-63), 
//FLASH_PAGE_NUM <= 255 (0-255) sizeof(FLASH_TAB) <= 256

//void SpiFlash_Read_Data
//(Flash_TAB,FLASH_BLOCK_NUM,FLASH_PAGE_NUM,sizeof(FLASH_TAB))
//FLASH_TAB for output *p, FLASH_BLOCK_NUM<= 63 (0-63)
//FLASH_PAGE_NUM <= 255 (0-255) sizeof(FLASH_TAB) <= 256

//SPIFlash_Erase_Sector(FLASH_BLOCK_NUM,FLASH_Sector)
//FLASH_BLOCK_NUM :��(0-63) 
//FLASH_BLOCK_NUM: (0-15)
void SPI_Flash_Busy()  //���������õȴ�����
{
	
  uint32_t SR_dat;
	uint32_t i; 
	i =0; // �ȴ�ѭ������
	do
	{
  SR_dat = SpiFlash_ReadSR();
	nrf_delay_us(1);
	i++;
		if(i>400000) break;   //д��ȴ���ʱ
	}while((SR_dat&SPIFLASH_WRITE_BUSYBIT)==0x01);
	
	nrf_delay_us(1);
}


/*********************************
extern uint8_t Devic_Num[];  //Devic_Num ����
�ڰ�ǰ�����ڱ����д��󶨺�
���󶨺����ڱ����Flash�ж�ȡ�İ󶨺�
*********************************/
/******************************
��ȡ�豸�󶨺�
λ�ã� 0 Block 0 Sector 0 PAGE 
λ�ã� 0 Block 0 Sector 1 PAGE //�����豸�󶨺�

��ע�� ���� 0 Block 0 Sector �������豸�󶨺�

*********************************/
void Read_Devic_Num ()  
{
 	uint8_t read_tab [Devic_PAGE_Len];
  uint8_t i;
	i =0;
  SpiFlash_Read_Data 
  (Devic_Num,Devic_Num_BLOCK,Devic_Num_PAGE,sizeof(read_tab));//��ȡ����
   
  if( read_tab[0] == PAGE_Heart_H &&
     read_tab[1] == PAGE_Heart_L ) //�ж���ʼ������Ч��
    {
			for(i =0;i<Devic_Num_Len;i++)  //ȷ��������Ч�󣬶�ȡ���ݵ�����
			{
				Devic_Num[i] = read_tab[i+2];		
			}
    }
		else
			Devic_Static = false;
}

/***************************************
д���豸�󶨺�
λ�ã� 0 Block 0 Sector 0 PAGE
λ�ã� 0 Block 0 Sector 1 PAGE //�����豸�󶨺�

��ע������ 0 Block 0 Sector �������豸�󶨺�
д�豸�󶨺ţ������������豸�ű���պ󣬼�2bitҳ��ʼ�벻����ʱ
***************************************/


void Write_Devic_Num () //д���µİ�����
{
	uint8_t read_tab [Devic_PAGE_Len];
	uint8_t tab[Devic_PAGE_Len];
	uint32_t i;
//	uint32_t dat;
	i =0;
	SpiFlash_Read_Data
	(read_tab,Devic_Num_BLOCK,
	Devic_Num_PAGE,Devic_PAGE_Len); //��ȡ����ȷ��������Ч��
	
	if(read_tab[0] == PAGE_Heart_H &&
		read_tab[1] == PAGE_Heart_L ) //�ж���ʼ������Ч��
		{
			//����ʼ������Чʱ�����԰��������ı�
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_Num not Write!\r\nDevic_static_true!\r\n");
			
			#endif
		}
		else
		{  //����ʼ������Чʱ����ʼд������
			SPIFlash_Erase_Sector(Devic_Num_BLOCK,Devic_Num_Sector); //�� �豸�󶨺������������в���
			
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

void CL_Devic_Num()  //����豸����Ϣ
{
	//Spi_Flash_Init ();
	
	SPIFlash_Erase_Sector(Devic_Num_BLOCK,Devic_Num_Sector);  //����豸����Ϣ
	
	//Spi_Flash_Sleep();
	
}

/********************************************************

�������ҳ����(һƬ��64*256ҳ)��ת��Ϊҳ�Ϳ�����
����д��Ѱַ

*******************************************************/

void PAGE_to_BLOCK_PAGE(uint32_t *BLOCK,uint32_t *PAGEO,uint32_t PAGE)
{
		*BLOCK = PAGE/256; //ÿ����256ҳ(0-255) д��Ϊ��ҳд��
		*PAGEO = PAGE%256; 
}

/*************************************************************

�������ҳ����(һƬ��64*256ҳ),ת��Ϊ�����Ϳ�����
����Ѱ��ҳ��Ӧ����������ɾ������������Ѱ��

**************************************************************/
void PAGE_to_Block_Sector(uint32_t *BLOCK,uint32_t *SECTOR,uint32_t PAGE)
{
	if(PAGE<256)
	{
		*SECTOR = PAGE/16;
		*BLOCK = 0;
	}
	else
	{
		*SECTOR = PAGE%256/16;
		*BLOCK = PAGE/256;
	}
}

/***************************************************************
���ڻ�ȡ���������ڵĿ��ţ�ʵ��ʹ����������Ϊ�����ָλ��
,�����ɶ�Ӧ���ҳ�� (д��ʱ��ҳΪ��С��λ)
��������ת��Ϊ����ֱ������д�����Ŀ��ҳ�����
***************************************************************/
void Sector_to_Block_Page(uint32_t *Block,uint32_t *PAGE,uint32_t SECTOR)
{
	if(SECTOR<16)
	{
		*PAGE = SECTOR*16; //����ת��Ϊҳ
		*Block =0;
	}
	else
	{
		*PAGE = SECTOR%16*16; //�������������Ա�Ȼ��ҳ��������
		*Block = SECTOR/16;
	}
}
/***************************************************************
���ڻ�ȡ���������ڿ�������ţ�
��������ת��Ϊ����������ŵ���ϣ����ڲ�������ʱ����û�������

***************************************************************/

void Sector_to_Block_Sector(uint32_t *Block,uint32_t *Sector,uint32_t SECTOR)
{
	if(SECTOR<16)
	{
		*Sector = SECTOR;  //����������С����ʼ������Сʱ
		*Block = 0;
	}
	else
	{
		*Sector = SECTOR%16;  //�ó�ʵ������������
		*Block = SECTOR/16;
	}
}

/*************************************************************
������ݹ��ܣ�����Ӧҳд��0x00��
ʵ���������Ĺ��ܣ�0
����д��Ԥ���ʼ���ݣ�

(��ǰֻԤ��3���������ͣ����룬��̬���룬���ż�¼)
*************************************************************/

void CL_FlashWord_Num() //����������洢����(������������Чʱ����������ͬʱʧЧ)
{
	uint8_t Sec_dat[256];
	uint32_t i;
	uint32_t Block,Sector,Page;
	
	#ifdef uart_start
	

		SEGGER_RTT_printf(0,"\r\n Start_CL_FlashWord_Num!\r\n");
		
	
	#endif
	
	
	for(i =0;i<sizeof(Sec_dat);i++)
	{
		Sec_dat[i] = 0xFF;
	}
	
	for(i =0;i<Cat_Dat_Sector_Num;i++)   // �������� Ŀ¼����
	{
		
		Sector_to_Block_Sector(&Block,&Sector,Cat_Dat_Sector1+i);  //��ȡ������Ӧ�������������
		SPI_Flash_Busy();
		SPIFlash_Erase_Sector ( Block,Sector ); //�û�ȡ�������ݣ�������Ӧ����
		SPI_Flash_Busy();
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"CL_Cat_Dat_Sector_Num: %d ! \r\n",i+Cat_Dat_Sector1);
		
		#endif
		
	}
		
//	Sector_to_Block_Sector(&Block,&Sector,Cat_Dat_Sector1);  //��ȡ������һ���������������
	
	//SPIFlash_Erase_Sector ( Block,Sepwctor ); //�û�ȡ�������ݣ�������Ӧ����
	
	SPI_Flash_Busy();
	
	Sector_to_Block_Page(&Block,&Page,Cat_Dat_Sector1);  //�������ţ���ȡ�����ҳ��
	
	for(i =0;i<Flash_Word_Num;i++)  //д������Ԥ����������
	{
		
		Sec_dat[i*7] = 0X55;
		Sec_dat[i*7+1] = 0xAA;
		switch (i)
		{
			case 0: 
				Sec_dat[i*7+2] = 0X01;  //������������ ����
					break;
			
			case 1:
				Sec_dat[i*7+2] = 0x02;  //��̬��������
					break;
			
			case 2:
				Sec_dat[i*7+2] = 0x03;  //���� ��ʧЧ��̬�����¼
					break;
			
			case 3:
				Sec_dat[i*7+2] = 0x11;  //���忪�ż�¼ ����
					break;
			
			case 4:
					Sec_dat[i*7+2] = 0x12;  //�������ԱA��Ƭ��Ϣ  ����
					break;
			
			case 5:
				Sec_dat[i*7+2] = 0x13;  //�������Ա���֤��Ϣ ����
					break;
			
			case 6:
				Sec_dat[i*7+2] = 0x14;  //�����û�A����Ϣ����
					break;
			
				case 7:
				Sec_dat[i*7+2] = 0x15;  //�����û����֤��Ϣ����
					break;
				
				case 8:
					Sec_dat[i*7+2] = 0x16;  //�����豸�������ݿ����к� @ck_auth_index
				break;
				
				case 9:
					Sec_dat[i*7+2] = 0x17;  //�����豸�������ݿ����к� @ nmg_auth_index
				break;
				
				case 10:
					Sec_dat[i*7+2] = 0x18;			//���屣�����Чָ������	
				break;
				
				case 11: 
					Sec_dat[i*7+2] = 0x04;			//���ö�̬��������
				break;
				
				case 12:
					
					Sec_dat[i*7+2] = More_Act_PassWord_Num_Code;			//���鶯̬��������
	
				//const uint8_t More_Act_PassWord_Num_Code = 0x05;			//����ʱЧ��̬����洢Ŀ¼

				break;
				
			default:  
				
			//	i =0;   //i ��������ѭ�� ��������²����ܷ���
				break;
		}
		
		Sec_dat[i*7+3] = 0x00;  // 0xFF��ʾ��Ŀ¼δ��ʹ�ù�(δ����ʹ��)��֮�����Ϊ0x00 ���ڹ۲�
		Sec_dat[i*7+4] = 0x00;
		Sec_dat[i*7+5] = 0x00;
		Sec_dat[i*7+6] = 0x00;
		
		
	}
	
	#ifdef uart_start
	
	for(i =0;i<sizeof(Sec_dat);i++)
	{

		SEGGER_RTT_printf(0,"%x,",Sec_dat[i]);
		if(i%16==0)
		{
			SEGGER_RTT_printf(0,"\r\n");
		}
	}
						
	SEGGER_RTT_printf(0,"\r\n");

	#endif
	
	SPI_Flash_Busy();
	SpiFlash_Write_Data(Sec_dat,Block,Page,sizeof(Sec_dat));
	SPI_Flash_Busy();
	
	Erase_PS_Print_Sector();		//��������ʱ���ָ������
	
		CL_PS_ID	 =0;			//�������ָ����Ϣ
		CL_PS_Run = CL_PS_Run_Del_All_Mode;
}

/******************************************************************
SPI_Flash 1�������������ֽڶ�ȡ ��ҳΪ��׼��ʼλ��,���ݾ�ȷ���ֽ�
******************************************************************/
void SPIFlash_Read_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len)
{
	uint32_t page_n;
	uint32_t page;
	uint32_t i;
	
	if(tab_len<256)
		SpiFlash_Read_Data(pBuffer,Block_Num,Page_Num,tab_len);
	else
	{
		page_n = tab_len/256;  //�������ȡ�������ж���ҳ
		page = Page_Num;				//��ʼҳ����
		for(i =0;i<page_n;i++)
		{
			SpiFlash_Read_Data (pBuffer+i*256,Block_Num,page+i,256);  //ѡ���Ӧҳ����˳���ȡ����
		}
		if (tab_len%256>0)  //�����������ʱ �ٶ�ȡһ������
		{
			SpiFlash_Read_Data (pBuffer+page_n*256,Block_Num,page+page_n,tab_len%256);  //�����ķ���ҳ���ݶ�ȡ���� (һ��������Ϣ���ݻ�ȡ)
		}				
	}
}
/*************************************************************************
SPI_Flash 1�������������ֽ�д�� ��ҳΪ��׼��ʼλ�ã����ݾ�ȷ���ֽ�
**************************************************************************/

void SPIFlash_Write_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len) 
{
	uint32_t page_n;
	uint32_t page;
	uint32_t i;
	
	if(tab_len<256)
	{
		
		SpiFlash_Write_Data(pBuffer,Block_Num,Page_Num,tab_len); //���볤��С��һҳ����ֻд��һҳ����
		
	}
		else
	{
		page_n = tab_len/256;  //�����д�������ж���ҳ
		page = Page_Num;				//��ʼҳ����
		for(i =0;i<page_n;i++)
			{
				SpiFlash_Write_Data (pBuffer+i*256,Block_Num,page+i,256);  //ѡ���Ӧҳ����˳��д������
				SPI_Flash_Busy();
			}
			if (tab_len%256>0)  //�����������ʱ ��д��һ������
			{
				SpiFlash_Write_Data (pBuffer+page_n*256,Block_Num,page+page_n,tab_len%256);  //�����ķ���ҳ����д�뵽���� (һ��������Ϣ����д��)
				SPI_Flash_Busy();
			}
	}
	SPI_Flash_Busy();
}

/*******************************************************************
�������������
������������������ݣ�������������

*******************************************************************/
//void CL_Flash_PassWord()
//{
//	uint8_t tab_dat1[256];
//	uint32_t i;
//	uint32_t Block,Sector,Page;
//	
//	for(i =0;i<sizeof(tab_dat1);i++)
//	tab_dat1[i] = 0xFF;
//	
//	Sector_to_Block_Page(&Block,&Page,1); //��������,��ȡ�����ҳ��
//	
//	SpiFlash_Read_Data (tab_dat1,Block,Page,sizeof(tab_dat1)); //��ȡ���ݵ�TAB 
//	SPI_Flash_Busy();
//	
//	for(i =0;i<3;i++)
//	{
//		if (tab_dat1[i*6]==0X55&&tab_dat1[i*6+1]==0xAA) //��������Чʱ��ʼ����
//		{
//			if(tab_dat1[i+2]==0x01)  //ѡ����������Ŀ¼�������
//			{
//				 tab_dat1[i+3] = 0x00;
//				 tab_dat1[i+4] = 0x00;
//				 tab_dat1[i+5] = 0x00;
//			}
//		}
//	}
//	Sector_to_Block_Sector(&Block,&Sector,1); //��������ҳ ��ΪĿ¼��СĿǰ��һҳ����(256byte)
//	SPI_Flash_Busy();
//	SPIFlash_Erase_Sector(Block,Sector);     //��������
//	SPI_Flash_Busy();
//	Sector_to_Block_Page(&Block,&Page,1);  //��ȡ��Ҫд���λ�� �顢ҳ��
//	SpiFlash_Write_Data(tab_dat1,Block,Page,sizeof(tab_dat1));  //д��һҳ ��ΪĿ¼
// 	SPI_Flash_Busy();
//	
//	//SPIFlash_Erase_Sector(0,0);
//	SPI_Flash_Busy();
//	
////	SpiFlash_Read_Data(tab_dat,Block,page,sizeof(tab_dat));
//}

void CL_Flash_PassWord()  //�����������
{
	Ev_Cat_Dat(PassWord_Num_code,0); //�������
}

void CL_Flash_OPEN_DOOR_NOTE()  //��տ��ż�¼����
{
	Ev_Cat_Dat(OpenDoorNote_Num_Code,0); //�������
}



/***************************************************
��ȡָ������Ŀ¼��Ϣ

���룺�������� Cat_Type
���أ�Ŀ¼��������������
***************************************************/

//uint32_t Get_Cat_Dat(uint8_t Cat_Type) //��ȡĿ¼��Ϣ
//{
//		uint8_t tab_dat[256];  //Ŀ¼����
//		uint32_t i;
//		uint32_t Block,Page;
//		
//		uint32_t len_i;
//		
//	for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] = 0xFF;
//	Sector_to_Block_Page (&Block,&Page,1);    //Ŀ¼����������Ϊ 1
//  	len_i =0xFFFFFF;
//	SpiFlash_Read_Data(tab_dat,Block,Page,sizeof(tab_dat));  //��ȡĿ¼��Ϣ
//	
//		for(i =0;i<Flash_Word_Num;i++) // 0x01 �������� 0x02 ��̬��������  0x11 ���ż�¼  0x12 ��Ƭ��Ϣ
//		{
//			if(tab_dat[i*7]==0x55&&tab_dat[i*7+1]==0xAA)  //��������Чʱ��ʼ��ȡĿ¼����
//			{
//				if(tab_dat[i*7+2]==Cat_Type) //��Ŀ��Ŀ¼���ݽ�����ȡ
//				{
//					len_i = tab_dat[i*7+3];
//					len_i<<=8;
//					len_i |= tab_dat[i*7+4];
//					len_i<<=8;
//					len_i |= tab_dat[i*7+5];
//					len_i<<=8;
//					len_i |=tab_dat[i*7+6];
//				}
//			}
//		}
//		
//	return len_i;

//}

uint32_t Get_Cat_Dat(uint8_t Cat_Type) //��ȡĿ¼��Ϣ
{
		uint8_t Sec_dat[Cat_Dat_len];  //Ŀ¼����
	
		uint32_t i;
		uint32_t Block,Page;
	//	uint32_t Dat_Sector;		// ��Ч������������ �����ʽΪ �ڵڼ���Ŀ¼����
		uint32_t len_i;
		
		len_i =0;
	
		#ifdef uart_start
	
		SEGGER_RTT_printf(0,"\r\nStart Get_Cat_Dat !\r\n");
		SEGGER_RTT_printf(0,"\r\n len_i: %x!\r\n",len_i);
	
		#endif
	
	
		for(i =0;i<sizeof(Sec_dat);i++) Sec_dat[i] = 0xFF;		// ��Ŀ¼������λ
		
		for(i =0;i<Cat_Dat_Sector_Num;i++)		// ��ÿ���������ж�ȡ
		{
		
			Sector_to_Block_Page (&Block,&Page,Cat_Dat_Sector1+i);    //Ŀ¼����������Ϊ 1
			
			SpiFlash_Read_Data(Sec_dat,Block,Page,sizeof(Sec_dat));  //��ȡĿ¼��Ϣ
			
			if((Sec_dat[0]==0X55)&&(Sec_dat[1]==0xAA))   //���ҵ���Ч��������ʱ
			{
				
			//	 Dat_Sector =i;
				 
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nCat_Dat_Sector_Num : %d !\r\n",Cat_Dat_Sector1+i);		//���� ��ȡ������Ч������
				
				for(i =0;i<sizeof(Sec_dat);i++)
				{
					SEGGER_RTT_printf(0,"%x,",Sec_dat[i]);
					if(i%16==0)
					{
						SEGGER_RTT_printf(0,"\r\n");
					}
				}
						
				SEGGER_RTT_printf(0,"\r\n");

				
				#endif
					
				for(i =0;i<Flash_Word_Num;i++) // 0x01 �������� 0x02 ��̬��������  0x11 ���ż�¼  0x12 ��Ƭ��Ϣ
					{
						if(Sec_dat[i*7]==0x55&&Sec_dat[i*7+1]==0xAA)  //��������Чʱ��ʼ��ȡĿ¼����
						{
							if(Sec_dat[i*7+2]==Cat_Type) //��Ŀ��Ŀ¼���ݽ�����ȡ
							{
								len_i = Sec_dat[i*7+3];
								len_i<<=8;
								len_i |= Sec_dat[i*7+4];
								len_i<<=8;
								len_i |= Sec_dat[i*7+5];
								len_i<<=8;
								len_i |=Sec_dat[i*7+6];
								
									#ifdef uart_start
				
									SEGGER_RTT_printf(0,"Cat_Dat_Type: 0x%x .\r\n",Cat_Type);		//���� Ŀ¼��������
										
									SEGGER_RTT_printf(0,"Cat_Dat_Valye: 0x%x .\r\n",len_i);		//���� Ŀ¼��������
									
									SEGGER_RTT_printf(0,"Get_Cat_Dat_Ok! .\r\n\r\n");		//���� Ŀ¼��������									
											
									#endif
										
									return len_i;
							}
						}
						 
					}

			}
			
		}
		
		return 0xFFFFFFFF;
		
}




/**********************************************************
��ָ��Ŀ¼��ֵ (ֱ�Ӹ��µ�ֵ)
���룺�������� Cat_Type 
���룺�������� Cat_Dat  ֻȡ�õ�24λ 3byte
**********************************************************/

//void Ev_Cat_Dat(uint8_t Cat_Type,uint32_t Cat_Dat)
//{
//	
//	
//	uint8_t tab_dat[256];  //Ŀ¼����
//	uint8_t tab_num[4];
//	uint32_t i;
////	uint32_t len_t;
//	uint32_t Block,Page,Sector;
//	
//	for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0xFF;
//	for(i =0;i<sizeof(tab_num);i++) tab_num[i] =0xFF;
//	
//	tab_num[0] = Cat_Dat>>24&0xFF;
//	tab_num[1] = Cat_Dat>>16&0xFF;  //�����ݽ���Ԥ���� ��ȡ��24bit
//	tab_num[2] = Cat_Dat>>8&0xFF;
//	tab_num[3] = Cat_Dat&0xFF;
//	
//	Sector_to_Block_Page (&Block,&Page,1);			//Ŀ¼����������Ϊ 1
//	
//	SpiFlash_Read_Data (tab_dat,Block,Page,sizeof(tab_dat));  //��ȡĿ¼��Ϣ
//	
//	for(i =0;i<Flash_Word_Num;i++)   //��5��Ŀ¼��ѡ����Ҫ�޸ĵ�Ŀ¼
//	{
//		if(tab_dat[i*7]==0x55&&tab_dat[i*7+1]==0xAA)    	//��������Чʱ��ʼ��ȡĿ¼����
//		{
//			if(tab_dat[i*7+2]==Cat_Type)  //��Ŀ¼���ݽ�����ȡ
//			{
//				tab_dat[i*7+3] = tab_num[0];		//���޸Ķ�ӦĿ¼�µ�����
//				tab_dat[i*7+4] = tab_num[1];
//				tab_dat[i*7+5] = tab_num[2];
//				tab_dat[i*7+6] = tab_num[3];
//				
//			}
//		}
//	}
//	Sector_to_Block_Sector (&Block,&Sector,1);   //д��ǰ�Ȳ���Flash
//	SPIFlash_Erase_Sector (Block,Sector);
//	SPI_Flash_Busy();
//	
//	Sector_to_Block_Page(&Block,&Page,1);
//	SpiFlash_Write_Data(tab_dat,Block,Page,sizeof(tab_dat));  //��������д��SPI_Flash
//	SPI_Flash_Busy();
//	
//}

void Ev_Cat_Dat(uint8_t Cat_Type,uint32_t Cat_Dat)
{
		
		uint8_t Sec_dat[Cat_Dat_len];  //Ŀ¼����
		uint8_t tab_num[4];
		uint8_t tab_Cl[4];
		uint32_t i;
		uint32_t Dat_Sector;
//		uint32_t len_t;
		uint32_t Block,Page,Sector;
		
	for(i =0;i<sizeof(Sec_dat);i++) Sec_dat[i] =0xFF;
	for(i =0;i<sizeof(tab_num);i++) tab_num[i] =0xFF;
	
	for(i =0;i<sizeof(tab_Cl);i++) tab_Cl[i] =0;			//׼��������յ�����
	
	tab_num[0] = Cat_Dat>>24&0xFF;
	tab_num[1] = Cat_Dat>>16&0xFF;  //�����ݽ���Ԥ���� ��ȡ��24bit
	tab_num[2] = Cat_Dat>>8&0xFF;
	tab_num[3] = Cat_Dat&0xFF;
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart Ev_Cat_Dat !\r\n");
	
	#endif
	
		
		
		for(i =0;i<Cat_Dat_Sector_Num;i++)		// ��ÿ���������ж�ȡ
		{
		
			Sector_to_Block_Page (&Block,&Page,Cat_Dat_Sector1+i);    //Ŀ¼����������Ϊ 1
			
			SpiFlash_Read_Data(Sec_dat,Block,Page,sizeof(Sec_dat));  //��ȡĿ¼��Ϣ
			
			
			if((Sec_dat[0]==0X55)&&(Sec_dat[1]==0xAA))   //���ҵ���Ч��������ʱ
			{
					#ifdef uart_start

					for(uint32_t is =0;is<sizeof(Sec_dat);is++)
					{

						SEGGER_RTT_printf(0,"%x,",Sec_dat[is]);
						if(is%16==0)
						{
							SEGGER_RTT_printf(0,"\r\n");
						}
					}
					
					SEGGER_RTT_printf(0,"\r\n");

					#endif
				
				Dat_Sector = Cat_Dat_Sector1+i;			// ��ȡ��Ӧ��������
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Dat_Sector_1: %d .\r\n",Cat_Dat_Sector1);
					
				SEGGER_RTT_printf(0,"Dat_Sector: %d .\r\n",Dat_Sector);
				
				SEGGER_RTT_printf(0,"Write_Cat_Type: 0x%x .\r\n",Cat_Type);
				
				SEGGER_RTT_printf(0,"Write_Cat_Dat: 0x%x .\r\n\r\n",Cat_Dat);
				
				
				#endif
					
				for(i =0;i<Flash_Word_Num;i++)   //�� Flash_Word_Num ��Ŀ¼��ѡ����Ҫ�޸ĵ�Ŀ¼
					{
						if(Sec_dat[i*7]==0x55&&Sec_dat[i*7+1]==0xAA)    	//��������Чʱ��ʼ��ȡĿ¼����
						{
							if(Sec_dat[i*7+2]==Cat_Type)  //��Ŀ¼���ݽ�����ȡ
							{
								
								Sec_dat[i*7+3] = tab_num[0];		//���޸Ķ�ӦĿ¼�µ�����
								Sec_dat[i*7+4] = tab_num[1];
								Sec_dat[i*7+5] = tab_num[2];
								Sec_dat[i*7+6] = tab_num[3];
								
							}
						}
					}
					
						#ifdef uart_start

					for(i =0;i<sizeof(Sec_dat);i++)
					{

						SEGGER_RTT_printf(0,"%x,",Sec_dat[i]);
						if(i%16==0)
						{
							SEGGER_RTT_printf(0,"\r\n");
						}
					}
					
					SEGGER_RTT_printf(0,"\r\n");

					#endif
					
					//�滻�����ݺ��Ƚ�����д�� ��һ������
					//�Ȳ�����һ������(����)
					
							Sector_to_Block_Page(&Block,&Page,Dat_Sector);
							SpiFlash_Write_Data(tab_Cl,Block,Page,sizeof(tab_Cl));  //����д��SPI_Flash ������������
							SPI_Flash_Busy();
					
							if((Dat_Sector-Cat_Dat_Sector1+1)==Cat_Dat_Sector_Num)  //�� ���� �����дﵽ����ʱ�����ص�һ������
								Dat_Sector = Cat_Dat_Sector1;			//�����������һ������ʱ����������д���һ������
							else
								Dat_Sector++;
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Nex_Cat_Dat_Sector: %d .\r\n",Dat_Sector);		//�����һ��Ҫд�������
							
							#endif
							
							Sector_to_Block_Sector (&Block,&Sector,Dat_Sector);   //д��ǰ�Ȳ���Flash
					  	SPIFlash_Erase_Sector (Block,Sector);
					  	SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,Dat_Sector);
							SpiFlash_Write_Data(Sec_dat,Block,Page,sizeof(Sec_dat));  //��������д��SPI_Flash ���ڱ�������
							SPI_Flash_Busy();
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Cat_Dat_Write_Ok! \r\n\r\n");
							
							#endif
			}
		}
	
	
}





/*********************************************************
���������ݽ��бȶԣ���������һ����Ԥ��һ�� �ȶ�ֵΪ tab_dat ��һ���ֻ�ȫ��
����˺���ǰ����Ҫ�Ƚ���������������д��tab_dat
���룺PW_Num ��Ҫ�ȶԵ���������
���룺PW_W ��Ҫ�ȶԵĲο�����
����� �ڼ���������ο�����һ�£�û�з��� Max_PassWord_Num
*********************************************************/
uint32_t PassWord_Same (uint32_t PW_Num,uint8_t *PW_W)
{
	uint32_t i;
	uint32_t now;  //״̬����
	uint8_t PS_W[PassWord_Len];  //��ѯ���뻺��
	uint32_t PW_MN;  // ����tab_dat �����λ��
	uint32_t PW_N; //��ǰ����λ�� Ҳ��Ϊ������������
	uint8_t PWI,PWD; //��ǰ���������� PWI��ʾ����Ĳο����룬PWO��ʾ ȡ��tab_dat������
	uint8_t PWS;  //��ǰ����һ�µ�����
	
	for(i =0;i<PassWord_Len;i++) //���ο����ݷ��뻺��
	{
	PS_W[i] = *PW_W;
	PW_W++;
	}
	PW_N =0;
	PW_MN = PW_Num * PassWord_Len;  //������ռ���
	
	now =0;
	PWS =0;
	while(PW_N<PW_MN) //��ʼ����
	{
		switch(now)
		{

			case 0:
				for(i =0;i<3;i++)  //�ȶ���ǰ���ֽ� �ж�ͷ�����������Ƿ�һ��
				{
					PWI = PS_W[i];
					PWD = tab_dat[PW_N+i];
					if(PWI==PWD) PWS++;
				}
				for(i = 11;i<(PassWord_Len-1);i++) //�жϺ���λֵ�Ƿ�һ��(����λΪ�������)
				{
					PWI = PS_W[i];
					PWD = tab_dat[PW_N+i];
					if(PWI==PWD) PWS++;
				}
				
				now++;
				break;
			case 1:
					if(PWS==(PassWord_Len-9)) //��������ȫһ��ʱ(8Ϊ����ʱ�����ֵ)��һ��SN 1 =9
					return PW_N; //������ζ��գ�������tab_dat����ͬ���ݵ���ʼλ��
					else now++;
					break;
			case 2:  //��һ�ζԱȲ�һ��ʱ׼����ʼ��һ�αȶ�
					PW_N +=PassWord_Len;  //��PWN������һ��������ʼλ��
					PWS =0;
					now =0;
					break;
		}
	}
	return PW_MN; //�������ݶ������ֵ����ʾû����ͬ������tab_dat��
}



/*******************************************************
������������������һ����������
���룺 PassWord  �����������ݵ�ָ��
����� ����ı��
******************************************************/
uint8_t Add_Flash_PassWord(uint8_t *PassWord)  //д��һ��������Flash ���� �����ʽ�� 6byte ��������+ ��Чʱ��+ʧЧʱ��
{
	static uint8_t PassWord_W[PassWord_Len];
	uint8_t tab1[2*A_Sector_PassWord_Num];
	uint32_t i,it,i1,i2,i3; 
	uint32_t len; //�������ݵ�ǰ�ܳ���
	uint8_t PassWord_Num;  //��������
	uint32_t Block,Sector,Page;  //����д��/��ȡ ��������
	uint32_t PW_Same;  //��ͬ��������λ��
	uint8_t  *PassWord_R; //��ȡ��������ָ��
	uint8_t PassWord_SN;  //��ʾ������
	//uint8_t PassWord_Secotr_Num;  //����Ҫ�޸ĵ������ţ�0��ʾ�ޣ�1��ʾ��������1,2��ʾ��������2
	
//Spi_Flash_Init();  //����SPI_FLASH ͨѶ
	
//	PassWord_Secotr_Num = 0;  //�������������
	
	for (i =0;i<sizeof(tab1);i++)
	{
		
		tab1[i] =0x00;
	}
	
	PassWord_Num = Get_Cat_Dat(0x01); //��ȡ��ǰ�洢����������
	len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���

	if(PassWord_Num>0) //���д洢������ʱ
	{
		Sector_to_Block_Page (&Block,&Page,PassWord_SN_Secotr); //��ȡ������������
		SPIFlash_Read_Tab (tab1,Block,Page,PassWord_Num);  //��ȡ���������ݳ��ȵ���������	
	}
	
	PassWord_W[0] = 0x55;
	PassWord_W[1] = 0xAA;  //��������ͷ
	
	for(i =0;i<(PassWord_Len-2);i++)  //��������ͷ�����ֽ�
	{
		PassWord_R = PassWord+i;
		PassWord_W[i+2] = *PassWord_R;
	}
	
	if(PassWord_Num<=A_Sector_PassWord_Num) //��������������С��һ����������ʱ
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��һ����������Ϣ
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ���������ݳ��ȵ���������	
		PW_Same = PassWord_Same (PassWord_Num,PassWord_W); //��ȡ���ݶԱȽ��
		it =0; //���ڼ�¼�޸��˶���byte����
		if(PW_Same<len) //��ת��ֵ����Ч���ݷ�Χ�ڣ���Ч�� ����ʱ���д�� (����������1�Ĳ�����֮��Ƚ�������)
		{
			for(i =0;i<(PassWord_Len-1);i++) //���޸��������
			{
				if(tab_dat[PW_Same+i]!=PassWord_W[i])
				{
					tab_dat[PW_Same+i] = PassWord_W[i]; //ֱ�Ӷ��������ݸ���(������һ��)
					it++;
				}
			}
			PassWord_SN = tab_dat[PW_Same+i]; //��ʱi�Ѿ��ӹ�1����ָ�����һλ�������
			if(it>0) //�����з����仯ʱд�����ݵ�Flash
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //��ȡ��������
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);  //��ȡ��������
				SPIFlash_Write_Tab(tab_dat,Block,Page,len);					//�µ�д������ Ŀ¼�������ı�
				SPI_Flash_Busy ();
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Pass_Word_TimeWrite!\r\n"); 
				#endif
			}
			return PassWord_SN;
		}
		else //��Ч ����һ�µ�����	���һ���µ����ݣ���ʱ��PW_Same����Ϊ��������һд�����ʼ��ַ
		{
			if(PassWord_Num<A_Sector_PassWord_Num) //����������1����ʣ��ռ�ʱ��ֱ��������1����������
			{
				for(i =0;i<(PassWord_Len-1);i++)  //���һ���������ݵ� tab_dat��
				{
						tab_dat[PW_Same+i] = PassWord_W[i];
				}
				PassWord_SN =0;
				i3 =0;
				if(PassWord_Num==0)
				{	
				PassWord_SN =1; //��������Ϊ0ʱ
				tab1[0] =1;
				}
				else
				{
					for(i1 =1;i1<(2*A_Sector_PassWord_Num+1);i1++)
					{
						for(i2 =0;i2<PassWord_Num;i2++)
						{
							if(tab1[i2]==i1) //�����кŴ���ʱ
							{
								i3 =1;
							}
						}
						if(i3!=1)
						{
							PassWord_SN = i1;  //ʹ����������
							tab1[PassWord_Num] = PassWord_SN;
							break;
						}
						else
							i3 =0;
					}
				}
				
//				i3 =0;
//				for(i1 =1;i1<(A_Sector_PassWord_Num+2);i1++) //Ѱ��δ��ռ�õ���� ����Ŵ���1��С����������1�������ɵ�����
//				{
//					for(i2 =0;i2<PassWord_Num;i2++)  // ��������������ѯ
//					{
//						if((tab_dat[i2*PassWord_Len+PassWord_Len-1])==i1) //�����кŴ���ʱ
//						{
//							i3 =1;
//						}
//					}
//					if(i3!=1) //������Ų�����ʱ
//					{
//						PassWord_SN = i1;//ʹ���������ţ�������ѭ��
//						break; 
//					}
//					else
//					{
//						i3 =0; //����ʱ���������ڱ�־λ׼���´���ѯ
//					}
//				}
				
				tab_dat[PW_Same+i] = PassWord_SN;
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_SN:%d\r\n",PassWord_SN); //����������
				#endif
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //�������� PassWord_Secotr1
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��������д������
				SPIFlash_Write_Tab(tab_dat,Block,Page,len+PassWord_Len);
				SPI_Flash_Busy();			

				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //�������� PassWord_SN_Secotr
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��������д������
				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(0X01,PassWord_Num+1);  //����Ŀ¼��
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Write_a_PassWord!\r\n");
				#endif
				return PassWord_SN;  //����д����������
			}
			else if(PassWord_Num==A_Sector_PassWord_Num) //��������������һ��������������ʱ(����һ������д������)��ʱ��������2д���һ������
			{
				//��ʱtab_dat�������(���޸���������1����)
				
				for(i =0;i<sizeof(tab_dat);i++) 
				tab_dat[i] =0xff; //��ջ���
				
				for(i =0;i<(PassWord_Len-1);i++)  //���һ���������ݵ� tab_dat��
					{
							tab_dat[i] = PassWord_W[i]; //д��һ�����ݣ�ֱ��д��
					}
					
				PassWord_SN =0;
				if(PassWord_Num==0)PassWord_SN =1; //��������Ϊ0ʱ
				else
				{
					for(i1 =1;i1<(2*A_Sector_PassWord_Num+1);i1++)
					{
						for(i2 =0;i2<PassWord_Num;i2++)
						{
							if(tab1[i2]==i1) //�����кŴ���ʱ
							{
								i3 =1;
							}
						}
						if(i3!=1)
						{
							PassWord_SN = i1;  //ʹ����������
							tab1[PassWord_Num] = PassWord_SN;
							break;
						}
						else i3 =0;
					}
				}
					
					tab_dat[i] = PassWord_SN;  //ֱ��ʹ����������Ÿ�ֵ��������2��һ������
					PassWord_SN = tab_dat[i];
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_SN:%d\r\n",PassWord_SN); //����������
				#endif
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //�������� PassWord_Secotr2
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //����������2��д������
				SPIFlash_Write_Tab(tab_dat,Block,Page,PassWord_Len);
				SPI_Flash_Busy();
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //�������� PassWord_SN_Secotr
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��������д������
				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(0X01,PassWord_Num+1);  //����Ŀ¼��
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Write_a_PassWord!\r\n");
				#endif
				return PassWord_SN;  //����д����������
			}
			else  //�������������쳣
			{
				#ifdef uart_start
				printf("PassWord_Num_Out!\r\n");
				#endif
				return 0;
			}
		}
	}
	else if((PassWord_Num>A_Sector_PassWord_Num)&&(PassWord_Num<=(2*A_Sector_PassWord_Num))) //��д�������ڴ��ڵ�һ�������͵ڶ�����������֮��
	{
		for(i =0;i<sizeof(tab_dat);i++) //��ʼ�жϵ�һ������
		tab_dat[i] =0xff; //��ջ���

		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��һ����������Ϣ
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //��ȡ���������ݳ��ȵ���������(��ʱ1��������,ȫ����ȡ)
		PW_Same = PassWord_Same (A_Sector_PassWord_Num,PassWord_W); //��ȡ��һ�����������ݶԱȽ�� //�����������ͼȶ�������ΪA_Sector_PassWord_Num
		if(PW_Same<(A_Sector_PassWord_Num*PassWord_Len)) //�жϵ�һ�����Ƿ�����ͬ����
		{
		//	PassWord_Secotr_Num=1; //��ʾ��һ��������ͬ���� //ֱ���滻������Ϣ����
			it =0; //���ڼ�¼�޸��˶���byte����
			for(i =0;i<(PassWord_Len-1);i++) //���޸��������
			{
				if(tab_dat[PW_Same+i]!=PassWord_W[i])
				{
					tab_dat[PW_Same+i] = PassWord_W[i]; //ֱ�Ӷ��������ݸ���(������һ��)
					it++;
				}
			}
			PassWord_SN = tab_dat[PW_Same+i]; //��ʱi�Ѿ��ӹ�1����ָ�����һλ�������
			if(it>0) //�����з����仯ʱд�����ݵ�Flash
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //��ȡ��������
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);  //��ȡ��������
				SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);					//�µ�д������ Ŀ¼�������ı�
				SPI_Flash_Busy ();
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Pass_Word_TimeWrite!\r\n"); 
				#endif
			}
			return PassWord_SN;
		}
		else if (PW_Same==(A_Sector_PassWord_Num*PassWord_Len)) //��δ�ڵ�һ���������ҵ���ͬ����ʱ �����ڶ����� �����������Ȼд��ڶ�����
		{
		for(i =0;i<sizeof(tab_dat);i++) //��ʼ�жϵڶ�������
		tab_dat[i] =0xff; //��ջ���
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ�ڶ�����������Ϣ
		SPIFlash_Read_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len);  //��ȡ���������ݳ��ȵ���������(��ʱ1��������,ȫ����ȡ)
		PW_Same = PassWord_Same (PassWord_Num-A_Sector_PassWord_Num,PassWord_W); //��ȡ�ڶ������������ݶԱȽ�� PassWord_Num-A_Sector_PassWord_Num Ϊʣ����Ҫ���յ�������Ϣ
		
		if(PW_Same<(len-A_Sector_PassWord_Num*PassWord_Len)) //�жϵڶ������Ƿ�����ͬ����
			{
			//	PassWord_Secotr_Num=2;  //��ʾ�ڶ���������ͬ����//ֱ���滻������Ϣ	
				it =0; //���ڼ�¼�޸��˶���byte����
				for(i =0;i<(PassWord_Len-1);i++) //���޸��������
				{
					if(tab_dat[PW_Same+i]!=PassWord_W[i])
					{
						tab_dat[PW_Same+i] = PassWord_W[i]; //ֱ�Ӷ��������ݸ���(������һ��)
						it++;
					}
				}
				
				PassWord_SN = tab_dat[PW_Same+i]; //��ʱi�Ѿ��ӹ�1����ָ�����һλ�������
				if(it>0) //�����з����仯ʱд�����ݵ�Flash
				{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //��ȡ��������
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);  //��ȡ��������
						SPIFlash_Write_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len); //�µ�д������ Ŀ¼�������ı�
						SPI_Flash_Busy ();
						#ifdef uart_start
						SEGGER_RTT_printf(0,"Pass_Word_TimeWrite!\r\n"); 
						#endif
				}
				return PassWord_SN;
			}
			else if((PassWord_Num<(2*A_Sector_PassWord_Num))&&(PW_Same==(len-A_Sector_PassWord_Num*PassWord_Len)))  //��δ�ڵڶ�����������ͬ�����һ���ʣ��ռ�ʱ��ֱ���ڵڶ�������д��һ������Ϣ
			{
				
				for (i =0;i<(PassWord_Len-1);i++)  //���һ���������ݵ� tab_dat��
				{
					tab_dat[PW_Same+i] = PassWord_W[i];
				}
				
				PassWord_SN =0;
				i3 =0;
				if(PassWord_Num==0)
				{
					PassWord_SN =1; //��������Ϊ0ʱ
				}
				else
				{
					for(i1 =1;i1<(2*A_Sector_PassWord_Num+1);i1++)
					{
						for(i2 =0;i2<PassWord_Num;i2++)
						{
							if(tab1[i2]==i1) //�����кŴ���ʱ
							{
								i3 =1;
							}
						}
						if(i3!=1)
						{
							PassWord_SN = i1;  //ʹ����������
							tab1[PassWord_Num] = PassWord_SN;
							break;
						}
						else i3 =0;
					}
				}
				
//				i3 =0;
//				for(i1 =A_Sector_PassWord_Num+1;i1<(A_Sector_PassWord_Num*2+2);i1++) //Ѱ��δ��ռ�õ���� ����Ŵ���1��С����������1�������ɵ�����
//				{
//					for(i2 =0;i2<(PassWord_Num-A_Sector_PassWord_Num);i2++)  // ��������������ѯ
//					{
//						if((tab_dat[i2*PassWord_Len+PassWord_Len-1])==i1) //�����кŴ���ʱ
//						{
//							i3 =1;
//						}
//					}
//					if(i3!=1) //������Ų�����ʱ
//					{
//						PassWord_SN = i1;//ʹ���������ţ�������ѭ��
//						break; 
//					}
//					else
//					{
//						i3 =0; //����ʱ���������ڱ�־λ׼���´���ѯ
//					}
//				}
				
				tab_dat[PW_Same+i] = PassWord_SN;
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_SN:%d\r\n",PassWord_SN); //����������
				#endif
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //�������� PassWord_Secotr2
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //����������2��д������
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len+PassWord_Len);
				SPI_Flash_Busy();
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //�������� PassWord_SN_Secotr
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��������д������
				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
				SPI_Flash_Busy();
				
				
				Ev_Cat_Dat(0X01,PassWord_Num+1);  //����Ŀ¼��
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Write_a_PassWord!\r\n");
				#endif
				return PassWord_SN;  //����д����������
			}
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_Secotr_Full!\r\n"); //��ʾ��д������
				#endif
				return 0;
			}
		}
	}
	return 0;
}

/*************************************************************************************
ɾ��һ���������������������
���룺 PassWord  ��Ҫɾ��������
*************************************************************************************/
void Del_Flash_PassWord(uint8_t *PassWord)
{
	uint8_t PassWord_W[PassWord_Len];
	uint32_t i;
	uint32_t len;  					//���ݻ����������
	uint32_t Block,Sector,Page;
	uint32_t PW_Same;  //��ͬ��������λ��
	uint8_t *PassWord_R;
	uint32_t PassWord_Num;
	
	
	PassWord_Num = Get_Cat_Dat(0x01); //��ȡ��ǰ�洢����������
	len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���
	PassWord_W[0] = 0x55;
	PassWord_W[1] = 0xAA;  //��������ͷ
	for(i =0;i<(PassWord_Len-2);i++)  //��������ͷ�����ֽ������ɾ���������ڱȶ�
	{
		PassWord_R = PassWord+i;
		PassWord_W[i+2] = *PassWord_R;
	}
	
	if(len<=(A_Sector_PassWord_Num*PassWord_Len)) //�������ܳ�����һ��������ʱ
	{
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��������
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ���������ݳ��ȵ���������
			
			
			PW_Same = PassWord_Same (PassWord_Num,PassWord_W); //��ȡ���ݶԱȽ��
			
			
			if(PW_Same<len) //�ҵ���Ҫɾ�������� PW_Same ��Ϊ�滻��ʼ��ַ len��Ϊ��ֹ��ַ
			{
				for(i =PW_Same;i<len;i++)  //�����������ƶ�һ����λ
				{
					tab_dat[i] =tab_dat[i+PassWord_Len];
				}
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-PassWord_Len);
				Ev_Cat_Dat(0x01,PassWord_Num-1);
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_Clear!\r\n");
				#endif
			}
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Not_Found_PassWord!\r\n");
				#endif
			}
	}
	else if((len>A_Sector_PassWord_Num*PassWord_Len)&&(len<=(2*A_Sector_PassWord_Num*PassWord_Len)))//�����볤�ȴ���һ������������������������������
	{
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��������
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //��ȡ��һ���������ݳ��ȵ���������
			
			PW_Same = PassWord_Same (A_Sector_PassWord_Num,PassWord_W); //��ȡ���ݶԱȽ��������1������
			
			if(PW_Same<(A_Sector_PassWord_Num*PassWord_Len)) //�ҵ���Ҫɾ�������� PW_Same ��Ϊ�滻��ʼ��ַ len��Ϊ��ֹ��ַ
			{
				
				for(i =PW_Same;i<len;i++)  //�����������ƶ�һ����λ
				{
					tab_dat[i] =tab_dat[i+PassWord_Len];
				}
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
				SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len); //д���һ��������
				Ev_Cat_Dat(0x01,PassWord_Num-1);
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_Clear!\r\n");
				#endif
			}
			else if(PW_Same==A_Sector_PassWord_Num*PassWord_Len) //��һ������û������ʱ���Ի�ȡ�ڶ�������
			{
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
					SPIFlash_Read_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len);  //��ȡ��һ���������ݳ��ȵ���������
					PW_Same = PassWord_Same (PassWord_Num-A_Sector_PassWord_Num,PassWord_W); //��ȡ���ݶԱȽ��
				  if(PW_Same<(len-A_Sector_PassWord_Num*PassWord_Len))
					{
						for(i =PW_Same;i<len;i++)  //�����������ƶ�һ����λ
						{
							tab_dat[i] =tab_dat[i+PassWord_Len];
						}
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);
						SPIFlash_Write_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len); //д��ڶ���������
						Ev_Cat_Dat(0x01,PassWord_Num-1);
						
						#ifdef uart_start
						SEGGER_RTT_printf(0,"PassWord_Clear!\r\n");
						#endif
				}
				else
				{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Not_Found_PassWord!\r\n");
				#endif
				}
			}
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Not_Found_PassWord!\r\n");
				#endif
			}
	}
}

/************************************************************************************************
��ȡ�������������ݣ�����ӡ���
���룺 Ҫ�����ӡ������ ÿ������ʹ��2k
������������2k������Ӻ����е��ã����򽫵���ϵͳ����
************************************************************************************************/
void read_Soctor(uint32_t sector)
{
		#ifdef uart_start
		uint32_t Block,Page;
		uint32_t i;
		for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0x00; //���ڶ�ȡ����
		Sector_to_Block_Page( &Block,&Page,sector );  //��ȡ�������ڿ��ҳ�׵�ַ
		
		SPIFlash_Read_Tab(tab_dat,Block,Page,sizeof(tab_dat));
		i =sector;
		SEGGER_RTT_printf (0,"sector %d:\r\n",i);
		nrf_delay_us(100);
		for(i =0;i<sizeof(tab_dat);i++)
		{
			if((i%14==0)&&(i!=0)&&((sector>=AD_TypeA_Data_Secotr1)&&(sector<(AD_TypeA_Data_Secotr1+AD_TypeA_Data_Sector_Num))))
				SEGGER_RTT_printf(0,"\r\n");
			else if((i%32==0)&&(i!=0)&&(((sector<AD_TypeA_Data_Secotr1)||(sector>=(AD_TypeA_Data_Secotr1+AD_TypeA_Data_Sector_Num)))))
				SEGGER_RTT_printf(0,"\r\n");
				
			
			SEGGER_RTT_printf(0,"%x ",tab_dat[i]);
			
			//nrf_delay_ms(3);
		}
	SEGGER_RTT_printf(0,"\r\n");
		#endif
}

/**********************************************************************************************
����ɾ������Ա����

***********************************************************************************************/
void Del_Admin_PassWord()
{
	uint32_t i,i1;
	uint16_t Admin_PassWord_Num;  //��ȡ���Ĺ���Ա������
	uint32_t len,len1;  					//���ݻ����������
	uint32_t Block,Sector,Page;
//	uint32_t PW_Same;  //��ͬ��������λ��
//	uint8_t *PassWord_R;
	uint32_t PassWord_Num;
	uint8_t tab1[2*A_Sector_PassWord_Num+1];
	Admin_PassWord_Num =0;
	PassWord_Num = Get_Cat_Dat(0x01); //��ȡ��ǰ�洢����������
	len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���
	uint8_t Sector1_Num;
	Sector1_Num =0;
	uint8_t *s;
	Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��ȡ��������
	SPIFlash_Read_Tab(tab1,Block,Page,sizeof(tab1));				  //��ȡ���������ݳ��ȵ���������
	
		if(PassWord_Num<=A_Sector_PassWord_Num)
		{
			len1 =len;  //���볤��С��1�ֽ�
		}
		else if((PassWord_Num>A_Sector_PassWord_Num)&&(PassWord_Num<=(2*A_Sector_PassWord_Num)))
		{
			len1 = A_Sector_PassWord_Num*PassWord_Len;
		}
		else 
		return; //�����������
		
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��һ������������
		SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //��ȡ���������ݳ��ȵ���������
		
		for(i =0;i<len1;)
		{
			if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
				&&(tab_dat[i+2]==Admin_PassWord)) //��ȡ������Ա�����
				{
					Admin_PassWord_Num++;  //ɾ���Ĺ���Ա����������һ
					for(i1 =i;i1<len1;i1++)  //��������һ�񣬸��ǵ�ǰλ��
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //�����жϵ�ǰ�����Ƿ�Ϊ����Ա����
					for(i1 =i/PassWord_Len;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //ǰ��PassWord_SN;
					}
				}
			else  //��ǰ���ݲ��ǹ���Ա����ʱ������һ�����볤��
				i+=PassWord_Len;
		}
		
		//���һ����ָ�������
		
		if(Admin_PassWord_Num>0)  //��������һ��������Ա���뷢���䶯ʱ
		{
			Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //��ȡ��������λ��
			SPIFlash_Erase_Sector(Block,Sector);   //��������
			SPI_Flash_Busy();
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //��ȡ��������λ��
			//len = len-(PassWord_Len*Admin_PassWord_Num);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //д���ַ��� ��������1
		}
		
		Sector1_Num = Admin_PassWord_Num; //���������������������
		Admin_PassWord_Num =0;

		if(PassWord_Num>A_Sector_PassWord_Num) //���ڶ�����������ʱ
		{
			len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //��ȡ���������ݳ��ȵ���������
			for(i =0;i<len1;)
			{
				if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
					&&(tab_dat[i+2]==Admin_PassWord))			//��ȡ������Ա�����
				{
					Admin_PassWord_Num++;  //ɾ���Ĺ���Ա����������һ
					for(i1 =i;i1<len1;i1++)  //��������һ�񣬸��ǵ�ǰλ��
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //�����жϵ�ǰ�����Ƿ�Ϊ����Ա����
					for(i1 =i/PassWord_Len+A_Sector_PassWord_Num;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //ǰ��PassWord_SN;
					}
				}
				else  //��ǰ���ݲ��ǹ���Ա����ʱ������һ�����볤��
					i+=PassWord_Len;
			}
			
			if(Admin_PassWord_Num>0)  //������2�������޸�ʱ����д������2
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //��ȡ��������λ��
				SPIFlash_Erase_Sector(Block,Sector);   //��������
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //��ȡ��������λ��
				//len = len-(PassWord_Len*Admin_PassWord_Num);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //д���ַ���
			}
		
			if(Sector1_Num>0) //��ʱAdmin_PassWord_Num Ϊ�ڶ�������ָ������ ����һ�������޸�ʱ
				{
					for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0xFF;
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��һ������������
					SPIFlash_Read_Tab(tab_dat,Block,Page,(A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);				  //��ȡ���������ݳ��ȵ���������
					
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
					
					s = &tab_dat[0];
					s+= (A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len;
					
					SPIFlash_Read_Tab(s,Block,Page,Sector1_Num*PassWord_Len);				  //��ȡ���������ݳ��ȵ���������
					
					Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //��ȡ��һ������������
					SPIFlash_Erase_Sector(Block,Sector);   //��������
					SPI_Flash_Busy();
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //��ȡ��������λ��
					//len = len-(PassWord_Len*Admin_PassWord_Num);
					SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //д��1�����ַ���
					
//					read_Soctor(PassWord_Secotr1);
//					NRF_WDT_Feed();
					i1 =0;
					if((A_Sector_PassWord_Num+Sector1_Num)<PassWord_Num) //���ڶ�����������������
					{
						len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
						SPIFlash_Read_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Admin_PassWord_Num)*PassWord_Len);				  //��ȡ���������ݳ��ȵ���������
						for(i =(Sector1_Num*PassWord_Len);i<((PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len);i++)
						{
							tab_dat[i1++] = tab_dat[i]; //ǰ������
						}
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //��ʼд�����ݵ�����2
						SPIFlash_Erase_Sector(Block,Sector);   //��������
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //��ȡ��������λ��
						//len = len-(PassWord_Len*Admin_PassWord_Num);
						SPIFlash_Write_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);  //д���ַ��� (��ʱiΪ��Ч���ݳ���
//					read_Soctor(PassWord_Secotr2);
//					NRF_WDT_Feed();
					}
					else //����ֱ�Ӳ����ڶ���������
					{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //��ʼд�����ݵ�����2
						SPIFlash_Erase_Sector(Block,Sector);   //��������
						SPI_Flash_Busy();
					}
				}
		}
		
		if((Admin_PassWord_Num>0)||(Sector1_Num>0))
		{
		//����޸ĺ�д��tab1����
		Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr);   //��ȡ��������λ��
		SPIFlash_Erase_Sector(Block,Sector);   //��������
		SPI_Flash_Busy();
		//len = len-(PassWord_Len*Admin_PassWord_Num);
								
		Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr);   //��ȡ��������λ��
		SPIFlash_Write_Tab(tab1,Block,Page,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));  //д���ַ��� ���������ű�
		SPI_Flash_Busy();
				
//		read_Soctor(4);
//		NRF_WDT_Feed();
		
		Ev_Cat_Dat(0x01,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));
		
		}
		Admin_PassWord_Num+=Sector1_Num;
		#ifdef uart_start
		SEGGER_RTT_printf(0,"CL_Admin_Num: %d!\r\n",Admin_PassWord_Num);
		#endif
		
		Del_A_Type_PS_Print(0X01);			//ɾ������Աָ��
}

/********************************************************************************************
����ɾ���û�����

*********************************************************************************************/
void Del_User_PassWord()
{
	uint32_t i,i1;
	uint16_t Admin_PassWord_Num;  //��ȡ���Ĺ���Ա������
	uint32_t len,len1;  					//���ݻ����������
	uint32_t Block,Sector,Page;
//	uint32_t PW_Same;  //��ͬ��������λ��
//	uint8_t *PassWord_R;
	uint32_t PassWord_Num;
	uint8_t tab1[2*A_Sector_PassWord_Num+1];
	Admin_PassWord_Num =0;
	PassWord_Num = Get_Cat_Dat(0x01); //��ȡ��ǰ�洢����������
	len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���
	uint8_t Sector1_Num;
	Sector1_Num =0;
	uint8_t *s;
	Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��ȡ��������
	SPIFlash_Read_Tab(tab1,Block,Page,sizeof(tab1));				  //��ȡ���������ݳ��ȵ���������
	
		if(PassWord_Num<=A_Sector_PassWord_Num)
		{
			len1 =len;  //���볤��С��1�ֽ�
		}
		else if((PassWord_Num>A_Sector_PassWord_Num)&&(PassWord_Num<=(2*A_Sector_PassWord_Num)))
		{
			len1 = A_Sector_PassWord_Num*PassWord_Len;
		}
		else 
		return; //�����������
		
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��һ������������
		SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //��ȡ���������ݳ��ȵ���������
		
		for(i =0;i<len1;)
		{
			if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
				&&(tab_dat[i+2]==User_PassWord)) //��ȡ������Ա�����
				{
					Admin_PassWord_Num++;  //ɾ���Ĺ���Ա����������һ
					for(i1 =i;i1<len1;i1++)  //��������һ�񣬸��ǵ�ǰλ��
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //�����жϵ�ǰ�����Ƿ�Ϊ����Ա����
					for(i1 =i/PassWord_Len;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //ǰ��PassWord_SN;
					}
				}
			else  //��ǰ���ݲ��ǹ���Ա����ʱ������һ�����볤��
				i+=PassWord_Len;
		}
		
		//���һ����ָ�������
		
		if(Admin_PassWord_Num>0)  //��������һ��������Ա���뷢���䶯ʱ
		{
			Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //��ȡ��������λ��
			SPIFlash_Erase_Sector(Block,Sector);   //��������
			SPI_Flash_Busy();
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //��ȡ��������λ��
			//len = len-(PassWord_Len*Admin_PassWord_Num);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //д���ַ��� ��������1
		}
		
		Sector1_Num = Admin_PassWord_Num; //���������������������
		Admin_PassWord_Num =0;

		if(PassWord_Num>A_Sector_PassWord_Num) //���ڶ�����������ʱ
		{
			len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //��ȡ���������ݳ��ȵ���������
			for(i =0;i<len1;)
			{
				if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
					&&(tab_dat[i+2]==User_PassWord))			//��ȡ������Ա�����
				{
					Admin_PassWord_Num++;  //ɾ���Ĺ���Ա����������һ
					for(i1 =i;i1<len1;i1++)  //��������һ�񣬸��ǵ�ǰλ��
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //�����жϵ�ǰ�����Ƿ�Ϊ����Ա����
					for(i1 =i/PassWord_Len+A_Sector_PassWord_Num;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //ǰ��PassWord_SN;
					}
				}
				else  //��ǰ���ݲ��ǹ���Ա����ʱ������һ�����볤��
					i+=PassWord_Len;
			}
			
			if(Admin_PassWord_Num>0)  //������2�������޸�ʱ����д������2
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //��ȡ��������λ��
				SPIFlash_Erase_Sector(Block,Sector);   //��������
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //��ȡ��������λ��
				//len = len-(PassWord_Len*Admin_PassWord_Num);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //д���ַ���
			}
		
			if(Sector1_Num>0) //��ʱAdmin_PassWord_Num Ϊ�ڶ�������ָ������ ����һ�������޸�ʱ
				{
					for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0xFF;
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��һ������������
					SPIFlash_Read_Tab(tab_dat,Block,Page,(A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);				  //��ȡ���������ݳ��ȵ���������
					
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
					
					s = &tab_dat[0];
					s+= (A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len;
					
					SPIFlash_Read_Tab(s,Block,Page,Sector1_Num*PassWord_Len);				  //��ȡ���������ݳ��ȵ���������
					
					Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //��ȡ��һ������������
					SPIFlash_Erase_Sector(Block,Sector);   //��������
					SPI_Flash_Busy();
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //��ȡ��������λ��
					//len = len-(PassWord_Len*Admin_PassWord_Num);
					SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //д��1�����ַ���
					
//					read_Soctor(PassWord_Secotr1);
//					NRF_WDT_Feed();
					i1 =0;
					if((A_Sector_PassWord_Num+Sector1_Num)<PassWord_Num) //���ڶ�����������������
					{
						len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
						SPIFlash_Read_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Admin_PassWord_Num)*PassWord_Len);				  //��ȡ���������ݳ��ȵ���������
						for(i =(Sector1_Num*PassWord_Len);i<((PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len);i++)
						{
							tab_dat[i1++] = tab_dat[i]; //ǰ������
						}
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //��ʼд�����ݵ�����2
						SPIFlash_Erase_Sector(Block,Sector);   //��������
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //��ȡ��������λ��
						//len = len-(PassWord_Len*Admin_PassWord_Num);
						SPIFlash_Write_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);  //д���ַ��� (��ʱiΪ��Ч���ݳ���
//					read_Soctor(PassWord_Secotr2);
//					NRF_WDT_Feed();
					}
					else //����ֱ�Ӳ����ڶ���������
					{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //��ʼд�����ݵ�����2
						SPIFlash_Erase_Sector(Block,Sector);   //��������
						SPI_Flash_Busy();
					}
				}
		}
		
		if((Admin_PassWord_Num>0)||(Sector1_Num>0))
		{
		//����޸ĺ�д��tab1����
		Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr);   //��ȡ��������λ��
		SPIFlash_Erase_Sector(Block,Sector);   //��������
		SPI_Flash_Busy();
		//len = len-(PassWord_Len*Admin_PassWord_Num);
								
		Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr);   //��ȡ��������λ��
		SPIFlash_Write_Tab(tab1,Block,Page,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));  //д���ַ��� ���������ű�
		SPI_Flash_Busy();
				
//		read_Soctor(4);
//		NRF_WDT_Feed();
		
		Ev_Cat_Dat(0x01,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));
		
		}
		Admin_PassWord_Num+=Sector1_Num;
		#ifdef uart_start
		SEGGER_RTT_printf(0,"CL_User_Num: %d!\r\n",Admin_PassWord_Num);
		#endif
		
		Del_A_Type_PS_Print(0X02);			//ɾ���û�ָ��
		
	//	Ev_Cat_Dat(0x04,0);						//ɾ�����ö�̬����

}


//				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //�������� PassWord_Secotr1
//				SPIFlash_Erase_Sector(Block,Sector);
//				SPI_Flash_Busy();
//				
//				
//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��������д������
//				SPIFlash_Write_Tab(tab_dat,Block,Page,len+PassWord_Len);
//				SPI_Flash_Busy();			

//				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //�������� PassWord_SN_Secotr
//				SPIFlash_Erase_Sector(Block,Sector);
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��������д������
//				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(0X01,PassWord_Num+1);  //����Ŀ¼��
//				


/*************************************************************
ɾ��һ������
���룺 ������
���أ� ��
**************************************************************/
//uint8_t Del_A_PassWord(uint8_t PassWord_SN)
//{
////	uint32_t len;  					//���ݻ����������
////	uint32_t Block,Sector,Page;
////	uint32_t PW_Same;  //��ͬ��������λ��
////	uint8_t *PassWord_R;
////	uint32_t PassWord_Num;
////	uint8_t i;
////	uint32_t it,k;
////	
////	PassWord_Num = Get_Cat_Dat(0x01); //��ȡ��ǰ�洢����������
////	len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���
////	
////		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��������
////		SPIFlash_Read_Tab(tab_dat,Block,Page,len);				  //��ȡ���������ݳ��ȵ���������

////	for(i =0;i<PassWord_Num;i++)
////	{
////		if(tab_dat[i*PassWord_Len+PassWord_Len-1]==PassWord_SN)
////		{
////			break;  //�ҵ���Ӧ����
////		}
////	} //��ʼ����ҵ�������
////	if(i<PassWord_Len) //�ҵ���������Ч
////	{
////		for(it =i*PassWord_Len;it<len;it++) //�滻����
////		{
////			tab_dat[it] = tab_dat[it+PassWord_Len];
////		}		
////		
////		#ifdef uart_start
////		SEGGER_RTT_printf(0,"PassWord_N: %d!\r\n",i);	//�ҵ��������
////		#endif
////		
////	Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //��ȡ��������λ��
////	SPIFlash_Erase_Sector(Block,Sector);   //��������
////	SPI_Flash_Busy();
////	Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //��ȡ��������λ��
////		len = len-PassWord_Len;  //��ȥһ������
////	SPIFlash_Write_Tab(tab_dat,Block,Page,len);  //д���ַ���
////	Ev_Cat_Dat(0x01,PassWord_Num-1);  //Ŀ¼ֵ��һ
////		return 1;
////	}
////	return 0;

//}


/********************************
����SN��ɾ��һ������

���룺 ��Ҫɾ�������SN��
����� �Ƿ�ɹ�ɾ������

********************************/
uint8_t Del_SN_PassWord(uint8_t PW_SN)
{
	static uint8_t PassWord_W[PassWord_Len];
	uint8_t tab1[2*A_Sector_PassWord_Num];
	uint32_t i,it,i1,i2,i3;
	uint32_t len1; //�������ݵ�ǰ�ܳ���
	uint8_t PassWord_Num;  //��������
	uint32_t Block,Sector,Page;  //����д��/��ȡ ��������
//	uint32_t PW_Same;  //��ͬ��������λ��
//	uint8_t  *PassWord_R; //��ȡ��������ָ��
//	uint8_t PassWord_SN;  //��ʾ������
	uint8_t PassWord_Secotr_Num;  //����Ҫ�޸ĵ������ţ�0��ʾ�ޣ�1��ʾ��������1,2��ʾ��������2
	
  PassWord_Secotr_Num = 0;  //�������������ָ��
	i3 =0;
	for(i =0;i<sizeof(tab1);i++)
	{
		
		tab1[i] =0x00; 
		
	}
	
	for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0;
	
	PassWord_Num = Get_Cat_Dat(0x01); //��ȡ��ǰ�洢����������
//	len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���
	
	if(PassWord_Num>0) //���д洢������ʱ
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��ȡ��������������
		SPIFlash_Read_Tab(tab1,Block,Page,PassWord_Num);  //��ȡ������������������������	
	}
	else return 0;  //û������ʱֱ�ӷ��ز���ʧ��
	
	for(i =0;i<PassWord_Num;i++)
	{
		if(tab1[i]==PW_SN)//�ж϶�Ӧֵ�Ƿ�������һ��
		{
			break;  //����ѭ��
		}
	}
	
	#ifdef uart_start
	SEGGER_RTT_printf(0,"\r\ni: %d!\r\n",i);	//�ҵ��������
	#endif
	//iΪ�������ڵ�����
	if(i>=PassWord_Num)
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Not_PassWord!\r\n");
		#endif
		return 0;  //SN����Ч�򷵻� 0
	}
	if(i<(2*A_Sector_PassWord_Num)) //��������������������
	{
		if(i<A_Sector_PassWord_Num)  //�ڵ�һ����
		{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);
		PassWord_Secotr_Num =1;
		i3 =i;
		}
		else  //�ڵڶ�����
		{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);
			PassWord_Secotr_Num =2; //ָ������
		i3 =i-A_Sector_PassWord_Num;
		}
		//i*=PassWord_Len;  //�����Ӧλ��
//		if(i<A_Sector_PassWord_Num)  // ȷ������λ�ú󣬻���������λ��
//			i3 =i;
//		else if((i>=A_Sector_PassWord_Num)&&(i<(2*A_Sector_PassWord_Num)))//�ڶ�������
//			//
//			i3 =i-A_Sector_PassWord_Num;
//		else
//			return 0;
//		
//		if(i3<A_Sector_PassWord_Num); //�ж�������Ч��
//		else return 0;
		
		if(tab_dat[i3*PassWord_Len+17]==tab1[i])  //����100��ֵ (ֵ��0-99)
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"PassWord_SN_Cheak ok!\r\n");	//�ҵ��������
			it = tab1[i];
			SEGGER_RTT_printf(0,"PassWord:%d\r\n",it);	//�ҵ��������
			
			#endif

				if((PassWord_Num<=2*A_Sector_PassWord_Num)&&(PassWord_Secotr_Num==1)) //��ֵС����������ʱ (2*A_Sector_PassWord_Num) ��������һ������
				{
	//				len1 =len;
	//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ����1
	//				SPIFlash_Read_Tab(tab_dat,Block,Page,len);
					len1 =A_Sector_PassWord_Num*PassWord_Len;
				}
				else if((PassWord_Num<=2*A_Sector_PassWord_Num)&&(PassWord_Secotr_Num==2)) //��ֵС����������ʱ (2*A_Sector_PassWord_Num) �������ڶ�������
				{
	//				len1 = A_Sector_PassWord_Num*PassWord_Len;//��ֵ֮Ϊ����������С
	//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ����2
	//				SPIFlash_Read_Tab(tab_dat,Block,Page,len-(A_Sector_PassWord_Num*PassWord_Len));
					len1 = A_Sector_PassWord_Num*PassWord_Len;  //len1ֵΪ�ڶ������������ܳ���
				}
				
				for(it =i3*PassWord_Len;it<len1;it++)  //ǰ����������
				{
					tab_dat[it]	=tab_dat[it+PassWord_Len];//�滻���λ��
				}
				
				for(it =i;it<(PassWord_Num-1);it++) //������������
				{
					tab1[it]=tab1[it+1];
				}
				
				if(PassWord_Secotr_Num==1) //�Ķ�һ����
				{
					if(PassWord_Num>A_Sector_PassWord_Num) //��������������1����������ʱ ��ȡ����������һ������ǰ��
					{
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ����2
						SPIFlash_Read_Tab(PassWord_W,Block,Page,PassWord_Len); //������2��һ������������ȡ��PassWord_Wλ��
						if((PassWord_W[0]==0x55)&&(PassWord_W[1]==0xAA))  //����ȡ������ʱ
						{
							i2 =0;
								for(i1 =(A_Sector_PassWord_Num-1)*PassWord_Len;i1<(A_Sector_PassWord_Num*PassWord_Len);i1++) //�����һ�����������
								{
									tab_dat[i1] =PassWord_W[i2++];  //������д��tab_dat
								}
								Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);
								SPIFlash_Erase_Sector(Block,Sector);
								SPI_Flash_Busy();
								Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
								SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len); //д���һ��������		
							
								Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ����2  //�޸ĵڶ���������
								SPIFlash_Read_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len); //������2����������ȡ��tab_datλ��	
								
								for(it =0;it<(PassWord_Num-A_Sector_PassWord_Num-1)*PassWord_Len;it++)
								{
									tab_dat[it] = tab_dat[it+PassWord_Len];  //�ڶ�������������һλ
								}
								if(it>=PassWord_Len) //���ڶ�������������ʱ
								{
									Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);
									SPIFlash_Erase_Sector(Block,Sector);
									SPI_Flash_Busy();
									Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);
									SPIFlash_Write_Tab(tab_dat,Block,Page,it); //д��ڶ���������		
								}
							}
						}
						else if(PassWord_Num<=A_Sector_PassWord_Num) //������������һ������������ʱ������ǰ��
						{
								Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //��ֱ֮�����һ��������
								SPIFlash_Erase_Sector(Block,Sector);
								SPI_Flash_Busy();
								Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
								SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len); //д���һ��������		
						}
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,len1);  //д���ַ���				
						Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //�������� PassWord_SN_Secotr
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
							
						Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��������д������
						SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num-1);
						SPI_Flash_Busy();
						Ev_Cat_Dat(0x01,PassWord_Num-1);  //Ŀ¼ֵ��һ
						
						return 1;
						
					}
					else if(PassWord_Secotr_Num==2)//�Ķ��˶�����
					{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //��ȡ��������λ��
						SPIFlash_Erase_Sector(Block,Sector);   //��������
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //��ȡ��������λ��
						len1 = len1-PassWord_Len;  //��ȥһ������������
						
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,len1);  //д���ַ���				
						Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //�������� PassWord_SN_Secotr
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
							
						Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //��������д������
						SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num-1);
						SPI_Flash_Busy();
						Ev_Cat_Dat(0x01,PassWord_Num-1);  //Ŀ¼ֵ��һ
						
						return 1;	
					}
					return 0;
		}
		else
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"PassWord_SN_Cheak_Error!\r\n");
			#endif
			return 0;
		}
	}
	return 0;
}


/*****************************************
����һ�����ż�¼

���룺 SN_Num: SN�� �ֻ�����Ϊ0

���룺 Key_type  //���ŷ�ʽ(����) �ֻ�����Ϊ0

���룺 Unix_t 	ʱ���ֵ

ָ�� 0x01 ����0x02  IC��0x03 ���֤ 0x04

*****************************************/
/*
void ADD_Op_Door_Note(uint8_t SN_Num,uint8_t Key_type,uint32_t Unix_t)  //���뿪�ż�¼ (����ʱ����)
{
		uint32_t len,len1,Note_Num;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page,Sector; 
		uint32_t it,i;
		uint8_t tab[Open_Note_Num_Len];
		
		Note_Num = Get_Cat_Dat(OpenDoorNote_Num_Code);  //��ȡ����
		len = Open_Note_Num_Len*Note_Num;
				
			#ifdef uart_start
		SEGGER_RTT_printf(0,"\r\nNote_Num: %d\r\n",Note_Num);
			#endif	
	
	
		for(i =0;i<sizeof(tab);i++)
		tab[i] =0x00;

		tab[0] =0x55;
		tab[1] =0xAA;
		tab[2] =SN_Num;
		tab[3] =Key_type;
		tab[4] =(Unix_t>>24)&0xFF;
		tab[5] =(Unix_t>>16)&0xFF;
		tab[6] =(Unix_t>>8)&0xFF;
		tab[7] =Unix_t&0xFF;
		
	  if(Note_Num<Open_Note_Size) //��ֵ�ڿ��ż�¼������ʱ
		{
			it = Note_Num/A_Sector_Open_Note_Size; //��ȡ������������
			len1= len-it*A_Sector_Open_Note_Size*Open_Note_Num_Len; //ת��Ϊһ�����ڳ��� ��Ϊ��ʼ��ַ
			for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it); //��ȡ��������������
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);  //��ȡ���������ݳ��ȵ���������	
			
			for(i =0;i<Open_Note_Num_Len;i++)  //����һ��������
			{
				tab_dat[i+len1] = tab[i];
			}
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1+it);  //�Ȳ�������
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1+A_Sector_Open_Note_Size); //д������������
				SPI_Flash_Busy();	
			
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);
		
//			read_Soctor(1);
//			NRF_WDT_Feed();
//			read_Soctor(500);
//			NRF_WDT_Feed();
			
			//��������
		}
		else //�����ż�¼ֵ���ڵ����������ʱ
		{
			Note_Num = Open_Note_Size - A_Sector_Open_Note_Size; //ֱ��ɾ�������һ������ ��ʼ�����µ�����
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1);  //�Ȳ�����������
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();		
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //��ȡ��������
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //д��2�������ݵ�1����
				SPI_Flash_Busy();	
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr2);  //�Ȳ���2��������
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //��ȡ��������
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //д��2�������ݵ�1����
				SPI_Flash_Busy();	
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr3);  //�Ȳ���3��������
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //��ȡ4��������
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //д��4�������ݵ�3 ����
				SPI_Flash_Busy();	
			//������������һ������
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr4);  //����4���� д��һ������
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Write_Tab(tab,Block,Page,Open_Note_Num_Len); //д�������ݵ�4����
				SPI_Flash_Busy();	
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);		//��������
		}
}

*/

/*****************************************
����һ�����ż�¼ ���°棩
20201123
���ż�¼��ʽ��
1�ֽ�����+2�ֽ�Կ��ID+1�ֽ�Կ�����鳤��+16�ֽ�Կ������+4�ֽ�ʱ��
����24�ֽ� 
���ͣ�
0�����뿪��
1��ָ�ƿ���
2���Զ��忨����
3�����ݿ�
5���ֻ�����
���룺
0+Կ��ID+���볤��+��������(6-10λ) +ʱ��
����
0x00+��������
0x00+0x01+Կ��ID
0x0A+���볤��
0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X00+��������
0X5E+0xAC+0XAD+0XDE			//ʱ��

ADD_Op_Door_Note :
���룺  
SN_Num: ����
Key_type: Կ��ID��
Unix_t�� ����ʱ��ʱ���


*****************************************/

u8 Op_Door_Buff[Open_Note_Num_Len];				//���ڻ��濪�ż�¼���� ��д�뻺��

void ADD_Op_Door_Note(uint16_t SN_Num,uint16_t Key_type,uint32_t Unix_t)  //���뿪�ż�¼ (����ʱ����)
{
	
		uint32_t len,len1,Note_Num;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page,Sector; 
		uint32_t it,i;
		Note_Num = Get_Cat_Dat(OpenDoorNote_Num_Code);  //��ȡ��ǰ���������
		len = Open_Note_Num_Len*Note_Num;
		it = Key_type;
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nNote_Num: %x\r\n",Note_Num);
	
		SEGGER_RTT_printf(0,"\r\nKey_type: %x\r\n",it);
		
		SEGGER_RTT_printf(0,"\r\nOp_Door_Buff: \r\n");
		
	
		for(i =0;i<Open_Note_Num_Len;i++)
		{
			SEGGER_RTT_printf(0,"%x",Op_Door_Buff[i]);
		}
		
		SEGGER_RTT_printf(0,"\r\n");
		
		#endif
		Get_Time(); //��ȡ����ʱ��
		Unix_t =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���		���ż�¼ʵ��ֱ��ʹ�õ���ʱ��
	
		Op_Door_Buff[0] =0xAA;
		
		Op_Door_Buff[1] =SN_Num>>8&0xFF;
		Op_Door_Buff[2] =SN_Num&0xFF;
		

		Op_Door_Buff[3] =Key_type>>8&0xFF;
		Op_Door_Buff[4] =Key_type&0xFF;
		

		Op_Door_Buff[22] =(Unix_t>>24)&0xFF;
		Op_Door_Buff[23] =(Unix_t>>16)&0xFF;
		Op_Door_Buff[24] =(Unix_t>>8)&0xFF;
		Op_Door_Buff[25] =Unix_t&0xFF;
		
		if(Note_Num<Open_Note_Size) //��ֵ�ڿ��ż�¼������ʱ
		{
			
			it = Note_Num/A_Sector_Open_Note_Size; //��ȡ������������
			
			len1= len-it*A_Sector_Open_Note_Size*Open_Note_Num_Len; //ת��Ϊһ�����ڳ��� ��Ϊ��ʼ��ַ
			
			for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it); //��ȡ��������������
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);  //��ȡ���������ݳ��ȵ���������	
			
			for(i =0;i<Open_Note_Num_Len;i++)  //����һ��������
			{
				
				tab_dat[i+len1] = Op_Door_Buff[i];
				
			}
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1+it);  //�Ȳ�������
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1+A_Sector_Open_Note_Size); //д������������
				SPI_Flash_Busy();	
			
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);
			
	//		read_Soctor(1);
	//		NRF_WDT_Feed();
//			read_Soctor(500);
//			NRF_WDT_Feed();
			
			//��������
			
		}
		else //�����ż�¼ֵ���ڵ����������ʱ
		{
			Note_Num = Open_Note_Size - A_Sector_Open_Note_Size; //ֱ��ɾ�������һ������ ��ʼ�����µ�����
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1);  //�Ȳ�����������
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();		
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //�ȶ�ȡ2��������
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //��д��2�������ݵ�1����
				SPI_Flash_Busy();	
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr2);  //����2��������
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //�ȶ�ȡ3��������
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //�ٽ�3��������д��2����
				SPI_Flash_Busy();	
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr3);  //����3��������
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //�ȶ�ȡ4��������
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //�ٽ�4��������д�뵽3����
				SPI_Flash_Busy();	
			//������������һ������
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr4);  //����4���� ����д��������
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Write_Tab(Op_Door_Buff,Block,Page,Open_Note_Num_Len); //д�������ݵ�4����
				SPI_Flash_Busy();	
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);		//����������
		}
		
		for(i =0;i<Open_Note_Num_Len;i++)			//д����ɺ��������
		{
			Op_Door_Buff[i] =0;
		}
		
	//		read_Soctor(Open_Note_Secotr1);
		//	NRF_WDT_Feed();
		
}


/***************************************

��ȡ���ż�¼ֵ���ڷ���

Read_Op_Note()

���أ� 1�� ��ȡ��Ч  0�� ��ȡ��Ч


***************************************/

uint8_t Read_Op_Tab[Open_Note_Num_Len];

uint8_t Read_Op_Note(uint16_t Read_Num)
{	
		uint32_t Num,len1;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page;
		uint32_t it,i;
		
		Spi_Flash_Init();  //����SPI_FLASH ͨѶ
	
		Num = Get_Cat_Dat(OpenDoorNote_Num_Code);				//��ȡ���ر���Ŀ��ż�¼����
			
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Read_Op_Note!\r\n");
		SEGGER_RTT_printf(0,"Read_Num: %d.\r\n",Read_Num);
		SEGGER_RTT_printf(0,"Num: %d.\r\n",Num);
		
		#endif
	
		if(Read_Num>Num)			//�������Ŀ��>�����������ʱ ����0 ����
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Read_Num_Error!\r\n");				//��ʾ��ȡ�������
			#endif
			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
			
			return 0;
		}
		
		for(i =0;i<sizeof(Read_Op_Tab);i++) Read_Op_Tab[i] =0x00;
		
		it = Read_Num/A_Sector_Open_Note_Size; //��������
		len1 = (Read_Num-it*A_Sector_Open_Note_Size)*Open_Note_Num_Len;  //������Ҫ��ȡ�ĳ���
		//��ʼ��ȡ����
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0x00;
		
		Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it); //��ȡ��������������
		SPIFlash_Read_Tab(tab_dat,Block,Page,len1);  //��ȡ���������ݳ��ȵ���������	
		
		for(i =0;i<sizeof(Read_Op_Tab);i++)
			Read_Op_Tab[i] = tab_dat[len1-Open_Note_Num_Len+i];
		
		
		 
		 
	   SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		 Spi_Flash_Sleep();
		 
		return 1;				//������Ч ����1 

}




/**********************************
���涯̬����

���룺Act_Type   ��̬�������� 
���룺*Act_PassWord ��Ҫ����Ķ�̬����
���룺Act_PassWord_Len ��̬���볤�� (������ͨ��̬����)
���룺past_unix_t  ��ͨ��̬�����ʧЧʱ�� ������������Ϊ0XFFFFFFFF
(
һ���������ͨ����
��ͨ���� 02 һ������01 
(��ͨ������Զֻռ�� ��̬����洢����0 λ�� ռ��16�ֽ� һ������ռ��֮��ռ䣬ÿ��ռ��9�ֽ� ���� 216��һ�������1����ͨ����Ŀռ�)
)
����ʱ�� (ʱ���)���������ʵ�ʹ���ʱ��� (����ǰ����������������ʱ���)

���أ���
**********************************/

const uint8_t Act_PassWord_SLen =20;  // �洢��̬���뻺�泤��

const uint8_t Common_Act_Len = 15;  // ��ͨ��̬���볤�� (0xAA+���볤��(6-9)+���루6-9λ��+����ʱ��(4�ֽ�))

const uint8_t Once_Act_Len  = 9;  //һ�����볤�� (0XAA+����(�̶�8λ)) 

const uint8_t Once_PassWord_Size = 216;  //�����ܴ洢24Сʱ�����ж�̬���� (ÿСʱ9����һ��24Сʱ ���� 9*24 =216��)

#define  Once_Act_Type     1  // ����һ�ζ�̬��������
#define  Common_Act_Type   2  // ������ͨ��̬��������

void Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t)
{
		
		uint32_t len;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page,Sector;
		uint32_t i;
			
		uint32_t Act_PassWord_Num;
	
		uint8_t tab[Act_PassWord_SLen]; //��̬���뻺��
		
		for(i =0;i<Act_PassWord_Len;i++)
		{
			
			tab[i] = *Act_PassWord;
			Act_PassWord++;
			
		}
		
//		Del_User_Key();			//���涯̬����ǰ��ɾ���û�Կ��
		
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;  //��ջ���
		
		Act_PassWord_Num = Get_Cat_Dat(0x02);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (������һ�ζ�̬���룬��ͨ��̬����ֱ�Ӵ���ڸ�������ʼ16�ֽ�����)
		
		len = (Act_PassWord_Num*Once_Act_Len)+Common_Act_Len;  //���㳤��  //��һλ���ڳ����滻��lenֵֻ���� Once_Act_Len ��С
	
		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
	
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������
		
		switch (Act_Type)				//���� һ�������ʱЧ����
		{
				case Once_Act_Type :  //������Ķ�̬������һ�ζ�̬����ʱ
				
				for (i = len;i>=(Common_Act_Len+Once_Act_Len);i--) //��һ�����µ������������б����ʼ�����һ�����µ���������ͨ��̬���������һ��һ�������
				{
					
					tab_dat[i] = tab_dat[i-Once_Act_Len];  //һ��������������(�����ͨ����)����������ʱ�����һ��һ������ᱻ����
				  
				}
				
				tab_dat[Common_Act_Len] = 0xAA;
				
				for(i =(Common_Act_Len+1);i<(Common_Act_Len+Once_Act_Len);i++) //��������д��
				{
					tab_dat[i] = tab[i-Common_Act_Len-1];
				}
				
				Act_PassWord_Num++;  //һ����������+1;
				
				if(Act_PassWord_Num>=Once_PassWord_Size)  //�����������������ֵʱ�������������������ֵ
					Act_PassWord_Num = Once_PassWord_Size;
				
				Ev_Cat_Dat(0x02,Act_PassWord_Num);  //����һ����������
				
				len = Act_PassWord_Num * Once_Act_Len+Common_Act_Len;  //������Ҫ����������ܳ���
				//���ݲ�����ɺ󣬿�ʼд���µ�����
				Sector_to_Block_Sector(&Block,&Sector,Act_PassWord_Secotr);  //������̬������������
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();	
				
				Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬��������
				SPI_Flash_Busy();	
						
				break;
				
				case Common_Act_Type :  //���������������ͨ��̬����ʱֱ���滻֮ǰ���� (��ͨ��̬���벻����)
				
//				tab_dat[0] = 0XAA;  //����ͷ
//				tab_dat[1] = Act_PassWord_Len;
//				if((Act_PassWord_Len>=6)&&(Act_PassWord_Len<=9))
//				{
//		 			for (i =2;i<=(Act_PassWord_Len+2);i++) //��������ֵ �����������볤����ȷ����������
//					{
//						 tab_dat[i] = tab[i-2];
//					}
//				}
//				
//				tab_dat[11] = past_unix_t>>24&0xff;
//				tab_dat[12] = past_unix_t>>16&0xff;
//				tab_dat[13] = past_unix_t>>8&0xff;
//				tab_dat[14] = past_unix_t&0xff;
				More_Act_PassWord_Save(tab,Act_PassWord_Len,past_unix_t);
				break;
				
				default :
					
					break;
		}
	

		//��̬����д�����
		
		//��ʱЧ����д����鶯̬������
	//	if(Act_Type==Common_Act_Type) //�ж��Ƿ�ΪʱЧ����
		//	More_Act_PassWord_Save(tab,Act_PassWord_Len,past_unix_t);
}

/*****************************************************************************

�������Ķ�̬����(ͬʱ������鶯̬������Ч)

��̬�����־

���룺 

*Act_PassWord ����ָ��  
Act_PassWord_Len ���볤��  
past_unix_t ��Чʱ��

��̬���� ��ʽ

0XAA + ��̬���볤��+ 10λ��̬����ռ� + 4λ��Чʱ��

******************************************************************************/
void More_Act_PassWord_Save(uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t)
{
		
		uint32_t len;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page,Sector;
		uint32_t i;
		uint32_t More_Act_PassWord_Max_Num;					//���α���������������
	
		uint32_t More_Act_PassWord_Num;
	
		uint8_t tab[Act_PassWord_SLen]; //��̬���뻺��
		uint8_t ir=0;  //ѭ������
	
	
			#ifdef uart_start
		
			SEGGER_RTT_printf(0,"More_Act_PassWord_Save!\r\n");
		
			#endif
	
		for(i =0;i<Act_PassWord_Len;i++)				//����̬����д�뻺��Ԥ��д��
		{
			tab[i] = *Act_PassWord;
			Act_PassWord++;
		}
		
		for(i =0;i<sizeof(tab_dat);i++)
			tab_dat[i] =0xFF;  //��ջ���
		
		More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (���鶯̬���뱣���ר������)
		
		if(More_Act_PassWord_Num&0xffff0000)   //����ȡ������ֵΪ�����̬�����״̬
		{
			
			More_Act_PassWord_Max_Num =More_Act_PassWord_MAX_Num;			//���ʱЧ��̬������Ч���������More_Act_PassWord_Max_Num ΪԤ�����ֵ
			More_Act_PassWord_Num&=0xFFFF;				//ת��Ϊ��ȷ��ֵ
		}
		else				//��֮���ǵ�����̬������Ч   ��ʼ����ֵΪ0 Ĭ�ϵ�����Ч
		{
			
			More_Act_PassWord_Max_Num = 1;
		//	More_Act_PassWord_Num&=0XFFFF;
			
		}
		
		//More_Act_PassWord_Secotr
		
		len = More_Act_PassWord_Num * More_Act_PassWord_Len;
		
		Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
	
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������
		//�ж��Ƿ�����ͬ����
		
		for(uint8_t it=0;it<More_Act_PassWord_Num;it++)  //����ж��Ƿ���һ������
		{
			
			//���жϳ����Ƿ�һ��
			if(tab_dat[it*More_Act_PassWord_Len+1]==Act_PassWord_Len)    //�жϳ����Ƿ�һ��
			{
				//����һ�µ��ж��Ƿ���һ������
				ir =0;			//����ռ���
				for(uint8_t is=0;is<Act_PassWord_Len;is++)
				{
					
					if(
							tab_dat[it*More_Act_PassWord_Len+2+is] ==tab[is]
						)
						ir++;
					
				}
				if(ir==Act_PassWord_Len)   //�ҵ�һ�����ݣ�����ʱ�䲻�޸�����
				{
					//����ͬ����������Act_PassWord_Len  ���޸�ʱ��
					
					tab_dat[it*More_Act_PassWord_Len+13] = past_unix_t>>24&0xff;
					tab_dat[it*More_Act_PassWord_Len+14] = past_unix_t>>16&0xff;
					tab_dat[it*More_Act_PassWord_Len+15] = past_unix_t>>8&0xff;
					tab_dat[it*More_Act_PassWord_Len+16] = past_unix_t&0xff;
								
							//���ݲ�����ɺ󣬿�ʼд���µ�����
					Sector_to_Block_Sector(&Block,&Sector,More_Act_PassWord_Secotr);  //�������鶯̬������������
					SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
					
					Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);
					SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�Ķ������ݵ���̬��������
					SPI_Flash_Busy();	          
					
					read_Soctor(More_Act_PassWord_Secotr);				//�����������
				
					#ifdef uart_start
				
					SEGGER_RTT_printf(0,"UP Dat_Time! Num: %x!\r\n",it);
				
					#endif
					
					return;   //�޸�ʱ���ֱ�ӽ�������
					
				}
				
			}
		}
		
		
		//û����ͬ����ʱ��������д�����
		
		tab_dat[len] = 0XAA;		//��������ͷ
		tab_dat[len+1] = Act_PassWord_Len;		//���볤��
		
		for(i =0;i<Act_PassWord_Len;i++)			//�����µĶ�̬����
		{
			
			tab_dat[i+len+2] = tab[i];	
		
		}
		
		tab_dat[len+13] = past_unix_t>>24&0xff;
		tab_dat[len+14] = past_unix_t>>16&0xff;
		tab_dat[len+15] = past_unix_t>>8&0xff;
		tab_dat[len+16] = past_unix_t&0xff;
	
		if(More_Act_PassWord_Num>=More_Act_PassWord_Max_Num)   //��ʱЧ��̬����������ֵ ��������д�������
		{
			for (i =0;i<(More_Act_PassWord_Max_Num*More_Act_PassWord_Len);i++)				//����Ǩ��һ��
			{
				tab_dat[i] = tab_dat[i+More_Act_PassWord_Len];
			}
			More_Act_PassWord_Num=More_Act_PassWord_Max_Num;   //����ʼ�ձ������ֵ
		}
		else
			More_Act_PassWord_Num++;			//��֮ ����+1
	
			len = More_Act_PassWord_Num * More_Act_PassWord_Len;  //������Ҫ����������ܳ���
		
		//���ݲ�����ɺ󣬿�ʼд���µ�����
		Sector_to_Block_Sector(&Block,&Sector,More_Act_PassWord_Secotr);  //�������鶯̬������������
		SPIFlash_Erase_Sector(Block,Sector);
		SPI_Flash_Busy();	
		
		Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�Ķ������ݵ���̬��������
		SPI_Flash_Busy();	                                   
		
		if(
				More_Act_PassWord_Max_Num ==More_Act_PassWord_MAX_Num
			)//�����������������ʱ������Ԥ��ֵ&0XFFFF0000
		{
			Ev_Cat_Dat(More_Act_PassWord_Num_Code,More_Act_PassWord_Num|0XFFFF0000);  //���¶��鶯̬��������
		}
		else
		Ev_Cat_Dat(More_Act_PassWord_Num_Code,1);  //���µ��鶯̬��������
		
		read_Soctor(More_Act_PassWord_Secotr);				//�����������
		
}



//Per_Act_PassWord_Secotr
/**************************************************************************
�������ö�̬����

���ö�̬��������  20��

�����ʽ��  0X55 + 8λ����

**************************************************************************/

#define Per_Act_Len 9					//1�� �������볤��
#define Per_Act_Num 20				// ������������

void Per_Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t)
{
		uint32_t len;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page,Sector;
		uint32_t i;
		uint8_t Per_Act_PassWord_Num =0;
		
		uint8_t tab[Act_PassWord_SLen]; //��̬���뻺��
		
		#ifdef uart_start
	
		SEGGER_RTT_printf(0,"\r\nStart_Per_Act_PassWord_Save!\r\n");
	
		#endif
	
		tab[0] =0x55;			//����ͷ
		
		for(i =1;i<Per_Act_Len;i++)			//���������뻺��������������³��ȱ���Ϊ8��
		{
			
			tab[i] = *Act_PassWord;
			Act_PassWord++;
			
		}
		

		
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;  //��ջ���

		Per_Act_PassWord_Num = Get_Cat_Dat(0x04);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (������һ�ζ�̬���룬��ͨ��̬����ֱ�Ӵ���ڸ�������ʼ16�ֽ�����)
		
		len = Per_Act_PassWord_Num*Per_Act_Len;  //���㳤��  //��һλ���ڳ����滻��lenֵֻ���� Once_Act_Len ��С
	
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
	
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Per_Act_PassWord_Num: %x!\r\n",Per_Act_PassWord_Num);
		
		#endif
		
		
		if(Per_Act_PassWord_Num<Per_Act_Num)					//����������������Χ��ʱ
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"NEED_A_Per_Act_PassWord!\r\n");
			
			#endif
			
			for(i =0;i<Per_Act_Len;i++)			//�����������뻺��
			{
				
				tab_dat[len+i] = tab[i];
				
			}
			
			Per_Act_PassWord_Num++;					//�������� + 1
			
			Ev_Cat_Dat(0x04,Per_Act_PassWord_Num);  //����������������
			
			len+=Per_Act_Len;			//���� + һ������
			
			//���ݲ�����ɺ󣬿�ʼд���µ�����
		Sector_to_Block_Sector(&Block,&Sector,Per_Act_PassWord_Secotr);  //������̬������������
		SPIFlash_Erase_Sector(Block,Sector);
		SPI_Flash_Busy();	
		
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬��������
		SPI_Flash_Busy();	
			
		}
		else				//����������������ʱ ����ǰ��һ���׵���һ�������д���µ�
		{
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"REP_A_Per_Act_PassWord!\r\n");
			
			#endif
			
			
			for(i =0;i<len;i++)
		
			{
				
				tab_dat[i] = tab_dat[i+Per_Act_Len];
			
			}
			
			for(i =0;i<Per_Act_Len;i++)					//���µ�����ŵ����
			{
				
				tab_dat[Per_Act_Len*Per_Act_Num-Per_Act_Len+i] =tab[i]; 
				
			}
			
			//�滻��Ӱ���������Բ���Ҫ�������� д�����ݳ���Ҳ���ı�
			
					//���ݲ�����ɺ󣬿�ʼд���µ�����
		Sector_to_Block_Sector(&Block,&Sector,Per_Act_PassWord_Secotr);  //������̬������������
		SPIFlash_Erase_Sector(Block,Sector);
		SPI_Flash_Busy();	
		
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬��������
		SPI_Flash_Busy();	
			
		}
		
	#ifdef uart_start

		SEGGER_RTT_printf(0,"Per_Act_PassWord: \r\n");
		
		for(i =0;i<len;i++)
		{
			SEGGER_RTT_printf(0,"%x,",tab_dat[i]);
			if((i%16)==0)
			{
				SEGGER_RTT_printf(0,"\r\n");
			}
		}
				SEGGER_RTT_printf(0,"\r\n");

	#endif
		
	
}


/******************************************************************************************

��̬������֤���� (֧����λ����)

���ܣ��ж����������Ƿ��뱣�����ͨ��̬����һ�£���ȷ�������Ƿ��ѹ���


���룺 PassWord_Len  ��������볤��
���룺 PW_Unix_T 		 ��ǰ����ʱ��


******************************************************************************************/

extern uint8_t Key_Num_Tab[];

//extern u8 Op_Door_Buff[Open_Note_Num_Len];				//���ڻ��濪�ż�¼���� ��д�뻺�� �����Ѷ���


uint8_t  Act_PassWord_rese =0;   //���������������뱾������������ʱ����1��֮�󽫲��������������ж�̬��������

uint8_t Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T)
{
	 
//		uint32_t len;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page;	 //��ȡFlash�����������
		uint32_t it,it1,i,is,is1;
		uint32_t Act_Unix_T; //�������Ч��̬�������Чʱ��
	//	uint8_t tab[10];  //��̬����Ƚϻ���
		
		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
		SPIFlash_Read_Tab(tab_dat,Block,Page,Common_Act_Len);  //��ȡ�������������ȳ�����������

		if(tab_dat[0]!=0xAA)  //��ͷ����������ʱ�����ض�̬������Ч��ֱ�ӽ�������
			return 0;
		
		it = tab_dat[1];  //�����볤��ֵ���뻺��
		
		if((it<6)||(it>9)) // �����볤��ֵ���Ϸ�ʱ�����ض�̬������Ч��ֱ�ӽ�������
			return 0;
		
		if(PassWord_Len<6)  //������ĳ��Ȳ����������������Ч��ֱ�ӽ�������
			return 0;
//		if(PassWord_Len>9)   //���ų���λ����
//			return 0;
		
		Act_Unix_T = tab_dat[11];  //����ʱ��� 24-31λ
		Act_Unix_T <<= 8;
		
		Act_Unix_T |= tab_dat[12];  //����ʱ��� 16-23λ
		Act_Unix_T <<=8;

		Act_Unix_T |= tab_dat[13];  //����ʱ��� 8-15λ
		Act_Unix_T <<=8;
		
		Act_Unix_T |= tab_dat[14];  //����ʱ��� 0-7λ
		
		it =0;  //���ڱ�־�����Ƿ���ȫһ�£��в�һ�µĵط��Ȳ�Ϊ0���������ѯ����������Ч
		it1 =0;  //���ڱ�ʶ����һ�µĸ���
		if(Act_Unix_T <PW_Unix_T) //��⵽������ʱЧ��̬�������ʱ��ɾ���ô���(ͨ������ʼλ��0��ʵ�֣�����Flash���)
		{
			tab_dat[0] =0x00;  //����λ�������ʾ��������ʧЧ
			Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
			SPIFlash_Write_Tab(tab_dat,Block,Page,1); //д���޸ĺ�����ݵ���̬��������
			SPI_Flash_Busy();	
			return 0;
		}
		
		if(tab_dat[1]<=PassWord_Len)
		{
			is1 = PassWord_Len-tab_dat[1];
			for(i =0;i<=is1;i++)  //��ʼ��λ������ѯ���Ƿ���һ�µ����� iΪ����ѯ������㣬��ȷ��������Ч���������������ڵ��ڱ��ض�̬���볤��
			{
				it1 =0;
				for(is =0;is<tab_dat[1];is++) //��ָ�����������ͷ��ʼ���ڶ�̬���볤�ȷ�Χ����ѯ�Ƿ���ȫһ��
				{
					if(Key_Num_Tab[i+is]!=tab_dat[is+2]) it=1;  //������ֵ��ͬʱ����it��1 ��ʾ�ȶ�ʧ��
					else it1++;
				}
				if((it1>=(tab_dat[1]-2))&&   //�ж�����λ��Ϊ ���볤�� -2
					(tab_dat[1]>6)&&						//���ƽ��Գ��ȴ���6��������Ч
					(it1<(tab_dat[1]))													//�����볤����һ�µ��������һ��ʱ�ж��¶�̬���벻��Ч
					)
				{
					
					Act_PassWord_rese =1;   //�ж�����λ��1
					
				}
				
			if(it==1) it =0;
				else 
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Off_Lin_PassWord_Time_Limit:%d\r\n",Act_Unix_T);
					
					#endif
					
							
					nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
					nrf_delay_ms(1); //�ȴ��ȶ�
					
					if(nrf_gpio_pin_read(5)==0)   //���ŷ���ʱ��ʾ���ѷ���  ����ʱ������̬���뿪��
					{
						
						nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
						
					//	input_speak(17); //������ʾ�ѷ���
				//		nrf_delay_ms(1200);
						return 0;
					}
				
					nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
			
					//��¼���ŵ�����
					Op_Door_Buff[5] = tab_dat[1];  //��¼����
					
					for(i =0;i<tab_dat[1];i++)			//��¼����
						Op_Door_Buff[i+6] = tab_dat[i+2];
					
					
					return 1;  //����ֵһ��ʱ������1����ʾ������ȷ����Ч��������
					
				}
			}
		}
		return 0;
}



/*****************************************************************************************

���鶯̬������֤����

���ܣ��ж����������Ƿ��뱣���ʱЧ��̬����һ�£���ȷ�������Ƿ��ѹ���


���룺 PassWord_Len  ��������볤��
���룺 PW_Unix_T 		 ��ǰ����ʱ��

*******************************************************************************************/
uint8_t More_Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T)
{
		//		uint32_t len;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page;	 //��ȡFlash�����������
		uint32_t it,it1,it2,i,is,is1;
		uint32_t Act_Unix_T; //�������Ч��̬�������Чʱ��
		uint32_t More_Act_PassWord_Num;
		uint32_t len;
		uint8_t ir;			//ѭ������
		
	//	uint8_t tab[Key_Num_Tab_Len]; //��̬���뻺��
		
	
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"More_Act_PassWord_Verify!\r\n");
		
		#endif
//	
//		for(i =0;i<PassWord_Len;i++)
//			tab[i] = Key_Num_Tab[i];
		
		More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (���鶯̬���뱣���ר������)
		More_Act_PassWord_Num&=0xFFFF;		//ȡ More_Act_PassWord_Num �������ֽ���Ч��ֵ
		len = More_Act_PassWord_Num * More_Act_PassWord_Len;
			
		Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������
		
//		if(PassWord_Len<6)  //������ĳ��Ȳ����������������Ч��ֱ�ӽ�������
//			return 0;
//		
		//����ж϶�̬�����Ƿ�ƥ��
		
		for(it=0;it<More_Act_PassWord_Num;it++)    //����ж�
		{
		
			#ifdef uart_start
					
			SEGGER_RTT_printf(0,"Cheak_PassWord_N: %x!\r\n",it);
					
			#endif
			
			if(tab_dat[it*More_Act_PassWord_Len+1]>PassWord_Len)    //���ر���Ķ�Ӧ���볤��Ҫ���ڵ�������ĳ���
			{
				;//���س��ȴ������볤�ȣ���Ȼ������
			}
			else    //�ҵ�һ��������
			{
				for(it1=0;
				it1<=(PassWord_Len-(tab_dat[it*More_Act_PassWord_Len+1]));  //���볤���뱾�س��ȵĲ�������ѭ������
					it1++)			//��λ�������ѭ��
				{

					ir =0;
					for(it2 =0;
							it2<(tab_dat[it*More_Act_PassWord_Len+1]);  //�ȶԱ�������
							it2++
							)
							{
								 if(Key_Num_Tab[it1+it2] == tab_dat[it*More_Act_PassWord_Len+2+it2])
								 {
									ir++;	 
								 }
								 
							}
							if(ir==tab_dat[it*More_Act_PassWord_Len+1])
							{
								//ƥ�䵽��ȷ����
										//����ʱ��
								
										#ifdef uart_start
										
										SEGGER_RTT_printf(0,"C_More_Act_PassWord_Num: %x!\r\n",it);
										
										#endif
										
										Act_Unix_T = tab_dat[it*More_Act_PassWord_Len+13];  //����ʱ��� 24-31λ
										Act_Unix_T <<= 8;
										
										Act_Unix_T |= tab_dat[it*More_Act_PassWord_Len+14];  //����ʱ��� 16-23λ
										Act_Unix_T <<=8;

										Act_Unix_T |= tab_dat[it*More_Act_PassWord_Len+15];  //����ʱ��� 8-15λ
										Act_Unix_T <<=8;
										
										Act_Unix_T |= tab_dat[it*More_Act_PassWord_Len+16];  //����ʱ��� 0-7λ
										
										if(Act_Unix_T >PW_Unix_T)
										{
											//������
											
												#ifdef uart_start
		
												SEGGER_RTT_printf(0,"More_Act_PassWord_Time_Limit:%d\r\n",Act_Unix_T);
												
												#endif
												
														
												nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
												nrf_delay_ms(1); //�ȴ��ȶ�
												
												if(nrf_gpio_pin_read(5)==0)   //���ŷ���ʱ��ʾ���ѷ���  ����ʱ������̬���뿪��
												{
													
													nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
													
												//	input_speak(17); //������ʾ�ѷ���
											//		nrf_delay_ms(1200);
													return 0;
												}
											
											nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
										
												//��¼���ŵ�����
												Op_Door_Buff[5] = tab_dat[1];  //��¼����
												
												for(i =0;i<tab_dat[it*More_Act_PassWord_Len+1];i++)			//��¼����
													Op_Door_Buff[i+6] = tab_dat[it*More_Act_PassWord_Len+2+i];
												
												#ifdef uart_start
		
												SEGGER_RTT_printf(0,"More_Act_PassWord_OP_Door!\r\n");
												
												#endif
												
												return 1;  //����ֵһ��ʱ������1����ʾ������ȷ����Ч��������
								
										}
									
								return 0;
							}
				}
			}
			
		}
		
		
	return 0;
}


/************************************************

uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord)

//ɾ������ ���ض�̬����

���룺 *PassWord  Ŀ�궯̬����

************************************************/
uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord)
{
	
	uint8_t tab[15];			//��̬���뻺��
	uint32_t Block,Page,Sector;	 //��ȡFlash�����������
	uint32_t it,it1,it2,i,is,is1;
	uint32_t More_Act_PassWord_Num;
	uint32_t len;
	uint8_t ir;			//ѭ������

	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Dle_A_More_Act_PassWord_Verify!\r\n");
	
	#endif
	
	More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (���鶯̬���뱣���ר������)
	More_Act_PassWord_Num&=0xFFFF;		//ȡ More_Act_PassWord_Num �������ֽ���Ч��ֵ
	len = More_Act_PassWord_Num * More_Act_PassWord_Len;
	
	Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
	SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������
	
	for(i=0;i<sizeof(tab);i++)
		tab[i] =0;
		
		tab[0] = *PassWord;				//��������������
		if(tab[0]<10)
		{
			for(i =0;i<tab[0];i++)				// ��������������������
			{
				PassWord++;
				tab[i+1] = *PassWord;
				
			}
		}
		else
			return 0;
		
		for(it=0;it<More_Act_PassWord_Num;it++)    //����ж�
		{
			
			#ifdef uart_start
					
			SEGGER_RTT_printf(0,"Cheak_PassWord_N: %x!\r\n",it);
					
			#endif                                  
			
			if(tab_dat[it*More_Act_PassWord_Len+1]==tab[0])    //���ر������������������볤��
			{
				//���Եȳ��������ж�
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Cheak_PassWord_N: %x!\r\n",it);

				#endif
				ir =0;
				for(it1 =0;it1<tab[0];it1++)
				{
					
					if(tab[it1+1]==tab_dat[it*More_Act_PassWord_Len+it1+2])
					{
						
						ir++;
						
					}
				}
				
				if(ir==tab[0])  //�� �ҵ�һ�µ� ����ʱ���ǵ��������
				{
					#ifdef uart_start
				
					SEGGER_RTT_printf(0,"Cheak_PassWord_OK: %x!\r\n",it);

					#endif
					for(i =0;i<((More_Act_PassWord_Num-it)*More_Act_PassWord_Len);i++)
					{
						
						tab_dat[it*More_Act_PassWord_Len+i] = tab_dat[(it+1)*More_Act_PassWord_Len+i];
						
					}
					//�޸������ݺ� ����д�����ݺ�����
					
					
						More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (���鶯̬���뱣���ר������)
					//	More_Act_PassWord_Num&=0xFFFF;		//ȡ More_Act_PassWord_Num �������ֽ���Ч��ֵ
					
					if((More_Act_PassWord_Num&0xFFFF)>=1)
						More_Act_PassWord_Num -=1;  //������һ
						
					Sector_to_Block_Sector(&Block,&Sector,More_Act_PassWord_Secotr);  //�������鶯̬������������
					SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
					
					Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);
					SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�Ķ������ݵ���̬��������
					SPI_Flash_Busy();	          
					
					Ev_Cat_Dat(More_Act_PassWord_Num_Code,More_Act_PassWord_Num);    //��������
					
					read_Soctor(More_Act_PassWord_Secotr);				//�����������
					
					return 1;   
						
				}
			}
		}
		return 1;
}



/********************************************************************************************
�������������ж�

�������벻��Ҫʱ��ͳ���

����1 ˵�����Կ���


*********************************************************************************************/
uint8_t Per_Act_PassWord_Verify(uint8_t PassWord_Len)
{
		
		uint32_t Block,Page;	 //��ȡFlash�����������
		uint32_t it,it1,is,is1;
//	uint32_t Act_Unix_T; //�������Ч��̬�������Чʱ��
		uint32_t len;
		uint32_t Per_Act_PassWord_Num;
	
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Per_Act_PassWord_Verify!\r\n");
		
		#endif
	
		len =0;
	
		Per_Act_PassWord_Num = Get_Cat_Dat(0x04);  //��ȡ��ǰ�豸�Ѿ�����Ķ�̬���� (������һ�ζ�̬���룬��ͨ��̬����ֱ�Ӵ���ڸ�������ʼ16�ֽ�����)
	
		#ifdef uart_start
	
		SEGGER_RTT_printf(0,"\r\n Per_Act_PassWord_Num: %x .\r\n",Per_Act_PassWord_Num);
			
		#endif
	
		len = Per_Act_PassWord_Num*Per_Act_Len;  //���㳤��  //��һλ���ڳ����滻��lenֵֻ���� Once_Act_Len ��С
	
		
	if(PassWord_Len>=8);			//�ж����볤�Ⱥ���
		else
			return 0;
				
	if(Per_Act_PassWord_Num==0)
		return 0;

		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Per_Act_PassWord: \r\n");
		
				for(uint32_t i =0;i<len;i++)
				{
					SEGGER_RTT_printf(0,"%x,",tab_dat[i]);
					if((i%16)==0)
					{
						SEGGER_RTT_printf(0,"\r\n");
					}
				}
		SEGGER_RTT_printf(0,"\r\n");
		SEGGER_RTT_printf(0,"\r\n");			
		
		SEGGER_RTT_printf(0,"PassWord_Len: %x!\r\n",PassWord_Len);
		
		#endif
	

		
	
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������
		
		
		
	//Key_Num_Tab
		
		for(it =0;it<Per_Act_PassWord_Num;it++)								//��Ҫ�жϵ���������
		{
				for(it1 =0;it1<=(PassWord_Len-8);it1++)			//���������λ�����ж϶��ٴ�
				{
						is1 =0;
						for(is =0;is<8;is++)
						{
							
								if(tab_dat[it*Per_Act_Len+is+1] == Key_Num_Tab[is+it1])
								is1++;
								
								if(is1==8)			//������һ��ʱ����1 (��ȷλ�������������볤��)
								{
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"it: %x!\r\n",it);
									SEGGER_RTT_printf(0,"it1: %x!\r\n",it1);
									SEGGER_RTT_printf(0,"is: %x!\r\n",is);
									SEGGER_RTT_printf(0,"is1: %x!\r\n",is1);
									
									#endif
									
										nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
										nrf_delay_ms(1); //�ȴ��ȶ�
							
										if(nrf_gpio_pin_read(5)==0)   //���ŷ���ʱ��ʾ���ѷ���  ����ʱ������̬���뿪��
										{
											
											nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
											
										//	input_speak(17); //������ʾ�ѷ���
										//	nrf_delay_ms(1200);
											return 0;
										}
									
									nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
									
								//�����ö�̬�������� ���뿪�ż�¼����
								for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff ����
									Op_Door_Buff[i1] =0;									
									
									Op_Door_Buff[5] = 8;		//���볤�� =  Key_Num_n
									
									for(uint8_t i1=0;i1<8;i1++)		//��ʱЧ��̬������������
									{
										 Op_Door_Buff[6+i1] =Key_Num_Tab[i1+it];				//�����Ӧλ�õ� ��������
										
									}
										
									return 1;
								}
						}
					
				}
		}
	
	return 0;
}



/******************************************************************************************

Once_Act_PassWord_Verify  //����ȷ��һ�������Ƿ��Ѿ���ʹ�ù�

//���룺 ��  �жϵ�����ֱ��ȡ Key_Num_Tab ��ǰ��8λ������ȷ�� Key_Num_n ��ֵΪ 8 (��һ�����볤��һ��)

��̬������Ҫ����Ч���24Сʱ��ʹ�ã�����ֻ��ʹ��һ�Ρ�

����� һ�������Ƿ���ڣ����ڷ���0(���ɿ���)�������ڷ���1 (�ɿ��ţ��漴���������̬����)

*******************************************************************************************/

uint32_t Once_Act_Run =0;  //һ�������ʹ�ü�¼��������24Сʱδʹ��һ������ʱ�����һ������
uint32_t Once_Act_Num =0;  //һ����������
uint8_t Once_Act_PassWord_Verify()
{
		
		uint32_t len;  //�����ܳ��Ⱥͺ���
		uint32_t Block,Page;  //��ȡFlash�����������
		uint32_t it,i,is;
		//uint32_t Act_Unix_T; //�������Ч��̬�������Чʱ��
		uint32_t Act_PassWord_Num;
	
		Act_PassWord_Num = Get_Cat_Dat (0x02);  //��ȡ�ѱ���Ķ�̬��������
		Once_Act_Num = Act_PassWord_Num;
		Once_Act_Run =1;
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;  //��ջ���
		
	  len = Once_Act_Len*Act_PassWord_Num+Common_Act_Len;
		
		if(Act_PassWord_Num==0)  // ������δ����һ������ʱ��������У�ԣ�ֱ�Ӵ���ö�̬����
		{
			Act_PassWord_Save(Once_Act_Type,Key_Num_Tab,8,0); //�����һ������д�� 
			
		//��һ���������� ���뿪�ż�¼����
			for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff ����
				Op_Door_Buff[i1] =0;
		
			Op_Door_Buff[5] = 8;		//�������볤�ȶ���6
			
			Op_Door_Buff[6] = Key_Num_Tab[0];		//���������һλ
			Op_Door_Buff[7] = Key_Num_Tab[1];	//��������ڶ�λ
			Op_Door_Buff[8] = Key_Num_Tab[2];	//�����������λ
			Op_Door_Buff[9] = Key_Num_Tab[3];		//�����������λ
			Op_Door_Buff[10] = Key_Num_Tab[4];		//�����������λ
			Op_Door_Buff[11] = Key_Num_Tab[5];	//�����������λ
			Op_Door_Buff[12] = Key_Num_Tab[6];	//�����������λ
			Op_Door_Buff[13] = Key_Num_Tab[7];	//�����������λ
					
				
			
			return 1; //��һ�������һ������ʱ������1 ������һ�Ρ�
			
		}
	  else if(Act_PassWord_Num>0)  //�����ر�����һ������ʱ������У�ԣ������붯̬����
		{
			
			Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ�������������ȳ�����������		
		  
			it =0;  //�ظ�ѭ��
			
			for(i =Common_Act_Len;i<len;) //��ѯ�Ƿ���һ�µ����ݣ����򷵻�0 ���迪�ţ�û����д�룬������һ�� ��ѯʱ������ͨ��̬���볤��
			{
					for(is =i;is<(i+Once_Act_Len);is++)
					 {
						 if(tab_dat[is+1] == Key_Num_Tab[is-i])
								it++;
					 }
					 if(it>=8) 
						 return 0;  //����ͬһ������ʱ������0,��������
					 
					 else it =0;
					 i+=Once_Act_Len;
			}
			
			Act_PassWord_Save(Once_Act_Type,Key_Num_Tab,8,0); //�����һ�����벻����ʱ�������һ������д�� 
			
			//��һ���������� ���뿪�ż�¼����
			for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff ����
				Op_Door_Buff[i1] =0;
		
			Op_Door_Buff[5] = 8;		//�������볤�ȶ���6
			
			Op_Door_Buff[6] = Key_Num_Tab[0];		//���������һλ
			Op_Door_Buff[7] = Key_Num_Tab[1];	//��������ڶ�λ
			Op_Door_Buff[8] = Key_Num_Tab[2];	//�����������λ
			Op_Door_Buff[9] = Key_Num_Tab[3];		//�����������λ
			Op_Door_Buff[10] = Key_Num_Tab[4];		//�����������λ
			Op_Door_Buff[11] = Key_Num_Tab[5];	//�����������λ
			Op_Door_Buff[12] = Key_Num_Tab[6];	//�����������λ
			Op_Door_Buff[13] = Key_Num_Tab[7];	//�����������λ
			
			return 1; //��һ�������һ������ʱ������1 ������һ�Ρ�
			
		}
		
			return 0;
	
}

const uint8_t History_Time_Len = 11;  //���� ��ʷ��̬���볤��  (0XAA+���볤�� + 9�ֽ����뱣��ռ�)
extern uint8_t Key_Num_n;
const uint8_t History_Max_Num = 30;  //��󱣴���ʷ��������

/***************************************************

History_Time_Act_PassWord  

����:

�µ�ʱЧ�Զ�̬���룬��Чǰ��Ҫ�����жϣ���������Ƿ���֮ǰ�Ѿ���ʹ�ù�

���룺 ֱ�Ӱ��������������

��� ��ͬ������ ����1����ͬ�����뷵��0 
��ͬ������ʱ�����벻�ܱ��棬Ҳ����ʹ�ã�
û��ͬ������ʱ���������룬����ʹ�á�

***************************************************/

uint8_t  History_Time_Act_PassWord()
{
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t it,i,is;
//			uint32_t Act_Unix_T; //�������Ч��̬�������Чʱ��
			uint32_t Act_PassWord_Num;

			
			Act_PassWord_Num = Get_Cat_Dat (0x03);  //��ȡ�ѱ���Ķ�̬��������
			
			len = Act_PassWord_Num * History_Time_Len;  //����� ���б��ض�̬��������
			
			for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0;  //��ջ���
			
			if(len>=History_Time_Len)  //�����ر�����һ�����ϵ���ʷʱЧ��̬����ʱ
			{
					Sector_to_Block_Page(&Block,&Page,History_PassWord_Secotr);  //���������Ż�ȡ�����ʼҳ
					SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������		
					
					it =0;  //�ظ�ѭ��
					for (i =0;i<len;) //��ѯ�Ƿ���һ�µ����ݣ����򷵻�0 ���迪�ţ�û����д�룬������һ�� ��ѯʱ������ͨ��̬���볤��
					{
							for (is =i;is<(i+Key_Num_n);is++)
							 {
									if((tab_dat[is+2] == Key_Num_Tab[is-i])&&(tab_dat[i+1]==Key_Num_n))  //������һ�£��Ҷ�Ӧλ����һ��ʱ��it ֵ��һ
										it++;
							 }
									 
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"it:%d\r\n",it);
								
								#endif
										 
							 if(it>=Key_Num_n)
								 return 0;  //����ͬ��ʷ����ʱ������0,��������
							 else 
								 it =0;
							 
							 i+=History_Time_Len;
					}
			}
			
			for (i=(len+History_Time_Len);i>=History_Time_Len;i--)  //�����벻һ��ʱ��ƽ������ճ���һ��λ��
			{
				tab_dat[i]=tab_dat[i-History_Time_Len];
			}
			
			tab_dat[0]=0xAA;  //���������ݰ�ͷ
			
			tab_dat[1]=Key_Num_n;  //���������ݳ���
			
			for (i=0;i<(History_Time_Len-2);i++)  //���µ���ʷ�������ݷŵ���ǰ�棬�����𲽺���
			{
					if(i<Key_Num_n)
					{
						tab_dat[i+2]=Key_Num_Tab[i];
					}
					else
					{
						tab_dat[i+2]=0;
					}
			}
		
		Act_PassWord_Num++;
		
		if(Act_PassWord_Num>=History_Max_Num)
			Act_PassWord_Num = History_Max_Num;
		
		len = Act_PassWord_Num * History_Time_Len;
		
		Ev_Cat_Dat(0x03,Act_PassWord_Num);  //��������
		
		Sector_to_Block_Sector(&Block,&Sector,History_PassWord_Secotr);  //������̬������������
		
		SPIFlash_Erase_Sector(Block,Sector);
		
		SPI_Flash_Busy();
		
		Sector_to_Block_Page(&Block,&Page,History_PassWord_Secotr);
		
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬��������
	 	SPI_Flash_Busy();	
		
		return 1;  //�������µ���ʷ���ݺ󣬷���1

}


/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


д��һ������ԱA����Ϣ�� ��Ӧ�洢����

���������ʽ��
���˿���
0XAA+4�ֽ�A����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� ÿ������13�ֽ�
����Ա����
0X55+4�ֽڿ���Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� ÿ������13�ֽ�

һ�������ɱ��湲��145������

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //����B�����ݵ�����1 (Ŀǰ����1������)


const uint8_t TypeA_Data_Len = 14;  	//һ��A�����ݵĳ���

const uint8_t TypeB_Data_Len = 18;		//һ��B�����ݵĳ���


*****************************************************************************/
/*
uint16_t Add_Flash_AD_TypeA_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
	
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeA_Data_Num;
			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeA_Data_Len];  //����
			
			uint32_t is1,is2;
			
		for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
				tab_dat[i] =0;
			
			for(i =0;i<4;i++)  //��uidֵ���� tab����
			{
					tab[i] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			len1 =0;
			len1 =0;
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
			if(TypeA_Data_Num==0XFFFFFF)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_TypeA_Data_Num_Error!\r\n");
				
				#endif
				
				return 0;
				
			}
			
			len = TypeA_Data_Num * TypeA_Data_Len;  //�ó�ռ�õĻ�����
	
			if(len>=TypeA_Data_Len)  //��len���� ���ڵ���һ�� TypeA_Data ���� ���ѱ���������ʱ
			{
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
			
				len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
				
				for(i =0;i<len;)
				{
					if(tab_dat[i]==0XAA) //��ͷ����ʱ
					{
						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])) //��������һ�µ�uidʱ ��len1 ����Ϊһ��uid����ʼ��ַ
						{
							//len1 = i;
							i = len;  //��i��ֵ����len1 ��ͨ����i��ֵ�ó�len �Խ���֮���ѭ��
						}
						else
						{
							i+=TypeA_Data_Len;  //��ѯ��һ��uid���л�����һ����ʼ��ַ
							len1 = i;					//ͬʱ���� len1 ��ֵ
						}
					}
					else
					{
						i = len;
						len1 =len;
					}
				}
				
				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
					&&(TypeA_Data_Num<A_Sector_TypeA_Data_Num)) //��len1����lenʱ����ʾ��������û��һ�µĿ���Ϣ
				{
					
					for(i =4;i>=1;i--)   //������д��ͷ
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //д��ͷ
					
					tab[5] = start_Unix>>24&0xFF;
					tab[6] = start_Unix>>16&0xFF;
					tab[7] = start_Unix>>8&0XFF;
					tab[8] = start_Unix&0xFF;
					
					tab[9]  =  stop_Unix>>24&0XFF;
					tab[10] =  stop_Unix>>16&0XFF;
					tab[11] =  stop_Unix>>8&0XFF;
					tab[12] =  stop_Unix&0xFF;
					
					for(is2 =1;is2<=A_Sector_TypeA_Data_Num;is2++)
					{
						for(is1 =1;is1<=TypeA_Data_Num;is1++)  //��һ������ Type A ������������Ѱ�ҿ��õ�SN��
						{
							if(tab_dat[is2*TypeA_Data_Len-1]==is2)//��SN�Ŵ���ʱ ����ѭ��
							{
								break;
							}
							
						}
						
						if(is2>TypeA_Data_Num)
						{
							break;
						}
						
						if(is2>A_Sector_TypeA_Data_Num)  //�� ���п���SN������ʱ������д�����������ʧ����Ϣ 
							return 0;
					}
					
					tab[TypeA_Data_Len-1] = is2;  //����ȡ���� SN ��д�� tab ���� ��д��
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
					
					#endif
					
					for(i = len1;i<(len1+TypeA_Data_Len);i++)
					{
						tab_dat[i] = tab[i-len1];  //��tabֵ���뵽tab_dat��				
					}
					
					if(tab_dat[len1]==0xAA)
					{
						
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //������̬������������
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeA_Data_Len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
							SPI_Flash_Busy();
							
							Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //���µ�������д��Ŀ¼
							
							return tab[TypeA_Data_Len-1];
					}
					return 0;
				}
				else if(len1<=len-TypeA_Data_Len)  //��len1 С�� len �� TypeA_Data_Len ˵����ID���ڱ����Ѵ��ڣ�ֻ����ʱ�䲻������������
					{
						
					if(tab_dat[len1]==0xAA)  //�����ݰ���ʼֵ��������ʱ
						{
							
							tab_dat[len1+5] = start_Unix>>24&0xFF;
							tab_dat[len1+6] = start_Unix>>16&0xFF;
							tab_dat[len1+7] = start_Unix>>8&0XFF;
							tab_dat[len1+8] = start_Unix&0xFF;
							
							tab_dat[len1+9]  =  stop_Unix>>24&0XFF;
							tab_dat[len1+10] =  stop_Unix>>16&0XFF;
							tab_dat[len1+11] =  stop_Unix>>8&0XFF;
							tab_dat[len1+12] =  stop_Unix&0xFF;
							
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //������̬������������
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
							SPI_Flash_Busy();
							
							return tab_dat[len1+TypeA_Data_Len-1];  //���ص�ǰ���ݵ�SN��
							
						}
						
					}
				else if (TypeA_Data_Num>=A_Sector_TypeA_Data_Num) //���洢�Ŀ����ݴ��ڵ��ڵ�������(һ������²����ܴ���)������д������
				{
					
					#ifdef uart_start
					SEGGER_RTT_printf(0,"AD_TypeA_Data_Sector1 Full!\r\n");
					#endif
					
					return 0;
					
				}
				
			}
			else if(len==0)  //��lenֵΪ��ʱ��ֱ���ڵ�һ��λ�ü����µ�
			{
				  
					for(i =4;i>=1;i--)   //������д��ͷ
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //д��ͷ
					
					tab[5] = start_Unix>>24&0xFF;
					tab[6] = start_Unix>>16&0xFF;
					tab[7] = start_Unix>>8&0XFF;
					tab[8] = start_Unix&0xFF;
					
					tab[9]  =  stop_Unix>>24&0XFF;
					tab[10] =  stop_Unix>>16&0XFF;
					tab[11] =  stop_Unix>>8&0XFF;
					tab[12] =  stop_Unix&0xFF;
					
					tab[13] = 1; //�����һ������ ��Ϊû������������ SN��Ϊ1
				
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
					
					#endif
					
				for(i=0;i<(TypeA_Data_Len);i++)  //��Ϊ len ֵΪ 0 �� ����ֱ����tab_dat ����ʼλ��д���һ������
				{
					
					tab_dat[i] = tab[i];  //�� tab ֵ���뵽 tab_dat ��
					
				}
				
				Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //��������Ա��������1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(AD_TypeA_Data_Num_Code,1);  //���µ�������д��Ŀ¼
				
				return tab[TypeA_Data_Len-1];  //�������һ�ֽڣ� SN ��
			}
			
			return 0;
}






*/

/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


д��һ������ԱA����Ϣ�� ��Ӧ�洢����

���������ʽ��
���˿���
0XAA+4�ֽ�A����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ��+ �����ڱ����к� ÿ������14�ֽ�
����Ա����
0XAA+4�ֽڿ���Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ��+ �����ڱ����к� ÿ������14�ֽ�

һ�������ɱ��湲��145������ ����ʹ�� ������ 100 �ֽ�����


���أ�
���ݱ�д���λ�ã�2�ֽڣ�1�ֽ� ����λ��+1�ֽ�������ID

����� 2020-0121


*****************************************************************************/

uint16_t Add_Flash_AD_TypeA_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			uint32_t i,it; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeA_Data_Len];  //����
	
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //��ʼǰ����ջ���
			tab[i] =0;
	
			tab[0] = 0xAA;		//�ڿ�ʼд������ͷ
	
			for(i =0;i<4;i++)  //��uidֵ���� tab����
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			TypeA_Data_Num_1 =0;
			tab[5] = start_Unix>>24&0xFF;			//����ʱ��
			tab[6] = start_Unix>>16&0xFF;
			tab[7] = start_Unix>>8&0XFF;
			tab[8] = start_Unix&0xFF;
			
			tab[9]  =  stop_Unix>>24&0XFF;
			tab[10] =  stop_Unix>>16&0XFF;
			tab[11] =  stop_Unix>>8&0XFF;
			tab[12] =  stop_Unix&0xFF;
			
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
			if(TypeA_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeA_Data_Num_Error!\r\n");					//����ȡ��������ֵΪ 0XFFFFFFʱ��˵��������������ݽṹ�쳣�����ټ���д��
				
				#endif
				
				return 0;
				
			}
			
			if(TypeA_Data_Num ==(AD_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		// ���������ﵽ����ʱ������д�� 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: AD_TypeA_Data_Write Full!\r\n");   //��ʾ���ݴﵽ���޾���
				
				#endif
				
				return 0;
				
			}
			else if(TypeA_Data_Num >(AD_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		//  ����������������ʱ�������쳣��Ϣ���Ҳ���д��
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeA_Data_Write OverFlow!\r\n");   //��ʾ���ݳ������޴���
				
				#endif
				
				
				return 0;
			
			}
			
			//��һ�о���ʱ��ʼ�����ڵ�һ������д������
			
			//������������Ѱ���Ƿ���һ�µĿ��ţ�����У����䶯λ�ã�ֱ�Ӹ���ʱ��
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
			
				Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
						
						)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
					{
						//����ͬ����Ч����ʱ������ʱ������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//���� ʱ�����ֵ
						
						#endif
						
						  //��ʼ����ʱ������
						if(
							tab_dat[(Dat_Num*TypeA_Data_Len)+5] == tab[5]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+6] == tab[6]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+7] == tab[7]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+8] == tab[8]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+9] == tab[9]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+10] == tab[10]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+11] == tab[11]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+12] == tab[12]
						
							)
						{
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//���� ʱ����³ɹ�
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
							
							return it;   //���� д���λ����Ϣ
					
						}
						else
						{
						tab_dat[(Dat_Num*TypeA_Data_Len)+5] = tab[5];
						tab_dat[(Dat_Num*TypeA_Data_Len)+6] = tab[6];
						tab_dat[(Dat_Num*TypeA_Data_Len)+7] = tab[7];
						tab_dat[(Dat_Num*TypeA_Data_Len)+8] = tab[8];
						tab_dat[(Dat_Num*TypeA_Data_Len)+9] = tab[9];
						tab_dat[(Dat_Num*TypeA_Data_Len)+10] = tab[10];
						tab_dat[(Dat_Num*TypeA_Data_Len)+11] = tab[11];
						tab_dat[(Dat_Num*TypeA_Data_Len)+12] = tab[12];
						
						//������ʱ�����ݺ��޸ĺ������д��ԭ������  �������
						
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeA_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //���µ�������д��Ŀ¼ ����û���������ݣ������������� ���Բ������������


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//���� ʱ����³ɹ�
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
						
						return it;   //���� д���λ����Ϣ
						
					}
					else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
										(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)	
									)
									{
										TypeA_Data_Num_1++;
										
										if(TypeA_Data_Num_1==TypeA_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
										{
											
											Dat_Num =A_Sector_TypeA_Data_Num;			//���ڽ�������ѭ��
											Sector_Num=AD_TypeA_Data_Sector_Num;   //���ڽ�����������ѭ��
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"AD_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//����У��� typeA ������
											
											#endif
											
										}
									
									}
									
						if(TypeA_Data_Num ==0)   //û�б�������ʱ  ֱ�ӽ���ѭ��
							{
									Dat_Num =A_Sector_TypeA_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=AD_TypeA_Data_Sector_Num;   //���ڽ�����������ѭ��
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"AD_TypeA_Null!\r\n");   //��ʾ�ռ�������
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"AD_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//����У��� typeA ������
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
		{
			
			Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //��ȡ������������
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	��������Ѱ�ҿ��е�����λ��
				{

					
					if(
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]>A_Sector_TypeA_Data_Num)
						
						)		//����һ�������ڵ����ݿ�ռ����ʱ����������д���������
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_AD_TypeA_Data!\r\n");
								
								#endif
							tab[13] = Dat_Num+1;  //��tab[13] ���� �ռ����к� ����д��
							
							for(i =0;i<TypeA_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeA_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //���µ�������д��Ŀ¼ �������������ݣ�������Ҫ����

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeA_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[13];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_AD_TypeA_ID: %x ! \r\n",it);  //����д���ID�� �����RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
}





/*****************************************************************************

���� ����Ա A �� ����
��� ����Ա A �� ���� ����


���м���� ����Ա A �� �µ�д��������ʹ��������������

�����  2020-0121

******************************************************************************/

void Erase_AD_TypeA_Sector()
{
	uint32_t Sector_Num;		//������
	uint32_t Block,Sector;  //��ȡFlash�����������
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_AD_TypeA_Sector!\r\n");
	
	#endif
	
	Sector_Num =AD_TypeA_Data_Secotr1;
	
	for(uint32_t i =0;i<AD_TypeA_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //��ȡƫ�ƺ��������������
						
		SPIFlash_Erase_Sector(Block,Sector); 		//��������
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_AD_TypeA_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}



/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


д��һ���û�A����Ϣ�� ��Ӧ�洢����

���������ʽ��
���˿���
0XAA+4�ֽ�A����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� ÿ������13�ֽ�
����Ա����
0X55+4�ֽڿ���Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� ÿ������13�ֽ�

һ�������ɱ��湲��145������

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //����B�����ݵ�����1 (Ŀǰ����1������) 


const uint8_t TypeA_Data_Len = 13;  	//һ��A�����ݵĳ���

const uint8_t TypeB_Data_Len = 17;		//һ��B�����ݵĳ���

���أ� ����� ID (���)

*****************************************************************************/

//uint8_t Add_Flash_US_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
//{
//	
//			uint32_t len;  //�����ܳ��Ⱥͺ���
//			uint32_t Block,Page,Sector;  //��ȡFlash�����������
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
//			uint8_t tab[TypeA_Data_Len];  //����
//			
//			uint32_t is1,is2;
//			
//		for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
//			
//			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
//				tab_dat[i] =0;
//			
//			for(i =0;i<4;i++)  //��uidֵ���� tab����
//			{
//					tab[i] = *uid;
//					uid++;
//			}
//			
//			uid-=i; //���������ָ�uidָ��
//			len1 =0;
//			len1 =0;
//			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
//			
//		if(TypeA_Data_Num==0XFFFFFF)
//			{
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"US_TypeA_Data_Num_Error!\r\n");
//				
//				#endif
//				
//				return 0;
//				
//			}
//			
//			
//			len = TypeA_Data_Num * TypeA_Data_Len;  //�ó�ռ�õĻ�����
//	
//			if(len>=TypeA_Data_Len)  //��len���� ���ڵ���һ�� TypeA_Data ���� ���ѱ���������ʱ
//			{
//				
//				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
//				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
//			
//				len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
//				
//				for(i =0;i<len;)
//				{
//					if(tab_dat[i]==0XAA) //��ͷ����ʱ
//					{
//						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])) //��������һ�µ�uidʱ ��len1 ����Ϊһ��uid����ʼ��ַ
//						{
//							//len1 = i;
//							i = len;  //��i��ֵ����len1 ��ͨ����i��ֵ�ó�len �Խ���֮���ѭ��
//						}
//						else
//						{
//							i+=TypeA_Data_Len;  //��ѯ��һ��uid���л�����һ����ʼ��ַ
//							len1 = i;					//ͬʱ���� len1 ��ֵ
//						}
//					}
//					else
//					{
//						i = len;
//						len1 =len;
//					}
//				}
//				
//				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
//					&&(TypeA_Data_Num<A_Sector_TypeA_Data_Num)) //��len1����lenʱ����ʾ��������û��һ�µĿ���Ϣ
//				{
//					
//					for(i =4;i>=1;i--)   //������д��ͷ
//						tab[i] = tab[i-1];
//					
//					tab[0] = 0xAA;  //д��ͷ
//					
//					tab[5] = start_Unix>>24&0xFF;
//					tab[6] = start_Unix>>16&0xFF;
//					tab[7] = start_Unix>>8&0XFF;
//					tab[8] = start_Unix&0xFF;
//					
//					tab[9]  =  stop_Unix>>24&0XFF;
//					tab[10] =  stop_Unix>>16&0XFF;
//					tab[11] =  stop_Unix>>8&0XFF;
//					tab[12] =  stop_Unix&0xFF;
//					
//					for(is2 =1;is2<=A_Sector_TypeA_Data_Num;is2++)
//					{
//						for(is1 =1;is1<=TypeA_Data_Num;is1++)  //��һ������ Type A ������������Ѱ�ҿ��õ�SN��
//						{
//							if(tab_dat[is2*TypeA_Data_Len-1]==is2)//��SN�Ŵ���ʱ ����ѭ��
//							{
//								break;
//							}
//							
//						}
//						
//						if(is2>TypeA_Data_Num)
//						{
//							break;
//						}
//						
//						if(is2>A_Sector_TypeA_Data_Num)  //�� ���п���SN������ʱ������д�����������ʧ����Ϣ 
//							return 0;
//					}
//					
//					tab[TypeA_Data_Len-1] = is2;  //����ȡ���� SN ��д�� tab ���� ��д��
//					
//					#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"US_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
//					
//					#endif
//					
//					for(i = len1;i<(len1+TypeA_Data_Len);i++)
//					{
//						tab_dat[i] = tab[i-len1];  //��tabֵ���뵽tab_dat��				
//					}
//					
//					if(tab_dat[len1]==0xAA)
//					{
//						
//							Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //������̬������������
//								
//							SPIFlash_Erase_Sector(Block,Sector);
//							
//							SPI_Flash_Busy();
//							
//							Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//							
//							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeA_Data_Len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
//							SPI_Flash_Busy();
//							
//							Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //���µ�������д��Ŀ¼
//							
//							return tab[TypeA_Data_Len-1];
//					}
//					return 0;
//				}
//				else if(len1<=len-TypeA_Data_Len)  //��len1 С�� len �� TypeA_Data_Len ˵����ID���ڱ����Ѵ��ڣ�ֻ����ʱ�䲻������������
//					{
//						
//					if(tab_dat[len1]==0xAA)  //�����ݰ���ʼֵ��������ʱ
//						{
//							
//							tab_dat[len1+5] = start_Unix>>24&0xFF;
//							tab_dat[len1+6] = start_Unix>>16&0xFF;
//							tab_dat[len1+7] = start_Unix>>8&0XFF;
//							tab_dat[len1+8] = start_Unix&0xFF;
//							
//							tab_dat[len1+9]  =  stop_Unix>>24&0XFF;
//							tab_dat[len1+10] =  stop_Unix>>16&0XFF;
//							tab_dat[len1+11] =  stop_Unix>>8&0XFF;
//							tab_dat[len1+12] =  stop_Unix&0xFF;
//							
//							Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //������̬������������
//								
//							SPIFlash_Erase_Sector(Block,Sector);
//							
//							SPI_Flash_Busy();
//							
//							Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//							
//							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
//							SPI_Flash_Busy();
//							
//							return tab_dat[len1+TypeA_Data_Len-1];  //���ص�ǰ���ݵ�SN��
//							
//						}
//						
//					}
//				else if (TypeA_Data_Num>=A_Sector_TypeA_Data_Num) //���洢�Ŀ����ݴ��ڵ��ڵ�������(һ������²����ܴ���)������д������
//				{
//					
//					#ifdef uart_start
//					SEGGER_RTT_printf(0,"US_TypeA_Data_Sector1 Full!\r\n");
//					#endif
//					
//					return 0;
//					
//				}
//				
//			}
//			else if(len==0)  //��lenֵΪ��ʱ��ֱ���ڵ�һ��λ�ü����µ�
//			{
//				  
//					for(i =4;i>=1;i--)   //������д��ͷ
//						tab[i] = tab[i-1];
//					
//					tab[0] = 0xAA;  //д��ͷ
//					
//					tab[5] = start_Unix>>24&0xFF;
//					tab[6] = start_Unix>>16&0xFF;
//					tab[7] = start_Unix>>8&0XFF;
//					tab[8] = start_Unix&0xFF;
//					
//					tab[9]  =  stop_Unix>>24&0XFF;
//					tab[10] =  stop_Unix>>16&0XFF;
//					tab[11] =  stop_Unix>>8&0XFF;
//					tab[12] =  stop_Unix&0xFF;
//					
//					tab[13] = 1; //�����һ������ ��Ϊû������������ SN��Ϊ1
//	
//					#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"US_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
//					
//					#endif
//					
//				for(i=0;i<(TypeA_Data_Len);i++)  //��Ϊ len ֵΪ 0 �� ����ֱ����tab_dat ����ʼλ��д���һ������
//				{
//					
//					tab_dat[i] = tab[i];  //�� tab ֵ���뵽 tab_dat ��
//					
//				}
//				
//				Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //��������Ա��������1
//				
//				SPIFlash_Erase_Sector(Block,Sector);
//				
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//				
//				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(US_TypeA_Data_Num_Code,1);  //���µ�������д��Ŀ¼
//				
//				return tab[TypeA_Data_Len-1];  //�������һ�ֽڣ� SN ��
//			}
//			
//			return 0;
//}


/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


//д��һ���û�A����Ϣ�� ��Ӧ�洢����

//���������ʽ��
//���˿���
//0XAA+4�ֽ�A����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� ÿ������13�ֽ�
//����Ա����
//0X55+4�ֽڿ���Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� ÿ������13�ֽ�

//һ�������ɱ��湲��145������  Ϊ���㱣��

д��һ���û�A����Ϣ�� ��Ӧ�洢����

���������ʽ��
���˿���
0XAA+4�ֽ�A����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ��+ �����ڱ����к� ÿ������14�ֽ�
����Ա����
0XAA+4�ֽڿ���Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ��+ �����ڱ����к� ÿ������14�ֽ�

һ�������ɱ��湲��145������ ����ʹ�� ������ 100 �ֽ�����


const uint8_t AD_TypeA_Data_Secotr1 = 15;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //����B�����ݵ�����1 (Ŀǰ����1������) 


const uint8_t TypeA_Data_Len = 13;  	//һ��A�����ݵĳ���

const uint8_t TypeB_Data_Len = 17;		//һ��B�����ݵĳ���


���أ�
���ݱ�д���λ�ã�2�ֽڣ�1�ֽ� ����λ��+1�ֽ�������ID

����� 2020-0121

*****************************************************************************/

uint16_t Add_Flash_US_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
	
				uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			uint32_t i,it; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeA_Data_Len];  //����
	
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //��ʼǰ����ջ���
			tab[i] =0;
	
			tab[0] = 0xAA;		//�ڿ�ʼд������ͷ
	
			for(i =0;i<4;i++)  //��uidֵ���� tab����
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			TypeA_Data_Num_1 =0;
			tab[5] = start_Unix>>24&0xFF;			//����ʱ��
			tab[6] = start_Unix>>16&0xFF;
			tab[7] = start_Unix>>8&0XFF;
			tab[8] = start_Unix&0xFF;
			
			tab[9]  =  stop_Unix>>24&0XFF;
			tab[10] =  stop_Unix>>16&0XFF;
			tab[11] =  stop_Unix>>8&0XFF;
			tab[12] =  stop_Unix&0xFF;
			
			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
			if(TypeA_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeA_Data_Num_Error!\r\n");					//����ȡ��������ֵΪ 0XFFFFFFʱ��˵��������������ݽṹ�쳣�����ټ���д��
				
				#endif
				
				return 0;
				
			}
			
			if(TypeA_Data_Num ==(US_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		// ���������ﵽ����ʱ������д�� 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: US_TypeA_Data_Write Full!\r\n");   //��ʾ���ݴﵽ���޾���
				
				#endif
				
				return 0;
				
			}
			else if(TypeA_Data_Num >(US_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		//  ����������������ʱ�������쳣��Ϣ���Ҳ���д��
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeA_Data_Write OverFlow!\r\n");   //��ʾ���ݳ������޴���
				
				#endif
				
				
				return 0;
			
			}
			
			//��һ�о���ʱ��ʼ�����ڵ�һ������д������
			
			//������������Ѱ���Ƿ���һ�µĿ��ţ�����У����䶯λ�ã�ֱ�Ӹ���ʱ��
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
			
				Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
						
						)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
					{
						//����ͬ����Ч����ʱ������ʱ������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//���� ʱ�����ֵ
						
						#endif
						
						  //��ʼ����ʱ������
						if(
							tab_dat[(Dat_Num*TypeA_Data_Len)+5] == tab[5]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+6] == tab[6]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+7] == tab[7]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+8] == tab[8]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+9] == tab[9]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+10] == tab[10]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+11] == tab[11]&&
							tab_dat[(Dat_Num*TypeA_Data_Len)+12] == tab[12]
						
							)
						{
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//���� ʱ����³ɹ�
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
							
							return it;   //���� д���λ����Ϣ
					
						}
						else
						{
						tab_dat[(Dat_Num*TypeA_Data_Len)+5] = tab[5];
						tab_dat[(Dat_Num*TypeA_Data_Len)+6] = tab[6];
						tab_dat[(Dat_Num*TypeA_Data_Len)+7] = tab[7];
						tab_dat[(Dat_Num*TypeA_Data_Len)+8] = tab[8];
						tab_dat[(Dat_Num*TypeA_Data_Len)+9] = tab[9];
						tab_dat[(Dat_Num*TypeA_Data_Len)+10] = tab[10];
						tab_dat[(Dat_Num*TypeA_Data_Len)+11] = tab[11];
						tab_dat[(Dat_Num*TypeA_Data_Len)+12] = tab[12];
						
						//������ʱ�����ݺ��޸ĺ������д��ԭ������  �������
						
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeA_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //���µ�������д��Ŀ¼ ����û���������ݣ������������� ���Բ������������


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//���� ʱ����³ɹ�
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
						
						return it;   //���� д���λ����Ϣ
						
					}
					else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
										(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)	
									)
									{
										TypeA_Data_Num_1++;
										
										if(TypeA_Data_Num_1==TypeA_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
										{
											
											Dat_Num =A_Sector_TypeA_Data_Num;			//���ڽ�������ѭ��
											Sector_Num=US_TypeA_Data_Sector_Num;   //���ڽ�����������ѭ��
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"US_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//����У��� typeA ������
											
											#endif
											
										}
									
									}
									
						if(TypeA_Data_Num ==0)   //û�б�������ʱ  ֱ�ӽ���ѭ��
							{
									Dat_Num =A_Sector_TypeA_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=US_TypeA_Data_Sector_Num;   //���ڽ�����������ѭ��
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"US_TypeA_Null!\r\n");   //��ʾ�ռ�������
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"US_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//����У��� typeA ������
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
		{
			
			Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //��ȡ������������
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	��������Ѱ�ҿ��е�����λ��
				{

					
					if(
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]>A_Sector_TypeA_Data_Num)
						
						)		//����һ�������ڵ����ݿ�ռ����ʱ����������д���������
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_US_TypeA_Data!\r\n");
								
								#endif
							tab[13] = Dat_Num+1;  //��tab[13] ���� �ռ����к� ����д��
							
							for(i =0;i<TypeA_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeA_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //���µ�������д��Ŀ¼ �������������ݣ�������Ҫ����

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeA_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[13];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_US_TypeA_ID: %x ! \r\n",it);  //����д���ID�� �����RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
	
}



/*****************************************************************************

���� �û� A �� ����
��� �û� A �� ���� ����


���м���� �û� A �� �µ�д��������ʹ��������������

�����  2020-0121

******************************************************************************/

void Erase_US_TypeA_Sector()
{
	uint32_t Sector_Num;		//������
	uint32_t Block,Sector;  //��ȡFlash�����������
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_US_TypeA_Sector!\r\n");
	
	#endif
	
	Sector_Num =US_TypeA_Data_Secotr1;
	
	for(uint32_t i =0;i<US_TypeA_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //��ȡƫ�ƺ��������������
						
		SPIFlash_Erase_Sector(Block,Sector); 		//��������
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_US_TypeA_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}





/*****************************************************************************

uint8_t Add_Flash_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


д��һ������ԱB����Ϣ�� ��Ӧ�洢����

���������ʽ��
���˿���
0XAA+4�ֽ�A����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� + 1�ֽ�SN�� ÿ������14�ֽ�
����Ա����
0X55+4�ֽڿ���Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� + 1�ֽ�SN�� ÿ������14�ֽ�

һ�������ɱ��湲��145������

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //����B�����ݵ�����1 (Ŀǰ����1������)


const uint8_t TypeA_Data_Len = 13;  	//һ��A�����ݵĳ���

const uint8_t TypeB_Data_Len = 17;		//һ��B�����ݵĳ���


*****************************************************************************/
/*
uint8_t Add_Flash_AD_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeB_Data_Len];  //����
			
			uint32_t is1,is2;
			
		for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
				tab_dat[i] =0;
			
			for(i =0;i<8;i++)  //��uidֵ���� tab����
			{
					tab[i] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			len1 =0;
			len1 =0;
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
			if(TypeB_Data_Num==0XFFFFFF)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_TypeB_Data_Num_Error!\r\n");
				
				#endif
				
				return 0;
				
			}
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //�ó�ռ�õĻ�����
	
			if(len>=TypeB_Data_Len)  //��len���� ���ڵ���һ�� TypeA_Data ���� ���ѱ���������ʱ
			{
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
			
				len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
				
				for(i =0;i<len;)
				{
					if(tab_dat[i]==0XAA) //��ͷ����ʱ
					{
						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])) //��������һ�µ�uidʱ ��len1 ����Ϊһ��uid����ʼ��ַ
						{
							//len1 = i;
							i = len;  //��i��ֵ����len1 ��ͨ����i��ֵ�ó�len �Խ���֮���ѭ��
						}
						else
						{
							i+=TypeB_Data_Len;  //��ѯ��һ��uid���л�����һ����ʼ��ַ
							len1 = i;					//ͬʱ���� len1 ��ֵ
						}
					}
					else
					{
						i = len;
						len1 =len;
					}
				}
				
				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
					&&(TypeB_Data_Num<A_Sector_TypeB_Data_Num)) //��len1����lenʱ����ʾ��������û��һ�µĿ���Ϣ
				{
					
					for(i =8;i>=1;i--)   //������д��ͷ
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //д��ͷ
					
					tab[9] = start_Unix>>24&0xFF;
					tab[10] = start_Unix>>16&0xFF;
					tab[11] = start_Unix>>8&0XFF;
					tab[12] = start_Unix&0xFF;
					
					tab[13]  =  stop_Unix>>24&0XFF;
					tab[14] =  stop_Unix>>16&0XFF;
					tab[15] =  stop_Unix>>8&0XFF;
					tab[16] =  stop_Unix&0xFF;
					
					for(is2 =1;is2<=A_Sector_TypeB_Data_Num;is2++)
					{
						for(is1 =1;is1<=TypeB_Data_Num;is1++)  //��һ������ Type A ������������Ѱ�ҿ��õ�SN��
						{
							if(tab_dat[is2*TypeB_Data_Len-1]==is2)//��SN�Ŵ���ʱ ����ѭ��
							{
								break;
							}
							
						}
						
						if(is2>TypeB_Data_Num)
						{
							break;
						}
						
						if(is2>A_Sector_TypeB_Data_Num)  //�� ���п���SN������ʱ������д�����������ʧ����Ϣ 
							return 0;
					}
					
					tab[TypeB_Data_Len-1] = is2;  //����ȡ���� SN ��д�� tab ���� ��д��
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
					for(i = len1;i<(len1+TypeB_Data_Len);i++)
					{
						tab_dat[i] = tab[i-len1];  //��tabֵ���뵽tab_dat��				
					}
					
					if(tab_dat[len1]==0xAA)
					{
						
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //������̬������������
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeB_Data_Len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
							SPI_Flash_Busy();
							
							Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //���µ�������д��Ŀ¼
							
							return tab[TypeB_Data_Len-1];
					}
					return 0;
				}
				else if(len1<=len-TypeB_Data_Len)  //��len1 С�� len �� TypeA_Data_Len ˵����ID���ڱ����Ѵ��ڣ�ֻ����ʱ�䲻������������
					{
						
					if(tab_dat[len1]==0xAA)  //�����ݰ���ʼֵ��������ʱ
						{
							
							tab_dat[len1+9] = start_Unix>>24&0xFF;
							tab_dat[len1+10] = start_Unix>>16&0xFF;
							tab_dat[len1+11] = start_Unix>>8&0XFF;
							tab_dat[len1+12] = start_Unix&0xFF;
							
							tab_dat[len1+13]  =  stop_Unix>>24&0XFF;
							tab_dat[len1+14] =  stop_Unix>>16&0XFF;
							tab_dat[len1+15] =  stop_Unix>>8&0XFF;
							tab_dat[len1+16] =  stop_Unix&0xFF;
							
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //������̬������������
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
							SPI_Flash_Busy();
							
							return tab_dat[len1+TypeB_Data_Len-1];  //���ص�ǰ���ݵ�SN��
							
						}
						
					}
				else if (TypeB_Data_Num>=A_Sector_TypeB_Data_Num) //���洢�Ŀ����ݴ��ڵ��ڵ�������(һ������²����ܴ���)������д������
				{
					
					#ifdef uart_start
					SEGGER_RTT_printf(0,"AD_TypeB_Data_Sector1 Full!\r\n");
					#endif
					
					return 0;
					
				}
				
			}
			else if(len==0)  //��lenֵΪ��ʱ��ֱ���ڵ�һ��λ�ü����µ�
			{
				  
					for(i =8;i>=1;i--)   //������д��ͷ
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //д��ͷ
					
					tab[9] = start_Unix>>24&0xFF;
					tab[10] = start_Unix>>16&0xFF;
					tab[11] = start_Unix>>8&0XFF;
					tab[12] = start_Unix&0xFF;
					
					tab[13]  =  stop_Unix>>24&0XFF;
					tab[14] =  stop_Unix>>16&0XFF;
					tab[15] =  stop_Unix>>8&0XFF;
					tab[16] =  stop_Unix&0xFF;
					
					tab[17] = 1; //�����һ������ ��Ϊû������������ SN��Ϊ1
	
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
				for(i=0;i<(TypeB_Data_Len);i++)  //��Ϊ len ֵΪ 0 �� ����ֱ����tab_dat ����ʼλ��д���һ������
				{
					
					tab_dat[i] = tab[i];  //�� tab ֵ���뵽 tab_dat ��
					
				}
				
				Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //��������Ա��������1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(AD_TypeB_Data_Num_Code,1);  //���µ�������д��Ŀ¼
				
				return tab[TypeB_Data_Len-1];  //�������һ�ֽڣ� SN ��
			}
			
			return 0;
}
*/





uint16_t Add_Flash_AD_TypeB_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint32_t i,it; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeB_Data_Len];  //����
	
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //��ʼǰ����ջ���
			tab[i] =0;
	
			tab[0] = 0xAA;		//�ڿ�ʼд������ͷ
	
			for(i =0;i<8;i++)  //��uidֵ���� tab����
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			TypeB_Data_Num_1 =0;
			tab[9] = start_Unix>>24&0xFF;			//����ʱ��
			tab[10] = start_Unix>>16&0xFF;
			tab[11] = start_Unix>>8&0XFF;
			tab[12] = start_Unix&0xFF;
			
			tab[13]  =  stop_Unix>>24&0XFF;
			tab[14] =  stop_Unix>>16&0XFF;
			tab[15] =  stop_Unix>>8&0XFF;
			tab[16] =  stop_Unix&0xFF;
			
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
			if(TypeB_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeB_Data_Num_Error!\r\n");					//����ȡ��������ֵΪ 0XFFFFFFʱ��˵��������������ݽṹ�쳣�����ټ���д��
				
				#endif
				
				return 0;
				
			}
			
			if(TypeB_Data_Num ==(AD_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		// ���������ﵽ����ʱ������д�� 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: AD_TypeB_Data_Write Full!\r\n");   //��ʾ���ݴﵽ���޾���
				
				#endif
				
				return 0;
				
			}
			else if(TypeB_Data_Num >(AD_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		//  ����������������ʱ�������쳣��Ϣ���Ҳ���д��
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeB_Data_Write OverFlow!\r\n");   //��ʾ���ݳ������޴���
				
				#endif
				
				
				return 0;
			
			}
			
			//��һ�о���ʱ��ʼ�����ڵ�һ������д������
			
			//������������Ѱ���Ƿ���һ�µĿ��ţ�����У����䶯λ�ã�ֱ�Ӹ���ʱ��
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
			
				Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&	// Type_B ���а��ֽ�ID
						(tab_dat[(Dat_Num*TypeB_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+5]==tab[5])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+6]==tab[6])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+7]==tab[7])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+8]==tab[8])&&
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
						
						)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
					{
						//����ͬ����Ч����ʱ������ʱ������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//���� ʱ�����ֵ
						
						#endif
						
						  //��ʼ����ʱ������
						if(			//��ʱ��δ�����仯ʱ�������дflash
							tab_dat[(Dat_Num*TypeB_Data_Len)+9] == tab[9]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+10] == tab[10]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+11] == tab[11]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+12] == tab[12]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+13] == tab[13]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+14] == tab[14]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+15] == tab[15]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+16] == tab[16]
						
							)
						{
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//���� ʱ����³ɹ�
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
							
							return it;   //���� д���λ����Ϣ
					
						}
						else
						{
						tab_dat[(Dat_Num*TypeB_Data_Len)+9] = tab[9];
						tab_dat[(Dat_Num*TypeB_Data_Len)+10] = tab[10];
						tab_dat[(Dat_Num*TypeB_Data_Len)+11] = tab[11];
						tab_dat[(Dat_Num*TypeB_Data_Len)+12] = tab[12];
						tab_dat[(Dat_Num*TypeB_Data_Len)+13] = tab[13];
						tab_dat[(Dat_Num*TypeB_Data_Len)+14] = tab[14];
						tab_dat[(Dat_Num*TypeB_Data_Len)+15] = tab[15];
						tab_dat[(Dat_Num*TypeB_Data_Len)+16] = tab[16];
						
						//������ʱ�����ݺ��޸ĺ������д��ԭ������  �������
						
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeB_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //���µ�������д��Ŀ¼ ����û���������ݣ������������� ���Բ������������


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//���� ʱ����³ɹ�
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
						
						return it;   //���� д���λ����Ϣ
						
					}
					else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
										(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)	
									)
									{
										TypeB_Data_Num_1++;
										
										if(TypeB_Data_Num_1==TypeB_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
										{
											
											Dat_Num =A_Sector_TypeB_Data_Num;			//���ڽ�������ѭ��
											Sector_Num=AD_TypeB_Data_Sector_Num;   //���ڽ�����������ѭ��
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"AD_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//����У��� typeA ������
											
											#endif
											
										}
									
									}
									
						if(TypeB_Data_Num ==0)   //û�б�������ʱ  ֱ�ӽ���ѭ��
							{
									Dat_Num =A_Sector_TypeB_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=AD_TypeB_Data_Sector_Num;   //���ڽ�����������ѭ��
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"AD_TypeB_Null!\r\n");   //��ʾ�ռ�������
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"AD_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//����У��� typeA ������(���еı�������)
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
		{
			
			Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //��ȡ������������
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	��������Ѱ�ҿ��е�����λ��
				{

					
					if(
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]>A_Sector_TypeB_Data_Num)
						
						)		//����һ�������ڵ����ݿ�ռ����ʱ����������д���������
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_AD_TypeB_Data!\r\n");
								
								#endif
							tab[TypeB_Data_Len-1] = Dat_Num+1;  //��tab[13] ���� �ռ����к� ����д��
							
							for(i =0;i<TypeB_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeB_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //���µ�������д��Ŀ¼ �������������ݣ�������Ҫ����

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[TypeB_Data_Len-1];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_AD_TypeB_ID: %x ! \r\n",it);  //����д���ID�� �����RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
}

/*****************************************************************************

uint8_t Add_Flash_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


д��һ���û�B����Ϣ�� ��Ӧ�洢����

���������ʽ��
���˿���
0XAA+8�ֽ�B����Ϣ(���к�)+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� + 1�ֽ�SN�� ÿ������18�ֽ�
����Ա����
0XAA+8�ֽ�B����Ϣ�����кţ�+4�ֽ���ʼʱ��+4�ֽڹ���ʱ�� + 1�ֽ�SN�� ÿ������18�ֽ�

һ�������ɱ��湲��145������

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //����A�����ݵ�����1 (Ŀǰ����1������)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //����B�����ݵ�����1 (Ŀǰ����1������)


const uint8_t TypeA_Data_Len = 13;  	//һ��A�����ݵĳ���

const uint8_t TypeB_Data_Len = 17;		//һ��B�����ݵĳ���


*****************************************************************************/
/*
uint8_t Add_Flash_US_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeB_Data_Len];  //����
			
			uint32_t is1,is2;
			
		for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
				tab_dat[i] =0;
			
			for(i =0;i<8;i++)  //��uidֵ���� tab����
			{
					tab[i] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			len1 =0;
			len1 =0;
			
			TypeB_Data_Num = Get_Cat_Dat ( US_TypeB_Data_Num_Code ); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
				if( TypeB_Data_Num==0XFFFFFF )
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_Num_Error!\r\n");
					
					#endif
					
					return 0;
					
				}
				
			len = TypeB_Data_Num * TypeB_Data_Len;  //�ó�ռ�õĻ�����
	
			if(len>=TypeB_Data_Len)  //��len���� ���ڵ���һ�� TypeA_Data ���� ���ѱ���������ʱ
			{
				
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
			
				len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
				
				for(i =0;i<len;)
				{
					if(tab_dat[i]==0XAA) //��ͷ����ʱ
					{
						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
							(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])) //��������һ�µ�uidʱ ��len1 ����Ϊһ��uid����ʼ��ַ
						{
							//len1 = i;
							i = len;  //��i��ֵ����len1 ��ͨ����i��ֵ�ó�len �Խ���֮���ѭ��
						}
						else
						{
							i+=TypeB_Data_Len;  //��ѯ��һ��uid���л�����һ����ʼ��ַ
							len1 = i;					//ͬʱ���� len1 ��ֵ
						}
					}
					else
					{
						i = len;
						len1 =len;
					}
				}
				
				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
					&&(TypeB_Data_Num<A_Sector_TypeB_Data_Num)) //��len1����lenʱ����ʾ��������û��һ�µĿ���Ϣ
				{
					
					for(i =8;i>=1;i--)   //������д��ͷ
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //д��ͷ
					
					tab[9] = start_Unix>>24&0xFF;
					tab[10] = start_Unix>>16&0xFF;
					tab[11] = start_Unix>>8&0XFF;
					tab[12] = start_Unix&0xFF;
					
					tab[13]  =  stop_Unix>>24&0XFF;
					tab[14] =  stop_Unix>>16&0XFF;
					tab[15] =  stop_Unix>>8&0XFF;
					tab[16] =  stop_Unix&0xFF;
					
					for(is2 =1;is2<=A_Sector_TypeB_Data_Num;is2++)
					{
						for(is1 =1;is1<=TypeB_Data_Num;is1++)  //��һ������ Type A ������������Ѱ�ҿ��õ�SN��
						{
							if(tab_dat[is2*TypeB_Data_Len-1]==is2)//��SN�Ŵ���ʱ ����ѭ��
							{
								break;
							}
							
						}
						
						if(is2>TypeB_Data_Num)
						{
							break;
						}
						
						if(is2>A_Sector_TypeB_Data_Num)  //�� ���п���SN������ʱ������д�����������ʧ����Ϣ 
							return 0;
					}
					
					tab[TypeB_Data_Len-1] = is2;  //����ȡ���� SN ��д�� tab ���� ��д��
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
					for(i = len1;i<(len1+TypeB_Data_Len);i++)
					{
						tab_dat[i] = tab[i-len1];  //��tabֵ���뵽tab_dat��				
					}
					
					if(tab_dat[len1]==0xAA)
					{
						
							Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //������̬������������
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeB_Data_Len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
							SPI_Flash_Busy();
							
							Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //���µ�������д��Ŀ¼
							
							return tab[TypeB_Data_Len-1];
					}
					return 0;
				}
				else if(len1<=len-TypeB_Data_Len)  //��len1 С�� len �� TypeA_Data_Len ˵����ID���ڱ����Ѵ��ڣ�ֻ����ʱ�䲻������������
					{
						
					if(tab_dat[len1]==0xAA)  //�����ݰ���ʼֵ��������ʱ
						{
							
							tab_dat[len1+9] = start_Unix>>24&0xFF;
							tab_dat[len1+10] = start_Unix>>16&0xFF;
							tab_dat[len1+11] = start_Unix>>8&0XFF;
							tab_dat[len1+12] = start_Unix&0xFF;
							
							tab_dat[len1+13]  =  stop_Unix>>24&0XFF;
							tab_dat[len1+14] =  stop_Unix>>16&0XFF;
							tab_dat[len1+15] =  stop_Unix>>8&0XFF;
							tab_dat[len1+16] =  stop_Unix&0xFF;
							
							Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //������̬������������
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //д���޸ĺ�����ݵ���̬�������� �����ڿ�ID���Ѵ��ڣ��ʹ���ԱTypeA �������������ı䣩
							SPI_Flash_Busy();
							
							return tab_dat[len1+TypeB_Data_Len-1];  //���ص�ǰ���ݵ�SN��
							
						}
						
					}
				else if (TypeB_Data_Num>=A_Sector_TypeB_Data_Num) //���洢�Ŀ����ݴ��ڵ��ڵ�������(һ������²����ܴ���)������д������
				{
					
					#ifdef uart_start
					SEGGER_RTT_printf(0,"US_TypeB_Data_Sector1 Full!\r\n");
					#endif
					
					return 0;
					
				}
				
			}
			else if(len==0)  //��lenֵΪ��ʱ��ֱ���ڵ�һ��λ�ü����µ�
			{
				  
					for(i =8;i>=1;i--)   //������д��ͷ
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //д��ͷ
					
					tab[9] = start_Unix>>24&0xFF;
					tab[10] = start_Unix>>16&0xFF;
					tab[11] = start_Unix>>8&0XFF;
					tab[12] = start_Unix&0xFF;
					
					tab[13]  =  stop_Unix>>24&0XFF;
					tab[14] =  stop_Unix>>16&0XFF;
					tab[15] =  stop_Unix>>8&0XFF;
					tab[16] =  stop_Unix&0xFF;
					
					tab[17] = 1; //�����һ������ ��Ϊû������������ SN��Ϊ1
	
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
				for(i=0;i<(TypeB_Data_Len);i++)  //��Ϊ len ֵΪ 0 �� ����ֱ����tab_dat ����ʼλ��д���һ������
				{
					
					tab_dat[i] = tab[i];  //�� tab ֵ���뵽 tab_dat ��
					
				}
				
				Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //��������Ա��������1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(US_TypeB_Data_Num_Code,1);  //���µ�������д��Ŀ¼
				
				return tab[TypeB_Data_Len-1];  //�������һ�ֽڣ� SN ��
			}
			
			return 0;
			
}
*/



uint16_t Add_Flash_US_TypeB_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint32_t i,it; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[TypeB_Data_Len];  //����
	
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //��ʼǰ����ջ���
			tab[i] =0;
	
			tab[0] = 0xAA;		//�ڿ�ʼд������ͷ
	
			for(i =0;i<8;i++)  //��uidֵ���� tab����
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //���������ָ�uidָ��
			TypeB_Data_Num_1 =0;
			tab[9] = start_Unix>>24&0xFF;			//����ʱ��
			tab[10] = start_Unix>>16&0xFF;
			tab[11] = start_Unix>>8&0XFF;
			tab[12] = start_Unix&0xFF;
			
			tab[13]  =  stop_Unix>>24&0XFF;
			tab[14] =  stop_Unix>>16&0XFF;
			tab[15] =  stop_Unix>>8&0XFF;
			tab[16] =  stop_Unix&0xFF;
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
			if(TypeB_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeB_Data_Num_Error!\r\n");					//����ȡ��������ֵΪ 0XFFFFFFʱ��˵��������������ݽṹ�쳣�����ټ���д��
				
				#endif
				
				return 0;
				
			}
			
			if(TypeB_Data_Num ==(US_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		// ���������ﵽ����ʱ������д�� 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: US_TypeB_Data_Write Full!\r\n");   //��ʾ���ݴﵽ���޾���
				
				#endif
				
				return 0;
				
			}
			else if(TypeB_Data_Num >(US_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		//  ����������������ʱ�������쳣��Ϣ���Ҳ���д��
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeB_Data_Write OverFlow!\r\n");   //��ʾ���ݳ������޴���
				
				#endif
				
				
				return 0;
			
			}
			
			//��һ�о���ʱ��ʼ�����ڵ�һ������д������
			
			//������������Ѱ���Ƿ���һ�µĿ��ţ�����У����䶯λ�ã�ֱ�Ӹ���ʱ��
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
			
				Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&	// Type_B ���а��ֽ�ID
						(tab_dat[(Dat_Num*TypeB_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+5]==tab[5])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+6]==tab[6])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+7]==tab[7])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+8]==tab[8])&&
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
						
						)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
					{
						//����ͬ����Ч����ʱ������ʱ������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//���� ʱ�����ֵ
						
						#endif
						
						  //��ʼ����ʱ������
						if(			//��ʱ��δ�����仯ʱ�������дflash
							tab_dat[(Dat_Num*TypeB_Data_Len)+9] == tab[9]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+10] == tab[10]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+11] == tab[11]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+12] == tab[12]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+13] == tab[13]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+14] == tab[14]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+15] == tab[15]&&
							tab_dat[(Dat_Num*TypeB_Data_Len)+16] == tab[16]
						
							)
						{
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//���� ʱ����³ɹ�
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
							
							return it;   //���� д���λ����Ϣ
					
						}
						else
						{
						tab_dat[(Dat_Num*TypeB_Data_Len)+9] = tab[9];
						tab_dat[(Dat_Num*TypeB_Data_Len)+10] = tab[10];
						tab_dat[(Dat_Num*TypeB_Data_Len)+11] = tab[11];
						tab_dat[(Dat_Num*TypeB_Data_Len)+12] = tab[12];
						tab_dat[(Dat_Num*TypeB_Data_Len)+13] = tab[13];
						tab_dat[(Dat_Num*TypeB_Data_Len)+14] = tab[14];
						tab_dat[(Dat_Num*TypeB_Data_Len)+15] = tab[15];
						tab_dat[(Dat_Num*TypeB_Data_Len)+16] = tab[16];
						
						//������ʱ�����ݺ��޸ĺ������д��ԭ������  �������
						
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeB_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //���µ�������д��Ŀ¼ ����û���������ݣ������������� ���Բ������������


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//���� ʱ����³ɹ�
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
						
						return it;   //���� д���λ����Ϣ
						
					}
					else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
										(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)	
									)
									{
										TypeB_Data_Num_1++;
										
										if(TypeB_Data_Num_1==TypeB_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
										{
											
											Dat_Num =A_Sector_TypeB_Data_Num;			//���ڽ�������ѭ��
											Sector_Num=US_TypeB_Data_Sector_Num;   //���ڽ�����������ѭ��
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"US_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//����У��� typeA ������
											
											#endif
											
										}
									
									}
									
						if(TypeB_Data_Num ==0)   //û�б�������ʱ  ֱ�ӽ���ѭ��
							{
									Dat_Num =A_Sector_TypeB_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=US_TypeB_Data_Sector_Num;   //���ڽ�����������ѭ��
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"US_TypeB_Null!\r\n");   //��ʾ�ռ�������
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"US_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//����У��� typeA ������(���еı�������)
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
		{
			
			Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //��ȡ������������
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	��������Ѱ�ҿ��е�����λ��
				{

					
					if(
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]>A_Sector_TypeB_Data_Num)
						
						)		//����һ�������ڵ����ݿ�ռ����ʱ����������д���������
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_US_TypeB_Data!\r\n");
								
								#endif
							tab[TypeB_Data_Len-1] = Dat_Num+1;  //��tab[13] ���� �ռ����к� ����д��
							
							for(i =0;i<TypeB_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeB_Data_Secotr1+Sector_Num));  //��ȡƫ�ƺ��������������
						
						SPIFlash_Erase_Sector(Block,Sector); 		//��������
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //���µ�������д��Ŀ¼ �������������ݣ�������Ҫ����

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[TypeB_Data_Len-1];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_US_TypeB_ID: %x ! \r\n",it);  //����д���ID�� �����RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
}



/***********************************************

���ܣ�
ɾ��ָ�� ��� ����Ա Type_A ������

���룺
		AD_Type_A_SN		��Ӧ�� ����Ա Type_A ������λ��

�����

 1 ɾ���ɹ�
 0 ɾ��ʧ��

***********************************************/

//uint8_t Del_AD_Type_A(uint8_t AD_Type_A_SN)
//{
//	
//			uint32_t len;  //�����ܳ��Ⱥͺ���
//			uint32_t Block,Page,Sector;  //��ȡFlash�����������
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
////			uint8_t tab[50];  //����

////			uint32_t is1,is2;
//	
////			for(i =0;i<sizeof(tab);i++)
////				tab[i] =0;
////			
//			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
//				tab_dat[i] =0;
//	
//			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
//	
//				if(TypeA_Data_Num==0XFFFFFF)
//				{
//					#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"AD_TypeA_Data_Num_Error!\r\n");
//					
//					#endif
//					
//					return 0;
//					
//				}
//			
//			len = TypeA_Data_Num * TypeA_Data_Len;  //�ó�ռ�õĻ�����
//			
//			Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
//			
//			len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
//			
//			for(i =TypeA_Data_Len;i<=len;i+=TypeA_Data_Len)
//			{
//				if(tab_dat[i-1]==AD_Type_A_SN)  //�� �������ݵ� SN �� �� Ҫɾ����Ŀ�� SN ��һ��ʱ
//				{
//				
//					break;
//				
//				}				
//			}
//			
//			if(i>len)  //�������������Ѵ��ڱ��ر������ݵķ�Χ ���� 0 ��ʾû�� SN һ�µ�����
//			{
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"AD_Type_A_SN_Full!\r\n");  //��ʾ ��һ�� SN ��
//				
//				#endif
//				
//				return 0;
//			
//			}
//			
//			//������һ�µ� SN ʱ
//			
//			len1 =i;  //�� i ֵ���� len1
//			
//			#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"tab_dat_AD_Type_A_SN:%d\r\n",tab_dat[len1-1]);
//			
//			#endif
//			
//			//�ú�������ݸ��� ��Ӧ SN�ŵ�����
//			/********************************************************************************
//			��ע ��ʹ���һ��������Ȼ�ڻ����У�
//			��������д��Flash ʱ���ȼ���һ���ֽڳ��ȣ�д����ٴζ�ȡFlashʱ���һ�����ݲ�������
//			********************************************************************************/
//			
//			for(i =len1;i<len;i++)
//				tab_dat[i-TypeA_Data_Len] = tab_dat[i];
//			
//				
//				Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //��������Ա��������1
//				
//				SPIFlash_Erase_Sector(Block,Sector);
//				
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
//				
//				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeA_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num-1);  //���µ�������д��Ŀ¼
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"Del_A_US_TypeA_Card!\r\n");
//				
//				#endif
//				
//				return 1;
//				
//}


/***********************************************

���ܣ�
ɾ��ָ�� ��� ����Ա Type_A ������

���룺
		TypeA_ID		��Ӧ�� ����Ա Type_A �� ID ��ָ��

�����

 1 ɾ���ɹ�
 0 ɾ��ʧ��
 2 TypeA ���Ų�����
 3 ������
 
 NO.202002
 
***********************************************/

uint8_t Del_AD_Type_A(uint8_t *TypeA_ID)
{
			uint32_t Block,Page;  //��ȡFlash�����������
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			
			uint8_t tab[5];
			
			TypeA_Data_Num_1 =0;
			
			tab[0] = 0xAA;		//�Ƚ�����ͷ����
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeA_ID;
					
					TypeA_ID++;
			
			}
			
			TypeA_ID-=sizeof(tab);		//�ָ�ָ��
			
			
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nAD_Type_A_Num: %d !\r\n",TypeA_Data_Num);		//�����ǰ�����������
				
				SEGGER_RTT_printf(0,"Start_Cark_AD_Type_A_ID !\r\n");		//��ʼѰ����Ҫɾ���� Type A ��ID
				
				#endif
			
			if(TypeA_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_Type_A_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // ��������ʱ���ؿ�
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
				
				Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
						if(
							(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
							
							
							)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_A_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeA_Data_Len)+TypeA_Data_Len-1]);		//��ֵͬ�� ID ��
										
						#endif
									
							for(uint8_t i=0;i<TypeA_Data_Len;i++)		//��ո������� ��Ч���ݿռ� ��д��
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] =0;		//	ͨ�������Ӧλ�������ٴ�д����ʵ����������
								
							}
							//���޸ĺ������д�� Flash ͨ������ķ�ʽ����������� �������ã�
							
						Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num-1);    //д���µ����ݵ�Ŀ¼
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nAD_Type_A_Num: %d !\r\n\r\n",TypeA_Data_Num-1);

						#endif
						
						return 1;
							
						}
						else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
					
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
							)
							{
								TypeA_Data_Num_1++;
								
								if(TypeA_Data_Num_1==TypeA_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
								{
									
									Dat_Num =A_Sector_TypeA_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=AD_TypeA_Data_Sector_Num;   //���ڽ�����������ѭ��
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! AD_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//����У��� typeA ������
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // Ĭ�Ϸ���ʧ��
}



///***********************************************

//���ܣ�
//ɾ��ָ�� ��� �û� Type_A ������

//���룺
//		US_Type_A_SN		��Ӧ�� �û� Type_A ������λ��

//�����

// 1 ɾ���ɹ�
// 0 ɾ��ʧ��

//***********************************************/

//uint8_t Del_US_Type_A(uint8_t US_Type_A_SN)
//{

//			uint32_t len;  //�����ܳ��Ⱥͺ���
//			uint32_t Block,Page,Sector;  //��ȡFlash�����������
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
////			uint8_t tab[50];  //����

////			uint32_t is1,is2;
//	
////			for(i =0;i<sizeof(tab);i++)
////				tab[i] =0;
//			
//			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
//				tab_dat[i] =0;
//	
//			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
//	
//				if(TypeA_Data_Num==0XFFFFFF)
//				{
//					
//					#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"US_TypeA_Data_Num_Error!\r\n");
//					
//					#endif
//					
//					return 0;
//					
//				}	
//				
//			len = TypeA_Data_Num * TypeA_Data_Len;  //�ó�ռ�õĻ�����
//			
//			Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
//			
//			len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
//			
//			for(i =TypeA_Data_Len;i<=len;i+=TypeA_Data_Len)
//			{
//				if(tab_dat[i-1]==US_Type_A_SN)  //�� �������ݵ� SN �� �� Ҫɾ����Ŀ�� SN ��һ��ʱ
//				{
//				
//					break;
//				
//				}				
//			}
//			
//			if(i>len)  //�������������Ѵ��ڱ��ر������ݵķ�Χ ���� 0 ��ʾû�� SN һ�µ�����
//			{
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"US_Type_A_SN_Full!\r\n");  //��ʾ ��һ�� SN ��
//				
//				#endif
//				
//				return 0;
//			
//			}
//			
//			//������һ�µ� SN ʱ
//			
//			len1 =i;  //�� i ֵ���� len1
//			
//			#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"tab_dat_US_Type_A_SN:%d\r\n",tab_dat[len1-1]);
//			
//			#endif
//			
//			//�ú�������ݸ��� ��Ӧ SN�ŵ�����
//			/********************************************************************************
//			��ע ��ʹ���һ��������Ȼ�ڻ����У�
//			��������д��Flash ʱ���ȼ���һ���ֽڳ��ȣ�д����ٴζ�ȡFlashʱ���һ�����ݲ�������
//			********************************************************************************/
//			
//			for(i =len1;i<len;i++)
//				tab_dat[i-TypeA_Data_Len] = tab_dat[i];
//			
//				
//				Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //��������Ա��������1
//				
//				SPIFlash_Erase_Sector(Block,Sector);
//				
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//				
//				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeA_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num-1);  //���µ�������д��Ŀ¼
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"Del_A_US_TypeA_Card!\r\n");
//				
//				#endif
//				
//				return 1;
//				
//}


/***********************************************

���ܣ�
ɾ��ָ�� ��� ����Ա Type_A ������

���룺
		TypeA_ID		��Ӧ�� ����Ա Type_A �� ID �� ָ��

�����

 1 ɾ���ɹ�
 0 ɾ��ʧ��
 2 TypeA ���Ų�����
 3 ������
 
 NO.202002
 
***********************************************/

uint8_t Del_US_Type_A(uint8_t *TypeA_ID)
{
			uint32_t Block,Page;  //��ȡFlash�����������
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			
			uint8_t tab[5];
			
			TypeA_Data_Num_1 =0;
			
			tab[0] = 0xAA;		//�Ƚ�����ͷ����
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeA_ID;
					
					TypeA_ID++;
			
			}
			
			TypeA_ID-=sizeof(tab);		//�ָ�ָ��
			
			
			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nUS_Type_A_Num: %d !\r\n",TypeA_Data_Num);		//�����ǰ�����������
				
				SEGGER_RTT_printf(0,"Start_Cark_US_Type_A_ID !\r\n");		//��ʼѰ����Ҫɾ���� Type A ��ID
				
				#endif
			
			if(TypeA_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"US_Type_A_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // ��������ʱ���ؿ�
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
				
				Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
						if(
							(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
							
							
							)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_A_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeA_Data_Len)+TypeA_Data_Len-1]);		//��ֵͬ�� ID ��
										
						#endif
									
							for(uint8_t i=0;i<TypeA_Data_Len;i++)		//��ո������� ��Ч���ݿռ� ��д��
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] =0;		//	ͨ�������Ӧλ�������ٴ�д����ʵ����������
								
							}
							//���޸ĺ������д�� Flash ͨ������ķ�ʽ����������� �������ã�
							
						Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num-1);    //д���µ����ݵ�Ŀ¼
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nUS_Type_A_Num: %d !\r\n\r\n",TypeA_Data_Num-1);

						#endif
						
						return 1;
							
						}
						else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
					
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
							)
							{
								TypeA_Data_Num_1++;
								
								if(TypeA_Data_Num_1==TypeA_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
								{
									
									Dat_Num =A_Sector_TypeA_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=US_TypeA_Data_Sector_Num;   //���ڽ�����������ѭ��
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! US_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//����У��� typeA ������
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // Ĭ�Ϸ���ʧ��
	
	
}

/***********************************************

���ܣ�
ɾ��ָ�� ��� ����Ա Type_B ������

���룺
		AD_Type_B_SN		��Ӧ�� ����Ա Type_B ������λ��

�����

 1 ɾ���ɹ�
 0 ɾ��ʧ��

***********************************************/
/*
uint8_t Del_AD_Type_B(uint8_t AD_Type_B_SN)
{
	
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
//			uint8_t tab[50];  //����

//			uint32_t is1,is2;
	
//			for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
				tab_dat[i] =0;
	
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
	
			if(TypeB_Data_Num==0XFFFFFF)
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_Num_Error!\r\n");
					
					#endif
					
					return 0;
					
				}
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //�ó�ռ�õĻ�����
			
			Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
			
			len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
			
			for(i =TypeB_Data_Len;i<=len;i+=TypeB_Data_Len)
			{
				if(tab_dat[i-1]==AD_Type_B_SN)  //�� �������ݵ� SN �� �� Ҫɾ����Ŀ�� SN ��һ��ʱ
				{
				
					break;
				
				}				
			}
			
			if(i>len)  //�������������Ѵ��ڱ��ر������ݵķ�Χ ���� 0 ��ʾû�� SN һ�µ�����
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_Type_B_SN_Full!\r\n");  //��ʾ ��һ�� SN ��
				
				#endif
				
				return 0;
			
			}
			
			//������һ�µ� SN ʱ
			
			len1 =i;  //�� i ֵ���� len1
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"tab_dat_AD_Type_B_SN:%d\r\n",tab_dat[len1-1]);
			
			#endif
			
			//�ú�������ݸ��� ��Ӧ SN�ŵ�����
		//	*******************************************************************************
		//	��ע ��ʹ���һ��������Ȼ�ڻ����У�
		//	��������д��Flash ʱ���ȼ���һ���ֽڳ��ȣ�д����ٴζ�ȡFlashʱ���һ�����ݲ�������
		//	*******************************************************************************
			
			for(i =len1;i<len;i++)
				tab_dat[i-TypeB_Data_Len] = tab_dat[i];
			
				
				Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //��������Ա��������1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeB_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num-1);  //���µ�������д��Ŀ¼
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Del_A_AD_TypeB_Card!\r\n");
				
				#endif
				
				return 1;
				
}
*/


uint8_t Del_AD_Type_B(uint8_t *TypeB_ID)
{
			uint32_t Block,Page;  //��ȡFlash�����������
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint8_t tab[9];
			
			TypeB_Data_Num_1 =0;
				
			for(uint8_t i;i<sizeof(tab);i++)			//��ջ���
			{
				tab[i] =0x00;
			}
	
			
			tab[0] = 0xAA;		//�Ƚ�����ͷ����
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeB_ID;
					
					TypeB_ID++;
			
			}
			
			TypeB_ID-=sizeof(tab);		//�ָ�ָ��
			
			
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nAD_Type_B_Num: %d !\r\n",TypeB_Data_Num);		//�����ǰ�����������
				
				SEGGER_RTT_printf(0,"Start_Cark_AD_Type_B_ID !\r\n");		//��ʼѰ����Ҫɾ���� Type A ��ID
				
				#endif
			
			if(TypeB_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_Type_B_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // ��������ʱ���ؿ�
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
				
				Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
						if(
							(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+1]==tab[1])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+2]==tab[2])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+3]==tab[3])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+4]==tab[4])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+5]==tab[5])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+6]==tab[6])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+7]==tab[7])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+8]==tab[8])&&
						
							(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
							
							
							)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_B_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1]);		//��ֵͬ�� ID ��
										
						#endif
									
							for(uint8_t i=0;i<TypeB_Data_Len;i++)		//��ո������� ��Ч���ݿռ� ��д��
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] =0;		//	ͨ�������Ӧλ�������ٴ�д����ʵ����������
								
							}
							//���޸ĺ������д�� Flash ͨ������ķ�ʽ����������� �������ã�
							
						Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num-1);    //д���µ����ݵ�Ŀ¼
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nAD_Type_B_Num: %d !\r\n\r\n",TypeB_Data_Num-1); //���ظ��º����������

						#endif
						
						return 1;
							
						}
						else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
							)
							{
								TypeB_Data_Num_1++;
								
								if(TypeB_Data_Num_1==TypeB_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
								{
									
									Dat_Num =A_Sector_TypeB_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=AD_TypeB_Data_Sector_Num;   //���ڽ�����������ѭ��
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! AD_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//����У��� typeA ������
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // Ĭ�Ϸ���ʧ��
}




/***********************************************

���ܣ�
ɾ��ָ�� ��� �û� Type_B ������

���룺
		AD_Type_B_SN		��Ӧ�� ����Ա Type_B ������λ��

�����

 1 ɾ���ɹ�
 0 ɾ��ʧ��

***********************************************/
/*
uint8_t Del_US_Type_B(uint8_t US_Type_B_SN)
{
	
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
//			uint8_t tab[50];  //����

//			uint32_t is1,is2;
	
//			for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
//			
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
				tab_dat[i] =0;
	
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
	
			if(TypeB_Data_Num==0XFFFFFF)
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_Num_Error!\r\n");
					
					#endif
					
					return 0;
					
				}
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //�ó�ռ�õĻ�����
			
			Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
			
			len1 =0; //��ʼʱ��len1 ֵ�õ���iһ��
			
			for(i =TypeB_Data_Len;i<=len;i+=TypeB_Data_Len)
			{
				if(tab_dat[i-1]==US_Type_B_SN)  //�� �������ݵ� SN �� �� Ҫɾ����Ŀ�� SN ��һ��ʱ
				{
				
					break;
				
				}				
			}
			
			if(i>len)  //�������������Ѵ��ڱ��ر������ݵķ�Χ ���� 0 ��ʾû�� SN һ�µ�����
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"US_Type_B_SN_Full!\r\n");  //��ʾ ��һ�� SN ��
				
				#endif
				
				return 0;
			
			}
			
			//������һ�µ� SN ʱ
			
			len1 =i;  //�� i ֵ���� len1
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"tab_dat_US_Type_B_SN:%d\r\n",tab_dat[len1-1]);
			
			#endif
			
			//�ú�������ݸ��� ��Ӧ SN�ŵ�����
		//	********************************************************************************
		//	��ע ��ʹ���һ��������Ȼ�ڻ����У�
		//	��������д��Flash ʱ���ȼ���һ���ֽڳ��ȣ�д����ٴζ�ȡFlashʱ���һ�����ݲ�������
		//	********************************************************************************
			
			for(i =len1;i<len;i++)
				tab_dat[i-TypeB_Data_Len] = tab_dat[i];
			
				
				Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //��������Ա��������1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeB_Data_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num-1);  //���µ�������д��Ŀ¼
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Del_A_US_TypeB_Card!\r\n");
				
				#endif
				
				return 1;
				
}
*/

uint8_t Del_US_Type_B(uint8_t *TypeB_ID)
{
			uint32_t Block,Page;  //��ȡFlash�����������
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint8_t tab[9];
			
			TypeB_Data_Num_1 =0;
				
			for(uint8_t i;i<sizeof(tab);i++)			//��ջ���
			{
				tab[i] =0x00;
			}
	
			
			tab[0] = 0xAA;		//�Ƚ�����ͷ����
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeB_ID;
					
					TypeB_ID++;
			
			}
			
			TypeB_ID-=sizeof(tab);		//�ָ�ָ��
			
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nUS_Type_B_Num: %d !\r\n",TypeB_Data_Num);		//�����ǰ�����������
				
				SEGGER_RTT_printf(0,"Start_Cark_US_Type_B_ID !\r\n");		//��ʼѰ����Ҫɾ���� Type A ��ID
				
				#endif
			
			if(TypeB_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"US_Type_B_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // ��������ʱ���ؿ�
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  ��ʾƫ�Ƶ�������
			{
				
				Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //��ȡ������������
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	����Ч������Ѱ���Ƿ���һ�µ�ID
				{
						if(
							(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+1]==tab[1])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+2]==tab[2])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+3]==tab[3])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+4]==tab[4])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+5]==tab[5])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+6]==tab[6])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+7]==tab[7])&&
							(tab_dat[(Dat_Num*TypeB_Data_Len)+8]==tab[8])&&
						
							(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
							
							
							)		//����һ�������ڵ����ݿ������Ƿ���Ч�������д��ֵһ��ʱ  ��������
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_B_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1]);		//��ֵͬ�� ID ��
										
						#endif
									
							for(uint8_t i=0;i<TypeB_Data_Len;i++)		//��ո������� ��Ч���ݿռ� ��д��
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] =0;		//	ͨ�������Ӧλ�������ٴ�д����ʵ����������
								
							}
							//���޸ĺ������д�� Flash ͨ������ķ�ʽ����������� �������ã�
							
						Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num-1);    //д���µ����ݵ�Ŀ¼
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nUS_Type_B_Num: %d !\r\n\r\n",TypeB_Data_Num-1); //���ظ��º����������

						#endif
						
						return 1;
							
						}
						else if(			// ��������Ч���ݽ���ͳ�� �������ﵽ�˼�ֹͣ ����  ��Լд��ʱ��
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
							)
							{
								TypeB_Data_Num_1++;
								
								if(TypeB_Data_Num_1==TypeB_Data_Num)  //���жϵ��������뱣���һ��ʱ���������ѭ��
								{
									
									Dat_Num =A_Sector_TypeB_Data_Num;			//���ڽ�������ѭ��
									Sector_Num=US_TypeB_Data_Sector_Num;   //���ڽ�����������ѭ��
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! US_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//����У��� typeA ������
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // Ĭ�Ϸ���ʧ��
}


/**********************************************************************************

uint32_t Need_TypeA_Data_SN

�ж� ���� �Ƿ��� ��ͬ�� A ������

 ���룺 Need_Uid

 4�ֽ� TypeA UID ��ָ�� 

���أ� �� UID �Ų�����ʱ ���� 0 ����ʱ���� TypeA��SN��
ȡ�� uint32_t ���� 

���λ���ֽ� 1 ʱ��ʾ���Ϊ ����Ա ���λ���ֽ�Ϊ 2 ʱ��ʾ���Ϊ �û� 

**********************************************************************************/

/*��������Чʱ���ڱ��� ������Ч���� ����Чʱ�� ��һ�ֽ����ڱ�ʾ �Ƿ���Ч��

01 ��ʾ ��Ч ����A ��ʱ�� 
02 ��ʾ ��Ч �û�A ��ʱ�� 
03 ��ʾ ��Ч ����B ��ʱ�� 
04 ��ʾ ��Ч �û�B ��ʱ�� 

������Ч
*/
//uint8_t TypeAB_Unix_T[9];

//uint32_t Need_TypeA_Data_SN(uint8_t *Need_Uid)
//{
//			uint32_t len;  //�����ܳ��Ⱥͺ���
//			uint32_t Block,Page;  //��ȡFlash�����������
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
//			uint8_t tab[50];  //����

//			for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
//			
//			for(i =0;i<sizeof(TypeAB_Unix_T);i++)
//				TypeAB_Unix_T[i] =0;
//	
//			for(i =0;i<4;i++)
//			{
//				
//					tab[i] = *Need_Uid;
//					Need_Uid++;
//				
//			}
//		
//			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
//	
//			
//			len = TypeA_Data_Num * TypeA_Data_Len;  //�ó�ռ�õĻ�����
//			
//			Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
//			
//			i =0;
//			
//			for(i =0;i<len;i+=TypeA_Data_Len)
//			{
//				
//				if(
//					
//					(tab_dat[i]==0xAA)&&																//�ж�ͷ��ȷʱ
//					(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
//					(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])
//				
//					)
//						//����һ�� ����Ա �����ڱ��ر���ʱ
//					
//					{
//						
//						TypeAB_Unix_T[0] = 0x01;		//��ʾ���ſ��ǹ���A �� ��ʾʱ���ָ��Ŀ���
//						
//						TypeAB_Unix_T[1] = tab_dat[i+5];
//						
//						TypeAB_Unix_T[2] = tab_dat[i+6];
//						
//						TypeAB_Unix_T[3] = tab_dat[i+7];
//						
//						TypeAB_Unix_T[4] = tab_dat[i+8];
//						
//						TypeAB_Unix_T[5] = tab_dat[i+9];
//						
//						TypeAB_Unix_T[6] = tab_dat[i+10];
//						
//						TypeAB_Unix_T[7] = tab_dat[i+11];
//						
//						TypeAB_Unix_T[8] = tab_dat[i+12];
//						
//						
//							//��ǰΪ ����Ա TypeA �� ����߰�λ����1
//						
//						return tab_dat[i+TypeA_Data_Len-1]+0x1000000; //���� ��Ч�� ����Ա SN ��
//						
//					}
//				
//			}
//			
//			
//			//������Ա�������в�����ʱ ����Ѱ���û�������
//			
//			
//			
//			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);
//			
//			len = TypeA_Data_Num * TypeA_Data_Len;
//			
//			Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);
//			
//			i =0;
//			
//			for( i =0;i<len;i+=TypeA_Data_Len )
//			{
//				
//				if(
//				
//					(tab_dat[i]==0xAA)&&
//					(tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&
//					(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])
//				
//					)
//				
//					//��������һ�� �û� �����ڱ��ر���ʱ
//				
//				{
//						
//						
//						TypeAB_Unix_T[0] = 0x01;   //��ʾ���ſ����û�A �� ��ʾʱ���ָ��Ŀ���
//						
//						TypeAB_Unix_T[1] = tab_dat[i+5];
//						
//						TypeAB_Unix_T[2] = tab_dat[i+6];
//						
//						TypeAB_Unix_T[3] = tab_dat[i+7];
//						
//						TypeAB_Unix_T[4] = tab_dat[i+8];
//						
//						TypeAB_Unix_T[5] = tab_dat[i+9];
//						
//						TypeAB_Unix_T[6] = tab_dat[i+10];
//						
//						TypeAB_Unix_T[7] = tab_dat[i+11];
//						
//						TypeAB_Unix_T[8] = tab_dat[i+12];
//					
//					
//						//��ǰΪ �û� Type A �� ����߰�λ����2
//					
//					
//					return tab_dat[i+TypeA_Data_Len-1]+0x2000000;		//���� ��Ч�� �û� SN ��
//					
//					
//				}
//				
//			}
//			
//			return 0;
//			
//}

/*********************************************

Need_TypeA_Data_SN

Ѱ������������ �Ƿ���һ�µ� ID  

���룺  ��ҪѰ�ҵ�ID��

���أ�  ID ���ݶ�Ӧ�� SN�� �� δ�ҵ� ��ȷ ID��ʱ������ 0 ֮�����Ч����

��������Ч SN ��ʱ  ˵��һ�µ�ID �ű����� tab_dat ���� �� �� SN �� λ��

��������ڻ���Ҫ�ж�����ʱ�����ܸĶ��򸲸� tab_dat ������������



************************************************/

uint8_t TypeAB_Unix_T[9];

/***********************

��������Чʱ���ڱ��� ������Ч���� ����Чʱ�� ��һ�ֽ����ڱ�ʾ �Ƿ���Ч��

01 ��ʾ ��Ч ����A ��ʱ�� 
02 ��ʾ ��Ч �û�A ��ʱ�� 
03 ��ʾ ��Ч ����B ��ʱ�� 
04 ��ʾ ��Ч �û�B ��ʱ�� 

������Ч

***********************/




uint32_t Need_TypeA_Data_SN(uint8_t *Need_Uid)
{
//			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page;  //��ȡFlash�����������
			
			uint32_t TypeA_Data_Num;
			uint32_t Cheak_Card_Num;		//У�Ե� ��Ƭ����
			uint32_t i; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[50];  //����

			for(i =0;i<sizeof(tab);i++)			//������ݻ���
				tab[i] =0;
			
			for(i =0;i<sizeof(TypeAB_Unix_T);i++)    //���ʱ�仺��
				TypeAB_Unix_T[i] =0;
	
			for(i =0;i<4;i++)				// ����ҪѰ�ҵ� Uid �������뵽 tab ����  
			{
				
					tab[i] = *Need_Uid;
					Need_Uid++;
				
			}
			
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);			//��ȡ ����Ա TypeA ����
			Cheak_Card_Num =0;
			//��ʼ�жϱ����Ƿ����һ�µĹ���ԱA��
			
			for(uint8_t Sector_Num=0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1+Sector_Num);  //���������Ż�ȡ�����ʼҳ  ��ȡ�������������� AD_TypeA_Data_Sector1 �� Sector_Num ƫ��λ��
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));					//��ȡָ������������
				
				i =0;
				
				for(i =0;i<(TypeA_Data_Len*A_Sector_TypeA_Data_Num);i+=TypeA_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(TypeA_Data_Num>0)		//ȷ�������ݴ���		
						
						)		
					// ����һ�µ� ����ԱA������ʱ
					{
						  TypeAB_Unix_T[0] = 0X01;		//��ʾ���ſ��� ����Ա A �� ��ʾʱ���ָ��Ŀ���
						  
							TypeAB_Unix_T[1] = tab_dat[i+5];
							
							TypeAB_Unix_T[2] = tab_dat[i+6];
							
							TypeAB_Unix_T[3] = tab_dat[i+7];
							
							TypeAB_Unix_T[4] = tab_dat[i+8];
							
							TypeAB_Unix_T[5] = tab_dat[i+9];
							
							TypeAB_Unix_T[6] = tab_dat[i+10];
							
							TypeAB_Unix_T[7] = tab_dat[i+11];
							
							TypeAB_Unix_T[8] = tab_dat[i+12];
							
							
								//��ǰΪ ����Ա TypeA �� ����߰�λ����1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_AD_TypeA_Card!\r\n");		//��ʾ��ȡ��һ�µ� ����Ա A �� ����
						SEGGER_RTT_printf(0,"AD_TypeA_Sector: 0x%x!\r\n",AD_TypeA_Data_Secotr1+Sector_Num);  //�������ڵĹ���Ա A ������
						SEGGER_RTT_printf(0,"AD_TypeA_ID: 0x%x! \r\n\r\n",tab_dat[i+13]);   //�����ڹ���Ա A �������ڵı��
						
						
						#endif
							
							return tab_dat[i+TypeA_Data_Len-1]+0x1000000; //���� ��Ч�� ����Ա SN ��
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //����һ����Ч����  У������ ��һ
					
					if(Cheak_Card_Num>=TypeA_Data_Num)		//��У������� �ﵽ����ʱ  �������ѭ��
					{
						i = TypeA_Data_Len*A_Sector_TypeA_Data_Num;		// ����ѭ������ʵ�ֽ�������
						Sector_Num=AD_TypeA_Data_Sector_Num;
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n Cheak_AD_TypeA_Card_Ok! \r\n\r\n");
						
						#endif
						
					}
					
				}
				
				
			}
			
			//��ʼ�ж��Ƿ����һ�µ��û�A��
			
			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);			//��ȡ ����Ա TypeA ����
			Cheak_Card_Num =0;
			//��ʼ�жϱ����Ƿ����һ�µĹ���ԱA��
			
			for(uint8_t Sector_Num=0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1+Sector_Num);  //���������Ż�ȡ�����ʼҳ  ��ȡ�������������� AD_TypeA_Data_Sector1 �� Sector_Num ƫ��λ��
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));					//��ȡָ������������
				
				i =0;
				
				for(i =0;i<(TypeA_Data_Len*A_Sector_TypeA_Data_Num);i+=TypeA_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(TypeA_Data_Num>0)
						)		
					// ����һ�µ� ����ԱA������ʱ
					{
						  TypeAB_Unix_T[0] = 0X02;		//��ʾ���ſ��� �û� A �� ��ʾʱ���ָ��Ŀ���
						  
							TypeAB_Unix_T[1] = tab_dat[i+5];
							
							TypeAB_Unix_T[2] = tab_dat[i+6];
							
							TypeAB_Unix_T[3] = tab_dat[i+7];
							
							TypeAB_Unix_T[4] = tab_dat[i+8];
							
							TypeAB_Unix_T[5] = tab_dat[i+9];
							
							TypeAB_Unix_T[6] = tab_dat[i+10];
							
							TypeAB_Unix_T[7] = tab_dat[i+11];
							
							TypeAB_Unix_T[8] = tab_dat[i+12];
							
							
								//��ǰΪ ����Ա TypeA �� ����߰�λ����1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_US_TypeA_Card!\r\n");		//��ʾ��ȡ��һ�µ� ����Ա A �� ����
						SEGGER_RTT_printf(0,"US_TypeA_Sector: 0x%x!\r\n",US_TypeA_Data_Secotr1+Sector_Num);  //�������ڵĹ���Ա A ������
						SEGGER_RTT_printf(0,"US_TypeA_ID: 0x%x! \r\n\r\n",tab_dat[i+13]);   //�����ڹ���Ա A �������ڵı��
						
						
						#endif
							
							return tab_dat[i+TypeA_Data_Len-1]+0x2000000; //���� ��Ч�� �û�A SN ��
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //����һ����Ч����  У������ ��һ
					
					if(Cheak_Card_Num>=TypeA_Data_Num)		//��У������� �ﵽ����ʱ  �������ѭ��
					{
						i = TypeA_Data_Len*A_Sector_TypeA_Data_Num;		// ����ѭ������ʵ�ֽ�������
						Sector_Num=US_TypeA_Data_Sector_Num;
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n Cheak_US_TypeA_Card_Ok! \r\n\r\n");
						
						#endif
						
					}
					
				}
				
				
			}
			
			
			return 0;
	
}



/**********************************************************************************

uint32_t Need_TypeB_Data_SN

�ж� ���� �Ƿ��� ��ͬ�� B ������

 ���룺 Need_Uid

 4�ֽ� TypeB UID ��ָ�� 

���أ� �� UID �Ų�����ʱ ���� 0 ����ʱ���� TypeB��SN��
ȡ�� uint32_t ���� 

���λ���ֽ� 3 ʱ��ʾ���Ϊ ����Ա ���λ���ֽ�Ϊ 4 ʱ��ʾ���Ϊ �û� 

**********************************************************************************/

/*
uint32_t Need_TypeB_Data_SN(uint8_t *Need_Uid)
{
			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num;
			uint32_t i; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[50];  //����

			for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(TypeAB_Unix_T);i++)
				TypeAB_Unix_T[i] =0;
	
			for(i =0;i<8;i++)  //�� TypeB �� UID �����뻺��
			{
				
					tab[i] = *Need_Uid;
					Need_Uid++;
				
			}
		
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // �Ȼ�ȡ�����Ѵ��ڵ�TypeA ��������
	
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //�ó�ռ�õĻ�����
			
			Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);  //���������Ż�ȡ�����ʼҳ
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //��ȡ�������������ȳ�����������
			
			i =0;
			
			for(i =0;i<len;i+=TypeB_Data_Len)
			{
				
				if(
					
					(tab_dat[i]==0xAA)&&																//�ж�ͷ��ȷʱ
					(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
					(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
					(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
					(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])
					
					)
					
						//����һ�� ����Ա �����ڱ��ر���ʱ
					
					{
						
						TypeAB_Unix_T[0] = 0x03;		//��ʾ���ſ��ǹ���B �� ��ʾʱ���ָ��Ŀ���
						
						TypeAB_Unix_T[1] = tab_dat[i+9];
						
						TypeAB_Unix_T[2] = tab_dat[i+10];
						
						TypeAB_Unix_T[3] = tab_dat[i+11];
						
						TypeAB_Unix_T[4] = tab_dat[i+12];
						
						TypeAB_Unix_T[5] = tab_dat[i+13];
						
						TypeAB_Unix_T[6] = tab_dat[i+14];
						
						TypeAB_Unix_T[7] = tab_dat[i+15];
						
						TypeAB_Unix_T[8] = tab_dat[i+16];
						
						
							//��ǰΪ ����Ա TypeB �� ����߰�λ����1
						
						return tab_dat[i+TypeB_Data_Len-1]+0x3000000; //���� ��Ч�� ����Ա SN ��
						
					}
				
			}
			
			
			//������Ա�������в�����ʱ ����Ѱ���û�������
			
			
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);
			
			len = TypeB_Data_Num * TypeB_Data_Len;
			
			Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);
			
			i =0;
			
			for( i =0;i<len;i+=TypeB_Data_Len )
			{
				
				if(
				
					(tab_dat[i]==0xAA)&&
					(tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&
					(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
					(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
					(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])
					
					)
				
					//��������һ�� �û� �����ڱ��ر���ʱ
				
				{
						
						
						TypeAB_Unix_T[0] = 0x04;   //��ʾ���ſ����û� B �� ��ʾʱ���ָ��Ŀ���
						
						TypeAB_Unix_T[1] = tab_dat[i+9];
						
						TypeAB_Unix_T[2] = tab_dat[i+10];
						
						TypeAB_Unix_T[3] = tab_dat[i+11];
						
						TypeAB_Unix_T[4] = tab_dat[i+12];
						
						TypeAB_Unix_T[5] = tab_dat[i+13];
						
						TypeAB_Unix_T[6] = tab_dat[i+14];
						
						TypeAB_Unix_T[7] = tab_dat[i+15];
						
						TypeAB_Unix_T[8] = tab_dat[i+16];
					
					
						//��ǰΪ �û� Type A �� ����߰�λ����2
					
					
					return tab_dat[i+TypeB_Data_Len-1]+0x4000000;		//���� ��Ч�� �û� SN ��
					
					
				}
				
			}
			
			return 0;
			
}*/



uint32_t Need_TypeB_Data_SN(uint8_t *Need_Uid)
{
//			uint32_t len;  //�����ܳ��Ⱥͺ���
			uint32_t Block,Page;  //��ȡFlash�����������
			
			uint32_t TypeB_Data_Num;
			uint32_t Cheak_Card_Num;		//У�Ե� ��Ƭ����
			uint32_t i; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[50];  //����

			for(i =0;i<sizeof(tab);i++)			//������ݻ���
				tab[i] =0;
			
			for(i =0;i<sizeof(TypeAB_Unix_T);i++)    //���ʱ�仺��
				TypeAB_Unix_T[i] =0;
	
			for(i =0;i<8;i++)				// ����ҪѰ�ҵ� Uid �������뵽 tab ����  
			{
				
					tab[i] = *Need_Uid;
					Need_Uid++;
				
			}
			
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);			//��ȡ ����Ա TypeA ����
			Cheak_Card_Num =0;
			//��ʼ�жϱ����Ƿ����һ�µĹ���ԱA��
			
			for(uint8_t Sector_Num=0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1+Sector_Num);  //���������Ż�ȡ�����ʼҳ  ��ȡ�������������� AD_TypeA_Data_Sector1 �� Sector_Num ƫ��λ��
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));					//��ȡָ������������
				
				i =0;
				
				for(i =0;i<(TypeB_Data_Len*A_Sector_TypeB_Data_Num);i+=TypeB_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
						(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])&&
						(TypeB_Data_Num>0)		//ȷ�������ݴ���
						)		
					// ����һ�µ� ����ԱB������ʱ
					{
						  TypeAB_Unix_T[0] = 0X03;		//��ʾ���ſ��� ����Ա B �� ��ʾʱ���ָ��Ŀ���
						  
							TypeAB_Unix_T[1] = tab_dat[i+9];
							
							TypeAB_Unix_T[2] = tab_dat[i+10];
							
							TypeAB_Unix_T[3] = tab_dat[i+11];
							
							TypeAB_Unix_T[4] = tab_dat[i+12];
							
							TypeAB_Unix_T[5] = tab_dat[i+13];
							
							TypeAB_Unix_T[6] = tab_dat[i+14];
							
							TypeAB_Unix_T[7] = tab_dat[i+15];
							
							TypeAB_Unix_T[8] = tab_dat[i+16];
							
							
								//��ǰΪ ����Ա TypeA �� ����߰�λ����1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_AD_TypeB_Card!\r\n");		//��ʾ��ȡ��һ�µ� ����Ա A �� ����
						SEGGER_RTT_printf(0,"AD_TypeB_Sector: 0x%x!\r\n",AD_TypeB_Data_Secotr1+Sector_Num);  //�������ڵĹ���Ա A ������
						SEGGER_RTT_printf(0,"AD_TypeB_ID: 0x%x! \r\n\r\n",tab_dat[i+TypeB_Data_Len-1]);   //�����ڹ���Ա A �������ڵı��
						
						
						#endif
							
							return tab_dat[i+TypeB_Data_Len-1]+0x3000000; //���� ��Ч�� ����ԱB SN ��
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //����һ����Ч����  У������ ��һ
					
					if(Cheak_Card_Num>=TypeB_Data_Num)		//��У������� �ﵽ����ʱ  �������ѭ��
					{
						i = TypeB_Data_Len*A_Sector_TypeB_Data_Num;		// ����ѭ������ʵ�ֽ�������
						Sector_Num=AD_TypeB_Data_Sector_Num;
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n Cheak_AD_TypeB_Card_Ok! \r\n\r\n");
						
						#endif
						
					}
					
				}
				
				
			}
			
			//��ʼ�ж��Ƿ����һ�µ��û�B��
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);			//��ȡ ����Ա TypeB ����
			Cheak_Card_Num =0;
			//��ʼ�жϱ����Ƿ����һ�µĹ���ԱB��
			
			for(uint8_t Sector_Num=0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1+Sector_Num);  //���������Ż�ȡ�����ʼҳ  ��ȡ�������������� AD_TypeA_Data_Sector1 �� Sector_Num ƫ��λ��
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));					//��ȡָ������������
				
				i =0;
				
				for(i =0;i<(TypeB_Data_Len*A_Sector_TypeB_Data_Num);i+=TypeB_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
						(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])&&
						(TypeB_Data_Num>0)	//ȷ�������ݴ���
						)		
					// ����һ�µ� ����ԱB������ʱ
					{
						  TypeAB_Unix_T[0] = 0X04;		//��ʾ���ſ��� �û� B �� ��ʾʱ���ָ��Ŀ���
						  
							TypeAB_Unix_T[1] = tab_dat[i+9];
							
							TypeAB_Unix_T[2] = tab_dat[i+10];
							
							TypeAB_Unix_T[3] = tab_dat[i+11];
							
							TypeAB_Unix_T[4] = tab_dat[i+12];
							
							TypeAB_Unix_T[5] = tab_dat[i+13];
							
							TypeAB_Unix_T[6] = tab_dat[i+14];
							
							TypeAB_Unix_T[7] = tab_dat[i+15];
							
							TypeAB_Unix_T[8] = tab_dat[i+16];
							
							
								//��ǰΪ �û� TypeB �� ����߰�λ����1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_US_TypeB_Card!\r\n");		//��ʾ��ȡ��һ�µ� ����Ա A �� ����
						SEGGER_RTT_printf(0,"US_TypeB_Sector: 0x%x!\r\n",US_TypeB_Data_Secotr1+Sector_Num);  //�������ڵĹ���Ա A ������
						SEGGER_RTT_printf(0,"US_TypeB_ID: 0x%x! \r\n\r\n",tab_dat[i+TypeB_Data_Len-1]);   //�����ڹ���Ա A �������ڵı��
						
						
						#endif
							
							return tab_dat[i+TypeB_Data_Len-1]+0x4000000; //���� ��Ч�� �û�B SN ��
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //����һ����Ч����  У������ ��һ
					
					if(Cheak_Card_Num>=TypeB_Data_Num)		//��У������� �ﵽ����ʱ  �������ѭ��
					{
						i = TypeB_Data_Len*A_Sector_TypeB_Data_Num;		// ����ѭ������ʵ�ֽ�������
						Sector_Num=US_TypeB_Data_Sector_Num;
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n Cheak_US_TypeB_Card_Ok! \r\n\r\n");
						
						#endif
						
					}
					
				}
				
				
			}
			
			
			return 0;
	
}

/*****************************************************************************

���� ����Ա B �� ����
��� ����Ա B �� ���� ����


���м���� �û� A �� �µ�д��������ʹ��������������

�����  2020-0121

******************************************************************************/

void Erase_AD_TypeB_Sector()
{
	uint32_t Sector_Num;		//������
	uint32_t Block,Sector;  //��ȡFlash�����������
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_AD_TypeB_Sector!\r\n");
	
	#endif
	
	Sector_Num =AD_TypeB_Data_Secotr1;
	
	for(uint32_t i =0;i<AD_TypeB_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //��ȡƫ�ƺ��������������
						
		SPIFlash_Erase_Sector(Block,Sector); 		//��������
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_AD_TypeB_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}

/*****************************************************************************

���� ����Ա B �� ����
��� ����Ա B �� ���� ����


���м���� �û� A �� �µ�д��������ʹ��������������

�����  2020-0121

******************************************************************************/

void Erase_US_TypeB_Sector()
{
	uint32_t Sector_Num;		//������
	uint32_t Block,Sector;  //��ȡFlash�����������
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_US_TypeB_Sector!\r\n");
	
	#endif
	
	Sector_Num =US_TypeB_Data_Secotr1;
	
	for(uint32_t i =0;i<US_TypeB_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //��ȡƫ�ƺ��������������
						
		SPIFlash_Erase_Sector(Block,Sector); 		//��������
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_US_TypeB_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}

/**********************************************
��ָ������д��Flash

���룺
PS_Type: ָ������/��Դ������Ա �û� ��̬���룩
����Ա�� 0X01
�û���   0X02

���Զ�̬���룺 0x03

�����ⲿ�ֽ��������������ɢ���ʴ˳����Դ� Flash ��ʼ�ͽ���


**********************************************/

uint16_t Add_Flash_PS_Print (uint16_t PS_Type,uint16_t PS_ID,uint32_t start_Unix,uint32_t stop_Unix)
{
	
			uint32_t Block,Page,Sector;  //��ȡFlash�����������
			
			uint32_t PS_Print_Data_Num,PS_Print_Data_Num_1;
			uint32_t i; //len1 ���ڱ�ʶ�����uid�Ƿ��Ѵ��� len1<len ʱ ��ʾuid�Ѵ��ڣ�len1=len ʱ ��ʾuid������
			uint8_t tab[PS_Print_Len];  //����
			
			uint16_t PS_Print_Act;		//���Զ�̬���������ɾ����־  ��Чʱ��д�������������1
	
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nStart_Add_Flash_PS_Print!\r\n");
			SEGGER_RTT_printf(0,"\r\nAdd_Flash_PS_Print_ID:%x!\r\n",PS_ID);
			
			#endif
	
			if(SPI0_Start==0)
			Spi_Flash_Init();		
			else
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nError: SPI_Busy!\r\n");
				
				#endif
				
				return 0XFFFF;
				
			}

			PS_Print_Act =0;
			
			PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//��ȡ ָ�Ƶ� ��������
			
			for(i =0;i<sizeof(tab_dat);i++)  //��ʼǰ����ջ���
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //��ʼǰ����ջ���
			tab[i] =0;
			 
		//	0x55+2�ֽ�ָ������+2�ֽ�ָ��ID+4�ֽ���ʼʵ��+4�ֽ�ʧЧʵ��
			
			tab[0] = 0x55;		//�ڿ�ʼд������ͷ
			
			tab[1] = PS_Type>>8&0xFF;
			tab[2] = PS_Type&0xFF;
			
			tab[3] = PS_ID>>8&0xFF;
			tab[4] = PS_ID&0XFF;
			
			tab[5] = start_Unix>>24&0xFF;			//����ʱ��
			tab[6] = start_Unix>>16&0xFF;
			tab[7] = start_Unix>>8&0XFF;
			tab[8] = start_Unix&0xFF;
			
			tab[9]  =  stop_Unix>>24&0XFF;
			tab[10] =  stop_Unix>>16&0XFF;
			tab[11] =  stop_Unix>>8&0XFF;
			tab[12] =  stop_Unix&0xFF;
			
			#ifdef uart_start
			
			for(i =0;i<sizeof(tab);i++)
			{
				SEGGER_RTT_printf(0,"%x,",tab[i]);
			}
			
			#endif
			
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
				
			SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //��ȡ������������
			
			
			if((tab[3]==0)&&(tab[4]<=PS_Print_Num));			//�ж������ ID ���Ƿ��ڷ�Χ��
			else 
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PS_ID: %x!\r\n",PS_ID);
				SEGGER_RTT_printf(0,"PS_IDH: %x!\r\n",tab[3]);
				SEGGER_RTT_printf(0,"PS_IDL: %x!\r\n",tab[4]);
				
				#endif
				SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
				Spi_Flash_Sleep ();
				
				return 0;		//���ڷ�Χ����ֱ�ӽ���
			}
			PS_Print_Data_Num_1 = tab[4];		// ��Ҫ���浽�� ID
			
			
			if((tab[2]==0x00)&&		//������� ID ����Ϊ ���Զ�̬����ʱ
				(tab[3]==0x03))
			{
				
				for(i =0;i<PS_Print_Num;i++)		//Ѱ�ұ����Ƿ��������
				{
					if( (tab_dat[i*PS_Print_Len]==0x55)&&
							(tab_dat[i*PS_Print_Len+1]==0x00)&&
							(tab_dat[i*PS_Print_Len+2]==0x03)
						
						)
					{
						
							tab_dat[i*PS_Print_Len] =0;
							tab_dat[i*PS_Print_Len+1] =0;
							tab_dat[i*PS_Print_Len+2] =0;		//�����ǰ���ڵ�  ��̬��������ָ������
							PS_Print_Act =1;		// �������̬�������� ָ��  ��־
//							PS_Print_Act_ID = tab_dat[i*PS_Print_Len+4];			//����ɾ���� ָ��ID
						
					}
					
				}
				
			}
			
					//��������д��  tab_dat
			
			for(i =0;i<PS_Print_Len;i++)			//�� tab_dat д���µ�����
			{
				
				tab_dat[PS_Print_Data_Num_1*PS_Print_Len+i] = tab[i];
				
			}
			
			//��������д���ӦFlash����
			
			Sector_to_Block_Sector(&Block,&Sector,PS_Print_Soctor1);  //��ȡ��������������		
			SPIFlash_Erase_Sector(Block,Sector); 		//��������			
			SPI_Flash_Busy();
			
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);
			
			SPIFlash_Write_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len); //д���޸ĺ�����ݵ���̬�������� ������������һ�����ݣ����ܳ���Ӧ�� TypeA_Data_Secotr1��
			SPI_Flash_Busy();
			
			
			if(PS_Print_Act==0)		//��δɾ����̬����ָ������ʱ����̬���� + 1
				Ev_Cat_Dat(PS_Print_Num_Code,PS_Print_Data_Num+1);    //д���µ����ݵ�Ŀ¼
			
			
			if(PS_Print_Act==1)			//��ɾ������̬������Դ��ָ��ʱ ɾ��ָ��ͷ�ڱ����ָ�� ����Ҫȷ�� ���д˺���ʱָ��ͷ���ϵ�
			{
					
					
			}
		
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nAdd_Flash_PS_Print_OK!\r\n");
			
			#endif
			
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep ();
			
		return 1;		// ��ɺ� return 1;
}

/***************************************
��ȡ���õ� ָ��ͷ�洢 ID

���룺  ��

�����  ���е� ָ�� ID  û�п���ID ʱ������ 0XFFFF

�����ⲿ�ֽ��������������ɢ���ʴ˳����Դ� Flash ��ʼ�ͽ���

*****************************************/

uint16_t Need_Of_Ps_Print_Data_IDLE_ID()
{
		
		uint32_t Block,Page;  //��ȡFlash�����������
		uint32_t i; 
		uint32_t PS_Print_Data_Num;
			
	
		#ifdef uart_start
			
		SEGGER_RTT_printf(0,"\r\nNeed_Of_Ps_Print_Data_IDLE_ID!\r\n");
			
		#endif
		
		if(SPI0_Start==0)
			Spi_Flash_Init();
		
//		else
//		{
//			
//			#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"\r\nError: SPI_Busy!\r\n");
//			
//			#endif
//			
//			return 0XFFFF;
//			
//		}
	
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//��ȡ ָ�Ƶ� ��������
			
		if(PS_Print_Data_Num>=PS_Print_Num)   //���洢������ʱ����������
		{
			#ifdef uart_start
			
				SEGGER_RTT_printf(0,"\r\nPS_Print_Data_ID_Full!\r\n");
			
			#endif
			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep ();
			
			return 0XFFFF;
		}
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ		
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //��ȡ������������	
		
		
		for(i =0;i<PS_Print_Num;i++)
		{
			if(tab_dat[i*PS_Print_Len]==0x55)
			{
				;
			}
			else
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nNeed_PS_Print_Data_ID: %x!\r\n",i);			//�����ȡ����ֵ
				
				#endif
				
				SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
				Spi_Flash_Sleep ();

				
				return i;			//��ȡ�����еĴ洢�ռ䣬���ؿռ�ID			
			}
		}
		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep ();
		
		
		return 0xFFFF;
		
}


/********************************************

����ָ������

�˺���û�������������� SPI FLASH ������
����SPI FLASH ����ʱʹ��

*********************************************/

void Erase_PS_Print_Sector()
{

		uint32_t Block,Sector;  //��ȡFlash�����������
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Erase_PS_Print_Sector!\r\n");
		
		#endif
		
		
		Sector_to_Block_Sector(&Block,&Sector,PS_Print_Soctor1);  //��ȡƫ�ƺ��������������
		
		SPI_Flash_Busy();
		SPIFlash_Erase_Sector(Block,Sector); 		//��������
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erase_PS_Print_Sector_Num: %d!\r\n",PS_Print_Soctor1);
		
		#endif
		
}


/*****************************************
�ж�ָ���Ƿ���ڣ���Ч��
������ڣ��ж�ʱ���Ƿ����

���룺 ָ�� ID

����� 0�������� (����Ч)

����� 32 bit ����  ��16λΪָ������  ��16λΪָ��ID(������ֵһ��)
�˺������������� SPI FLASH ����
// ��������	0x55+2�ֽ�ָ������+2�ֽ�ָ��ID+4�ֽ���ʼʵ��+4�ֽ�ʧЧʵ��
****************************************/

uint32_t Opinion_PS_Print_Valid(uint16_t PS_Print_ID)
{
		uint32_t Block,Page;  //��ȡFlash�����������
		uint32_t i; 
		uint32_t PS_Print_Data_Num,PS_Print_Data_Num_1;
			
		uint32_t PS_Start_Unix,PS_Stop_Unix;		//��ȡ���ı���ָ�� UNIX ʱ�䣬���ڱȶ�
		uint32_t return_dat;		//Ԥ�践��ֵ
	
		return_dat =0;
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Opinion_PS_Print_Valid!\r\n");
		
		#endif
		
		
		if(SPI0_Start==0)
			Spi_Flash_Init();
		else
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nError: SPI_Busy!\r\n");
			
			#endif
			
			return 0XFFFF;
			
		}
		
		Get_Time(); //��ȡ����ʱ��
		
		if(TIME_STATE==true)
		{
			;
		}
		else
		{			
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep ();
			
			return 0XFFFF;
		}
		
		Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
						
		
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//��ȡ ָ�Ƶ� ��������
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ		
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //��ȡ������������	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_Soctor1_Buff:\r\n");
		
		for(uint32_t is =0;is<PS_Print_Num*PS_Print_Len;is++)
		{
			SEGGER_RTT_printf(0," %x,",tab_dat[is]);
			
			if(is%16==0)
			{
				SEGGER_RTT_printf(0,"\r\n");
			}
		}
				SEGGER_RTT_printf(0,"\r\n");
		
		SEGGER_RTT_printf(0,"Unix:%x.\r\n",Unix_T);
		
		#endif
		
		for(i =0;i<PS_Print_Num;i++)
		{
			if((tab_dat[i*PS_Print_Len]==0x55)&&
				 (tab_dat[i*PS_Print_Len+3] == (PS_Print_ID>>8&0xFF))&&
				 (tab_dat[i*PS_Print_Len+4] == (PS_Print_ID&0XFF))				//�ж�ID �Ƿ�һ��
				)
			{
				
				
				PS_Start_Unix = tab_dat[i*PS_Print_Len+5];
				PS_Start_Unix <<= 8;	//���ư�λ
				PS_Start_Unix |= tab_dat[i*PS_Print_Len+6];
				PS_Start_Unix <<= 8;
				PS_Start_Unix |= tab_dat[i*PS_Print_Len+7];
				PS_Start_Unix <<=8;
				PS_Start_Unix |= tab_dat[i*PS_Print_Len+8];
				
				PS_Stop_Unix = tab_dat[i*PS_Print_Len+9];
				PS_Stop_Unix <<=8;
				PS_Stop_Unix |= tab_dat[i*PS_Print_Len+10];
				PS_Stop_Unix <<=8;
				PS_Stop_Unix |= tab_dat[i*PS_Print_Len+11];
				PS_Stop_Unix<<=8;
				PS_Stop_Unix |= tab_dat[i*PS_Print_Len+12];
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"PS_Start_Unix: %x.\r\n",PS_Start_Unix);
				SEGGER_RTT_printf(0,"PS_Stop_Unix: %x.\r\n",PS_Stop_Unix);
				
				
				#endif
				
				if((Unix_T>PS_Start_Unix)&&(Unix_T<PS_Stop_Unix)) //����ǰָ��ID��ʱ����Чʱ
				{
					return_dat = tab_dat[i*PS_Print_Len+1];
					return_dat <<= 8;
					return_dat |= tab_dat[i*PS_Print_Len+2];
					return_dat <<= 8;
					return_dat |= tab_dat[i*PS_Print_Len+3];
					return_dat <<= 8;
					return_dat |= tab_dat[i*PS_Print_Len+4];
					
					//����ֵΪ 16�ֽ� ָ������+16�ֽ�ָ��ID
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Return_Dat: %x��\r\n",return_dat);		//�����ǰ�ķ���ֵ
					
					#endif
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep ();
					
					return return_dat;			//�����ʶ�Ӧָ�Ƶ����ݷ���
				}
				else				//��ʱ����Чʱ
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"PS_Print_Time_Invalid!\r\n");
					
					#endif
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep ();
					
					return 0XFFFF;
					
				}
				
			}
			else if(tab_dat[i*PS_Print_Len]==0xAA)
			{
				PS_Print_Data_Num_1++;
				
				if(PS_Print_Data_Num_1>=PS_Print_Data_Num)	//�������жϵ���Ч�������ﵽ��������ʱ������
				{
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep ();
					
					return 0XFFFF;
				}
			}
			
		}
		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep ();
		
		
		return 0XFFFF;
		
}


/**********************************************

ɾ��ĳһ���͵�ָ��
���룺PS_print_Type ָ������
����� ��
�˺���û���������� SPI FLASH ����
*********************************************/

void  Del_A_Type_PS_Print(uint16_t PS_Print_Type)
{
		uint32_t Block,Page;  //��ȡFlash�����������
		uint32_t PS_Print_Data_Num;
		uint32_t Del_PS_Print_Data_Num;		//ɾ����������
		uint32_t i;
		uint8_t PS_Type1,PS_Type2;		// ���ڶԱ�Ѱ��һ�µ���
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Del_A_Type_PS_Print!\r\n");	
		
		#endif	
	
	
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//��ȡ ָ�Ƶ� ��������
		
		if((PS_Print_Data_Num==0)||(PS_Print_Data_Num>PS_Print_Num))
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Ps_Print_Data_Num: %x .\r\n",PS_Print_Num);
			
			#endif
			
			//�����д����û������ʱ�׵��������
			
			return;
		}
		
		//����������ʼ��һ��
		
		//�����ͷ����ֽڱ��浽����
		
		PS_Type1 = PS_Print_Type>>8&0xFF;
		PS_Type2 = PS_Print_Type&0xFF;
		Del_PS_Print_Data_Num =0;
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //��ȡ������������
		
		
		for(i =0;i<PS_Print_Num;i++)			//Ѱ��һ�µ�����
		{
			if((tab_dat[i*PS_Print_Len]==0x55)&&
				 (tab_dat[i*PS_Print_Len+1]==PS_Type1)&&
				 (tab_dat[i*PS_Print_Len+2]==PS_Type2)			//�ж�����һ��
				)
				{
					for(uint8_t it;it<PS_Print_Len;it++)		//�����Ҫɾ��������
					{
						
						tab_dat[i*PS_Print_Len+it] = 0;
					
					}
					
					Del_PS_Print_Data_Num++;
					if(Del_PS_Print_Data_Num>PS_Print_Data_Num)		//��ɾ����������������������ʱ������Ϊһ���󡣣�һ�㲻�ᷢ����
					{
						
						Del_PS_Print_Data_Num = PS_Print_Data_Num;
						break;		//ͬʱ�������ѭ��
						
					}		
				}
		}
		
		if(Del_PS_Print_Data_Num==PS_Print_Data_Num)
		{
			//��ɾ���������뱾��ӵ�е�������һ��ʱ�����л���������
			
			for(i =0;i<(PS_Print_Num*PS_Print_Len);i++)
			{
				tab_dat[i] =0;		//��ջ���
			}
		}
		
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);
			
			SPIFlash_Write_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len); //��������д��
			SPI_Flash_Busy();
			
		
			Ev_Cat_Dat(PS_Print_Num_Code,(PS_Print_Data_Num-Del_PS_Print_Data_Num));    //д���µ����ݵ�Ŀ¼
			
			if((PS_Print_Data_Num-Del_PS_Print_Data_Num)==0)
			{
				//������ָ������Ϊ0ʱ�������ָ��ͷָ��
				
					CL_PS_ID	 =0;			//�������ָ����Ϣ
					CL_PS_Run = CL_PS_Run_Del_All_Mode;				//������־
				
			}
		
			#ifdef uart_start
		
			SEGGER_RTT_printf(0,"Del_PS_Print_Type:%x.\r\n",PS_Print_Type);			//���ɾ��������
			SEGGER_RTT_printf(0,"Del_PS_Num: %d.\r\n",Del_PS_Print_Data_Num);		//���ɾ��������
			
			#endif
			
}


/*******************************************

ɾ��ĳһ��ָ��
���룺 PS_Print_ID ָ��ID
����� ��

�˺���û���������� SPI FLASH ����
*********************************************/

uint8_t Del_A_ID_PS_Print(uint16_t PS_Print_ID)
{
		
		uint32_t Block,Page;  //��ȡFlash�����������
		uint32_t PS_Print_Data_Num;
		uint32_t Del_PS_Print_Data_Num;		//ɾ����������
		uint32_t i;
		uint8_t PS_ID1,PS_ID2;		// ���ڶԱ�Ѱ��һ�µ���
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Del_A_ID_PS_Print!\r\n");
	
		SEGGER_RTT_printf(0,"PS_Print_ID: %x .\r\n",PS_Print_ID);
		
		#endif	
	
	
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//��ȡ ָ�Ƶ� ��������
		
		if((PS_Print_Data_Num==0)||(PS_Print_Data_Num>PS_Print_Num))
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Ps_Print_Data_Num: %x .\r\n",PS_Print_Num);
			
			#endif
			
			//�����д����û������ʱ�׵��������
			
			return 0;
		
		}
		
		//����������ʼ��һ��
		
		//�����ͷ����ֽڱ��浽����
		
		PS_ID1 = PS_Print_ID>>8&0xFF;
		PS_ID2 = PS_Print_ID&0xFF;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_ID1: %x!\r\n",PS_ID1);
		SEGGER_RTT_printf(0,"PS_ID2: %x!\r\n",PS_ID2);
		
		#endif
		
		Del_PS_Print_Data_Num =0;
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //��ȡ������������
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_Soctor1_Buff:\r\n");
		
		for(uint32_t is =0;is<PS_Print_Num*PS_Print_Len;is++)
		{
			SEGGER_RTT_printf(0," %x,",tab_dat[is]);
			
			if(is%16==0)
			{
				SEGGER_RTT_printf(0,"\r\n");
			}
		}
		
		#endif
		
		for(i =0;i<PS_Print_Num;i++)			//Ѱ��һ�µ�����
		{
			if((tab_dat[i*PS_Print_Len]==0x55)&&
				 (tab_dat[i*PS_Print_Len+3]==PS_ID1)&&
				 (tab_dat[i*PS_Print_Len+4]==PS_ID2)			//�ж�����һ��
				)
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\nNeed_A_Ps_Print!\r\n");
					
					#endif
					
					for(uint8_t it=0;it<PS_Print_Len;it++)		//�����Ҫɾ��������
					{
						
						tab_dat[i*PS_Print_Len+it] = 0;
					
					}
					
					Del_PS_Print_Data_Num =1;
					
					if(Del_PS_Print_Data_Num>PS_Print_Data_Num)		//��ɾ����������������������ʱ������Ϊһ���󡣣�һ�㲻�ᷢ����
					{
						
						Del_PS_Print_Data_Num = PS_Print_Data_Num;
					//	break;		//ͬʱ�������ѭ��
						
					}
					break;
				}
		}
		
		if(Del_PS_Print_Data_Num==PS_Print_Data_Num)
		{
			//��ɾ���������뱾��ӵ�е�������һ��ʱ�����л���������
			
			for(i =0;i<(PS_Print_Num*PS_Print_Len);i++)
			{
				tab_dat[i] =0;		//��ջ���
			}
		}
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"New_PS_Soctor1_Buff:\r\n");
		
		for(uint32_t is =0;is<PS_Print_Num*PS_Print_Len;is++)
		{
			SEGGER_RTT_printf(0," %x,",tab_dat[is]);
			
			if(is%16==0)
			{
				SEGGER_RTT_printf(0,"\r\n");
			}
		}
		
		#endif
		
			
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);
			
			SPIFlash_Write_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len); //��������д��
			SPI_Flash_Busy();
			
			
			Ev_Cat_Dat(PS_Print_Num_Code,(PS_Print_Data_Num-Del_PS_Print_Data_Num));    //д���µ����ݵ�Ŀ¼
			
			if((PS_Print_Data_Num-Del_PS_Print_Data_Num)==0)
			{
				//������ָ������Ϊ0ʱ�������ָ��ͷָ��
				
					CL_PS_ID	 =0;			//�������ָ����Ϣ
					CL_PS_Run = CL_PS_Run_Del_All_Mode;				//������־
				
			}
			#ifdef uart_start
		
			SEGGER_RTT_printf(0,"Del_PS_Print_Type:%x.\r\n",PS_Print_ID);			//���ɾ��������
			SEGGER_RTT_printf(0,"Del_PS_Num: %d.\r\n",Del_PS_Print_Data_Num);		//���ɾ��������	
		
			#endif
		
		CL_PS_ID	 =PS_Print_ID;
		CL_PS_Run = CL_PS_Run_Del_Single_Mode;
		
		return 1;
		
}

/*

TSM12MC_ ����������ֵ����

���룺 Ҫд�������ֵ

�ɹ����� 1 ʧ�ܷ���0
*/


uint8_t TSM12MC_Sen_Code_update(uint8_t *s)
{
	
		uint32_t Block,Page;  //��ȡFlash�����������
		uint32_t Sector;  //��ȡFlash�����������
		
		uint8_t Sen_Tab[12];		//���ð���������
		uint32_t i;
		
		for(i =0;i<12;i++)			//������д�뻺��
		{
			if(*s<16) //�ж����ݺ�����
			{
				Sen_Tab[i] =*s;
				s++;				
			}
			else
			{
				return 0;			//���ݲ�����ʱ����0 
			}
		}
		
		Sector_to_Block_Sector(&Block,&Sector,TSM12MC_Sen_Sector_Num);  //��ȡƫ�ƺ��������������
		
		SPI_Flash_Busy();
		
		SPIFlash_Erase_Sector(Block,Sector); 		//��������
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erase_TSM12MC_Sen_Sector!\r\n");		//��ʾɾ���ɹ�
		
		#endif	
		
		Sector_to_Block_Page(&Block,&Page,TSM12MC_Sen_Sector_Num);
		
		SPIFlash_Write_Tab(Sen_Tab,Block,Page,12); //��������д��
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"TSM12MC_Sen_Write_OK!\r\n");
		
			read_Soctor(TSM12MC_Sen_Sector_Num);
		#endif
		
		TSM12MC_Sen_Read();
		
		if(MPR121_RUN)			//�����廽�ѵ�����£�д�봥��оƬ
		{
			TSM12_Exit_Sleep_Mode();
			TSM12_read_tast();
		}
		
		return 1;
		
}

extern uint8_t TSM12_RUN_Sen[6];

#define XD_Key			1			// �� �������� XD �͵�ʱ
//#define X_Key				1		// �� �������� X �͵�ʱ

void TSM12MC_Sen_Read()     //����������������ͬ����оƬ��
{
		uint32_t Block,Page;  //��ȡFlash�����������
		uint8_t Sen_Tab[12];		//���ð���������
		
		SPI_Flash_Busy();
		Sector_to_Block_Page(&Block,&Page,TSM12MC_Sen_Sector_Num);  //����ƫ�ƺ�������Ż�ȡ�����ʼҳ
		SPIFlash_Read_Tab(Sen_Tab,Block,Page,12);							  //��ȡ�����еĴ�������������
	
	#ifdef XD_Key					// XD �� ��ǿ 8
	
	if(Sen_Tab[7]<=0x0F)
		Sen_Tab[7] +=3;	//���� 8 ������ �� 3
	
	if(Sen_Tab[7]>=0x0F)
		Sen_Tab[7] = 0x0F;
	
	#endif

	#ifdef X_Key					// X �� ��ǿ 6
	
	if(Sen_Tab[5]<=0x0F)
		Sen_Tab[5] +=3;	//���� 6 ������ �� 3
	
	if(Sen_Tab[5]>=0x0F)
		Sen_Tab[5] = 0x0F;
	
	#endif
	

	
		for(uint8_t i=0;i<12;i++)    	//��12��������ֵת��Ϊ����оƬ�Ĵ������õ�ֵ	
		{
			
			switch(Sen_Tab[i])
			{
				case 0:   Sen_Tab[i] = 0x07;		//���������
					break;
				case 1:		Sen_Tab[i] = 0x07+0x08;		//��������ȼ�ǿ
					break;
				case 2:		Sen_Tab[i] = 0x06;
					break;
				case 3:		Sen_Tab[i] = 0x06+0x08;
					break;
				case 4:		Sen_Tab[i] = 0x05;
					break;
				case 5:		Sen_Tab[i] = 0x05+0x08;
					break;
				case 6:		Sen_Tab[i] = 0x04;
					break;
				case 7:		Sen_Tab[i] = 0x04+0x08;
					break;
				case 8:		Sen_Tab[i] = 0x03;
					break;
				case 9:		Sen_Tab[i] = 0x03+0x08;
					break;
				case 10:	Sen_Tab[i] = 0x02;
					break;
				case 11:	Sen_Tab[i] = 0x02+0x08;
					break;
				case 12:	Sen_Tab[i] = 0x01;
					break;
				case 13:	Sen_Tab[i] = 0x01+0x08;
					break;
				case 14:	Sen_Tab[i] = 0x00;
					break;
				case 15:	Sen_Tab[i] = 0x00+0x08;
					break;
				
				default:
					if(i==7)
						Sen_Tab[i] = 0x01;
					else
					Sen_Tab[i] = 0x0B;			//δ����(������Ч)�������Ĭ��������
				break;
			}
			
		}
		
	//����ͨ��2������
		TSM12_RUN_Sen[0] = Sen_Tab[1];
		TSM12_RUN_Sen[0] = TSM12_RUN_Sen[0]<<4;
		
		//����ͨ��1 ������
		TSM12_RUN_Sen[0] |= Sen_Tab[2];
		
		//����ͨ��4 ������
		TSM12_RUN_Sen[1] = Sen_Tab[7];
		TSM12_RUN_Sen[1] = TSM12_RUN_Sen[1]<<4;
		
		//����ͨ��3 ������
		TSM12_RUN_Sen[1] |= Sen_Tab[0];
		
		//����ͨ��6 ������
		TSM12_RUN_Sen[2] = Sen_Tab[9];
		TSM12_RUN_Sen[2] = TSM12_RUN_Sen[2]<<4;
		
		//����ͨ��5 ������
		TSM12_RUN_Sen[2] |= Sen_Tab[8];
		
		//����ͨ��8 ������
		TSM12_RUN_Sen[3] = Sen_Tab[11];
		TSM12_RUN_Sen[3] = TSM12_RUN_Sen[3]<<4;
		
		//����ͨ��7 ������
		TSM12_RUN_Sen[3] |= Sen_Tab[10];
		
		//����ͨ��10 ������
		TSM12_RUN_Sen[4] = Sen_Tab[11];
		TSM12_RUN_Sen[4] = TSM12_RUN_Sen[4]<<4;
		
		//����ͨ��9 ������
		TSM12_RUN_Sen[4] |= Sen_Tab[10];
				
		//����ͨ��12 ������
		TSM12_RUN_Sen[5] = Sen_Tab[5];
		TSM12_RUN_Sen[5] = TSM12_RUN_Sen[5]<<4;
		
		//����ͨ��11 ������
		TSM12_RUN_Sen[5] |= Sen_Tab[4];
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Sen_Tab:\r\n");
		
		#endif
		
		for(uint8_t i =0;i<6;i++)
		{
			
			#ifdef uart_start
			
				SEGGER_RTT_printf(0,"%x,",TSM12_RUN_Sen[i]);
			
			#endif
			
		}
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\n");
		
		#endif
				
}




