#include <Unix_Time.h>

//ʱ��תʱ���
uint32_t  Time_to_Unix
(uint16_t year,uint8_t mon,uint8_t day,
uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t year1;
	uint32_t sec_t; //�ۼ�����
	uint16_t i;
	sec_t =0;
	
	year1 = year-1970;  //������ݲ�
	
	for(i =0;i<year1;i++) //���㵽����1��1��8ʱ��ʱ���
	{
		if((1970+i)%4) //������
		{
			sec_t+= 3600*24*365;
		}
		else if((1970+i)%100) //������������
		{
			sec_t+=3600*24*366;
		}
		else if((1970+i)%400)  //������ķ�����
		{
			sec_t+=3600*24*365;
		}
		else //����������
		{
			sec_t+=3600*24*366;
		}
	}
	
	if (year%4) //��ǰ��ݷ�����
	{
		for(i =1;i<mon;i++)
		{
			switch (i)
			{
				case 1: //1��
					sec_t+=3600*24*31;
					break;
				case 2: //2��
					sec_t+=3600*24*28;
					break;
				case 3: //3��
					sec_t+=3600*24*31;
					break;
				case 4: //4��
					sec_t+=3600*24*30;
					break;
				case 5: //5��
					sec_t+=3600*24*31;
					break;
				case 6: //6��
					sec_t+=3600*24*30;
					break;
				case 7: //7��
					sec_t+=3600*24*31;
					break;
				case 8: //8��
					sec_t+=3600*24*31;
					break;
				case 9: //9��
					sec_t+=3600*24*30;
					break;
				case 10: //10��
					sec_t+=3600*24*31;
					break;
				case 11: //11��
					sec_t+=3600*24*30;
					break;
				case 12: //12��
					sec_t+=3600*24*31;
					break;
				
			}
		}
	}
	else if (year%100) //������������
	{
			for(i =1;i<mon;i++)
				{
					switch (i)
					{
						case 1: //1��
							sec_t+=3600*24*31;
							break;
						case 2: //2��
							sec_t+=3600*24*29;
							break;
						case 3: //3��
							sec_t+=3600*24*31;
							break;
						case 4: //4��
							sec_t+=3600*24*30;
							break;
						case 5: //5��
							sec_t+=3600*24*31;
							break;
						case 6: //6��
							sec_t+=3600*24*30;
							break;
						case 7: //7��
							sec_t+=3600*24*31;
							break;
						case 8: //8��
							sec_t+=3600*24*31;
							break;
						case 9: //9��
							sec_t+=3600*24*30;
							break;
						case 10: //10��
							sec_t+=3600*24*31;
							break;
						case 11: //11��
							sec_t+=3600*24*30;
							break;
						case 12: //12��
							sec_t+=3600*24*31;
							break;
						
					}
				}
	}
	else if (year%400) //������ķ�����
	{
			for(i =1;i<mon;i++)
			{
				switch (i)
				{
					case 1: //1��
						sec_t+=3600*24*31;
						break;
					case 2: //2��
						sec_t+=3600*24*28;
						break;
					case 3: //3��
						sec_t+=3600*24*31;
						break;
					case 4: //4��
						sec_t+=3600*24*30;
						break;
					case 5: //5��
						sec_t+=3600*24*31;
						break;
					case 6: //6��
						sec_t+=3600*24*30;
						break;
					case 7: //7��
						sec_t+=3600*24*31;
						break;
					case 8: //8��
						sec_t+=3600*24*31;
						break;
					case 9: //9��
						sec_t+=3600*24*30;
						break;
					case 10: //10��
						sec_t+=3600*24*31;
						break;
					case 11: //11��
						sec_t+=3600*24*30;
						break;
					case 12: //12��
						sec_t+=3600*24*31;
						break;
					
				}
			}

	}
	else //����������
	{
			for(i =1;i<mon;i++)
			{
				switch (i)
				{
					case 1: //1��
						sec_t+=3600*24*31;
						break;
					case 2: //2��
						sec_t+=3600*24*29;
						break;
					case 3: //3��
						sec_t+=3600*24*31;
						break;
					case 4: //4��
						sec_t+=3600*24*30;
						break;
					case 5: //5��
						sec_t+=3600*24*31;
						break;
					case 6: //6��
						sec_t+=3600*24*30;
						break;
					case 7: //7��
						sec_t+=3600*24*31;
						break;
					case 8: //8��
						sec_t+=3600*24*31;
						break;
					case 9: //9��
						sec_t+=3600*24*30;
						break;
					case 10: //10��
						sec_t+=3600*24*31;
						break;
					case 11: //11��
						sec_t+=3600*24*30;
						break;
					case 12: //12��
						sec_t+=3600*24*31;
						break;
					
				}
			}
	}
	
	//��ʼ��������
	sec_t+=(day-1)*3600*24;
	
	//����ʱ��
	sec_t+=hour*3600;
	sec_t+=min*60;
	sec_t+=sec;
	
	sec_t-=3600*8;  //��ȥ��������� (����ʱ��)
	return sec_t;
}


//ʱ���תʱ��
uint16_t time1[6];
/*
����ʱ�仺�� 
time1[0] year
time1[1] mon
time1[2] day
time1[3] hour
time1[4] min
time1[5] sec

*/

void Unix_Time_to_Time(uint32_t unix_time)  
{
	uint16_t year1;
	uint32_t ysec;  //ͳ��������
	uint16_t mon1; //��ͳ��
	uint16_t day1;  //��ͳ��
	uint16_t hour1; //ʱͳ��
	uint16_t min1; //��ͳ��
	
	unix_time+=8*3600;  //���ת��Ϊ����ʱ�� 1970/1/1/8:00
	year1 =0;
	ysec =0;
	do
	{	
		unix_time-=ysec; //��һ��ѭ��ʱ��0 ��Ϊ��Ч
		year1++;
		if((1969+year1)%4) //������
		{
			ysec = 365*3600*24;
		}
		else if((1969+year1)%100) //����������
		{
			ysec = 366*3600*24;
		}
		else if((1969+year1)%400) //������ķ�����
		{
			ysec = 365*3600*24;
		}
		else		//����������
		{
			ysec = 366*3600*24;
		}
	
	}while(unix_time>=ysec); //��ʣ��unixʱ����ڵ�ǰ�����ʱ���������
	
	time1[0] = 1969+year1;
	mon1 =0; //��1�·�����
	ysec =0;
	if((1969+year1)%4)  //������
	{
		do
		{
			unix_time-=ysec;  //��һ��ѭ����0 ��Ϊ��Ч
			mon1++;	
			switch (mon1)
				{
				case 1:
					ysec = 3600*24*31;
					break;
				case 2:
					ysec = 3600*24*28;
					break;
				case 3:
					ysec = 3600*24*31;
					break;
				case 4:
					ysec = 3600*24*30;
					break;
				case 5:
					ysec = 3600*24*31;
					break;
				case 6:
					ysec = 3600*24*30;
					break;
				case 7:
					ysec = 3600*24*31;
					break;
				case 8:
					ysec = 3600*24*31;
					break;
				case 9:
					ysec = 3600*24*30;
					break;
				case 10:
					ysec = 3600*24*31;
					break;
				case 11:
					ysec = 3600*24*30;
					break;
				case 12:
					ysec = 3600*24*31;	
					break;
				}
		}while(unix_time>=ysec);
	}
	else if ((1969+year1)%100) //����������
	{
			do
		{
			unix_time-=ysec;  //��һ��ѭ����0 ��Ϊ��Ч				
			mon1++;
				switch (mon1)
					{
					case 1:
						ysec = 3600*24*31;
						break;
					case 2:
						ysec = 3600*24*29;
						break;
					case 3:
						ysec = 3600*24*31;
						break;
					case 4:
						ysec = 3600*24*30;
						break;
					case 5:
						ysec = 3600*24*31;
						break;
					case 6:
						ysec = 3600*24*30;
						break;
					case 7:
						ysec = 3600*24*31;
						break;
					case 8:
						ysec = 3600*24*31;
						break;
					case 9:
						ysec = 3600*24*30;
						break;
					case 10:
						ysec = 3600*24*31;
						break;
					case 11:
						ysec = 3600*24*30;
						break;
					case 12:
						ysec = 3600*24*31;	
						break;
					}
	
		}while(unix_time>=ysec); //���¸��³������Ƚ�
	}
	else if ((1969+year1)%400)  //������ķ�����
	{
			mon1++;
			do
			{
				unix_time-=ysec;  //��һ��ѭ����0 ��Ϊ��Ч
					
				switch (mon1)
					{
					case 1:
						ysec = 3600*24*31;
						break;
					case 2:
						ysec = 3600*24*28;
						break;
					case 3:
						ysec = 3600*24*31;
						break;
					case 4:
						ysec = 3600*24*30;
						break;
					case 5:
						ysec = 3600*24*31;
						break;
					case 6:
						ysec = 3600*24*30;
						break;
					case 7:
						ysec = 3600*24*31;
						break;
					case 8:
						ysec = 3600*24*31;
						break;
					case 9:
						ysec = 3600*24*30;
						break;
					case 10:
						ysec = 3600*24*31;
						break;
					case 11:
						ysec = 3600*24*30;
						break;
					case 12:
						ysec = 3600*24*31;	
						break;
					}

			}while(unix_time>=ysec);
	}
	else 			//����������
	{
			mon1++;
			do
			{
				unix_time-=ysec;  //��һ��ѭ����0 ��Ϊ��Ч
					
				switch (mon1)
					{
					case 1:
						ysec = 3600*24*31;
						break;
					case 2:
						ysec = 3600*24*29;
						break;
					case 3:
						ysec = 3600*24*31;
						break;
					case 4:
						ysec = 3600*24*30;
						break;
					case 5:
						ysec = 3600*24*31;
						break;
					case 6:
						ysec = 3600*24*30;
						break;
					case 7:
						ysec = 3600*24*31;
						break;
					case 8:
						ysec = 3600*24*31;
						break;
					case 9:
						ysec = 3600*24*30;
						break;
					case 10:
						ysec = 3600*24*31;
						break;
					case 11:
						ysec = 3600*24*30;
						break;
					case 12:
						ysec = 3600*24*31;	
						break;
					}

		}while(unix_time>=ysec);
	}
	time1[1] = mon1;
	day1 =0;
	//��ͳ��
	do
	{
		day1++;
	}while(unix_time>=(3600*24*day1));//�ж�ʣ��ʱ���Ƿ�����һ����
	unix_time-=(3600*24*(day1-1));
	time1[2] = day1;
	//ʱͳ��
	hour1 =0;
	while(unix_time>=3600)
	{
		hour1++;
		unix_time-=3600;
	}
	time1[3] = hour1;
	min1 =0;
	while(unix_time>=60)
	{
		min1++;
		unix_time-=60;
	}
	time1[4] = min1;
	time1[5] = unix_time;  //ʣ����ֱ�Ӹ�ֵ
}
