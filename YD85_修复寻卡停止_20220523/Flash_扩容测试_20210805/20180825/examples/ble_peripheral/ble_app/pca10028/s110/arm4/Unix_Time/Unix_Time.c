#include <Unix_Time.h>

//时间转时间戳
uint32_t  Time_to_Unix
(uint16_t year,uint8_t mon,uint8_t day,
uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t year1;
	uint32_t sec_t; //累加秒数
	uint16_t i;
	sec_t =0;
	
	year1 = year-1970;  //计算年份差
	
	for(i =0;i<year1;i++) //计算到本年1月1日8时的时间戳
	{
		if((1970+i)%4) //非闰年
		{
			sec_t+= 3600*24*365;
		}
		else if((1970+i)%100) //非世纪年闰年
		{
			sec_t+=3600*24*366;
		}
		else if((1970+i)%400)  //世纪年的非闰年
		{
			sec_t+=3600*24*365;
		}
		else //世纪年闰年
		{
			sec_t+=3600*24*366;
		}
	}
	
	if (year%4) //当前年份非闰年
	{
		for(i =1;i<mon;i++)
		{
			switch (i)
			{
				case 1: //1月
					sec_t+=3600*24*31;
					break;
				case 2: //2月
					sec_t+=3600*24*28;
					break;
				case 3: //3月
					sec_t+=3600*24*31;
					break;
				case 4: //4月
					sec_t+=3600*24*30;
					break;
				case 5: //5月
					sec_t+=3600*24*31;
					break;
				case 6: //6月
					sec_t+=3600*24*30;
					break;
				case 7: //7月
					sec_t+=3600*24*31;
					break;
				case 8: //8月
					sec_t+=3600*24*31;
					break;
				case 9: //9月
					sec_t+=3600*24*30;
					break;
				case 10: //10月
					sec_t+=3600*24*31;
					break;
				case 11: //11月
					sec_t+=3600*24*30;
					break;
				case 12: //12月
					sec_t+=3600*24*31;
					break;
				
			}
		}
	}
	else if (year%100) //非世纪年闰年
	{
			for(i =1;i<mon;i++)
				{
					switch (i)
					{
						case 1: //1月
							sec_t+=3600*24*31;
							break;
						case 2: //2月
							sec_t+=3600*24*29;
							break;
						case 3: //3月
							sec_t+=3600*24*31;
							break;
						case 4: //4月
							sec_t+=3600*24*30;
							break;
						case 5: //5月
							sec_t+=3600*24*31;
							break;
						case 6: //6月
							sec_t+=3600*24*30;
							break;
						case 7: //7月
							sec_t+=3600*24*31;
							break;
						case 8: //8月
							sec_t+=3600*24*31;
							break;
						case 9: //9月
							sec_t+=3600*24*30;
							break;
						case 10: //10月
							sec_t+=3600*24*31;
							break;
						case 11: //11月
							sec_t+=3600*24*30;
							break;
						case 12: //12月
							sec_t+=3600*24*31;
							break;
						
					}
				}
	}
	else if (year%400) //世纪年的非闰年
	{
			for(i =1;i<mon;i++)
			{
				switch (i)
				{
					case 1: //1月
						sec_t+=3600*24*31;
						break;
					case 2: //2月
						sec_t+=3600*24*28;
						break;
					case 3: //3月
						sec_t+=3600*24*31;
						break;
					case 4: //4月
						sec_t+=3600*24*30;
						break;
					case 5: //5月
						sec_t+=3600*24*31;
						break;
					case 6: //6月
						sec_t+=3600*24*30;
						break;
					case 7: //7月
						sec_t+=3600*24*31;
						break;
					case 8: //8月
						sec_t+=3600*24*31;
						break;
					case 9: //9月
						sec_t+=3600*24*30;
						break;
					case 10: //10月
						sec_t+=3600*24*31;
						break;
					case 11: //11月
						sec_t+=3600*24*30;
						break;
					case 12: //12月
						sec_t+=3600*24*31;
						break;
					
				}
			}

	}
	else //世纪年闰年
	{
			for(i =1;i<mon;i++)
			{
				switch (i)
				{
					case 1: //1月
						sec_t+=3600*24*31;
						break;
					case 2: //2月
						sec_t+=3600*24*29;
						break;
					case 3: //3月
						sec_t+=3600*24*31;
						break;
					case 4: //4月
						sec_t+=3600*24*30;
						break;
					case 5: //5月
						sec_t+=3600*24*31;
						break;
					case 6: //6月
						sec_t+=3600*24*30;
						break;
					case 7: //7月
						sec_t+=3600*24*31;
						break;
					case 8: //8月
						sec_t+=3600*24*31;
						break;
					case 9: //9月
						sec_t+=3600*24*30;
						break;
					case 10: //10月
						sec_t+=3600*24*31;
						break;
					case 11: //11月
						sec_t+=3600*24*30;
						break;
					case 12: //12月
						sec_t+=3600*24*31;
						break;
					
				}
			}
	}
	
	//开始计算日期
	sec_t+=(day-1)*3600*24;
	
	//加上时间
	sec_t+=hour*3600;
	sec_t+=min*60;
	sec_t+=sec;
	
	sec_t-=3600*8;  //减去东八区误差 (北京时间)
	return sec_t;
}


//时间戳转时间
uint16_t time1[6];
/*
生成时间缓存 
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
	uint32_t ysec;  //统计秒数量
	uint16_t mon1; //月统计
	uint16_t day1;  //日统计
	uint16_t hour1; //时统计
	uint16_t min1; //分统计
	
	unix_time+=8*3600;  //起点转换为北京时间 1970/1/1/8:00
	year1 =0;
	ysec =0;
	do
	{	
		unix_time-=ysec; //第一次循环时减0 视为无效
		year1++;
		if((1969+year1)%4) //非闰年
		{
			ysec = 365*3600*24;
		}
		else if((1969+year1)%100) //非世纪闰年
		{
			ysec = 366*3600*24;
		}
		else if((1969+year1)%400) //世纪年的非闰年
		{
			ysec = 365*3600*24;
		}
		else		//世纪年闰年
		{
			ysec = 366*3600*24;
		}
	
	}while(unix_time>=ysec); //当剩余unix时间大于当前年份总时长则继续减
	
	time1[0] = 1969+year1;
	mon1 =0; //从1月份算起
	ysec =0;
	if((1969+year1)%4)  //非闰年
	{
		do
		{
			unix_time-=ysec;  //第一次循环减0 视为无效
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
	else if ((1969+year1)%100) //非世纪闰年
	{
			do
		{
			unix_time-=ysec;  //第一次循环减0 视为无效				
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
	
		}while(unix_time>=ysec); //与下个月长度做比较
	}
	else if ((1969+year1)%400)  //世纪年的非闰年
	{
			mon1++;
			do
			{
				unix_time-=ysec;  //第一次循环减0 视为无效
					
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
	else 			//世纪年闰年
	{
			mon1++;
			do
			{
				unix_time-=ysec;  //第一次循环减0 视为无效
					
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
	//日统计
	do
	{
		day1++;
	}while(unix_time>=(3600*24*day1));//判断剩余时间是否满足一整天
	unix_time-=(3600*24*(day1-1));
	time1[2] = day1;
	//时统计
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
	time1[5] = unix_time;  //剩下秒直接赋值
}
