#ifndef __FLASH_RW_H_
#define __FLASH_RW_H_

 #include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "nrf_delay.h"
#include "mpr121.h"
#include "w25q32.h"
#include "BL5372.h"
#include "Devic_Init.h"  //�豸��ʼ��


#define Devic_Num_BLOCK  0  //�����豸�Ŵ洢����
#define Devic_Num_PAGE   0  //�豸�󶨺�����ҳ
#define Devic_Num_Sector 0  //�豸�󶨺����������š�

#define Devic_PAGE_Len  40  //�豸������ʹ�ÿռ��С
#define Devic_Num_Len   40  // �豸����(�����㲥��������)
#define PAGE_Heart_H   0x55  //����ҳ��ʼ�����ֽ�
#define PAGE_Heart_L   0xAA  //����ҳ��ʼ�����ֽ�




extern const uint8_t Cat_Dat_Sector1;						//�趨Ŀ¼��������ʼ��ַ

extern const uint8_t Cat_Dat_Sector_Num;				// �趨 Ŀ¼ ���ĵ� ��������

extern const uint32_t Cat_Dat_len;				// �趨 Ŀ¼ ���ݵ��ܳ���



extern const uint8_t PassWord_Len;  //�����������ݳ���

extern const uint8_t Open_Note_Num_Len; //�������ż�¼����

extern const uint8_t PassWord_Secotr1;  //����������������

extern const uint8_t PassWord_Secotr2;  //����������������

extern const uint8_t PassWord_SN_Secotr;  //�����Ŵ洢���� (ÿ���Զ�������ı��)

extern const uint8_t Act_PassWord_Secotr;  //�洢��̬����

extern const uint8_t History_PassWord_Secotr;          //��ʷ��̬����

extern const uint8_t Per_Act_PassWord_Secotr;				//�������ö�̬����


extern const uint16_t AD_TypeA_Data_Secotr1;  //����A�����ݵ�����1 (Ŀǰ����1������)

extern const uint16_t AD_TypeB_Data_Secotr1;  //����B�����ݵ�����1 (Ŀǰ����1������)

extern const uint16_t US_TypeA_Data_Secotr1;  //����A�����ݵ�����1 (Ŀǰ����1������)

extern const uint16_t US_TypeB_Data_Secotr1;  //����B�����ݵ�����1 (Ŀǰ����1������)

// �Ϸ�ÿ������ͷ ��� 105 ������Ϊ�˱�֤���ݰ�ȫ����Ҫ�����������������100������

extern const uint8_t TypeA_Data_Len;  	//һ��A�����ݵĳ���

extern const uint8_t TypeB_Data_Len;		//һ��B�����ݵĳ���

extern const uint8_t A_Sector_TypeA_Data_Num;			//һ����������� A �� ��������

extern const uint8_t A_Sector_TypeB_Data_Num;			// һ����������� B �� ��������

extern const uint8_t AD_TypeA_Data_Sector_Num;		// AD_TypeA_Data  ��������ʹ�õ���������

extern const uint8_t US_TypeA_Data_Sector_Num;		// US_TypeA_Data  ��������ʹ�õ���������

extern const uint8_t AD_TypeB_Data_Sector_Num;		// AD_TypeB_Data  ��������ʹ�õ���������

extern const uint8_t US_TypeB_Data_Sector_Num;		// US_TypeB_Data  ��������ʹ�õ���������







//#define PassWord_Len 18  //����һ���������ݵĳ���
//#define PassWord_Secotr 2 //��������洢����λ��
//#define Max_PassWord_Num  100  //���壬����֧�ֵ���������

extern const uint8_t PassWord_Len;  //�������볤��
extern const uint8_t Open_Note_Num_Len; //�������ż�¼����

extern const uint8_t PassWord_Secotr1;
extern const uint8_t PassWord_Secotr2;  //����������������
extern const uint8_t Max_PassWord_Num;
extern const uint8_t PassWord_SN_Secotr;  //�����Ŵ洢����

extern uint32_t Unix_T;  //��ǰоƬ��ʱ���


extern const uint8_t A_Sector_TypeA_Data_Num;

extern const uint8_t A_Sector_TypeB_Data_Num;


extern const uint8_t OpenDoorNote_Num_Code; 
extern const uint16_t Open_Note_Secotr1;
extern const uint16_t Open_Note_Secotr2;
extern const uint16_t Open_Note_Secotr3;
extern const uint16_t Open_Note_Secotr4;  //�ĸ����ż�¼ר������


extern const uint16_t AD_TypeA_Data_Secotr1;  //����A�����ݵ�����1 (Ŀǰ����1������)

extern const uint16_t AD_TypeB_Data_Secotr1;  //����B�����ݵ�����1 (Ŀǰ����1������)

extern const uint16_t US_TypeA_Data_Secotr1;  //����A�����ݵ�����1 (Ŀǰ����1������)

extern const uint16_t US_TypeB_Data_Secotr1;  //����B�����ݵ�����1 (Ŀǰ����1������)


//const uint8_t Flash_Word_Num =8;  //���� Flash Ŀ¼��������

//const uint8_t OpenDoorNote_Num_Code =0x11; 

//const uint8_t Act_PassWord_Num_Code =0x02;// ��̬����洢Ŀ¼���

extern  const uint8_t More_Act_PassWord_Num_Code;			//����ʱЧ��̬����洢Ŀ¼

extern const uint8_t AD_TypeA_Data_Num_Code; // ����ԱA����Ϣ�洢Ŀ¼���

extern const uint8_t AD_TypeB_Data_Num_Code; // ����ԱB����Ϣ�洢Ŀ¼���

extern const uint8_t US_TypeA_Data_Num_Code; // �û�A����Ϣ�洢Ŀ¼���

extern const uint8_t US_TypeB_Data_Num_Code; // �û�B����Ϣ�洢Ŀ¼���

extern const uint8_t CK_Auth_Index_Code;  //�����豸�������ݿ����к� @ck_auth_index

extern const uint8_t Nmg_Auth_Index_Code;  //�����豸�������ݿ����к� @ nmg_auth_index


extern const uint8_t PS_Print_Len;		//0x55+2�ֽ�ָ������+2�ֽ�ָ��ID+4�ֽ���ʼʵ��+4�ֽ�ʧЧʵ��

extern const uint8_t PS_Print_Num;			//���浽���ָ������

extern const uint16_t PS_Print_Soctor1;  //����ָ����������1

extern uint8_t SPI0_Start; //ȷ��SPI0 �Ƿ�����

//Ŀ¼�� ��������

/*��ע��  
Ŀ¼������������ͬʱʹ�ã����ǰ�����˳������д�뱣��ͬһ������ 
��ƽ��Flash��дĥ���Դﵽ��������۲�д����
д����һ������ʱ����Ҫʹ����������Ч��  ���������㣩
*/
extern const uint8_t Cat_Dat_Sector1;						//�趨Ŀ¼��������ʼ��ַ

extern const uint8_t Cat_Dat_Sector_Num;				// �趨 Ŀ¼ ���ĵ� ��������

extern const uint32_t Cat_Dat_len;				// �趨 Ŀ¼ ���ݵ��ܳ���




extern const uint16_t Open_Note_Size;  //���忪�ż�¼����
extern const uint16_t A_Sector_Open_Note_Size;


extern const uint8_t Max_PassWord_Num;  //�����������
extern const uint8_t A_Sector_PassWord_Num;  //���嵥�����������������ɵ���������


//#define PassWord_Num_BLOCK 0 //��������������������
//#define PassWord_Num_PAGE 0 // ����������������ҳ
//#define PassWord_Num_Sector 1   //��������������������
 
//����Ŀ¼ͷ��ʶ
#define PassWord_Num_code 0x01  //�������������������� 

extern const uint8_t Act_PassWord_Num_code;
extern const uint8_t OpenDoorNote_Num_Code;  //���ż�¼������������

#define Admin_PassWord  0x12		//�������Ա����ǰ׺
#define User_PassWord  0x22		//�����û�����ǰ׺

void SPI_Flash_Busy(void);
void Read_Devic_Num(void);
void Write_Devic_Num(void);
void CL_Devic_Num(void);
void CL_FlashWord_Num(void);

void PAGE_to_Block_Page (uint32_t *BLOCK,uint32_t *PAGEO,uint32_t PAGE);
void PAGE_to_BLOCK_Sector (uint32_t *BLOCK,uint32_t *SECTOR,uint32_t PAGE);
void Sector_to_Block_Page (uint32_t *Block,uint32_t *PAGE,uint32_t SECTOR);
void Sector_to_Block_Sector (uint32_t *Block,uint32_t *Sector,uint32_t SECTOR);

void CL_Flash_PassWord(void);

uint8_t Add_Flash_PassWord(uint8_t *PassWord); //д��һ������
void Del_Flash_PassWord(uint8_t *PassWord); //ɾ��һ������
void read_Soctor(uint32_t sector); //��ȡ�������� (2k)���ݣ�����ӡ���

void Ev_Cat_Dat(uint8_t Cat_Type,uint32_t Cat_Dat);//д��Ŀ¼��Ϣ

uint32_t Get_Cat_Dat(uint8_t Cat_Type); //��ȡĿ¼��Ϣ

void SPIFlash_Read_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len);

void SPIFlash_Write_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len);

void Del_Admin_PassWord(void);  // ɾ������Ա����
void Del_User_PassWord(void);   //ɾ���û�����

uint8_t Del_A_PassWord(uint8_t PassWord_SN);  //�����������ɾ��һ������
uint8_t Del_SN_PassWord(uint8_t PW_SN);  //�����������ɾ������(2������)

void ADD_Op_Door_Note(uint16_t SN_Num,uint16_t Key_type,uint32_t Unix_t);  //���뿪�ż�¼ (����ʱ����)


uint8_t Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T);//�ж�����������Ƿ�ͱ��ر������Ч��̬����һ��

uint8_t  History_Time_Act_PassWord(void);


uint16_t Add_Flash_AD_TypeA_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //�������ԱA������
uint16_t Add_Flash_US_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //�����û�A������
uint16_t Add_Flash_AD_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //�������Ա B������
uint16_t Add_Flash_US_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //�����û�B������


uint8_t Del_AD_Type_A(uint8_t *AD_Type_A_SN);									//ɾ�� ָ�� SN ����Ա A ������

uint8_t Del_US_Type_A(uint8_t *US_Type_A_SN);								//ɾ�� ָ�� SN �û� A ������

uint8_t Del_AD_Type_B(uint8_t *TypeB_ID);								//ɾ�� ָ�� SN ����Ա A ������

uint8_t Del_US_Type_B(uint8_t *TypeB_ID);						//ɾ�� ָ�� SN �û� B ������

uint32_t Need_TypeA_Data_SN(uint8_t *Need_Uid); 		  //�ж� ���� �Ƿ��� ��ͬ�� A ������

uint32_t Need_TypeB_Data_SN(uint8_t *Need_Uid);			

void CL_Flash_OPEN_DOOR_NOTE(void);  //��տ��ż�¼����

uint8_t Once_Act_PassWord_Verify(void);    //����ȷ��һ�������Ƿ��Ѿ���ʹ�ù�
 
void Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t);    //���涯̬����

void More_Act_PassWord_Save(uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t);     //�������Ķ�̬����(ͬʱ������鶯̬������Ч)

uint8_t More_Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T);				//���鶯̬���뿪��ָ��

uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord);				//�Ӷ��鶯̬������ɾ��һ��


void Erase_AD_TypeA_Sector(void);			// ���� AD_TypeA_Sector ����

void Erase_US_TypeA_Sector(void);			// ���� US_TypeA_Sector ����

void Erase_AD_TypeB_Sector(void);			// ���� AD_TypeB_Sector ����

void Erase_US_TypeB_Sector(void);			// ���� US_TypeB_Sector ����

uint16_t Add_Flash_PS_Print (uint16_t PS_Type,uint16_t PS_ID,uint32_t start_Unix,uint32_t stop_Unix);			//�� FLASH д��ָ��ͷ����

uint16_t Need_Of_Ps_Print_Data_IDLE_ID(void);    // ��ȡ ָ��ͷ���е�ID

void Erase_PS_Print_Sector(void);				//����ָ������

uint8_t Del_A_ID_PS_Print(uint16_t PS_Print_ID);		//ͨ�� ID ɾ��ָ��

uint32_t Opinion_PS_Print_Valid(uint16_t PS_Print_ID);			//��֤ ID �Ƿ���Ч

void  Del_A_Type_PS_Print(uint16_t PS_Print_Type);

void Per_Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t);				//������������

uint8_t Per_Act_PassWord_Verify(uint8_t PassWord_Len);		//�жϱ�����������

uint8_t TSM12MC_Sen_Code_update(uint8_t *s);				//TSM12MC_ ����������ֵ����

void TSM12MC_Sen_Read(void);     //����������������ͬ����оƬ��

uint8_t Read_Op_Note(uint16_t Read_Num);			//��ȡ���ż�¼ֵ���ڷ���

#endif
