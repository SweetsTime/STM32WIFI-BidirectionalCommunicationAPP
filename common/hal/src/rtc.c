
#include "rtc.h"
#include <stdio.h>
#include "misc.h"
#include "stm32f10x.h"
/***********************************************************
* 名称: rtc_init()
* 功能: 初始化rtc， 并设置rtc中断
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void rtc_init(void)
{
  //给后备域提供时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
  // 使能写 Backup domain 
  PWR_BackupAccessCmd(ENABLE);
  //打开外部低频晶振
  RCC_LSEConfig(RCC_LSE_ON);
  //等待外部低频晶振工作正常
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
  //外部低频晶振作为RTC晶振源
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  //使能RTC
  RCC_RTCCLKCmd(ENABLE);

  RTC_WaitForSynchro();

  RTC_WaitForLastTask();
  // 使能RTC秒中断
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  //等待
  RTC_WaitForLastTask(); 
  //(32.768 KHz)/(32767+1)
  RTC_SetPrescaler(32767);

  RTC_WaitForLastTask();
   
  //设置 rtc 中断
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}

/***********************************************************
* 名称: set_rtc_time()
* 功能: 设置rtc 时钟的初始化值
* 参数: 输入 : char year :设定年；
*       输入 : moth：设定月11；
*       输入 : char day ：设定日期；
*       输入 : hour：设定小时；
*       输入 : char min ：设定分钟；
*       输入 : char sec ：设定秒；
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void set_rtc_time(unsigned char year ,unsigned char moth,
                unsigned char day ,unsigned char hour,
                unsigned char min ,unsigned char sec)
{
  unsigned long temp;
  temp = year*360*24*3600+(moth-1)*30*24*3600+(day-1)*24*3600+hour*3600+min*60+sec;
  RTC_WaitForLastTask();

  RTC_SetCounter(temp);

  RTC_WaitForLastTask(); 
}
/***********************************************************
* 名称: read_rtc_time()
* 功能: 读取rtc 时钟的值，并且在串口中打印出来。
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/

void read_rtc_time(void)
{
  unsigned long Time_Value;
  unsigned short Day_Value;
  Time_Struct TimeStruct1;
     
  Time_Value = RTC_GetCounter();
  Day_Value = Time_Value/(24*3600) ;
         
  TimeStruct1.Year = Day_Value/360;
  TimeStruct1.Month = (Day_Value - TimeStruct1.Year*360)/30;
  TimeStruct1.Day = (Day_Value - TimeStruct1.Year*360)%30;
  TimeStruct1.Month += 1;
  TimeStruct1.Day += 1;
          
  TimeStruct1.Hour =  (Time_Value - Day_Value *24*3600)/3600;         
  TimeStruct1.Min =   (Time_Value - Day_Value *24*3600 - TimeStruct1.Hour*3600)/60;
  TimeStruct1.Sec =    Time_Value - Day_Value *24*3600 - TimeStruct1.Hour*3600 - TimeStruct1.Min*60;
         
  printf("20%d年  %d月  %d日\r\n", TimeStruct1.Year, TimeStruct1.Month, TimeStruct1.Day);
  printf("%dh : %dm : %ds\n", TimeStruct1.Hour, TimeStruct1.Min, TimeStruct1.Sec );           
}
/***********************************************************
* 名称: RTC_IRQHandler()
* 功能: RTC中断处理函数，每1秒产生一次中断， 每次中断将始终打印到串口上
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_SEC)!=RESET)
   {
     RTC_ClearFlag(RTC_IT_SEC);
     read_rtc_time();
   }
}