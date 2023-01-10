#include <Flash_RW.h>
#include <BLE_Data.h>



//卡片存储扇区范围到 440 号为止，新的功能扇区从445号开始

const uint16_t TSM12MC_Sen_Sector_Num =447;		//定义保存按键灵敏度的扇区

//目录区 扇区参数

/*
备注：  
目录的扇区并不是同时使用，而是按扇区顺序轮流写入保存同一个数据 
以平衡Flash擦写磨损，以达到理想的理论擦写次数
写入下一个扇区时，需要使其他扇区无效化  （数据清零）
*/


const uint8_t Cat_Dat_Sector1 = 10;						//设定目录扇区的起始地址

const uint8_t Cat_Dat_Sector_Num =10;				// 设定 目录 消耗的 扇区数量

const uint32_t Cat_Dat_len =256;				// 设定 目录 数据的总长度



const uint8_t PassWord_Len = 18;  //单条密码数据长度

const uint8_t Open_Note_Num_Len = 26; //单条开门记录长度

const uint8_t PassWord_Secotr1 = 2;  //密码数据所在扇区

const uint8_t PassWord_Secotr2 = 3;  //密码数据所在扇区

const uint8_t PassWord_SN_Secotr = 4;  //密码编号存储扇区 (每个自定义密码的编号)

const uint8_t Act_PassWord_Secotr = 5;  //存储动态密码

const uint8_t History_PassWord_Secotr = 6;          //历史动态密码

const uint8_t Per_Act_PassWord_Secotr = 7;				//保存永久动态密码

const uint16_t More_Act_PassWord_Secotr = 445;				//保存多条动态密码的目录（目前共计一个扇区）

const uint16_t More_Act_PassWord_Len = 17;					//定义多条动态密码数据长度

const uint8_t More_Act_PassWord_MAX_Num = 100;					//定义多条动态密码区容量

const uint16_t AD_TypeA_Data_Secotr1 = 25;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint16_t AD_TypeB_Data_Secotr1 = 130;  //保存B卡数据的扇区1 (目前共计1个扇区)

const uint16_t US_TypeA_Data_Secotr1 = 235;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint16_t US_TypeB_Data_Secotr1 = 340;  //保存B卡数据的扇区1 (目前共计1个扇区)

// 上方每个扇区头 间隔 105 个扇区为了保证数据安全，需要将最大扇区数控制在100个以内

const uint8_t TypeA_Data_Len = 14;  	//一条A卡数据的长度

const uint8_t TypeB_Data_Len = 18;		//一条B卡数据的长度

const uint8_t A_Sector_TypeA_Data_Num = 100;			//一个扇区保存的 A 卡 数据数量

const uint8_t A_Sector_TypeB_Data_Num = 100;			// 一个扇区保存的 B 卡 数据数量

const uint8_t AD_TypeA_Data_Sector_Num =5;		// AD_TypeA_Data  保存数据使用的扇区个数

const uint8_t US_TypeA_Data_Sector_Num =5;		// US_TypeA_Data  保存数据使用的扇区个数

const uint8_t AD_TypeB_Data_Sector_Num =5;		// AD_TypeB_Data  保存数据使用的扇区个数

const uint8_t US_TypeB_Data_Sector_Num =5;		// US_TypeB_Data  保存数据使用的扇区个数


extern u8 Op_Door_Buff[Open_Note_Num_Len];				//用于缓存开门记录数据 待写入缓存


/*(存储1个动态密码和n个一次密码)
起始位置存储最后一个有效的时效或永久动态密码 

之后区域全用于存储一次密码用于防止一次密码二次开门

格式： 第一个密码

*/

const uint8_t Flash_Word_Num =15;  //定义 Flash 目录类型数量

const uint8_t OpenDoorNote_Num_Code =0x11; 		//开门记录数值



//0x03;  //定义 已失效动态密码记录
//0X01;  //定义密码数量 类型

const uint8_t Act_PassWord_Num_Code =0x02;// 动态密码存储目录编号

const uint8_t Failure_Act_PassWord_Num_Code = 0X03;				//Sec_dat[i*7+2] = 0x03;  //定义 已失效动态密码记录

const uint8_t Per_Act_PassWord_Num_Code =0x04;// 永久动态密码存储目录编号

const uint8_t More_Act_PassWord_Num_Code = 0x05;			//多组时效动态密码存储目录

const uint8_t AD_TypeA_Data_Num_Code = 0X12; // 管理员A卡信息存储目录编号

const uint8_t AD_TypeB_Data_Num_Code = 0X13; // 管理员B卡信息存储目录编号

const uint8_t US_TypeA_Data_Num_Code = 0X14; // 用户A卡信息存储目录编号

const uint8_t US_TypeB_Data_Num_Code = 0X15; // 用户B卡信息存储目录编号

const uint8_t CK_Auth_Index_Code = 0x16;  //定义设备客人数据卡序列号 @ck_auth_index

const uint8_t Nmg_Auth_Index_Code = 0x17;  //定义设备管理数据卡序列号 @ nmg_auth_index

const uint8_t PS_Print_Num_Code = 0x18;			//定义保存的有效指纹数量



const uint16_t Open_Note_Secotr1 = 700;
const uint16_t Open_Note_Secotr2 = 701;
const uint16_t Open_Note_Secotr3 = 702;
const uint16_t Open_Note_Secotr4 = 703; 

//四个开门记录专用扇区


uint8_t Read_Op_Tab[Open_Note_Num_Len];

const uint16_t Open_Note_Size =240;  //定义开门记录容量
const uint16_t A_Sector_Open_Note_Size = 60;

const uint8_t Max_PassWord_Num = 200;  //最大密码数量
const uint8_t A_Sector_PassWord_Num = 100;  //定义单个密码扇区所能容纳的密码数量



const uint8_t PS_Print_Len = 13;		//0x55+2字节指纹类型+2字节指纹ID+4字节起始实际+4字节失效实际

const uint8_t PS_Print_Num =100;			//保存到指纹数量

const uint16_t PS_Print_Soctor1 = 446;  //定义指纹数据扇区1



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
//FLASH_BLOCK_NUM :　(0-63) 
//FLASH_BLOCK_NUM: (0-15)
void SPI_Flash_Busy()  //扇区擦除用等待函数
{
	
  uint32_t SR_dat;
	uint32_t i; 
	i =0; // 等待循环次数
	do
	{
  SR_dat = SpiFlash_ReadSR();
	nrf_delay_us(1);
	i++;
		if(i>400000) break;   //写入等待超时
	}while((SR_dat&SPIFLASH_WRITE_BUSYBIT)==0x01);
	
	nrf_delay_us(1);
}


/*********************************
extern uint8_t Devic_Num[];  //Devic_Num 声明
在绑定前，用于保存带写入绑定号
，绑定后，用于保存从Flash中读取的绑定号
*********************************/
/******************************
读取设备绑定号
位置： 0 Block 0 Sector 0 PAGE 
位置： 0 Block 0 Sector 1 PAGE //备份设备绑定号

备注： 整个 0 Block 0 Sector 仅保存设备绑定号

*********************************/
void Read_Devic_Num ()  
{
 	uint8_t read_tab [Devic_PAGE_Len];
  uint8_t i;
	i =0;
  SpiFlash_Read_Data 
  (Devic_Num,Devic_Num_BLOCK,Devic_Num_PAGE,sizeof(read_tab));//获取数据
   
  if( read_tab[0] == PAGE_Heart_H &&
     read_tab[1] == PAGE_Heart_L ) //判断起始数据有效性
    {
			for(i =0;i<Devic_Num_Len;i++)  //确定数据有效后，读取数据到缓存
			{
				Devic_Num[i] = read_tab[i+2];		
			}
    }
		else
			Devic_Static = false;
}

/***************************************
写入设备绑定号
位置： 0 Block 0 Sector 0 PAGE
位置： 0 Block 0 Sector 1 PAGE //备份设备绑定号

备注：整个 0 Block 0 Sector 仅保存设备绑定号
写设备绑定号，仅允许发生在设备号被清空后，即2bit页起始码不合理时
***************************************/


void Write_Devic_Num () //写入新的绑定数据
{
	uint8_t read_tab [Devic_PAGE_Len];
	uint8_t tab[Devic_PAGE_Len];
	uint32_t i;
//	uint32_t dat;
	i =0;
	SpiFlash_Read_Data
	(read_tab,Devic_Num_BLOCK,
	Devic_Num_PAGE,Devic_PAGE_Len); //获取数据确定数据有效性
	
	if(read_tab[0] == PAGE_Heart_H &&
		read_tab[1] == PAGE_Heart_L ) //判断起始数据有效性
		{
			//当起始数据有效时，不对绑定区做出改变
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Devic_Num not Write!\r\nDevic_static_true!\r\n");
			
			#endif
		}
		else
		{  //当起始数据无效时，开始写入数据
			SPIFlash_Erase_Sector(Devic_Num_BLOCK,Devic_Num_Sector); //对 设备绑定好所在扇区进行擦除
			
			SPI_Flash_Busy(); //等待擦除完成
			
			tab[0] = PAGE_Heart_H;
			tab[1] = PAGE_Heart_L;
			for (i =0;i<Devic_Num_Len;i++)
			{
				tab[i+2] = Devic_Num[i];
			}
			SpiFlash_Write_Data(tab,Devic_Num_BLOCK,Devic_Num_PAGE,sizeof(tab));
			SPI_Flash_Busy(); //等待写入完成
			
			SpiFlash_Read_Data (read_tab,Devic_Num_BLOCK,Devic_Num_PAGE,Devic_PAGE_Len);
			#ifdef uart_start 
			SEGGER_RTT_printf(0,"Devic_Num Write OK!\r\nDevic_Num:\r\n");
			for(i =0;i<sizeof(read_tab);i++)
			SEGGER_RTT_printf(0,"%x,",read_tab[i]);
			SEGGER_RTT_printf(0,"\r\n",read_tab[i]);
			
			#endif
		}
}

void CL_Devic_Num()  //清除设备绑定信息
{
	//Spi_Flash_Init ();
	
	SPIFlash_Erase_Sector(Devic_Num_BLOCK,Devic_Num_Sector);  //清除设备绑定信息
	
	//Spi_Flash_Sleep();
	
}

/********************************************************

将输入的页数据(一片有64*256页)，转换为页和块的组合
用于写入寻址

*******************************************************/

void PAGE_to_BLOCK_PAGE(uint32_t *BLOCK,uint32_t *PAGEO,uint32_t PAGE)
{
		*BLOCK = PAGE/256; //每块有256页(0-255) 写入为按页写入
		*PAGEO = PAGE%256; 
}

/*************************************************************

将输入的页数据(一片有64*256页),转换为扇区和块的组合
用于寻到页对应的扇区进行删除，或功能扇区寻找

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
用于获取，扇区所在的块编号，实际使用中以扇区为基本分割单位。
,并生成对应块的页号 (写入时以页为最小单位)
将扇区号转换为可以直接用于写操作的块和页的组合
***************************************************************/
void Sector_to_Block_Page(uint32_t *Block,uint32_t *PAGE,uint32_t SECTOR)
{
	if(SECTOR<16)
	{
		*PAGE = SECTOR*16; //扇区转换为页
		*Block =0;
	}
	else
	{
		*PAGE = SECTOR%16*16; //由于扇区的特性必然是页的整倍数
		*Block = SECTOR/16;
	}
}
/***************************************************************
用于获取，扇区所在块和扇区号，
将扇区号转换为，块和扇区号的组合，用于擦除扇区时，获得基本参数

***************************************************************/

void Sector_to_Block_Sector(uint32_t *Block,uint32_t *Sector,uint32_t SECTOR)
{
	if(SECTOR<16)
	{
		*Sector = SECTOR;  //当输入扇区小于起始扇区大小时
		*Block = 0;
	}
	else
	{
		*Sector = SECTOR%16;  //得出实际所在扇区号
		*Block = SECTOR/16;
	}
}

/*************************************************************
清除数据功能：给对应页写入0x00，
实现清空密码的功能，0
并且写入预设初始数据，

(当前只预设3种数据类型，密码，动态密码，开门记录)
*************************************************************/

void CL_FlashWord_Num() //清除数据量存储数据(当密码数量无效时，密码数据同时失效)
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
	
	for(i =0;i<Cat_Dat_Sector_Num;i++)   // 擦除所有 目录扇区
	{
		
		Sector_to_Block_Sector(&Block,&Sector,Cat_Dat_Sector1+i);  //获取扇区对应区块号与扇区号
		SPI_Flash_Busy();
		SPIFlash_Erase_Sector ( Block,Sector ); //用获取到的数据，擦除对应区块
		SPI_Flash_Busy();
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"CL_Cat_Dat_Sector_Num: %d ! \r\n",i+Cat_Dat_Sector1);
		
		#endif
		
	}
		
//	Sector_to_Block_Sector(&Block,&Sector,Cat_Dat_Sector1);  //获取扇区第一个区块号与扇区号
	
	//SPIFlash_Erase_Sector ( Block,Sepwctor ); //用获取到的数据，擦除对应区块
	
	SPI_Flash_Busy();
	
	Sector_to_Block_Page(&Block,&Page,Cat_Dat_Sector1);  //用扇区号，获取块号与页号
	
	for(i =0;i<Flash_Word_Num;i++)  //写入三组预留数据区间
	{
		
		Sec_dat[i*7] = 0X55;
		Sec_dat[i*7+1] = 0xAA;
		switch (i)
		{
			case 0: 
				Sec_dat[i*7+2] = 0X01;  //定义密码数量 类型
					break;
			
			case 1:
				Sec_dat[i*7+2] = 0x02;  //动态密码数量
					break;
			
			case 2:
				Sec_dat[i*7+2] = 0x03;  //定义 已失效动态密码记录
					break;
			
			case 3:
				Sec_dat[i*7+2] = 0x11;  //定义开门记录 类型
					break;
			
			case 4:
					Sec_dat[i*7+2] = 0x12;  //定义管理员A卡片信息  类型
					break;
			
			case 5:
				Sec_dat[i*7+2] = 0x13;  //定义管理员身份证信息 类型
					break;
			
			case 6:
				Sec_dat[i*7+2] = 0x14;  //定义用户A卡信息类型
					break;
			
				case 7:
				Sec_dat[i*7+2] = 0x15;  //定义用户身份证信息类型
					break;
				
				case 8:
					Sec_dat[i*7+2] = 0x16;  //定义设备客人数据卡序列号 @ck_auth_index
				break;
				
				case 9:
					Sec_dat[i*7+2] = 0x17;  //定义设备管理数据卡序列号 @ nmg_auth_index
				break;
				
				case 10:
					Sec_dat[i*7+2] = 0x18;			//定义保存的有效指纹数量	
				break;
				
				case 11: 
					Sec_dat[i*7+2] = 0x04;			//永久动态密码数量
				break;
				
				case 12:
					
					Sec_dat[i*7+2] = More_Act_PassWord_Num_Code;			//多组动态密码数量
	
				//const uint8_t More_Act_PassWord_Num_Code = 0x05;			//多组时效动态密码存储目录

				break;
				
			default:  
				
			//	i =0;   //i 出错，重新循环 正常情况下不可能发生
				break;
		}
		
		Sec_dat[i*7+3] = 0x00;  // 0xFF表示该目录未被使用过(未激活使用)，之后清空为0x00 便于观察
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
	
	Erase_PS_Print_Sector();		//重置数据时清空指纹数据
	
		CL_PS_ID	 =0;			//清空所有指纹信息
		CL_PS_Run = CL_PS_Run_Del_All_Mode;
}

/******************************************************************
SPI_Flash 1个扇区内任意字节读取 以页为基准起始位置,数据精确到字节
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
		page_n = tab_len/256;  //算出待读取的数据有多少页
		page = Page_Num;				//起始页载入
		for(i =0;i<page_n;i++)
		{
			SpiFlash_Read_Data (pBuffer+i*256,Block_Num,page+i,256);  //选择对应页，按顺序读取数据
		}
		if (tab_len%256>0)  //当最后还有余数时 再读取一次余数
		{
			SpiFlash_Read_Data (pBuffer+page_n*256,Block_Num,page+page_n,tab_len%256);  //将最后的非整页数据读取出来 (一般用于信息数据获取)
		}				
	}
}
/*************************************************************************
SPI_Flash 1个扇区内任意字节写入 以页为基准起始位置，数据精确到字节
**************************************************************************/

void SPIFlash_Write_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len) 
{
	uint32_t page_n;
	uint32_t page;
	uint32_t i;
	
	if(tab_len<256)
	{
		
		SpiFlash_Write_Data(pBuffer,Block_Num,Page_Num,tab_len); //载入长度小于一页，则只写入一页数据
		
	}
		else
	{
		page_n = tab_len/256;  //算出待写入数据有多少页
		page = Page_Num;				//起始页载入
		for(i =0;i<page_n;i++)
			{
				SpiFlash_Write_Data (pBuffer+i*256,Block_Num,page+i,256);  //选择对应页，按顺序写入数据
				SPI_Flash_Busy();
			}
			if (tab_len%256>0)  //当最后还有余数时 再写入一次余数
			{
				SpiFlash_Write_Data (pBuffer+page_n*256,Block_Num,page+page_n,tab_len%256);  //将最后的非整页数据写入到扇区 (一般用于信息数据写入)
				SPI_Flash_Busy();
			}
	}
	SPI_Flash_Busy();
}

/*******************************************************************
仅清空密码数据
用于清除所有密码数据，保留其他数据

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
//	Sector_to_Block_Page(&Block,&Page,1); //用扇区号,获取块号与页号
//	
//	SpiFlash_Read_Data (tab_dat1,Block,Page,sizeof(tab_dat1)); //读取数据到TAB 
//	SPI_Flash_Busy();
//	
//	for(i =0;i<3;i++)
//	{
//		if (tab_dat1[i*6]==0X55&&tab_dat1[i*6+1]==0xAA) //当数据有效时开始处理
//		{
//			if(tab_dat1[i+2]==0x01)  //选择密码所属目录进行清除
//			{
//				 tab_dat1[i+3] = 0x00;
//				 tab_dat1[i+4] = 0x00;
//				 tab_dat1[i+5] = 0x00;
//			}
//		}
//	}
//	Sector_to_Block_Sector(&Block,&Sector,1); //擦除所在页 因为目录大小目前在一页以内(256byte)
//	SPI_Flash_Busy();
//	SPIFlash_Erase_Sector(Block,Sector);     //擦除扇区
//	SPI_Flash_Busy();
//	Sector_to_Block_Page(&Block,&Page,1);  //获取需要写入的位置 块、页号
//	SpiFlash_Write_Data(tab_dat1,Block,Page,sizeof(tab_dat1));  //写入一页 作为目录
// 	SPI_Flash_Busy();
//	
//	//SPIFlash_Erase_Sector(0,0);
//	SPI_Flash_Busy();
//	
////	SpiFlash_Read_Data(tab_dat,Block,page,sizeof(tab_dat));
//}

void CL_Flash_PassWord()  //清空密码数量
{
	Ev_Cat_Dat(PassWord_Num_code,0); //清空密码
}

void CL_Flash_OPEN_DOOR_NOTE()  //清空开门记录数量
{
	Ev_Cat_Dat(OpenDoorNote_Num_Code,0); //清空密码
}



/***************************************************
获取指定数据目录信息

输入：数据类型 Cat_Type
返回：目录类型下数据内容
***************************************************/

//uint32_t Get_Cat_Dat(uint8_t Cat_Type) //获取目录信息
//{
//		uint8_t tab_dat[256];  //目录缓存
//		uint32_t i;
//		uint32_t Block,Page;
//		
//		uint32_t len_i;
//		
//	for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] = 0xFF;
//	Sector_to_Block_Page (&Block,&Page,1);    //目录所在扇区号为 1
//  	len_i =0xFFFFFF;
//	SpiFlash_Read_Data(tab_dat,Block,Page,sizeof(tab_dat));  //获取目录信息
//	
//		for(i =0;i<Flash_Word_Num;i++) // 0x01 密码数量 0x02 动态密码数量  0x11 开门记录  0x12 卡片信息
//		{
//			if(tab_dat[i*7]==0x55&&tab_dat[i*7+1]==0xAA)  //当数据有效时开始提取目录内容
//			{
//				if(tab_dat[i*7+2]==Cat_Type) //对目标目录内容进行提取
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

uint32_t Get_Cat_Dat(uint8_t Cat_Type) //获取目录信息
{
		uint8_t Sec_dat[Cat_Dat_len];  //目录缓存
	
		uint32_t i;
		uint32_t Block,Page;
	//	uint32_t Dat_Sector;		// 有效数据所在扇区 表达形式为 在第几个目录扇区
		uint32_t len_i;
		
		len_i =0;
	
		#ifdef uart_start
	
		SEGGER_RTT_printf(0,"\r\nStart Get_Cat_Dat !\r\n");
		SEGGER_RTT_printf(0,"\r\n len_i: %x!\r\n",len_i);
	
		#endif
	
	
		for(i =0;i<sizeof(Sec_dat);i++) Sec_dat[i] = 0xFF;		// 对目录数据置位
		
		for(i =0;i<Cat_Dat_Sector_Num;i++)		// 对每个扇区进行读取
		{
		
			Sector_to_Block_Page (&Block,&Page,Cat_Dat_Sector1+i);    //目录所在扇区号为 1
			
			SpiFlash_Read_Data(Sec_dat,Block,Page,sizeof(Sec_dat));  //获取目录信息
			
			if((Sec_dat[0]==0X55)&&(Sec_dat[1]==0xAA))   //当找到有效数据扇区时
			{
				
			//	 Dat_Sector =i;
				 
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nCat_Dat_Sector_Num : %d !\r\n",Cat_Dat_Sector1+i);		//返回 获取到的有效扇区号
				
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
					
				for(i =0;i<Flash_Word_Num;i++) // 0x01 密码数量 0x02 动态密码数量  0x11 开门记录  0x12 卡片信息
					{
						if(Sec_dat[i*7]==0x55&&Sec_dat[i*7+1]==0xAA)  //当数据有效时开始提取目录内容
						{
							if(Sec_dat[i*7+2]==Cat_Type) //对目标目录内容进行提取
							{
								len_i = Sec_dat[i*7+3];
								len_i<<=8;
								len_i |= Sec_dat[i*7+4];
								len_i<<=8;
								len_i |= Sec_dat[i*7+5];
								len_i<<=8;
								len_i |=Sec_dat[i*7+6];
								
									#ifdef uart_start
				
									SEGGER_RTT_printf(0,"Cat_Dat_Type: 0x%x .\r\n",Cat_Type);		//返回 目录数据类型
										
									SEGGER_RTT_printf(0,"Cat_Dat_Valye: 0x%x .\r\n",len_i);		//返回 目录数据类型
									
									SEGGER_RTT_printf(0,"Get_Cat_Dat_Ok! .\r\n\r\n");		//返回 目录数据类型									
											
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
对指定目录赋值 (直接赋新的值)
输入：数据类型 Cat_Type 
输入：输入数据 Cat_Dat  只取用低24位 3byte
**********************************************************/

//void Ev_Cat_Dat(uint8_t Cat_Type,uint32_t Cat_Dat)
//{
//	
//	
//	uint8_t tab_dat[256];  //目录缓存
//	uint8_t tab_num[4];
//	uint32_t i;
////	uint32_t len_t;
//	uint32_t Block,Page,Sector;
//	
//	for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0xFF;
//	for(i =0;i<sizeof(tab_num);i++) tab_num[i] =0xFF;
//	
//	tab_num[0] = Cat_Dat>>24&0xFF;
//	tab_num[1] = Cat_Dat>>16&0xFF;  //对数据进行预处理 共取用24bit
//	tab_num[2] = Cat_Dat>>8&0xFF;
//	tab_num[3] = Cat_Dat&0xFF;
//	
//	Sector_to_Block_Page (&Block,&Page,1);			//目录所在扇区号为 1
//	
//	SpiFlash_Read_Data (tab_dat,Block,Page,sizeof(tab_dat));  //获取目录信息
//	
//	for(i =0;i<Flash_Word_Num;i++)   //在5条目录中选中需要修改的目录
//	{
//		if(tab_dat[i*7]==0x55&&tab_dat[i*7+1]==0xAA)    	//当数据有效时开始提取目录内容
//		{
//			if(tab_dat[i*7+2]==Cat_Type)  //对目录内容进行提取
//			{
//				tab_dat[i*7+3] = tab_num[0];		//仅修改对应目录下的数据
//				tab_dat[i*7+4] = tab_num[1];
//				tab_dat[i*7+5] = tab_num[2];
//				tab_dat[i*7+6] = tab_num[3];
//				
//			}
//		}
//	}
//	Sector_to_Block_Sector (&Block,&Sector,1);   //写入前先擦除Flash
//	SPIFlash_Erase_Sector (Block,Sector);
//	SPI_Flash_Busy();
//	
//	Sector_to_Block_Page(&Block,&Page,1);
//	SpiFlash_Write_Data(tab_dat,Block,Page,sizeof(tab_dat));  //将新数据写入SPI_Flash
//	SPI_Flash_Busy();
//	
//}

void Ev_Cat_Dat(uint8_t Cat_Type,uint32_t Cat_Dat)
{
		
		uint8_t Sec_dat[Cat_Dat_len];  //目录缓存
		uint8_t tab_num[4];
		uint8_t tab_Cl[4];
		uint32_t i;
		uint32_t Dat_Sector;
//		uint32_t len_t;
		uint32_t Block,Page,Sector;
		
	for(i =0;i<sizeof(Sec_dat);i++) Sec_dat[i] =0xFF;
	for(i =0;i<sizeof(tab_num);i++) tab_num[i] =0xFF;
	
	for(i =0;i<sizeof(tab_Cl);i++) tab_Cl[i] =0;			//准备用于清空的数据
	
	tab_num[0] = Cat_Dat>>24&0xFF;
	tab_num[1] = Cat_Dat>>16&0xFF;  //对数据进行预处理 共取用24bit
	tab_num[2] = Cat_Dat>>8&0xFF;
	tab_num[3] = Cat_Dat&0xFF;
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart Ev_Cat_Dat !\r\n");
	
	#endif
	
		
		
		for(i =0;i<Cat_Dat_Sector_Num;i++)		// 对每个扇区进行读取
		{
		
			Sector_to_Block_Page (&Block,&Page,Cat_Dat_Sector1+i);    //目录所在扇区号为 1
			
			SpiFlash_Read_Data(Sec_dat,Block,Page,sizeof(Sec_dat));  //获取目录信息
			
			
			if((Sec_dat[0]==0X55)&&(Sec_dat[1]==0xAA))   //当找到有效数据扇区时
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
				
				Dat_Sector = Cat_Dat_Sector1+i;			// 获取对应的扇区号
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Dat_Sector_1: %d .\r\n",Cat_Dat_Sector1);
					
				SEGGER_RTT_printf(0,"Dat_Sector: %d .\r\n",Dat_Sector);
				
				SEGGER_RTT_printf(0,"Write_Cat_Type: 0x%x .\r\n",Cat_Type);
				
				SEGGER_RTT_printf(0,"Write_Cat_Dat: 0x%x .\r\n\r\n",Cat_Dat);
				
				
				#endif
					
				for(i =0;i<Flash_Word_Num;i++)   //在 Flash_Word_Num 条目录中选中需要修改的目录
					{
						if(Sec_dat[i*7]==0x55&&Sec_dat[i*7+1]==0xAA)    	//当数据有效时开始提取目录内容
						{
							if(Sec_dat[i*7+2]==Cat_Type)  //对目录内容进行提取
							{
								
								Sec_dat[i*7+3] = tab_num[0];		//仅修改对应目录下的数据
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
					
					//替换完数据后，先将数据写入 下一个扇区
					//先擦除上一个扇区(清零)
					
							Sector_to_Block_Page(&Block,&Page,Dat_Sector);
							SpiFlash_Write_Data(tab_Cl,Block,Page,sizeof(tab_Cl));  //将空写入SPI_Flash 用于销毁数据
							SPI_Flash_Busy();
					
							if((Dat_Sector-Cat_Dat_Sector1+1)==Cat_Dat_Sector_Num)  //当 扇区 号序列达到上限时，返回第一个扇区
								Dat_Sector = Cat_Dat_Sector1;			//数据来自最后一个扇区时，将新数据写入第一个扇区
							else
								Dat_Sector++;
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Nex_Cat_Dat_Sector: %d .\r\n",Dat_Sector);		//输出下一个要写入的扇区
							
							#endif
							
							Sector_to_Block_Sector (&Block,&Sector,Dat_Sector);   //写入前先擦除Flash
					  	SPIFlash_Erase_Sector (Block,Sector);
					  	SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,Dat_Sector);
							SpiFlash_Write_Data(Sec_dat,Block,Page,sizeof(Sec_dat));  //将新数据写入SPI_Flash 用于保存数据
							SPI_Flash_Busy();
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Cat_Dat_Write_Ok! \r\n\r\n");
							
							#endif
			}
		}
	
	
}





/*********************************************************
对密码数据进行比对，并返回哪一个与预设一致 比对值为 tab_dat 中一部分或全部
进入此函数前，需要先将待对照密码数据写入tab_dat
输入：PW_Num 需要比对的密码总数
输入：PW_W 需要比对的参考密码
输出： 第几个密码与参考密码一致，没有返回 Max_PassWord_Num
*********************************************************/
uint32_t PassWord_Same (uint32_t PW_Num,uint8_t *PW_W)
{
	uint32_t i;
	uint32_t now;  //状态机用
	uint8_t PS_W[PassWord_Len];  //查询密码缓存
	uint32_t PW_MN;  // 对照tab_dat 的最大位置
	uint32_t PW_N; //当前对照位置 也作为对照密码的起点
	uint8_t PWI,PWD; //当前对照数缓存 PWI表示载入的参考密码，PWO表示 取自tab_dat的密码
	uint8_t PWS;  //当前对照一致的数据
	
	for(i =0;i<PassWord_Len;i++) //将参考数据放入缓存
	{
	PS_W[i] = *PW_W;
	PW_W++;
	}
	PW_N =0;
	PW_MN = PW_Num * PassWord_Len;  //载入对照极限
	
	now =0;
	PWS =0;
	while(PW_N<PW_MN) //开始对照
	{
		switch(now)
		{

			case 0:
				for(i =0;i<3;i++)  //先对照前三字节 判断头与密码类型是否一致
				{
					PWI = PS_W[i];
					PWD = tab_dat[PW_N+i];
					if(PWI==PWD) PWS++;
				}
				for(i = 11;i<(PassWord_Len-1);i++) //判断后六位值是否一致(第七位为密码序号)
				{
					PWI = PS_W[i];
					PWD = tab_dat[PW_N+i];
					if(PWI==PWD) PWS++;
				}
				
				now++;
				break;
			case 1:
					if(PWS==(PassWord_Len-9)) //当数据完全一致时(8为两段时间戳的值)加一个SN 1 =9
					return PW_N; //结束这次对照，并返回tab_dat中相同数据的起始位置
					else now++;
					break;
			case 2:  //当一次对比不一致时准备开始下一次比对
					PW_N +=PassWord_Len;  //将PWN跳到下一个数据起始位置
					PWS =0;
					now =0;
					break;
		}
	}
	return PW_MN; //返回数据队列最大值，表示没有相同密码在tab_dat中
}



/*******************************************************
往密码扇区载入或更新一个密码数据
输入： PassWord  载入密码数据的指针
输出： 密码的编号
******************************************************/
uint8_t Add_Flash_PassWord(uint8_t *PassWord)  //写入一个密码至Flash 区域 密码格式： 6byte 密码数据+ 生效时间+失效时间
{
	static uint8_t PassWord_W[PassWord_Len];
	uint8_t tab1[2*A_Sector_PassWord_Num];
	uint32_t i,it,i1,i2,i3; 
	uint32_t len; //密码数据当前总长度
	uint8_t PassWord_Num;  //密码数量
	uint32_t Block,Sector,Page;  //定义写入/读取 扇区参数
	uint32_t PW_Same;  //相同密码所在位置
	uint8_t  *PassWord_R; //读取载入密码指针
	uint8_t PassWord_SN;  //表示密码编号
	//uint8_t PassWord_Secotr_Num;  //设置要修改的扇区号，0表示无，1表示密码扇区1,2表示密码扇区2
	
//Spi_Flash_Init();  //开启SPI_FLASH 通讯
	
//	PassWord_Secotr_Num = 0;  //先清空密码扇区
	
	for (i =0;i<sizeof(tab1);i++)
	{
		
		tab1[i] =0x00;
	}
	
	PassWord_Num = Get_Cat_Dat(0x01); //获取当前存储的密码数量
	len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度

	if(PassWord_Num>0) //当有存储的数据时
	{
		Sector_to_Block_Page (&Block,&Page,PassWord_SN_Secotr); //获取密码扇区数据
		SPIFlash_Read_Tab (tab1,Block,Page,PassWord_Num);  //获取与密码数据长度等量的数据	
	}
	
	PassWord_W[0] = 0x55;
	PassWord_W[1] = 0xAA;  //载入数据头
	
	for(i =0;i<(PassWord_Len-2);i++)  //跳过报文头的两字节
	{
		PassWord_R = PassWord+i;
		PassWord_W[i+2] = *PassWord_R;
	}
	
	if(PassWord_Num<=A_Sector_PassWord_Num) //当密码数据总数小于一个扇区容量时
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取第一密码扇区信息
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //获取与密码数据长度等量的数据	
		PW_Same = PassWord_Same (PassWord_Num,PassWord_W); //获取数据对比结果
		it =0; //用于记录修改了多少byte数据
		if(PW_Same<len) //当转换值在有效数据范围内（有效） 更新时间后写入 (对密码扇区1的操作，之后既结束操作)
		{
			for(i =0;i<(PassWord_Len-1);i++) //不修改最后的序号
			{
				if(tab_dat[PW_Same+i]!=PassWord_W[i])
				{
					tab_dat[PW_Same+i] = PassWord_W[i]; //直接对整条数据更新(密码区一致)
					it++;
				}
			}
			PassWord_SN = tab_dat[PW_Same+i]; //此时i已经加过1，既指向最后一位密码序号
			if(it>0) //数据有发生变化时写入数据到Flash
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //获取扇区数据
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);  //获取扇区数据
				SPIFlash_Write_Tab(tab_dat,Block,Page,len);					//新的写入数据 目录不发生改变
				SPI_Flash_Busy ();
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Pass_Word_TimeWrite!\r\n"); 
				#endif
			}
			return PassWord_SN;
		}
		else //无效 即无一致的数据	添加一条新的数据，此时，PW_Same可作为密码扇区一写入的起始地址
		{
			if(PassWord_Num<A_Sector_PassWord_Num) //当密码扇区1内有剩余空间时，直接在扇区1加入新密码
			{
				for(i =0;i<(PassWord_Len-1);i++)  //添加一条密码数据到 tab_dat中
				{
						tab_dat[PW_Same+i] = PassWord_W[i];
				}
				PassWord_SN =0;
				i3 =0;
				if(PassWord_Num==0)
				{	
				PassWord_SN =1; //当密码数为0时
				tab1[0] =1;
				}
				else
				{
					for(i1 =1;i1<(2*A_Sector_PassWord_Num+1);i1++)
					{
						for(i2 =0;i2<PassWord_Num;i2++)
						{
							if(tab1[i2]==i1) //当序列号存在时
							{
								i3 =1;
							}
						}
						if(i3!=1)
						{
							PassWord_SN = i1;  //使用这个密码号
							tab1[PassWord_Num] = PassWord_SN;
							break;
						}
						else
							i3 =0;
					}
				}
				
//				i3 =0;
//				for(i1 =1;i1<(A_Sector_PassWord_Num+2);i1++) //寻找未被占用的序号 密码号大于1，小于密码扇区1所能容纳的数量
//				{
//					for(i2 =0;i2<PassWord_Num;i2++)  // 在密码数量内轮询
//					{
//						if((tab_dat[i2*PassWord_Len+PassWord_Len-1])==i1) //当序列号存在时
//						{
//							i3 =1;
//						}
//					}
//					if(i3!=1) //当密码号不存在时
//					{
//						PassWord_SN = i1;//使用这个密码号，并结束循环
//						break; 
//					}
//					else
//					{
//						i3 =0; //存在时清空密码存在标志位准备下次轮询
//					}
//				}
				
				tab_dat[PW_Same+i] = PassWord_SN;
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_SN:%d\r\n",PassWord_SN); //输出密码序号
				#endif
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //擦除扇区 PassWord_Secotr1
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //在扇区中写入数据
				SPIFlash_Write_Tab(tab_dat,Block,Page,len+PassWord_Len);
				SPI_Flash_Busy();			

				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //擦除扇区 PassWord_SN_Secotr
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //在扇区中写入数据
				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(0X01,PassWord_Num+1);  //更新目录表
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Write_a_PassWord!\r\n");
				#endif
				return PassWord_SN;  //返回写入的密码序号
			}
			else if(PassWord_Num==A_Sector_PassWord_Num) //当密码数量等于一个密码扇区容量时(向下一个扇区写入密码)此时密码扇区2写入第一个数据
			{
				//此时tab_dat数据清空(不修改密码扇区1内容)
				
				for(i =0;i<sizeof(tab_dat);i++) 
				tab_dat[i] =0xff; //清空缓存
				
				for(i =0;i<(PassWord_Len-1);i++)  //添加一条密码数据到 tab_dat中
					{
							tab_dat[i] = PassWord_W[i]; //写第一条数据，直接写入
					}
					
				PassWord_SN =0;
				if(PassWord_Num==0)PassWord_SN =1; //当密码数为0时
				else
				{
					for(i1 =1;i1<(2*A_Sector_PassWord_Num+1);i1++)
					{
						for(i2 =0;i2<PassWord_Num;i2++)
						{
							if(tab1[i2]==i1) //当序列号存在时
							{
								i3 =1;
							}
						}
						if(i3!=1)
						{
							PassWord_SN = i1;  //使用这个密码号
							tab1[PassWord_Num] = PassWord_SN;
							break;
						}
						else i3 =0;
					}
				}
					
					tab_dat[i] = PassWord_SN;  //直接使用新扇区编号赋值密码扇区2第一个密码
					PassWord_SN = tab_dat[i];
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_SN:%d\r\n",PassWord_SN); //输出密码序号
				#endif
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //擦除扇区 PassWord_Secotr2
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //在密码扇区2中写入数据
				SPIFlash_Write_Tab(tab_dat,Block,Page,PassWord_Len);
				SPI_Flash_Busy();
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //擦除扇区 PassWord_SN_Secotr
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //在扇区中写入数据
				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(0X01,PassWord_Num+1);  //更新目录表
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Write_a_PassWord!\r\n");
				#endif
				return PassWord_SN;  //返回写入的密码序号
			}
			else  //密码数量数据异常
			{
				#ifdef uart_start
				printf("PassWord_Num_Out!\r\n");
				#endif
				return 0;
			}
		}
	}
	else if((PassWord_Num>A_Sector_PassWord_Num)&&(PassWord_Num<=(2*A_Sector_PassWord_Num))) //当写入数据在大于第一个扇区和第二个扇区饱和之间
	{
		for(i =0;i<sizeof(tab_dat);i++) //开始判断第一密码区
		tab_dat[i] =0xff; //清空缓存

		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取第一密码扇区信息
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //获取与密码数据长度等量的数据(此时1扇区饱和,全部截取)
		PW_Same = PassWord_Same (A_Sector_PassWord_Num,PassWord_W); //获取第一密码扇区数据对比结果 //整个扇区饱和既对照数量为A_Sector_PassWord_Num
		if(PW_Same<(A_Sector_PassWord_Num*PassWord_Len)) //判断第一扇区是否有相同密码
		{
		//	PassWord_Secotr_Num=1; //表示第一扇区有相同密码 //直接替换更新信息即可
			it =0; //用于记录修改了多少byte数据
			for(i =0;i<(PassWord_Len-1);i++) //不修改最后的序号
			{
				if(tab_dat[PW_Same+i]!=PassWord_W[i])
				{
					tab_dat[PW_Same+i] = PassWord_W[i]; //直接对整条数据更新(密码区一致)
					it++;
				}
			}
			PassWord_SN = tab_dat[PW_Same+i]; //此时i已经加过1，既指向最后一位密码序号
			if(it>0) //数据有发生变化时写入数据到Flash
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //获取扇区数据
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);  //获取扇区数据
				SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);					//新的写入数据 目录不发生改变
				SPI_Flash_Busy ();
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Pass_Word_TimeWrite!\r\n"); 
				#endif
			}
			return PassWord_SN;
		}
		else if (PW_Same==(A_Sector_PassWord_Num*PassWord_Len)) //当未在第一密码扇区找到相同密码时 搜索第二扇区 且新增密码必然写入第二扇区
		{
		for(i =0;i<sizeof(tab_dat);i++) //开始判断第二密码区
		tab_dat[i] =0xff; //清空缓存
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取第二密码扇区信息
		SPIFlash_Read_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len);  //获取与密码数据长度等量的数据(此时1扇区饱和,全部截取)
		PW_Same = PassWord_Same (PassWord_Num-A_Sector_PassWord_Num,PassWord_W); //获取第二密码扇区数据对比结果 PassWord_Num-A_Sector_PassWord_Num 为剩余需要对照的密码信息
		
		if(PW_Same<(len-A_Sector_PassWord_Num*PassWord_Len)) //判断第二扇区是否有相同密码
			{
			//	PassWord_Secotr_Num=2;  //表示第二扇区有相同密码//直接替换更新信息	
				it =0; //用于记录修改了多少byte数据
				for(i =0;i<(PassWord_Len-1);i++) //不修改最后的序号
				{
					if(tab_dat[PW_Same+i]!=PassWord_W[i])
					{
						tab_dat[PW_Same+i] = PassWord_W[i]; //直接对整条数据更新(密码区一致)
						it++;
					}
				}
				
				PassWord_SN = tab_dat[PW_Same+i]; //此时i已经加过1，既指向最后一位密码序号
				if(it>0) //数据有发生变化时写入数据到Flash
				{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //获取扇区数据
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);  //获取扇区数据
						SPIFlash_Write_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len); //新的写入数据 目录不发生改变
						SPI_Flash_Busy ();
						#ifdef uart_start
						SEGGER_RTT_printf(0,"Pass_Word_TimeWrite!\r\n"); 
						#endif
				}
				return PassWord_SN;
			}
			else if((PassWord_Num<(2*A_Sector_PassWord_Num))&&(PW_Same==(len-A_Sector_PassWord_Num*PassWord_Len)))  //当未在第二扇区发现相同密码且还有剩余空间时，直接在第二扇区后写入一条新信息
			{
				
				for (i =0;i<(PassWord_Len-1);i++)  //添加一条密码数据到 tab_dat中
				{
					tab_dat[PW_Same+i] = PassWord_W[i];
				}
				
				PassWord_SN =0;
				i3 =0;
				if(PassWord_Num==0)
				{
					PassWord_SN =1; //当密码数为0时
				}
				else
				{
					for(i1 =1;i1<(2*A_Sector_PassWord_Num+1);i1++)
					{
						for(i2 =0;i2<PassWord_Num;i2++)
						{
							if(tab1[i2]==i1) //当序列号存在时
							{
								i3 =1;
							}
						}
						if(i3!=1)
						{
							PassWord_SN = i1;  //使用这个密码号
							tab1[PassWord_Num] = PassWord_SN;
							break;
						}
						else i3 =0;
					}
				}
				
//				i3 =0;
//				for(i1 =A_Sector_PassWord_Num+1;i1<(A_Sector_PassWord_Num*2+2);i1++) //寻找未被占用的序号 密码号大于1，小于密码扇区1所能容纳的数量
//				{
//					for(i2 =0;i2<(PassWord_Num-A_Sector_PassWord_Num);i2++)  // 在密码数量内轮询
//					{
//						if((tab_dat[i2*PassWord_Len+PassWord_Len-1])==i1) //当序列号存在时
//						{
//							i3 =1;
//						}
//					}
//					if(i3!=1) //当密码号不存在时
//					{
//						PassWord_SN = i1;//使用这个密码号，并结束循环
//						break; 
//					}
//					else
//					{
//						i3 =0; //存在时清空密码存在标志位准备下次轮询
//					}
//				}
				
				tab_dat[PW_Same+i] = PassWord_SN;
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_SN:%d\r\n",PassWord_SN); //输出密码序号
				#endif
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //擦除扇区 PassWord_Secotr2
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //在密码扇区2中写入数据
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len+PassWord_Len);
				SPI_Flash_Busy();
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //擦除扇区 PassWord_SN_Secotr
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //在扇区中写入数据
				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
				SPI_Flash_Busy();
				
				
				Ev_Cat_Dat(0X01,PassWord_Num+1);  //更新目录表
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"Write_a_PassWord!\r\n");
				#endif
				return PassWord_SN;  //返回写入的密码序号
			}
			else
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_Secotr_Full!\r\n"); //提示已写满密码
				#endif
				return 0;
			}
		}
	}
	return 0;
}

/*************************************************************************************
删除一个密码扇区里的密码数据
输入： PassWord  需要删除的密码
*************************************************************************************/
void Del_Flash_PassWord(uint8_t *PassWord)
{
	uint8_t PassWord_W[PassWord_Len];
	uint32_t i;
	uint32_t len;  					//数据缓存最大区域
	uint32_t Block,Sector,Page;
	uint32_t PW_Same;  //相同密码所在位置
	uint8_t *PassWord_R;
	uint32_t PassWord_Num;
	
	
	PassWord_Num = Get_Cat_Dat(0x01); //获取当前存储的密码数量
	len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度
	PassWord_W[0] = 0x55;
	PassWord_W[1] = 0xAA;  //载入数据头
	for(i =0;i<(PassWord_Len-2);i++)  //跳过报文头的两字节载入待删除密码用于比对
	{
		PassWord_R = PassWord+i;
		PassWord_W[i+2] = *PassWord_R;
	}
	
	if(len<=(A_Sector_PassWord_Num*PassWord_Len)) //当数据总长度在一个扇区内时
	{
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取扇区数据
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //获取与密码数据长度等量的数据
			
			
			PW_Same = PassWord_Same (PassWord_Num,PassWord_W); //获取数据对比结果
			
			
			if(PW_Same<len) //找到需要删除的密码 PW_Same 作为替换起始地址 len作为终止地址
			{
				for(i =PW_Same;i<len;i++)  //将数据往左移动一个单位
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
	else if((len>A_Sector_PassWord_Num*PassWord_Len)&&(len<=(2*A_Sector_PassWord_Num*PassWord_Len)))//当密码长度大于一个扇区容量，且在两个扇区容量内
	{
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取扇区数据
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //获取与一个密码数据长度等量的数据
			
			PW_Same = PassWord_Same (A_Sector_PassWord_Num,PassWord_W); //获取数据对比结果（整个1扇区）
			
			if(PW_Same<(A_Sector_PassWord_Num*PassWord_Len)) //找到需要删除的密码 PW_Same 作为替换起始地址 len作为终止地址
			{
				
				for(i =PW_Same;i<len;i++)  //将数据往左移动一个单位
				{
					tab_dat[i] =tab_dat[i+PassWord_Len];
				}
				
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
				SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len); //写入第一扇区内容
				Ev_Cat_Dat(0x01,PassWord_Num-1);
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PassWord_Clear!\r\n");
				#endif
			}
			else if(PW_Same==A_Sector_PassWord_Num*PassWord_Len) //第一个扇区没有内容时尝试获取第二个扇区
			{
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
					SPIFlash_Read_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len);  //获取与一个密码数据长度等量的数据
					PW_Same = PassWord_Same (PassWord_Num-A_Sector_PassWord_Num,PassWord_W); //获取数据对比结果
				  if(PW_Same<(len-A_Sector_PassWord_Num*PassWord_Len))
					{
						for(i =PW_Same;i<len;i++)  //将数据往左移动一个单位
						{
							tab_dat[i] =tab_dat[i+PassWord_Len];
						}
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);
						SPIFlash_Write_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len); //写入第二扇区内容
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
读取整个扇区的内容，并打印输出
输入： 要输出打印的扇区 每个扇区使用2k
不可在已声明2k缓存的子函数中调用，否则将导致系统错误
************************************************************************************************/
void read_Soctor(uint32_t sector)
{
		#ifdef uart_start
		uint32_t Block,Page;
		uint32_t i;
		for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0x00; //用于读取缓存
		Sector_to_Block_Page( &Block,&Page,sector );  //获取扇区所在块和页首地址
		
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
定向删除管理员密码

***********************************************************************************************/
void Del_Admin_PassWord()
{
	uint32_t i,i1;
	uint16_t Admin_PassWord_Num;  //获取到的管理员密码数
	uint32_t len,len1;  					//数据缓存最大区域
	uint32_t Block,Sector,Page;
//	uint32_t PW_Same;  //相同密码所在位置
//	uint8_t *PassWord_R;
	uint32_t PassWord_Num;
	uint8_t tab1[2*A_Sector_PassWord_Num+1];
	Admin_PassWord_Num =0;
	PassWord_Num = Get_Cat_Dat(0x01); //获取当前存储的密码数量
	len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度
	uint8_t Sector1_Num;
	Sector1_Num =0;
	uint8_t *s;
	Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //获取扇区数据
	SPIFlash_Read_Tab(tab1,Block,Page,sizeof(tab1));				  //获取与密码数据长度等量的数据
	
		if(PassWord_Num<=A_Sector_PassWord_Num)
		{
			len1 =len;  //密码长度小于1字节
		}
		else if((PassWord_Num>A_Sector_PassWord_Num)&&(PassWord_Num<=(2*A_Sector_PassWord_Num)))
		{
			len1 = A_Sector_PassWord_Num*PassWord_Len;
		}
		else 
		return; //否则结束运行
		
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取第一密码扇区数据
		SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //获取与密码数据长度等量的数据
		
		for(i =0;i<len1;)
		{
			if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
				&&(tab_dat[i+2]==Admin_PassWord)) //获取到管理员密码后
				{
					Admin_PassWord_Num++;  //删除的管理员密码数量加一
					for(i1 =i;i1<len1;i1++)  //数据左移一格，覆盖当前位置
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //继续判断当前数据是否为管理员密码
					for(i1 =i/PassWord_Len;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //前移PassWord_SN;
					}
				}
			else  //当前数据不是管理员密码时，跳过一个密码长度
				i+=PassWord_Len;
		}
		
		//清除一扇区指定密码后
		
		if(Admin_PassWord_Num>0)  //当搜索到一扇区管理员密码发生变动时
		{
			Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //获取密码扇区位置
			SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
			SPI_Flash_Busy();
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //获取密码扇区位置
			//len = len-(PassWord_Len*Admin_PassWord_Num);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //写入字符表 更新扇区1
		}
		
		Sector1_Num = Admin_PassWord_Num; //保留扇区清除的密码数量
		Admin_PassWord_Num =0;

		if(PassWord_Num>A_Sector_PassWord_Num) //当第二扇区有数据时
		{
			len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //获取与密码数据长度等量的数据
			for(i =0;i<len1;)
			{
				if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
					&&(tab_dat[i+2]==Admin_PassWord))			//获取到管理员密码后
				{
					Admin_PassWord_Num++;  //删除的管理员密码数量加一
					for(i1 =i;i1<len1;i1++)  //数据左移一格，覆盖当前位置
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //继续判断当前数据是否为管理员密码
					for(i1 =i/PassWord_Len+A_Sector_PassWord_Num;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //前移PassWord_SN;
					}
				}
				else  //当前数据不是管理员密码时，跳过一个密码长度
					i+=PassWord_Len;
			}
			
			if(Admin_PassWord_Num>0)  //当扇区2有数据修改时重新写入扇区2
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //获取密码扇区位置
				SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //获取密码扇区位置
				//len = len-(PassWord_Len*Admin_PassWord_Num);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //写入字符表
			}
		
			if(Sector1_Num>0) //此时Admin_PassWord_Num 为第二扇区的指定密码 当第一扇区有修改时
				{
					for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0xFF;
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取第一密码扇区数据
					SPIFlash_Read_Tab(tab_dat,Block,Page,(A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);				  //获取与密码数据长度等量的数据
					
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
					
					s = &tab_dat[0];
					s+= (A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len;
					
					SPIFlash_Read_Tab(s,Block,Page,Sector1_Num*PassWord_Len);				  //获取与密码数据长度等量的数据
					
					Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //获取第一密码扇区数据
					SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
					SPI_Flash_Busy();
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //获取密码扇区位置
					//len = len-(PassWord_Len*Admin_PassWord_Num);
					SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //写入1扇区字符表
					
//					read_Soctor(PassWord_Secotr1);
//					NRF_WDT_Feed();
					i1 =0;
					if((A_Sector_PassWord_Num+Sector1_Num)<PassWord_Num) //当第二扇区还有冗余数据
					{
						len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
						SPIFlash_Read_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Admin_PassWord_Num)*PassWord_Len);				  //获取与密码数据长度等量的数据
						for(i =(Sector1_Num*PassWord_Len);i<((PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len);i++)
						{
							tab_dat[i1++] = tab_dat[i]; //前移数据
						}
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //开始写入数据到扇区2
						SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //获取密码扇区位置
						//len = len-(PassWord_Len*Admin_PassWord_Num);
						SPIFlash_Write_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);  //写入字符表 (此时i为有效数据长度
//					read_Soctor(PassWord_Secotr2);
//					NRF_WDT_Feed();
					}
					else //否则直接擦除第二扇区数据
					{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //开始写入数据到扇区2
						SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
						SPI_Flash_Busy();
					}
				}
		}
		
		if((Admin_PassWord_Num>0)||(Sector1_Num>0))
		{
		//完成修改后，写入tab1数据
		Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr);   //获取密码扇区位置
		SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
		SPI_Flash_Busy();
		//len = len-(PassWord_Len*Admin_PassWord_Num);
								
		Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr);   //获取密码扇区位置
		SPIFlash_Write_Tab(tab1,Block,Page,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));  //写入字符表 更新密码编号表
		SPI_Flash_Busy();
				
//		read_Soctor(4);
//		NRF_WDT_Feed();
		
		Ev_Cat_Dat(0x01,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));
		
		}
		Admin_PassWord_Num+=Sector1_Num;
		#ifdef uart_start
		SEGGER_RTT_printf(0,"CL_Admin_Num: %d!\r\n",Admin_PassWord_Num);
		#endif
		
		Del_A_Type_PS_Print(0X01);			//删除管理员指纹
}

/********************************************************************************************
定向删除用户密码

*********************************************************************************************/
void Del_User_PassWord()
{
	uint32_t i,i1;
	uint16_t Admin_PassWord_Num;  //获取到的管理员密码数
	uint32_t len,len1;  					//数据缓存最大区域
	uint32_t Block,Sector,Page;
//	uint32_t PW_Same;  //相同密码所在位置
//	uint8_t *PassWord_R;
	uint32_t PassWord_Num;
	uint8_t tab1[2*A_Sector_PassWord_Num+1];
	Admin_PassWord_Num =0;
	PassWord_Num = Get_Cat_Dat(0x01); //获取当前存储的密码数量
	len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度
	uint8_t Sector1_Num;
	Sector1_Num =0;
	uint8_t *s;
	Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //获取扇区数据
	SPIFlash_Read_Tab(tab1,Block,Page,sizeof(tab1));				  //获取与密码数据长度等量的数据
	
		if(PassWord_Num<=A_Sector_PassWord_Num)
		{
			len1 =len;  //密码长度小于1字节
		}
		else if((PassWord_Num>A_Sector_PassWord_Num)&&(PassWord_Num<=(2*A_Sector_PassWord_Num)))
		{
			len1 = A_Sector_PassWord_Num*PassWord_Len;
		}
		else 
		return; //否则结束运行
		
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取第一密码扇区数据
		SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //获取与密码数据长度等量的数据
		
		for(i =0;i<len1;)
		{
			if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
				&&(tab_dat[i+2]==User_PassWord)) //获取到管理员密码后
				{
					Admin_PassWord_Num++;  //删除的管理员密码数量加一
					for(i1 =i;i1<len1;i1++)  //数据左移一格，覆盖当前位置
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //继续判断当前数据是否为管理员密码
					for(i1 =i/PassWord_Len;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //前移PassWord_SN;
					}
				}
			else  //当前数据不是管理员密码时，跳过一个密码长度
				i+=PassWord_Len;
		}
		
		//清除一扇区指定密码后
		
		if(Admin_PassWord_Num>0)  //当搜索到一扇区管理员密码发生变动时
		{
			Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //获取密码扇区位置
			SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
			SPI_Flash_Busy();
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //获取密码扇区位置
			//len = len-(PassWord_Len*Admin_PassWord_Num);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //写入字符表 更新扇区1
		}
		
		Sector1_Num = Admin_PassWord_Num; //保留扇区清除的密码数量
		Admin_PassWord_Num =0;

		if(PassWord_Num>A_Sector_PassWord_Num) //当第二扇区有数据时
		{
			len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
			Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);				  //获取与密码数据长度等量的数据
			for(i =0;i<len1;)
			{
				if((tab_dat[i]==0x55)&&(tab_dat[i+1]==0xAA)
					&&(tab_dat[i+2]==User_PassWord))			//获取到管理员密码后
				{
					Admin_PassWord_Num++;  //删除的管理员密码数量加一
					for(i1 =i;i1<len1;i1++)  //数据左移一格，覆盖当前位置
					{
						tab_dat[i1] = tab_dat[i1+PassWord_Len];
					} //继续判断当前数据是否为管理员密码
					for(i1 =i/PassWord_Len+A_Sector_PassWord_Num;i1<sizeof(tab1);i1++)
					{
						tab1[i1] = tab1[i1+1]; //前移PassWord_SN;
					}
				}
				else  //当前数据不是管理员密码时，跳过一个密码长度
					i+=PassWord_Len;
			}
			
			if(Admin_PassWord_Num>0)  //当扇区2有数据修改时重新写入扇区2
			{
				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //获取密码扇区位置
				SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
				SPI_Flash_Busy();
				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //获取密码扇区位置
				//len = len-(PassWord_Len*Admin_PassWord_Num);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len1-(Admin_PassWord_Num*PassWord_Len));  //写入字符表
			}
		
			if(Sector1_Num>0) //此时Admin_PassWord_Num 为第二扇区的指定密码 当第一扇区有修改时
				{
					for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0xFF;
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取第一密码扇区数据
					SPIFlash_Read_Tab(tab_dat,Block,Page,(A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);				  //获取与密码数据长度等量的数据
					
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
					
					s = &tab_dat[0];
					s+= (A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len;
					
					SPIFlash_Read_Tab(s,Block,Page,Sector1_Num*PassWord_Len);				  //获取与密码数据长度等量的数据
					
					Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //获取第一密码扇区数据
					SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
					SPI_Flash_Busy();
					Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //获取密码扇区位置
					//len = len-(PassWord_Len*Admin_PassWord_Num);
					SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //写入1扇区字符表
					
//					read_Soctor(PassWord_Secotr1);
//					NRF_WDT_Feed();
					i1 =0;
					if((A_Sector_PassWord_Num+Sector1_Num)<PassWord_Num) //当第二扇区还有冗余数据
					{
						len1 = (PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len;
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
						SPIFlash_Read_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Admin_PassWord_Num)*PassWord_Len);				  //获取与密码数据长度等量的数据
						for(i =(Sector1_Num*PassWord_Len);i<((PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len);i++)
						{
							tab_dat[i1++] = tab_dat[i]; //前移数据
						}
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //开始写入数据到扇区2
						SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //获取密码扇区位置
						//len = len-(PassWord_Len*Admin_PassWord_Num);
						SPIFlash_Write_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num-Sector1_Num)*PassWord_Len);  //写入字符表 (此时i为有效数据长度
//					read_Soctor(PassWord_Secotr2);
//					NRF_WDT_Feed();
					}
					else //否则直接擦除第二扇区数据
					{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2); //开始写入数据到扇区2
						SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
						SPI_Flash_Busy();
					}
				}
		}
		
		if((Admin_PassWord_Num>0)||(Sector1_Num>0))
		{
		//完成修改后，写入tab1数据
		Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr);   //获取密码扇区位置
		SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
		SPI_Flash_Busy();
		//len = len-(PassWord_Len*Admin_PassWord_Num);
								
		Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr);   //获取密码扇区位置
		SPIFlash_Write_Tab(tab1,Block,Page,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));  //写入字符表 更新密码编号表
		SPI_Flash_Busy();
				
//		read_Soctor(4);
//		NRF_WDT_Feed();
		
		Ev_Cat_Dat(0x01,(PassWord_Num-(Admin_PassWord_Num+Sector1_Num)));
		
		}
		Admin_PassWord_Num+=Sector1_Num;
		#ifdef uart_start
		SEGGER_RTT_printf(0,"CL_User_Num: %d!\r\n",Admin_PassWord_Num);
		#endif
		
		Del_A_Type_PS_Print(0X02);			//删除用户指纹
		
	//	Ev_Cat_Dat(0x04,0);						//删除永久动态密码

}


//				Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //擦除扇区 PassWord_Secotr1
//				SPIFlash_Erase_Sector(Block,Sector);
//				SPI_Flash_Busy();
//				
//				
//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //在扇区中写入数据
//				SPIFlash_Write_Tab(tab_dat,Block,Page,len+PassWord_Len);
//				SPI_Flash_Busy();			

//				Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //擦除扇区 PassWord_SN_Secotr
//				SPIFlash_Erase_Sector(Block,Sector);
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //在扇区中写入数据
//				SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num+1);
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(0X01,PassWord_Num+1);  //更新目录表
//				


/*************************************************************
删除一个密码
输入： 密码编号
返回： 无
**************************************************************/
//uint8_t Del_A_PassWord(uint8_t PassWord_SN)
//{
////	uint32_t len;  					//数据缓存最大区域
////	uint32_t Block,Sector,Page;
////	uint32_t PW_Same;  //相同密码所在位置
////	uint8_t *PassWord_R;
////	uint32_t PassWord_Num;
////	uint8_t i;
////	uint32_t it,k;
////	
////	PassWord_Num = Get_Cat_Dat(0x01); //获取当前存储的密码数量
////	len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度
////	
////		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取扇区数据
////		SPIFlash_Read_Tab(tab_dat,Block,Page,len);				  //获取与密码数据长度等量的数据

////	for(i =0;i<PassWord_Num;i++)
////	{
////		if(tab_dat[i*PassWord_Len+PassWord_Len-1]==PassWord_SN)
////		{
////			break;  //找到对应密码
////		}
////	} //开始清除找到的密码
////	if(i<PassWord_Len) //找到的密码有效
////	{
////		for(it =i*PassWord_Len;it<len;it++) //替换数据
////		{
////			tab_dat[it] = tab_dat[it+PassWord_Len];
////		}		
////		
////		#ifdef uart_start
////		SEGGER_RTT_printf(0,"PassWord_N: %d!\r\n",i);	//找到的密码号
////		#endif
////		
////	Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);  //获取密码扇区位置
////	SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
////	SPI_Flash_Busy();
////	Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);   //获取密码扇区位置
////		len = len-PassWord_Len;  //减去一个密码
////	SPIFlash_Write_Tab(tab_dat,Block,Page,len);  //写入字符表
////	Ev_Cat_Dat(0x01,PassWord_Num-1);  //目录值减一
////		return 1;
////	}
////	return 0;

//}


/********************************
根据SN号删除一个密码

输入： 需要删除密码的SN号
输出： 是否成功删除密码

********************************/
uint8_t Del_SN_PassWord(uint8_t PW_SN)
{
	static uint8_t PassWord_W[PassWord_Len];
	uint8_t tab1[2*A_Sector_PassWord_Num];
	uint32_t i,it,i1,i2,i3;
	uint32_t len1; //密码数据当前总长度
	uint8_t PassWord_Num;  //密码数量
	uint32_t Block,Sector,Page;  //定义写入/读取 扇区参数
//	uint32_t PW_Same;  //相同密码所在位置
//	uint8_t  *PassWord_R; //读取载入密码指针
//	uint8_t PassWord_SN;  //表示密码编号
	uint8_t PassWord_Secotr_Num;  //设置要修改的扇区号，0表示无，1表示密码扇区1,2表示密码扇区2
	
  PassWord_Secotr_Num = 0;  //先清空密码扇区指向
	i3 =0;
	for(i =0;i<sizeof(tab1);i++)
	{
		
		tab1[i] =0x00; 
		
	}
	
	for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0;
	
	PassWord_Num = Get_Cat_Dat(0x01); //获取当前存储的密码数量
//	len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度
	
	if(PassWord_Num>0) //当有存储的数据时
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //获取密码编号扇区数据
		SPIFlash_Read_Tab(tab1,Block,Page,PassWord_Num);  //获取与密码数据数量等量的数据	
	}
	else return 0;  //没有密码时直接返回操作失败
	
	for(i =0;i<PassWord_Num;i++)
	{
		if(tab1[i]==PW_SN)//判断对应值是否和载入的一样
		{
			break;  //结束循环
		}
	}
	
	#ifdef uart_start
	SEGGER_RTT_printf(0,"\r\ni: %d!\r\n",i);	//找到的密码号
	#endif
	//i为密码所在的区域
	if(i>=PassWord_Num)
	{
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Not_PassWord!\r\n");
		#endif
		return 0;  //SN码无效则返回 0
	}
	if(i<(2*A_Sector_PassWord_Num)) //当密码在整个密码区内
	{
		if(i<A_Sector_PassWord_Num)  //在第一扇区
		{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);
		PassWord_Secotr_Num =1;
		i3 =i;
		}
		else  //在第二扇区
		{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);
			PassWord_Secotr_Num =2; //指明扇区
		i3 =i-A_Sector_PassWord_Num;
		}
		//i*=PassWord_Len;  //计算对应位置
//		if(i<A_Sector_PassWord_Num)  // 确定扇区位置后，换算扇区内位置
//			i3 =i;
//		else if((i>=A_Sector_PassWord_Num)&&(i<(2*A_Sector_PassWord_Num)))//第二扇区内
//			//
//			i3 =i-A_Sector_PassWord_Num;
//		else
//			return 0;
//		
//		if(i3<A_Sector_PassWord_Num); //判断数据有效性
//		else return 0;
		
		if(tab_dat[i3*PassWord_Len+17]==tab1[i])  //修正100个值 (值域0-99)
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"PassWord_SN_Cheak ok!\r\n");	//找到的密码号
			it = tab1[i];
			SEGGER_RTT_printf(0,"PassWord:%d\r\n",it);	//找到的密码号
			
			#endif

				if((PassWord_Num<=2*A_Sector_PassWord_Num)&&(PassWord_Secotr_Num==1)) //当值小于整块扇区时 (2*A_Sector_PassWord_Num) 且数据在一扇区内
				{
	//				len1 =len;
	//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //读取扇区1
	//				SPIFlash_Read_Tab(tab_dat,Block,Page,len);
					len1 =A_Sector_PassWord_Num*PassWord_Len;
				}
				else if((PassWord_Num<=2*A_Sector_PassWord_Num)&&(PassWord_Secotr_Num==2)) //当值小于整块扇区时 (2*A_Sector_PassWord_Num) 且数据在二扇区内
				{
	//				len1 = A_Sector_PassWord_Num*PassWord_Len;//反之值为整个扇区大小
	//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //读取扇区2
	//				SPIFlash_Read_Tab(tab_dat,Block,Page,len-(A_Sector_PassWord_Num*PassWord_Len));
					len1 = A_Sector_PassWord_Num*PassWord_Len;  //len1值为第二扇区内密码总长度
				}
				
				for(it =i3*PassWord_Len;it<len1;it++)  //前移密码扇区
				{
					tab_dat[it]	=tab_dat[it+PassWord_Len];//替换这个位置
				}
				
				for(it =i;it<(PassWord_Num-1);it++) //后移密码扇区
				{
					tab1[it]=tab1[it+1];
				}
				
				if(PassWord_Secotr_Num==1) //改动一扇区
				{
					if(PassWord_Num>A_Sector_PassWord_Num) //当密码数量大于1个扇区内容时 截取后面扇区第一个数据前移
					{
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //读取扇区2
						SPIFlash_Read_Tab(PassWord_W,Block,Page,PassWord_Len); //将扇区2第一个数据内容提取到PassWord_W位置
						if((PassWord_W[0]==0x55)&&(PassWord_W[1]==0xAA))  //当获取到数据时
						{
							i2 =0;
								for(i1 =(A_Sector_PassWord_Num-1)*PassWord_Len;i1<(A_Sector_PassWord_Num*PassWord_Len);i1++) //在最后一个区域填补数据
								{
									tab_dat[i1] =PassWord_W[i2++];  //将数据写入tab_dat
								}
								Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1);
								SPIFlash_Erase_Sector(Block,Sector);
								SPI_Flash_Busy();
								Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
								SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len); //写入第一扇区内容		
							
								Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //读取扇区2  //修改第二扇区内容
								SPIFlash_Read_Tab(tab_dat,Block,Page,(PassWord_Num-A_Sector_PassWord_Num)*PassWord_Len); //将扇区2数据内容提取到tab_dat位置	
								
								for(it =0;it<(PassWord_Num-A_Sector_PassWord_Num-1)*PassWord_Len;it++)
								{
									tab_dat[it] = tab_dat[it+PassWord_Len];  //第二扇区密码左移一位
								}
								if(it>=PassWord_Len) //当第二扇区还有数据时
								{
									Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);
									SPIFlash_Erase_Sector(Block,Sector);
									SPI_Flash_Busy();
									Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);
									SPIFlash_Write_Tab(tab_dat,Block,Page,it); //写入第二扇区内容		
								}
							}
						}
						else if(PassWord_Num<=A_Sector_PassWord_Num) //当密码总数在一个扇区容量内时，无需前移
						{
								Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr1); //反之直接清除一扇区即可
								SPIFlash_Erase_Sector(Block,Sector);
								SPI_Flash_Busy();
								Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1);
								SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len); //写入第一扇区内容		
						}
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,len1);  //写入字符表				
						Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //擦除扇区 PassWord_SN_Secotr
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
							
						Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //在扇区中写入数据
						SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num-1);
						SPI_Flash_Busy();
						Ev_Cat_Dat(0x01,PassWord_Num-1);  //目录值减一
						
						return 1;
						
					}
					else if(PassWord_Secotr_Num==2)//改动了二扇区
					{
						Sector_to_Block_Sector(&Block,&Sector,PassWord_Secotr2);  //获取密码扇区位置
						SPIFlash_Erase_Sector(Block,Sector);   //擦除扇区
						SPI_Flash_Busy();
						Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2);   //获取密码扇区位置
						len1 = len1-PassWord_Len;  //减去一个扇区的密码
						
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,len1);  //写入字符表				
						Sector_to_Block_Sector(&Block,&Sector,PassWord_SN_Secotr); //擦除扇区 PassWord_SN_Secotr
						SPIFlash_Erase_Sector(Block,Sector);
						SPI_Flash_Busy();
							
						Sector_to_Block_Page(&Block,&Page,PassWord_SN_Secotr); //在扇区中写入数据
						SPIFlash_Write_Tab(tab1,Block,Page,PassWord_Num-1);
						SPI_Flash_Busy();
						Ev_Cat_Dat(0x01,PassWord_Num-1);  //目录值减一
						
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
载入一条开门记录

输入： SN_Num: SN号 手机开门为0

输入： Key_type  //开门方式(类型) 手机开门为0

输入： Unix_t 	时间戳值

指纹 0x01 密码0x02  IC卡0x03 身份证 0x04

*****************************************/
/*
void ADD_Op_Door_Note(uint8_t SN_Num,uint8_t Key_type,uint32_t Unix_t)  //载入开门记录 (开门时调用)
{
		uint32_t len,len1,Note_Num;  //定义总长度和号码
		uint32_t Block,Page,Sector; 
		uint32_t it,i;
		uint8_t tab[Open_Note_Num_Len];
		
		Note_Num = Get_Cat_Dat(OpenDoorNote_Num_Code);  //获取数量
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
		
	  if(Note_Num<Open_Note_Size) //当值在开门记录容量内时
		{
			it = Note_Num/A_Sector_Open_Note_Size; //获取扇区容量倍数
			len1= len-it*A_Sector_Open_Note_Size*Open_Note_Num_Len; //转换为一扇区内长度 成为起始地址
			for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it); //获取密码编号扇区数据
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);  //获取与密码数据长度等量的数据	
			
			for(i =0;i<Open_Note_Num_Len;i++)  //载入一条新数据
			{
				tab_dat[i+len1] = tab[i];
			}
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1+it);  //先擦除数据
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1+A_Sector_Open_Note_Size); //写入新数据数据
				SPI_Flash_Busy();	
			
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);
		
//			read_Soctor(1);
//			NRF_WDT_Feed();
//			read_Soctor(500);
//			NRF_WDT_Feed();
			
			//更新数据
		}
		else //当开门记录值大于等于最大容量时
		{
			Note_Num = Open_Note_Size - A_Sector_Open_Note_Size; //直接删除最早的一个扇区 开始载入新的数据
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1);  //先擦除最早数据
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();		
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //读取扇区数据
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //写入2扇区数据到1扇区
				SPI_Flash_Busy();	
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr2);  //先擦除2扇区数据
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //读取扇区数据
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //写入2扇区数据到1扇区
				SPI_Flash_Busy();	
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr3);  //先擦除3扇区数据
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //读取4扇区数据
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //写入4扇区数据到3 扇区
				SPI_Flash_Busy();	
			//整体数据上移一个扇区
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr4);  //擦除4扇区 写入一条数据
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Write_Tab(tab,Block,Page,Open_Note_Num_Len); //写入新数据到4扇区
				SPI_Flash_Busy();	
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);		//更新数据
		}
}

*/

/*****************************************
载入一条开门记录 （新版）
20201123
开门记录格式：
1字节类型+2字节钥匙ID+1字节钥匙详情长度+16字节钥匙详情+4字节时间
共计24字节 
类型：
0：密码开门
1：指纹开门
2：自定义卡开门
3：数据卡
5：手机开门
密码：
0+钥匙ID+密码长度+密码内容(6-10位) +时间
例：
0x00+密码类型
0x00+0x01+钥匙ID
0x0A+密码长度
0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X00+密码内容
0X5E+0xAC+0XAD+0XDE			//时间

ADD_Op_Door_Note :
输入：  
SN_Num: 类型
Key_type: 钥匙ID号
Unix_t： 开门时的时间戳


*****************************************/

u8 Op_Door_Buff[Open_Note_Num_Len];				//用于缓存开门记录数据 待写入缓存

void ADD_Op_Door_Note(uint16_t SN_Num,uint16_t Key_type,uint32_t Unix_t)  //载入开门记录 (开门时调用)
{
	
		uint32_t len,len1,Note_Num;  //定义总长度和号码
		uint32_t Block,Page,Sector; 
		uint32_t it,i;
		Note_Num = Get_Cat_Dat(OpenDoorNote_Num_Code);  //获取当前保存的数量
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
		Get_Time(); //获取板载时间
		Unix_t =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳		开门记录实际直接使用调用时间
	
		Op_Door_Buff[0] =0xAA;
		
		Op_Door_Buff[1] =SN_Num>>8&0xFF;
		Op_Door_Buff[2] =SN_Num&0xFF;
		

		Op_Door_Buff[3] =Key_type>>8&0xFF;
		Op_Door_Buff[4] =Key_type&0xFF;
		

		Op_Door_Buff[22] =(Unix_t>>24)&0xFF;
		Op_Door_Buff[23] =(Unix_t>>16)&0xFF;
		Op_Door_Buff[24] =(Unix_t>>8)&0xFF;
		Op_Door_Buff[25] =Unix_t&0xFF;
		
		if(Note_Num<Open_Note_Size) //当值在开门记录容量内时
		{
			
			it = Note_Num/A_Sector_Open_Note_Size; //获取扇区容量倍数
			
			len1= len-it*A_Sector_Open_Note_Size*Open_Note_Num_Len; //转换为一扇区内长度 成为起始地址
			
			for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it); //获取密码编号扇区数据
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,len1);  //获取与密码数据长度等量的数据	
			
			for(i =0;i<Open_Note_Num_Len;i++)  //载入一条新数据
			{
				
				tab_dat[i+len1] = Op_Door_Buff[i];
				
			}
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1+it);  //先擦除数据
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it);
			SPIFlash_Write_Tab(tab_dat,Block,Page,len1+A_Sector_Open_Note_Size); //写入新数据数据
				SPI_Flash_Busy();	
			
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);
			
	//		read_Soctor(1);
	//		NRF_WDT_Feed();
//			read_Soctor(500);
//			NRF_WDT_Feed();
			
			//更新数据
			
		}
		else //当开门记录值大于等于最大容量时
		{
			Note_Num = Open_Note_Size - A_Sector_Open_Note_Size; //直接删除最早的一个扇区 开始载入新的数据
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr1);  //先擦除最早数据
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();		
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //先读取2扇区数据
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //再写入2扇区数据到1扇区
				SPI_Flash_Busy();	
			
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr2);  //擦除2扇区数据
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //先读取3扇区数据
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr2);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //再将3扇区数据写到2扇区
				SPI_Flash_Busy();	
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr3);  //擦除3扇区数据
			SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //先读取4扇区数据
			
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr3);
			SPIFlash_Write_Tab(tab_dat,Block,Page,A_Sector_Open_Note_Size*Open_Note_Num_Len); //再将4扇区数据写入到3扇区
				SPI_Flash_Busy();	
			//整体数据上移一个扇区
			Sector_to_Block_Sector(&Block,&Sector,Open_Note_Secotr4);  //擦除4扇区 用以写入新数据
			SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();	
			Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr4);
			SPIFlash_Write_Tab(Op_Door_Buff,Block,Page,Open_Note_Num_Len); //写入新数据到4扇区
				SPI_Flash_Busy();	
			Ev_Cat_Dat(OpenDoorNote_Num_Code,Note_Num+1);		//更新数据量
		}
		
		for(i =0;i<Open_Note_Num_Len;i++)			//写入完成后清空数据
		{
			Op_Door_Buff[i] =0;
		}
		
	//		read_Soctor(Open_Note_Secotr1);
		//	NRF_WDT_Feed();
		
}


/***************************************

读取开门记录值用于返回

Read_Op_Note()

返回： 1： 读取有效  0： 读取无效


***************************************/

uint8_t Read_Op_Tab[Open_Note_Num_Len];

uint8_t Read_Op_Note(uint16_t Read_Num)
{	
		uint32_t Num,len1;  //定义总长度和号码
		uint32_t Block,Page;
		uint32_t it,i;
		
		Spi_Flash_Init();  //开启SPI_FLASH 通讯
	
		Num = Get_Cat_Dat(OpenDoorNote_Num_Code);				//获取本地保存的开门记录数量
			
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Read_Op_Note!\r\n");
		SEGGER_RTT_printf(0,"Read_Num: %d.\r\n",Read_Num);
		SEGGER_RTT_printf(0,"Num: %d.\r\n",Num);
		
		#endif
	
		if(Read_Num>Num)			//当输入的目标>本地最大数量时 返回0 弹出
		{
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Read_Num_Error!\r\n");				//提示读取数据溢出
			#endif
			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
			
			return 0;
		}
		
		for(i =0;i<sizeof(Read_Op_Tab);i++) Read_Op_Tab[i] =0x00;
		
		it = Read_Num/A_Sector_Open_Note_Size; //计算扇区
		len1 = (Read_Num-it*A_Sector_Open_Note_Size)*Open_Note_Num_Len;  //计算需要读取的长度
		//开始读取扇区
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0x00;
		
		Sector_to_Block_Page(&Block,&Page,Open_Note_Secotr1+it); //获取密码编号扇区数据
		SPIFlash_Read_Tab(tab_dat,Block,Page,len1);  //获取与密码数据长度等量的数据	
		
		for(i =0;i<sizeof(Read_Op_Tab);i++)
			Read_Op_Tab[i] = tab_dat[len1-Open_Note_Num_Len+i];
		
		
		 
		 
	   SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		 Spi_Flash_Sleep();
		 
		return 1;				//数据有效 返回1 

}




/**********************************
保存动态密码

输入：Act_Type   动态密码类型 
输入：*Act_PassWord 需要保存的动态密码
输入：Act_PassWord_Len 动态密码长度 (用于普通动态密码)
输入：past_unix_t  普通动态密码的失效时间 永久密码设置为0XFFFFFFFF
(
一次密码和普通密码
普通密码 02 一次密码01 
(普通密码永远只占用 动态密码存储区域0 位置 占用16字节 一次密码占用之后空间，每个占用9字节 共计 216个一次密码和1个普通密码的空间)
)
过期时间 (时间戳)载入密码的实际过期时间戳 (输入前需消除生成密码与时间差)

返回：无
**********************************/

const uint8_t Act_PassWord_SLen =20;  // 存储动态密码缓存长度

const uint8_t Common_Act_Len = 15;  // 普通动态密码长度 (0xAA+密码长度(6-9)+密码（6-9位）+过期时间(4字节))

const uint8_t Once_Act_Len  = 9;  //一次密码长度 (0XAA+密码(固定8位)) 

const uint8_t Once_PassWord_Size = 216;  //满足能存储24小时内所有动态密码 (每小时9个，一天24小时 共计 9*24 =216个)

#define  Once_Act_Type     1  // 定义一次动态密码类型
#define  Common_Act_Type   2  // 定义普通动态密码类型

void Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t)
{
		
		uint32_t len;  //定义总长度和号码
		uint32_t Block,Page,Sector;
		uint32_t i;
			
		uint32_t Act_PassWord_Num;
	
		uint8_t tab[Act_PassWord_SLen]; //动态密码缓存
		
		for(i =0;i<Act_PassWord_Len;i++)
		{
			
			tab[i] = *Act_PassWord;
			Act_PassWord++;
			
		}
		
//		Del_User_Key();			//保存动态密码前先删除用户钥匙
		
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;  //清空缓存
		
		Act_PassWord_Num = Get_Cat_Dat(0x02);  //获取当前设备已经保存的动态密码 (仅保存一次动态密码，普通动态密码直接存放在该扇区起始16字节区域)
		
		len = (Act_PassWord_Num*Once_Act_Len)+Common_Act_Len;  //计算长度  //第一位用于持续替换，len值只有有 Once_Act_Len 大小
	
		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);  //根据扇区号获取块和起始页
	
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据
		
		switch (Act_Type)				//仅限 一次密码和时效密码
		{
				case Once_Act_Type :  //当输入的动态密码是一次动态密码时
				
				for (i = len;i>=(Common_Act_Len+Once_Act_Len);i--) //第一个更新的数据在数据列表的起始，最后一个更新的数据在普通动态密码区域和一个一次密码后
				{
					
					tab_dat[i] = tab_dat[i-Once_Act_Len];  //一次密码整体右移(相对普通密码)，当容量满时，最后一个一次密码会被抛弃
				  
				}
				
				tab_dat[Common_Act_Len] = 0xAA;
				
				for(i =(Common_Act_Len+1);i<(Common_Act_Len+Once_Act_Len);i++) //将新密码写入
				{
					tab_dat[i] = tab[i-Common_Act_Len-1];
				}
				
				Act_PassWord_Num++;  //一次密码数量+1;
				
				if(Act_PassWord_Num>=Once_PassWord_Size)  //当密码数量大于最大值时，密码数量保留在最大值
					Act_PassWord_Num = Once_PassWord_Size;
				
				Ev_Cat_Dat(0x02,Act_PassWord_Num);  //更新一次密码数量
				
				len = Act_PassWord_Num * Once_Act_Len+Common_Act_Len;  //计算需要保存的数据总长度
				//数据操作完成后，开始写入新的数据
				Sector_to_Block_Sector(&Block,&Sector,Act_PassWord_Secotr);  //擦除动态密码扇区数据
				SPIFlash_Erase_Sector(Block,Sector);
				SPI_Flash_Busy();	
				
				Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
				SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区
				SPI_Flash_Busy();	
						
				break;
				
				case Common_Act_Type :  //当输入的密码是普通动态密码时直接替换之前密码 (普通动态密码不计数)
				
//				tab_dat[0] = 0XAA;  //加入头
//				tab_dat[1] = Act_PassWord_Len;
//				if((Act_PassWord_Len>=6)&&(Act_PassWord_Len<=9))
//				{
//		 			for (i =2;i<=(Act_PassWord_Len+2);i++) //载入密码值 根据输入密码长度来确定载入数据
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
	

		//动态密码写入完成
		
		//将时效密码写入多组动态密码区
	//	if(Act_Type==Common_Act_Type) //判断是否为时效密码
		//	More_Act_PassWord_Save(tab,Act_PassWord_Len,past_unix_t);
}

/*****************************************************************************

保存更多的动态密码(同时允许多组动态密码生效)

动态密码标志

输入： 

*Act_PassWord 密码指针  
Act_PassWord_Len 密码长度  
past_unix_t 有效时间

动态密码 格式

0XAA + 动态密码长度+ 10位动态密码空间 + 4位有效时间

******************************************************************************/
void More_Act_PassWord_Save(uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t)
{
		
		uint32_t len;  //定义总长度和号码
		uint32_t Block,Page,Sector;
		uint32_t i;
		uint32_t More_Act_PassWord_Max_Num;					//本次保存允许的最大数量
	
		uint32_t More_Act_PassWord_Num;
	
		uint8_t tab[Act_PassWord_SLen]; //动态密码缓存
		uint8_t ir=0;  //循环计数
	
	
			#ifdef uart_start
		
			SEGGER_RTT_printf(0,"More_Act_PassWord_Save!\r\n");
		
			#endif
	
		for(i =0;i<Act_PassWord_Len;i++)				//将动态密码写入缓存预留写入
		{
			tab[i] = *Act_PassWord;
			Act_PassWord++;
		}
		
		for(i =0;i<sizeof(tab_dat);i++)
			tab_dat[i] =0xFF;  //清空缓存
		
		More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //获取当前设备已经保存的动态密码 (多组动态密码保存的专用区域)
		
		if(More_Act_PassWord_Num&0xffff0000)   //当读取到的数值为多个动态密码的状态
		{
			
			More_Act_PassWord_Max_Num =More_Act_PassWord_MAX_Num;			//多个时效动态密码有效情况下设置More_Act_PassWord_Max_Num 为预设最大值
			More_Act_PassWord_Num&=0xFFFF;				//转换为正确数值
		}
		else				//反之则是单个动态密码有效   初始化后值为0 默认单个有效
		{
			
			More_Act_PassWord_Max_Num = 1;
		//	More_Act_PassWord_Num&=0XFFFF;
			
		}
		
		//More_Act_PassWord_Secotr
		
		len = More_Act_PassWord_Num * More_Act_PassWord_Len;
		
		Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);  //根据扇区号获取块和起始页
	
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据
		//判断是否有相同密码
		
		for(uint8_t it=0;it<More_Act_PassWord_Num;it++)  //逐个判断是否有一致密码
		{
			
			//先判断长度是否一致
			if(tab_dat[it*More_Act_PassWord_Len+1]==Act_PassWord_Len)    //判断长度是否一致
			{
				//长度一致的判断是否有一致内容
				ir =0;			//先清空计数
				for(uint8_t is=0;is<Act_PassWord_Len;is++)
				{
					
					if(
							tab_dat[it*More_Act_PassWord_Len+2+is] ==tab[is]
						)
						ir++;
					
				}
				if(ir==Act_PassWord_Len)   //找到一致数据，更新时间不修改数量
				{
					//当相同的数量等于Act_PassWord_Len  仅修改时间
					
					tab_dat[it*More_Act_PassWord_Len+13] = past_unix_t>>24&0xff;
					tab_dat[it*More_Act_PassWord_Len+14] = past_unix_t>>16&0xff;
					tab_dat[it*More_Act_PassWord_Len+15] = past_unix_t>>8&0xff;
					tab_dat[it*More_Act_PassWord_Len+16] = past_unix_t&0xff;
								
							//数据操作完成后，开始写入新的数据
					Sector_to_Block_Sector(&Block,&Sector,More_Act_PassWord_Secotr);  //擦除多组动态密码扇区数据
					SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
					
					Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);
					SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的多组数据到动态密码扇区
					SPI_Flash_Busy();	          
					
					read_Soctor(More_Act_PassWord_Secotr);				//输出扇区数据
				
					#ifdef uart_start
				
					SEGGER_RTT_printf(0,"UP Dat_Time! Num: %x!\r\n",it);
				
					#endif
					
					return;   //修改时间后直接结束程序
					
				}
				
			}
		}
		
		
		//没有相同密码时将新数据写入最后
		
		tab_dat[len] = 0XAA;		//载入数据头
		tab_dat[len+1] = Act_PassWord_Len;		//载入长度
		
		for(i =0;i<Act_PassWord_Len;i++)			//载入新的动态密码
		{
			
			tab_dat[i+len+2] = tab[i];	
		
		}
		
		tab_dat[len+13] = past_unix_t>>24&0xff;
		tab_dat[len+14] = past_unix_t>>16&0xff;
		tab_dat[len+15] = past_unix_t>>8&0xff;
		tab_dat[len+16] = past_unix_t&0xff;
	
		if(More_Act_PassWord_Num>=More_Act_PassWord_Max_Num)   //当时效动态密码等于最大值 覆盖最早写入的密码
		{
			for (i =0;i<(More_Act_PassWord_Max_Num*More_Act_PassWord_Len);i++)				//数据迁移一格
			{
				tab_dat[i] = tab_dat[i+More_Act_PassWord_Len];
			}
			More_Act_PassWord_Num=More_Act_PassWord_Max_Num;   //数量始终保持最大值
		}
		else
			More_Act_PassWord_Num++;			//反之 数量+1
	
			len = More_Act_PassWord_Num * More_Act_PassWord_Len;  //计算需要保存的数据总长度
		
		//数据操作完成后，开始写入新的数据
		Sector_to_Block_Sector(&Block,&Sector,More_Act_PassWord_Secotr);  //擦除多组动态密码扇区数据
		SPIFlash_Erase_Sector(Block,Sector);
		SPI_Flash_Busy();	
		
		Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的多组数据到动态密码扇区
		SPI_Flash_Busy();	                                   
		
		if(
				More_Act_PassWord_Max_Num ==More_Act_PassWord_MAX_Num
			)//当满足多组密码条件时，输入预设值&0XFFFF0000
		{
			Ev_Cat_Dat(More_Act_PassWord_Num_Code,More_Act_PassWord_Num|0XFFFF0000);  //更新多组动态密码数量
		}
		else
		Ev_Cat_Dat(More_Act_PassWord_Num_Code,1);  //更新单组动态密码数量
		
		read_Soctor(More_Act_PassWord_Secotr);				//输出扇区数据
		
}



//Per_Act_PassWord_Secotr
/**************************************************************************
保存永久动态密码

永久动态密码数量  20个

保存格式：  0X55 + 8位密码

**************************************************************************/

#define Per_Act_Len 9					//1条 永久密码长度
#define Per_Act_Num 20				// 永久密码容量

void Per_Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t)
{
		uint32_t len;  //定义总长度和号码
		uint32_t Block,Page,Sector;
		uint32_t i;
		uint8_t Per_Act_PassWord_Num =0;
		
		uint8_t tab[Act_PassWord_SLen]; //动态密码缓存
		
		#ifdef uart_start
	
		SEGGER_RTT_printf(0,"\r\nStart_Per_Act_PassWord_Save!\r\n");
	
		#endif
	
		tab[0] =0x55;			//设置头
		
		for(i =1;i<Per_Act_Len;i++)			//将密码载入缓存区（正常情况下长度必须为8）
		{
			
			tab[i] = *Act_PassWord;
			Act_PassWord++;
			
		}
		

		
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;  //清空缓存

		Per_Act_PassWord_Num = Get_Cat_Dat(0x04);  //获取当前设备已经保存的动态密码 (仅保存一次动态密码，普通动态密码直接存放在该扇区起始16字节区域)
		
		len = Per_Act_PassWord_Num*Per_Act_Len;  //计算长度  //第一位用于持续替换，len值只有有 Once_Act_Len 大小
	
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);  //根据扇区号获取块和起始页
	
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Per_Act_PassWord_Num: %x!\r\n",Per_Act_PassWord_Num);
		
		#endif
		
		
		if(Per_Act_PassWord_Num<Per_Act_Num)					//当数据量在容量范围内时
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"NEED_A_Per_Act_PassWord!\r\n");
			
			#endif
			
			for(i =0;i<Per_Act_Len;i++)			//将新数据载入缓存
			{
				
				tab_dat[len+i] = tab[i];
				
			}
			
			Per_Act_PassWord_Num++;					//密码数量 + 1
			
			Ev_Cat_Dat(0x04,Per_Act_PassWord_Num);  //更新永久密码数量
			
			len+=Per_Act_Len;			//长度 + 一个数据
			
			//数据操作完成后，开始写入新的数据
		Sector_to_Block_Sector(&Block,&Sector,Per_Act_PassWord_Secotr);  //擦除动态密码扇区数据
		SPIFlash_Erase_Sector(Block,Sector);
		SPI_Flash_Busy();	
		
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区
		SPI_Flash_Busy();	
			
		}
		else				//当永久密码数量满时 整体前进一格抛掉第一个在最后写入新的
		{
			
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"REP_A_Per_Act_PassWord!\r\n");
			
			#endif
			
			
			for(i =0;i<len;i++)
		
			{
				
				tab_dat[i] = tab_dat[i+Per_Act_Len];
			
			}
			
			for(i =0;i<Per_Act_Len;i++)					//将新的密码放到最后
			{
				
				tab_dat[Per_Act_Len*Per_Act_Num-Per_Act_Len+i] =tab[i]; 
				
			}
			
			//替换不影响数量所以不需要更新数量 写入数据长度也不改变
			
					//数据操作完成后，开始写入新的数据
		Sector_to_Block_Sector(&Block,&Sector,Per_Act_PassWord_Secotr);  //擦除动态密码扇区数据
		SPIFlash_Erase_Sector(Block,Sector);
		SPI_Flash_Busy();	
		
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区
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

动态密码验证函数 (支持虚位密码)

功能：判断输入密码是否与保存的普通动态密码一致，并确定密码是否已过期


输入： PassWord_Len  输入的密码长度
输入： PW_Unix_T 		 当前板载时间


******************************************************************************************/

extern uint8_t Key_Num_Tab[];

//extern u8 Op_Door_Buff[Open_Note_Num_Len];				//用于缓存开门记录数据 待写入缓存 顶部已定义


uint8_t  Act_PassWord_rese =0;   //当发现输入密码与本地密码有相似时，置1，之后将不再允许此密码进行动态密码运算

uint8_t Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T)
{
	 
//		uint32_t len;  //定义总长度和号码
		uint32_t Block,Page;	 //读取Flash扇区所需变量
		uint32_t it,it1,i,is,is1;
		uint32_t Act_Unix_T; //保存的有效动态密码的有效时间
	//	uint8_t tab[10];  //动态密码比较缓存
		
		Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);  //根据扇区号获取块和起始页
		SPIFlash_Read_Tab(tab_dat,Block,Page,Common_Act_Len);  //读取与现有数据量等长的扇区数据

		if(tab_dat[0]!=0xAA)  //当头不满足内容时，本地动态密码无效，直接结束操作
			return 0;
		
		it = tab_dat[1];  //将密码长度值载入缓存
		
		if((it<6)||(it>9)) // 当密码长度值不合法时，本地动态密码无效，直接结束操作
			return 0;
		
		if(PassWord_Len<6)  //当输入的长度不够，输入的密码无效，直接结束操作
			return 0;
//		if(PassWord_Len>9)   //不排除虚位密码
//			return 0;
		
		Act_Unix_T = tab_dat[11];  //载入时间戳 24-31位
		Act_Unix_T <<= 8;
		
		Act_Unix_T |= tab_dat[12];  //载入时间戳 16-23位
		Act_Unix_T <<=8;

		Act_Unix_T |= tab_dat[13];  //载入时间戳 8-15位
		Act_Unix_T <<=8;
		
		Act_Unix_T |= tab_dat[14];  //载入时间戳 0-7位
		
		it =0;  //用于标志密码是否完全一致，有不一致的地方既不为0，需继续轮询，或密码无效
		it1 =0;  //用于标识密码一致的个数
		if(Act_Unix_T <PW_Unix_T) //检测到当本地时效动态密码过期时，删掉该词条(通过将起始位置0来实现，降低Flash损耗)
		{
			tab_dat[0] =0x00;  //将该位清除，表示该密码已失效
			Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);
			SPIFlash_Write_Tab(tab_dat,Block,Page,1); //写入修改后的数据到动态密码扇区
			SPI_Flash_Busy();	
			return 0;
		}
		
		if(tab_dat[1]<=PassWord_Len)
		{
			is1 = PassWord_Len-tab_dat[1];
			for(i =0;i<=is1;i++)  //开始虚位密码轮询，是否有一致的密码 i为待轮询检测的起点，需确保起点后有效输入密码数量大于等于本地动态密码长度
			{
				it1 =0;
				for(is =0;is<tab_dat[1];is++) //从指定输入密码的头开始，在动态密码长度范围内轮询是否完全一致
				{
					if(Key_Num_Tab[i+is]!=tab_dat[is+2]) it=1;  //当有数值不同时，将it置1 表示比对失败
					else it1++;
				}
				if((it1>=(tab_dat[1]-2))&&   //判定相似位数为 密码长度 -2
					(tab_dat[1]>6)&&						//限制仅对长度大于6的密码有效
					(it1<(tab_dat[1]))													//当密码长度与一致的密码个数一致时判断新动态密码不生效
					)
				{
					
					Act_PassWord_rese =1;   //判定相似位置1
					
				}
				
			if(it==1) it =0;
				else 
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Off_Lin_PassWord_Time_Limit:%d\r\n",Act_Unix_T);
					
					#endif
					
							
					nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
					nrf_delay_ms(1); //等待稳定
					
					if(nrf_gpio_pin_read(5)==0)   //当门反锁时提示门已反锁  反锁时不允许动态密码开门
					{
						
						nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
						
					//	input_speak(17); //语音提示已反锁
				//		nrf_delay_ms(1200);
						return 0;
					}
				
					nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
			
					//记录开门的密码
					Op_Door_Buff[5] = tab_dat[1];  //记录长度
					
					for(i =0;i<tab_dat[1];i++)			//记录内容
						Op_Door_Buff[i+6] = tab_dat[i+2];
					
					
					return 1;  //当数值一样时，返回1，表示密码正确且有效，允许开门
					
				}
			}
		}
		return 0;
}



/*****************************************************************************************

多组动态密码验证函数

功能：判断输入密码是否与保存的时效动态密码一致，并确定密码是否已过期


输入： PassWord_Len  输入的密码长度
输入： PW_Unix_T 		 当前板载时间

*******************************************************************************************/
uint8_t More_Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T)
{
		//		uint32_t len;  //定义总长度和号码
		uint32_t Block,Page;	 //读取Flash扇区所需变量
		uint32_t it,it1,it2,i,is,is1;
		uint32_t Act_Unix_T; //保存的有效动态密码的有效时间
		uint32_t More_Act_PassWord_Num;
		uint32_t len;
		uint8_t ir;			//循环计数
		
	//	uint8_t tab[Key_Num_Tab_Len]; //动态密码缓存
		
	
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"More_Act_PassWord_Verify!\r\n");
		
		#endif
//	
//		for(i =0;i<PassWord_Len;i++)
//			tab[i] = Key_Num_Tab[i];
		
		More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //获取当前设备已经保存的动态密码 (多组动态密码保存的专用区域)
		More_Act_PassWord_Num&=0xFFFF;		//取 More_Act_PassWord_Num 低两个字节有效数值
		len = More_Act_PassWord_Num * More_Act_PassWord_Len;
			
		Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);  //根据扇区号获取块和起始页
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据
		
//		if(PassWord_Len<6)  //当输入的长度不够，输入的密码无效，直接结束操作
//			return 0;
//		
		//逐个判断动态密码是否匹配
		
		for(it=0;it<More_Act_PassWord_Num;it++)    //逐个判断
		{
		
			#ifdef uart_start
					
			SEGGER_RTT_printf(0,"Cheak_PassWord_N: %x!\r\n",it);
					
			#endif
			
			if(tab_dat[it*More_Act_PassWord_Len+1]>PassWord_Len)    //本地保存的对应密码长度要大于等于输入的长度
			{
				;//本地长度大于输入长度，必然不成立
			}
			else    //找到一样的密码
			{
				for(it1=0;
				it1<=(PassWord_Len-(tab_dat[it*More_Act_PassWord_Len+1]));  //输入长度与本地长度的差决定外框循环次数
					it1++)			//虚位密码外框循环
				{

					ir =0;
					for(it2 =0;
							it2<(tab_dat[it*More_Act_PassWord_Len+1]);  //比对本地密码
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
								//匹配到正确密码
										//载入时间
								
										#ifdef uart_start
										
										SEGGER_RTT_printf(0,"C_More_Act_PassWord_Num: %x!\r\n",it);
										
										#endif
										
										Act_Unix_T = tab_dat[it*More_Act_PassWord_Len+13];  //载入时间戳 24-31位
										Act_Unix_T <<= 8;
										
										Act_Unix_T |= tab_dat[it*More_Act_PassWord_Len+14];  //载入时间戳 16-23位
										Act_Unix_T <<=8;

										Act_Unix_T |= tab_dat[it*More_Act_PassWord_Len+15];  //载入时间戳 8-15位
										Act_Unix_T <<=8;
										
										Act_Unix_T |= tab_dat[it*More_Act_PassWord_Len+16];  //载入时间戳 0-7位
										
										if(Act_Unix_T >PW_Unix_T)
										{
											//允许开门
											
												#ifdef uart_start
		
												SEGGER_RTT_printf(0,"More_Act_PassWord_Time_Limit:%d\r\n",Act_Unix_T);
												
												#endif
												
														
												nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
												nrf_delay_ms(1); //等待稳定
												
												if(nrf_gpio_pin_read(5)==0)   //当门反锁时提示门已反锁  反锁时不允许动态密码开门
												{
													
													nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
													
												//	input_speak(17); //语音提示已反锁
											//		nrf_delay_ms(1200);
													return 0;
												}
											
											nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
										
												//记录开门的密码
												Op_Door_Buff[5] = tab_dat[1];  //记录长度
												
												for(i =0;i<tab_dat[it*More_Act_PassWord_Len+1];i++)			//记录内容
													Op_Door_Buff[i+6] = tab_dat[it*More_Act_PassWord_Len+2+i];
												
												#ifdef uart_start
		
												SEGGER_RTT_printf(0,"More_Act_PassWord_OP_Door!\r\n");
												
												#endif
												
												return 1;  //当数值一样时，返回1，表示密码正确且有效，允许开门
								
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

//删除单个 本地动态密码

输入： *PassWord  目标动态密码

************************************************/
uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord)
{
	
	uint8_t tab[15];			//动态密码缓存
	uint32_t Block,Page,Sector;	 //读取Flash扇区所需变量
	uint32_t it,it1,it2,i,is,is1;
	uint32_t More_Act_PassWord_Num;
	uint32_t len;
	uint8_t ir;			//循环计数

	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"Dle_A_More_Act_PassWord_Verify!\r\n");
	
	#endif
	
	More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //获取当前设备已经保存的动态密码 (多组动态密码保存的专用区域)
	More_Act_PassWord_Num&=0xFFFF;		//取 More_Act_PassWord_Num 低两个字节有效数值
	len = More_Act_PassWord_Num * More_Act_PassWord_Len;
	
	Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);  //根据扇区号获取块和起始页
	SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据
	
	for(i=0;i<sizeof(tab);i++)
		tab[i] =0;
		
		tab[0] = *PassWord;				//先载入密码数量
		if(tab[0]<10)
		{
			for(i =0;i<tab[0];i++)				// 根据密码数量载入数据
			{
				PassWord++;
				tab[i+1] = *PassWord;
				
			}
		}
		else
			return 0;
		
		for(it=0;it<More_Act_PassWord_Num;it++)    //逐个判断
		{
			
			#ifdef uart_start
					
			SEGGER_RTT_printf(0,"Cheak_PassWord_N: %x!\r\n",it);
					
			#endif                                  
			
			if(tab_dat[it*More_Act_PassWord_Len+1]==tab[0])    //本地保存的密码等于载入密码长度
			{
				//仅对等长的密码判断
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
				
				if(ir==tab[0])  //当 找到一致的 密码时覆盖掉这个密码
				{
					#ifdef uart_start
				
					SEGGER_RTT_printf(0,"Cheak_PassWord_OK: %x!\r\n",it);

					#endif
					for(i =0;i<((More_Act_PassWord_Num-it)*More_Act_PassWord_Len);i++)
					{
						
						tab_dat[it*More_Act_PassWord_Len+i] = tab_dat[(it+1)*More_Act_PassWord_Len+i];
						
					}
					//修改完数据后 重新写入内容和数量
					
					
						More_Act_PassWord_Num = Get_Cat_Dat(More_Act_PassWord_Num_Code);  //获取当前设备已经保存的动态密码 (多组动态密码保存的专用区域)
					//	More_Act_PassWord_Num&=0xFFFF;		//取 More_Act_PassWord_Num 低两个字节有效数值
					
					if((More_Act_PassWord_Num&0xFFFF)>=1)
						More_Act_PassWord_Num -=1;  //总数减一
						
					Sector_to_Block_Sector(&Block,&Sector,More_Act_PassWord_Secotr);  //擦除多组动态密码扇区数据
					SPIFlash_Erase_Sector(Block,Sector);
					SPI_Flash_Busy();	
					
					Sector_to_Block_Page(&Block,&Page,More_Act_PassWord_Secotr);
					SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的多组数据到动态密码扇区
					SPI_Flash_Busy();	          
					
					Ev_Cat_Dat(More_Act_PassWord_Num_Code,More_Act_PassWord_Num);    //更新总数
					
					read_Soctor(More_Act_PassWord_Secotr);				//输出扇区数据
					
					return 1;   
						
				}
			}
		}
		return 1;
}



/********************************************************************************************
本地永久密码判断

永久密码不需要时间和长度

返回1 说明可以开门


*********************************************************************************************/
uint8_t Per_Act_PassWord_Verify(uint8_t PassWord_Len)
{
		
		uint32_t Block,Page;	 //读取Flash扇区所需变量
		uint32_t it,it1,is,is1;
//	uint32_t Act_Unix_T; //保存的有效动态密码的有效时间
		uint32_t len;
		uint32_t Per_Act_PassWord_Num;
	
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Per_Act_PassWord_Verify!\r\n");
		
		#endif
	
		len =0;
	
		Per_Act_PassWord_Num = Get_Cat_Dat(0x04);  //获取当前设备已经保存的动态密码 (仅保存一次动态密码，普通动态密码直接存放在该扇区起始16字节区域)
	
		#ifdef uart_start
	
		SEGGER_RTT_printf(0,"\r\n Per_Act_PassWord_Num: %x .\r\n",Per_Act_PassWord_Num);
			
		#endif
	
		len = Per_Act_PassWord_Num*Per_Act_Len;  //计算长度  //第一位用于持续替换，len值只有有 Once_Act_Len 大小
	
		
	if(PassWord_Len>=8);			//判断密码长度合理
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
	

		
	
		Sector_to_Block_Page(&Block,&Page,Per_Act_PassWord_Secotr);  //根据扇区号获取块和起始页
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据
		
		
		
	//Key_Num_Tab
		
		for(it =0;it<Per_Act_PassWord_Num;it++)								//需要判断的密码数量
		{
				for(it1 =0;it1<=(PassWord_Len-8);it1++)			//按多出多少位决定判断多少次
				{
						is1 =0;
						for(is =0;is<8;is++)
						{
							
								if(tab_dat[it*Per_Act_Len+is+1] == Key_Num_Tab[is+it1])
								is1++;
								
								if(is1==8)			//当密码一致时返回1 (正确位数等于永久密码长度)
								{
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"it: %x!\r\n",it);
									SEGGER_RTT_printf(0,"it1: %x!\r\n",it1);
									SEGGER_RTT_printf(0,"is: %x!\r\n",is);
									SEGGER_RTT_printf(0,"is1: %x!\r\n",is1);
									
									#endif
									
										nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
										nrf_delay_ms(1); //等待稳定
							
										if(nrf_gpio_pin_read(5)==0)   //当门反锁时提示门已反锁  反锁时不允许动态密码开门
										{
											
											nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
											
										//	input_speak(17); //语音提示已反锁
										//	nrf_delay_ms(1200);
											return 0;
										}
									
									nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
									
								//将永久动态密码内容 载入开门记录详情
								for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff 长度
									Op_Door_Buff[i1] =0;									
									
									Op_Door_Buff[5] = 8;		//密码长度 =  Key_Num_n
									
									for(uint8_t i1=0;i1<8;i1++)		//将时效动态密码数据载入
									{
										 Op_Door_Buff[6+i1] =Key_Num_Tab[i1+it];				//载入对应位置的 永久密码
										
									}
										
									return 1;
								}
						}
					
				}
		}
	
	return 0;
}



/******************************************************************************************

Once_Act_PassWord_Verify  //用于确认一次密码是否已经被使用过

//输入： 无  判断的密码直接取 Key_Num_Tab 最前面8位，且需确认 Key_Num_n 的值为 8 (和一次密码长度一致)

动态密码需要在生效后的24小时内使用，能且只能使用一次。

输出： 一次密码是否存在，存在返回0(不可开门)，不存在返回1 (可开门，随即存下这个动态密码)

*******************************************************************************************/

uint32_t Once_Act_Run =0;  //一次密码的使用记录，当超过24小时未使用一次密码时，清空一次密码
uint32_t Once_Act_Num =0;  //一次密码数量
uint8_t Once_Act_PassWord_Verify()
{
		
		uint32_t len;  //定义总长度和号码
		uint32_t Block,Page;  //读取Flash扇区所需变量
		uint32_t it,i,is;
		//uint32_t Act_Unix_T; //保存的有效动态密码的有效时间
		uint32_t Act_PassWord_Num;
	
		Act_PassWord_Num = Get_Cat_Dat (0x02);  //获取已保存的动态密码数量
		Once_Act_Num = Act_PassWord_Num;
		Once_Act_Run =1;
		for(i =0;i<sizeof(tab_dat);i++)tab_dat[i] =0xFF;  //清空缓存
		
	  len = Once_Act_Len*Act_PassWord_Num+Common_Act_Len;
		
		if(Act_PassWord_Num==0)  // 当本地未保存一次密码时，不进行校对，直接存入该动态密码
		{
			Act_PassWord_Save(Once_Act_Type,Key_Num_Tab,8,0); //将这个一次密码写入 
			
		//将一次密码内容 载入开门记录详情
			for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff 长度
				Op_Door_Buff[i1] =0;
		
			Op_Door_Buff[5] = 8;		//本地密码长度都是6
			
			Op_Door_Buff[6] = Key_Num_Tab[0];		//本地密码第一位
			Op_Door_Buff[7] = Key_Num_Tab[1];	//本地密码第二位
			Op_Door_Buff[8] = Key_Num_Tab[2];	//本地密码第三位
			Op_Door_Buff[9] = Key_Num_Tab[3];		//本地密码第四位
			Op_Door_Buff[10] = Key_Num_Tab[4];		//本地密码第五位
			Op_Door_Buff[11] = Key_Num_Tab[5];	//本地密码第六位
			Op_Door_Buff[12] = Key_Num_Tab[6];	//本地密码第六位
			Op_Door_Buff[13] = Key_Num_Tab[7];	//本地密码第六位
					
				
			
			return 1; //当一次密码第一次输入时，返回1 允许开门一次。
			
		}
	  else if(Act_PassWord_Num>0)  //当本地保存有一次密码时，进行校对，并存入动态密码
		{
			
			Sector_to_Block_Page(&Block,&Page,Act_PassWord_Secotr);  //根据扇区号获取块和起始页
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //读取与现有数据量等长的扇区数据		
		  
			it =0;  //重复循环
			
			for(i =Common_Act_Len;i<len;) //轮询是否有一致的数据，有则返回0 不予开门，没有则写入，允许开门一次 轮询时跳过普通动态密码长度
			{
					for(is =i;is<(i+Once_Act_Len);is++)
					 {
						 if(tab_dat[is+1] == Key_Num_Tab[is-i])
								it++;
					 }
					 if(it>=8) 
						 return 0;  //有相同一次密码时，返回0,不允许开门
					 
					 else it =0;
					 i+=Once_Act_Len;
			}
			
			Act_PassWord_Save(Once_Act_Type,Key_Num_Tab,8,0); //当这个一次密码不存在时，将这个一次密码写入 
			
			//将一次密码内容 载入开门记录详情
			for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff 长度
				Op_Door_Buff[i1] =0;
		
			Op_Door_Buff[5] = 8;		//本地密码长度都是6
			
			Op_Door_Buff[6] = Key_Num_Tab[0];		//本地密码第一位
			Op_Door_Buff[7] = Key_Num_Tab[1];	//本地密码第二位
			Op_Door_Buff[8] = Key_Num_Tab[2];	//本地密码第三位
			Op_Door_Buff[9] = Key_Num_Tab[3];		//本地密码第四位
			Op_Door_Buff[10] = Key_Num_Tab[4];		//本地密码第五位
			Op_Door_Buff[11] = Key_Num_Tab[5];	//本地密码第六位
			Op_Door_Buff[12] = Key_Num_Tab[6];	//本地密码第六位
			Op_Door_Buff[13] = Key_Num_Tab[7];	//本地密码第六位
			
			return 1; //当一次密码第一次输入时，返回1 允许开门一次。
			
		}
		
			return 0;
	
}

const uint8_t History_Time_Len = 11;  //单个 历史动态密码长度  (0XAA+密码长度 + 9字节密码保存空间)
extern uint8_t Key_Num_n;
const uint8_t History_Max_Num = 30;  //最大保存历史密码数量

/***************************************************

History_Time_Act_PassWord  

功能:

新的时效性动态密码，生效前需要进行判断，这个密码是否在之前已经有使用过

输入： 直接按键缓存里的数据

输出 有同样密码 返回1，无同样密码返回0 
有同样密码时，密码不能保存，也不能使用，
没有同样密码时，保存密码，可以使用。

***************************************************/

uint8_t  History_Time_Act_PassWord()
{
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t it,i,is;
//			uint32_t Act_Unix_T; //保存的有效动态密码的有效时间
			uint32_t Act_PassWord_Num;

			
			Act_PassWord_Num = Get_Cat_Dat (0x03);  //获取已保存的动态密码数量
			
			len = Act_PassWord_Num * History_Time_Len;  //换算出 现有本地动态密码总数
			
			for(i =0;i<sizeof(tab_dat);i++) tab_dat[i] =0;  //清空缓存
			
			if(len>=History_Time_Len)  //当本地保存有一个以上的历史时效动态密码时
			{
					Sector_to_Block_Page(&Block,&Page,History_PassWord_Secotr);  //根据扇区号获取块和起始页
					SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据		
					
					it =0;  //重复循环
					for (i =0;i<len;) //轮询是否有一致的数据，有则返回0 不予开门，没有则写入，允许开门一次 轮询时跳过普通动态密码长度
					{
							for (is =i;is<(i+Key_Num_n);is++)
							 {
									if((tab_dat[is+2] == Key_Num_Tab[is-i])&&(tab_dat[i+1]==Key_Num_n))  //当长度一致，且对应位密码一样时，it 值加一
										it++;
							 }
									 
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"it:%d\r\n",it);
								
								#endif
										 
							 if(it>=Key_Num_n)
								 return 0;  //有相同历史密码时，返回0,不允许开门
							 else 
								 it =0;
							 
							 i+=History_Time_Len;
					}
			}
			
			for (i=(len+History_Time_Len);i>=History_Time_Len;i--)  //当密码不一致时，平移密码空出第一个位置
			{
				tab_dat[i]=tab_dat[i-History_Time_Len];
			}
			
			tab_dat[0]=0xAA;  //载入新数据包头
			
			tab_dat[1]=Key_Num_n;  //载入新数据长度
			
			for (i=0;i<(History_Time_Len-2);i++)  //将新的历史密码数据放到最前面，整体逐步后移
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
		
		Ev_Cat_Dat(0x03,Act_PassWord_Num);  //更新数量
		
		Sector_to_Block_Sector(&Block,&Sector,History_PassWord_Secotr);  //擦除动态密码扇区数据
		
		SPIFlash_Erase_Sector(Block,Sector);
		
		SPI_Flash_Busy();
		
		Sector_to_Block_Page(&Block,&Page,History_PassWord_Secotr);
		
		SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区
	 	SPI_Flash_Busy();	
		
		return 1;  //保存完新的历史数据后，返回1

}


/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


写入一条管理员A卡信息到 对应存储区域

数据载入格式：
客人卡：
0XAA+4字节A卡信息(序列号)+4字节起始时间+4字节过期时间 每条共计13字节
管理员卡：
0X55+4字节卡信息（序列号）+4字节起始时间+4字节过期时间 每条共计13字节

一个扇区可保存共计145条数据

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //保存B卡数据的扇区1 (目前共计1个扇区)


const uint8_t TypeA_Data_Len = 14;  	//一条A卡数据的长度

const uint8_t TypeB_Data_Len = 18;		//一条B卡数据的长度


*****************************************************************************/
/*
uint16_t Add_Flash_AD_TypeA_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
	
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeA_Data_Num;
			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeA_Data_Len];  //缓存
			
			uint32_t is1,is2;
			
		for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
				tab_dat[i] =0;
			
			for(i =0;i<4;i++)  //将uid值载入 tab缓存
			{
					tab[i] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			len1 =0;
			len1 =0;
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
			if(TypeA_Data_Num==0XFFFFFF)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_TypeA_Data_Num_Error!\r\n");
				
				#endif
				
				return 0;
				
			}
			
			len = TypeA_Data_Num * TypeA_Data_Len;  //得出占用的缓存数
	
			if(len>=TypeA_Data_Len)  //当len数量 大于等于一条 TypeA_Data 长度 既已保存有数据时
			{
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);  //根据扇区号获取块和起始页
				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
			
				len1 =0; //开始时将len1 值置得与i一致
				
				for(i =0;i<len;)
				{
					if(tab_dat[i]==0XAA) //当头满足时
					{
						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])) //当本地有一致的uid时 将len1 设置为一致uid的起始地址
						{
							//len1 = i;
							i = len;  //将i的值赋予len1 并通过将i的值置成len 以结束之后的循环
						}
						else
						{
							i+=TypeA_Data_Len;  //查询完一个uid后，切换到下一个起始地址
							len1 = i;					//同时更新 len1 的值
						}
					}
					else
					{
						i = len;
						len1 =len;
					}
				}
				
				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
					&&(TypeA_Data_Num<A_Sector_TypeA_Data_Num)) //当len1等于len时，表示本地数据没有一致的卡信息
				{
					
					for(i =4;i>=1;i--)   //将数据写入头
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //写入头
					
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
						for(is1 =1;is1<=TypeA_Data_Num;is1++)  //在一个扇区 Type A 卡数据总数内寻找可用的SN号
						{
							if(tab_dat[is2*TypeA_Data_Len-1]==is2)//当SN号存在时 结束循环
							{
								break;
							}
							
						}
						
						if(is2>TypeA_Data_Num)
						{
							break;
						}
						
						if(is2>A_Sector_TypeA_Data_Num)  //当 所有可用SN号用完时，结束写入操作，返回失败信息 
							return 0;
					}
					
					tab[TypeA_Data_Len-1] = is2;  //将获取到的 SN 号写入 tab 缓存 待写入
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
					
					#endif
					
					for(i = len1;i<(len1+TypeA_Data_Len);i++)
					{
						tab_dat[i] = tab[i-len1];  //将tab值载入到tab_dat中				
					}
					
					if(tab_dat[len1]==0xAA)
					{
						
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //擦除动态密码扇区数据
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeA_Data_Len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
							SPI_Flash_Busy();
							
							Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //将新的数据量写入目录
							
							return tab[TypeA_Data_Len-1];
					}
					return 0;
				}
				else if(len1<=len-TypeA_Data_Len)  //当len1 小于 len 减 TypeA_Data_Len 说明卡ID号在本地已存在，只更新时间不增加数据总量
					{
						
					if(tab_dat[len1]==0xAA)  //当数据包起始值满足条件时
						{
							
							tab_dat[len1+5] = start_Unix>>24&0xFF;
							tab_dat[len1+6] = start_Unix>>16&0xFF;
							tab_dat[len1+7] = start_Unix>>8&0XFF;
							tab_dat[len1+8] = start_Unix&0xFF;
							
							tab_dat[len1+9]  =  stop_Unix>>24&0XFF;
							tab_dat[len1+10] =  stop_Unix>>16&0XFF;
							tab_dat[len1+11] =  stop_Unix>>8&0XFF;
							tab_dat[len1+12] =  stop_Unix&0xFF;
							
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //擦除动态密码扇区数据
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
							SPI_Flash_Busy();
							
							return tab_dat[len1+TypeA_Data_Len-1];  //返回当前数据的SN号
							
						}
						
					}
				else if (TypeA_Data_Num>=A_Sector_TypeA_Data_Num) //当存储的卡数据大于等于单个容量(一般情况下不可能大于)，不在写入数据
				{
					
					#ifdef uart_start
					SEGGER_RTT_printf(0,"AD_TypeA_Data_Sector1 Full!\r\n");
					#endif
					
					return 0;
					
				}
				
			}
			else if(len==0)  //当len值为空时，直接在第一个位置加入新的
			{
				  
					for(i =4;i>=1;i--)   //将数据写入头
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //写入头
					
					tab[5] = start_Unix>>24&0xFF;
					tab[6] = start_Unix>>16&0xFF;
					tab[7] = start_Unix>>8&0XFF;
					tab[8] = start_Unix&0xFF;
					
					tab[9]  =  stop_Unix>>24&0XFF;
					tab[10] =  stop_Unix>>16&0XFF;
					tab[11] =  stop_Unix>>8&0XFF;
					tab[12] =  stop_Unix&0xFF;
					
					tab[13] = 1; //载入第一个数据 因为没有其他数据设 SN号为1
				
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
					
					#endif
					
				for(i=0;i<(TypeA_Data_Len);i++)  //因为 len 值为 0 ， 所以直接在tab_dat 的起始位置写入第一条数据
				{
					
					tab_dat[i] = tab[i];  //将 tab 值载入到 tab_dat 中
					
				}
				
				Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //擦除管理员密码扇区1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(AD_TypeA_Data_Num_Code,1);  //将新的数据量写入目录
				
				return tab[TypeA_Data_Len-1];  //返回最后一字节， SN 号
			}
			
			return 0;
}






*/

/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


写入一条管理员A卡信息到 对应存储区域

数据载入格式：
客人卡：
0XAA+4字节A卡信息(序列号)+4字节起始时间+4字节过期时间+ 扇区内编序列号 每条共计14字节
管理员卡：
0XAA+4字节卡信息（序列号）+4字节起始时间+4字节过期时间+ 扇区内编序列号 每条共计14字节

一个扇区可保存共计145条数据 方便使用 仅保留 100 字节容量


返回：
数据被写入的位置，2字节，1字节 扇区位置+1字节扇区内ID

添加于 2020-0121


*****************************************************************************/

uint16_t Add_Flash_AD_TypeA_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			uint32_t i,it; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeA_Data_Len];  //缓存
	
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //开始前先清空缓存
			tab[i] =0;
	
			tab[0] = 0xAA;		//在开始写入数据头
	
			for(i =0;i<4;i++)  //将uid值载入 tab缓存
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			TypeA_Data_Num_1 =0;
			tab[5] = start_Unix>>24&0xFF;			//载入时间
			tab[6] = start_Unix>>16&0xFF;
			tab[7] = start_Unix>>8&0XFF;
			tab[8] = start_Unix&0xFF;
			
			tab[9]  =  stop_Unix>>24&0XFF;
			tab[10] =  stop_Unix>>16&0XFF;
			tab[11] =  stop_Unix>>8&0XFF;
			tab[12] =  stop_Unix&0xFF;
			
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
			if(TypeA_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeA_Data_Num_Error!\r\n");					//当读取到的数据值为 0XFFFFFF时，说明数量保存的数据结构异常，不再继续写入
				
				#endif
				
				return 0;
				
			}
			
			if(TypeA_Data_Num ==(AD_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		// 当数据量达到上限时，不再写入 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: AD_TypeA_Data_Write Full!\r\n");   //表示数据达到上限警告
				
				#endif
				
				return 0;
				
			}
			else if(TypeA_Data_Num >(AD_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		//  当数据量大于上限时，返回异常信息，且不再写入
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeA_Data_Write OverFlow!\r\n");   //表示数据超过上限错误
				
				#endif
				
				
				return 0;
			
			}
			
			//当一切就绪时开始尝试在第一个扇区写入数据
			
			//先在所有扇区寻找是否有一致的卡号，如果有，不变动位置，直接更新时间
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
			
				Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
						
						)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
					{
						//有相同的有效数据时，更新时间数据
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//返回 时间更新值
						
						#endif
						
						  //开始更新时间数据
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
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//返回 时间更新成功
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
							
							return it;   //返回 写入的位置信息
					
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
						
						//更新完时间数据后将修改后的数据写回原来扇区  操作完成
						
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeA_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //将新的数据量写入目录 由于没有增加数据，所以总量不变 可以不进行这项操作


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//返回 时间更新成功
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
						
						return it;   //返回 写入的位置信息
						
					}
					else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
										(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)	
									)
									{
										TypeA_Data_Num_1++;
										
										if(TypeA_Data_Num_1==TypeA_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
										{
											
											Dat_Num =A_Sector_TypeA_Data_Num;			//用于结束条件循环
											Sector_Num=AD_TypeA_Data_Sector_Num;   //用于结束扇区条件循环
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"AD_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//返回校验的 typeA 卡数量
											
											#endif
											
										}
									
									}
									
						if(TypeA_Data_Num ==0)   //没有保存数据时  直接结束循环
							{
									Dat_Num =A_Sector_TypeA_Data_Num;			//用于结束条件循环
									Sector_Num=AD_TypeA_Data_Sector_Num;   //用于结束扇区条件循环
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"AD_TypeA_Null!\r\n");   //表示空间无数据
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"AD_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//返回校验的 typeA 卡数量
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
		{
			
			Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //读取整个扇区数据
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	在扇区中寻找空闲的数据位置
				{

					
					if(
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]>A_Sector_TypeA_Data_Num)
						
						)		//当有一个扇区内的数据块空间空闲时，将新数据写入这个区块
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_AD_TypeA_Data!\r\n");
								
								#endif
							tab[13] = Dat_Num+1;  //在tab[13] 载入 空间序列号 用于写入
							
							for(i =0;i<TypeA_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeA_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //将新的数据量写入目录 由于增加了数据，所以需要更新

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeA_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[13];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_AD_TypeA_ID: %x ! \r\n",it);  //将新写入的ID号 输出到RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
}





/*****************************************************************************

擦除 管理员 A 卡 数据
清空 管理员 A 卡 扇区 数据


运行即清空 管理员 A 卡 新的写入程序必须使用这条函数擦除

添加于  2020-0121

******************************************************************************/

void Erase_AD_TypeA_Sector()
{
	uint32_t Sector_Num;		//扇区号
	uint32_t Block,Sector;  //读取Flash扇区所需变量
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_AD_TypeA_Sector!\r\n");
	
	#endif
	
	Sector_Num =AD_TypeA_Data_Secotr1;
	
	for(uint32_t i =0;i<AD_TypeA_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //获取偏移后待擦除扇区数据
						
		SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_AD_TypeA_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}



/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


写入一条用户A卡信息到 对应存储区域

数据载入格式：
客人卡：
0XAA+4字节A卡信息(序列号)+4字节起始时间+4字节过期时间 每条共计13字节
管理员卡：
0X55+4字节卡信息（序列号）+4字节起始时间+4字节过期时间 每条共计13字节

一个扇区可保存共计145条数据

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //保存B卡数据的扇区1 (目前共计1个扇区) 


const uint8_t TypeA_Data_Len = 13;  	//一条A卡数据的长度

const uint8_t TypeB_Data_Len = 17;		//一条B卡数据的长度

返回： 保存的 ID (序号)

*****************************************************************************/

//uint8_t Add_Flash_US_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
//{
//	
//			uint32_t len;  //定义总长度和号码
//			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
//			uint8_t tab[TypeA_Data_Len];  //缓存
//			
//			uint32_t is1,is2;
//			
//		for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
//			
//			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
//				tab_dat[i] =0;
//			
//			for(i =0;i<4;i++)  //将uid值载入 tab缓存
//			{
//					tab[i] = *uid;
//					uid++;
//			}
//			
//			uid-=i; //运算结束后恢复uid指针
//			len1 =0;
//			len1 =0;
//			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
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
//			len = TypeA_Data_Num * TypeA_Data_Len;  //得出占用的缓存数
//	
//			if(len>=TypeA_Data_Len)  //当len数量 大于等于一条 TypeA_Data 长度 既已保存有数据时
//			{
//				
//				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);  //根据扇区号获取块和起始页
//				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
//			
//				len1 =0; //开始时将len1 值置得与i一致
//				
//				for(i =0;i<len;)
//				{
//					if(tab_dat[i]==0XAA) //当头满足时
//					{
//						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])) //当本地有一致的uid时 将len1 设置为一致uid的起始地址
//						{
//							//len1 = i;
//							i = len;  //将i的值赋予len1 并通过将i的值置成len 以结束之后的循环
//						}
//						else
//						{
//							i+=TypeA_Data_Len;  //查询完一个uid后，切换到下一个起始地址
//							len1 = i;					//同时更新 len1 的值
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
//					&&(TypeA_Data_Num<A_Sector_TypeA_Data_Num)) //当len1等于len时，表示本地数据没有一致的卡信息
//				{
//					
//					for(i =4;i>=1;i--)   //将数据写入头
//						tab[i] = tab[i-1];
//					
//					tab[0] = 0xAA;  //写入头
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
//						for(is1 =1;is1<=TypeA_Data_Num;is1++)  //在一个扇区 Type A 卡数据总数内寻找可用的SN号
//						{
//							if(tab_dat[is2*TypeA_Data_Len-1]==is2)//当SN号存在时 结束循环
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
//						if(is2>A_Sector_TypeA_Data_Num)  //当 所有可用SN号用完时，结束写入操作，返回失败信息 
//							return 0;
//					}
//					
//					tab[TypeA_Data_Len-1] = is2;  //将获取到的 SN 号写入 tab 缓存 待写入
//					
//					#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"US_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
//					
//					#endif
//					
//					for(i = len1;i<(len1+TypeA_Data_Len);i++)
//					{
//						tab_dat[i] = tab[i-len1];  //将tab值载入到tab_dat中				
//					}
//					
//					if(tab_dat[len1]==0xAA)
//					{
//						
//							Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //擦除动态密码扇区数据
//								
//							SPIFlash_Erase_Sector(Block,Sector);
//							
//							SPI_Flash_Busy();
//							
//							Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//							
//							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeA_Data_Len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
//							SPI_Flash_Busy();
//							
//							Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //将新的数据量写入目录
//							
//							return tab[TypeA_Data_Len-1];
//					}
//					return 0;
//				}
//				else if(len1<=len-TypeA_Data_Len)  //当len1 小于 len 减 TypeA_Data_Len 说明卡ID号在本地已存在，只更新时间不增加数据总量
//					{
//						
//					if(tab_dat[len1]==0xAA)  //当数据包起始值满足条件时
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
//							Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //擦除动态密码扇区数据
//								
//							SPIFlash_Erase_Sector(Block,Sector);
//							
//							SPI_Flash_Busy();
//							
//							Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//							
//							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
//							SPI_Flash_Busy();
//							
//							return tab_dat[len1+TypeA_Data_Len-1];  //返回当前数据的SN号
//							
//						}
//						
//					}
//				else if (TypeA_Data_Num>=A_Sector_TypeA_Data_Num) //当存储的卡数据大于等于单个容量(一般情况下不可能大于)，不在写入数据
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
//			else if(len==0)  //当len值为空时，直接在第一个位置加入新的
//			{
//				  
//					for(i =4;i>=1;i--)   //将数据写入头
//						tab[i] = tab[i-1];
//					
//					tab[0] = 0xAA;  //写入头
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
//					tab[13] = 1; //载入第一个数据 因为没有其他数据设 SN号为1
//	
//					#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"US_TypeA_Data_SN: %d\r\n",tab[TypeA_Data_Len-1]);
//					
//					#endif
//					
//				for(i=0;i<(TypeA_Data_Len);i++)  //因为 len 值为 0 ， 所以直接在tab_dat 的起始位置写入第一条数据
//				{
//					
//					tab_dat[i] = tab[i];  //将 tab 值载入到 tab_dat 中
//					
//				}
//				
//				Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //擦除管理员密码扇区1
//				
//				SPIFlash_Erase_Sector(Block,Sector);
//				
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//				
//				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(US_TypeA_Data_Num_Code,1);  //将新的数据量写入目录
//				
//				return tab[TypeA_Data_Len-1];  //返回最后一字节， SN 号
//			}
//			
//			return 0;
//}


/*****************************************************************************

uint8_t Add_Flash_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


//写入一条用户A卡信息到 对应存储区域

//数据载入格式：
//客人卡：
//0XAA+4字节A卡信息(序列号)+4字节起始时间+4字节过期时间 每条共计13字节
//管理员卡：
//0X55+4字节卡信息（序列号）+4字节起始时间+4字节过期时间 每条共计13字节

//一个扇区可保存共计145条数据  为方便保存

写入一条用户A卡信息到 对应存储区域

数据载入格式：
客人卡：
0XAA+4字节A卡信息(序列号)+4字节起始时间+4字节过期时间+ 扇区内编序列号 每条共计14字节
管理员卡：
0XAA+4字节卡信息（序列号）+4字节起始时间+4字节过期时间+ 扇区内编序列号 每条共计14字节

一个扇区可保存共计145条数据 方便使用 仅保留 100 字节容量


const uint8_t AD_TypeA_Data_Secotr1 = 15;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //保存B卡数据的扇区1 (目前共计1个扇区) 


const uint8_t TypeA_Data_Len = 13;  	//一条A卡数据的长度

const uint8_t TypeB_Data_Len = 17;		//一条B卡数据的长度


返回：
数据被写入的位置，2字节，1字节 扇区位置+1字节扇区内ID

添加于 2020-0121

*****************************************************************************/

uint16_t Add_Flash_US_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
	
				uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			uint32_t i,it; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeA_Data_Len];  //缓存
	
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //开始前先清空缓存
			tab[i] =0;
	
			tab[0] = 0xAA;		//在开始写入数据头
	
			for(i =0;i<4;i++)  //将uid值载入 tab缓存
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			TypeA_Data_Num_1 =0;
			tab[5] = start_Unix>>24&0xFF;			//载入时间
			tab[6] = start_Unix>>16&0xFF;
			tab[7] = start_Unix>>8&0XFF;
			tab[8] = start_Unix&0xFF;
			
			tab[9]  =  stop_Unix>>24&0XFF;
			tab[10] =  stop_Unix>>16&0XFF;
			tab[11] =  stop_Unix>>8&0XFF;
			tab[12] =  stop_Unix&0xFF;
			
			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
			if(TypeA_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeA_Data_Num_Error!\r\n");					//当读取到的数据值为 0XFFFFFF时，说明数量保存的数据结构异常，不再继续写入
				
				#endif
				
				return 0;
				
			}
			
			if(TypeA_Data_Num ==(US_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		// 当数据量达到上限时，不再写入 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: US_TypeA_Data_Write Full!\r\n");   //表示数据达到上限警告
				
				#endif
				
				return 0;
				
			}
			else if(TypeA_Data_Num >(US_TypeA_Data_Sector_Num*A_Sector_TypeA_Data_Num))		//  当数据量大于上限时，返回异常信息，且不再写入
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeA_Data_Write OverFlow!\r\n");   //表示数据超过上限错误
				
				#endif
				
				
				return 0;
			
			}
			
			//当一切就绪时开始尝试在第一个扇区写入数据
			
			//先在所有扇区寻找是否有一致的卡号，如果有，不变动位置，直接更新时间
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
			
				Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
						
						)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
					{
						//有相同的有效数据时，更新时间数据
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//返回 时间更新值
						
						#endif
						
						  //开始更新时间数据
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
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//返回 时间更新成功
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
							
							return it;   //返回 写入的位置信息
					
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
						
						//更新完时间数据后将修改后的数据写回原来扇区  操作完成
						
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeA_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //将新的数据量写入目录 由于没有增加数据，所以总量不变 可以不进行这项操作


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//返回 时间更新成功
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeA_Data_Len)+13];
						
						return it;   //返回 写入的位置信息
						
					}
					else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
										(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)	
									)
									{
										TypeA_Data_Num_1++;
										
										if(TypeA_Data_Num_1==TypeA_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
										{
											
											Dat_Num =A_Sector_TypeA_Data_Num;			//用于结束条件循环
											Sector_Num=US_TypeA_Data_Sector_Num;   //用于结束扇区条件循环
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"US_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//返回校验的 typeA 卡数量
											
											#endif
											
										}
									
									}
									
						if(TypeA_Data_Num ==0)   //没有保存数据时  直接结束循环
							{
									Dat_Num =A_Sector_TypeA_Data_Num;			//用于结束条件循环
									Sector_Num=US_TypeA_Data_Sector_Num;   //用于结束扇区条件循环
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"US_TypeA_Null!\r\n");   //表示空间无数据
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"US_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//返回校验的 typeA 卡数量
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
		{
			
			Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //读取整个扇区数据
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	在扇区中寻找空闲的数据位置
				{

					
					if(
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]>A_Sector_TypeA_Data_Num)
						
						)		//当有一个扇区内的数据块空间空闲时，将新数据写入这个区块
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_US_TypeA_Data!\r\n");
								
								#endif
							tab[13] = Dat_Num+1;  //在tab[13] 载入 空间序列号 用于写入
							
							for(i =0;i<TypeA_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeA_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num+1);  //将新的数据量写入目录 由于增加了数据，所以需要更新

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeA_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[13];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_US_TypeA_ID: %x ! \r\n",it);  //将新写入的ID号 输出到RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
	
}



/*****************************************************************************

擦除 用户 A 卡 数据
清空 用户 A 卡 扇区 数据


运行即清空 用户 A 卡 新的写入程序必须使用这条函数擦除

添加于  2020-0121

******************************************************************************/

void Erase_US_TypeA_Sector()
{
	uint32_t Sector_Num;		//扇区号
	uint32_t Block,Sector;  //读取Flash扇区所需变量
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_US_TypeA_Sector!\r\n");
	
	#endif
	
	Sector_Num =US_TypeA_Data_Secotr1;
	
	for(uint32_t i =0;i<US_TypeA_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //获取偏移后待擦除扇区数据
						
		SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_US_TypeA_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}





/*****************************************************************************

uint8_t Add_Flash_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


写入一条管理员B卡信息到 对应存储区域

数据载入格式：
客人卡：
0XAA+4字节A卡信息(序列号)+4字节起始时间+4字节过期时间 + 1字节SN号 每条共计14字节
管理员卡：
0X55+4字节卡信息（序列号）+4字节起始时间+4字节过期时间 + 1字节SN号 每条共计14字节

一个扇区可保存共计145条数据

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //保存B卡数据的扇区1 (目前共计1个扇区)


const uint8_t TypeA_Data_Len = 13;  	//一条A卡数据的长度

const uint8_t TypeB_Data_Len = 17;		//一条B卡数据的长度


*****************************************************************************/
/*
uint8_t Add_Flash_AD_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeB_Data_Len];  //缓存
			
			uint32_t is1,is2;
			
		for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
				tab_dat[i] =0;
			
			for(i =0;i<8;i++)  //将uid值载入 tab缓存
			{
					tab[i] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			len1 =0;
			len1 =0;
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
			if(TypeB_Data_Num==0XFFFFFF)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_TypeB_Data_Num_Error!\r\n");
				
				#endif
				
				return 0;
				
			}
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //得出占用的缓存数
	
			if(len>=TypeB_Data_Len)  //当len数量 大于等于一条 TypeA_Data 长度 既已保存有数据时
			{
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);  //根据扇区号获取块和起始页
				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
			
				len1 =0; //开始时将len1 值置得与i一致
				
				for(i =0;i<len;)
				{
					if(tab_dat[i]==0XAA) //当头满足时
					{
						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])) //当本地有一致的uid时 将len1 设置为一致uid的起始地址
						{
							//len1 = i;
							i = len;  //将i的值赋予len1 并通过将i的值置成len 以结束之后的循环
						}
						else
						{
							i+=TypeB_Data_Len;  //查询完一个uid后，切换到下一个起始地址
							len1 = i;					//同时更新 len1 的值
						}
					}
					else
					{
						i = len;
						len1 =len;
					}
				}
				
				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
					&&(TypeB_Data_Num<A_Sector_TypeB_Data_Num)) //当len1等于len时，表示本地数据没有一致的卡信息
				{
					
					for(i =8;i>=1;i--)   //将数据写入头
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //写入头
					
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
						for(is1 =1;is1<=TypeB_Data_Num;is1++)  //在一个扇区 Type A 卡数据总数内寻找可用的SN号
						{
							if(tab_dat[is2*TypeB_Data_Len-1]==is2)//当SN号存在时 结束循环
							{
								break;
							}
							
						}
						
						if(is2>TypeB_Data_Num)
						{
							break;
						}
						
						if(is2>A_Sector_TypeB_Data_Num)  //当 所有可用SN号用完时，结束写入操作，返回失败信息 
							return 0;
					}
					
					tab[TypeB_Data_Len-1] = is2;  //将获取到的 SN 号写入 tab 缓存 待写入
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
					for(i = len1;i<(len1+TypeB_Data_Len);i++)
					{
						tab_dat[i] = tab[i-len1];  //将tab值载入到tab_dat中				
					}
					
					if(tab_dat[len1]==0xAA)
					{
						
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //擦除动态密码扇区数据
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeB_Data_Len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
							SPI_Flash_Busy();
							
							Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //将新的数据量写入目录
							
							return tab[TypeB_Data_Len-1];
					}
					return 0;
				}
				else if(len1<=len-TypeB_Data_Len)  //当len1 小于 len 减 TypeA_Data_Len 说明卡ID号在本地已存在，只更新时间不增加数据总量
					{
						
					if(tab_dat[len1]==0xAA)  //当数据包起始值满足条件时
						{
							
							tab_dat[len1+9] = start_Unix>>24&0xFF;
							tab_dat[len1+10] = start_Unix>>16&0xFF;
							tab_dat[len1+11] = start_Unix>>8&0XFF;
							tab_dat[len1+12] = start_Unix&0xFF;
							
							tab_dat[len1+13]  =  stop_Unix>>24&0XFF;
							tab_dat[len1+14] =  stop_Unix>>16&0XFF;
							tab_dat[len1+15] =  stop_Unix>>8&0XFF;
							tab_dat[len1+16] =  stop_Unix&0xFF;
							
							Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //擦除动态密码扇区数据
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
							SPI_Flash_Busy();
							
							return tab_dat[len1+TypeB_Data_Len-1];  //返回当前数据的SN号
							
						}
						
					}
				else if (TypeB_Data_Num>=A_Sector_TypeB_Data_Num) //当存储的卡数据大于等于单个容量(一般情况下不可能大于)，不在写入数据
				{
					
					#ifdef uart_start
					SEGGER_RTT_printf(0,"AD_TypeB_Data_Sector1 Full!\r\n");
					#endif
					
					return 0;
					
				}
				
			}
			else if(len==0)  //当len值为空时，直接在第一个位置加入新的
			{
				  
					for(i =8;i>=1;i--)   //将数据写入头
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //写入头
					
					tab[9] = start_Unix>>24&0xFF;
					tab[10] = start_Unix>>16&0xFF;
					tab[11] = start_Unix>>8&0XFF;
					tab[12] = start_Unix&0xFF;
					
					tab[13]  =  stop_Unix>>24&0XFF;
					tab[14] =  stop_Unix>>16&0XFF;
					tab[15] =  stop_Unix>>8&0XFF;
					tab[16] =  stop_Unix&0xFF;
					
					tab[17] = 1; //载入第一个数据 因为没有其他数据设 SN号为1
	
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
				for(i=0;i<(TypeB_Data_Len);i++)  //因为 len 值为 0 ， 所以直接在tab_dat 的起始位置写入第一条数据
				{
					
					tab_dat[i] = tab[i];  //将 tab 值载入到 tab_dat 中
					
				}
				
				Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //擦除管理员密码扇区1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(AD_TypeB_Data_Num_Code,1);  //将新的数据量写入目录
				
				return tab[TypeB_Data_Len-1];  //返回最后一字节， SN 号
			}
			
			return 0;
}
*/





uint16_t Add_Flash_AD_TypeB_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint32_t i,it; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeB_Data_Len];  //缓存
	
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //开始前先清空缓存
			tab[i] =0;
	
			tab[0] = 0xAA;		//在开始写入数据头
	
			for(i =0;i<8;i++)  //将uid值载入 tab缓存
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			TypeB_Data_Num_1 =0;
			tab[9] = start_Unix>>24&0xFF;			//载入时间
			tab[10] = start_Unix>>16&0xFF;
			tab[11] = start_Unix>>8&0XFF;
			tab[12] = start_Unix&0xFF;
			
			tab[13]  =  stop_Unix>>24&0XFF;
			tab[14] =  stop_Unix>>16&0XFF;
			tab[15] =  stop_Unix>>8&0XFF;
			tab[16] =  stop_Unix&0xFF;
			
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
			if(TypeB_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeB_Data_Num_Error!\r\n");					//当读取到的数据值为 0XFFFFFF时，说明数量保存的数据结构异常，不再继续写入
				
				#endif
				
				return 0;
				
			}
			
			if(TypeB_Data_Num ==(AD_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		// 当数据量达到上限时，不再写入 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: AD_TypeB_Data_Write Full!\r\n");   //表示数据达到上限警告
				
				#endif
				
				return 0;
				
			}
			else if(TypeB_Data_Num >(AD_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		//  当数据量大于上限时，返回异常信息，且不再写入
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: AD_TypeB_Data_Write OverFlow!\r\n");   //表示数据超过上限错误
				
				#endif
				
				
				return 0;
			
			}
			
			//当一切就绪时开始尝试在第一个扇区写入数据
			
			//先在所有扇区寻找是否有一致的卡号，如果有，不变动位置，直接更新时间
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
			
				Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&	// Type_B 卡有八字节ID
						(tab_dat[(Dat_Num*TypeB_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+5]==tab[5])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+6]==tab[6])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+7]==tab[7])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+8]==tab[8])&&
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
						
						)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
					{
						//有相同的有效数据时，更新时间数据
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//返回 时间更新值
						
						#endif
						
						  //开始更新时间数据
						if(			//当时间未发生变化时，不需擦写flash
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
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//返回 时间更新成功
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
							
							return it;   //返回 写入的位置信息
					
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
						
						//更新完时间数据后将修改后的数据写回原来扇区  操作完成
						
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeB_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //将新的数据量写入目录 由于没有增加数据，所以总量不变 可以不进行这项操作


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//返回 时间更新成功
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
						
						return it;   //返回 写入的位置信息
						
					}
					else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
										(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)	
									)
									{
										TypeB_Data_Num_1++;
										
										if(TypeB_Data_Num_1==TypeB_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
										{
											
											Dat_Num =A_Sector_TypeB_Data_Num;			//用于结束条件循环
											Sector_Num=AD_TypeB_Data_Sector_Num;   //用于结束扇区条件循环
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"AD_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//返回校验的 typeA 卡数量
											
											#endif
											
										}
									
									}
									
						if(TypeB_Data_Num ==0)   //没有保存数据时  直接结束循环
							{
									Dat_Num =A_Sector_TypeB_Data_Num;			//用于结束条件循环
									Sector_Num=AD_TypeB_Data_Sector_Num;   //用于结束扇区条件循环
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"AD_TypeB_Null!\r\n");   //表示空间无数据
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"AD_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//返回校验的 typeA 卡数量(现有的保存数量)
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
		{
			
			Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //读取整个扇区数据
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	在扇区中寻找空闲的数据位置
				{

					
					if(
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]>A_Sector_TypeB_Data_Num)
						
						)		//当有一个扇区内的数据块空间空闲时，将新数据写入这个区块
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_AD_TypeB_Data!\r\n");
								
								#endif
							tab[TypeB_Data_Len-1] = Dat_Num+1;  //在tab[13] 载入 空间序列号 用于写入
							
							for(i =0;i<TypeB_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(AD_TypeB_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //将新的数据量写入目录 由于增加了数据，所以需要更新

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[TypeB_Data_Len-1];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_AD_TypeB_ID: %x ! \r\n",it);  //将新写入的ID号 输出到RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
}

/*****************************************************************************

uint8_t Add_Flash_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)


写入一条用户B卡信息到 对应存储区域

数据载入格式：
客人卡：
0XAA+8字节B卡信息(序列号)+4字节起始时间+4字节过期时间 + 1字节SN号 每条共计18字节
管理员卡：
0XAA+8字节B卡信息（序列号）+4字节起始时间+4字节过期时间 + 1字节SN号 每条共计18字节

一个扇区可保存共计145条数据

const uint8_t AD_TypeA_Data_Secotr1 = 15;  //保存A卡数据的扇区1 (目前共计1个扇区)

const uint8_t AD_TypeB_Data_Secotr1 = 16;  //保存B卡数据的扇区1 (目前共计1个扇区)


const uint8_t TypeA_Data_Len = 13;  	//一条A卡数据的长度

const uint8_t TypeB_Data_Len = 17;		//一条B卡数据的长度


*****************************************************************************/
/*
uint8_t Add_Flash_US_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeB_Data_Len];  //缓存
			
			uint32_t is1,is2;
			
		for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
				tab_dat[i] =0;
			
			for(i =0;i<8;i++)  //将uid值载入 tab缓存
			{
					tab[i] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			len1 =0;
			len1 =0;
			
			TypeB_Data_Num = Get_Cat_Dat ( US_TypeB_Data_Num_Code ); // 先获取本地已存在的TypeA 卡数据量
			
				if( TypeB_Data_Num==0XFFFFFF )
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_Num_Error!\r\n");
					
					#endif
					
					return 0;
					
				}
				
			len = TypeB_Data_Num * TypeB_Data_Len;  //得出占用的缓存数
	
			if(len>=TypeB_Data_Len)  //当len数量 大于等于一条 TypeA_Data 长度 既已保存有数据时
			{
				
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);  //根据扇区号获取块和起始页
				SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
			
				len1 =0; //开始时将len1 值置得与i一致
				
				for(i =0;i<len;)
				{
					if(tab_dat[i]==0XAA) //当头满足时
					{
						if((tab_dat[i+1]==tab[0])&&(tab_dat[i+2]==tab[1])&&(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
							(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])) //当本地有一致的uid时 将len1 设置为一致uid的起始地址
						{
							//len1 = i;
							i = len;  //将i的值赋予len1 并通过将i的值置成len 以结束之后的循环
						}
						else
						{
							i+=TypeB_Data_Len;  //查询完一个uid后，切换到下一个起始地址
							len1 = i;					//同时更新 len1 的值
						}
					}
					else
					{
						i = len;
						len1 =len;
					}
				}
				
				if((len1==len)//&&(len1<=(len1-TypeA_Data_Len))
					&&(TypeB_Data_Num<A_Sector_TypeB_Data_Num)) //当len1等于len时，表示本地数据没有一致的卡信息
				{
					
					for(i =8;i>=1;i--)   //将数据写入头
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //写入头
					
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
						for(is1 =1;is1<=TypeB_Data_Num;is1++)  //在一个扇区 Type A 卡数据总数内寻找可用的SN号
						{
							if(tab_dat[is2*TypeB_Data_Len-1]==is2)//当SN号存在时 结束循环
							{
								break;
							}
							
						}
						
						if(is2>TypeB_Data_Num)
						{
							break;
						}
						
						if(is2>A_Sector_TypeB_Data_Num)  //当 所有可用SN号用完时，结束写入操作，返回失败信息 
							return 0;
					}
					
					tab[TypeB_Data_Len-1] = is2;  //将获取到的 SN 号写入 tab 缓存 待写入
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
					for(i = len1;i<(len1+TypeB_Data_Len);i++)
					{
						tab_dat[i] = tab[i-len1];  //将tab值载入到tab_dat中				
					}
					
					if(tab_dat[len1]==0xAA)
					{
						
							Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //擦除动态密码扇区数据
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len+TypeB_Data_Len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
							SPI_Flash_Busy();
							
							Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //将新的数据量写入目录
							
							return tab[TypeB_Data_Len-1];
					}
					return 0;
				}
				else if(len1<=len-TypeB_Data_Len)  //当len1 小于 len 减 TypeA_Data_Len 说明卡ID号在本地已存在，只更新时间不增加数据总量
					{
						
					if(tab_dat[len1]==0xAA)  //当数据包起始值满足条件时
						{
							
							tab_dat[len1+9] = start_Unix>>24&0xFF;
							tab_dat[len1+10] = start_Unix>>16&0xFF;
							tab_dat[len1+11] = start_Unix>>8&0XFF;
							tab_dat[len1+12] = start_Unix&0xFF;
							
							tab_dat[len1+13]  =  stop_Unix>>24&0XFF;
							tab_dat[len1+14] =  stop_Unix>>16&0XFF;
							tab_dat[len1+15] =  stop_Unix>>8&0XFF;
							tab_dat[len1+16] =  stop_Unix&0xFF;
							
							Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //擦除动态密码扇区数据
								
							SPIFlash_Erase_Sector(Block,Sector);
							
							SPI_Flash_Busy();
							
							Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
							
							SPIFlash_Write_Tab(tab_dat,Block,Page,len); //写入修改后的数据到动态密码扇区 （由于卡ID号已存在，故管理员TypeA 卡总量不发生改变）
							SPI_Flash_Busy();
							
							return tab_dat[len1+TypeB_Data_Len-1];  //返回当前数据的SN号
							
						}
						
					}
				else if (TypeB_Data_Num>=A_Sector_TypeB_Data_Num) //当存储的卡数据大于等于单个容量(一般情况下不可能大于)，不在写入数据
				{
					
					#ifdef uart_start
					SEGGER_RTT_printf(0,"US_TypeB_Data_Sector1 Full!\r\n");
					#endif
					
					return 0;
					
				}
				
			}
			else if(len==0)  //当len值为空时，直接在第一个位置加入新的
			{
				  
					for(i =8;i>=1;i--)   //将数据写入头
						tab[i] = tab[i-1];
					
					tab[0] = 0xAA;  //写入头
					
					tab[9] = start_Unix>>24&0xFF;
					tab[10] = start_Unix>>16&0xFF;
					tab[11] = start_Unix>>8&0XFF;
					tab[12] = start_Unix&0xFF;
					
					tab[13]  =  stop_Unix>>24&0XFF;
					tab[14] =  stop_Unix>>16&0XFF;
					tab[15] =  stop_Unix>>8&0XFF;
					tab[16] =  stop_Unix&0xFF;
					
					tab[17] = 1; //载入第一个数据 因为没有其他数据设 SN号为1
	
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_SN: %d\r\n",tab[TypeB_Data_Len-1]);
					
					#endif
					
				for(i=0;i<(TypeB_Data_Len);i++)  //因为 len 值为 0 ， 所以直接在tab_dat 的起始位置写入第一条数据
				{
					
					tab_dat[i] = tab[i];  //将 tab 值载入到 tab_dat 中
					
				}
				
				Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //擦除管理员密码扇区1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(US_TypeB_Data_Num_Code,1);  //将新的数据量写入目录
				
				return tab[TypeB_Data_Len-1];  //返回最后一字节， SN 号
			}
			
			return 0;
			
}
*/



uint16_t Add_Flash_US_TypeB_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix)
{
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint32_t i,it; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[TypeB_Data_Len];  //缓存
	
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //开始前先清空缓存
			tab[i] =0;
	
			tab[0] = 0xAA;		//在开始写入数据头
	
			for(i =0;i<8;i++)  //将uid值载入 tab缓存
			{
					tab[i+1] = *uid;
					uid++;
			}
			
			uid-=i; //运算结束后恢复uid指针
			TypeB_Data_Num_1 =0;
			tab[9] = start_Unix>>24&0xFF;			//载入时间
			tab[10] = start_Unix>>16&0xFF;
			tab[11] = start_Unix>>8&0XFF;
			tab[12] = start_Unix&0xFF;
			
			tab[13]  =  stop_Unix>>24&0XFF;
			tab[14] =  stop_Unix>>16&0XFF;
			tab[15] =  stop_Unix>>8&0XFF;
			tab[16] =  stop_Unix&0xFF;
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
			if(TypeB_Data_Num==0XFFFFFF)			
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeB_Data_Num_Error!\r\n");					//当读取到的数据值为 0XFFFFFF时，说明数量保存的数据结构异常，不再继续写入
				
				#endif
				
				return 0;
				
			}
			
			if(TypeB_Data_Num ==(US_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		// 当数据量达到上限时，不再写入 
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Warning: US_TypeB_Data_Write Full!\r\n");   //表示数据达到上限警告
				
				#endif
				
				return 0;
				
			}
			else if(TypeB_Data_Num >(US_TypeB_Data_Sector_Num*A_Sector_TypeB_Data_Num))		//  当数据量大于上限时，返回异常信息，且不再写入
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Error: US_TypeB_Data_Write OverFlow!\r\n");   //表示数据超过上限错误
				
				#endif
				
				
				return 0;
			
			}
			
			//当一切就绪时开始尝试在第一个扇区写入数据
			
			//先在所有扇区寻找是否有一致的卡号，如果有，不变动位置，直接更新时间
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
			
				Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
				{
					if(
						
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&	// Type_B 卡有八字节ID
						(tab_dat[(Dat_Num*TypeB_Data_Len)+1]==tab[1])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+2]==tab[2])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+3]==tab[3])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+4]==tab[4])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+5]==tab[5])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+6]==tab[6])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+7]==tab[7])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+8]==tab[8])&&
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
						
						)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
					{
						//有相同的有效数据时，更新时间数据
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time!\r\n");			//返回 时间更新值
						
						#endif
						
						  //开始更新时间数据
						if(			//当时间未发生变化时，不需擦写flash
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
							
							SEGGER_RTT_printf(0,"Time_OK!\r\n");			//返回 时间更新成功
							
							#endif
							
							it =Sector_Num;
							it<<=8;
							it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
							
							return it;   //返回 写入的位置信息
					
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
						
						//更新完时间数据后将修改后的数据写回原来扇区  操作完成
						
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeB_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
					//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num);  //将新的数据量写入目录 由于没有增加数据，所以总量不变 可以不进行这项操作


						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Updat_Time_OK!\r\n");			//返回 时间更新成功
						
						#endif
					}
						it =Sector_Num;
						it<<=8;
						it|=tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1];
						
						return it;   //返回 写入的位置信息
						
					}
					else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
										(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
										(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)	
									)
									{
										TypeB_Data_Num_1++;
										
										if(TypeB_Data_Num_1==TypeB_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
										{
											
											Dat_Num =A_Sector_TypeB_Data_Num;			//用于结束条件循环
											Sector_Num=US_TypeB_Data_Sector_Num;   //用于结束扇区条件循环
											
											#ifdef uart_start
											
											SEGGER_RTT_printf(0,"US_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//返回校验的 typeA 卡数量
											
											#endif
											
										}
									
									}
									
						if(TypeB_Data_Num ==0)   //没有保存数据时  直接结束循环
							{
									Dat_Num =A_Sector_TypeB_Data_Num;			//用于结束条件循环
									Sector_Num=US_TypeB_Data_Sector_Num;   //用于结束扇区条件循环
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"US_TypeB_Null!\r\n");   //表示空间无数据
								
								#endif
								
							}
					
				}
				

				
			}
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"US_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//返回校验的 typeA 卡数量(现有的保存数量)
			
			#endif
	
			
		for(uint8_t Sector_Num =0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
		{
			
			Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
			
			SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //读取整个扇区数据
		
			
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	在扇区中寻找空闲的数据位置
				{

					
					if(
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]!=tab[0])||
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]>A_Sector_TypeB_Data_Num)
						
						)		//当有一个扇区内的数据块空间空闲时，将新数据写入这个区块
						{
							
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Add_A_US_TypeB_Data!\r\n");
								
								#endif
							tab[TypeB_Data_Len-1] = Dat_Num+1;  //在tab[13] 载入 空间序列号 用于写入
							
							for(i =0;i<TypeB_Data_Len;i++)
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] = tab[i];
								
							}
							
						Sector_to_Block_Sector(&Block,&Sector,(US_TypeB_Data_Secotr1+Sector_Num));  //获取偏移后待擦除扇区数据
						
						SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
						
						SPI_Flash_Busy();
						
						Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
						
						Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num+1);  //将新的数据量写入目录 由于增加了数据，所以需要更新

					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_Add_OK!\r\n");
					
					#endif
							
						it = Sector_Num;
						it<<=8;
						it|=tab[TypeB_Data_Len-1];
						
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"New_US_TypeB_ID: %x ! \r\n",it);  //将新写入的ID号 输出到RTT 
						
						#endif
						
						
						return it;
						
						}
					
					
				}
		}	
			
	
    	return 0;
}



/***********************************************

功能：
删除指定 编号 管理员 Type_A 卡数据

输入：
		AD_Type_A_SN		对应的 管理员 Type_A 卡数据位置

输出：

 1 删除成功
 0 删除失败

***********************************************/

//uint8_t Del_AD_Type_A(uint8_t AD_Type_A_SN)
//{
//	
//			uint32_t len;  //定义总长度和号码
//			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
////			uint8_t tab[50];  //缓存

////			uint32_t is1,is2;
//	
////			for(i =0;i<sizeof(tab);i++)
////				tab[i] =0;
////			
//			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
//				tab_dat[i] =0;
//	
//			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
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
//			len = TypeA_Data_Num * TypeA_Data_Len;  //得出占用的缓存数
//			
//			Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);  //根据扇区号获取块和起始页
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
//			
//			len1 =0; //开始时将len1 值置得与i一致
//			
//			for(i =TypeA_Data_Len;i<=len;i+=TypeA_Data_Len)
//			{
//				if(tab_dat[i-1]==AD_Type_A_SN)  //当 这条数据的 SN 号 与 要删除的目标 SN 号一致时
//				{
//				
//					break;
//				
//				}				
//			}
//			
//			if(i>len)  //当搜索的区域已大于本地保存数据的范围 返回 0 表示没有 SN 一致的数据
//			{
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"AD_Type_A_SN_Full!\r\n");  //提示 无一致 SN 号
//				
//				#endif
//				
//				return 0;
//			
//			}
//			
//			//当存在一致的 SN 时
//			
//			len1 =i;  //将 i 值赋给 len1
//			
//			#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"tab_dat_AD_Type_A_SN:%d\r\n",tab_dat[len1-1]);
//			
//			#endif
//			
//			//用后面的数据覆盖 对应 SN号的数据
//			/********************************************************************************
//			备注 即使最后一条数据仍然在缓存中，
//			但由于在写入Flash 时长度减少一条字节长度，写入后再次读取Flash时最后一条数据不复存在
//			********************************************************************************/
//			
//			for(i =len1;i<len;i++)
//				tab_dat[i-TypeA_Data_Len] = tab_dat[i];
//			
//				
//				Sector_to_Block_Sector(&Block,&Sector,AD_TypeA_Data_Secotr1);  //擦除管理员密码扇区1
//				
//				SPIFlash_Erase_Sector(Block,Sector);
//				
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);
//				
//				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeA_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num-1);  //将新的数据量写入目录
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

功能：
删除指定 编号 管理员 Type_A 卡数据

输入：
		TypeA_ID		对应的 管理员 Type_A 卡 ID 号指针

输出：

 1 删除成功
 0 删除失败
 2 TypeA 卡号不存在
 3 无数据
 
 NO.202002
 
***********************************************/

uint8_t Del_AD_Type_A(uint8_t *TypeA_ID)
{
			uint32_t Block,Page;  //读取Flash扇区所需变量
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			
			uint8_t tab[5];
			
			TypeA_Data_Num_1 =0;
			
			tab[0] = 0xAA;		//先将数据头载入
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeA_ID;
					
					TypeA_ID++;
			
			}
			
			TypeA_ID-=sizeof(tab);		//恢复指针
			
			
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nAD_Type_A_Num: %d !\r\n",TypeA_Data_Num);		//输出当前保存的数据量
				
				SEGGER_RTT_printf(0,"Start_Cark_AD_Type_A_ID !\r\n");		//开始寻找需要删除的 Type A 卡ID
				
				#endif
			
			if(TypeA_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_Type_A_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // 当无数据时返回空
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
				
				Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
				{
						if(
							(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
							
							
							)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_A_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeA_Data_Len)+TypeA_Data_Len-1]);		//相同值的 ID 号
										
						#endif
									
							for(uint8_t i=0;i<TypeA_Data_Len;i++)		//清空该区域内 有效数据空间 再写入
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] =0;		//	通过清零对应位置数据再次写入来实现数据消除
								
							}
							//将修改后的数据写入 Flash 通过清零的方式避免擦除过程 （测试用）
							
						Sector_to_Block_Page(&Block,&Page,(AD_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(AD_TypeA_Data_Num_Code,TypeA_Data_Num-1);    //写入新的数据到目录
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nAD_Type_A_Num: %d !\r\n\r\n",TypeA_Data_Num-1);

						#endif
						
						return 1;
							
						}
						else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
					
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
							)
							{
								TypeA_Data_Num_1++;
								
								if(TypeA_Data_Num_1==TypeA_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
								{
									
									Dat_Num =A_Sector_TypeA_Data_Num;			//用于结束条件循环
									Sector_Num=AD_TypeA_Data_Sector_Num;   //用于结束扇区条件循环
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! AD_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//返回校验的 typeA 卡数量
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // 默认返回失败
}



///***********************************************

//功能：
//删除指定 编号 用户 Type_A 卡数据

//输入：
//		US_Type_A_SN		对应的 用户 Type_A 卡数据位置

//输出：

// 1 删除成功
// 0 删除失败

//***********************************************/

//uint8_t Del_US_Type_A(uint8_t US_Type_A_SN)
//{

//			uint32_t len;  //定义总长度和号码
//			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
////			uint8_t tab[50];  //缓存

////			uint32_t is1,is2;
//	
////			for(i =0;i<sizeof(tab);i++)
////				tab[i] =0;
//			
//			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
//				tab_dat[i] =0;
//	
//			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
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
//			len = TypeA_Data_Num * TypeA_Data_Len;  //得出占用的缓存数
//			
//			Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);  //根据扇区号获取块和起始页
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
//			
//			len1 =0; //开始时将len1 值置得与i一致
//			
//			for(i =TypeA_Data_Len;i<=len;i+=TypeA_Data_Len)
//			{
//				if(tab_dat[i-1]==US_Type_A_SN)  //当 这条数据的 SN 号 与 要删除的目标 SN 号一致时
//				{
//				
//					break;
//				
//				}				
//			}
//			
//			if(i>len)  //当搜索的区域已大于本地保存数据的范围 返回 0 表示没有 SN 一致的数据
//			{
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"US_Type_A_SN_Full!\r\n");  //提示 无一致 SN 号
//				
//				#endif
//				
//				return 0;
//			
//			}
//			
//			//当存在一致的 SN 时
//			
//			len1 =i;  //将 i 值赋给 len1
//			
//			#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"tab_dat_US_Type_A_SN:%d\r\n",tab_dat[len1-1]);
//			
//			#endif
//			
//			//用后面的数据覆盖 对应 SN号的数据
//			/********************************************************************************
//			备注 即使最后一条数据仍然在缓存中，
//			但由于在写入Flash 时长度减少一条字节长度，写入后再次读取Flash时最后一条数据不复存在
//			********************************************************************************/
//			
//			for(i =len1;i<len;i++)
//				tab_dat[i-TypeA_Data_Len] = tab_dat[i];
//			
//				
//				Sector_to_Block_Sector(&Block,&Sector,US_TypeA_Data_Secotr1);  //擦除管理员密码扇区1
//				
//				SPIFlash_Erase_Sector(Block,Sector);
//				
//				SPI_Flash_Busy();
//				
//				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1);
//				
//				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeA_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
//				SPI_Flash_Busy();
//				
//				Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num-1);  //将新的数据量写入目录
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

功能：
删除指定 编号 管理员 Type_A 卡数据

输入：
		TypeA_ID		对应的 管理员 Type_A 卡 ID 号 指针

输出：

 1 删除成功
 0 删除失败
 2 TypeA 卡号不存在
 3 无数据
 
 NO.202002
 
***********************************************/

uint8_t Del_US_Type_A(uint8_t *TypeA_ID)
{
			uint32_t Block,Page;  //读取Flash扇区所需变量
			uint32_t TypeA_Data_Num,TypeA_Data_Num_1;
			
			uint8_t tab[5];
			
			TypeA_Data_Num_1 =0;
			
			tab[0] = 0xAA;		//先将数据头载入
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeA_ID;
					
					TypeA_ID++;
			
			}
			
			TypeA_ID-=sizeof(tab);		//恢复指针
			
			
			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nUS_Type_A_Num: %d !\r\n",TypeA_Data_Num);		//输出当前保存的数据量
				
				SEGGER_RTT_printf(0,"Start_Cark_US_Type_A_ID !\r\n");		//开始寻找需要删除的 Type A 卡ID
				
				#endif
			
			if(TypeA_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"US_Type_A_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // 当无数据时返回空
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
				
				Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeA_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
				{
						if(
							(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+1]==tab[1])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+2]==tab[2])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+3]==tab[3])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+4]==tab[4])&&
							(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
							
							
							)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_A_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeA_Data_Len)+TypeA_Data_Len-1]);		//相同值的 ID 号
										
						#endif
									
							for(uint8_t i=0;i<TypeA_Data_Len;i++)		//清空该区域内 有效数据空间 再写入
							{
								
								tab_dat[(Dat_Num*TypeA_Data_Len)+i] =0;		//	通过清零对应位置数据再次写入来实现数据消除
								
							}
							//将修改后的数据写入 Flash 通过清零的方式避免擦除过程 （测试用）
							
						Sector_to_Block_Page(&Block,&Page,(US_TypeA_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeA_Data_Len*A_Sector_TypeA_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(US_TypeA_Data_Num_Code,TypeA_Data_Num-1);    //写入新的数据到目录
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nUS_Type_A_Num: %d !\r\n\r\n",TypeA_Data_Num-1);

						#endif
						
						return 1;
							
						}
						else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
					
						(tab_dat[(Dat_Num*TypeA_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeA_Data_Len)+(TypeA_Data_Len-1)]<=A_Sector_TypeA_Data_Num)
						
							)
							{
								TypeA_Data_Num_1++;
								
								if(TypeA_Data_Num_1==TypeA_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
								{
									
									Dat_Num =A_Sector_TypeA_Data_Num;			//用于结束条件循环
									Sector_Num=US_TypeA_Data_Sector_Num;   //用于结束扇区条件循环
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! US_TypeA_Cheak_Num: %d !\r\n",TypeA_Data_Num_1);			//返回校验的 typeA 卡数量
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // 默认返回失败
	
	
}

/***********************************************

功能：
删除指定 编号 管理员 Type_B 卡数据

输入：
		AD_Type_B_SN		对应的 管理员 Type_B 卡数据位置

输出：

 1 删除成功
 0 删除失败

***********************************************/
/*
uint8_t Del_AD_Type_B(uint8_t AD_Type_B_SN)
{
	
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
//			uint8_t tab[50];  //缓存

//			uint32_t is1,is2;
	
//			for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
			
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
				tab_dat[i] =0;
	
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
	
			if(TypeB_Data_Num==0XFFFFFF)
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"AD_TypeB_Data_Num_Error!\r\n");
					
					#endif
					
					return 0;
					
				}
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //得出占用的缓存数
			
			Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);  //根据扇区号获取块和起始页
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
			
			len1 =0; //开始时将len1 值置得与i一致
			
			for(i =TypeB_Data_Len;i<=len;i+=TypeB_Data_Len)
			{
				if(tab_dat[i-1]==AD_Type_B_SN)  //当 这条数据的 SN 号 与 要删除的目标 SN 号一致时
				{
				
					break;
				
				}				
			}
			
			if(i>len)  //当搜索的区域已大于本地保存数据的范围 返回 0 表示没有 SN 一致的数据
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_Type_B_SN_Full!\r\n");  //提示 无一致 SN 号
				
				#endif
				
				return 0;
			
			}
			
			//当存在一致的 SN 时
			
			len1 =i;  //将 i 值赋给 len1
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"tab_dat_AD_Type_B_SN:%d\r\n",tab_dat[len1-1]);
			
			#endif
			
			//用后面的数据覆盖 对应 SN号的数据
		//	*******************************************************************************
		//	备注 即使最后一条数据仍然在缓存中，
		//	但由于在写入Flash 时长度减少一条字节长度，写入后再次读取Flash时最后一条数据不复存在
		//	*******************************************************************************
			
			for(i =len1;i<len;i++)
				tab_dat[i-TypeB_Data_Len] = tab_dat[i];
			
				
				Sector_to_Block_Sector(&Block,&Sector,AD_TypeB_Data_Secotr1);  //擦除管理员密码扇区1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeB_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num-1);  //将新的数据量写入目录
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Del_A_AD_TypeB_Card!\r\n");
				
				#endif
				
				return 1;
				
}
*/


uint8_t Del_AD_Type_B(uint8_t *TypeB_ID)
{
			uint32_t Block,Page;  //读取Flash扇区所需变量
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint8_t tab[9];
			
			TypeB_Data_Num_1 =0;
				
			for(uint8_t i;i<sizeof(tab);i++)			//清空缓存
			{
				tab[i] =0x00;
			}
	
			
			tab[0] = 0xAA;		//先将数据头载入
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeB_ID;
					
					TypeB_ID++;
			
			}
			
			TypeB_ID-=sizeof(tab);		//恢复指针
			
			
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nAD_Type_B_Num: %d !\r\n",TypeB_Data_Num);		//输出当前保存的数据量
				
				SEGGER_RTT_printf(0,"Start_Cark_AD_Type_B_ID !\r\n");		//开始寻找需要删除的 Type A 卡ID
				
				#endif
			
			if(TypeB_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"AD_Type_B_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // 当无数据时返回空
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
				
				Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
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
							
							
							)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_B_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1]);		//相同值的 ID 号
										
						#endif
									
							for(uint8_t i=0;i<TypeB_Data_Len;i++)		//清空该区域内 有效数据空间 再写入
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] =0;		//	通过清零对应位置数据再次写入来实现数据消除
								
							}
							//将修改后的数据写入 Flash 通过清零的方式避免擦除过程 （测试用）
							
						Sector_to_Block_Page(&Block,&Page,(AD_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(AD_TypeB_Data_Num_Code,TypeB_Data_Num-1);    //写入新的数据到目录
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nAD_Type_B_Num: %d !\r\n\r\n",TypeB_Data_Num-1); //返回更新后的数据数量

						#endif
						
						return 1;
							
						}
						else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
							)
							{
								TypeB_Data_Num_1++;
								
								if(TypeB_Data_Num_1==TypeB_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
								{
									
									Dat_Num =A_Sector_TypeB_Data_Num;			//用于结束条件循环
									Sector_Num=AD_TypeB_Data_Sector_Num;   //用于结束扇区条件循环
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! AD_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//返回校验的 typeA 卡数量
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // 默认返回失败
}




/***********************************************

功能：
删除指定 编号 用户 Type_B 卡数据

输入：
		AD_Type_B_SN		对应的 管理员 Type_B 卡数据位置

输出：

 1 删除成功
 0 删除失败

***********************************************/
/*
uint8_t Del_US_Type_B(uint8_t US_Type_B_SN)
{
	
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num;
			uint32_t i,len1; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
//			uint8_t tab[50];  //缓存

//			uint32_t is1,is2;
	
//			for(i =0;i<sizeof(tab);i++)
//				tab[i] =0;
//			
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
				tab_dat[i] =0;
	
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
	
			if(TypeB_Data_Num==0XFFFFFF)
				{
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"US_TypeB_Data_Num_Error!\r\n");
					
					#endif
					
					return 0;
					
				}
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //得出占用的缓存数
			
			Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);  //根据扇区号获取块和起始页
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
			
			len1 =0; //开始时将len1 值置得与i一致
			
			for(i =TypeB_Data_Len;i<=len;i+=TypeB_Data_Len)
			{
				if(tab_dat[i-1]==US_Type_B_SN)  //当 这条数据的 SN 号 与 要删除的目标 SN 号一致时
				{
				
					break;
				
				}				
			}
			
			if(i>len)  //当搜索的区域已大于本地保存数据的范围 返回 0 表示没有 SN 一致的数据
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"US_Type_B_SN_Full!\r\n");  //提示 无一致 SN 号
				
				#endif
				
				return 0;
			
			}
			
			//当存在一致的 SN 时
			
			len1 =i;  //将 i 值赋给 len1
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"tab_dat_US_Type_B_SN:%d\r\n",tab_dat[len1-1]);
			
			#endif
			
			//用后面的数据覆盖 对应 SN号的数据
		//	********************************************************************************
		//	备注 即使最后一条数据仍然在缓存中，
		//	但由于在写入Flash 时长度减少一条字节长度，写入后再次读取Flash时最后一条数据不复存在
		//	********************************************************************************
			
			for(i =len1;i<len;i++)
				tab_dat[i-TypeB_Data_Len] = tab_dat[i];
			
				
				Sector_to_Block_Sector(&Block,&Sector,US_TypeB_Data_Secotr1);  //擦除管理员密码扇区1
				
				SPIFlash_Erase_Sector(Block,Sector);
				
				SPI_Flash_Busy();
				
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1);
				
				SPIFlash_Write_Tab(tab_dat,Block,Page,len-TypeB_Data_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
				SPI_Flash_Busy();
				
				Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num-1);  //将新的数据量写入目录
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"Del_A_US_TypeB_Card!\r\n");
				
				#endif
				
				return 1;
				
}
*/

uint8_t Del_US_Type_B(uint8_t *TypeB_ID)
{
			uint32_t Block,Page;  //读取Flash扇区所需变量
			uint32_t TypeB_Data_Num,TypeB_Data_Num_1;
			uint8_t tab[9];
			
			TypeB_Data_Num_1 =0;
				
			for(uint8_t i;i<sizeof(tab);i++)			//清空缓存
			{
				tab[i] =0x00;
			}
	
			
			tab[0] = 0xAA;		//先将数据头载入
	
			for(uint8_t i=0;i<sizeof(tab);i++)
			{
					
					tab[i+1] = *TypeB_ID;
					
					TypeB_ID++;
			
			}
			
			TypeB_ID-=sizeof(tab);		//恢复指针
			
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
			
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nUS_Type_B_Num: %d !\r\n",TypeB_Data_Num);		//输出当前保存的数据量
				
				SEGGER_RTT_printf(0,"Start_Cark_US_Type_B_ID !\r\n");		//开始寻找需要删除的 Type A 卡ID
				
				#endif
			
			if(TypeB_Data_Num==0)
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"US_Type_B_Data_Num =0!\r\n");
				
				#endif
				
				
				return 3;  // 当无数据时返回空
			}
			
			
			for(uint8_t Sector_Num =0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)				//Sector_Num  表示偏移的扇区数
			{
				
				Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));  //根据偏移后的扇区号获取块和起始页
				
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));							  //读取整个扇区数据
				
				for(uint8_t Dat_Num =0;Dat_Num<A_Sector_TypeB_Data_Num;Dat_Num++)			//	在有效数据中寻找是否有一致的ID
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
							
							
							)		//当有一个扇区内的数据块内容是否有效，且与待写入值一致时  更新数据
						{
								
						#ifdef uart_start
				
							SEGGER_RTT_printf(0,"Cheak_Type_B_Num: %d !\r\n",
															 tab_dat[(Dat_Num*TypeB_Data_Len)+TypeB_Data_Len-1]);		//相同值的 ID 号
										
						#endif
									
							for(uint8_t i=0;i<TypeB_Data_Len;i++)		//清空该区域内 有效数据空间 再写入
							{
								
								tab_dat[(Dat_Num*TypeB_Data_Len)+i] =0;		//	通过清零对应位置数据再次写入来实现数据消除
								
							}
							//将修改后的数据写入 Flash 通过清零的方式避免擦除过程 （测试用）
							
						Sector_to_Block_Page(&Block,&Page,(US_TypeB_Data_Secotr1+Sector_Num));
						
						SPIFlash_Write_Tab(tab_dat,Block,Page,TypeB_Data_Len*A_Sector_TypeB_Data_Num); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
						SPI_Flash_Busy();
							
						Ev_Cat_Dat(US_TypeB_Data_Num_Code,TypeB_Data_Num-1);    //写入新的数据到目录
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nUS_Type_B_Num: %d !\r\n\r\n",TypeB_Data_Num-1); //返回更新后的数据数量

						#endif
						
						return 1;
							
						}
						else if(			// 对其中有效数据进行统计 数据量达到了即停止 搜索  节约写入时间
					
						(tab_dat[(Dat_Num*TypeB_Data_Len)+0]==tab[0])&&
						(tab_dat[(Dat_Num*TypeB_Data_Len)+(TypeB_Data_Len-1)]<=A_Sector_TypeB_Data_Num)
						
							)
							{
								TypeB_Data_Num_1++;
								
								if(TypeB_Data_Num_1==TypeB_Data_Num)  //当判断的数据量与保存的一致时，结束这次循环
								{
									
									Dat_Num =A_Sector_TypeB_Data_Num;			//用于结束条件循环
									Sector_Num=US_TypeB_Data_Sector_Num;   //用于结束扇区条件循环
									
									#ifdef uart_start
									
									SEGGER_RTT_printf(0,"Warning: Null_ID! US_TypeB_Cheak_Num: %d !\r\n",TypeB_Data_Num_1);			//返回校验的 typeA 卡数量
									
									#endif
									
									return 2;
									
								}
							
							}
				}
				
				
			}
	
	return 0;   // 默认返回失败
}


/**********************************************************************************

uint32_t Need_TypeA_Data_SN

判断 本地 是否有 相同的 A 卡数据

 输入： Need_Uid

 4字节 TypeA UID 号指针 

返回： 当 UID 号不存在时 返回 0 存在时返回 TypeA卡SN号
取用 uint32_t 类型 

最高位四字节 1 时表示这个为 管理员 最高位四字节为 2 时表示这个为 用户 

**********************************************************************************/

/*当数据有效时用于保存 本地有效数据 的有效时间 第一字节用于表示 是否有效。

01 表示 有效 管理A 卡时间 
02 表示 有效 用户A 卡时间 
03 表示 有效 管理B 卡时间 
04 表示 有效 用户B 卡时间 

其他无效
*/
//uint8_t TypeAB_Unix_T[9];

//uint32_t Need_TypeA_Data_SN(uint8_t *Need_Uid)
//{
//			uint32_t len;  //定义总长度和号码
//			uint32_t Block,Page;  //读取Flash扇区所需变量
//			
//			uint32_t TypeA_Data_Num;
//			uint32_t i; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
//			uint8_t tab[50];  //缓存

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
//			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
//	
//			
//			len = TypeA_Data_Num * TypeA_Data_Len;  //得出占用的缓存数
//			
//			Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1);  //根据扇区号获取块和起始页
//			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
//			
//			i =0;
//			
//			for(i =0;i<len;i+=TypeA_Data_Len)
//			{
//				
//				if(
//					
//					(tab_dat[i]==0xAA)&&																//判断头正确时
//					(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
//					(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])
//				
//					)
//						//当有一致 管理员 数据在本地保存时
//					
//					{
//						
//						TypeAB_Unix_T[0] = 0x01;		//表示这张卡是管理A 卡 表示时间戳指向的卡型
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
//							//当前为 管理员 TypeA 卡 故最高八位加上1
//						
//						return tab_dat[i+TypeA_Data_Len-1]+0x1000000; //返回 有效的 管理员 SN 号
//						
//					}
//				
//			}
//			
//			
//			//当管理员卡数据中不存在时 尝试寻找用户卡区域
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
//					//当本地有一致 用户 数据在本地保存时
//				
//				{
//						
//						
//						TypeAB_Unix_T[0] = 0x01;   //表示这张卡是用户A 卡 表示时间戳指向的卡型
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
//						//当前为 用户 Type A 卡 故最高八位加上2
//					
//					
//					return tab_dat[i+TypeA_Data_Len-1]+0x2000000;		//返回 有效的 用户 SN 号
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

寻找数据扇区内 是否有一致的 ID  

输入：  需要寻找的ID号

返回：  ID 数据对应的 SN号 当 未找到 正确 ID号时，返回 0 之类的无效数据

当返回有效 SN 号时  说明一致的ID 号保存在 tab_dat 缓存 的 第 SN 号 位置

所以如果在还需要判断数据时，不能改动或覆盖 tab_dat 数据区域内容



************************************************/

uint8_t TypeAB_Unix_T[9];

/***********************

当数据有效时用于保存 本地有效数据 的有效时间 第一字节用于表示 是否有效。

01 表示 有效 管理A 卡时间 
02 表示 有效 用户A 卡时间 
03 表示 有效 管理B 卡时间 
04 表示 有效 用户B 卡时间 

其他无效

***********************/




uint32_t Need_TypeA_Data_SN(uint8_t *Need_Uid)
{
//			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page;  //读取Flash扇区所需变量
			
			uint32_t TypeA_Data_Num;
			uint32_t Cheak_Card_Num;		//校对的 卡片数量
			uint32_t i; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[50];  //缓存

			for(i =0;i<sizeof(tab);i++)			//清空数据缓存
				tab[i] =0;
			
			for(i =0;i<sizeof(TypeAB_Unix_T);i++)    //清空时间缓存
				TypeAB_Unix_T[i] =0;
	
			for(i =0;i<4;i++)				// 将需要寻找的 Uid 数据载入到 tab 缓存  
			{
				
					tab[i] = *Need_Uid;
					Need_Uid++;
				
			}
			
			TypeA_Data_Num = Get_Cat_Dat(AD_TypeA_Data_Num_Code);			//获取 管理员 TypeA 数据
			Cheak_Card_Num =0;
			//开始判断本地是否存在一致的管理员A卡
			
			for(uint8_t Sector_Num=0;Sector_Num<AD_TypeA_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,AD_TypeA_Data_Secotr1+Sector_Num);  //根据扇区号获取块和起始页  读取的数据扇区等于 AD_TypeA_Data_Sector1 加 Sector_Num 偏移位置
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));					//读取指定扇区的数据
				
				i =0;
				
				for(i =0;i<(TypeA_Data_Len*A_Sector_TypeA_Data_Num);i+=TypeA_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(TypeA_Data_Num>0)		//确定有数据存在		
						
						)		
					// 当有一致的 管理员A卡数据时
					{
						  TypeAB_Unix_T[0] = 0X01;		//表示这张卡是 管理员 A 卡 表示时间戳指向的卡型
						  
							TypeAB_Unix_T[1] = tab_dat[i+5];
							
							TypeAB_Unix_T[2] = tab_dat[i+6];
							
							TypeAB_Unix_T[3] = tab_dat[i+7];
							
							TypeAB_Unix_T[4] = tab_dat[i+8];
							
							TypeAB_Unix_T[5] = tab_dat[i+9];
							
							TypeAB_Unix_T[6] = tab_dat[i+10];
							
							TypeAB_Unix_T[7] = tab_dat[i+11];
							
							TypeAB_Unix_T[8] = tab_dat[i+12];
							
							
								//当前为 管理员 TypeA 卡 故最高八位加上1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_AD_TypeA_Card!\r\n");		//表示获取到一致的 管理员 A 卡 数据
						SEGGER_RTT_printf(0,"AD_TypeA_Sector: 0x%x!\r\n",AD_TypeA_Data_Secotr1+Sector_Num);  //数据所在的管理员 A 卡扇区
						SEGGER_RTT_printf(0,"AD_TypeA_ID: 0x%x! \r\n\r\n",tab_dat[i+13]);   //数据在管理员 A 卡扇区内的编号
						
						
						#endif
							
							return tab_dat[i+TypeA_Data_Len-1]+0x1000000; //返回 有效的 管理员 SN 号
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //遇到一个有效数据  校验数据 加一
					
					if(Cheak_Card_Num>=TypeA_Data_Num)		//当校验的数据 达到上限时  结束这次循环
					{
						i = TypeA_Data_Len*A_Sector_TypeA_Data_Num;		// 销毁循环条件实现介绍搜索
						Sector_Num=AD_TypeA_Data_Sector_Num;
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n Cheak_AD_TypeA_Card_Ok! \r\n\r\n");
						
						#endif
						
					}
					
				}
				
				
			}
			
			//开始判断是否存在一致的用户A卡
			
			TypeA_Data_Num = Get_Cat_Dat(US_TypeA_Data_Num_Code);			//获取 管理员 TypeA 数据
			Cheak_Card_Num =0;
			//开始判断本地是否存在一致的管理员A卡
			
			for(uint8_t Sector_Num=0;Sector_Num<US_TypeA_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,US_TypeA_Data_Secotr1+Sector_Num);  //根据扇区号获取块和起始页  读取的数据扇区等于 AD_TypeA_Data_Sector1 加 Sector_Num 偏移位置
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeA_Data_Len*A_Sector_TypeA_Data_Num));					//读取指定扇区的数据
				
				i =0;
				
				for(i =0;i<(TypeA_Data_Len*A_Sector_TypeA_Data_Num);i+=TypeA_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(TypeA_Data_Num>0)
						)		
					// 当有一致的 管理员A卡数据时
					{
						  TypeAB_Unix_T[0] = 0X02;		//表示这张卡是 用户 A 卡 表示时间戳指向的卡型
						  
							TypeAB_Unix_T[1] = tab_dat[i+5];
							
							TypeAB_Unix_T[2] = tab_dat[i+6];
							
							TypeAB_Unix_T[3] = tab_dat[i+7];
							
							TypeAB_Unix_T[4] = tab_dat[i+8];
							
							TypeAB_Unix_T[5] = tab_dat[i+9];
							
							TypeAB_Unix_T[6] = tab_dat[i+10];
							
							TypeAB_Unix_T[7] = tab_dat[i+11];
							
							TypeAB_Unix_T[8] = tab_dat[i+12];
							
							
								//当前为 管理员 TypeA 卡 故最高八位加上1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_US_TypeA_Card!\r\n");		//表示获取到一致的 管理员 A 卡 数据
						SEGGER_RTT_printf(0,"US_TypeA_Sector: 0x%x!\r\n",US_TypeA_Data_Secotr1+Sector_Num);  //数据所在的管理员 A 卡扇区
						SEGGER_RTT_printf(0,"US_TypeA_ID: 0x%x! \r\n\r\n",tab_dat[i+13]);   //数据在管理员 A 卡扇区内的编号
						
						
						#endif
							
							return tab_dat[i+TypeA_Data_Len-1]+0x2000000; //返回 有效的 用户A SN 号
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //遇到一个有效数据  校验数据 加一
					
					if(Cheak_Card_Num>=TypeA_Data_Num)		//当校验的数据 达到上限时  结束这次循环
					{
						i = TypeA_Data_Len*A_Sector_TypeA_Data_Num;		// 销毁循环条件实现介绍搜索
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

判断 本地 是否有 相同的 B 卡数据

 输入： Need_Uid

 4字节 TypeB UID 号指针 

返回： 当 UID 号不存在时 返回 0 存在时返回 TypeB卡SN号
取用 uint32_t 类型 

最高位四字节 3 时表示这个为 管理员 最高位四字节为 4 时表示这个为 用户 

**********************************************************************************/

/*
uint32_t Need_TypeB_Data_SN(uint8_t *Need_Uid)
{
			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num;
			uint32_t i; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[50];  //缓存

			for(i =0;i<sizeof(tab);i++)
				tab[i] =0;
			
			for(i =0;i<sizeof(TypeAB_Unix_T);i++)
				TypeAB_Unix_T[i] =0;
	
			for(i =0;i<8;i++)  //将 TypeB 卡 UID 号载入缓存
			{
				
					tab[i] = *Need_Uid;
					Need_Uid++;
				
			}
		
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code); // 先获取本地已存在的TypeA 卡数据量
	
			
			len = TypeB_Data_Num * TypeB_Data_Len;  //得出占用的缓存数
			
			Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1);  //根据扇区号获取块和起始页
			SPIFlash_Read_Tab(tab_dat,Block,Page,len);							  //读取与现有数据量等长的扇区数据
			
			i =0;
			
			for(i =0;i<len;i+=TypeB_Data_Len)
			{
				
				if(
					
					(tab_dat[i]==0xAA)&&																//判断头正确时
					(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
					(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
					(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
					(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])
					
					)
					
						//当有一致 管理员 数据在本地保存时
					
					{
						
						TypeAB_Unix_T[0] = 0x03;		//表示这张卡是管理B 卡 表示时间戳指向的卡型
						
						TypeAB_Unix_T[1] = tab_dat[i+9];
						
						TypeAB_Unix_T[2] = tab_dat[i+10];
						
						TypeAB_Unix_T[3] = tab_dat[i+11];
						
						TypeAB_Unix_T[4] = tab_dat[i+12];
						
						TypeAB_Unix_T[5] = tab_dat[i+13];
						
						TypeAB_Unix_T[6] = tab_dat[i+14];
						
						TypeAB_Unix_T[7] = tab_dat[i+15];
						
						TypeAB_Unix_T[8] = tab_dat[i+16];
						
						
							//当前为 管理员 TypeB 卡 故最高八位加上1
						
						return tab_dat[i+TypeB_Data_Len-1]+0x3000000; //返回 有效的 管理员 SN 号
						
					}
				
			}
			
			
			//当管理员卡数据中不存在时 尝试寻找用户卡区域
			
			
			
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
				
					//当本地有一致 用户 数据在本地保存时
				
				{
						
						
						TypeAB_Unix_T[0] = 0x04;   //表示这张卡是用户 B 卡 表示时间戳指向的卡型
						
						TypeAB_Unix_T[1] = tab_dat[i+9];
						
						TypeAB_Unix_T[2] = tab_dat[i+10];
						
						TypeAB_Unix_T[3] = tab_dat[i+11];
						
						TypeAB_Unix_T[4] = tab_dat[i+12];
						
						TypeAB_Unix_T[5] = tab_dat[i+13];
						
						TypeAB_Unix_T[6] = tab_dat[i+14];
						
						TypeAB_Unix_T[7] = tab_dat[i+15];
						
						TypeAB_Unix_T[8] = tab_dat[i+16];
					
					
						//当前为 用户 Type A 卡 故最高八位加上2
					
					
					return tab_dat[i+TypeB_Data_Len-1]+0x4000000;		//返回 有效的 用户 SN 号
					
					
				}
				
			}
			
			return 0;
			
}*/



uint32_t Need_TypeB_Data_SN(uint8_t *Need_Uid)
{
//			uint32_t len;  //定义总长度和号码
			uint32_t Block,Page;  //读取Flash扇区所需变量
			
			uint32_t TypeB_Data_Num;
			uint32_t Cheak_Card_Num;		//校对的 卡片数量
			uint32_t i; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[50];  //缓存

			for(i =0;i<sizeof(tab);i++)			//清空数据缓存
				tab[i] =0;
			
			for(i =0;i<sizeof(TypeAB_Unix_T);i++)    //清空时间缓存
				TypeAB_Unix_T[i] =0;
	
			for(i =0;i<8;i++)				// 将需要寻找的 Uid 数据载入到 tab 缓存  
			{
				
					tab[i] = *Need_Uid;
					Need_Uid++;
				
			}
			
			TypeB_Data_Num = Get_Cat_Dat(AD_TypeB_Data_Num_Code);			//获取 管理员 TypeA 数据
			Cheak_Card_Num =0;
			//开始判断本地是否存在一致的管理员A卡
			
			for(uint8_t Sector_Num=0;Sector_Num<AD_TypeB_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,AD_TypeB_Data_Secotr1+Sector_Num);  //根据扇区号获取块和起始页  读取的数据扇区等于 AD_TypeA_Data_Sector1 加 Sector_Num 偏移位置
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));					//读取指定扇区的数据
				
				i =0;
				
				for(i =0;i<(TypeB_Data_Len*A_Sector_TypeB_Data_Num);i+=TypeB_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
						(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])&&
						(TypeB_Data_Num>0)		//确定有数据存在
						)		
					// 当有一致的 管理员B卡数据时
					{
						  TypeAB_Unix_T[0] = 0X03;		//表示这张卡是 管理员 B 卡 表示时间戳指向的卡型
						  
							TypeAB_Unix_T[1] = tab_dat[i+9];
							
							TypeAB_Unix_T[2] = tab_dat[i+10];
							
							TypeAB_Unix_T[3] = tab_dat[i+11];
							
							TypeAB_Unix_T[4] = tab_dat[i+12];
							
							TypeAB_Unix_T[5] = tab_dat[i+13];
							
							TypeAB_Unix_T[6] = tab_dat[i+14];
							
							TypeAB_Unix_T[7] = tab_dat[i+15];
							
							TypeAB_Unix_T[8] = tab_dat[i+16];
							
							
								//当前为 管理员 TypeA 卡 故最高八位加上1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_AD_TypeB_Card!\r\n");		//表示获取到一致的 管理员 A 卡 数据
						SEGGER_RTT_printf(0,"AD_TypeB_Sector: 0x%x!\r\n",AD_TypeB_Data_Secotr1+Sector_Num);  //数据所在的管理员 A 卡扇区
						SEGGER_RTT_printf(0,"AD_TypeB_ID: 0x%x! \r\n\r\n",tab_dat[i+TypeB_Data_Len-1]);   //数据在管理员 A 卡扇区内的编号
						
						
						#endif
							
							return tab_dat[i+TypeB_Data_Len-1]+0x3000000; //返回 有效的 管理员B SN 号
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //遇到一个有效数据  校验数据 加一
					
					if(Cheak_Card_Num>=TypeB_Data_Num)		//当校验的数据 达到上限时  结束这次循环
					{
						i = TypeB_Data_Len*A_Sector_TypeB_Data_Num;		// 销毁循环条件实现介绍搜索
						Sector_Num=AD_TypeB_Data_Sector_Num;
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n Cheak_AD_TypeB_Card_Ok! \r\n\r\n");
						
						#endif
						
					}
					
				}
				
				
			}
			
			//开始判断是否存在一致的用户B卡
			
			TypeB_Data_Num = Get_Cat_Dat(US_TypeB_Data_Num_Code);			//获取 管理员 TypeB 数据
			Cheak_Card_Num =0;
			//开始判断本地是否存在一致的管理员B卡
			
			for(uint8_t Sector_Num=0;Sector_Num<US_TypeB_Data_Sector_Num;Sector_Num++)
			{
				Sector_to_Block_Page(&Block,&Page,US_TypeB_Data_Secotr1+Sector_Num);  //根据扇区号获取块和起始页  读取的数据扇区等于 AD_TypeA_Data_Sector1 加 Sector_Num 偏移位置
				SPIFlash_Read_Tab(tab_dat,Block,Page,(TypeB_Data_Len*A_Sector_TypeB_Data_Num));					//读取指定扇区的数据
				
				i =0;
				
				for(i =0;i<(TypeB_Data_Len*A_Sector_TypeB_Data_Num);i+=TypeB_Data_Len)
				{
					
					if(
						
						(tab_dat[i]==0xAA)&&
						(tab_dat[i+1] == tab[0])&&(tab_dat[i+2]==tab[1])&&
						(tab_dat[i+3]==tab[2])&&(tab_dat[i+4]==tab[3])&&
						(tab_dat[i+5]==tab[4])&&(tab_dat[i+6]==tab[5])&&
						(tab_dat[i+7]==tab[6])&&(tab_dat[i+8]==tab[7])&&
						(TypeB_Data_Num>0)	//确定有数据存在
						)		
					// 当有一致的 管理员B卡数据时
					{
						  TypeAB_Unix_T[0] = 0X04;		//表示这张卡是 用户 B 卡 表示时间戳指向的卡型
						  
							TypeAB_Unix_T[1] = tab_dat[i+9];
							
							TypeAB_Unix_T[2] = tab_dat[i+10];
							
							TypeAB_Unix_T[3] = tab_dat[i+11];
							
							TypeAB_Unix_T[4] = tab_dat[i+12];
							
							TypeAB_Unix_T[5] = tab_dat[i+13];
							
							TypeAB_Unix_T[6] = tab_dat[i+14];
							
							TypeAB_Unix_T[7] = tab_dat[i+15];
							
							TypeAB_Unix_T[8] = tab_dat[i+16];
							
							
								//当前为 用户 TypeB 卡 故最高八位加上1
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\nNeed_US_TypeB_Card!\r\n");		//表示获取到一致的 管理员 A 卡 数据
						SEGGER_RTT_printf(0,"US_TypeB_Sector: 0x%x!\r\n",US_TypeB_Data_Secotr1+Sector_Num);  //数据所在的管理员 A 卡扇区
						SEGGER_RTT_printf(0,"US_TypeB_ID: 0x%x! \r\n\r\n",tab_dat[i+TypeB_Data_Len-1]);   //数据在管理员 A 卡扇区内的编号
						
						
						#endif
							
							return tab_dat[i+TypeB_Data_Len-1]+0x4000000; //返回 有效的 用户B SN 号
							
					}
						else if(tab_dat[i]==0xAA)
							Cheak_Card_Num++;   //遇到一个有效数据  校验数据 加一
					
					if(Cheak_Card_Num>=TypeB_Data_Num)		//当校验的数据 达到上限时  结束这次循环
					{
						i = TypeB_Data_Len*A_Sector_TypeB_Data_Num;		// 销毁循环条件实现介绍搜索
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

擦除 管理员 B 卡 数据
清空 管理员 B 卡 扇区 数据


运行即清空 用户 A 卡 新的写入程序必须使用这条函数擦除

添加于  2020-0121

******************************************************************************/

void Erase_AD_TypeB_Sector()
{
	uint32_t Sector_Num;		//扇区号
	uint32_t Block,Sector;  //读取Flash扇区所需变量
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_AD_TypeB_Sector!\r\n");
	
	#endif
	
	Sector_Num =AD_TypeB_Data_Secotr1;
	
	for(uint32_t i =0;i<AD_TypeB_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //获取偏移后待擦除扇区数据
						
		SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_AD_TypeB_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}

/*****************************************************************************

擦除 管理员 B 卡 数据
清空 管理员 B 卡 扇区 数据


运行即清空 用户 A 卡 新的写入程序必须使用这条函数擦除

添加于  2020-0121

******************************************************************************/

void Erase_US_TypeB_Sector()
{
	uint32_t Sector_Num;		//扇区号
	uint32_t Block,Sector;  //读取Flash扇区所需变量
	
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nStart_Erast_US_TypeB_Sector!\r\n");
	
	#endif
	
	Sector_Num =US_TypeB_Data_Secotr1;
	
	for(uint32_t i =0;i<US_TypeB_Data_Sector_Num;i++)
	{
		
		Sector_to_Block_Sector(&Block,&Sector,(Sector_Num+i));  //获取偏移后待擦除扇区数据
						
		SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erast_US_TypeB_Sector_Num: %d!\r\n",Sector_Num+i);
		
		#endif
		
		
	}
}

/**********************************************
把指纹数据写入Flash

输入：
PS_Type: 指纹类型/来源（管理员 用户 动态密码）
管理员： 0X01
用户：   0X02

来自动态密码： 0x03

由于这部分较其他功能相对松散，故此程序自带 Flash 开始和结束


**********************************************/

uint16_t Add_Flash_PS_Print (uint16_t PS_Type,uint16_t PS_ID,uint32_t start_Unix,uint32_t stop_Unix)
{
	
			uint32_t Block,Page,Sector;  //读取Flash扇区所需变量
			
			uint32_t PS_Print_Data_Num,PS_Print_Data_Num_1;
			uint32_t i; //len1 用于标识输入的uid是否已存在 len1<len 时 表示uid已存在，len1=len 时 表示uid不存在
			uint8_t tab[PS_Print_Len];  //缓存
			
			uint16_t PS_Print_Act;		//来自动态密码的数据删减标志  有效时，写入结束总数不加1
	
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
			
			PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//获取 指纹的 数据数量
			
			for(i =0;i<sizeof(tab_dat);i++)  //开始前先清空缓存
			tab_dat[i] =0;
	
			for(i =0;i<sizeof(tab);i++)  //开始前先清空缓存
			tab[i] =0;
			 
		//	0x55+2字节指纹类型+2字节指纹ID+4字节起始实际+4字节失效实际
			
			tab[0] = 0x55;		//在开始写入数据头
			
			tab[1] = PS_Type>>8&0xFF;
			tab[2] = PS_Type&0xFF;
			
			tab[3] = PS_ID>>8&0xFF;
			tab[4] = PS_ID&0XFF;
			
			tab[5] = start_Unix>>24&0xFF;			//载入时间
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
			
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //根据偏移后的扇区号获取块和起始页
				
			SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //读取整个扇区数据
			
			
			if((tab[3]==0)&&(tab[4]<=PS_Print_Num));			//判断输入的 ID 号是否在范围内
			else 
			{
				#ifdef uart_start
				SEGGER_RTT_printf(0,"PS_ID: %x!\r\n",PS_ID);
				SEGGER_RTT_printf(0,"PS_IDH: %x!\r\n",tab[3]);
				SEGGER_RTT_printf(0,"PS_IDL: %x!\r\n",tab[4]);
				
				#endif
				SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
				Spi_Flash_Sleep ();
				
				return 0;		//不在范围内则直接结束
			}
			PS_Print_Data_Num_1 = tab[4];		// 需要保存到的 ID
			
			
			if((tab[2]==0x00)&&		//当输入的 ID 类型为 来自动态密码时
				(tab[3]==0x03))
			{
				
				for(i =0;i<PS_Print_Num;i++)		//寻找本地是否存在其他
				{
					if( (tab_dat[i*PS_Print_Len]==0x55)&&
							(tab_dat[i*PS_Print_Len+1]==0x00)&&
							(tab_dat[i*PS_Print_Len+2]==0x03)
						
						)
					{
						
							tab_dat[i*PS_Print_Len] =0;
							tab_dat[i*PS_Print_Len+1] =0;
							tab_dat[i*PS_Print_Len+2] =0;		//清除当前存在的  动态密码类型指纹数据
							PS_Print_Act =1;		// 清除过动态密码类型 指纹  标志
//							PS_Print_Act_ID = tab_dat[i*PS_Print_Len+4];			//载入删除的 指纹ID
						
					}
					
				}
				
			}
			
					//将新数据写入  tab_dat
			
			for(i =0;i<PS_Print_Len;i++)			//往 tab_dat 写入新的数据
			{
				
				tab_dat[PS_Print_Data_Num_1*PS_Print_Len+i] = tab[i];
				
			}
			
			//将新数据写入对应Flash扇区
			
			Sector_to_Block_Sector(&Block,&Sector,PS_Print_Soctor1);  //获取待擦除扇区数据		
			SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据			
			SPI_Flash_Busy();
			
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);
			
			SPIFlash_Write_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len); //写入修改后的数据到动态密码扇区 （由于增加了一条数据，故总长度应加 TypeA_Data_Secotr1）
			SPI_Flash_Busy();
			
			
			if(PS_Print_Act==0)		//当未删除动态密码指纹数据时，动态密码 + 1
				Ev_Cat_Dat(PS_Print_Num_Code,PS_Print_Data_Num+1);    //写入新的数据到目录
			
			
			if(PS_Print_Act==1)			//当删除过动态密码来源的指纹时 删除指纹头内保存的指纹 且需要确保 运行此函数时指纹头已上电
			{
					
					
			}
		
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nAdd_Flash_PS_Print_OK!\r\n");
			
			#endif
			
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep ();
			
		return 1;		// 完成后 return 1;
}

/***************************************
获取闲置的 指纹头存储 ID

输入：  无

输出：  空闲的 指纹 ID  没有空闲ID 时，返回 0XFFFF

由于这部分较其他功能相对松散，故此程序自带 Flash 开始和结束

*****************************************/

uint16_t Need_Of_Ps_Print_Data_IDLE_ID()
{
		
		uint32_t Block,Page;  //读取Flash扇区所需变量
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
	
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//获取 指纹的 数据数量
			
		if(PS_Print_Data_Num>=PS_Print_Num)   //当存储容量满时，结束运作
		{
			#ifdef uart_start
			
				SEGGER_RTT_printf(0,"\r\nPS_Print_Data_ID_Full!\r\n");
			
			#endif
			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep ();
			
			return 0XFFFF;
		}
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //根据偏移后的扇区号获取块和起始页		
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //读取整个扇区数据	
		
		
		for(i =0;i<PS_Print_Num;i++)
		{
			if(tab_dat[i*PS_Print_Len]==0x55)
			{
				;
			}
			else
			{
				
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\nNeed_PS_Print_Data_ID: %x!\r\n",i);			//输出获取到的值
				
				#endif
				
				SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
				Spi_Flash_Sleep ();

				
				return i;			//获取到空闲的存储空间，返回空间ID			
			}
		}
		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep ();
		
		
		return 0xFFFF;
		
}


/********************************************

擦除指纹扇区

此函数没有配置自主开启 SPI FLASH 的能力
需在SPI FLASH 开启时使用

*********************************************/

void Erase_PS_Print_Sector()
{

		uint32_t Block,Sector;  //读取Flash扇区所需变量
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Erase_PS_Print_Sector!\r\n");
		
		#endif
		
		
		Sector_to_Block_Sector(&Block,&Sector,PS_Print_Soctor1);  //获取偏移后待擦除扇区数据
		
		SPI_Flash_Busy();
		SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erase_PS_Print_Sector_Num: %d!\r\n",PS_Print_Soctor1);
		
		#endif
		
}


/*****************************************
判断指纹是否存在（有效）
如果存在，判断时间是否过期

输入： 指纹 ID

输出： 0：不存在 (或无效)

输出的 32 bit 数据  高16位为指纹类型  低16位为指纹ID(与输入值一致)
此函数有自主开启 SPI FLASH 能力
// 保存类型	0x55+2字节指纹类型+2字节指纹ID+4字节起始实际+4字节失效实际
****************************************/

uint32_t Opinion_PS_Print_Valid(uint16_t PS_Print_ID)
{
		uint32_t Block,Page;  //读取Flash扇区所需变量
		uint32_t i; 
		uint32_t PS_Print_Data_Num,PS_Print_Data_Num_1;
			
		uint32_t PS_Start_Unix,PS_Stop_Unix;		//读取到的本地指纹 UNIX 时间，用于比对
		uint32_t return_dat;		//预设返回值
	
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
		
		Get_Time(); //获取板载时间
		
		if(TIME_STATE==true)
		{
			;
		}
		else
		{			
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep ();
			
			return 0XFFFF;
		}
		
		Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
						
		
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//获取 指纹的 数据数量
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //根据偏移后的扇区号获取块和起始页		
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //读取整个扇区数据	
		
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
				 (tab_dat[i*PS_Print_Len+4] == (PS_Print_ID&0XFF))				//判断ID 是否一致
				)
			{
				
				
				PS_Start_Unix = tab_dat[i*PS_Print_Len+5];
				PS_Start_Unix <<= 8;	//左移八位
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
				
				if((Unix_T>PS_Start_Unix)&&(Unix_T<PS_Stop_Unix)) //当当前指纹ID的时间有效时
				{
					return_dat = tab_dat[i*PS_Print_Len+1];
					return_dat <<= 8;
					return_dat |= tab_dat[i*PS_Print_Len+2];
					return_dat <<= 8;
					return_dat |= tab_dat[i*PS_Print_Len+3];
					return_dat <<= 8;
					return_dat |= tab_dat[i*PS_Print_Len+4];
					
					//返回值为 16字节 指纹类型+16字节指纹ID
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Return_Dat: %x！\r\n",return_dat);		//输出当前的返回值
					
					#endif
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep ();
					
					return return_dat;			//将合适对应指纹的数据返回
				}
				else				//当时间无效时
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"PS_Print_Time_Invalid!\r\n");
					
					#endif
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep ();
					
					return 0XFFFF;
					
				}
				
			}
			else if(tab_dat[i*PS_Print_Len]==0xAA)
			{
				PS_Print_Data_Num_1++;
				
				if(PS_Print_Data_Num_1>=PS_Print_Data_Num)	//当加入判断的有效数据量达到已有数据时，加入
				{
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep ();
					
					return 0XFFFF;
				}
			}
			
		}
		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep ();
		
		
		return 0XFFFF;
		
}


/**********************************************

删除某一类型的指纹
输入：PS_print_Type 指纹类型
输出： 无
此函数没有自主开启 SPI FLASH 能力
*********************************************/

void  Del_A_Type_PS_Print(uint16_t PS_Print_Type)
{
		uint32_t Block,Page;  //读取Flash扇区所需变量
		uint32_t PS_Print_Data_Num;
		uint32_t Del_PS_Print_Data_Num;		//删除的数据量
		uint32_t i;
		uint8_t PS_Type1,PS_Type2;		// 用于对比寻找一致的类
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Del_A_Type_PS_Print!\r\n");	
		
		#endif	
	
	
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//获取 指纹的 数据数量
		
		if((PS_Print_Data_Num==0)||(PS_Print_Data_Num>PS_Print_Num))
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Ps_Print_Data_Num: %x .\r\n",PS_Print_Num);
			
			#endif
			
			//数量有错误或没有数据时抛掉这次运行
			
			return;
		}
		
		//数量正常开始下一步
		
		//将类型分两字节保存到缓存
		
		PS_Type1 = PS_Print_Type>>8&0xFF;
		PS_Type2 = PS_Print_Type&0xFF;
		Del_PS_Print_Data_Num =0;
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //根据偏移后的扇区号获取块和起始页
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //读取整个扇区数据
		
		
		for(i =0;i<PS_Print_Num;i++)			//寻找一致的类型
		{
			if((tab_dat[i*PS_Print_Len]==0x55)&&
				 (tab_dat[i*PS_Print_Len+1]==PS_Type1)&&
				 (tab_dat[i*PS_Print_Len+2]==PS_Type2)			//判断类型一致
				)
				{
					for(uint8_t it;it<PS_Print_Len;it++)		//清空需要删除的数据
					{
						
						tab_dat[i*PS_Print_Len+it] = 0;
					
					}
					
					Del_PS_Print_Data_Num++;
					if(Del_PS_Print_Data_Num>PS_Print_Data_Num)		//当删除数据量大于已有数据量时，配置为一样大。（一般不会发生）
					{
						
						Del_PS_Print_Data_Num = PS_Print_Data_Num;
						break;		//同时跳出这个循环
						
					}		
				}
		}
		
		if(Del_PS_Print_Data_Num==PS_Print_Data_Num)
		{
			//当删除数据量与本地拥有的数据量一致时对所有缓存进行清空
			
			for(i =0;i<(PS_Print_Num*PS_Print_Len);i++)
			{
				tab_dat[i] =0;		//清空缓存
			}
		}
		
			Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);
			
			SPIFlash_Write_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len); //将新数据写入
			SPI_Flash_Busy();
			
		
			Ev_Cat_Dat(PS_Print_Num_Code,(PS_Print_Data_Num-Del_PS_Print_Data_Num));    //写入新的数据到目录
			
			if((PS_Print_Data_Num-Del_PS_Print_Data_Num)==0)
			{
				//当本地指纹数量为0时触发清空指纹头指令
				
					CL_PS_ID	 =0;			//清空所有指纹信息
					CL_PS_Run = CL_PS_Run_Del_All_Mode;				//触发标志
				
			}
		
			#ifdef uart_start
		
			SEGGER_RTT_printf(0,"Del_PS_Print_Type:%x.\r\n",PS_Print_Type);			//输出删除的类型
			SEGGER_RTT_printf(0,"Del_PS_Num: %d.\r\n",Del_PS_Print_Data_Num);		//输出删除的数量
			
			#endif
			
}


/*******************************************

删除某一个指纹
输入： PS_Print_ID 指纹ID
输出： 无

此函数没有自主开启 SPI FLASH 能力
*********************************************/

uint8_t Del_A_ID_PS_Print(uint16_t PS_Print_ID)
{
		
		uint32_t Block,Page;  //读取Flash扇区所需变量
		uint32_t PS_Print_Data_Num;
		uint32_t Del_PS_Print_Data_Num;		//删除的数据量
		uint32_t i;
		uint8_t PS_ID1,PS_ID2;		// 用于对比寻找一致的类
		
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nStart_Del_A_ID_PS_Print!\r\n");
	
		SEGGER_RTT_printf(0,"PS_Print_ID: %x .\r\n",PS_Print_ID);
		
		#endif	
	
	
		PS_Print_Data_Num = Get_Cat_Dat(PS_Print_Num_Code);			//获取 指纹的 数据数量
		
		if((PS_Print_Data_Num==0)||(PS_Print_Data_Num>PS_Print_Num))
		{
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Ps_Print_Data_Num: %x .\r\n",PS_Print_Num);
			
			#endif
			
			//数量有错误或没有数据时抛掉这次运行
			
			return 0;
		
		}
		
		//数量正常开始下一步
		
		//将类型分两字节保存到缓存
		
		PS_ID1 = PS_Print_ID>>8&0xFF;
		PS_ID2 = PS_Print_ID&0xFF;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"PS_ID1: %x!\r\n",PS_ID1);
		SEGGER_RTT_printf(0,"PS_ID2: %x!\r\n",PS_ID2);
		
		#endif
		
		Del_PS_Print_Data_Num =0;
		
		Sector_to_Block_Page(&Block,&Page,PS_Print_Soctor1);  //根据偏移后的扇区号获取块和起始页
		SPIFlash_Read_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len);							  //读取整个扇区数据
		
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
		
		for(i =0;i<PS_Print_Num;i++)			//寻找一致的类型
		{
			if((tab_dat[i*PS_Print_Len]==0x55)&&
				 (tab_dat[i*PS_Print_Len+3]==PS_ID1)&&
				 (tab_dat[i*PS_Print_Len+4]==PS_ID2)			//判断类型一致
				)
				{
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\nNeed_A_Ps_Print!\r\n");
					
					#endif
					
					for(uint8_t it=0;it<PS_Print_Len;it++)		//清空需要删除的数据
					{
						
						tab_dat[i*PS_Print_Len+it] = 0;
					
					}
					
					Del_PS_Print_Data_Num =1;
					
					if(Del_PS_Print_Data_Num>PS_Print_Data_Num)		//当删除数据量大于已有数据量时，配置为一样大。（一般不会发生）
					{
						
						Del_PS_Print_Data_Num = PS_Print_Data_Num;
					//	break;		//同时跳出这个循环
						
					}
					break;
				}
		}
		
		if(Del_PS_Print_Data_Num==PS_Print_Data_Num)
		{
			//当删除数据量与本地拥有的数据量一致时对所有缓存进行清空
			
			for(i =0;i<(PS_Print_Num*PS_Print_Len);i++)
			{
				tab_dat[i] =0;		//清空缓存
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
			
			SPIFlash_Write_Tab(tab_dat,Block,Page,PS_Print_Num*PS_Print_Len); //将新数据写入
			SPI_Flash_Busy();
			
			
			Ev_Cat_Dat(PS_Print_Num_Code,(PS_Print_Data_Num-Del_PS_Print_Data_Num));    //写入新的数据到目录
			
			if((PS_Print_Data_Num-Del_PS_Print_Data_Num)==0)
			{
				//当本地指纹数量为0时触发清空指纹头指令
				
					CL_PS_ID	 =0;			//清空所有指纹信息
					CL_PS_Run = CL_PS_Run_Del_All_Mode;				//触发标志
				
			}
			#ifdef uart_start
		
			SEGGER_RTT_printf(0,"Del_PS_Print_Type:%x.\r\n",PS_Print_ID);			//输出删除的类型
			SEGGER_RTT_printf(0,"Del_PS_Num: %d.\r\n",Del_PS_Print_Data_Num);		//输出删除的数量	
		
			#endif
		
		CL_PS_ID	 =PS_Print_ID;
		CL_PS_Run = CL_PS_Run_Del_Single_Mode;
		
		return 1;
		
}

/*

TSM12MC_ 灵敏度设置值保存

输入： 要写入的设置值

成功返回 1 失败返回0
*/


uint8_t TSM12MC_Sen_Code_update(uint8_t *s)
{
	
		uint32_t Block,Page;  //读取Flash扇区所需变量
		uint32_t Sector;  //读取Flash扇区所需变量
		
		uint8_t Sen_Tab[12];		//设置按键灵敏度
		uint32_t i;
		
		for(i =0;i<12;i++)			//将数据写入缓存
		{
			if(*s<16) //判断数据合理性
			{
				Sen_Tab[i] =*s;
				s++;				
			}
			else
			{
				return 0;			//数据不合理时返回0 
			}
		}
		
		Sector_to_Block_Sector(&Block,&Sector,TSM12MC_Sen_Sector_Num);  //获取偏移后待擦除扇区数据
		
		SPI_Flash_Busy();
		
		SPIFlash_Erase_Sector(Block,Sector); 		//擦除数据
		
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Erase_TSM12MC_Sen_Sector!\r\n");		//提示删除成功
		
		#endif	
		
		Sector_to_Block_Page(&Block,&Page,TSM12MC_Sen_Sector_Num);
		
		SPIFlash_Write_Tab(Sen_Tab,Block,Page,12); //将新数据写入
		SPI_Flash_Busy();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"TSM12MC_Sen_Write_OK!\r\n");
		
			read_Soctor(TSM12MC_Sen_Sector_Num);
		#endif
		
		TSM12MC_Sen_Read();
		
		if(MPR121_RUN)			//触摸板唤醒的情况下，写入触摸芯片
		{
			TSM12_Exit_Sleep_Mode();
			TSM12_read_tast();
		}
		
		return 1;
		
}

extern uint8_t TSM12_RUN_Sen[6];

#define XD_Key			1			// 当 按键板是 XD 型的时
//#define X_Key				1		// 当 按键板是 X 型的时

void TSM12MC_Sen_Read()     //将触摸灵敏度数据同步给芯片端
{
		uint32_t Block,Page;  //读取Flash扇区所需变量
		uint8_t Sen_Tab[12];		//设置按键灵敏度
		
		SPI_Flash_Busy();
		Sector_to_Block_Page(&Block,&Page,TSM12MC_Sen_Sector_Num);  //根据偏移后的扇区号获取块和起始页
		SPIFlash_Read_Tab(Sen_Tab,Block,Page,12);							  //读取扇区中的触摸灵敏度数据
	
	#ifdef XD_Key					// XD 型 增强 8
	
	if(Sen_Tab[7]<=0x0F)
		Sen_Tab[7] +=3;	//按键 8 灵敏度 加 3
	
	if(Sen_Tab[7]>=0x0F)
		Sen_Tab[7] = 0x0F;
	
	#endif

	#ifdef X_Key					// X 型 增强 6
	
	if(Sen_Tab[5]<=0x0F)
		Sen_Tab[5] +=3;	//按键 6 灵敏度 加 3
	
	if(Sen_Tab[5]>=0x0F)
		Sen_Tab[5] = 0x0F;
	
	#endif
	

	
		for(uint8_t i=0;i<12;i++)    	//将12个灵敏度值转换为触摸芯片寄存器可用的值	
		{
			
			switch(Sen_Tab[i])
			{
				case 0:   Sen_Tab[i] = 0x07;		//最低灵敏度
					break;
				case 1:		Sen_Tab[i] = 0x07+0x08;		//最低灵敏度加强
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
					Sen_Tab[i] = 0x0B;			//未设置(参数无效)的情况下默认灵敏度
				break;
			}
			
		}
		
	//配置通道2灵敏度
		TSM12_RUN_Sen[0] = Sen_Tab[1];
		TSM12_RUN_Sen[0] = TSM12_RUN_Sen[0]<<4;
		
		//配置通道1 灵敏度
		TSM12_RUN_Sen[0] |= Sen_Tab[2];
		
		//配置通道4 灵敏度
		TSM12_RUN_Sen[1] = Sen_Tab[7];
		TSM12_RUN_Sen[1] = TSM12_RUN_Sen[1]<<4;
		
		//配置通道3 灵敏度
		TSM12_RUN_Sen[1] |= Sen_Tab[0];
		
		//配置通道6 灵敏度
		TSM12_RUN_Sen[2] = Sen_Tab[9];
		TSM12_RUN_Sen[2] = TSM12_RUN_Sen[2]<<4;
		
		//配置通道5 灵敏度
		TSM12_RUN_Sen[2] |= Sen_Tab[8];
		
		//配置通道8 灵敏度
		TSM12_RUN_Sen[3] = Sen_Tab[11];
		TSM12_RUN_Sen[3] = TSM12_RUN_Sen[3]<<4;
		
		//配置通道7 灵敏度
		TSM12_RUN_Sen[3] |= Sen_Tab[10];
		
		//配置通道10 灵敏度
		TSM12_RUN_Sen[4] = Sen_Tab[11];
		TSM12_RUN_Sen[4] = TSM12_RUN_Sen[4]<<4;
		
		//配置通道9 灵敏度
		TSM12_RUN_Sen[4] |= Sen_Tab[10];
				
		//配置通道12 灵敏度
		TSM12_RUN_Sen[5] = Sen_Tab[5];
		TSM12_RUN_Sen[5] = TSM12_RUN_Sen[5]<<4;
		
		//配置通道11 灵敏度
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




