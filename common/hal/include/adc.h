#ifndef ADC_H
#define ADC_H
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#define ADC1_DR_Address ((uint32_t)0x4001244C)

void temp_init(void);
uint16_t read_ADC(void);
void adc_init(void);

#endif