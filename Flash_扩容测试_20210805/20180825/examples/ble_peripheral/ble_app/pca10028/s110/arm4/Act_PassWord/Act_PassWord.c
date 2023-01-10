#include <Act_PassWord.h>

/**
	 *
	 *  ʱ����̬�����㷨 (�����㷨)
	 *
	 * @dvc_id 		�豸ID����������
	 * @ck_code   	�豸У���룬�豸��ʱ�ɳ���д�뵽�豸���豸����
	 * @random      ����룬0-9 �� ��������ɣ�����
	 * @intime		��Чʱ�䣬��ʽΪ��ȷ��Сʱ��ʱ��� �� ����427280 = 1538208000 / 60 (�÷�)/ 60 (��ʱ)
	 * @outtime		ʧЧʱ�䣬��ʽΪ��ȷ��Сʱ��ʱ��� �� ����427280 = 1538208000 / 60 (�÷�)/ 60 (��ʱ)
	 *
	 *
	 *  ��סʱ����24Сʱ�ڵģ�ȡ6λ���룬����ʱ���鳤��Ϊ2
	 *  ��סʱ����72Сʱ�ڵģ�ȡ7λ���룬����ʱ���鳤��Ϊ3
	 *  ��סʱ����8760Сʱ�ڵģ�ȡ8λ���룬����ʱ���鳤��Ϊ4
	 *  ��סʱ����8761 - 87600Сʱ�ڵģ�ȡ9λ����
	 *
	 *  ������[���ܿ�(3λ)]+[�����(1λ)]+[ʱ����]��ɣ���5865 8 56
	 *
	 *  ���ܿ��㷨��(dvc_id *  intime + ck_node * outtime)%1000
	 *  ������㷨�� ���ֱ��ָ�����豸ֱ�Ӱ���ָ��λ�ö�ȡ(��4λ)
	 *  ʱ�����㷨��hour_size(��intime��outtime�������) + (((dvc_id + ck_node) * random) %75)
	 * */

/****************
ģ�⶯̬���������㷨  
****************/
extern uint32_t id1,id2;
uint8_t etp[10]; //��������������ַ��� 
uint8_t enc_time_psw(uint32_t intime,uint32_t outtime,uint32_t random)
{
	uint64_t hour_size = outtime-intime;
	uint64_t dvc_id;
	//uint32_t id11,id12,id13,id14;
	long long int enc_val;
	uint64_t ck_node;  //�豸У����
	uint64_t last;
	
	dvc_id = id1&0xffff;
	if((dvc_id&0xff00)>0x7F00) //���߰��ֽڸ���0x7F ��ո߰��ֽ�
		dvc_id&=0x00ff; //��ո߰��ֽ�
	dvc_id<<=16;
	dvc_id|=id2&0xffff; //ƴ�ӳ�������ID��
	
//	dvc_id = 128981;
//	id11 = dvc_id;
//	id11 = id11&0xff000000;
//	id11>>=24;
//	
//	id12 = dvc_id;
//	id12 = id12&0xff0000;
//	id12>>=16;
//	
//	id13 = dvc_id;
//	id13 = id13&0xff00;
//	id13>>=8;
//	
//	id14 = dvc_id;
//	id14 = id14&0xff;
//	
//	dvc_id =0;
//	
//	dvc_id |=id14;
//	dvc_id<<=8;
//	
//	dvc_id|=id13;
//	dvc_id<<=8;
//	
//	dvc_id|=id12;
//	dvc_id<<=8;
//	
//	dvc_id|=id11;
//	//dvc_id<<=8;
	
	
	ck_node =0;
	
	ck_node |= Devic_Num[28];
	ck_node<<=8;
	ck_node |= Devic_Num[29];
	ck_node<<=8;
	ck_node |= Devic_Num[30];
	ck_node<<=8;
	ck_node |= Devic_Num[31];
	

	enc_val = ((dvc_id%1000)*(intime%1000) + (ck_node%1000) * (outtime%1000))%1000;

	
	etp[0] = enc_val/100%10;
	etp[1] = enc_val/10%10;
	etp[2] = enc_val%10; //��ֵ��������
	etp[3] = random;
	
	if(hour_size <= 24) //һ�����ڵĿ�������Ϊ��λ��
	{ //������������λ
		last =hour_size + (((dvc_id + ck_node) * random)%75);
		etp[4] =  last/10%10;
		etp[5] =  last%10;
		return 6; //�������ɵ�����λ��
	}
	else if(hour_size <= 72) //һ�����⣬�������ڵĿ�������Ϊ��λ��
	{
		last = hour_size+ ((dvc_id + ck_node)*random%927);
		etp[4] = last/100%10;
		etp[5] = last/10%10;
		etp[6] = last/1%10;
		return 7;
	}
	else if(hour_size <= 24*365) //�����⣬365���ڵ�����
	{
		last = hour_size + ((dvc_id + ck_node) * random*1239);
		etp[4] = last/1000%10;
		etp[5] = last/100%10;
		etp[6] = last/10%10;
		etp[7] = last/1%10;
		return 8;
	}
	else if(hour_size <= 24*365*10)
	{
		last = hour_size + ((dvc_id + ck_node)*1239);
		etp[4] = last/10000%10;
		etp[5] = last/1000%10;
		etp[6] = last/100%10;
		etp[7] = last/10%10;
		etp[8] = last/1%10;
		return 9;
	}
	return 0;
}

 /**
     *
     *  ���ö�̬�����㷨
     *
     * @dvc_id 		�豸ID����������
     * @ck_code   	�豸У���룬�豸��ʱ�ɳ���д�뵽�豸���豸����
     * @random      ����룬0-9 �� ��������ɣ�����
     * @intime      ��������ʱ��
     *
     *  ȡʱ��Ϊ8Сʱ��8λ���룬���ñܿ�8λʱЧ����Ĺ��������������סʱ�������Ϸ�У���ã����������Ƿ���ָ��ʱ����Ч��
     *
     *  ������[���ܿ�(3λ)]+[�����(1λ)]+[ʱ����]��ɣ���5865 8 568
     *
     *  ���ܿ��㷨��(dvc_id *  intime + ck_node * outtime)%1000
     *  ������㷨�� ���ֱ��ָ�����豸ֱ�Ӱ���ָ��λ�ö�ȡ(��4λ)
     *  ʱ�����㷨��hour_size(��intime��outtime�������) + (((dvc_id + ck_node) * random) %1239)
     * */

    /**
     *
     *  һ���Զ�̬�����㷨
     *
     * @dvc_id 		�豸ID����������
     * @ck_code   	�豸У���룬�豸��ʱ�ɳ���д�뵽�豸���豸����
     * @random      ����룬0-9 �� ��������ɣ�����
     * @intime      ��������ʱ��
     *
     *  ȡʱ��Ϊ6Сʱ��8λ���룬���ñܿ�8λʱЧ��������ö�̬����Ĺ���һ���Զ�̬������֤ͨ��������ָ�����򣬺�������ʧЧ
     *
     *  ������[���ܿ�(3λ)]+[�����(1λ)]+[ʱ����]��ɣ���5865 8 568
     *
     *
     *  ���ܿ��㷨��(dvc_id *  intime + ck_node * outtime)%1000
     *  ������㷨�� ���ֱ��ָ�����豸ֱ�Ӱ���ָ��λ�ö�ȡ(��4λ)
     *  ʱ�����㷨��hour_size(��intime��outtime�������) + (((dvc_id + ck_node) * random) %1239)
     * */

/*******************************
��̬�������
*@dvc_id �豸ID ����ʱ����
*@ck_code �豸У���룬�豸��ʱ�ɳ���д�뵽�豸���豸����
*@random ����룬0-9 ��������ɣ�����
*@intime ��������ʱ��
*@psw ��������ֵ
*@psl �������볤��
*@unix_t ��ǰ����unixʱ��
*  ������[���ܿ�(3λ)]+[�����(1λ)]+[ʱ����]��ɣ���5865 8 568
*
*  ���ܿ��㷨��(dvc_id *  intime + ck_node * outtime)%1000
*  ������㷨�� ���ֱ��ָ�����豸ֱ�Ӱ���ָ��λ�ö�ȡ(��4λ)
*  ʱ�����㷨��hour_size(��intime��outtime�������) + (((dvc_id + ck_node) * random) %1239)
* */

uint32_t  des_psw(uint8_t *psw,uint8_t psl,uint32_t unix_t)
{
	
//	uint32_t nowHour;
	uint64_t enc_val;
	uint64_t random_val;
	uint64_t last_val =0;
	uint32_t size =0;
	uint64_t dvc_id;
	uint64_t ck_node;  //�豸У����
	uint8_t i;
	uint8_t tab[10]; //�������뻺��
	uint32_t thour;
	uint32_t outtime;
	
//	nowHour = unix_t;


	
	for(i =0;i<sizeof(tab);i++) //���������뻺��
	{
		
		if(i<psl)
			{ 
				tab[i] =*psw;
				psw++;
			}
			else tab[i] =0;
	}
	
	random_val = tab[3];//�����
	
	dvc_id = id1&0xffff;
	if((dvc_id&0xff00)>0x7F00) //���߰��ֽڸ���0x7F ��ո߰��ֽ�
		dvc_id&=0x00ff; //��ո߰��ֽ�
	
	dvc_id<<=16;
	dvc_id|=id2&0xffff; //ƴ�ӳ�������ID��
	ck_node =0;
	
	ck_node |= Devic_Num[28];
	ck_node<<=8;
	ck_node |= Devic_Num[29];
	ck_node<<=8;
	ck_node |= Devic_Num[30];
	ck_node<<=8;
	ck_node |= Devic_Num[31];
	
	if(psl==6) //������Ϊ6ʱ
	{
		last_val = tab[4]*10;
		last_val+=tab[5];
		size = last_val- (((dvc_id + ck_node)*random_val)%75);  //�����볤��Ϊ6ʱ����ȡ��Чʱ��
		if(size>24 || size<=00)
			return 0; //���Ϸ�����
	}
	else if (psl==7)
	{
		last_val = tab[4]*100;
		last_val +=tab[5]*10;
		last_val +=tab[6];
		size = last_val - (((dvc_id + ck_node)*random_val)%927);  //�����볤��Ϊ7ʱ����ȡ��Чʱ��
		if(size <= 24 || size>72)
			return 0;  //���Ϸ�����
	}
	else if (psl==8)
	{
		last_val = tab[4]*1000;
		last_val += tab[5]*100;
		last_val += tab[6]*10;
		last_val += tab[7];
		size = last_val - (((dvc_id + ck_node)*random_val)%1239);
		if((size <= 72 || size > 8760) && size != 6 && size !=8)
		{
			//8λ�������ʱ�����Ϊ6 ��һ�������룬8λ�����������Ϊ8������������
			return 0; //������Ϊ���Ϸ�������
		}
	}
	else if (psl==9)
	{
		last_val = tab[4]*10000;
		last_val += tab[5]*1000;
		last_val += tab[6]*100;
		last_val += tab[7]*10;
		last_val += tab[8];
		size = (last_val - ((dvc_id + ck_node)*random_val)%12399);
		if(size<=8760 || size>87600)
			return 0; //��Ϊ���Ϸ�����
	}
	
	
	enc_val =0;
	enc_val = tab[0]*100;  //��ǰ��λУ�����������
	enc_val += tab[1]*10;
	enc_val += tab[2];

	for (int i = 0 ; i < 25 ; i ++)
	{
			thour = unix_t - i ;
			outtime = thour + size;
		
			if(((dvc_id%1000) *  (thour%1000) + (ck_node%1000) * (outtime%1000))%1000 == enc_val)
			{
					size|=i<<24;  //����ʼʱ���뱾��ʱ��Ĳ���뷵��ֵ�ĸ�8�ֽ�(��Сʱǰ��Ч 0-24 ������Ч)
					return size;
			}
	}
	return 0;
}


///***************************************
// 
//��̬�����㷨 ���ж�������ȷ
//��̬���뱣�����
//55+AA+��������+����+����ɾ��ʱ�� һ��
//*****************************************/

//void Act_PassWord_Run() //��̬����������
//{
//	uint8_t i;
//	uint32_t dvc_id,vrf_id,hour_size,enc_date;
//	uint8_t AP_Num;  //�ж��Ƿ��ж�̬�������
//	AP_Num = Get_Cat_Dat(0x02);  //��ȡ��̬�������� (0��1)
//	#ifdef uart_start
//	SEGGER_RTT_printf(0,"Act_PassWord_Run!\r\n"); //��ʾ���붯̬�������
//	#endif
//	if(AP_Num==0) //��̬���뿪��״̬ (����Ч��̬��������)
//	{
//		
//	}
//}

/************************************************************

���ݿ� �㷨

���룺 Data_Card_Type   
���ݿ����� 
01�� �Ϳ�
02������

���أ� ���ݿ��Ƿ��ܿ���
1 ����
0 ���ܿ���

�Ϳ�����

1�ֽڿ����ԣ��Ϳ�Ϊ1������Ϊ2��+3λ���ܿ�+4�ֽڿ�ʼʱ��+4�ֽڽ���ʱ��+4λ�Ϳ����к�

��������

1�ֽڿ����ԣ��Ϳ�Ϊ1������Ϊ2��+3λ���ܿ� + 2�ֽ�¥��� + 4�ֽڿ�ʼʱ��+4�ֽڽ���ʱ��+4λ�������к�

����У������Դ�� ���ڹ���
		Devic_Num[32] 
		Devic_Num[33] 
		Devic_Num[34] 
		Devic_Num[35] ����У����
		
�豸У������Դ��  ���ڿͿ�
		Devic_Num[28] 
		Devic_Num[29] 
		Devic_Num[30] 
		Devic_Num[31]  �滻�豸У����

¥����Դ��
		Devic_Num[36]
		Devic_Num[37]
		 
		���룺 Card_AU ���ؿ�����

*************************************************************/

extern uint32_t Unix_T;  //��ǰоƬ��ʱ���

extern uint8_t DatCrd_AU;					//�������ݿ�����
 
extern uint8_t read_TypeA_Data[128];  //A���������򻺴�
uint8_t Data_Card_Run()
{
	
	uint64_t dvc_id;			//�豸ID ��������
	uint64_t ck_code;		//�豸У���룬�豸��ʱ�г���д�뵽�豸���豸����
	uint64_t mng_code;  //����У���룬�豸��ʱ�ɳ���д�뵽�豸���豸����
	uint64_t intime; 		//��Чʱ��
	uint64_t outtime;		//ʧЧʱ��
	uint64_t ck_auth_index; //��Ȩ���кţ���Ȩ���Ժ��豸����У��ɹ������кţ����ڵ����豸����������кŷ���Ч�����ڿͿ�
	uint64_t nmg_auth_index;  //��Ȩ���кţ����ڹ���
	uint64_t Floor_Num;  //�豸¥���
	
	uint64_t Ck_Data;  //���ܼ�����
	uint64_t Card_Ck_Data;  //���ڼ��ܼ�����
	uint64_t anth_index;  //�������к�
	uint8_t i;
	
	uint8_t Sen_Tab[12];
	
	
	DatCrd_AU =0;
	
	//���� �޸������ȵĹ��� �������ݸ�ʽ��������
	
	if((read_TypeA_Data[17]==0x55)&&(read_TypeA_Data[18]==0xAA))
	{
		if(read_TypeA_Data[19]==0xE7)   //��⵽ ���������ȵ�ָ��
		{
			for(i =0;i<12;i++)
			{
				
				Sen_Tab[i] = read_TypeA_Data[20+i];
				
			}
		}
		
		if(TSM12MC_Sen_Code_update(Sen_Tab)==1)   //���´�����������
		{	
			
			input_speak(9);				//��ʾ�����ɹ�
			
			nrf_delay_ms(1000);		//�ӳ�1��			
		}
		
		return 0;   //��Ȼ���������ݿ���ʽ����
	}
	
	
	
	
	dvc_id =0;
	ck_code =0;
	mng_code =0;
	outtime =0;
	ck_auth_index =0;
	nmg_auth_index =0;
	Floor_Num =0;
	
	Get_Time();
	Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���	
	
	  //��ȡ�Ϳ�����������Ȩ���к�
	ck_auth_index = Get_Cat_Dat(CK_Auth_Index_Code);  //�Ϳ����к�
	nmg_auth_index = Get_Cat_Dat(Nmg_Auth_Index_Code);  //�������к�
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"ck_auth_index:%d\r\n",ck_auth_index);
	SEGGER_RTT_printf(0,"nmg_auth_index:%d\r\n",nmg_auth_index);
	
	#endif
	
	Floor_Num = Devic_Num[36];
	Floor_Num<<=8;
	Floor_Num|=Devic_Num[37];	
	
	uint32_t Card_Floor_Num;  //���� ¥���
	
	
	dvc_id = id1&0xffff;
	if((dvc_id&0xff00)>0x7F00) //���߰��ֽڸ���0x7F ��ո߰��ֽ�
		dvc_id&=0x00ff; //��ո߰��ֽ�
	
	dvc_id<<=16;
	dvc_id|=id2&0xffff; //ƴ�ӳ�������ID��
	
	
	//��ȡ�豸У����
	
	ck_code = Devic_Num[28];
	ck_code<<=8;
	
	ck_code |= Devic_Num[29];
	ck_code<<=8;
	
	ck_code |= Devic_Num[30];
	ck_code<<=8;
	
	ck_code |= Devic_Num[31];
	
	
	
	//��ȡ�豸����У����
	
	mng_code = Devic_Num[32];
	mng_code<<=8;
	
	mng_code |=Devic_Num[33];
	mng_code<<=8;
	
	mng_code |=Devic_Num[34];
	mng_code<<=8;
	
	mng_code |=Devic_Num[35];
	
	Floor_Num = Devic_Num[36];
	Floor_Num<<=8;
	
	Floor_Num|=Devic_Num[37];
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Devic_Num:\r\n");

	for(uint8_t i=0;i<sizeof(Devic_Num);i++)
	
	SEGGER_RTT_printf(0," %x",Devic_Num[i]);
	
	SEGGER_RTT_printf(0," \r\n ");
	
	SEGGER_RTT_printf(0,"ck_code:%x\r\n",ck_code);
	SEGGER_RTT_printf(0,"mng_code:%x\r\n",mng_code);
	
	SEGGER_RTT_printf(0,"read_TypeA_Data[17]:%x\r\n",read_TypeA_Data[17]);
	SEGGER_RTT_printf(0,"read_TypeA_Data[16]:%x\r\n",read_TypeA_Data[16]);
	
	#endif
	
	//intime = 
	
	if(read_TypeA_Data[17]==0x01)  //�������ǿͿ�ʱ
	{
		//�ж����к�
		//�����ϵ����к� ���浽����
		anth_index = read_TypeA_Data[29];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[30];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[31];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[32];
		
		//�����ڵ���λ���ܿ鱣�浽����
		
		Card_Ck_Data = read_TypeA_Data[18];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[19];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[20];

		
		//���뿪ʼʱ��
		
		intime = read_TypeA_Data[21];
		intime<<=8;
		
		intime |= read_TypeA_Data[22];
		intime<<=8;
		
		intime |= read_TypeA_Data[23];
		intime <<=8;
		
		intime |= read_TypeA_Data[24];
		
		
		outtime = read_TypeA_Data[25];
		outtime <<=8;
		
		outtime |= read_TypeA_Data[26];
		outtime <<=8;
		
		outtime |= read_TypeA_Data[27];
		outtime <<=8;
		
		outtime |= read_TypeA_Data[28];
		
		

			if (ck_auth_index>anth_index)  //�����ص����кŴ��ڿ�Ƭʱ ����Ч
		{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Error_ck_auth_index>anth_index!\r\n");
				#endif
			
				input_speak(29);					//�ڱ�����Ŵ��ڿ�Ƭ�����ʱ���������Ų���ʾ��(������)
				nrf_delay_ms(500);

				
				for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
				return 0;
		
		}
//		
		//Ck_Data = (((dvc_id%100000) * (ck_code%100000)));//%1000000;
		
		Ck_Data = ((dvc_id + ck_code) + anth_index)%1000000;
		
	//	Ck_Data = ((dvc_id%1000000+ck_code%1000000)+ck_auth_index%1000000);
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Ck_Data:%d\r\n",Ck_Data);
		SEGGER_RTT_printf(0,"Card_Ck_Data:%d\r\n",Card_Ck_Data);
		
		#endif
		
		if((Card_Ck_Data==Ck_Data)&&(TIME_STATE==true)&&((intime<Unix_T)&&(outtime>Unix_T)))  //����Կ��ȷ����ʼʱ��С�����ڣ�����ʱ��������ڵ�ʱ���  (�Ϳ�)
		{
			if(anth_index>ck_auth_index) //����Ƭ�ϵ����кŴ��ڱ���ʱ
			{
				
				Ev_Cat_Dat(CK_Auth_Index_Code,anth_index); //�������к�
				ck_auth_index  =anth_index; //ʹ���µĿͿ����к�
				
			}			
			
			nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
			nrf_delay_ms(1); //�ȴ��ȶ�
			if(nrf_gpio_pin_read(5)==0)   //���ŷ���ʱ ��������
			{
				nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
				
				input_speak(17); //������ʾ�ѷ���
				nrf_delay_ms(1200);
				
				for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
				return 0;
			}
			
			nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
			
			DatCrd_AU =2;		//�Ϳ�����2
			
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
			
			return 1;
		
		}
		else if((intime>Unix_T)||(outtime<Unix_T))  //�ͻ��������ݿ�ʱ��ʧЧ
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Error_CK_Time_Error!\r\n");
				
			#endif
			input_speak(16); //ʱ����Ч����ʾ��Ƭ����
			nrf_delay_ms(1000);
		}
		else if(Card_Ck_Data!=Ck_Data)
		{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Error_ck_Error!\r\n");
				#endif
		}
	}
	else if(read_TypeA_Data[17]==0x02)  //�������ǹ���ʱ
	{
				//�ж����к�
		//�����ϵ����к� ���浽����
		anth_index = read_TypeA_Data[31];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[32];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[33];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[34];
		
		//�����ڵ���λ���ܿ鱣�浽����
		
		Card_Ck_Data = read_TypeA_Data[18];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[19];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[20];
		
		Card_Floor_Num = read_TypeA_Data[21];
		Card_Floor_Num<<=8;
		
		Card_Floor_Num |= read_TypeA_Data[22];
		
				//���뿪ʼʱ��
		
		intime = read_TypeA_Data[23];
		intime<<=8;
		
		intime |= read_TypeA_Data[24];
		intime<<=8;
		
		intime |= read_TypeA_Data[25];
		intime <<=8;
		
		intime |= read_TypeA_Data[26];
		
		
		outtime = read_TypeA_Data[27];
		outtime <<=8;
		
		outtime |= read_TypeA_Data[28];
		outtime <<=8;
		
		outtime |= read_TypeA_Data[29];
		outtime <<=8;
		
		outtime |= read_TypeA_Data[30];
		
		
		Ck_Data = (mng_code + Card_Floor_Num + anth_index)%1000000;
		

		if(nmg_auth_index>anth_index)  //�����ص����кŴ��ڿ�Ƭʱ
		{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Error_nmg_auth_index>anth_index!\r\n");
				#endif
			
				input_speak(29);					//�ڱ�����Ŵ��ڿ�Ƭ�����ʱ���������Ų���ʾ��(������)
				nrf_delay_ms(500);
				
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
			
			return 0;  //����Ч
		
		}
		
		if(Card_Floor_Num!=0)  //����Ƭ��¥�㿨ʱ
		{
				nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�
				nrf_delay_ms(1); //�ȴ��ȶ�
				if(nrf_gpio_pin_read(5)==0)   //���ŷ���ʱ ��������
				{
					nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
					
					input_speak(17); //������ʾ�ѷ���
					nrf_delay_ms(1200);
					
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
					
					return 0;
				}
				
			nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
			
		}

		
		if((TIME_STATE==true)&&((Card_Floor_Num==Floor_Num)||(Card_Floor_Num==0))&&((intime<Unix_T)&&(outtime>Unix_T))&&
			(Card_Ck_Data==Ck_Data))
		{
			if(anth_index>nmg_auth_index) //����Ƭ�ϵ����кŴ��ڱ���ʱ
			{
				
				Ev_Cat_Dat(Nmg_Auth_Index_Code,anth_index); //�������к�
				ck_auth_index  =anth_index; //ʹ���µĿͿ����к�
				
			}
			
			DatCrd_AU =1;  //��������1
			
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
			
			return 1;
			
		}
		else if((intime>Unix_T)||(outtime<Unix_T))  //�����������ݿ�ʱ��ʧЧ
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Error_CK_Time_Error!\r\n");
				
			#endif
			input_speak(16); //ʱ����Ч����ʾ��Ƭ����
			nrf_delay_ms(1000);
			
		}
		
	}
	
//		for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//����ǰ��ջ�������
//		{
//			
//			read_TypeA_Data[i] =0XFF;
//		
//		}		
//	
	
	return 0;
}


