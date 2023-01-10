#ifndef __CARDOPER_H
#define __CARDOPER_H

#include "mcu.h"
#include "typedef.h"

extern uint8_t block_length;
extern uint8_t mifare_block_buff[256];

uint8_t mifare1_readDemo(void);
uint8_t SmartTypeA_readDemo(void);

sta_result_t TypeA_test(void);
sta_result_t TypeB_test(void);

uint8_t Type_A_Rrun (void);

uint8_t Type_A_Null_Key_Rrun (void);

sta_result_t Type_B_Rrun(void);

#endif
