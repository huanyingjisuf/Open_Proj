#include <Act_PassWord.h>

/**
	 *
	 *  时长动态密码算法 (加密算法)
	 *
	 * @dvc_id 		设备ID，出厂烧入
	 * @ck_code   	设备校验码，设备绑定时由程序写入到设备，设备保存
	 * @random      随机码，0-9 ， 由软件生成，明码
	 * @intime		生效时间，格式为精确到小时的时间戳 ， 例：427280 = 1538208000 / 60 (得分)/ 60 (得时)
	 * @outtime		失效时间，格式为精确到小时的时间戳 ， 例：427280 = 1538208000 / 60 (得分)/ 60 (得时)
	 *
	 *
	 *  入住时长在24小时内的，取6位密码，其中时长块长度为2
	 *  入住时长在72小时内的，取7位密码，其中时长块长度为3
	 *  入住时长在8760小时内的，取8位密码，其中时长块长度为4
	 *  入住时长在8761 - 87600小时内的，取9位密码
	 *
	 *  密码由[加密块(3位)]+[随机数(1位)]+[时长块]组成，例5865 8 56
	 *
	 *  加密块算法：(dvc_id *  intime + ck_node * outtime)%1000
	 *  随机数算法： 软件直接指定，设备直接按照指定位置读取(第4位)
	 *  时长块算法：hour_size(由intime和outtime相减所得) + (((dvc_id + ck_node) * random) %75)
	 * */

/****************
模拟动态密码生成算法  
****************/
extern uint32_t id1,id2;
uint8_t etp[10]; //定义生成密码的字符串 
uint8_t enc_time_psw(uint32_t intime,uint32_t outtime,uint32_t random)
{
	uint64_t hour_size = outtime-intime;
	uint64_t dvc_id;
	//uint32_t id11,id12,id13,id14;
	long long int enc_val;
	uint64_t ck_node;  //设备校验码
	uint64_t last;
	
	dvc_id = id1&0xffff;
	if((dvc_id&0xff00)>0x7F00) //当高八字节高于0x7F 清空高八字节
		dvc_id&=0x00ff; //清空高八字节
	dvc_id<<=16;
	dvc_id|=id2&0xffff; //拼接出完整的ID号
	
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
	etp[2] = enc_val%10; //将值载入数据
	etp[3] = random;
	
	if(hour_size <= 24) //一天以内的卡，密码为六位数
	{ //则计算密码后两位
		last =hour_size + (((dvc_id + ck_node) * random)%75);
		etp[4] =  last/10%10;
		etp[5] =  last%10;
		return 6; //返回生成的密码位数
	}
	else if(hour_size <= 72) //一天以外，三天以内的卡，密码为七位数
	{
		last = hour_size+ ((dvc_id + ck_node)*random%927);
		etp[4] = last/100%10;
		etp[5] = last/10%10;
		etp[6] = last/1%10;
		return 7;
	}
	else if(hour_size <= 24*365) //三天外，365天内的密码
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
     *  永久动态密码算法
     *
     * @dvc_id 		设备ID，出厂烧入
     * @ck_code   	设备校验码，设备绑定时由程序写入到设备，设备保存
     * @random      随机码，0-9 ， 由软件生成，明码
     * @intime      密码生成时间
     *
     *  取时长为8小时的8位密码，正好避开8位时效密码的规则，永久密码的入住时长制作合法校验用，不做密码是否在指定时段有效用
     *
     *  密码由[加密块(3位)]+[随机数(1位)]+[时长块]组成，例5865 8 568
     *
     *  加密块算法：(dvc_id *  intime + ck_node * outtime)%1000
     *  随机数算法： 软件直接指定，设备直接按照指定位置读取(第4位)
     *  时长块算法：hour_size(由intime和outtime相减所得) + (((dvc_id + ck_node) * random) %1239)
     * */

    /**
     *
     *  一次性动态密码算法
     *
     * @dvc_id 		设备ID，出厂烧入
     * @ck_code   	设备校验码，设备绑定时由程序写入到设备，设备保存
     * @random      随机码，0-9 ， 由软件生成，明码
     * @intime      密码生成时间
     *
     *  取时长为6小时的8位密码，正好避开8位时效密码和永久动态密码的规则，一次性动态密码验证通过即存入指定区域，后续输入失效
     *
     *  密码由[加密块(3位)]+[随机数(1位)]+[时长块]组成，例5865 8 568
     *
     *
     *  加密块算法：(dvc_id *  intime + ck_node * outtime)%1000
     *  随机数算法： 软件直接指定，设备直接按照指定位置读取(第4位)
     *  时长块算法：hour_size(由intime和outtime相减所得) + (((dvc_id + ck_node) * random) %1239)
     * */

/*******************************
动态密码解密
*@dvc_id 设备ID 出厂时存在
*@ck_code 设备校验码，设备绑定时由程序写入到设备，设备保存
*@random 随机码，0-9 由软件生成，明码
*@intime 密码生成时间
*@psw 输入密码值
*@psl 输入密码长度
*@unix_t 当前板载unix时间
*  密码由[加密块(3位)]+[随机数(1位)]+[时长块]组成，例5865 8 568
*
*  加密块算法：(dvc_id *  intime + ck_node * outtime)%1000
*  随机数算法： 软件直接指定，设备直接按照指定位置读取(第4位)
*  时长块算法：hour_size(由intime和outtime相减所得) + (((dvc_id + ck_node) * random) %1239)
* */

uint32_t  des_psw(uint8_t *psw,uint8_t psl,uint32_t unix_t)
{
	
//	uint32_t nowHour;
	uint64_t enc_val;
	uint64_t random_val;
	uint64_t last_val =0;
	uint32_t size =0;
	uint64_t dvc_id;
	uint64_t ck_node;  //设备校验码
	uint8_t i;
	uint8_t tab[10]; //输入密码缓存
	uint32_t thour;
	uint32_t outtime;
	
//	nowHour = unix_t;


	
	for(i =0;i<sizeof(tab);i++) //将密码载入缓存
	{
		
		if(i<psl)
			{ 
				tab[i] =*psw;
				psw++;
			}
			else tab[i] =0;
	}
	
	random_val = tab[3];//随机码
	
	dvc_id = id1&0xffff;
	if((dvc_id&0xff00)>0x7F00) //当高八字节高于0x7F 清空高八字节
		dvc_id&=0x00ff; //清空高八字节
	
	dvc_id<<=16;
	dvc_id|=id2&0xffff; //拼接出完整的ID号
	ck_node =0;
	
	ck_node |= Devic_Num[28];
	ck_node<<=8;
	ck_node |= Devic_Num[29];
	ck_node<<=8;
	ck_node |= Devic_Num[30];
	ck_node<<=8;
	ck_node |= Devic_Num[31];
	
	if(psl==6) //当长度为6时
	{
		last_val = tab[4]*10;
		last_val+=tab[5];
		size = last_val- (((dvc_id + ck_node)*random_val)%75);  //当密码长度为6时，获取有效时间
		if(size>24 || size<=00)
			return 0; //不合法密码
	}
	else if (psl==7)
	{
		last_val = tab[4]*100;
		last_val +=tab[5]*10;
		last_val +=tab[6];
		size = last_val - (((dvc_id + ck_node)*random_val)%927);  //当密码长度为7时，获取有效时间
		if(size <= 24 || size>72)
			return 0;  //不合法密码
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
			//8位密码算出时长算出为6 是一次性密码，8位密码算出长度为8则是永久密码
			return 0; //否则视为不合法的密码
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
			return 0; //视为不合法密码
	}
	
	
	enc_val =0;
	enc_val = tab[0]*100;  //将前三位校验码载入变量
	enc_val += tab[1]*10;
	enc_val += tab[2];

	for (int i = 0 ; i < 25 ; i ++)
	{
			thour = unix_t - i ;
			outtime = thour + size;
		
			if(((dvc_id%1000) *  (thour%1000) + (ck_node%1000) * (outtime%1000))%1000 == enc_val)
			{
					size|=i<<24;  //将起始时间与本地时间的差带入返回值的高8字节(几小时前生效 0-24 超出无效)
					return size;
			}
	}
	return 0;
}


///***************************************
// 
//动态密码算法 及判断密码正确
//动态密码保存规则：
//55+AA+密码类型+密码+密码删除时间 一次
//*****************************************/

//void Act_PassWord_Run() //动态密码主函数
//{
//	uint8_t i;
//	uint32_t dvc_id,vrf_id,hour_size,enc_date;
//	uint8_t AP_Num;  //判断是否有动态密码存在
//	AP_Num = Get_Cat_Dat(0x02);  //读取动态密码数量 (0或1)
//	#ifdef uart_start
//	SEGGER_RTT_printf(0,"Act_PassWord_Run!\r\n"); //提示进入动态密码操作
//	#endif
//	if(AP_Num==0) //动态密码开放状态 (无有效动态密码密码)
//	{
//		
//	}
//}

/************************************************************

数据卡 算法

输入： Data_Card_Type   
数据卡类型 
01： 客卡
02：管理卡

返回： 数据卡是否能开门
1 开门
0 不能开门

客卡规则：

1字节卡属性（客卡为1，管理卡为2）+3位加密块+4字节开始时间+4字节结束时间+4位客卡序列号

主卡规则：

1字节卡属性（客卡为1，管理卡为2）+3位加密块 + 2字节楼层号 + 4字节开始时间+4字节结束时间+4位主卡序列号

主体校验码来源： 用于管理卡
		Devic_Num[32] 
		Devic_Num[33] 
		Devic_Num[34] 
		Devic_Num[35] 主体校验码
		
设备校验码来源：  用于客卡
		Devic_Num[28] 
		Devic_Num[29] 
		Devic_Num[30] 
		Devic_Num[31]  替换设备校验码

楼层来源：
		Devic_Num[36]
		Devic_Num[37]
		 
		输入： Card_AU 返回卡类型

*************************************************************/

extern uint32_t Unix_T;  //当前芯片内时间戳

extern uint8_t DatCrd_AU;					//区分数据卡类型
 
extern uint8_t read_TypeA_Data[128];  //A卡数据区域缓存
uint8_t Data_Card_Run()
{
	
	uint64_t dvc_id;			//设备ID 出厂烧入
	uint64_t ck_code;		//设备校验码，设备绑定时有程序写入到设备，设备保存
	uint64_t mng_code;  //主体校验码，设备绑定时由程序写入到设备，设备保存
	uint64_t intime; 		//生效时间
	uint64_t outtime;		//失效时间
	uint64_t ck_auth_index; //授权序列号，授权徐略好设备保存校验成功的序列号，大于等于设备索堡村的序列号方有效，用于客卡
	uint64_t nmg_auth_index;  //授权序列号，用于管理卡
	uint64_t Floor_Num;  //设备楼层号
	
	uint64_t Ck_Data;  //加密计算结果
	uint64_t Card_Ck_Data;  //卡内加密计算结果
	uint64_t anth_index;  //卡内序列号
	uint8_t i;
	
	uint8_t Sen_Tab[12];
	
	
	DatCrd_AU =0;
	
	//加入 修改灵敏度的功能 这种数据格式不允许开门
	
	if((read_TypeA_Data[17]==0x55)&&(read_TypeA_Data[18]==0xAA))
	{
		if(read_TypeA_Data[19]==0xE7)   //检测到 设置灵敏度的指令
		{
			for(i =0;i<12;i++)
			{
				
				Sen_Tab[i] = read_TypeA_Data[20+i];
				
			}
		}
		
		if(TSM12MC_Sen_Code_update(Sen_Tab)==1)   //更新触摸板灵敏度
		{	
			
			input_speak(9);				//提示操作成功
			
			nrf_delay_ms(1000);		//延迟1秒			
		}
		
		return 0;   //必然不允许数据卡方式开门
	}
	
	
	
	
	dvc_id =0;
	ck_code =0;
	mng_code =0;
	outtime =0;
	ck_auth_index =0;
	nmg_auth_index =0;
	Floor_Num =0;
	
	Get_Time();
	Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳	
	
	  //获取客卡与主卡的授权序列号
	ck_auth_index = Get_Cat_Dat(CK_Auth_Index_Code);  //客卡序列号
	nmg_auth_index = Get_Cat_Dat(Nmg_Auth_Index_Code);  //主卡序列号
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"ck_auth_index:%d\r\n",ck_auth_index);
	SEGGER_RTT_printf(0,"nmg_auth_index:%d\r\n",nmg_auth_index);
	
	#endif
	
	Floor_Num = Devic_Num[36];
	Floor_Num<<=8;
	Floor_Num|=Devic_Num[37];	
	
	uint32_t Card_Floor_Num;  //卡内 楼层号
	
	
	dvc_id = id1&0xffff;
	if((dvc_id&0xff00)>0x7F00) //当高八字节高于0x7F 清空高八字节
		dvc_id&=0x00ff; //清空高八字节
	
	dvc_id<<=16;
	dvc_id|=id2&0xffff; //拼接出完整的ID号
	
	
	//获取设备校验码
	
	ck_code = Devic_Num[28];
	ck_code<<=8;
	
	ck_code |= Devic_Num[29];
	ck_code<<=8;
	
	ck_code |= Devic_Num[30];
	ck_code<<=8;
	
	ck_code |= Devic_Num[31];
	
	
	
	//获取设备主体校验码
	
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
	
	if(read_TypeA_Data[17]==0x01)  //当数据是客卡时
	{
		//判断序列号
		//将卡上的序列号 保存到本地
		anth_index = read_TypeA_Data[29];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[30];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[31];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[32];
		
		//将卡内的三位加密块保存到本地
		
		Card_Ck_Data = read_TypeA_Data[18];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[19];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[20];

		
		//载入开始时间
		
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
		
		

			if (ck_auth_index>anth_index)  //当本地的序列号大于卡片时 卡无效
		{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Error_ck_auth_index>anth_index!\r\n");
				#endif
			
				input_speak(29);					//在本地序号大于卡片内序号时，不允许开门并提示滴(被覆盖)
				nrf_delay_ms(500);

				
				for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
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
		
		if((Card_Ck_Data==Ck_Data)&&(TIME_STATE==true)&&((intime<Unix_T)&&(outtime>Unix_T)))  //当密钥正确，开始时间小于现在，结束时间大于现在的时间戳  (客卡)
		{
			if(anth_index>ck_auth_index) //当卡片上的序列号大于本地时
			{
				
				Ev_Cat_Dat(CK_Auth_Index_Code,anth_index); //更新序列号
				ck_auth_index  =anth_index; //使用新的客卡序列号
				
			}			
			
			nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
			nrf_delay_ms(1); //等待稳定
			if(nrf_gpio_pin_read(5)==0)   //当门反锁时 不允许开门
			{
				nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
				
				input_speak(17); //语音提示已反锁
				nrf_delay_ms(1200);
				
				for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
				return 0;
			}
			
			nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
			
			DatCrd_AU =2;		//客卡返回2
			
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
			
			return 1;
		
		}
		else if((intime>Unix_T)||(outtime<Unix_T))  //客户类型数据卡时间失效
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Error_CK_Time_Error!\r\n");
				
			#endif
			input_speak(16); //时间无效，提示卡片到期
			nrf_delay_ms(1000);
		}
		else if(Card_Ck_Data!=Ck_Data)
		{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Error_ck_Error!\r\n");
				#endif
		}
	}
	else if(read_TypeA_Data[17]==0x02)  //当数据是管理卡时
	{
				//判断序列号
		//将卡上的序列号 保存到本地
		anth_index = read_TypeA_Data[31];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[32];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[33];
		anth_index<<=8;
		
		anth_index |= read_TypeA_Data[34];
		
		//将卡内的三位加密块保存到本地
		
		Card_Ck_Data = read_TypeA_Data[18];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[19];
		Card_Ck_Data<<=8;
		
		Card_Ck_Data |= read_TypeA_Data[20];
		
		Card_Floor_Num = read_TypeA_Data[21];
		Card_Floor_Num<<=8;
		
		Card_Floor_Num |= read_TypeA_Data[22];
		
				//载入开始时间
		
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
		

		if(nmg_auth_index>anth_index)  //当本地的序列号大于卡片时
		{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Error_nmg_auth_index>anth_index!\r\n");
				#endif
			
				input_speak(29);					//在本地序号大于卡片内序号时，不允许开门并提示滴(被覆盖)
				nrf_delay_ms(500);
				
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
			
			return 0;  //卡无效
		
		}
		
		if(Card_Floor_Num!=0)  //当卡片是楼层卡时
		{
				nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口
				nrf_delay_ms(1); //等待稳定
				if(nrf_gpio_pin_read(5)==0)   //当门反锁时 不允许开门
				{
					nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
					
					input_speak(17); //语音提示已反锁
					nrf_delay_ms(1200);
					
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
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
			if(anth_index>nmg_auth_index) //当卡片上的序列号大于本地时
			{
				
				Ev_Cat_Dat(Nmg_Auth_Index_Code,anth_index); //更新序列号
				ck_auth_index  =anth_index; //使用新的客卡序列号
				
			}
			
			DatCrd_AU =1;  //管理卡返回1
			
			for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
				{
					read_TypeA_Data[i] =0XFF;
				}			
			
			
			return 1;
			
		}
		else if((intime>Unix_T)||(outtime<Unix_T))  //管理类型数据卡时间失效
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Error_CK_Time_Error!\r\n");
				
			#endif
			input_speak(16); //时间无效，提示卡片到期
			nrf_delay_ms(1000);
			
		}
		
	}
	
//		for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//返回前清空缓存数据
//		{
//			
//			read_TypeA_Data[i] =0XFF;
//		
//		}		
//	
	
	return 0;
}


