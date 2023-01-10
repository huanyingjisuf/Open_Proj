/******************************************************************************/
/*               (C) ˹���������������Ƽ����޹�˾(SKYRELAY)                   */
/*                                                                            */
/* �˴�����˹���������������Ƽ����޹�˾Ϊ֧�ֿͻ���д��ʾ�������һ����       */
/* ����ʹ��˹���������оƬ���û������޳�ʹ�øô��룬���豣������������       */
/* �������Ϊ��ʾʹ�ã�����֤�������е�Ӧ��Ҫ�󣬵����ض��淶������ʱ��       */
/* ʹ����������ȷ�ϴ����Ƿ����Ҫ�󣬲���Ҫ��ʱ����������Ӧ���޸ġ�           */
/* (V1.10)                                                                    */
/******************************************************************************/
/**
 ******************************************************************************
 ** �� �� ���� iso14443_3a.c
 **
 ** �ļ������� ����ISO14443-3 type A�Ĳ��������������ͱ���
 **
 ** �汾��ʷ:
 ** 2015-10-05 V1.00  EH   ��һ����ʽ�汾
 ** 2017-3-25  V.10  Release    ���´���ͺ����ṹ�����·���
 ** 2018-09-02 V1.2.0 Release   �޸��˲��ֺ�������������ͻ�㷨����
 ******************************************************************************/
#include "iso14443.h"


/**
 ******************************************************************************
 ** \��  ��  type A "Request" ����,command = 26H
 **
 ** \��  ��  none
 ** \����ֵ  sta_result_t ����״̬��Ok���ɹ���������ʧ��
 ******************************************************************************/
sta_result_t piccRequestA(uint8_t *ATQA)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[1];
    tmpBuf[0] = REQA;       // 0x26
    sky1311WriteReg(ADDR_TX_CTRL, TX_POLE_HIGH|TX_PARITY_ODD);      // TX odd parity, no CRC
    sky1311WriteReg(ADDR_RX_CTRL, RX_PARITY_EN|RX_PARITY_ODD);      // RX odd parity, no CRC
    sky1311WriteReg(ADDR_TX_BIT_NUM, 0x07);
    sky1311WriteReg(ADDR_TX_BYTE_NUM, 0x01);

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 1);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, ATQA, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,1,ATQA,&tmpSize);

    if(sta == Ok && tmpSize == 2)
        return Ok;
    else
        return NoResponse;
}

/**
 ******************************************************************************
 ** \��  ��  type A "Wake-Up" ����,command = 52H
 **
 ** \��  ��  none
 ** \����ֵ  sta_result_t ����״̬��Ok���ɹ���������ʧ��
 ******************************************************************************/
sta_result_t piccWakeupA(uint8_t *ATQA)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[1];

    tmpBuf[0] = WUPA;       // 0x52
    sky1311WriteReg(ADDR_TX_CTRL, TX_POLE_HIGH|TX_PARITY_ODD);      // TX odd parity, no CRC
    sky1311WriteReg(ADDR_RX_CTRL, RX_PARITY_EN|RX_PARITY_ODD);      // RX odd parity, no CRC
    sky1311WriteReg(ADDR_TX_BIT_NUM, 0x07);
    sky1311WriteReg(ADDR_TX_BYTE_NUM, 0x01);

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 1);
	//sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, ATQA, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,1,ATQA,&tmpSize);
    return sta;
//    if(tmpSize != 2)
//        return Error;
//    else
//        return sta;
}
/**
 ******************************************************************************
 ** \��  ��  type A������ͻѭ��
 **
 ** \��  ��  SEL,  (93h, 95h, 97h)
 **          uint8_t randBit λ��ͻʱѡ�������0��1;
 **          uint8_t* uid ��õ�UID����������׵�ַ
 ** \����ֵ  return Ok on success, Error on error
 ******************************************************************************/
sta_result_t piccAntiA(uint8_t SEL, uint8_t rand_bit, uint8_t *uid)
{
    uint8_t NVB= 0x20;
    uint8_t currValidBytes=0, currValidBits=0;
    uint8_t recBytes, recBits;
    uint8_t hasCollision = 0;
	uint8_t tmpSize = 0;
	uint8_t tmpBuf[12];
	uint8_t g_rx_buff[12];
    uint8_t i;
    uint8_t recBit8;

    /* config registor of sky1311 */
    sky1311WriteReg(ADDR_RX_NUM_H, COLL_EN);        // anti-collision on
    sky1311WriteReg(ADDR_TX_CTRL, TX_POLE_HIGH|TX_PARITY_ODD);
    sky1311WriteReg(ADDR_RX_START_BIT_NUM,0);
    /* STEP 1: send SEL + 20H to PICC */
    tmpBuf[0] = SEL;
    tmpBuf[1] = NVB;
    if(Ok != bitCollisionTrans(tmpBuf, 2 , 8)){
        sky1311WriteReg(ADDR_RX_NUM_H, 0x00);               // disable anti-collision
        return Error;
    }
    delay(5);
    do{
        /* ����PICC���ص������ֽ����Ͳ�������λ���������ݱ��浽������ */
        recBytes = sky1311ReadReg(ADDR_RX_NUM_L);
        recBit8 = sky1311ReadReg(ADDR_RX_NUM_H);
        recBits = (recBit8&0xf0)>>4;
        //recBits = ((sky1311ReadReg(ADDR_RX_NUM_H) & 0xf0) >> 4);
        tmpSize = sky1311ReadReg(ADDR_FIFO_LEN);
        sky1311ReadFifo(g_rx_buff, tmpSize);
        if((sky1311ReadReg(ADDR_RX_NUM_H) & RX_FORBID) == 0)   // no collision happens
            hasCollision = 0;
        else
            hasCollision = 1;

        /* ���յ���UID���ݺ�֮ǰ������ƴ�Ӻϲ� */
        if(currValidBits == 0){         // ��ǰUIDȫ�����������ֽڣ����ֽ�ƴ��
            for(i=0;i<tmpSize;i++){
                uid[currValidBytes++] = g_rx_buff[i];
            }
            currValidBits = recBits;
        }
        else{                           // ��ǰUID���һ���ֽڲ���������һ���ֽ���Ҫ��λƴ��
             if(recBytes == 0){              // ���յ�һ���ֽ������ݣ�����Ҳ��������
                uid[currValidBytes-1] += g_rx_buff[0]<<currValidBits;    // ��λƴ�ӵ�һ���ֽ�
                currValidBits += recBits;
                if(currValidBits==8){
                    currValidBytes++;
                    currValidBits = 0;
                }
            }
            else{                           // �յ�����ֽ�����
                uid[currValidBytes-1] |= g_rx_buff[0];//<<currValidBits;   // ��λƴ�ӵ�һ���ֽ�
                for(i=1;i<tmpSize;i++){           // ����ʣ����ֽ�
                    uid[currValidBytes++] = g_rx_buff[i];
                }
                currValidBits = recBits;
            }
        }
        if(hasCollision){

            /* ��UID���ݼ���rand_bit */
            if(currValidBits==0){
                uid[currValidBytes] = rand_bit;
                currValidBits = 1;
                NVB = 0x20+((currValidBytes)<<4) |  currValidBits;
                currValidBytes++;
            }
            else if(currValidBits == 7){
                uid[currValidBytes-1] += rand_bit<<7;
                currValidBits = 0;
                NVB = 0x20+((currValidBytes)<<4);
            }
            else{       // currValidBits= 1 ... 6
                uid[currValidBytes-1] += rand_bit<<currValidBits;
                currValidBits++;
                NVB = 0x20+((currValidBytes-1)<<4) |  currValidBits;
            }

            tmpBuf[0] = SEL;
            tmpBuf[1] = NVB;
            for(uint8_t i=0;i<currValidBytes;i++){
                tmpBuf[2+i] = uid[i];
            }
            sky1311WriteReg(ADDR_RX_START_BIT_NUM,currValidBits);
            if(Ok != bitCollisionTrans(tmpBuf, currValidBytes+2 , currValidBits)){
                sky1311WriteReg(ADDR_RX_NUM_H, 0x00);// disable anti-collision
                sky1311WriteReg(ADDR_RX_START_BIT_NUM,0);
                return Error;
            }
        }

    }while(hasCollision);
    sky1311WriteReg(ADDR_RX_NUM_H, 0x00); // disable anti-collision
    sky1311WriteReg(ADDR_RX_START_BIT_NUM,0);
    return Ok;
}

/**
 ******************************************************************************
 ** \��  ��  type Aѡ��
 **
 ** \��  ��  uint8_t SEL, ѡ�����У�93H��95H�� 97H
 **          uint8_t *uid, PICC 4 bytes UID
 **          uint8_t *SAK
 ** \����ֵ  sta_result_t�� ѡ��״ֵ̬
 ******************************************************************************/
sta_result_t piccSelectA(uint8_t SEL,uint8_t *uid, uint8_t *SAK)
{
    sta_result_t sta;
		uint16_t tmpSize;
		uint8_t tmpBuf[10];

    sky1311WriteReg(ADDR_TX_CTRL, TX_CRC_EN|TX_PARITY_ODD|TX_POLE_HIGH); // TX odd parity, with CRC
    sky1311WriteReg(ADDR_RX_CTRL, RX_CRC_EN|RX_PARITY_EN|RX_PARITY_ODD);
    sky1311WriteReg(ADDR_TX_BIT_NUM, 0x08);                              // complete bytes

    tmpBuf[0] = SEL;
    tmpBuf[1] = 0x70;
    tmpBuf[2] = uid[0];
    tmpBuf[3] = uid[1];
    tmpBuf[4] = uid[2];
    tmpBuf[5] = uid[3];
    tmpBuf[6] = uid[4];

	//sky1311_fifo_tx(TYPE_A, tmpBuf, 7);
	//sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, SAK, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,7,SAK,&tmpSize);
    if(sta == Ok && tmpSize == 3)
        return Ok;
    else
        return NoResponse;
}
/**
 ******************************************************************************
 ** \��  ��  type A����,��ֹ�Կ��Ĳ���
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void piccHaltA(void)
{
    sky1311WriteReg(ADDR_FIFO, 0x50);
    sky1311WriteReg(ADDR_FIFO, 0x00);
    sky1311WriteReg(ADDR_TX_BYTE_NUM, 0x02);
    sky1311WriteCmd(CMD_TX);                     // CMD TX
}
/*****************************************************************************/
/*                      ������ISO14443-4����  �����ã�                        */
/*****************************************************************************/

#define TCL_I_BLOCK               0x02    //I-Block                             000x xx1x
#define TCL_I_BLOCK_RFU           0x02    //I-BLOCK_RFU                         00xx xx1x
#define TCL_R_BLOCK               0xA2    //R-Block                             1010 x01x
#define TCL_R_BLOCK_RFU           0xA0    //R-Block_RFU                         1010 x0xx
#define TCL_R_BLOCK_ACK           0xA2    //R-Block-ACK                         1010 x01x
#define TCL_R_BLOCK_ACK_RFU       0xA0    //R-Block-ACK_RFU                     1010 x0xx
#define TCL_R_BLOCK_NAK           0xB2    //R-Block-NAK                         1011 x01x
#define TCL_R_BLOCK_NAK_RFU       0xB0    //R-Block-NAK_RFU                     1011 x0xx
#define TCL_S_BLOCK_DES           0xC2    //S-Block                             1100 x010
#define TCL_S_BLOCK_DES_RFU       0xC0    //S-Block_RFU                         1100 x0xx
#define TCL_S_BLOCK_WTX           0xF2    //S-Block                             1111 x010
#define TCL_S_BLOCK_WTX_RFU       0xF0    //S-Block_RFU                         1111 x0xx
#define TCL_PPSS_BLOCK            0xD0    //PPS S-Block                         1101 xxxx
#define POWER_LEVEL_INDICATION    0x00    //CID byte                            xx00 0000
//#define TCL_S_BLOCK               0xC2    //S-Block                             11xx x010

#define TCL_M_I_BLOCK             0xE2    //I-BLOCK MASK                        1110 0010
#define TCL_M_I_BLOCK_RFU         0xC2    //I-BLOCK_RFU MASK                    1100 0010
#define TCL_M_R_BLOCK             0xE6    //R-BLOCK MASK                        1110 0110
#define TCL_M_R_BLOCK_RFU         0xE4    //R-BLOCK_RFU MASK                    1110 0100
//#define TCL_M_S_BLOCK             0xF6    //S-BLOCK MASK                        1111 0111
#define TCL_M_S_BLOCK             0xF7    //S-BLOCK MASK                        1111 0111
#define TCL_M_S_BLOCK_RFU         0xF4    //S-BLOCK_RFU MASK                    1111 0100
#define TCL_M_CHAINING            0x10    //CHAIN MASK                          0001 0000
#define TCL_M_CID                 0x08    //CID following                       0000 1000
#define TCL_M_NAD                 0x04    //NAD following                       0000 0100
#define TCL_M_BLK_NUM             0x01    //BLOCK NUM MASK                      0000 0001
#define TCL_M_NAK                 0x10    //R-NAK                               0001 0000
#define TCL_M_POWER_LEVEL         0x00    //Power Level Indication              0000 0000
#define TCL_M_WTXM                0x3F    //Waiting Time eXtension Multipier    0011 1111

uint8_t  g_FWI;
uint8_t  g_bCID;
uint16_t  g_bFSD;
uint16_t  g_bFSC;
uint8_t  g_bBlockNum;
uint8_t  g_bCIDFollowed;

uint8_t  g_bLastTransmitPCB;
uint8_t  * g_pbaLastTransmitBuf;
uint16_t g_wLastTransmitLength;
/**
 ******************************************************************************
 ** \��  ��  ȡ��ѡ��
 **
 ** \��  ��  ������
 ** \����ֵ  sta_result_t��״̬
 ******************************************************************************/
sta_result_t piccDeselectA(void)
{
    sta_result_t sta;
	uint16_t tmpSize;
	uint8_t tmpBuf[2];
	uint8_t g_rx_buff[4];

    tmpBuf[0] = 0xCA;
    tmpBuf[1] = 0x00;

	//sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
	//sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, g_rx_buff, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,g_rx_buff,&tmpSize);

    if(sta == Ok && tmpSize == 4)
        return Ok;
    else
        return NoResponse;
}
/**
 ******************************************************************************
 ** \��  ��  ��ȡtype A����ATSֵ
 **
 ** \��  ��  none
 ** \����ֵ  sta_result_t��ATSֵ
 ******************************************************************************/
#if 0
void getFSCFSD(uint8_t bFSCIFSDI, uint8_t bType)
{
    if(bFSCIFSDI > 8)
    {
        bFSCIFSDI = 8;
    }
    if(bType == 0)
    {
        switch(bFSCIFSDI)
        {
        case 0:
            g_bFSC = 16;
            break;

        case 1:
            g_bFSC = 24;
            break;

        case 2:
            g_bFSC = 32;
            break;

        case 3:
            g_bFSC = 40;
            break;

        case 4:
            g_bFSC = 48;
            break;

        case 5:
            g_bFSC = 64;
            break;

        case 6:
            g_bFSC = 96;
            break;

        case 7:
            g_bFSC = 128;
            break;

        case 8:
            g_bFSC = 256;
            break;
        }
    }
    else
    {
        switch(bFSCIFSDI)
        {
        case 0:
            g_bFSD = 16;
            break;

        case 1:
            g_bFSD = 24;
            break;

        case 2:
            g_bFSD = 32;
            break;

        case 3:
            g_bFSD = 40;
            break;

        case 4:
            g_bFSD = 48;
            break;

        case 5:
            g_bFSD = 64;
            break;

        case 6:
            g_bFSD = 96;
            break;

        case 7:
            g_bFSD = 128;
            break;

        case 8:
            g_bFSD = 256;
            break;
        }
    }
}
#endif
sta_result_t piccATS(uint8_t *ATS)
{
	uint8_t count;
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[2];
    count = 1;
    g_FWI = 4;
    tmpBuf[0] = 0xE0;
    tmpBuf[1] = 0x80;

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, ATS, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,ATS,&tmpSize);

    if(sta == Ok && (tmpSize == ATS[0] + 2))
    {
        g_bCID = tmpBuf[0] & 0x0F;

        if(ATS[1] & 0x10)
        {
            count++;
            g_FWI = ((ATS[2] >> 4) & 0x0F);
            if(g_FWI > 14)
            {
                g_FWI = 14;
            }
        }

        if(ATS[1] & 0x20)
        {
            count++;
        }

        if(ATS[1] & 0x40)
        {
            count++;

            if(ATS[count] & 0x02)
            {
                g_bCIDFollowed = 1;
            }
            else
            {
                g_bCIDFollowed = 0;
            }
        }
        else
        {
            g_bCIDFollowed = 0;
        }

        g_bBlockNum = 0;
        //getFSCFSD((ATS[1] >> 4) & 0x0F, 0);
        //getFSCFSD((tmpBuf[1] >> 4) & 0x0F, 1);

        //frame_delay = 32 * (2 << g_FWI) + 10;
        //timer0_value = (uint8_t)(frame_delay & 0x0000ff);
        //timer1_value = (uint8_t)((frame_delay >> 8) & 0x0000ff);
        //timer2_value = (uint8_t)((frame_delay >> 16) & 0x00003f);
        //timer2_value = timer2_value + 0x40;
    }
    else
        return NoResponse;
    return sta;
}

/**
 ******************************************************************************
 ** \��  ��  ��ȡPPSֵ
 **
 ** \��  ��  ���õı���
 ** \����ֵ  sta_result_t��PPSֵ
 ******************************************************************************/
sta_result_t piccPPS(uint16_t high_rate, uint8_t *pps)
{
    sta_result_t sta;
    uint8_t tmpBuf[3]={0xD0,0x11,0x00};
    uint16_t tmpSize;

    if(high_rate == 212)
        tmpBuf[2] = 0x05;                   // RATE = 212k
    else if(high_rate == 424)
        tmpBuf[2] = 0x0A;                   // RATE = 424k

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 3);
	//sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, pps, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,3,pps,&tmpSize);

    return sta;
}




