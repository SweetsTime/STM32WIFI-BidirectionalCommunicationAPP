
#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f10x.h"

extern uint32_t ulTimeDelay;

void clock_init();
void SysTick_Handler(void);
#endif