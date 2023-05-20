/***********************************************************
***********************************************************/
#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/***********************************************************
* 名称: leds_init()
* 功能: 初始化led的GPIO时钟，并初始化为输出模式
* 参数: 无
* 返回: 无
* 修改:
* 注释: 1) 在使用stm32的led前，必须先调用这个初始化函数
***********************************************************/
void leds_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 初始化GPIO时钟 */
  RCC_APB2PeriphClockCmd(D1_GPIO_ACC | D2_GPIO_ACC, ENABLE);

  /* 设置LED的引脚为推挽输出 */
  GPIO_InitStructure.GPIO_Pin = D1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = D2_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D2_GPIO_PORT, &GPIO_InitStructure);
  
  //D1和D2默认关闭
  D1_off();
  D2_off(); 
}

/***********************************************************
* 名称: D1_off()
* 功能: 熄灭D1
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D1_off(void)
{
  GPIO_SetBits(D1_GPIO_PORT, D1_GPIO_PIN);
}

/***********************************************************
* 名称: D2_off()
* 功能: 熄灭D2
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D2_off(void)
{
  GPIO_SetBits(D2_GPIO_PORT, D2_GPIO_PIN);
}

/***********************************************************
* 名称: D1_on()
* 功能: 点亮D1
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D1_on(void)
{
  GPIO_ResetBits(D1_GPIO_PORT, D1_GPIO_PIN);
}

/***********************************************************
* 名称: D2_on()
* 功能: 点亮D2
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D2_on(void)
{
  GPIO_ResetBits(D2_GPIO_PORT, D2_GPIO_PIN);
}

/***********************************************************
* 名称: D1_toggle()
* 功能: 反转D1
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D1_toggle(void)
{
  GPIO_WriteBit(D1_GPIO_PORT, D1_GPIO_PIN, (BitAction)!GPIO_ReadOutputDataBit(D1_GPIO_PORT, D1_GPIO_PIN));
}

/***********************************************************
* 名称: D2_toggle()
* 功能: 反转D2
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D2_toggle(void)
{
  GPIO_WriteBit(D2_GPIO_PORT, D2_GPIO_PIN, (BitAction)!GPIO_ReadOutputDataBit(D2_GPIO_PORT, D2_GPIO_PIN));
}
