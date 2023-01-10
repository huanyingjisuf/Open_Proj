#include "mcu.h"
#include "stm8_eval.h"
#include "sky1311_drv.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "cardoper.h"
#include "userdef.h"

uint16_t rcValue;
boolean hasCardIn=false;
#define  CARDREMOVED        0
#define  HASCARDIN          1
#define  READCARDOVER       2
#define  WAITREMOVE         4
#define  GOTOSLEEP          8
uint8_t  cardStatus = CARDREMOVED;


void main(void)
{
    __disable_interrupt();
    system_Init();
    LED_Init();


#if 0		/* ѭ���������ԣ��ȶ�Type A�������Զ���ÿ��ŵ���Ϣ������Type A �����Type B�������֤��  */

    //sky1311WriteReg(ADDR_MFOUT_SEL, 0x33);      // MFOUT SEL ��for debug��
    while(1)
    {
        if(Ok != TypeA_test())              // read Type A card
        {
            if(Ok!= TypeB_test())      // read Type B card
            {
                LED_OFF();
            }
        }
        sky1311Reset();
        DelayMS(300);
    }
#else	/* ʹ�õ��Զ��쿨���ܣ���У׼�ڲ�RC���RCУ׼ֵ�Ͷ�Ӧ��ADֵ���������Զ��쿨���ܣ��п����������ж� */
    /* RC Ƶ��У׼�������ӽ�13.56M��RCֵ�Լ���Ӧ��ADֵ
       У׼ʱ�����п������������忿�����ߣ�����У׼��ֵ����ȷ
    */
    rcValue = sky1311RCFreqCali();
#if DEBUG==1
    if(rcValue == 0)        // û�вɼ������ʵ�ADֵ
    {
      uart_puts("RC Cali Error\n");
      while(1)
      {
        LED_OFF();
        DelayMS(300);
        LED_ON();
        DelayMS(300);
      }
    }
#endif
    while(1)
    {
        if(cardStatus == CARDREMOVED)           // û���ڳ��ڣ��Ѿ����ߣ�
        {
            irqClearAll();              // ���SKY1311�������ж�
            checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
            cardStatus = GOTOSLEEP;
            system_ActiveHalt();        // MCU ��������״̬
        }
        else if(cardStatus == GOTOSLEEP)
        {
            /* ������MCU ���ߺ��� */
            if(hasCardIn)           // �쿨�жϲ����������˱��
            {
#if DEBUG==1
                uint8_t currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5);
                NEW_ROW();
                PUTS("CurrAdcVal=");
                PRINT(currAdcVal);
                NEW_ROW();
#endif
                irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // ���жϼĴ���
                if((0==irq_sta) &&                      // û���κ��жϲ�������
                   (0!=SKY1311_IRQ_READ())) // IRQ ���ű�ߵ�ƽ
                {
                    checkCardDeinit();          // ֹͣ�Զ��쿨
                    cardStatus = HASCARDIN;     // ���ñ�ǣ��п�����
                }
                else
                {
                    irqClearAll();
                    cardStatus = CARDREMOVED;

                }

#if DEBUG == 1
                uart_puts("\r\n**** Detected Card ****\r\n");
#endif
            }
            else
            {
                cardStatus = CARDREMOVED;
            }
        }
        else if(cardStatus == HASCARDIN)        // �п��ڳ���
        {
            if((Ok == TypeA_test()) || (Ok == TypeB_test()))          // ���Զ������ж��Ƿ�Type A ����
            {
                cardStatus = WAITREMOVE; // ��Ƕ������
            }
            else        // û�ж�����
            {
                //rcValue = sky1311RCFreqCali(); // ����У׼RCƵ��
                cardStatus = CARDREMOVED;
                LED_OFF();
            }
        }
        else if(cardStatus == WAITREMOVE)       // �ȴ����뿪
        {
            DelayMS(100);                   // ��΢��ʱһ�£�����̫Ƶ���ж��Ƿ��뿪
            if(checkCardRemove(rcValue))    // �жϿ��Ƿ��뿪
            {
                cardStatus = CARDREMOVED;       // ��ǿ��Ѿ��뿪
                LED_OFF();
            }
        }
    }
#endif
}