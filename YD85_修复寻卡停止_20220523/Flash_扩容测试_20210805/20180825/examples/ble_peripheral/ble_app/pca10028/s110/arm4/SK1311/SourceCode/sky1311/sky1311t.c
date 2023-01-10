/******************************************************************************/
/*               (C) ˹���������������Ƽ����޹�˾(SKYRELAY)                   */
/*                                                                            */
/* �˴�����˹���������������Ƽ����޹�˾Ϊ֧�ֿͻ���д��ʾ�������һ����       */
/* ����ʹ��˹���������оƬ���û������޳�ʹ�øô��룬���豣������������       */
/* �������Ϊ��ʾʹ�ã�����֤�������е�Ӧ��Ҫ�󣬵����ض��淶������ʱ��       */
/* ʹ����������ȷ�ϴ����Ƿ����Ҫ�󣬲���Ҫ��ʱ����������Ӧ���޸ġ�           */
/* (V1.13)                                                                    */
/******************************************************************************/
/**
 ******************************************************************************
 ** �� �� ���� sky1311T.c
 **
 ** �ļ������� ����sky1311s�����к����������ͱ���
 **
 ** �汾��ʷ:
 ** 2015-10-05 V1.00  EH   ��һ����ʽ�汾
 ** 2017-3-25  V.10  Release    ���´���ͺ����ṹ�����·���
 ** 2018-06-12 V1.1.2 Release   ���������շ��������޸Ĳ��ִ�������������
 ** 2018-09-02 V1.2.0 Release   �޸��˶ദ���������������շ��������޸��˲��ֺ����Ľṹ��ʵ�ַ�ʽ
 ******************************************************************************/
#include "mcu.h"
#include "sky1311_drv.h"
#include "sky1311t_reg.h"
#include "typedef.h"
#include "userdef.h"
/**
 ******************************************************************************
 ** \������ ȫ�ֱ�������
 **
 ******************************************************************************/
#if CHECKCARD==1
    uint8_t maxRCADVal=MAXADVAL;         // ��⵽�����ADֵ
    boolean freqScanNeed=true;  // ϵͳ��ҪRCƵ��ɨ��
#endif

uint8_t  err_sta;   // ����״̬����
uint8_t  irq_sta;   // �ж�����״̬����

/**
 ******************************************************************************
 ** \��  ��  ���üĴ�������λ
 **
 ** \��  ��  uint8_t regAddr �Ĵ�����ַ�� uint8_t mask Ҫ���õ�"����"�ֽ�
 ** \����ֵ  none
 ******************************************************************************/
void SetBitMask(uint8_t regAddr, uint8_t mask)
{
    uint8_t tmp;
    tmp = sky1311ReadReg(regAddr);
    sky1311WriteReg(regAddr, tmp|mask);
}
/**
 ******************************************************************************
 ** \��  ��  ����Ĵ�������λ
 **
 ** \��  ��  uint8_t regAddr �Ĵ�����ַ�� uint8_t mask Ҫ�����"����"�ֽ�
 ** \����ֵ  none
 ******************************************************************************/
void ClearBitMask(uint8_t regAddr, uint8_t mask)
{
    uint8_t tmp;
    tmp = sky1311ReadReg(regAddr);
    sky1311WriteReg(regAddr, tmp & ~mask);
}

/**
 ******************************************************************************
 ** \��  ��  ������Բ�����ʼ��
 **
 ** \��  ��  ѡ��ͨ��MFOUT��������źţ�
 **          bit2,1,0
 **             0 0 0 rx phase signal after demod, before sample
 **             0 0 1 tx bit stream
 **             0 1 0 rx bit stream
 **             0 1 1 rx data from analog
 **             1 0 0 DO from digital
 ** \����ֵ  none
 ******************************************************************************/
/*static void directModeInit(uint8_t mfout_sel)
{
    sky1311WriteReg(ADDR_MFOUT_SEL,  mfout_sel);
}*/

/**
 ******************************************************************************
 ** \��  ��  ��PCD����
 **
 ** \��  ��  PCD���ͣ� TYPE_A, TYPE_B
 ** \����ֵ  none
 ******************************************************************************/
void pcdAntennaOn(uint8_t pcdType)
{
    if(pcdType == TYPE_A){
        sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
    }else if(pcdType == TYPE_B){
        sky1311WriteReg(ADDR_ANA_CFG2, ANA2_B);
    }
    //SetBitMask(ADDR_TX_CTRL, TX_POLE);
    SetBitMask(ADDR_ANA_CFG1, TX_EN);
}

/**
 ******************************************************************************
 ** \��  ��  �ر�PCD����
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void pcdAntennaOff(void)
{
    ClearBitMask(ADDR_ANA_CFG1, TX_EN);  // Disable TX
    //ClearBitMask(ADDR_TX_CTRL, TX_POLE);
}
/**
 ******************************************************************************
 ** \��  ��  ��������жϱ��
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void irqClearAll(void)
{
    sky1311WriteReg(ADDR_IRQ_STA, 0xFF);
}
/**
 ******************************************************************************
 ** \��  ��  ���ָ�����жϱ��
 **
 ** \��  ��  uint8_t irq
 ** \����ֵ  none
 ******************************************************************************/
void irqClear(uint8_t irq)
{
    sky1311WriteReg(ADDR_IRQ_STA,irq);
}
/**
 ******************************************************************************
 ** \��  ��  ����sky1311�뿨ͨ�ŵı���, ISO14443-4���ֹ涨���౶�ٵ�PICC
 **
 ** \��  ��  ���õı��٣�106��212��424
 ** \����ֵ  none
 ******************************************************************************/
void bitRateSet(uint16_t high_rate)
{
    sky1311WriteReg(ADDR_FSM_STATE, RATE_SEL);                           // enable high-speed mode
    if(high_rate == 212){
        sky1311WriteReg(ADDR_TX_CTRL, TX_212|TX_POLE_HIGH|TX_CRC_EN|TX_PARITY_ODD);
        sky1311WriteReg(ADDR_RATE_CTRL, RX_RATE_WID_2|RX_RATE_212|RX_RATE_PAR_ODD|RX_RATE_CRC_EN|RX_RATE_PAR_EN);
        sky1311WriteReg(ADDR_TX_PUL_WID,0x14);                           // tx pulse width = 1.5u
        sky1311WriteReg(ADDR_RATE_FRAME_END,0x40);                       // stop counter = 0x40
        sky1311WriteReg(ADDR_RATE_THRES,0x20);                           // threshold = 0x20
    }else if(high_rate == 424){
        sky1311WriteReg(ADDR_TX_CTRL, TX_424|TX_POLE_HIGH|TX_CRC_EN|TX_PARITY_ODD);
        sky1311WriteReg(ADDR_RATE_CTRL, RX_RATE_WID_2|RX_RATE_424|RX_RATE_PAR_ODD|RX_RATE_CRC_EN|RX_RATE_PAR_EN);
        sky1311WriteReg(ADDR_TX_PUL_WID,0x0A);                           // tx pulse width = 0.8u
        sky1311WriteReg(ADDR_RATE_FRAME_END,0x20);                       // stop counter = 0x20
        sky1311WriteReg(ADDR_RATE_THRES,0x10);                           // threshold = 0x10
    }
}
/**
 ******************************************************************************
 ** \��  ��  ѡ��type A ����Ϊ��������
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void typeAOperate(void)
{
    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);             // analogA select
    sky1311WriteReg(ADDR_FSM_STATE, TYPE_A_SEL);        // typeA select
    sky1311WriteReg(ADDR_CRC_CTRL, CRC_A);              // crcA enable
    sky1311WriteReg(ADDR_TX_PUL_WID,0x26);              // set to default value
    sky1311WriteReg(ADDR_M1_CTRL, 0x00);                // disable M1 operation
    sky1311WriteReg(ADDR_ANA_CFG3, 0x09);
    sky1311WriteReg(ADDR_ANA_CFG3, 0x81);
}
/**
 ******************************************************************************
 ** \��  ��  ѡ��type B ����Ϊ��������
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void typeBOperate(void)
{
    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_B);            // analogB select
    sky1311WriteReg(ADDR_FSM_STATE, TYPE_B_SEL);       // typeB select
    sky1311WriteReg(ADDR_CRC_CTRL, CRC_B);             // crcB enable
    sky1311WriteReg(ADDR_TX_PUL_WID,0x26);              // set to default value
    sky1311WriteReg(ADDR_M1_CTRL, 0x00);                // disable M1 operation
    sky1311WriteReg(ADDR_ANA_CFG3, 0x09);
    sky1311WriteReg(ADDR_ANA_CFG3, 0x81);
}
/**
 ******************************************************************************
 ** \��  ��  ģ�������ʼ��������ģ������Ĵ���
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
static void analogInit(void)
{
    sky1311WriteReg(ADDR_ANA_CFG0, RD_PAVOL); //  7-4: RC OSC Freq
                                              //  3-2: PA Driver
                                              //  1: ixtal
                                              //  0: External LDO

    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
    //sky1311WriteReg(ADDR_ANA_CFG1, 0xFC);   //  7: txen
                                              //  6: rxen
                                              //  5-4: clk_sel
                                              //     00(osc off, xtal off)
                                              //     01(osc on, xtal off)
                                              //     10(osc off, xtal 13.56)
                                              //     11(osc off, xtal 27.12)
    sky1311WriteReg(ADDR_ANA_CFG1, 0x00);
    //DelayMS(1);
    delay(100);
    sky1311WriteReg(ADDR_ANA_CFG1, 0xFC);   //sky1311WriteReg(ADDR_ANA_CFG1, 0xF4);
}

static void analogInit_A(void)
{
    sky1311WriteReg(ADDR_ANA_CFG0, RD_PAVOL_A); //  7-4: RC OSC Freq
                                              //  3-2: PA Driver
                                              //  1: ixtal
                                              //  0: External LDO

    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
    //sky1311WriteReg(ADDR_ANA_CFG1, 0xFC);   //  7: txen
                                              //  6: rxen
                                              //  5-4: clk_sel
                                              //     00(osc off, xtal off)
                                              //     01(osc on, xtal off)
                                              //     10(osc off, xtal 13.56)
                                              //     11(osc off, xtal 27.12)
    sky1311WriteReg(ADDR_ANA_CFG1, 0x00);
    //DelayMS(1);
    delay(100);
    sky1311WriteReg(ADDR_ANA_CFG1, 0xFC);   //sky1311WriteReg(ADDR_ANA_CFG1, 0xF4);
}

static void analogInit_B(void)
{

	sky1311WriteReg(ADDR_ANA_CFG0, RD_PAVOL_B); //  7-4: RC OSC Freq
                                              //  3-2: PA Driver
                                              //  1: ixtal
                                              //  0: External LDO

    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
    //sky1311WriteReg(ADDR_ANA_CFG1, 0xFC);   //  7: txen
                                              //  6: rxen
                                              //  5-4: clk_sel
                                              //     00(osc off, xtal off)
                                              //     01(osc on, xtal off)
                                              //     10(osc off, xtal 13.56)
                                              //     11(osc off, xtal 27.12)
    sky1311WriteReg(ADDR_ANA_CFG1, 0x00);
    //DelayMS(1);
    delay(100);
    sky1311WriteReg(ADDR_ANA_CFG1, 0xFC);   //sky1311WriteReg(ADDR_ANA_CFG1, 0xF4);

}



/**
 ******************************************************************************
 ** \��  ��  ��ʼ��SKY1311S
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void sky1311Init(void)
{
    sky1311WriteCmd(CMD_SW_RST);                // reset status
    analogInit();
//    while(0==(irq_sta & 0x02))
//    {
//        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
//    }
    DelayMS(3);  //Crystal Stable time
    sky1311WriteReg(ADDR_TIME_OUT2, 0x8F);       // time_out timer stop condition = beginning of RX SOF
    sky1311WriteReg(ADDR_TIME_OUT1, 0xFF);
    sky1311WriteReg(ADDR_TIME_OUT0, 0xFF);
    sky1311WriteReg(ADDR_RX_PUL_DETA, 0x34);     // ��4λʶ������˹��������Խ���ݴ�����Խǿ
    sky1311WriteReg(ADDR_RX_PRE_PROC, 0x00);
    sky1311WriteReg(ADDR_RX_START_BIT_NUM,0);
    sky1311WriteReg(ADDR_MOD_SRC,0x02);
    sky1311WriteReg(ADDR_IRQ_EN, IRQ_M1_EN|IRQ_TOUT_EN|IRQ_TX_EN|IRQ_RX_EN|IRQ_HIGH_EN|IRQ_LOW_EN);    // enable all IRQ
    sky1311WriteReg(ADDR_FIFO_CTRL,8);            // set water-level of FIFO
}

void sky1311Init_A(void)
{
    sky1311WriteCmd(CMD_SW_RST);                // reset status
    analogInit_A();
//    while(0==(irq_sta & 0x02))
//    {
//        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
//    }
    DelayMS(3);  //Crystal Stable time
    sky1311WriteReg(ADDR_TIME_OUT2, 0x8F);       // time_out timer stop condition = beginning of RX SOF
    sky1311WriteReg(ADDR_TIME_OUT1, 0xFF);
    sky1311WriteReg(ADDR_TIME_OUT0, 0xFF);
    sky1311WriteReg(ADDR_RX_PUL_DETA, 0x34);     // ��4λʶ������˹��������Խ���ݴ�����Խǿ
    sky1311WriteReg(ADDR_RX_PRE_PROC, 0x00);
    sky1311WriteReg(ADDR_RX_START_BIT_NUM,0);
    sky1311WriteReg(ADDR_MOD_SRC,0x02);
    sky1311WriteReg(ADDR_IRQ_EN, IRQ_M1_EN|IRQ_TOUT_EN|IRQ_TX_EN|IRQ_RX_EN|IRQ_HIGH_EN|IRQ_LOW_EN);    // enable all IRQ
    sky1311WriteReg(ADDR_FIFO_CTRL,8);            // set water-level of FIFO
}

void sky1311Init_B(void)
{
    sky1311WriteCmd(CMD_SW_RST);                // reset status
    analogInit_B();
//    while(0==(irq_sta & 0x02))
//    {
//        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
//    }
    DelayMS(3);  //Crystal Stable time
    sky1311WriteReg(ADDR_TIME_OUT2, 0x8F);       // time_out timer stop condition = beginning of RX SOF
    sky1311WriteReg(ADDR_TIME_OUT1, 0xFF);
    sky1311WriteReg(ADDR_TIME_OUT0, 0xFF);
    sky1311WriteReg(ADDR_RX_PUL_DETA, 0x34);     // ��4λʶ������˹��������Խ���ݴ�����Խǿ
    sky1311WriteReg(ADDR_RX_PRE_PROC, 0x00);
    sky1311WriteReg(ADDR_RX_START_BIT_NUM,0);
    sky1311WriteReg(ADDR_MOD_SRC,0x02);
    sky1311WriteReg(ADDR_IRQ_EN, IRQ_M1_EN|IRQ_TOUT_EN|IRQ_TX_EN|IRQ_RX_EN|IRQ_HIGH_EN|IRQ_LOW_EN);    // enable all IRQ
    sky1311WriteReg(ADDR_FIFO_CTRL,8);            // set water-level of FIFO
}




/**
 ******************************************************************************
 ** \��  ��  SKY1311S��λ��������disable״̬
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void sky1311Reset(void)
{
    sky1311WriteReg(ADDR_ANA_CFG1, 0x04);   // close TX, RX, OSC off
    sky1311WriteCmd(CMD_SW_RST);            // reset state machine
    SKY1311_DISABLE();
}

/**
 ******************************************************************************
 ** \��  ��  ��λPICC���رճ�5ms,�ٴ�
 **
 ** \��  ��  none
 ** \����ֵ  none
 ******************************************************************************/
void resetPicc(void)
{
    ClearBitMask(ADDR_ANA_CFG1,TX_EN);
    DelayMS(5);
    SetBitMask(ADDR_ANA_CFG1,TX_EN);
    DelayMS(10);

}
/**
 ******************************************************************************
 ** \��  ��  ���ݷ��亯����������д��FIFO�в�ͨ����Ƶ�ӿڷ��͸�PICC
 **
 ** \��  ��   uint8_t txType,        // �������ͣ�TYPE_A | TYPE_B��
 **           uint8_t *txBuff,      // ��������
 **           uint16_t txSize       // ���ݴ�С
 ** \����ֵ  none
 ******************************************************************************/
void sky1311_fifo_tx(uint8_t txType, uint8_t *txBuff, uint16_t txSize)
{
    uint16_t tx_res = txSize;
//		uint16_t TX_NUM;
    volatile uint16_t delayCount;
    sky1311WriteCmd(CMD_IDLE);              // reset state machine to Idle mode
    sky1311WriteCmd(CMD_CLR_FF);            // clear FIFO
    irqClearAll();                          // clear all IRQ state
		
//		TX_NUM =0;
    //sky1311WriteReg(ADDR_FIFO_CTRL,8);      // set water-level of FIFO
    /* write numbers */
    if(txType == TYPE_A){
        sky1311WriteReg(ADDR_TX_BYTE_NUM, txSize & 0x00ff);
        if(txSize > 255)
            sky1311WriteReg(ADDR_TX_BIT_NUM, ((txSize & 0x0f00)>>4|8));// write the length to tx_byte register
    }else if(txType == TYPE_B){
        sky1311WriteReg(ADDR_TX_B_BYTE_NUM, txSize & 0x00ff);
        if(txSize > 255)
            sky1311WriteReg(ADDR_TX_B_EGT_NUM, (txSize & 0x0300)>>4);
    }
    /* when TX length<=FIFO's depth, write all data to FIFO */
    if( txSize <= 64){
        sky1311WriteFifo(txBuff, txSize);
        sky1311WriteCmd(CMD_TX_RX);             // transceive & into receive mode
    }
    /* when TX length > FIFO's depth */
    else{               // txLen > 64
        sky1311WriteFifo(txBuff, 64);       // send 64 bytes
        sky1311WriteCmd(CMD_TX_RX);
        tx_res = txSize - 64;
        while(tx_res>0){                    // send remain bytes
            //if(0!= (irq_sta = sky1311ReadReg(ADDR_IRQ_STA))){
            if(1==SKY1311_IRQ_READ()){
                irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
                if(irq_sta & IRQ_LOW){  // FIFO low
                    if(tx_res>=56){
                        sky1311WriteFifo(&txBuff[txSize - tx_res], 56);
                        tx_res -=56;
                    }
                    else{
                        sky1311WriteFifo(&txBuff[txSize - tx_res], tx_res);
                        tx_res = 0;
                    }

                    irqClear(irq_sta);//irqClearAll();
                }
                else        // the other IRQ
                    irqClear(irq_sta);//return;
            }
        }
    }
    /* wait TX finished */
    while(1){
        delayCount = 0x4FFF;
        while( 0==SKY1311_IRQ_READ() && --delayCount);      // ���IRQ�����Ƿ����ж�
        //while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);   // ��ȡ�Ĵ����ж��Ƿ����ж�
        irq_sta=sky1311ReadReg(ADDR_IRQ_STA);
        if((delayCount==0)||(irq_sta & IRQ_TOUT)){             // tiemout
            sky1311WriteCmd(CMD_IDLE);
            return;
        }
        if(irq_sta & IRQ_TX){
            irqClear(IRQ_TX);
            return;
        }
        else{
            irqClear(irq_sta);  //irqClearAll();
        }
				
    }
}
/**
 ******************************************************************************
 ** \��  ��  ���ݽ��պ�������FIFO�����ݶ�����
 **
 ** \��  ��   uint8_t txType,        // �������ͣ�TYPE_A | TYPE_B��
              uint8_t rateType,      // �Ƿ�౶��
 **           uint8_t *rxBuff,      // ��������
 **           uint16_t rxSize       // ���ݴ�С
 ** \����ֵ  Ok,��ȷ���յ����� ; ��������������
 ******************************************************************************/
sta_result_t sky1311_fifo_rx(uint8_t rxType, uint8_t rateType, uint8_t *rxBuff, uint16_t *rxSize)
{
    uint16_t rx_buf_cnt=0;
    uint8_t  byte_num_H;
    uint8_t  byte_num_L;
    uint8_t  temp_len;
    uint8_t  bit_n=0;

    volatile uint16_t delayCount;

    while(1){
       delayCount=0x4FFF;                        // delay
        while( 0==SKY1311_IRQ_READ() && --delayCount);      // ���IRQ�����Ƿ����ж�
        //while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount); // ��ȡ�Ĵ����ж��Ƿ����ж�

        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
        err_sta = sky1311ReadReg(ADDR_ERR_STA);

        if((delayCount==0)||(irq_sta & IRQ_TOUT)){             // tiemout
            sky1311WriteCmd(CMD_IDLE);
            irqClearAll();
            return NoResponse;
        }
        else if(irq_sta & IRQ_HIGH){        // FIFO High
            sky1311ReadFifo(&rxBuff[rx_buf_cnt], 56);   // load next 56 bytes into FIFO
            rx_buf_cnt += 56;
            irqClear(IRQ_HIGH);
        }
        else if(irq_sta & IRQ_RX){          // Received
            if( ((sky1311ReadReg(ADDR_FIFO_LEN))<1) || (err_sta & 0xC0) ){  // error exits
                sky1311WriteCmd(CMD_CLR_FF);           // noise occur, restart the rx
                sky1311WriteCmd(CMD_RX);
                irqClear(IRQ_RX);  //irqClearAll();
                return Error;
            }
            else{
                irqClearAll();
                if(err_sta & 0xc0)
                    return Error;
                temp_len = sky1311ReadReg(ADDR_FIFO_LEN);               // get FIFO length
                sky1311ReadFifo(&rxBuff[rx_buf_cnt], temp_len);        // get data ,FIFO-->rx_buf
                rx_buf_cnt += temp_len;

                if(rxType == TYPE_A){
                    if(rateType){
                        byte_num_H = sky1311ReadReg(ADDR_RATE_RX_BIT);
                        byte_num_L = sky1311ReadReg(ADDR_RATE_RX_BYTE);
                    }else{
                        byte_num_H = sky1311ReadReg(ADDR_RX_NUM_H);
                        byte_num_L = sky1311ReadReg(ADDR_RX_NUM_L);
                        bit_n = ((sky1311ReadReg(ADDR_RX_NUM_H)) & 0xf0)>>4;
                        if(bit_n) sky1311ReadFifo(&rxBuff[rx_buf_cnt], 1);
                    }
                        *rxSize  = ((byte_num_H & 0x01) << 8) | byte_num_L;

                        if(bit_n)
                        {
                            *rxSize = ((*rxSize) + 1);
                        }
                }
                else if(rxType == TYPE_B){
                    byte_num_H = sky1311ReadReg(ADDR_RX_B_CTRL);
                    byte_num_L = sky1311ReadReg(ADDR_RX_B_BYTE_NUM);
                    *rxSize  = ( (byte_num_H & 0x80) << 1 ) | byte_num_L;
                }

                return Ok;
            }
        }
        else if(irq_sta & IRQ_LOW){    //���IRQ_LOW�ж�
          irqClear(IRQ_LOW);
        }
        else{
            irqClearAll();
            //return NoResponse;
        }
    }
}
/**
 ******************************************************************************
 ** \��  ��  �������ݺ�����ת���������
 **          Ҫ���͵����ݱ����ڱ���pSendDataBuf�У����ͻ�������
 **          ���յ����ݱ����ڱ���pRcvDataBuf�����յ������������ڱ���pRcvLen��
 ** \��  ��  uint8_t cardType, ������ ��TYPE_A | TYPE_B��
 **          uint8_t rateType, �Ƿ�֧�ֶ౶�٣�0��֧�֣�1֧��
 **          uint8_t *pSendDataBuf, ���ͻ�����
 **          uint8_t ucSendLen �������ݵĳ��ȣ�
 **          uint8_t *pRcvDataBuf, �������ݻ�����
 **          uint16_t *pRcvLen �� �������ݳ���
 **
 ** \����ֵ  Ok: ���յ����ص����ݣ� ������û���յ����ص�����
 ******************************************************************************/
sta_result_t ExchangeData(uint8_t cardType, uint8_t rateType,
                    uint8_t *pSendDataBuf, uint16_t ucSendLen,
                    uint8_t *pRcvDataBuf,  uint16_t *pRcvLen)
{
    sta_result_t sta;
    sky1311_fifo_tx( cardType, pSendDataBuf, ucSendLen);
    sta = sky1311_fifo_rx(cardType, rateType, pRcvDataBuf, pRcvLen);
    return sta;
}

/**
 ******************************************************************************
 ** \��  ��  Type A ����ͻ���ݷ��ͺ�����������д��FIFO�в�ͨ����Ƶ�ӿڷ��͸�PICC
 **          ���͵����ݱ�����ȫ�ֱ���tx_buf�У����ͻ�����
 **
 ** \��  ��  uint8_t* txBuf           ���ݻ�����
 **          uint8_t txLen          ��������ݳ��ȣ������������Ĳ��֣�,
 **          uint8_t lastBitNum     ���һ���ֽ���Чλ��
 ** \����ֵ  Ok--�ɹ����䣬Timeout--��ʱ, Error -- ��������
 ******************************************************************************/
sta_result_t bitCollisionTrans(uint8_t* txBuf, uint8_t txLen, uint8_t lastBitNum)
{
    volatile uint16_t delayCount;
    if(txLen>7)          // The maximum length of for transmission from PCD to PICC shall be 55 data bits
        return Error;
    sky1311WriteCmd(CMD_IDLE);              // reset state machine to Idle mode
    sky1311WriteCmd(CMD_CLR_FF);            // clear FIFO
    irqClearAll();                          // clear all IRQ state
    /* write numbers */
    sky1311WriteReg(ADDR_TX_BYTE_NUM, txLen);
    sky1311WriteReg(ADDR_TX_BIT_NUM,lastBitNum);
    sky1311WriteFifo(txBuf, txLen);         // write data to FIFO
    sky1311WriteCmd(CMD_TX_RX);             // transceive & into receive mode
    /* wait TX finished */
    while(1){
        delayCount = 0x4FFF;                // timeout count
        while( 0==SKY1311_IRQ_READ() && --delayCount);      // ���IRQ�����Ƿ����ж�
        //while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);   // waiting for TX STOP IRQ
        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
        if((delayCount==0)||(irq_sta & IRQ_TOUT)){
            sky1311WriteCmd(CMD_IDLE);
            irqClearAll();
            return Timeout;
        }
        else if(irq_sta & IRQ_TX){
            irqClear(IRQ_TX);
            break;
        }
        else{
            irqClearAll();
        }
    }
    /* �ȴ������������ */
    delayCount = 0x4FFF;                            // timeout count
    while( 0==SKY1311_IRQ_READ() && --delayCount);      // ���IRQ�����Ƿ����ж�
    //while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);     // waiting for RX STOP IRQ

    irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
    if((delayCount==0)||(irq_sta & IRQ_TOUT)){
        sky1311WriteCmd(CMD_IDLE);
        irqClearAll();
        return Timeout;
    }else if(!(irq_sta & IRQ_RX)){
        irqClearAll();
        return Error;
    }
    irqClear(irq_sta);
    return Ok;
}


/**
 ******************************************************************************
 ** \��  ��  ɨ�����ŵ�RCƵ�ʣ�Ϊ�͹���ѯ���ã�
 **          ע�⣺��Ҫ�ڶ���������û���κε��������ڵ�ʱɨ��
 **
 ** \��  ��  none
 ** \����ֵ  ��8λ�ɼ��������ADֵ��
         bit8--bit11 ��ʾ��ֵ���ʱ��Ƶ��ֵ����ӦAnalog0�Ĵ�����bit7--bit4
   		 bit12 ��ʾ�Ƿ�ɨ�赽���ֵ��1--ɨ�赽��0--û�С�
   		 bit13--15 û��ʹ�ã�����Ϊ0
 ******************************************************************************/
uint16_t sky1311RCFreqCali(void)
{
    uint8_t currAdcVal=0;       // ��ǰADֵ
    uint8_t maxAdcVal=0;        // ���ADֵ
    uint8_t currRCFreqVal=0;     // ��ǰƵ��ֵ
    uint8_t MaxRCFreqVal = 0;    // ����Ƶ��ֵ
    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C); //OSC Enable
    sky1311WriteReg(ADDR_ANA_CFG2, 0xA0);
	for(currRCFreqVal=0;currRCFreqVal<16;currRCFreqVal++)   // from 0000 to 1111 scan
    {
        sky1311WriteReg(ADDR_ANA_CFG0, (currRCFreqVal << 4) | CHK_PAVOL); // ����Ƶ��
        /*  �ó���������β���AD�ɼ���ת�� */
        sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x40);   // pull up RSSI_EN
        delay(30);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x60);   // pull up TX_EN
        delay(10);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x70);   // pull up ADC_EN
        delay(10);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x50);   // pull low TX_EN
        delay(20);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x00);   // pull low ADC_EN, RSSI_EN
        delay(10);
        currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5); // read ADC value from analog5 register
        if(currAdcVal > maxAdcVal){			    // ��ǰƵ�ʵ�ADֵ���������ֵ
            maxAdcVal = currAdcVal; 			// �õ�ǰֵȡ�����ֵ
            MaxRCFreqVal = currRCFreqVal;   	// ���µ�ǰƵ��ֵ
        }
	}
    if(maxAdcVal>MINADVAL){
        maxRCADVal = maxAdcVal;
			
			
#if uart_start
			
			SEGGER_RTT_printf(0,"\r\n--RC frequency callbate, RC Param = %d  AD Value= %d\r\n",MaxRCFreqVal,maxRCADVal);
			
//        NEW_ROW();
//        PUTS("--RC frequency calibate, ");
//        PUTS("RC Param = ");
//        PRINT(MaxRCFreqVal);
//        PUTS("    AD Value=");
//        PRINT(maxRCADVal);
//        NEW_ROW();
#endif
        return ((0x10 | MaxRCFreqVal) << 8 | maxAdcVal);		// ���سɹ���Ǻ�Ƶ��ֵ
    }
    else{
        maxRCADVal = MAXADVAL;
#if uart_start
			
			SEGGER_RTT_printf(0,"\r\n--RC frequency callbate, RC Param = %d  AD Value= %d\r\n",MaxRCFreqVal,maxRCADVal);
			
			SEGGER_RTT_printf(0,"--RC frequency calibrate: Error!\r\n");
//        NEW_ROW();
//        PUTS("--RC frequency calibrate: Error!");
//        NEW_ROW();
#endif
        return 0;
    }
}

/**
 ******************************************************************************
 ** \��  ��  ����Ƿ��п���������뿪��(�͹���ѯ��)
 **
 ** \��  ��  uint8_t RCƵ���趨ֵ
 ** \����ֵ  û�ж�����0  (���õ͹���ѯ�����)
 **          �п����룬1  (���ö������)
 **          �п��뿪��2  (��������У׼��ѯ��Ƶ�ʱ��)
 ******************************************************************************/
void checkCardInit (uint16_t rc_val)
{
	
    uint8_t MaxRCFreqVal, MaxADCVal;

		sky1311Reset();
	
	
		
		
		
    MaxRCFreqVal = (uint8_t)((rc_val >> 4) & 0xF0); // RCƵ��У׼ֵ
    MaxADCVal = (uint8_t)((rc_val & 0xFF));         // ��Ӧ�����ADֵ
    sky1311WriteReg(ADDR_ANA_CFG1, 0x00);
    sky1311Enable();
    DelayMS(20);
    sky1311WriteReg(ADDR_ANA_CFG0, MaxRCFreqVal | CHK_PAVOL);
    sky1311WriteReg(ADDR_ANA_CFG2, 0x10);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
    sky1311WriteReg(ADDR_ANA_CFG7, ADC_SAMPLE_5US | TX_SETTLE_5US | RSSI_DELTA);
    sky1311WriteReg(ADDR_ANA_CFG8, MaxADCVal-RSSI_ABS);
    sky1311WriteReg(ADDR_ANA_CFG6, WKU_EN | WKU_OR | WKU_400MS | RSSI_2AVG);
    DelayMS(20);
    irqClearAll();
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C);
    SKY1311_DISABLE();
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Start_Check_Card!\r\n");
		
		#endif
		
}

void checkCardDeinit(void)
{
    SKY1311_ENABLE();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG6, 0x22);
    sky1311WriteReg(ADDR_ANA_CFG7, 0x3F);
    sky1311WriteReg(ADDR_ANA_CFG8, 0xFF);
    SKY1311_DISABLE();
}

/**
 ******************************************************************************
 ** \��  ��  ����Ƿ��п��뿪��
 **
 ** \��  ��  uint8_t RCƵ��ɨ��õ������ADֵ
 ** \����ֵ  true,���Ѿ��뿪�� false�� ��û���뿪
 ******************************************************************************/
boolean checkCardRemove(uint16_t rc_val)
{
    uint8_t maxRCFreqVal;
    uint8_t maxAdcVal;
    uint8_t currAdcVal;       // ��ǰADֵ
    maxRCFreqVal = (uint8_t)((rc_val >> 4) & 0xF0);
    maxAdcVal = (uint8_t)(rc_val & 0xff);
    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C); //OSC Enable
    sky1311WriteReg(ADDR_ANA_CFG2, 0xA0);
    sky1311WriteReg(ADDR_ANA_CFG0, maxRCFreqVal | CHK_PAVOL);

    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x40);   // pull up RSSI_EN
    delay(30);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x60);   // pull up TX_EN
    delay(10);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x70);   // pull up ADC_EN
    delay(10);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x50);   // pull low TX_EN
    delay(20);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);   // pull low ADC_EN, RSSI_EN
    delay(10);
    currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5);
    sky1311Disable();
#if uart_start
			  SEGGER_RTT_printf(0,"currAdcVal= %d\r\n",currAdcVal);
				
				
//        PUTS("currAdcVal=");
//        PRINT(currAdcVal);
//        uart_putChar('\r');
//				
#endif
    if(currAdcVal > (maxAdcVal - RSSI_ABS))
    {
        return true;
    }
    else
        return false;

}
