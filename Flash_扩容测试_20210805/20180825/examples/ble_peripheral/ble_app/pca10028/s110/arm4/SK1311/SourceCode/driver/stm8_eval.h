#include "Devic_Init.h" 

#ifndef __STM8L1526_EVAL_H
#define __STM8L1526_EVAL_H

/* Includes ------------------------------------------------------------------*/
/**
  * @brief  SKY1311 SPI Interface pins
  */
//#define SKY1311_SPI                           SPI1
//#define SKY1311_SPI_CLK                       CLK_Peripheral_SPI1
//#define SKY1311_SPI_GPIO_PORT                 GPIOB
//#define SKY1311_SPI_SCK_PIN                   GPIO_Pin_5                 /* PB.05 */
//#define SKY1311_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
//#define SKY1311_SPI_MISO_PIN                  GPIO_Pin_7                  /* PB.07 */
//#define SKY1311_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
//#define SKY1311_SPI_MOSI_PIN                  GPIO_Pin_6                  /* PB.06 */
//#define SKY1311_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
//#define SKY1311_SPI_CS_PIN                    GPIO_Pin_4                  /* PB.04 */
//#define SKY1311_SPI_CS_GPIO_PORT              GPIOB                       /* GPIOB */
//#define SKY1311_IRQ_PIN                       GPIO_Pin_2                  /* PB.02 */
//#define SKY1311_IRQ_GPIO_PORT                 GPIOB                       /* GPIOB */
//#define SKY1311_RSSI_PIN                      GPIO_Pin_1                  /* PB.01 */
//#define SKY1311_RSSI_GPIO_PORT                GPIOB                       /* GPIOB */
//#define SKY1311_PD_PIN                        GPIO_Pin_0                  /* PB.00 */
//#define SKY1311_PD_GPIO_PORT                  GPIOB                       /* GPIOB */

#define SKY1311_SPI_SCK_PIN 18
#define SKY1311_IRQ_PIN  16
#define SKY1311_SPI_MISO_PIN  19
#define SKY1311_SPI_MOSI_PIN  17
#define SKY1311_SPI_CS_PIN  15
#define SKY1311_PD_PIN  14

//#define    SPIFlash_Enable_CS    nrf_gpio_pin_clear(SPI_FLASH_CS)   //片选有效
//#define    SPIFlash_Disable_CS   nrf_gpio_pin_set(SPI_FLASH_CS)     //片选无效

#define SKY1311_CS_LOW()    nrf_gpio_pin_clear(SKY1311_SPI_CS_PIN)//nrf_delay_ms(1)// GPIO_ResetBits(SKY1311_SPI_CS_GPIO_PORT, SKY1311_SPI_CS_PIN)
#define SKY1311_CS_HIGH()    nrf_gpio_pin_set(SKY1311_SPI_CS_PIN)//nrf_delay_ms(1)// GPIO_SetBits(SKY1311_SPI_CS_GPIO_PORT, SKY1311_SPI_CS_PIN)

#define SKY1311_IRQ_READ()    nrf_gpio_pin_read(SKY1311_IRQ_PIN)//GPIO_ReadInputDataBit(SKY1311_IRQ_GPIO_PORT,SKY1311_IRQ_PIN)

#define SKY1311_DISABLE()    nrf_gpio_pin_clear(SKY1311_PD_PIN) //nrf_delay_ms(1) // GPIO_ResetBits(SKY1311_PD_GPIO_PORT, SKY1311_PD_PIN)
#define SKY1311_ENABLE()     nrf_gpio_pin_set(SKY1311_PD_PIN)//nrf_delay_ms(1)// GPIO_SetBits(SKY1311_PD_GPIO_PORT, SKY1311_PD_PIN)
#define sky1311Disable()     SKY1311_DISABLE()
#define sky1311Enable()     SKY1311_ENABLE()

/* GPIO for I2C*/
//#define I2C_GPIO_PORT           GPIOC
//#define I2C_SDA_PIN             GPIO_Pin_0
//#define I2C_SDA_PORT            GPIOC
//#define I2C_SCL_PIN             GPIO_Pin_1
//#define I2C_SCL_PORT            GPIOC
//#define I2C_IRQ_PIN             GPIO_Pin_4
//#define I2C_IRQ_PORT            GPIOC

/* GPIO for UART */
//#define UART_GPIO_PORT          GPIOC
//#define UART_TX_PIN             GPIO_Pin_5
//#define UART_TX_PORT            GPIOC
//#define UART_RX_PIN             GPIO_Pin_6
//#define UART_RX_PORT            GPIOC
///* GPIO for LED*/
//#define LED_PIN                 GPIO_Pin_0
//#define LED_GPIO_PORT           GPIOD
#define LED_ON()                 nrf_delay_ms(1)//GPIO_SetBits(LED_GPIO_PORT, LED_PIN)
#define LED_OFF()               nrf_delay_ms(1)// GPIO_ResetBits(LED_GPIO_PORT, LED_PIN)

void DelayMS(__IO uint16_t nTime);
void TimingDelay_Init(void);

void delay(uint8_t i);
void SKY1311_SPI_Init(void);
void SKY1311_Init(void);
void SKY1311_DeInit(void);
void I2C_Interface_Init(void);
void Uart_Inerface_Init(void);
void uart_putChar(uint8_t ch);
void uart_sendData(uint8_t *txBuff, uint8_t txSize);
void uart_putHex(uint8_t data);
void uart_printBuffer(uint8_t* buff, uint16_t size);
void RTC_WupkSet(void);
void systemClk_Init(void);
void system_Init(void);
void system_ActiveHalt(void);
void uart_puts(char *str);
void uart_newrow(void);
void LED_Init(void);

#define NEW_ROW()   uart_newrow()
#define PUTS(x)     uart_puts(x)
#define PRINT(x)    uart_putHex(x)

#endif

