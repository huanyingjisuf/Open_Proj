#ifndef _ACT_PASSWORD_H_
#define _ACT_PASSWORD_H_



#include <Devic_Init.h>


uint8_t enc_time_psw(uint32_t intime,uint32_t outtime,uint32_t random); //ʱЧ������ܺ���
uint32_t des_psw(uint8_t *psw,uint8_t psl,uint32_t unix_t);

uint8_t Data_Card_Run(void);  //���ݿ�����

#endif
