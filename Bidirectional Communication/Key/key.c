#include "key.h"

/***********************************************************
* 函数功能：变量定义
* 函数参数：无
* 返回值：无
***********************************************************/
uint8_t Key1_KeyPushedFlag = fail;
uint8_t KeyPushedCount = fail;

/***********************************************************
* 函数功能：按键引脚定义
* 函数参数：无
* 返回值：无
***********************************************************/
uint8_t _Select_Key1(void){     
  return GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);}           //按键1    

/***********************************************************
* 函数功能：按键初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void Key_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 初始化GPIO时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}


/***********************************************************
* 函数功能：按键扫描
* 函数参数：WhichKey 扫描按键函数   KeyPushedFlag 按下标志
* 返回值：无
* 注：KeyPushedFlag 按下标志  只是表示按键被按下，但是一个合格的
      按键函数需要检测按键抬起后，才视为一次完整的按键动作
***********************************************************/
uint8_t ScanKey(_Key WhichKey,uint8_t* KeyPushedFlag)
{
  if(!WhichKey() || (*KeyPushedFlag))
  {
    KeyPushedCount++;                                   //次计数在定时器4的中断中清除  周期为1S
    if(KeyPushedCount > 1)                          //一段时间的延迟 消抖
    {
      KeyPushedCount = 0;
      *KeyPushedFlag = success; 
    }
    if(WhichKey() && (*KeyPushedFlag))
    {
      KeyPushedCount = 0;
      *KeyPushedFlag = fail;
      return success;
    }
  }
  return fail;
}

/***********************************************************
* 函数功能：按键扫描
* 函数参数：无
* 返回值：无
* 注：只有当引脚的状态改变后，才会去判断切换模式
      程序在不停的检测引脚的状态。
***********************************************************/
void ScanKey_Operation(void)
{ 
  if(ScanKey(_Select_Key1,&Key1_KeyPushedFlag))
  {
    D2_toggle();
  }
}
