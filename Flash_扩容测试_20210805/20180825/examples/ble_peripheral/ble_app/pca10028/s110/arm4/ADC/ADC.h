#ifndef _ADC_H_
#define _ADC_H_

#include "nrf51.h"
#include <stdio.h>
#include "nrf_gpio.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"

typedef unsigned long   u32;
typedef unsigned short  u16;
typedef unsigned char   u8;

#define adc 2
#define adc_nf 2  //用GPIO3控制 采样电源


void adc_init(u8 ADC_res,u8 ADC_input_selection,u8 ADC_interrupt_enabled);
u16 nrf_adc_read(void);
void adc_start(void);
void ADC_N(void);
void ADC_F(void);
#endif
