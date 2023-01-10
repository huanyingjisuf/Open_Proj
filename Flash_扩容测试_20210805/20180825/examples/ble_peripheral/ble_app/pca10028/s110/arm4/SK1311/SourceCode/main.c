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


#if 0		/* 循环读卡测试，先读Type A卡，并自动获得卡号等信息，若无Type A 卡则读Type B卡（身份证）  */

    //sky1311WriteReg(ADDR_MFOUT_SEL, 0x33);      // MFOUT SEL （for debug）
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
#else	/* 使用低自动检卡功能，先校准内部RC获得RC校准值和对应的AD值，再配置自动检卡功能，有卡靠近产生中断 */
    /* RC 频率校准，获得最接近13.56M的RC值以及对应的AD值
       校准时不能有卡或者其它物体靠近天线，否则校准的值不正确
    */
    rcValue = sky1311RCFreqCali();
#if DEBUG==1
    if(rcValue == 0)        // 没有采集到合适的AD值
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
        if(cardStatus == CARDREMOVED)           // 没卡在场内（已经移走）
        {
            irqClearAll();              // 清除SKY1311的所有中断
            checkCardInit(rcValue);     // 配置进入自动检卡状态
            cardStatus = GOTOSLEEP;
            system_ActiveHalt();        // MCU 进入休眠状态
        }
        else if(cardStatus == GOTOSLEEP)
        {
            /* 以下是MCU 休眠后唤醒 */
            if(hasCardIn)           // 检卡中断产生，设置了标记
            {
#if DEBUG==1
                uint8_t currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5);
                NEW_ROW();
                PUTS("CurrAdcVal=");
                PRINT(currAdcVal);
                NEW_ROW();
#endif
                irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // 读中断寄存器
                if((0==irq_sta) &&                      // 没有任何中断产生，且
                   (0!=SKY1311_IRQ_READ())) // IRQ 引脚变高电平
                {
                    checkCardDeinit();          // 停止自动检卡
                    cardStatus = HASCARDIN;     // 设置标记，有卡靠近
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
        else if(cardStatus == HASCARDIN)        // 有卡在场内
        {
            if((Ok == TypeA_test()) || (Ok == TypeB_test()))          // 尝试读卡，判断是否Type A 卡，
            {
                cardStatus = WAITREMOVE; // 标记读卡完成
            }
            else        // 没有读到卡
            {
                //rcValue = sky1311RCFreqCali(); // 重新校准RC频率
                cardStatus = CARDREMOVED;
                LED_OFF();
            }
        }
        else if(cardStatus == WAITREMOVE)       // 等待卡离开
        {
            DelayMS(100);                   // 稍微延时一下，不用太频繁判断是否卡离开
            if(checkCardRemove(rcValue))    // 判断卡是否离开
            {
                cardStatus = CARDREMOVED;       // 标记卡已经离开
                LED_OFF();
            }
        }
    }
#endif
}