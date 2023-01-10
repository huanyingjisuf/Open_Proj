#include "sky1311_drv.h"
#include "sky1311t_reg.h"
#include "w25q32.h"





uint8_t SPIByteCom(uint8_t ch)  //SPI��������һ��
{
	
    uint8_t res;
	
	res =SPI_Tranfer(ch);
	
//    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty
//    SKY1311_SPI->DR = ch;                  // write to data register
//    while((SKY1311_SPI->SR & 0x01) == 0); // wait for RX buffer not empty
//    res = SKY1311_SPI->DR ;                // read RX buffer
//    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    return res;
}
 /******************************************************************************
 ** \��  ��  ͨ��SPI�ӿ���1311дһ���ֽڵ�����
 **
 ** \��  ��  ������
 ** \����ֵ  none
 ******************************************************************************/
void sky1311WriteCmd(uint8_t cmd)
{
    SKY1311_CS_LOW();

    cmd = (cmd & 0x1F) | 0x80;	        // bit7,6,5 = 100b, mean command
    /*
    SKY1311_SPI->DR = cmd;              // write to data register

    while((SKY1311_SPI->SR & 0x01) == 0); // wait for RX buffer not empty

    uint8_t temp = SKY1311_SPI->DR ;    // flush RX buffer

    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty

    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    temp = SKY1311_SPI->DR ;                // flush RX buffer
    */
    SPIByteCom(cmd);

    SKY1311_CS_HIGH();

}
/**
 ******************************************************************************
 ** \��  ��  ͨ��SPI�ӿ���1311�ļĴ���дһ���ֽ�����
 **
 ** \��  ��  uint8_t regAdd: �Ĵ�����ַ�� uint8_t data: Ҫд�������
 ** \����ֵ  none
 ******************************************************************************/
void sky1311WriteReg(uint8_t regAdd, uint8_t data)
{

    SKY1311_CS_LOW();

    regAdd      =   (regAdd & 0x3F);        // bit7,6=00, config as addr/write mode
/*
    SKY1311_SPI->DR = regAdd;               // write to data register

    while((SKY1311_SPI->SR & 0x01) == 0);  // wait for RX buffer not empty

    uint8_t temp = SKY1311_SPI->DR ;        // flush RX buffer

    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty

    SKY1311_SPI->DR = data;                 // write "data"to data register

    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty

    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    temp = SKY1311_SPI->DR ;                // flush RX buffer
 */
    SPIByteCom(regAdd);
    SPIByteCom(data);

    SKY1311_CS_HIGH();
}
/**
 ******************************************************************************
 ** \��  ��  ͨ��SPI�ӿڶ�ȡ1311�ļĴ���
 **
 ** \��  ��  uint8_t regAdd: �Ĵ�����ַ
 ** \����ֵ  uint8_t �Ĵ�������
 ******************************************************************************/
uint8_t sky1311ReadReg(uint8_t regAdd)
{
	
    uint8_t value;
    SKY1311_CS_LOW();

    regAdd      =   (regAdd & 0x3F) | 0x40;            // bit7,6=01, config as addr/read mode
/*
    value = SKY1311_SPI->DR;

    while((SKY1311_SPI->SR & 0x02) == 0);

    SKY1311_SPI->DR = regAdd;

    while((SKY1311_SPI->SR & 0x01) == 0);

    value = SKY1311_SPI->DR;

    while((SKY1311_SPI->SR & 0x02) == 0);

    SKY1311_SPI->DR = 0xFF;

    while((SKY1311_SPI->SR & 0x01) == 0);

    value = SKY1311_SPI->DR;
*/
    SPIByteCom(regAdd);
    value = SPIByteCom(0xFF);

    SKY1311_CS_HIGH();
    return value;

}
/**
 ******************************************************************************
 ** \��  ��  ͨ��SPI�ӿ���1311��FIFOдָ����Ŀ������
 **
 ** \��  ��  uint8_t* ��������ͷ��ַ�� uint8_t count: Ҫд�����������
 ** \����ֵ  none
 ******************************************************************************/
void sky1311WriteFifo(uint8_t *data, uint8_t count)
{
    uint8_t add;
    SKY1311_CS_LOW();

    add      =   (ADDR_FIFO & 0x3F);               // bit7,6=00, config as addr/write mode
/*
    SKY1311_SPI->DR = add;

    while((SKY1311_SPI->SR & 0x01) == 0);

    uint8_t temp = SKY1311_SPI->DR ;

    while((SKY1311_SPI->SR & 0x02) == 0);

    while(count--)
    {
        SKY1311_SPI->DR = *data++;

        while((SKY1311_SPI->SR & 0x02) == 0);
    }
    //delay(2);
    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    temp = SKY1311_SPI->DR ;                // flush RX buffer
*/
    SPIByteCom(add);
    while(count--)
    {
        SPIByteCom(*data++);
    }

    SKY1311_CS_HIGH();

}
/**
 ******************************************************************************
 ** \��  ��  ͨ��SPI�ӿ���1311��FIFO��ȡָ����Ŀ������
 **
 ** \��  ��  uint8_t* data �����ȡ���ݵĻ������׵�ַ�� uint8_t count ��ȡ���ֽ���
 ** \����ֵ  none
 ******************************************************************************/
void sky1311ReadFifo(uint8_t *data, uint8_t count)
{
    uint8_t add;

    SKY1311_CS_LOW();

    add   =   (ADDR_FIFO & 0x3F) | 0x40;            // bit7,6=01, config as addr/read mode
/*
    uint8_t temp = SKY1311_SPI->DR;

    while((SKY1311_SPI->SR & 0x02) == 0);

    SKY1311_SPI->DR = add;

    while((SKY1311_SPI->SR & 0x01) == 0);

    temp = SKY1311_SPI->DR;

    while(count--)
    {
      while((SKY1311_SPI->SR & 0x02) == 0);

      SKY1311_SPI->DR = 0xFF;

      while((SKY1311_SPI->SR & 0x01) == 0);

      *data++ = SKY1311_SPI->DR;
    }

    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
*/
    SPIByteCom(add);
    while(count--)
    {
        *data++ = SPIByteCom(0xFF);
    }

    SKY1311_CS_HIGH();
}

