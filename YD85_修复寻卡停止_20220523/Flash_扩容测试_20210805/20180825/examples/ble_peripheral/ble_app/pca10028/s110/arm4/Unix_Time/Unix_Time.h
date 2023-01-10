#ifndef UNIX_TIME_H
#define UNIX_TIME_H
#include "nrf.h"

extern uint16_t time1[];  //时间戳转时间的缓存
/*
生成时间缓存 
time1[0] year
time1[1] mon
time1[2] day
time1[3] hour
time1[4] min
time1[5] sec

*/

uint32_t  Time_to_Unix
(uint16_t year,uint8_t mon,uint8_t day,
uint8_t hour,uint8_t min,uint8_t sec);

void Unix_Time_to_Time(uint32_t unix_time);



#endif
