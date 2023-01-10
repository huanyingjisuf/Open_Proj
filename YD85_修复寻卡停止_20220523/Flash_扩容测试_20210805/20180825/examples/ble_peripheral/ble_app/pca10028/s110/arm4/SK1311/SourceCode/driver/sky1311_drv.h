/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SKY1311_DRV_H
#define __SKY1311_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "stm8_eval.h"
#include "mcu.h"

void sky1311WriteCmd(uint8_t cmd);
void sky1311WriteReg(uint8_t regAdd, uint8_t data);
uint8_t sky1311ReadReg(uint8_t regAdd);
void sky1311WriteFifo(uint8_t *data, uint8_t count);
void sky1311ReadFifo(uint8_t *data, uint8_t count);
//void SKY1311_SYSCLK(void);


#endif

