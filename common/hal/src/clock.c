
#include <stdio.h>
#include "clock.h"

uint32_t ulTimeDelay; //全局延时计数器

/***********************************************************
* 名称: clock_init()
* 功能: 时钟初始化函数
* 参数: 输入 ulTick，每秒产生SystemCoreClock / ulTick 个中断
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void clock_init(uint32_t ulTick)
{
    if (SysTick_Config(SystemCoreClock / ulTick))
    {
        while(1);
    }
}

/***********************************************************
* 名称: SysTick_Handler()
* 功能: systick中断处理函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void SysTick_Handler(void)
{
  (void)SysTick->CTRL;
  SCB->ICSR = SCB_ICSR_PENDSTCLR;
  ulTimeDelay++;
}