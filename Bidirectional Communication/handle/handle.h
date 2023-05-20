#ifndef _HANDLE_H_
#define _HANDLE_H_

#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "tim.h"
#include "led.h"
#include "key.h"
#include "wifi.h"

#define DefaultNum 0

typedef enum{
  fail = 0,
  success,
  choice
}State;

void EquipInit(void);
void SysTickInit(uint8_t time);
void TimInit(void);
void UsartInit(void);
void USART_Operation(void);
void NetStateMonitor(void);

#endif

