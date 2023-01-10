/******************************************************************************/
/*               (C) ˹���������������Ƽ����޹�˾(SKYRELAY)                   */
/*                                                                            */
/* �˴�����˹���������������Ƽ����޹�˾Ϊ֧�ֿͻ���д��ʾ�������һ����       */
/* ����ʹ��˹���������оƬ���û������޳�ʹ�øô��룬���豣������������       */
/* �������Ϊ��ʾʹ�ã�����֤�������е�Ӧ��Ҫ�󣬵����ض��淶������ʱ��       */
/* ʹ����������ȷ�ϴ����Ƿ����Ҫ�󣬲���Ҫ��ʱ����������Ӧ���޸ġ�           */
/* (V1.00)                                                                    */
/******************************************************************************/
/**
 ******************************************************************************
 ** �� �� ���� iso14443.h
 **
 ** �ļ������� ��ISO14443�Լ�M1���йصĲ��������������Լ���������
 **
 ** �汾��ʷ:
 ** 2015-10-05 V1.00  Mark   ��ʽ�汾
 ** 2018-09-02 V1.2.0 Mark   ��ʽ�汾
 ******************************************************************************/
#ifndef __ISO14443_H_
#define __ISO14443_H_

#include "mcu.h"
#include "sky1311t.h"
#include "sky1311_drv.h"

 /**
 ******************************************************************************
 ** \������ȫ��Ԥ����
 **
 ******************************************************************************/
#define     SEL1            0x93
#define     SEL2            0x95
#define     SEL3            0x97

#define     REQA            0x26
#define     WUPA            0x52
#define     SELECT          0x90
#define     HALTA           0x50
#define     PATS            0xE0
#define     PPS             0xD0

#define	    APF_CODE	    0x05		// REQB����֡ǰ׺�ֽ�APf
#define	    APN_CODE	    0x05		// REQB����֡ǰ׺�ֽ�APn
#define	    APC_CODE	    0x1D		// ATTRIB����֡ǰ׺�ֽ�APC
#define	    HALTB_CODE	    0x50		// ��������

#define     REQIDEL		    0x00		// IDLE
#define     REQALL		    0x08		// ALL

#define     M1_AUTH         0x60
#define     M1_AUTH_KEYA    0x60
#define     M1_AUTH_KEYB    0x61
#define     M1_READ         0x30
#define     M1_WRITE        0xA0
#define     M1_INCREMENT    0xC1
#define     M1_DECREMENT    0xC0
#define     M1_RESTORE      0xC2
#define     M1_TRANSFER     0xB0
#define     M1_ACK          0x0A



//I-block (00xxxxxx)b (not (00xxx101)b)
//R-block (10xxxxxx)b (not (1001xxxx)b)
//S-block (11xxxxxx)b (not (1110xxxx)b and not (1101xxxx)b)
/**
 ******************************************************************************
 ** \������ �������Ͷ���
 **
 ******************************************************************************/

////============= ȫ�ֱ����ͺ������� ================================================================
typedef struct
{
	uint8_t PUPI[4];					// αΨһPICC��ʶ��
	uint8_t AppDat[4];					// Ӧ������
	uint8_t ProtInf[3];					// ������Ϣ
}nAQTB_t;

 /******************************************************************************
 ** \������ ȫ�ֱ�������
 **
 ******************************************************************************/

/**
 ******************************************************************************
 ** \������ ��������
 **
 ******************************************************************************/
sta_result_t M1_Authentication_Auto(uint8_t auth_mode,uint8_t *m1Key,uint8_t *uid, uint8_t blockAddr);
sta_result_t M1_Authentication(uint8_t auth_mode,uint8_t *m1Key,uint8_t *uid, uint8_t blockAddr);
sta_result_t M1_PwdChange(uint8_t blockAddr, uint8_t *key_a, uint8_t *key_b);
sta_result_t M1_Write(uint8_t blockAddr, uint8_t* data);
sta_result_t M1_Read(uint8_t blockAddr, uint8_t *blockBuff);
sta_result_t M1_Increment(uint8_t blockAddr, uint32_t value);
sta_result_t M1_Decrement(uint8_t blockAddr, uint32_t value);
sta_result_t M1_Transfer(uint8_t blockAddr);
sta_result_t M1_Restore(uint8_t blockAddr);

sta_result_t piccRequestA(uint8_t *ATQA);
sta_result_t piccWakeupA(uint8_t *ATQA);
sta_result_t piccAntiA(uint8_t SEL, uint8_t rand_bit, uint8_t *uid);
sta_result_t piccSelectA(uint8_t SEL,uint8_t *uid, uint8_t *SAK);
void piccHaltA(void);
sta_result_t piccDeselectA(void);

sta_result_t piccRequestB(uint8_t ucReqCode, uint8_t ucAFI, uint8_t N, nAQTB_t *pATQB);
sta_result_t piccSlotMarker(uint8_t N, nAQTB_t *pATQB);
sta_result_t piccAttrib(uint8_t *pPUPI, uint8_t ucDSI_DRI,uint8_t MAX_FSDI,
				 uint8_t ucCID, uint8_t ucProType, uint8_t *pHigherLayerINF, uint8_t ucINFLen,
				 uint8_t *pAATTRIB, uint8_t *pRLen);
sta_result_t piccHaltB(uint8_t *pPUPI);
sta_result_t piccDeselectB(uint8_t *param);

sta_result_t piccATS(uint8_t *ATS);
sta_result_t piccPPS(uint16_t high_rate, uint8_t *pps);

uint8_t ISO14443_3_Deal_Flow(uint8_t *rxBuff, uint8_t *rxSize);

sta_result_t ISO14443_Transcive(uint8_t *txBuf, uint16_t txSize, uint8_t *rxBuf, uint16_t *rxSize);

void getFSCFSD(uint8_t bFSCIFSDI, uint8_t bType);
#endif
