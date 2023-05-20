#include "handle.h"
#include "shtxx.h"
#include "LQ12864.h"
extern TxNetDataUnit  TNDU;

uint8_t UART2State = fail;
uint8_t UART3State = fail;
uint8_t UART4State = fail;
uint8_t UART5State = fail;

/***********************************************************
* 函数功能：系统时钟初始化
* 函数参数：time 系统时钟频率
* 返回值：无
***********************************************************/
void SysTickInit(uint8_t time)
{
  delay_init(time);
}

/***********************************************************
* 函数功能：定时器初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void TimInit(void)
{
  time2_init();
}

/***********************************************************
* 函数功能：串口初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void UsartInit(void)
{
  Initial_USART1(57600);
}

/***********************************************************
* 函数功能：LED初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void LedInit(void)
{
  leds_init();
}


/***********************************************************
* 函数功能：设备初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void EquipInit(void)
{
  SystemInit();                          //初始化系统
  
  SysTickInit(72);                       //滴答定时器初始化
  
  TimInit();                            //定时器初始化

  UsartInit();                          //串口初始化
  
  WiFi_Init();                          //ESP8266初始化
  
  UART_NVIC_Configuration();            //串口中断配置
  
  OLED_Init();
  SHTXX_Init();
  
  LedInit();                            //LED初始化
  
  Usart_SendNumString(USART1,"D1=ON D1=OFF\r\n",14);

  delay_s(2);                           //等待设备启动
}

/***********************************************************
* 函数功能：串口1相关操作
* 函数参数：USART 串口接收到的数据
* 返回值：无
***********************************************************/
void USART1_DataProcessing(UsartType* USART)
{
  if(USART->USART_DataFinishFlag)
  {
    USART->USART_DataFinishFlag &= ~0x01;
     
    memcpy(TNDU.TxIP,Target_IP,strlen((const char*)Target_IP));                //获取IP
    TNDU.TxLength = USART->USART_RxLength;                                      //获取长度
    memcpy(TNDU.TxData,USART->USART_RxEffecDataBuf,USART->USART_RxLength);       //获取传输内容
    
    WifiTxDataFlag |= 0x01;                    //发送数据标志
  }
}

/***********************************************************
* 函数功能：串口1接收数据
* 函数参数：无
* 返回值：无
***********************************************************/
void USART1_Receive(void)
{
  USART_Unit_ReceiveData(&USART_Unit[1]);
  
  USART1_DataProcessing(&USART_Unit[1]);
}

/***********************************************************
* 函数功能：串口相关操作
* 函数参数：无
* 返回值：无
***********************************************************/
void USART_Operation(void)
{
  USART1_Receive();
}

/***********************************************************
* 函数功能：其他操作
* 函数参数：无
* 返回值：无
***********************************************************/
void Other_Operation(void)
{
  ScanKey_Operation();
}

/***********************************************************
* 函数功能：网络状态监控
* 函数参数：无
* 返回值：无
* 注：如果网络出现异常会调用该函数，该函数会清除网络连接的所有状态位
      网络会回复到最初的状态
***********************************************************/
void NetStateMonitor(void)
{
  if(NetModeAbnormal)
  {
    NetModeAbnormal &= ~0x01;                   //清除异常标志
    
    WifiModeSetDown &= ~0x01;                //清除网络初始化标志
    CMD_CountNum = 0;                             //网络连接进度数清零，使程序从最初开始
    WifiCCCNum = 0;                             //网络发送数据进度数清零，使程序从最初开始
    CMD_OtherCount &= ~0x01;                   //拼接函数的计数清零
    
    memset(&AT_CWJAP[9],0,sizeof(AT_CWJAP)-9);          //清除发送内容
    memset(&AT_CIPSTA[10],0,sizeof(AT_CIPSTA)-10);              //清除发送内容
    memset(&AT_CIPSTART[12],0,sizeof(AT_CIPSTART)-12);
    memset(&AT_CIPSEND[11],0,sizeof(AT_CIPSEND)-11);
    memset(&AT_CIPSERVER[13],0,sizeof(AT_CIPSERVER)-13);

  }
}
