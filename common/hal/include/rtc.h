#ifndef RTC_H
#define RTC_H
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"

typedef struct
{
	unsigned char Sec;
	unsigned char Min;
	unsigned char Hour;
	unsigned char Day;
	unsigned char Month;
	unsigned char Year;
}Time_Struct;

void rtc_init(void);
void set_rtc_time(unsigned char year ,unsigned char moth,
                unsigned char day ,unsigned char hour,
                unsigned char min ,unsigned char sec);
void read_rtc_time(void);

#endif