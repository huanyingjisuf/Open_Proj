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
#include "Devic_Init.h"  //设备初始化


#define Devic_Num_BLOCK  0  //声明设备号存储区块
#define Devic_Num_PAGE   0  //设备绑定号所在页
#define Devic_Num_Sector 0  //设备绑定号所在扇区号。

#define Devic_PAGE_Len  40  //设备号扇区使用空间大小
#define Devic_Num_Len   40  // 设备绑定码(包含广播名称内容)
#define PAGE_Heart_H   0x55  //扇区页起始数高字节
#define PAGE_Heart_L   0xAA  //扇区页起始数低字节




extern const uint8_t Cat_Dat_Sector1;						//设定目录扇区的起始地址

extern const uint8_t Cat_Dat_Sector_Num;				// 设定 目录 消耗的 扇区数量

extern const uint32_t Cat_Dat_len;				// 设定 目录 数据的总长度



extern const uint8_t PassWord_Len;  //单条密码数据长度

extern const uint8_t Open_Note_Num_Len; //单条开门记录长度

extern const uint8_t PassWord_Secotr1;  //密码数据所在扇区

extern const uint8_t PassWord_Secotr2;  //密码数据所在扇区

extern const uint8_t PassWord_SN_Secotr;  //密码编号存储扇区 (每个自定义密码的编号)

extern const uint8_t Act_PassWord_Secotr;  //存储动态密码

extern const uint8_t History_PassWord_Secotr;          //历史动态密码

extern const uint8_t Per_Act_PassWord_Secotr;				//保存永久动态密码


extern const uint16_t AD_TypeA_Data_Secotr1;  //保存A卡数据的扇区1 (目前共计1个扇区)

extern const uint16_t AD_TypeB_Data_Secotr1;  //保存B卡数据的扇区1 (目前共计1个扇区)

extern const uint16_t US_TypeA_Data_Secotr1;  //保存A卡数据的扇区1 (目前共计1个扇区)

extern const uint16_t US_TypeB_Data_Secotr1;  //保存B卡数据的扇区1 (目前共计1个扇区)

// 上方每个扇区头 间隔 105 个扇区为了保证数据安全，需要将最大扇区数控制在100个以内

extern const uint8_t TypeA_Data_Len;  	//一条A卡数据的长度

extern const uint8_t TypeB_Data_Len;		//一条B卡数据的长度

extern const uint8_t A_Sector_TypeA_Data_Num;			//一个扇区保存的 A 卡 数据数量

extern const uint8_t A_Sector_TypeB_Data_Num;			// 一个扇区保存的 B 卡 数据数量

extern const uint8_t AD_TypeA_Data_Sector_Num;		// AD_TypeA_Data  保存数据使用的扇区个数

extern const uint8_t US_TypeA_Data_Sector_Num;		// US_TypeA_Data  保存数据使用的扇区个数

extern const uint8_t AD_TypeB_Data_Sector_Num;		// AD_TypeB_Data  保存数据使用的扇区个数

extern const uint8_t US_TypeB_Data_Sector_Num;		// US_TypeB_Data  保存数据使用的扇区个数







//#define PassWord_Len 18  //定义一条密码数据的长度
//#define PassWord_Secotr 2 //定义密码存储扇区位置
//#define Max_PassWord_Num  100  //定义，最大可支持的密码数量

extern const uint8_t PassWord_Len;  //单条密码长度
extern const uint8_t Open_Note_Num_Len; //单条开门记录长度

extern const uint8_t PassWord_Secotr1;
extern const uint8_t PassWord_Secotr2;  //密码数据所在扇区
extern const uint8_t Max_PassWord_Num;
extern const uint8_t PassWord_SN_Secotr;  //密码编号存储扇区

extern uint32_t Unix_T;  //当前芯片内时间戳


extern const uint8_t A_Sector_TypeA_Data_Num;

extern const uint8_t A_Sector_TypeB_Data_Num;


extern const uint8_t OpenDoorNote_Num_Code; 
extern const uint16_t Open_Note_Secotr1;
extern const uint16_t Open_Note_Secotr2;
extern const uint16_t Open_Note_Secotr3;
extern const uint16_t Open_Note_Secotr4;  //四个开门记录专用扇区


extern const uint16_t AD_TypeA_Data_Secotr1;  //保存A卡数据的扇区1 (目前共计1个扇区)

extern const uint16_t AD_TypeB_Data_Secotr1;  //保存B卡数据的扇区1 (目前共计1个扇区)

extern const uint16_t US_TypeA_Data_Secotr1;  //保存A卡数据的扇区1 (目前共计1个扇区)

extern const uint16_t US_TypeB_Data_Secotr1;  //保存B卡数据的扇区1 (目前共计1个扇区)


//const uint8_t Flash_Word_Num =8;  //定义 Flash 目录类型数量

//const uint8_t OpenDoorNote_Num_Code =0x11; 

//const uint8_t Act_PassWord_Num_Code =0x02;// 动态密码存储目录编号

extern  const uint8_t More_Act_PassWord_Num_Code;			//多组时效动态密码存储目录

extern const uint8_t AD_TypeA_Data_Num_Code; // 管理员A卡信息存储目录编号

extern const uint8_t AD_TypeB_Data_Num_Code; // 管理员B卡信息存储目录编号

extern const uint8_t US_TypeA_Data_Num_Code; // 用户A卡信息存储目录编号

extern const uint8_t US_TypeB_Data_Num_Code; // 用户B卡信息存储目录编号

extern const uint8_t CK_Auth_Index_Code;  //定义设备客人数据卡序列号 @ck_auth_index

extern const uint8_t Nmg_Auth_Index_Code;  //定义设备管理数据卡序列号 @ nmg_auth_index


extern const uint8_t PS_Print_Len;		//0x55+2字节指纹类型+2字节指纹ID+4字节起始实际+4字节失效实际

extern const uint8_t PS_Print_Num;			//保存到最大指纹数量

extern const uint16_t PS_Print_Soctor1;  //定义指纹数据扇区1

extern uint8_t SPI0_Start; //确认SPI0 是否被启用

//目录区 扇区参数

/*备注：  
目录的扇区并不是同时使用，而是按扇区顺序轮流写入保存同一个数据 
以平衡Flash擦写磨损，以达到理想的理论擦写次数
写入下一个扇区时，需要使其他扇区无效化  （数据清零）
*/
extern const uint8_t Cat_Dat_Sector1;						//设定目录扇区的起始地址

extern const uint8_t Cat_Dat_Sector_Num;				// 设定 目录 消耗的 扇区数量

extern const uint32_t Cat_Dat_len;				// 设定 目录 数据的总长度




extern const uint16_t Open_Note_Size;  //定义开门记录容量
extern const uint16_t A_Sector_Open_Note_Size;


extern const uint8_t Max_PassWord_Num;  //最大密码数量
extern const uint8_t A_Sector_PassWord_Num;  //定义单个密码扇区所能容纳的密码数量


//#define PassWord_Num_BLOCK 0 //声明密码数量保存区块
//#define PassWord_Num_PAGE 0 // 声明密码数量所在页
//#define PassWord_Num_Sector 1   //声明密码数量所在扇区
 
//定义目录头标识
#define PassWord_Num_code 0x01  //声明密码数量数据类型 

extern const uint8_t Act_PassWord_Num_code;
extern const uint8_t OpenDoorNote_Num_Code;  //开门记录数量数据类型

#define Admin_PassWord  0x12		//定义管理员密码前缀
#define User_PassWord  0x22		//定义用户密码前缀

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

uint8_t Add_Flash_PassWord(uint8_t *PassWord); //写入一个密码
void Del_Flash_PassWord(uint8_t *PassWord); //删除一个密码
void read_Soctor(uint32_t sector); //读取整个扇区 (2k)内容，并打印输出

void Ev_Cat_Dat(uint8_t Cat_Type,uint32_t Cat_Dat);//写入目录信息

uint32_t Get_Cat_Dat(uint8_t Cat_Type); //获取目录信息

void SPIFlash_Read_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len);

void SPIFlash_Write_Tab(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t tab_len);

void Del_Admin_PassWord(void);  // 删除管理员密码
void Del_User_PassWord(void);   //删除用户密码

uint8_t Del_A_PassWord(uint8_t PassWord_SN);  //根据密码序号删除一个密码
uint8_t Del_SN_PassWord(uint8_t PW_SN);  //根据密码序号删除密码(2个扇区)

void ADD_Op_Door_Note(uint16_t SN_Num,uint16_t Key_type,uint32_t Unix_t);  //载入开门记录 (开门时调用)


uint8_t Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T);//判断输入的密码是否和本地保存的有效动态密码一致

uint8_t  History_Time_Act_PassWord(void);


uint16_t Add_Flash_AD_TypeA_Data (uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //载入管理员A卡数据
uint16_t Add_Flash_US_TypeA_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //载入用户A卡数据
uint16_t Add_Flash_AD_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //载入管理员 B卡数据
uint16_t Add_Flash_US_TypeB_Data(uint8_t *uid,uint32_t start_Unix,uint32_t stop_Unix);  //载入用户B卡数据


uint8_t Del_AD_Type_A(uint8_t *AD_Type_A_SN);									//删除 指定 SN 管理员 A 卡数据

uint8_t Del_US_Type_A(uint8_t *US_Type_A_SN);								//删除 指定 SN 用户 A 卡数据

uint8_t Del_AD_Type_B(uint8_t *TypeB_ID);								//删除 指定 SN 管理员 A 卡数据

uint8_t Del_US_Type_B(uint8_t *TypeB_ID);						//删除 指定 SN 用户 B 卡数据

uint32_t Need_TypeA_Data_SN(uint8_t *Need_Uid); 		  //判断 本地 是否有 相同的 A 卡数据

uint32_t Need_TypeB_Data_SN(uint8_t *Need_Uid);			

void CL_Flash_OPEN_DOOR_NOTE(void);  //清空开门记录数量

uint8_t Once_Act_PassWord_Verify(void);    //用于确认一次密码是否已经被使用过
 
void Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t);    //保存动态密码

void More_Act_PassWord_Save(uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t);     //保存更多的动态密码(同时允许多组动态密码生效)

uint8_t More_Act_PassWord_Verify(uint8_t PassWord_Len,uint32_t PW_Unix_T);				//多组动态密码开门指令

uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord);				//从多组动态密码中删除一个


void Erase_AD_TypeA_Sector(void);			// 擦除 AD_TypeA_Sector 数据

void Erase_US_TypeA_Sector(void);			// 擦除 US_TypeA_Sector 数据

void Erase_AD_TypeB_Sector(void);			// 擦除 AD_TypeB_Sector 数据

void Erase_US_TypeB_Sector(void);			// 擦除 US_TypeB_Sector 数据

uint16_t Add_Flash_PS_Print (uint16_t PS_Type,uint16_t PS_ID,uint32_t start_Unix,uint32_t stop_Unix);			//向 FLASH 写入指纹头数据

uint16_t Need_Of_Ps_Print_Data_IDLE_ID(void);    // 获取 指纹头空闲的ID

void Erase_PS_Print_Sector(void);				//擦除指纹扇区

uint8_t Del_A_ID_PS_Print(uint16_t PS_Print_ID);		//通过 ID 删除指纹

uint32_t Opinion_PS_Print_Valid(uint16_t PS_Print_ID);			//验证 ID 是否有效

void  Del_A_Type_PS_Print(uint16_t PS_Print_Type);

void Per_Act_PassWord_Save(uint8_t Act_Type,uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t);				//保存永久密码

uint8_t Per_Act_PassWord_Verify(uint8_t PassWord_Len);		//判断本地永久密码

uint8_t TSM12MC_Sen_Code_update(uint8_t *s);				//TSM12MC_ 灵敏度设置值保存

void TSM12MC_Sen_Read(void);     //将触摸灵敏度数据同步给芯片端

uint8_t Read_Op_Note(uint16_t Read_Num);			//读取开门记录值用于返回

#endif
