#ifndef __F_print_H_
#define __F_print_H_
#include "Devic_Init.h"  //�豸��ʼ��
// ָ��ͷ���ݽӿ�ͨ�����ݰ�

/*
���ݰ���ʽ��

���Ͱ���

���ݰ������ࣺ
����ʶ = 01: �����
����ʶ = 02: ���ݰ������к����� 
����ʶ = 08: ���һ�����ݰ�����������

���е����ݰ���Ҫ�Ӱ�ͷ�� 0xEF01.

У����ǴӰ���ʶ��У���֮�������ֽ�֮�ͣ����� 2 �ֽڵĽ�λ���ԡ� 

������ = ��������У��ͣ�ָ����������ݣ������ֽ���������У��ͣ������������� �ȱ�����ֽ����� 

01 �������ʽ�� 

0XEF01 + 0XFFFFFFFF + 1�ֽڰ���ʶ(0x01) + 2�ֽڰ����� 
+ 1�ֽ�ָ�� + N�ֽڲ��� + 2byte У���

02 ���ݰ���ʽ��
0XEF01 + 0XFFFFFFFF + 1�ֽڰ���ʶ(0X02) + 
2�ֽڰ����� + N �ֽ����� + 2�ֽ�У���

08 ���ݰ���ʽ��
0XEF01 + 0XFFFFFFFF + 1�ֽڰ���ʶ(0X08) + 
2�ֽڰ����� + N �ֽ����� + 2�ֽ�У���

���ݰ����ܵ�������ִ�����̣��������ָ�����Ӧ������档 

�´����ϴ������ݰ���ʽ��ͬ�� 

������ = ��������У��ͣ�ָ����������ݣ������ֽ���������У��ͣ������������� �ȱ�����ֽ����� 

У����ǴӰ���ʶ��У���֮�������ֽ�֮�ͣ����� 2 �ֽڵĽ�λ���ԡ� 

оƬ��ַ��û������֮ǰΪȱʡ�� 0xffffffff��һ����λ��ͨ��ָ��������оƬ��ַ�������� �����ݰ������밴�����ɵĵ�ַ�շ���оƬ���ܾ���ַ��������ݰ��� 

���ڶ��ֽڵĸ��ֽ���ǰ���ֽ��ں��� 2bytes �� 00 06 ��ʾ 0006�������� 0600���� 

ָ��Ӧ��

Ӧ���ǽ��й�����ִ����������ϱ�����λ����Ӧ������в��������ɸ��������ݰ���
��λ��ֻ �����յ� SOC ��Ӧ��������ȷ�� SOC �հ������ָ��ִ������� 

Ӧ�����ʽ��

0XEF01 + 0XFFFFFFFF + ����ʶ(0x07) + 
2 �ֽڰ����� + ȷ���� (1�ֽ�) + ���ز�����N�ֽڣ�+ 2�ֽ�У���

 ȷ���붨�壺 
00H����ʾָ��ִ����ϻ� OK�� 
01H����ʾ���ݰ����մ��� 
02H����ʾ��������û����ָ�� 
03H����ʾ¼��ָ��ͼ��ʧ�ܣ� 
04H����ʾָ��ͼ��̫�ɡ�̫���������������� 
05H����ʾָ��ͼ��̫ʪ��̫���������������� 
06H����ʾָ��ͼ��̫�Ҷ������������� 
07H����ʾָ��ͼ����������������̫�٣������̫С���������������� 
08H����ʾָ�Ʋ�ƥ�䣻 
09H����ʾû������ָ�ƣ� 
0aH����ʾ�����ϲ�ʧ�ܣ� 
0bH����ʾ����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ�� 
0cH����ʾ��ָ�ƿ��ģ��������Ч�� 
0dH����ʾ�ϴ�����ʧ�ܣ� 
0eH����ʾģ�鲻�ܽ��պ������ݰ��� 
0fH����ʾ�ϴ�ͼ��ʧ�ܣ� 
10H����ʾɾ��ģ��ʧ�ܣ� 
11H����ʾ���ָ�ƿ�ʧ�ܣ� 
12H����ʾ���ܽ���͹���״̬�� 
13H����ʾ�����ȷ�� 
14H����ʾϵͳ��λʧ�ܣ� 
15H����ʾ��������û����Чԭʼͼ��������ͼ�� 
16H����ʾ��������ʧ�ܣ� 
17H����ʾ����ָ�ƻ����βɼ�֮����ָû���ƶ����� 
18H����ʾ��д FLASH ���� 
f0H���к������ݰ���ָ���ȷ���պ��� 0xf0 Ӧ�� 
f1H���к������ݰ���ָ�������� 0xf1 Ӧ�� 
f2H����ʾ��д�ڲ� FLASH ʱ��У��ʹ���  
f3H����ʾ��д�ڲ� FLASH ʱ������ʶ����  
f4H����ʾ��д�ڲ� FLASH ʱ�������ȴ��� 
f5H����ʾ��д�ڲ� FLASH ʱ�����볤��̫���� 
f6H����ʾ��д�ڲ� FLASH ʱ����д FLASH ʧ�ܣ� 
19H�����������ʧ�ܣ� 
1aH����Ч�Ĵ����ţ� 
1bH���Ĵ����趨���ݴ���ţ� 
1cH�����±�ҳ��ָ������ 
1dH���˿ڲ���ʧ�ܣ� 
1eH���Զ�ע�ᣨenroll��ʧ�ܣ� 
1fH��ָ�ƿ����� 
20H���豸��ַ���� 
21H���������� 
22 H��ָ��ģ��ǿգ� 
23 H��ָ��ģ��Ϊ�գ� 
24 H��ָ�ƿ�Ϊ�գ� 
25 H��¼��������ô��� 
26 H����ʱ�� 
27 H��ָ���Ѵ��ڣ� 
28 H��ָ��ģ���й����� 
29 H����������ʼ��ʧ�ܣ� 
2AH��efH��Reserved�� 


*/

#define F_Print_IRQ			27

extern uint8_t Input_Ps_Lmage_Type;			//¼��ָ������(����Ա/�û�)
extern uint8_t Input_Ps_Source;					//¼��ָ����Դ������������������
extern uint8_t Test_Ps_Mark;						//���ָ��ͷ��־
extern uint32_t Start_PS_Unix;					//¼��ָ����Чʱ��
extern uint32_t Stop_PS_Unix;						//¼��ָ��ʧЧʱ��
extern uint32_t F_print_Enable;			//�ڳ�ʼ��ʱ����Ƿ����ָ��ͷ
extern uint8_t Input_Ps_Lmage_Run_Now;			//¼��ָ�����е�״̬��

#define Admin_Ps_Lmage	1
#define User_Ps_Lmage		2

extern uint8_t BLE_LINK;  //BLE����״̬ʵʱ����

extern const uint8_t PS_Getlmage[12];  //����ָ����ڶ�ȡָ��ͼ��

extern const uint8_t Null_Getlmage[12];  //Ӧ��ָ�δ��ȡ��ָ��(��ָ�ư���)

extern const uint8_t Err_Getlmage[12];  //Ӧ��ָ�������̫�٣�ʧ�ܣ��������ΰ�ѹ����

extern const uint8_t Be_Getlmage[12]; //Ӧ��ָ���ȡ��ָ�� ����ָ�ư��£�

extern const uint8_t Get_Dat_Save_1[13]; //�����������洢�ڻ����� 01

extern const uint8_t Get_Dat_Save_2[13]; //�����������洢�ڻ����� 02

extern const uint8_t Get_Dat_Save_3[13]; //�����������洢�ڻ����� 03

extern const uint8_t Get_Dat_Save_4[13]; // �����������洢�ڻ����� 04
	
extern const uint8_t Get_Dat_Save_5[13]; // �����������洢�ڻ����� 05

extern const uint8_t Get_Dat_Save_6[13]; // �����������洢�ڻ�����  06

extern const uint8_t Merge_Lmage_Dat[12]; //�ϲ� ��ȡ����ָ����������

extern const uint8_t Ps_Save_ID[15];	//����ָ�Ƶ�ָ��ID��

extern const uint8_t Se_Lmage_Dat[17];					   //���������Ƿ��������ָ��

extern const uint8_t Not_Se_Lmage[16]; 	//����δ������ƥ��ָ��

extern const uint8_t Se_Lmarge[16];			//������ָ��

extern const uint8_t PS_Del_A_Lmage[16];  //ɾ��һ��ָ��ָ��

extern const uint8_t PS_Del_A_Lmage_ACK[12];  //����ָ��ɾ���ɹ�Ӧ��

extern const uint8_t PS_Del_All_Lmage[12];	 // ɾ������ָ��ָ��

extern const uint8_t PS_Del_Lmage_ACK[12];		 //ɾ������ָ����� Ӧ��

extern const uint8_t PS_Read_Lmage_Num[12];		 //��ȡ�����ָ������

extern const uint8_t PS_Lmage_Num_ACK[14];		 	//Ӧ���ָ����

extern const uint8_t PS_HandShake[12];						//��ָ��ģ������

extern const uint8_t PS_HandShake_ACK[12]; 			//ָ��ģ������Ӧ��

extern const uint8_t PS_PowDlow[12]; 					//ָ������ָ��

extern const uint8_t PS_PowDlow_ACK[12]; 			//ָ������ָ��Ӧ��

extern uint8_t Ps_Data_Tab[50];								//ָ��ͷ�������ݻ���

extern const uint8_t PS_LED_CFG[16];					// ָ��ͷ LED ����ָ��

extern const uint8_t PS_LED_POWER[16]; 		//���� ָ��ͷ �� ��Դ

extern const uint8_t  PS_LED_R1[16];								//ָ��ͷ LED ��ɫ ��һ��ָ��

extern const uint8_t  PS_LED_R2[16];								//ָ��ͷ LED ��ɫ �ڶ���ָ��

extern const uint8_t PS_LED_G1[16];		//ָ��ͷ LED ��ɫ ��һ��ָ��

extern const uint8_t PS_LED_G2[16];		//ָ��ͷ LED ��ɫ �ڶ���ָ��

extern const uint8_t PS_LED_B1[16];			//ָ��ͷ LED ��ɫ ��һ��ָ��

extern const uint8_t PS_LED_B2[16];			//ָ��ͷ LED ��ɫ �ڶ���ָ��

extern const uint8_t PS_LED_W1[16];     //ָ��ͷ LED ��ɫ  ��һ��ָ��

extern const uint8_t PS_LED_W2[16];     //ָ��ͷ LED ��ɫ  �ڶ���ָ��

extern const uint8_t PS_LED_Sleep[16];		//ʹLED ����оƬ����  �͹���ģʽ

extern uint8_t Controllable_Power_State;		//ָ��ͷ��Դ״̬

extern uint8_t UART_RUN;   //���ڱ�ʾ���ڿ���״̬

extern uint8_t F_Print_Run_Flag;				//ָ��ͷ���б�־λ

extern uint16_t CL_PS_ID;			//�趨 Ҫɾ���� ָ�� ID  
extern uint16_t CL_PS_Run;				//�趨 ɾ��ָ��ͷ����

extern uint8_t CL_PS_Print_Now;


extern const uint8_t CL_PS_Run_Del_Nor_Mode;					//ɾ�� ָ��ID ���ܹ���
extern const uint8_t CL_PS_Run_Del_Single_Mode;				//ɾ������ ָ��ID ��ģʽ
extern const uint8_t CL_PS_Run_Del_All_Mode;					//ɾ������ ָ��ID ��ģʽ
extern const uint8_t CL_PS_Run_Del_Batch_Mode;				//����ɾ�� ָ��ID ��ģʽ




void Controllable_Power_No(void); //����ָ��MCU��Դ
void Controllable_Power_Off(void);		//�ر�ָ��MCU��Դ



void Input_PS_Lmage(void);					//¼��ָ��

void PS_Data_Cheak(uint8_t *s,uint8_t len);			//�ڴ��ڰ��л�ȡָ�����ݰ�
void PS_Input_Order_Run(uint8_t *s,uint8_t len);			//ָ��ͷ��������ָ���ж�
uint8_t Input_PS_Lmage_Run(uint32_t Lmage_Start_Unix,uint32_t Lmage_Stop_Unix,uint32_t Ps_ID);			//����ָ��¼�빦��

uint8_t F_print_Goto_Sleep(void);			//�ر�ָ��ģ���Դ

uint8_t CL_Ps_Print(void);		//���ָ��ͷ����


void PS_LED_Run(uint8_t LED_CR);					// ָ��ͷ LED ����

void PS_LED_Shutdown(void);								 // 

#endif
