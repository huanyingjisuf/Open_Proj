#include "mcu.h"
#include "sky1311_drv.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "userdef.h"
#include  <Flash_RW.h>
#if 0
/* 按照字符串输出10进制数 */
void Dec2Str(uint32_t DecNum, uint8_t *DecStr, uint8_t *size)
{
    uint8_t temp[22];
    uint8_t i=0;
    *size = 0;
    do{
        temp[i++] = (uint8_t)(DecNum%10)+0x30;
        DecNum/=10;

    }while(DecNum>0);
    *size = i;

    for(i=0;i<*size;i++)
        DecStr[i] = temp[(*size)-i-1];

}
#endif

extern uint8_t read_TypeA_Data[128];  //A卡数据区域缓存

/*****************************************************************************/
/*****************************************************************************/
/***************************************************************
    M1卡读取扇区操作示例
   读取扇区数据并通过串口打印出来
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t mifare1_ReadTest(uint8_t *UID)
{
    sta_result_t sta;
    uint8_t tmpBuf[20];
    uint8_t default_key[] =
	//	{1,2,3,4,5,6};
	//{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
	{0x0A, 0x26, 0x69, 0xAB, 0xF4, 0x77};
    uint8_t  block_num=0;
		uint8_t i,it;
	
	for(i =0;i<sizeof(read_TypeA_Data);i++)  //清空数据区缓存
			read_TypeA_Data[i] =0;
				it =0;  //清空缓存位置值准备写入
#ifdef uart_start
    SEGGER_RTT_printf(0,"\r\nRead Mifare1 test\r\n");
#endif
    /* read test Mifare 1K card (0--63 block) */
 //   for(sector_num=4;sector_num<16;sector_num++)// read sector0--16
	
	
	
    for(block_num=0X28;block_num<0X2C;) // read block0--63
    {
//				block_num = 0X28;
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key , UID, block_num);

        if(Ok == sta)
        {
            /*  read 4 block from 1 sector */
            for(uint8_t i=0;i<4;i++)
            {
									sta = M1_Read(block_num, tmpBuf);
									if(sta==Ok)
									{
										
							#ifdef uart_start
								
							SEGGER_RTT_printf(0,"Block ");	
							SEGGER_RTT_printf(0,"%x ",block_num);	
							SEGGER_RTT_printf(0,":\r\n");
								
							#endif
															
								for(i =0;i<16;i++)
										{
											#ifdef uart_start
											SEGGER_RTT_printf(0,"%x ",tmpBuf[i]);
											#endif
											//同时将读到的数据写入缓存区
											if(it<sizeof(read_TypeA_Data))  //当数据在缓存区范围内时 写入缓存区
											{
												read_TypeA_Data[0] = 0x01;  //表示A 卡 数据区有效，也用于判断是否为加密卡
												read_TypeA_Data[it+1] = tmpBuf[i];  //将数据写入外部专用缓存区域
												it++;
											}
										}
										#ifdef uart_start
								SEGGER_RTT_printf(0,"\r\n");
										#endif
	//                    uart_puts("Block ");
	//                    uart_putHex(block_num);
	//                    uart_puts(" : ");
	//                    uart_printBuffer(tmpBuf,16);
	//                    uart_newrow();
											block_num++;
								Type_AB_UID[0] = 0x01;  //起始标志数据为A卡数据
										
                }
                else
									{
										//当数据区读取失败时
										read_TypeA_Data[0] =0xFF;  //表示数据失效
										
										
										
#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Block 0x%x Reader Error\r\n",block_num);
										
//                    uart_puts("Block 0x");
//                    uart_putHex(block_num);
//                    uart_puts("Reader Error\r\n");
#endif
                    //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    //return sta;
                }
            }
        }
        else
        {
					read_TypeA_Data[0] =0xFF;  //表示数据无效
#ifdef uart_start
					
//		SEGGER_RTT_printf(0,"Sector 0x%x Authentication Error!\r\n",sector_num);
					
						
//            uart_puts("Sector 0x");
//            uart_putHex(sector_num);
//            uart_puts("Authentication Error\r\n");
#endif
            sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            return sta;
					
        }



//#if DEBUG==1
//        uart_newrow();
//#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);    // next Authentication
    
		}
		
		
		
//#if 0
//    /* read test Mifare 1K card (64--127 block) */
//  //  for(sector_num=16;sector_num<32;sector_num++)// read sector16--31  (block64--127 )
//    for(block_num=64;block_num<128;)
////    {
//		
//		//block_num = 0x27;
//        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
//
//        if( Ok == sta )
//        {
//            for(uint8_t i=0;i<4;i++)    /* 4 blocks/sector */
//            {
//                sta = M1_Read(block_num, tmpBuf);
//                if(sta==Ok)
//                {
//#ifdef uart_start
//		
//					SEGGER_RTT_printf(0,"Block %x:\r\n",block_num);
//					
//					for(i =0;i<16;i++)
//					SEGGER_RTT_printf(0,"%x ",tmpBuf[i]);
//					
//					SEGGER_RTT_printf(0,"\r\n");
//				
//					
//					
//									
////                    uart_puts("Block ");
////                    uart_putHex(block_num);
////                    uart_puts(" : ");
////                    uart_printBuffer(tmpBuf,16);
////                    uart_newrow();
//#endif
//                    block_num++;
//                }
//                else
//                {
//                    sky1311WriteReg(ADDR_M1_CTRL, 0x00);
//                    return sta;

//                }
//            }
//        }
//        else{
//#ifdef uart_start
//					
//			SEGGER_RTT_printf(0,"Block 0x&x Anthentication Error!\r\n");
//			
////            uart_puts("Block 0x");
////            uart_putHex(block_num);
////            uart_puts("Authentication Error\r\n");
//#endif

//            piccHaltA();
//            DelayMS(5);
//            sta = piccWakeupA(tmpBuf);
//            if(Ok == sta)
//            {
//                sta = piccSelectA(SEL1,UID, tmpBuf);
//                if(Ok != sta){
//#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"PICC Select Error!\r\n");
//									
////                    uart_puts("PICC Select Error!\r\n");
//#endif
//                }
//            }
//            else
//            {
//#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"PICC Request Error! @ Block 0x%x\r\n",block_num);
////                uart_puts("PICC Request Error! @ ");
////                uart_puts("Block 0x");
////                uart_putHex(block_num);
//#endif
//            }
//            //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
//            //return sta;
//        }
////#if DEBUG==1
////        uart_newrow();
////#endif
//        sky1311WriteReg(ADDR_M1_CTRL, 0x81);

//    }

//    /* read test Mifare 4K card (128--255 block) */
//    //for(sector_num=32;sector_num<40;sector_num++)// read sector32--39  (block128--255 )
//    for(block_num=128;block_num<255;)
//    {
//        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
//        if(Ok == sta)
//        {
//            for(uint8_t i=0;i<16;i++)       /* 16 blocks/sector */
//            {
//                sta = M1_Read(block_num, tmpBuf);
//                if(sta==Ok)
//                {
//#ifdef uart_start
//									
//				SEGGER_RTT_printf(0,"Block %x :",block_num);
//				
//				for(i =0;i<16;i++)
//					SEGGER_RTT_printf(0,"%x",tmpbuf[i]);

//			  SEGGER_RTT_printf(0,"\r\n");
////                    uart_puts("Block ");
////                    uart_putHex(block_num);
////                    uart_puts(" : ");
////                    uart_printBuffer(tmpBuf,16);
////                    uart_newrow();

//#endif
//                    if(block_num == 255)
//                    {
//                        sky1311WriteReg(ADDR_M1_CTRL, 0x00);
//                        return Ok;
//                    }
//                    else
//                    {
//                        block_num++;
//                    }
//                }
//                else
//                {
//                    sky1311WriteReg(ADDR_M1_CTRL, 0x00);
//                    return sta;
//                }
//            }
//        }
//        else{
//#ifdef uart_start
//					
//					SEGGER_RTT_printf(0,"Block %x Anthentication Error!\r\n"block_num);
////            uart_puts("Block 0x");
////            uart_putHex(block_num);
////            uart_puts("Authentication Error\r\n");
//#endif

//            piccHaltA();
//            sky1311WriteReg(ADDR_M1_CTRL, 0x00);
//            DelayMS(5);
//            sta = piccWakeupA(tmpBuf);
//            if(Ok == sta)
//            {
//                sta = piccSelectA(SEL1,UID, tmpBuf);
//                if(Ok != sta){
//#ifdef uart_start
//					SEGGER_RTT_ptintf(0,"PICC Select Error!\r\n");
////                    uart_puts("PICC Select Error!\r\n");
//                    return Ok
//#endif
//                }
//            }
//            else
//            {
//#ifdef uart_start
//							
//							SEGGER_RTT_printf(0,"PICC Request Error! @ Block 0x%x\r\n",block_num);
////                uart_puts("PICC Request Error! @ ");
////                uart_puts("Block 0x");
////                uart_putHex(block_num);
//                return Ok;
//#endif
//            }
//            //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
//            //return Ok;
//        }
////#if DEBUG==1
////        uart_newrow();
////#endif
//        sky1311WriteReg(ADDR_M1_CTRL, 0x81);
//    }
//#endif
    //sky1311Reset();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\nread_TypeA_Data:\r\n");
		
		#endif
		
		for(i =0;i<sizeof(read_TypeA_Data);i++)
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"%x",read_TypeA_Data[i]);
			
			#endif
			
			if((i!=0)&&(i%30==0))
			{
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\n");
				
				#endif
			}
		}
		
			#ifdef uart_start
				
				SEGGER_RTT_printf(0,"\r\n");
			
			#endif
		
    return Ok;
}

/***************************************************************
    M1卡写入操作示例
    M1卡写入Block4 (Sector 1) 和Block8（Sector2）操作。
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t mifare1_WriteTest(uint8_t *UID)
{
    sta_result_t sta;
    uint8_t default_key[] = 
//	{1,2,3,4,5,6};
{0x0A, 0x26, 0x69, 0xAB, 0xF4, 0x77};
 //{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t i;
    uint8_t writeData[] = 
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    uint8_t keya[]={0x0A, 0x26, 0x69, 0xAB, 0xF4, 0x77};
    uint8_t keyb[]= {0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
	//	uint8_t block_num;

#ifdef uart_start
    
	SEGGER_RTT_printf(0,"\r\nWrite Mifare1 test \r\n");
	//	uart_puts("\r\nWrite Mifare1 test\r\n");

#endif

		
    sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, 0x2B);
    if(Ok==sta)
    {
        sta = M1_PwdChange(0X2B,keya,keyb);
        if(Ok == sta)
        {
            sky1311WriteReg(ADDR_M1_CTRL, 0x81);
        }
    }
    else
    {
        piccHaltA();
        sky1311WriteReg(ADDR_M1_CTRL, 0x00);
			
			#ifdef uart_start
			
		SEGGER_RTT_printf(0,"Write_Error!\r\n");	
			
			#endif
			
        return Error;

    }
    for(i=0;i<3;i++)
    {
        irq_sta = sky1311ReadReg(ADDR_M1_CTRL);

        uint8_t block_num = 0X28+i;
        sta = M1_Write(block_num, writeData);
			
#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Block Write %x \r\n",block_num);
			
//        uart_puts("Block write ");
//        uart_putHex(block_num);
#endif
        if(Ok == sta)
        {
#ifdef uart_start
					
				SEGGER_RTT_printf(0,"Success\r\n");
          //  uart_puts("Success\r\n");
#endif
        }
        else{
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Failed\r\n");
           // uart_puts("Failed\r\n");
#endif
        }

    }
    return Ok;
}

/***************************************************************
    CPU卡操作示例
   读取CPU卡的UID等，并使用部分APDU做测试
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t smartTypeA_test(void)
{
    sta_result_t sta;
    uint8_t tmpBuf[100];
    uint16_t tmpSize;
#ifdef uart_start
	SEGGER_RTT_printf(0,"\r\nSmart Type A test\r\n");
   // uart_puts("\r\nSmart Type A test\r\n");
#endif
    sta = piccATS(tmpBuf);
    if(sta == NoResponse){
        sky1311Reset();
#ifdef uart_start
			
	SEGGER_RTT_printf(0,"\r\nRATS error!\r\n");
			
//        uart_puts("\r\nRATS error!\r\n");
#endif
        return ErrorAts;
    }
    else{
#ifdef uart_start
			
			SEGGER_RTT_printf(0,"RATS : ");
			
			for(uint8_t i =0;i<32;i++)
			SEGGER_RTT_printf(0,"%x ",tmpBuf[i]);
			SEGGER_RTT_printf(0,"\r\n");

//    	uart_puts("RATS : ");
//    	uart_printBuffer(tmpBuf,32);
//        uart_newrow();
//			
#endif
			}
#if 0
    /* 00 a4 04 00 00 */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x04;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x00;

    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{

#ifdef uart_start
			
			SEGGER_RTT_printf(0,"Select SD:");
			
			for(i =0;i<(sizeof(tmpbuf)-2);i++)
			{
				SEGGER_RTT_printf(0,"%x ",tmpBuf[i]);
			}
			SEGGER_RTT_printf(0,"\r\n");

//        uart_puts("Select SD:");
//    	uart_printBuffer(tmpBuf,tmpSize-2);
//        uart_newrow();
#endif
    }
#endif

     /* select MF file */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x3F;
    tmpBuf[7] = 0x00;   // MF
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#ifdef uart_start
			
      
			SEGGER_RTT_printf(0,"Select MF:");
			
			for(uint8_t i =0;i<(sizeof(tmpBuf)-2);i++)
  			SEGGER_RTT_printf(0,"%x ",tmpBuf[i]);
			
			SEGGER_RTT_printf(0,"\r\n");
			
//				uart_puts("Select MF:");
//				uart_printBuffer(tmpBuf,tmpSize-2);
//        uart_newrow();
//			
#endif
    }
#if 0
    /*获取随机数*/
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0x84;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x08;
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
			
#ifdef uart_start
			
			SEGGER_RTT_printf(0,"RM: ");
			
			for(i =0;i<(sizeof[tmpBuf]-2);i++)
				SEGGER_RTT_printf(0,"%x ",tmpBuf[i]);
			
			SEGGER_RTT_printf(0,"\r\n");
			
			
//        uart_puts("RM:");
//    	 uart_printBuffer(tmpBuf,tmpSize-2);
//        uart_newrow();
//			
#endif
    }
#endif

#if 0
    /* 读一卡通有关的信息 */
    tmpBuf[0] = 0x03;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x10;
    tmpBuf[7] = 0x01;

    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Read Card Info:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
        uart_puts("Card No: ");
        DecNum = (uint32_t) tmpBuf[40]<<24;
        DecNum += (uint32_t) tmpBuf[39]<<16;
        DecNum += (uint32_t) tmpBuf[38]<<8;
        DecNum += (uint32_t) tmpBuf[37];

        Dec2Str(DecNum,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_newrow();

#endif
    }
    /* 读余额 */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x80;
    tmpBuf[2] = 0x5C;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x02;
    tmpBuf[5] = 0x04;

    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Read Balance:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
        uart_puts("Balance: ￥");
        DecNum = (uint32_t) (tmpBuf[1]&0x0f)<<24;
        DecNum += (uint32_t) tmpBuf[2]<<16;
        DecNum += (uint32_t) tmpBuf[3]<<8;
        DecNum += (uint32_t) tmpBuf[4];
        uint8_t decimal = DecNum%100;
        DecNum = DecNum/100;
        Dec2Str(DecNum,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_putChar('.');
        Dec2Str(decimal,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_newrow();

#endif
    }

    for(i=1;i<=10;i++)
    {
        /* 读取交易记录 */
        tmpBuf[0] = 0x02;
        tmpBuf[1] = 0x00;   // CLA Class
        tmpBuf[2] = 0xB2;   // INS Instruction
        tmpBuf[3] = i;//0x01;   // P1 Parameter 1
        tmpBuf[4] = 0xC4;//0xC5;   // P2 Parameter 2
        tmpBuf[5] = 0x00;   // Le

        sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
        if(sta == NoResponse)
        {
            return NoResponse;
        }
        else{
    #if DEBUG==1
            uart_puts("Read Record:");
            uart_printBuffer(tmpBuf,tmpSize-2);
            uart_newrow();
    #endif
        }
    }
#endif //if 0
    sky1311Reset();
    //LED_OFF();
    return Ok;
}
/******************************************************************************/
/*  Type A PICC test */
sta_result_t TypeA_test(void)
{
    sta_result_t sta;
    uint8_t SAK=0;
    uint8_t ATQA[2]={0};
    uint8_t UID[12]={0};
    uint8_t *uid;
    uint8_t sel=SEL1;
    uint8_t uidSize=0;
    uint8_t tmpBuf[10];
    uint8_t i;
    SKY1311_ENABLE(); 	    // chip enable PD2
    sky1311Init();			// init sky1311s
    //resetPicc();            // 复位卡片()
    DelayMS(10);             // 等待卡片工作
    typeAOperate();
    //sta = piccRequestA(tmpBuf);     // send REQA command to card
    sta = piccWakeupA(tmpBuf);        // send WUPA command to card
    if(Ok == sta)
    {
        LED_ON();
        ATQA[0] = tmpBuf[1];
        ATQA[1] = tmpBuf[0];
        uidSize = tmpBuf[0]>>6;
#ifdef uart_start
			
				SEGGER_RTT_printf(0,"\r\nType-A Card Test\r\n");
				SEGGER_RTT_printf(0,"ATQA:");
				SEGGER_RTT_printf(0,"%x\r\n",ATQA);
			
//        uart_puts("\r\nType-A Card Test\r\n");
//        uart_puts("ATQA:");
//        uart_printBuffer(ATQA,2);
//        uart_newrow();

#endif
    }
    else
    {
        sky1311Reset();
        return ErrorRequest;            // 这里返回，有可能是没卡或者没读到
    }
    uid = &UID[0];
    sel = SEL1;
    do{
        sta = piccAntiA(sel, 1, uid);   // 防碰撞循环，测试获得一张卡的UID，参数'1'表示遇到冲突时选择1
        if(Ok == sta)
        {
            uint8_t checkSum = uid[0] ^ uid[1] ^ uid[2] ^ uid[3];
            if(checkSum != uid[4])
            {
#ifdef uart_start
                SEGGER_RTT_printf(0,"\r\nAnti error!\r\n");
#endif
                sky1311Reset();
                return ErrorAnticollision;
            }

#ifdef uart_start
						
						SEGGER_RTT_printf(0,"UID:");
						SEGGER_RTT_printf(0,"%x",UID[0]);
						SEGGER_RTT_printf(0,"%x",UID[1]);
						SEGGER_RTT_printf(0,"%x",UID[2]);
						SEGGER_RTT_printf(0,"%x\r\n",UID[3]);
						
            //uart_puts("UID:");
            //uart_printBuffer(UID,4);
            //uart_newrow();
#endif
        }
        else
        {
            sky1311Reset();
#ifdef uart_start
					SEGGER_RTT_printf(0,"Anti error!\r\n");
           // uart_puts("\r\nAnti error!\r\n");
#endif
            return ErrorAnticollision;
        }
        sta = piccSelectA(sel,uid, tmpBuf);        // Select PICC
        if(Ok == sta)
        {
            SAK = tmpBuf[0];
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"SAK:");
					SEGGER_RTT_printf(0,"%x\r\n",tmpBuf[0]);
					
            //uart_puts("SAK:");
            //uart_printBuffer(tmpBuf,1);
#endif
        }
        else
        {
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\nSelect error!\r\n");
          //  uart_puts("\r\nSelect error!\r\n");
#endif
            return ErrorSelect;
        }


        if(uidSize!=0 && uid[0] == 0x88)
        {
            for(i=0;i<3;i++){
                uid[i] = uid[i+1];
            }
            uid += 3;
            sel += 2;

        }
    }while( (SAK&0x04)!=0 );

#ifdef uart_start
		
		SEGGER_RTT_printf(0,"UID:");

		SEGGER_RTT_printf(0,"%x",UID[0]);
		SEGGER_RTT_printf(0,"%x",UID[1]);
		SEGGER_RTT_printf(0,"%x",UID[2]);
		SEGGER_RTT_printf(0,"%x",UID[3]);
		SEGGER_RTT_printf(0,"%x",UID[4]);
		SEGGER_RTT_printf(0,"%x",UID[5]);
		SEGGER_RTT_printf(0,"%x",UID[6]);
		SEGGER_RTT_printf(0,"%x",UID[7]);
		SEGGER_RTT_printf(0,"%x",UID[8]);
		SEGGER_RTT_printf(0,"%x\r\n",UID[9]);
		SEGGER_RTT_printf(0,"SAK:%x\r\n",tmpBuf[0]);
		
//    uart_puts("UID:");
//    uart_printBuffer(UID,10);
//    uart_newrow();
//    uart_puts("SAK:");
//    uart_printBuffer(tmpBuf,1);

#endif
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Card a!\r\n");
		#endif
#if CARDTYPE==AUTO

    if((SAK & 0x24) == 0x20){           //UID complete, PICC compliant with ISO/IEC 14443-4
        sta = smartTypeA_test();
    }
    else if((SAK==0x08 || SAK == 0x18) && ((ATQA[1]&0xB4) == 0x04 || (ATQA[1]&0xB2) == 0x02)){// M1 card
  //      sta = mifare1_WriteTest(UID);
        sta = mifare1_ReadTest(UID);
    }
//		else
//		{
//			read_TypeA_Data[0] =0xFF;  //表示数据无效
//		}
#elif CARDTYPE==ONLYM1

//sta = mifare1_ReadTest(UID);

#elif CARDTYPE==ONLYSMART
  //  sta = smartTypeA_test();
#elif CARDTYPE==SKY1311TEST
  // sta = sky1311_ReadTest();
#endif
		
	mifare1_WriteTest(UID);  //写入 数据测试	
    sky1311Reset();
    return sta;
}




/*****************************************************

运行 Type_A 卡搜索 过程

读取到有效A卡时，在 Type_AB_UID 中写入当前A卡的UID号
并返回 OK 表示读卡成功

******************************************************/

uint8_t Type_AB_UID[20];  //AB卡UID数据缓存

extern uint8_t Start_Manual_Add_Card; 

uint8_t Type_A_Rrun ()
{
	
		sta_result_t sta;
    uint8_t SAK=0;
    uint8_t ATQA[2]={0};
    uint8_t UID[12]={0};
    uint8_t *uid;
    uint8_t sel=SEL1;
    uint8_t uidSize=0;
    uint8_t tmpBuf[10];
    uint8_t i;
		uint32_t it;  //计数，放死循环
		
		for(i =0;i<sizeof(UID);i++)
		UID[i] =0;
		
		for(i =0;i<sizeof(ATQA);i++)
		ATQA[i] =0;
		
		for(i =0;i<sizeof(tmpBuf);i++)
		tmpBuf[i] =0;
		
		for(i =0;i<sizeof(Type_AB_UID);i++)
		Type_AB_UID[i] =0;
		
    SKY1311_ENABLE(); 	    // chip enable PD2
    sky1311Init_A();			// init sky1311s
	  //resetPicc();            // 复位卡片()
    DelayMS(10);             // 等待卡片工作
    typeAOperate();
    //sta = piccRequestA(tmpBuf);     // send REQA command to card
    sta = piccWakeupA(tmpBuf);        // send WUPA command to card
		
		for(i =0;i<sizeof(Type_AB_UID);i++) Type_AB_UID[i] =0x00;  //清空UID缓存
		
		if(Ok == sta)
    {
			
        LED_ON();
        ATQA[0] = tmpBuf[1];
        ATQA[1] = tmpBuf[0];
        uidSize = tmpBuf[0]>>6;
			
#ifdef uart_start
			
				SEGGER_RTT_printf(0,"\r\nType-A Card Test\r\n");
				SEGGER_RTT_printf(0,"ATQA:"); //书应答字节
				SEGGER_RTT_printf(0,"%x\r\n",ATQA);
			
//        uart_puts("\r\nType-A Card Test\r\n");
//        uart_puts("ATQA:");
//        uart_printBuffer(ATQA,2);
//        uart_newrow();

#endif
			
    }
    else
    {
        sky1311Reset();
        return ErrorRequest;            // 这里返回，有可能是没卡或者没读到
    }
	  uid = &UID[0];
    sel = SEL1;	
		it =0;  //清空计数
		    do{
        sta = piccAntiA(sel, 1, uid);   // 防碰撞循环，测试获得一张卡的UID，参数'1'表示遇到冲突时选择1
        if(Ok == sta)
        {
            uint8_t checkSum = uid[0] ^ uid[1] ^ uid[2] ^ uid[3];
            if(checkSum != uid[4])
            {
							
								#ifdef uart_start
							
                SEGGER_RTT_printf(0,"\r\nAnti error!\r\n");

								#endif
							
                sky1311Reset();
                return ErrorAnticollision;
            }

#ifdef uart_start
						
						SEGGER_RTT_printf(0,"UID:");
						SEGGER_RTT_printf(0,"%x",UID[0]);
						SEGGER_RTT_printf(0,"%x",UID[1]);
						SEGGER_RTT_printf(0,"%x",UID[2]);
						SEGGER_RTT_printf(0,"%x\r\n",UID[3]);
						
            //uart_puts("UID:");
            //uart_printBuffer(UID,4);
            //uart_newrow();
#endif
						//获取到有效UID号
					
					if((Start_Manual_Add_Card==1)||(Start_Manual_Add_Card==3))  //当设备在 手动发卡状态 且为 A 卡模式时
						Start_Manual_Add_Card |=0x10;  //标志读到的是A卡
					
					for(i =0;i<sizeof(Type_AB_UID);i++)
						Type_AB_UID[i] =0;
					
					Type_AB_UID[0] = 0x01;  //起始标志数据为A卡数据
					
					Type_AB_UID[1] = UID[0];
					Type_AB_UID[2] = UID[1];
					Type_AB_UID[3] = UID[2];
					Type_AB_UID[4] = UID[3];
					
					
					
        }
        else
        {
            sky1311Reset();
#ifdef uart_start
					SEGGER_RTT_printf(0,"Anti error!\r\n");
           // uart_puts("\r\nAnti error!\r\n");
#endif
            return ErrorAnticollision;
        }
        sta = piccSelectA(sel,uid, tmpBuf);        // Select PICC
        if(Ok == sta)
        {
            SAK = tmpBuf[0];
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"SAK:");
					SEGGER_RTT_printf(0,"%x\r\n",tmpBuf[0]);
					
            //uart_puts("SAK:");
            //uart_printBuffer(tmpBuf,1);
#endif
        }
        else
        {
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\nSelect error!\r\n");
          //  uart_puts("\r\nSelect error!\r\n");
#endif
            return ErrorSelect;
        }


        if(uidSize!=0 && uid[0] == 0x88)
        {
            for(i=0;i<3;i++){
                uid[i] = uid[i+1];
            }
            uid += 3;
            sel += 2;

        }
			it++;	
			
			if(it>=20)
				{	
				it =0;  //当循环次数超过预设上限，it被清空 并结束这次循环
				break;
				}
    }while( (SAK&0x04)!=0 );
		
//			for(i =0;i<sizeof(UID);i++)
//		Type_AB_UID[i] = UID[i];  //将UID转存到外部
//		
	#ifdef uart_start   //输出ID号
		
		SEGGER_RTT_printf(0,"it: %d\r\n",it); //反馈读id号时的循环次数

		SEGGER_RTT_printf(0,"UID:");

		SEGGER_RTT_printf(0,"%x",UID[0]);
		SEGGER_RTT_printf(0,"%x",UID[1]);
		SEGGER_RTT_printf(0,"%x",UID[2]);
		SEGGER_RTT_printf(0,"%x",UID[3]);
		SEGGER_RTT_printf(0,"%x",UID[4]);
		SEGGER_RTT_printf(0,"%x",UID[5]);
		SEGGER_RTT_printf(0,"%x",UID[6]);
		SEGGER_RTT_printf(0,"%x",UID[7]);
		SEGGER_RTT_printf(0,"%x",UID[8]);
		SEGGER_RTT_printf(0,"%x\r\n",UID[9]);
		SEGGER_RTT_printf(0,"SAK:%x\r\n",tmpBuf[0]);
		
//    uart_puts("UID:");
//    uart_printBuffer(UID,10);
//    uart_newrow();
//    uart_puts("SAK:");
//    uart_printBuffer(tmpBuf,1);

	#endif
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Card a!\r\n");
		#endif
		
		Type_AB_UID[0] = 0xFF;  //起始标志数据为A卡数据
		
		if((SAK==0x08 || SAK == 0x18) && ((ATQA[1]&0xB4) == 0x04 || (ATQA[1]&0xB2) == 0x02)){// M1 card
  //      sta = mifare1_WriteTest(UID);
        sta = mifare1_ReadTest(UID);
    }
		else
		{
			
			read_TypeA_Data[0] =0xFF;  //设置数据无效
			sta =Error;
		}
		
	  sky1311Reset();  //重置 SKY1311 
		
		
    return sta;	
}


/*********************************

	不考虑密码 存粹读取 A卡 ID 号

**********************************/

uint8_t Type_A_Null_Key_Rrun ()
{
	
		sta_result_t sta;
    uint8_t SAK=0;
//    uint8_t ATQA[2]={0};
    uint8_t UID[12]={0};
    uint8_t *uid;
    uint8_t sel=SEL1;
    uint8_t uidSize=0;
    uint8_t tmpBuf[10];
    uint8_t i;
		uint32_t it;  //计数，放死循环
		
		for(i =0;i<sizeof(UID);i++)
		UID[i] =0;
		
//		for(i =0;i<sizeof(ATQA);i++)
//		ATQA[i] =0;
		
		for(i =0;i<sizeof(tmpBuf);i++)
		tmpBuf[i] =0;
		
		for(i =0;i<sizeof(Type_AB_UID);i++)
		Type_AB_UID[i] =0;
		
    SKY1311_ENABLE(); 	    // chip enable PD2
    sky1311Init_A();			// init sky1311s
	  //resetPicc();            // 复位卡片()
    DelayMS(10);             // 等待卡片工作
    typeAOperate();
    //sta = piccRequestA(tmpBuf);     // send REQA command to card
    sta = piccWakeupA(tmpBuf);        // send WUPA command to card
		
		for(i =0;i<sizeof(Type_AB_UID);i++) Type_AB_UID[i] =0x00;  //清空UID缓存
		
		if(Ok == sta)
    {
			
        LED_ON();
//        ATQA[0] = tmpBuf[1];
//        ATQA[1] = tmpBuf[0];
        uidSize = tmpBuf[0]>>6;
			
#ifdef uart_start
			
//				SEGGER_RTT_printf(0,"\r\nType-A Card Test\r\n");
//				SEGGER_RTT_printf(0,"ATQA:"); //书应答字节
//				SEGGER_RTT_printf(0,"%x\r\n",ATQA);
			
//        uart_puts("\r\nType-A Card Test\r\n");
//        uart_puts("ATQA:");
//        uart_printBuffer(ATQA,2);
//        uart_newrow();

#endif
			
    }
    else
    {
			
        sky1311Reset();
        return ErrorRequest;            // 这里返回，有可能是没卡或者没读到
    }
		
	  uid = &UID[0];
    sel = SEL1;	
		it =0;  //清空计数
		
		    do{
        sta = piccAntiA(sel, 1, uid);   // 防碰撞循环，测试获得一张卡的UID，参数'1'表示遇到冲突时选择1
        if(Ok == sta)
        {
          
						uint8_t checkSum = uid[0] ^ uid[1] ^ uid[2] ^ uid[3];
					
            if(checkSum != uid[4])
            {
							
								#ifdef uart_start
							
                SEGGER_RTT_printf(0,"\r\nAnti error!\r\n");

								#endif
							
                sky1311Reset();
                return ErrorAnticollision;
            }

#ifdef uart_start
						
						SEGGER_RTT_printf(0,"UID:");
						SEGGER_RTT_printf(0,"%x",UID[0]);
						SEGGER_RTT_printf(0,"%x",UID[1]);
						SEGGER_RTT_printf(0,"%x",UID[2]);
						SEGGER_RTT_printf(0,"%x\r\n",UID[3]);
						
            //uart_puts("UID:");
            //uart_printBuffer(UID,4);
            //uart_newrow();
#endif
						//获取到有效UID号
					
					if((Start_Manual_Add_Card==1)||(Start_Manual_Add_Card==3))  //当设备在 手动发卡状态 且为 A 卡模式时
						Start_Manual_Add_Card |=0x10;  //标志读到的是A卡
					
					for(i =0;i<sizeof(Type_AB_UID);i++)
						Type_AB_UID[i] =0;
					
					Type_AB_UID[0] = 0x01;  //起始标志数据为A卡数据
					
					Type_AB_UID[1] = UID[0];
					Type_AB_UID[2] = UID[1];
					Type_AB_UID[3] = UID[2];
					Type_AB_UID[4] = UID[3];
					
					
					
        }
        else
        {
            sky1311Reset();
#ifdef uart_start
					SEGGER_RTT_printf(0,"Anti error!\r\n");
           // uart_puts("\r\nAnti error!\r\n");
#endif
            return ErrorAnticollision;
        }
        sta = piccSelectA(sel,uid, tmpBuf);        // Select PICC
        if(Ok == sta)
        {
            SAK = tmpBuf[0];
					
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"SAK:");
					SEGGER_RTT_printf(0,"%x\r\n",tmpBuf[0]);
					
            //uart_puts("SAK:");
            //uart_printBuffer(tmpBuf,1);
#endif
        }
        else
        {
#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\nSelect error!\r\n");
          //  uart_puts("\r\nSelect error!\r\n");
#endif
            return ErrorSelect;
        }


        if(uidSize!=0 && uid[0] == 0x88)
        {
            for(i=0;i<3;i++){
                uid[i] = uid[i+1];
            }
            uid += 3;
            sel += 2;

        }
			it++;	
			
			if(it>=20)
				{	
				it =0;  //当循环次数超过预设上限，it被清空 并结束这次循环
				break;
				}
    }while( (SAK&0x04)!=0 );
		
//			for(i =0;i<sizeof(UID);i++)
//		Type_AB_UID[i] = UID[i];  //将UID转存到外部
//		
	#ifdef uart_start   //输出ID号
		
		SEGGER_RTT_printf(0,"it: %d\r\n",it); //反馈读id号时的循环次数

		SEGGER_RTT_printf(0,"UID:");

		SEGGER_RTT_printf(0,"%x",UID[0]);
		SEGGER_RTT_printf(0,"%x",UID[1]);
		SEGGER_RTT_printf(0,"%x",UID[2]);
		SEGGER_RTT_printf(0,"%x",UID[3]);
		SEGGER_RTT_printf(0,"%x",UID[4]);
		SEGGER_RTT_printf(0,"%x",UID[5]);
		SEGGER_RTT_printf(0,"%x",UID[6]);
		SEGGER_RTT_printf(0,"%x",UID[7]);
		SEGGER_RTT_printf(0,"%x",UID[8]);
		SEGGER_RTT_printf(0,"%x\r\n",UID[9]);
		SEGGER_RTT_printf(0,"SAK:%x\r\n",tmpBuf[0]);
		
//    uart_puts("UID:");
//    uart_printBuffer(UID,10);
//    uart_newrow();
//    uart_puts("SAK:");
//    uart_printBuffer(tmpBuf,1);

	#endif
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Card a!\r\n");
		#endif
		
	//	Type_AB_UID[0] = 0xFF;  //起始标志数据为A卡数据
		
//		if((SAK==0x08 || SAK == 0x18) && ((ATQA[1]&0xB4) == 0x04 || (ATQA[1]&0xB2) == 0x02)){// M1 card				//此处不设置ID
//  //      sta = mifare1_WriteTest(UID);
//        sta = mifare1_ReadTest(UID);
//    }
//		else
//		{
//			
//			read_TypeA_Data[0] =0xFF;  //设置数据无效
//		
//		}
		
	  sky1311Reset();  //重置 SKY1311 
		
		Type_AB_UID[0] = 0x01;  //起始标志数据为A卡数据
		
		Type_AB_UID[1] = UID[0];
		Type_AB_UID[2] = UID[1];
		Type_AB_UID[3] = UID[2];
		Type_AB_UID[4] = UID[3];
		read_TypeA_Data[0]=0x01;
		
    return sta;	
}





/***************************************************************
    TYPE B卡操作示例 （以读身份证串号为例）
   读取TYPE B卡的UID等，并使用部分APDU做测试
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t TypeB_test(void)
{
    sta_result_t sta;

    nAQTB_t ATQB;
    uint8_t tmpBuf[100];
    uint16_t tmpSize;
    uint8_t  attSize;
		uint8_t i;

    SKY1311_ENABLE(); 	    // chip enable PD2
    //DelayMS(5);
		sky1311Reset();
    sky1311Init_B();			// init sky1311s
    //resetPicc();            // 复位卡片
    DelayMS(10);             // 等待卡片工作
    typeBOperate();

    sta = piccRequestB(REQALL,0, 0, &ATQB);
    if(Ok != sta)
    {
        sky1311Reset();
        return NoResponse;
    }
		
		#ifdef uart_start
    
		SEGGER_RTT_printf(0,"\r\nType-B Card test\r\n");
		//uart_puts("\r\nType-B Card test\r\n");
    //LED_ON();

		#endif
		
		for(i =0;i<sizeof(tmpBuf);i++)  //清空缓存
			tmpBuf[i] =0;

    sta = piccAttrib(ATQB.PUPI, 0, 8, 0, 1, tmpBuf, 0, tmpBuf, &attSize);
    if( Ok != sta )
    {
        sky1311Reset();
        return NoResponse;
    }

    /* 读身份证ID  ISO14443-4 */
    tmpBuf[0] = 0x00;
    tmpBuf[1] = 0x36;
    tmpBuf[2] = 0x00;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x08;
    //sky1311_fifo_tx(TYPE_B, tmpBuf, 5);
    //sta = sky1311_fifo_rx(TYPE_B, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,5,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"ID Card: ");
			for(i =0;i<(tmpSize-2);i++)
				SEGGER_RTT_printf(0,"%x",tmpBuf[i]);
	
			SEGGER_RTT_printf (0,"\r\n");
			#endif
			
//		for(i =0;i<sizeof(Type_AB_UID);i++)
//			Type_AB_UID[i] =0;
//			
//			Type_AB_UID[0] = 0x02;  //起始标志数据为B卡数据
//			
//			for(i =0;i<8;i++)
//			{
//				Type_AB_UID[i+1] = tmpBuf[i];
//			}
			
//        uart_puts("ID Card:");
//        uart_printBuffer(tmpBuf,tmpSize-2);
//        uart_newrow();
    }
    sky1311Reset();
    return Ok;
}

/*******************************************************

运行 Type B 卡读取程序

********************************************************/

sta_result_t Type_B_Rrun(void)
{
	  sta_result_t sta;

    nAQTB_t ATQB;
    uint8_t tmpBuf[100];
    uint16_t tmpSize;
    uint8_t  attSize;
		uint8_t i;
	
		for(i =0;i<sizeof(Type_AB_UID);i++) Type_AB_UID[i] =0x00;  //清空 UID缓存
	
	   SKY1311_ENABLE(); 	    // chip enable PD2
    //DelayMS(5);
    sky1311Init_B();			// init sky1311s
    //resetPicc();            // 复位卡片
    DelayMS(10);             // 等待卡片工作
    typeBOperate();
		
			#ifdef uart_start
    
		SEGGER_RTT_printf(0,"\r\nType-B Card test!\r\n");
		//uart_puts("\r\nType-B Card test\r\n");
    //LED_ON();
		

		#endif
		
		sta = piccRequestB(REQALL,0, 0, &ATQB);  //寻找读卡范围内是否有B卡
    
		if(Ok != sta)
    {
        sky1311Reset();
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"\r\nType-B Card Error!\r\n");
			
			#endif
			
        return NoResponse;
    }
		for(i =0;i<sizeof(Type_AB_UID);i++)
		{
			Type_AB_UID[i] = 0;
		}
	
		#ifdef uart_start
    
		SEGGER_RTT_printf(0,"\r\nFind Type-B Card !\r\n");
		//uart_puts("\r\nType-B Card test\r\n");
    //LED_ON();
		
		#endif
	 
		sta = piccAttrib(ATQB.PUPI, 0, 8, 0, 1, tmpBuf, 0, tmpBuf, &attSize);	
		
	
    /* 读身份证ID  ISO14443-4 */
    tmpBuf[0] = 0x00;
    tmpBuf[1] = 0x36;
    tmpBuf[2] = 0x00;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x08;
    //sky1311_fifo_tx(TYPE_B, tmpBuf, 5);
    //sta = sky1311_fifo_rx(TYPE_B, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,5,tmpBuf,&tmpSize);		
		
  if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
			
			#ifdef uart_start
			SEGGER_RTT_printf(0,"ID Card: ");
			for(i =0;i<(tmpSize-2);i++)
				SEGGER_RTT_printf(0,"%x",tmpBuf[i]);
	
			SEGGER_RTT_printf (0,"\r\n");
			#endif
			
			Type_AB_UID[0] = 0x02;  //表示 UID 号是身份证
			for(i =0;i<8;i++)
			{
				Type_AB_UID[i+1] = tmpBuf[i];
			}
			
//        uart_puts("ID Card:");
//        uart_printBuffer(tmpBuf,tmpSize-2);
//        uart_newrow();
    }
	 
		sky1311Reset();
		
    return Ok;
}
