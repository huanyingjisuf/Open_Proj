/**
  ******************************************************************************
  * @file    stm8_eval.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file provides firmware functions to manage Leds, push-buttons
  *          and COM ports available on STM8L Evaluation Boards from STMicroelectronics.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8_eval.h"
#include "sky1311_drv.h"
#include "sky1311t.h"
//#include "stm8l15x.h"
#include "Devic_Init.h" 
#include "Userdef.h"

const uint8_t hexToASCII[16] ={0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x41, 0x42, 0x43, 0x44, 0x45,0x46};

void delay(volatile uint8_t i){
//    while(i--);
//    {
//        __no_operation();
//        __no_operation();
//        __no_operation();
//        __no_operation();
//    }
	nrf_delay_us(10);
}

//#define TIM2_PERIOD  (uint8_t) 124
/**
  * @addtogroup TIMING_DELAY_Functions
  * @{
  */
/**
  * @brief  timing delay init:to generate 1 ms time base using TIM2 update interrupt
  * @note   The external low speed clock (LSE) is used to ensure timing accuracy.
  *         This function should be updated in case of use of other clock source.
  * @param  None
  * @retval None
  */
void TimingDelay_Init(void)
{
//  /* Enable TIM2 clock */
//  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
//  /* Time Base configuration */
//  TIM2_TimeBaseInit(TIM2_Prescaler_128,TIM2_CounterMode_Up,TIM2_PERIOD);//定时器2,将主时钟16分频.自动装载FA=124
//  TIM2_SetCounter(TIM2_PERIOD);                                //设定计数值FA=124
//  TIM2_UpdateRequestConfig(TIM2_UpdateSource_Global);
//  /* Clear TIM2 update flag */
//  TIM2_ClearFlag(TIM2_FLAG_Update);
//  /* Enable update interrupt */
//  //TIM2_ITConfig(TIM2_IT_Update, ENABLE);
//  //TIM2_Cmd(ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void DelayMS(volatile uint16_t nTime)  //延迟函数
{
//    TIM2_SetCounter(TIM2_PERIOD);
//    TIM2_Cmd(ENABLE);
//    while(nTime>0)
//    {
//        if(1==(TIM2->SR1 & 0x01))
//        {
//            TIM2_ClearITPendingBit(TIM2_IT_Update);
//            if(nTime!=0)  nTime--;
//        }
//    }
//    TIM2_Cmd(DISABLE);
	while(nTime--)
		nrf_delay_ms(1);
}

/* 初始化SPI接口，将对应的GPIO初始化为SPI功能 */
void SKY1311_SPI_Init(void)
{
  /* Set the MOSI,MISO and SCK at high level */
  //GPIO_ExternalPullUpConfig(SKY1311_SPI_SCK_GPIO_PORT, SKY1311_SPI_SCK_PIN | \
                            //SKY1311_SPI_MISO_PIN | SKY1311_SPI_MOSI_PIN, ENABLE);

//    GPIO_Init(SKY1311_SPI_SCK_GPIO_PORT, SKY1311_SPI_SCK_PIN, GPIO_Mode_Out_PP_High_Fast);  // PB.05
//    GPIO_Init(SKY1311_SPI_MISO_GPIO_PORT, SKY1311_SPI_MISO_PIN, GPIO_Mode_In_FL_No_IT);     // PB.07
//    GPIO_Init(SKY1311_SPI_MOSI_GPIO_PORT, SKY1311_SPI_MOSI_PIN, GPIO_Mode_Out_PP_High_Fast);// PB.06
//    GPIO_Init(SKY1311_SPI_CS_GPIO_PORT, SKY1311_SPI_CS_PIN, GPIO_Mode_Out_PP_High_Fast);    // PB.04
	
//    GPIO_Init(SKY1311_IRQ_GPIO_PORT, SKY1311_IRQ_PIN, GPIO_Mode_In_FL_No_IT);               // PB.02
//    GPIO_Init(SKY1311_RSSI_GPIO_PORT, SKY1311_RSSI_PIN, GPIO_Mode_In_FL_No_IT);             // PB.01
//    GPIO_Init(SKY1311_PD_GPIO_PORT, SKY1311_PD_PIN, GPIO_Mode_Out_PP_High_Fast);            // PB.00

//	nrf_gpio_cfg_input(SKY1311_IRQ_PIN,NRF_GPIO_PIN_PULLDOWN);  //配置 IRQ 为下拉输入模式
	nrf_gpio_cfg_output(SKY1311_SPI_CS_PIN);
	nrf_gpio_cfg_output(SKY1311_PD_PIN);  //设置 PD 为输出
	
    /*GPIOB->DDR = (GPIOB->DDR | 0x20 | 0x40 | 0x10);
    GPIOB->CR1 = (GPIOB->CR1 | 0x20 | 0x40 | 0x10);
    GPIOB->CR2 = (GPIOB->CR2 | 0x20 | 0x40 | 0x10);

    GPIOB->DDR = (GPIOB->DDR & 0x7F);
    GPIOB->CR1 = (GPIOB->CR1 | 0x80);
    GPIOB->CR2 = (GPIOB->CR2 & 0x7F);

    GPIOB->ODR = (GPIOB->ODR & 0xFD);
    GPIOB->DDR = (GPIOB->DDR & 0xFD);
    GPIOB->CR1 = (GPIOB->CR1 & 0xFD);
    GPIOB->CR2 = (GPIOB->CR2 & 0xFD);*/

  /* SKY1311_SPI Periph clock enable */
//  CLK_PeripheralClockConfig(SKY1311_SPI_CLK, ENABLE);
//  /* SPI configuration */
//  SPI_Init(SKY1311_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
//           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
//           SPI_NSS_Soft, 0x07);
//    /* Enable SPI  */
//  SPI_Cmd(SKY1311_SPI, ENABLE);
    /* Deselect the FLASH: Chip Select high */
	
  SKY1311_CS_HIGH();
}

void SKY1311_Init(void)
{
    /* MCU Port init for sky1311 */
    SKY1311_ENABLE();
    sky1311Init();
 }

void SKY1311_DeInit(void)
{
    sky1311Reset();     // sky1311 reset and disable
    SKY1311_DISABLE(); // GPIO_ResetBits(SKY1311_PB_GPIO_PORT, SKY1311_PB_PIN);   // PD脚拉低
    SKY1311_CS_HIGH(); // SPI 的CS 脚拉高（可能外部有上拉电阻，因此上拉）
}

void LED_Init(void)
{
    //LED_OFF();
   // GPIO_Init(LED_GPIO_PORT, LED_PIN, GPIO_Mode_Out_PP_Low_Slow); // led ctrl pin for output
}

/* I2C接口初始化 */
void I2C_Interface_Init(void)
{

//    //GPIOC->DDR = (GPIOC->DDR | 0x10);
//    //GPIOC->CR1 = (GPIOC->CR1 | 0x10);
//    //GPIOC->CR2 = (GPIOC->CR2 | 0x10);
//    //GPIOC->ODR = (GPIOC->ODR | 0x10);

//    GPIO_Init(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_Mode_In_FL_No_IT);        // PC.00
//    GPIO_Init(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_Mode_Out_PP_High_Fast);   // PC.01
//    GPIO_Init(I2C_IRQ_PORT, I2C_IRQ_PIN, GPIO_Mode_Out_PP_High_Fast);   // PC.04

//    CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
//    I2C_DeInit(I2C1);
//    I2C_ITConfig(I2C1, (I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
//    I2C_Init(I2C1, I2C_MAX_STANDARD_FREQ, 0x30 ,I2C_Mode_I2C, I2C_DutyCycle_2,
//    I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
//    I2C_Cmd(I2C1, ENABLE);
	
}


/* UART接口初始化 */
void Uart_Inerface_Init(void)
{
//  GPIO_Init(UART_TX_PORT,UART_TX_PIN, GPIO_Mode_Out_PP_Low_Fast);
//  GPIO_Init(UART_TX_PORT,UART_RX_PIN, GPIO_Mode_In_PU_No_IT);
//  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
//  USART_DeInit(USART1);
//  USART_Init(USART1, (uint32_t)115200, USART_WordLength_8b, USART_StopBits_1,
//             USART_Parity_No, USART_Mode_Tx);
//  USART_Cmd(USART1, ENABLE);
}

void uart_putChar(uint8_t ch)
{
//  USART_SendData8(USART1, ch);
//  while((USART1->SR & 0x80) == 0x00);
}

void uart_sendData(uint8_t *txBuff, uint8_t txSize)
{
//  while(txSize--)
//      uart_putChar(*txBuff++);
//  return ;
}

void uart_putHex(uint8_t data)
{
//    uint8_t tmp;
//    tmp = (data>>4) & 0x0f;
//    uart_putChar(hexToASCII[tmp]);
//    tmp = data & 0x0f;
//    uart_putChar(hexToASCII[tmp]);
//    uart_putChar(' ');
}

void uart_printBuffer(uint8_t* buff, uint16_t size)
{
//    while(size--){
//        uart_putHex(*buff++);
//    }
//    //uart_putChar('\r');
//    //uart_putChar('\n');
}

void uart_puts(char *str)
{
//    while(*str) uart_putChar(*str++);
//    //uart_putChar('\r');
//    //uart_putChar('\n');
}

void uart_newrow()
{
//    uart_putChar('\r');
//    uart_putChar('\n');
}
/*  */
void systemClk_Init(void)
{
//    //CLK_DeInit();
//    //CLK_HSICmd(ENABLE);
//    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
//    CLK_ClockSecuritySystemEnable();
}
/*  将与读卡芯片IRQ引脚连接的IO口设置为可外部中断 */
void IRQ_Init(void)
{
//    //IRQ Init
//    EXTI_ClearITPendingBit(EXTI_IT_Pin2);
//    GPIO_Init(SKY1311_IRQ_GPIO_PORT, SKY1311_IRQ_PIN, GPIO_Mode_In_FL_IT);
//    EXTI_SelectPort(EXTI_Port_B);
//    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);
}

void IRQ_DeInit(void)
{
//    //IRQ Init
//    EXTI_ClearITPendingBit(EXTI_IT_Pin2);
//    GPIO_Init(SKY1311_IRQ_GPIO_PORT, SKY1311_IRQ_PIN, GPIO_Mode_In_FL_No_IT);
//    EXTI_DeInit();
}
/* 设置初始化状态的MCU GPIO */
void systemGPIOInit(void)
{
//    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);  // PA0, PA1, PA2, PA3 output low

//    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);   // SPI_CS
//    GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_FL_No_IT);        // RSSI

//    GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); // I2C interface
//    GPIO_Init(GPIOC, GPIO_Pin_5 | GPIO_Pin_6 , GPIO_Mode_Out_PP_High_Slow);     // uart interface

//    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);        // status LED
}
/* 系统初始化 */
void system_Init(void)
{
//    /* Select HSI as system clock source */
//    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
//    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

 //   systemGPIOInit();
//    /*
//    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOB, 0x79, GPIO_Mode_Out_PP_Low_Slow); // Except IRQ,RSSI,MISO
//    GPIO_Init(GPIOC, 0xDF, GPIO_Mode_Out_PP_Low_Slow); // Except UART_TX
//    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);*/
//    /* initialize peripherals  */
//    Uart_Inerface_Init();
    SKY1311_SPI_Init();
//    TimingDelay_Init();
}


//void wakeup_Init(void)
//{
////	GPIO_Init(UART_RX_PORT,UART_RX_PIN, GPIO_Mode_In_PU_No_IT);
////	GPIO_Init(SKY1311_SPI_MISO_GPIO_PORT, SKY1311_SPI_MISO_PIN, GPIO_Mode_In_FL_No_IT);
////	//GPIO_Init(SKY1311_IRQ_GPIO_PORT, SKY1311_IRQ_PIN, GPIO_Mode_In_FL_No_IT);
////	GPIO_Init(SKY1311_RSSI_GPIO_PORT, SKY1311_RSSI_PIN, GPIO_Mode_In_FL_No_IT);
////	//GPIO_Init(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_Mode_In_FL_No_IT);
//}

//static void systemHalt_GPIOConfig(void)
//{
////    /* set GPIO to halt state */
////    GPIO_Init(UART_RX_PORT,UART_RX_PIN, GPIO_Mode_Out_PP_Low_Slow);

////    GPIO_Init(SKY1311_SPI_GPIO_PORT, SKY1311_RSSI_PIN | SKY1311_SPI_MISO_PIN | \
////            SKY1311_SPI_MOSI_PIN | SKY1311_SPI_SCK_PIN ,GPIO_Mode_Out_PP_Low_Slow);

////    GPIO_Init(I2C_GPIO_PORT, I2C_SDA_PIN|I2C_SCL_PIN|I2C_IRQ_PIN, GPIO_Mode_Out_PP_High_Slow);
//}


/* 进入低功耗 active halt 状态 */
//inline void system_ActiveHalt(void)
//{
////    systemHalt_GPIOConfig(); // GPIO config
////    IRQ_Init();             // 开启外部中断，有卡可以唤醒MCU
////    __enable_interrupt();   // 开启全局中断
////    PWR_UltraLowPowerCmd(ENABLE);// 电源管理进入低功耗模式
////    halt();             // 进入停机模式，等待唤醒
////    IRQ_DeInit();       // 禁止外部中断
////    wakeup_Init();      // 唤醒后IO口初始化
//}




