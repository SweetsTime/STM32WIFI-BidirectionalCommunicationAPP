#include "wdg.h"
#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"
#include <stdio.h>

void iwdg_init(void)
{
  // 使能写 IWDG_PR 和 IWDG_RLR 寄存器 
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  // IWDG 计数时钟: 40KHz(LSI) / 32 = 1.25 KHz 
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  //设定重载值0x4dc ，大约1ms需要重载一次
  IWDG_SetReload(0x4DC);

  // 重载IWDG 
  IWDG_ReloadCounter();

  // 使能 IWDG (LSI 时钟自动被硬件使能)
  IWDG_Enable();
}

void wwdg_init(void)
{
    //使能 WWDG 时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

  // WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  //设定窗口值为 65 
  WWDG_SetWindowValue(65);

  //使能 WWDG 并设定计数值为127, WWDG时间 = ~4 ms * 64 = 262 ms */
  WWDG_Enable(127);

  //清 EWI 标志
  WWDG_ClearFlag();

  //使能  EW 中断
  WWDG_EnableIT();
  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void WWDG_IRQHandler(void)
{
  //重载 WWDG 计数器值
  WWDG_SetCounter(0x7F);
  //清 EWI 标志
  WWDG_ClearFlag();
  led1_toggle();
  printf("wwdg reloaded!\n");
}