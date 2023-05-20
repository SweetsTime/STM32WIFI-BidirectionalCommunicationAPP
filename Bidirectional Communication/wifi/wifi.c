#include "wifi.h"

/*变量声明*/
uint8_t WifiModeSetDown = fail;                 //WIFI模式设置完成标志位
uint8_t NetModeAbnormal = fail;                //网络模式设置异常标志位
uint8_t CMD_Count = 0;                          //用来确定命令只发一次
uint16_t CMD_ReSendTim = 0;                      //重发时间检测
uint8_t CMD_ReSendCount = 0;                    //重发次数检测
uint8_t CMD_ReturnOK = 0;                       //返回了OK命令
uint8_t CMD_CountNum = 0;                       //用来确定执行到了哪条命令
uint8_t CMD_ReturnReady = 0;                       //返回了ready命令
uint8_t CMD_OtherCount = 0;                     //拼接函数的计数，只执行一次
uint8_t NetMode = Net_Online;                           //网络模式 在线或者离线  上电默认是在线模式
uint8_t CIPSTART_Flag = 0;                      //CIPSTART命令标志
uint8_t WifiClientModeFinishFlag = 0;           //WIFI客户端模式设置完成
uint8_t WifiReDataFlag = 0;                         //wifi接收到数据
uint8_t CMD_ReturnSign = 0;                       //返回了>命令
uint8_t WifiSendDataNum = 0;                    //wifi发送数据流程
uint8_t CMD_ReturnSendOK = 0;                       //返回了send OK命令
uint8_t WifiTxDataFlag = 0;                         //wifi发送数据
uint8_t WifiServerModeFinishFlag = 0;           //WIFI服务端模式设置完成
uint8_t WifiCCCNum = 0;                         //WIFI通道连接状态改变之后操作流程
uint8_t WifiUartSignLoc = 0;                    //wifi串口接收到的数据标识符后一个字节的位置
uint8_t WifiUartReLeng = 0;                     //wifi串口接受到的数据长度
uint8_t CMD_RSTFlag = 0;                        //wifi发送的为复位命令   
uint8_t NoneEquipOnlineFlag = 0;                //设备无连接与被连接标志位

uint8_t WifiReNetChannel[5] = {0};              //WIFI接收到的网络数据的通道
uint8_t WifiReNetDataLength[5] = {0};           //WIFI接收到的网络数据的长度
uint8_t WifiReNetData[255] = {0};               //WIFI接收到的网络数据

uint8_t AT_AT[]         = {"AT\r\n"};                     //连接测试命令
uint8_t AT_ATE0[]       = {"ATE0\r\n"};                   //不需要回显
uint8_t AT_RST[]        = {"AT+RST\r\n"};                 //重启模块
uint8_t AT_CWMODE[]     = {"AT+CWMODE=0\r\n"};           //设置
uint8_t AT_CWJAP[50]    = {"AT+CWJAP="};                 //连接AP
uint8_t AT_CWSAP[50]    = {"AT+CWSAP="};                 //创建AP
uint8_t AT_CIPMUX[]     = {"AT+CIPMUX=0\r\n"};           //设置连接数量 0单连接 1多连接
uint8_t AT_CIPSERVER[25]  = {"AT+CIPSERVER="};          //开启端口号 作为服务端
uint8_t AT_CIPSEND[20]  = {"AT+CIPSEND="};              //发送数据  最大长度为2048
uint8_t AT_CIPSTA[30]   = {"AT+CIPSTA="};               //设置STA的IP
uint8_t AT_CIPAP[20]    = {"AT+CIPAP="};                 //设置AP的IP
uint8_t AT_CIPSTART[50] = {"AT+CIPSTART="};              //建立TCP/UDP连接
uint8_t AT_CIPSTATUS[]  = {"AT+CIPSTATUS\r\n"};          //获取连接状态

uint8_t Exit_Passthrough[] = {"+++"};                   //退出透传
uint8_t AT_RESTORE[] = {"AT+RESTORE\r\n"};              //恢复出厂设置


uint8_t Wifi_Name[15]   = {"wifi-001"};                 //WIFI名称
uint8_t Wifi_Pwd[12]    = {"1234567890"};                //WIFI密码
uint8_t Self_IP[15]     = {"192.168.43.142"};              //自身IP
uint8_t Self_Port[5]    = {"8080"};                      //自身开启的端口
uint8_t MUX             = 0;                             //0 单连接  1 多连接
uint8_t Net[5]          = {"TCP"};                       //网络连接方式
uint8_t Target_IP[15]   = {"192.168.43.1"};              //目标IP
uint8_t Target_Port[5]  = {"8086"};                      //目标端口

NetConnectUnit NCU[MaxNum];
RxNetDataUnit  RNDU;
TxNetDataUnit  TNDU;

/***********************************************************
* 函数功能：WIFI相关引脚的初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void Wifi_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 初始化GPIO时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOA,GPIO_Pin_0);       //设置为运行模式   低电平为烧写模式
}

/***********************************************************
* 函数功能：WIFI相关的初始化
* 函数参数：无
* 返回值：无
***********************************************************/
void WiFi_Init(void)
{
  Initial_USART3(115200);
  
  /*退出透传模式*/
  WifiSendCmd((uint8_t *)Exit_Passthrough,(uint8_t)strlen((const char*)Exit_Passthrough));
  delay_ms(1000);
  WifiSendCmd((uint8_t *)Exit_Passthrough,(uint8_t)strlen((const char*)Exit_Passthrough));
  delay_ms(1000);
  /*恢复出厂设置*/
  WifiSendCmd((uint8_t *)AT_RESTORE,(uint8_t)strlen((const char*)AT_RESTORE));
}

/***********************************************************
* 函数功能：串口发送
* 函数参数：data 发送的数据   leng长度
* 返回值：无
***********************************************************/
void WifiSendCmd(uint8_t* data,uint8_t Leng)
{
  Usart_SendNumString(USART3,data,Leng);
  Usart_SendNumString(USART1,data,Leng);
}

/***********************************************************
* 函数功能：发送AT命令的函数
* 函数参数：CMD AT命令 FinishFlag 发送成功标志位类型  ReSendTime 超时时间  ReSendCount 重传次数 DataType 命令类型(是普通命令还是数据命令)
* 返回值：finish 成功  fail 失败
***********************************************************/
uint8_t SendCMDProcess(uint8_t* CMD,uint8_t Length,uint8_t* FinishFlag,uint16_t ReSendTime,uint8_t ReSendCount,uint8_t DataType)
{
  if(!CMD_Count)
  {
    CMD_Count = success;
    WifiSendCmd(CMD,Length);
    CMD_ReSendTim = fail;                                           //还原重发时间
  }
  if(*FinishFlag)
  {
    CMD_Count = fail;
    *FinishFlag = fail;                                         //清除接收到OK标志位
    CMD_ReSendTim = fail;                                       //还原重发时间
    CMD_ReSendCount = fail;                                     //还原重发次数
    
    return success;
  }

  if(CMD_ReSendTim >= ReSendTime)                                 //重发
  {
    CMD_ReSendTim = fail;
    CMD_Count = fail;
    CMD_ReSendCount++;
    if(CMD_ReSendCount >= ReSendCount)                           //重发几次无反应
    {
      CMD_ReSendCount = fail;
      if(DataType == CMD_Type)                            //普通命令异常报错（例如，配置的AT命令）
      {
        NetModeAbnormal = success;                        //AT命令发送异常
      }
      else if(DataType == NetData_Type)                   //网络数据，不报错（例如，网络数据的重发）
      {
        CMD_Count = fail;
        *FinishFlag = fail;                               //清除接收到OK标志位
        return success;
      } 
    }
  }
  return fail;
}

/***********************************************************
* 函数功能：AT测试命令
* 函数参数：无
* 返回值：无
***********************************************************/
void CMD_ATTEST(uint8_t* CMD_Num)
{
  if(SendCMDProcess(AT_AT,strlen((const char*)AT_AT),&CMD_ReturnOK,200,3,CMD_Type))           //重传时间最小单位为5ms
    (*CMD_Num)++;
}

/***********************************************************
* 函数功能：关闭回显命令
* 函数参数：无
* 返回值：无
***********************************************************/
void CMD_ATEO(uint8_t* CMD_Num)
{
  if(SendCMDProcess(AT_ATE0,strlen((const char*)AT_ATE0),&CMD_ReturnOK,200,3,CMD_Type))         //重传时间最小单位为5ms
    (*CMD_Num)++;
}

/***********************************************************
* 函数功能：设置模块模式
* 函数参数：WifiState Mode 模块模式  
            1 STA模式  
            2 AP模式  
            3 STA&AP模式
* 返回值：无
***********************************************************/
void CMD_CWMODE(WifiState Mode,uint8_t* CMD_Num)
{ 
  switch(Mode)
  {
    case ESP8266_STA:           AT_CWMODE[10] = '1';            break;
    case ESP8266_AP:            AT_CWMODE[10] = '2';            break;
    case ESP8266_STAandAP:      AT_CWMODE[10] = '3';            break;
    default:                                                    break;
  }
  if(SendCMDProcess(AT_CWMODE,strlen((const char*)AT_CWMODE),&CMD_ReturnOK,200,3,CMD_Type))       //重传时间最小单位为5ms
    (*CMD_Num)++;
}

/***********************************************************
* 函数功能：复位模块
* 函数参数：无
* 返回值：无
***********************************************************/
void CMD_RESET(uint8_t* CMD_Num)
{
  CMD_RSTFlag |= 0x01;
  if(SendCMDProcess(AT_RST,strlen((const char*)AT_RST),&CMD_ReturnReady,1000,3,CMD_Type))          //重传时间最小单位为5ms
  {
    (*CMD_Num)++;
    CMD_RSTFlag &= ~0x01;
  }
}

/***********************************************************
* 函数功能：查看字符串中指定字符所含的位置
* 函数参数：string 字符串 sign 指定的字符  num 第几个
* 返回值：所含字符的位置
***********************************************************/
uint8_t LocationOfSign(uint8_t* string,uint8_t sign,uint8_t Num)
{
  uint8_t i = 0,Count = 0;
  while(string[i])
  {
    if(string[i] == sign)
    {
      Count++;
      if(Count == Num)
        break;
    } 
    i++;
    if(i >= 50)
      return fail;
  }
  return i;
}

/***********************************************************
* 函数功能：命令拼接函数
* 函数参数：*CMD  命令   
            *Data  需要添加的数据
            Channel 通道号
            注 当创建WIFI的时候会使用  加入WIFI的时候参数为0
            EncryptType 加密类型
            0  open
            2  WPA_PSK
            3  WPA2_PSK
            4  WPA_WPA2_PSK
* 返回值：无
***********************************************************/
void CMDJoint(uint8_t* CMD,uint8_t* Data,uint8_t* Data2,uint8_t* Channel,uint8_t EncryptType)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)Data);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)Data2);
    strcat((char *)CMD,(const char *)"\"");
    if(memcmp(CMD,"AT+CWSAP",8) == 0)
    {
      strcat((char *)CMD,(const char *)",");
      strcat((char *)CMD,(const char *)Channel);
      strcat((char *)CMD,(const char *)",");
      strcat((char *)CMD,(const char *)&EncryptType);
    }
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* 函数功能：连接WIFI网络
* 函数参数：*name  WIFI名字   
            *PSWord  WIFI密码
* 返回值：无
***********************************************************/
void CMD_CWJAP(uint8_t *name,uint8_t *PSWord,uint8_t* CMD_Num)
{
  CMDJoint(AT_CWJAP,name,PSWord,0,0);
  if(SendCMDProcess(AT_CWJAP,strlen((const char*)AT_CWJAP),&CMD_ReturnOK,1000,3,CMD_Type))          //重传时间最小单位为5ms
  {
    (*CMD_Num)++;
    
    CMD_OtherCount = fail;
    memset(&AT_CWJAP[9],0,sizeof(AT_CWJAP)-9);          //清除发送内容
  }
}

/***********************************************************
* 函数功能：IP拼接函数
* 函数参数：CMD 需要加的命令  IP 添加的IP
* 返回值：无
***********************************************************/
void IPJoinCMD(uint8_t* CMD,uint8_t* IP)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)IP);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* 函数功能：设置IP
* 函数参数：IP 要设置的IP地址  
* 返回值：无
***********************************************************/
void CMD_CIPSTA(uint8_t* IP,uint8_t* CMD_Num)
{
  IPJoinCMD(AT_CIPSTA,IP);
  if(SendCMDProcess(AT_CIPSTA,strlen((const char*)AT_CIPSTA),&CMD_ReturnOK,200,3,CMD_Type))          //重传时间最小单位为5ms
  {
    CMD_OtherCount &= ~0x01;
    *CMD_Num = 0;
    WifiModeSetDown |= 0x01;
    memset(&AT_CIPSTA[10],0,sizeof(AT_CIPSTA)-10);              ////清除发送内容
  }
}

/***********************************************************
* 函数功能：设置为STA模式
* 函数参数：无
* 返回值：无
***********************************************************/
void ModeSetSTA(void)
{
  switch(CMD_CountNum)
  {
    case 0:     CMD_ATTEST(&CMD_CountNum);                      break;                  //AT测试命令
    case 1:     CMD_ATEO(&CMD_CountNum);                        break;                  //关闭回显命令
    case 2:     CMD_CWMODE(ESP8266_STA,&CMD_CountNum);          break;                  //设置模块模式 STA/AP/STA+AP
    case 3:     CMD_RESET(&CMD_CountNum);                       break;                  //复位命令
    case 4:     CMD_ATEO(&CMD_CountNum);                        break;                  //关闭回显命令
    case 5:     CMD_CWJAP(Wifi_Name,Wifi_Pwd,&CMD_CountNum);    break;                  //加入WIFI网络命令
    case 6:     CMD_CIPSTA(Self_IP,&CMD_CountNum);              break;                  //设置自身IP命令
    default:                                                    break;
  }
}

/***********************************************************
* 函数功能：设置模块模式
* 函数参数：WifiState Mode      模块模式  
            ESP8266_STA         STA模式  
            ESP8266_AP          AP模式       暂不开放     
            ESP8266_STAandAP    STA&AP模式  暂不开放
* 返回值：无
* 备注： 定时去检查状态，定时时间为300ms
         设置模块模式为STA或者AP 当为STA模式时 加入WIFI网络  
         当为AP模式时 创建一个WIFI网络
***********************************************************/
void WIFIMode(WifiState Mode)
{
  if(!WifiModeSetDown)               //250ms 模式设置未完成  模块无异常
  {
    switch(Mode)
    {
      case ESP8266_STA:                                   
      {
        ModeSetSTA();
        break;
      }
      case ESP8266_AP:                                    
      {
        //ModeSetAP();
        break;
      }
      case ESP8266_STAandAP:                              
      {
      //  ModeSetSTAandAP();
        break;
      }
    }
  }
}

/***********************************************************
* 函数功能：是否开启多连接
* 函数参数：Num  多连接表示
            0  关闭   
            1  开启
            
            TypeCount 流程数 服务端或客户端
* 返回值：无
***********************************************************/
void CMD_CIPMUX(uint8_t Num,uint8_t* CMD_Num)
{
  switch(Num)
  {
    case 0:     AT_CIPMUX[10] = '0';    break;
    case 1:     AT_CIPMUX[10] = '1';    break;
    default:                            break;
  }
  if(SendCMDProcess(AT_CIPMUX,strlen((const char*)AT_CIPMUX),&CMD_ReturnOK,200,3,CMD_Type))       //重传时间最小单位为5ms
    (*CMD_Num)++;
}

/***********************************************************
* 函数功能：开始拼接函数
* 函数参数：CMD     命令      
            Channel 通道号 多连接有效  0xff 单连接
            Net 连接方式 tcp/udp
            IP  PORT端口
* 返回值：无
***********************************************************/
void StartJoinCmd(uint8_t* CMD,uint8_t Channel,uint8_t* Net,uint8_t* IP,uint8_t* Port)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    if(Channel != 0xff)
    {
      strcat((char *)CMD,(const char *)&Channel);
      strcat((char *)CMD,(const char *)",");
    }
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)Net);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)IP);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)Port);
    
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* 函数功能：建立网络通道
* 函数参数：
            channel 连接通道 
            注：如果为多连接 填写连接通道号  如果为单连接 填写0XFF  
            Net  连接方式  TCP/UDP
            IP
            目标IP
            PORT
            目标端口号
* 返回值：无
***********************************************************/
void CMD_CIPSTART(uint8_t channel,uint8_t* Net,uint8_t* IP,uint8_t* Port,uint8_t* CMD_Num)
{
  if(NetMode == Net_Offline)                                                  //如果是离线模式跳过与外界建立SOCKET
  {
//    CIPSTART_Flag &= ~0x01;
    CMD_OtherCount = fail;
    (*CMD_Num)++;       
  }
  else                                                                          //在线模式
  {
    StartJoinCmd(AT_CIPSTART,channel,Net,IP,Port);
//    CIPSTART_Flag |= 0x01;
    if(SendCMDProcess(AT_CIPSTART,strlen((const char*)AT_CIPSTART),&CMD_ReturnOK,1000,3,CMD_Type))       //重传时间最小单位为5ms
    {
//      CIPSTART_Flag &= ~0x01;
      CMD_OtherCount = fail;
      (*CMD_Num)++;    
      memset(&AT_CIPSTART[12],0,sizeof(AT_CIPSTART)-12);
      
      if(channel == 0xff)
        WifiClientModeFinishFlag |= 0x01;
    }
  }
}

/***********************************************************
* 函数功能：查看字符串中指定字符所含的数量
* 函数参数：string 字符串 sign 指定的字符
* 返回值：所含字节数量
***********************************************************/
uint8_t TheNumOfSign(uint8_t* string,uint8_t sign)
{
  uint8_t i = 0;
  uint8_t num = 0;
  while(string[i])
  {
    if(string[i] == sign)
    {
      num++;
    }
    i++;
    if(i >= 50)
      return fail;
  }
  return num;
}

/***********************************************************
* 函数功能：数字字符串转换为字符型
* 函数参数：string 字符串数字
* 返回值：数字字符
***********************************************************/
uint8_t String2Num(uint8_t* String)
{
  uint8_t L = 0;
  L = strlen((const char *)String);
  switch(L)
  {
    case 1:     return String[0] - 0x30;
    case 2:     return (String[0] - 0x30)*10 + (String[1] - 0x30);
    case 3:     return (String[0] - 0x30)*100 + (String[1] - 0x30)*10 + (String[2] - 0x30);
    default:    return fail;
  }
}

/***********************************************************
* 函数功能：WIFI接收到网络信息
* 函数参数：无
* 返回值：无
struct RxNetDataUnit  RNDU[MaxNum]; 接收数据的结构体数组
***********************************************************/
void WifiReceiveNetData(void)
{
  if(WifiReDataFlag)
  {
    WifiReDataFlag &= ~0x01;
    
    if(TheNumOfSign(WifiReNetData,',') > 1)
    {     
      memcpy(WifiReNetChannel,&WifiReNetData[LocationOfSign(WifiReNetData,',',1)+1],LocationOfSign(WifiReNetData,',',2) - LocationOfSign(WifiReNetData,',',1) - 1);
      RNDU.RxChannel = WifiReNetChannel[0];
      memcpy(WifiReNetDataLength,&WifiReNetData[LocationOfSign(WifiReNetData,',',2)+1],LocationOfSign(WifiReNetData,':',1) - LocationOfSign(WifiReNetData,',',2) - 1);
      RNDU.RxLength = String2Num(WifiReNetDataLength);
      memcpy(RNDU.RxData,&WifiReNetData[LocationOfSign(WifiReNetData,':',1)+1],RNDU.RxLength);
    }
    else 
    {
      RNDU.RxChannel = error;
      memcpy(WifiReNetDataLength,&WifiReNetData[LocationOfSign(WifiReNetData,',',1)+1],LocationOfSign(WifiReNetData,':',1) - LocationOfSign(WifiReNetData,',',1) - 1);
      RNDU.RxLength = String2Num(WifiReNetDataLength);
      memcpy(RNDU.RxData,&WifiReNetData[LocationOfSign(WifiReNetData,':',1)+1],RNDU.RxLength);
    }
    if (memcmp(RNDU.RxData,"D1=ON",strlen("D1=ON")) == 0) //D1=ON   开D1灯
    {
      D1_on();
    }
    if (memcmp(RNDU.RxData,"D1=OFF",strlen("D1=OFF")) == 0) //D1=OFF  关D1灯
    {
      D1_off();
    }
    
    memset(&RNDU.RxChannel,0,sizeof(RNDU));             //清除接收缓冲区
  }
}

/***********************************************************
* 函数功能：从对应的结构体中查询IP对应的通道号
* 函数参数：IP IP地址 Struct 对应的结构体
* 返回值：通道号 error 错误
***********************************************************/
uint8_t CheckChannelFromIP(uint8_t* IP,NetConnectUnit* Struct)
{
  uint8_t i = 0;
  while(Struct[i].Channel)
  {
    if(memcmp(Struct[i].ConnectIP,IP,strlen((const char*)IP)) == 0)
      return Struct[i].Channel-0x30;
    i++;
  }
  return error;
}

/***********************************************************
* 函数功能：发送拼接函数
* 函数参数：CMD     命令 
            Leng    长度      

            Channel 通道号 多连接有效  0xff 单连接
* 返回值：无
***********************************************************/
void SendJoinCMD(uint8_t* CMD,uint8_t Leng,uint8_t Channel)
{
  uint8_t LengArry[5] = {0};
  uint8_t One = 0;
  uint8_t Ten = 0;
  uint8_t Hun = 0;
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    Hun = Leng/100;
    Ten = Leng/10%10;
    One = Leng%10;
    if(Hun > 0)
    {
      LengArry[0] = Hun+0x30;
      LengArry[1] = Ten+0x30;
      LengArry[2] = One+0x30;
    }
    else if(Ten > 0)
    {
      LengArry[0] = Ten+0x30;
      LengArry[1] = One+0x30;
    }
    else if(One > 0)
    {
      LengArry[0] = One+0x30;
    }
    
    if(Channel != 0xff)
    { 
      Channel += 0x30;
      strcat((char *)CMD,(const char *)&Channel);
      strcat((char *)CMD,(const char *)",");
    }
    strcat((char *)CMD,(const char *)&LengArry);
    
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* 函数功能：发送数据
* 函数参数：Channel 通道号 多连接有效  0xff 单连接
            Leng 发送数据长度
* 返回值：无
***********************************************************/
void CMD_CIPSEND(uint8_t Channel,uint8_t Leng,uint8_t* CMD_Num)
{
  SendJoinCMD(AT_CIPSEND,Leng,Channel);

  if(SendCMDProcess(AT_CIPSEND,strlen((const char*)AT_CIPSEND),&CMD_ReturnSign,200,3,CMD_Type))       //重传时间最小单位为5ms
  {
    (*CMD_Num)++;
    memset(&AT_CIPSEND[11],0,sizeof(AT_CIPSEND)-11);
    CMD_OtherCount = fail;
  }
}

/***********************************************************
* 函数功能：发送数据
* 函数参数：Data 发送的数据
* 返回值：无
***********************************************************/
void DataSend(uint8_t *Data,uint8_t Length,uint8_t* CMD_Num)
{
  if(SendCMDProcess(Data,Length,&CMD_ReturnSendOK,200,3,NetData_Type))       //重传时间最小单位为5ms
  {
    *CMD_Num = 0;
    TNDU.TxDataFinish &= ~0x01;
    memset(TNDU.TxIP,0,sizeof(TNDU));
    WifiTxDataFlag &= ~0x01;
  }
}

/***********************************************************
* 函数功能：发送数据
* 函数参数：Channel 通道号 多连接有效  0xff 单连接
            Data 发送的数据
* 返回值：无
***********************************************************/
void WifiSend(uint8_t Channel,uint8_t *Data,uint8_t Length)
{
  switch(WifiSendDataNum)
  {
    case 0:     CMD_CIPSEND(Channel,Length,&WifiSendDataNum);   break;
    case 1:     DataSend(Data,Length,&WifiSendDataNum);         break;
    default:                                                    break;
  }
}

/***********************************************************
* 函数功能：发送数据
* 函数参数：IP IP地址
            Data 发送的数据
* 返回值：无
***********************************************************/
void WifiSendDataFromIP(uint8_t* IP,uint8_t* Data,uint8_t Length)
{
  uint8_t result = 0;
  if(!WifiClientModeFinishFlag)                  //非客户端模式
    result = CheckChannelFromIP(IP,NCU);
  else
    result = 0xff;
  
  if(result != error)                           //目标IP在 IP表中
    WifiSend(result,Data,Length);
  else                                          //目标IP不在 IP表中
  {
    TNDU.TxDataFinish &= ~0x01;
    memset(TNDU.TxIP,0,sizeof(TNDU));
    WifiTxDataFlag &= ~0x01;
  }
}

/***********************************************************
* 函数功能：WIFI发送网络信息
* 函数参数：无
* 返回值：无
struct TxNetDataUnit  TNDU;  发送数据的结构体数组
***********************************************************/
void WifiSendNetData(void)
{
  if(WifiTxDataFlag)
  {
    WifiSendDataFromIP(TNDU.TxIP,TNDU.TxData,TNDU.TxLength);  
  }
}

/***********************************************************
* 函数功能：发送数据和接收数据的操作
* 函数参数：无
* 返回值：无
***********************************************************/
void RxTxDataOperation(void)
{
  WifiReceiveNetData();                         //wifi网络数据提取
  WifiSendNetData();                            //WIFI发送网络数据
}

/***********************************************************
* 函数功能：客户端的操作
* 函数参数：无
* 返回值：无
      注：主要包括实现多连接，设置自身IP，创建TCP等
***********************************************************/
void ClientRxTxData(void)
{
  RxTxDataOperation();
}

/***********************************************************
* 函数功能：设置为客户端
* 函数参数：无
* 返回值：无
      注：主要包括实现多连接，设置自身IP，创建TCP等
***********************************************************/
void ClientOperation(void)
{
  switch(CMD_CountNum)
  {
    case 0:     CMD_CIPMUX(0,&CMD_CountNum);                                    break;          //设置连接模式为单连接或多连接
    case 1:     CMD_CIPSTART(0xff,Net,Target_IP,Target_Port,&CMD_CountNum);     break;          //建立网路络通道
    case 2:     WifiSendCmd("AT+CIPMODE=1\r\n",strlen("AT+CIPMODE=1\r\n"));delay_ms(300);
                WifiSendCmd("AT+CIPSEND\r\n",strlen("AT+CIPSEND\r\n"));delay_ms(300);
                WifiConfigFlag = 1;break;//ServerClientRxTxData();                                         break;          //收发数据
    default:                                                                    break;
  }
}

/***********************************************************
* 函数功能：端口拼接函数
* 函数参数：CMD     命令 
            Port    端口号      

            Num     连接状态
            0       关闭
            1       开启
* 返回值：无
***********************************************************/
void PortJoinCMD(uint8_t* CMD,uint8_t* Port,uint8_t Num)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    switch(Num)
    {
      case 0:   strcat((char *)CMD,(const char *)"0");  break;
      case 1:   strcat((char *)CMD,(const char *)"1");  break;
      default:                                            break;
    }
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)Port);
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* 函数功能：是否开启服务端 端口
* 函数参数：Num 
            0  关闭   
            1  开启
            Port 端口号
* 返回值：无
***********************************************************/
void CMD_CIPSERVER(uint8_t Num,uint8_t *Port,uint8_t* CMD_Num)
{
  PortJoinCMD(AT_CIPSERVER,Port,Num);
  if(SendCMDProcess(AT_CIPSERVER,strlen((const char*)AT_CIPSERVER),&CMD_ReturnOK,200,3,CMD_Type))       //重传时间最小单位为5ms
  {
    CMD_OtherCount = fail;
    (*CMD_Num)++; 
    if(NetMode == Net_Offline)                                                  //如果是离线模式跳过与外界建立SOCKET
      (*CMD_Num)++;    
    WifiServerModeFinishFlag |= 0x01;
    memset(&AT_CIPSERVER[13],0,sizeof(AT_CIPSERVER)-13);
  } 
}

/***********************************************************
* 函数功能：获取连接状态
* 函数参数：无
* 返回值：无
***********************************************************/
void CMD_CIPSTATUS(uint8_t* CMD_Num)
{
  if(SendCMDProcess(AT_CIPSTATUS,strlen((const char*)AT_CIPSTATUS),&CMD_ReturnOK,200,3,CMD_Type))           //重传时间最小单位为5ms
    (*CMD_Num)++;
}

/***********************************************************
* 函数功能：服务端客户端的收发操作
* 函数参数：无
* 返回值：无
***********************************************************/
void ServerClientRxTxData(void)
{
  switch(WifiCCCNum)
  {
    case 0:     CMD_CIPSTATUS(&WifiCCCNum);     break;          //获取当前模块已连接和被连接的信息 包括IP和端口
    case 1:     RxTxDataOperation();            break;          //收发操作
    default:                                    break;
  }
}

/***********************************************************
* 函数功能：设置为服务端
* 函数参数：无
* 返回值：无
      注：主要包括实现多连接，设置自身IP，创建TCP等
***********************************************************/
void ServerOperation(void)
{
  switch(CMD_CountNum)
  {
    case 0:     CMD_CIPMUX(1,&CMD_CountNum);                                    break;          //设置网路连接模式为单链接或多连接
    case 1:     CMD_CIPSERVER(1,Self_Port,&CMD_CountNum);                       break;          //开启服务端口
    case 2:     CMD_CIPSTART(0X31,Net,Target_IP,Target_Port,&CMD_CountNum);     break;          //创建网络通道
    case 3:     ServerClientRxTxData();                                         break;          //收发数据
    default:                                                                    break;
  }
  
}

/***********************************************************
* 函数功能：设置模块子模式
* 函数参数：WifiChildState 模块子模式  
            C_Client 客户端模式  
            C_Server 服务端模式  
* 返回值：无
* 备注： 定时去检查状态，定时时间为300ms
         当为客户端时，开启多连接，建立TCP连接
         当为服务端时，开启多连接，开启服务端口
***********************************************************/
void WIFIChildMode(WifiChildState C_Mode)
{
  if(WifiModeSetDown)                     //250ms
  {
    switch(C_Mode)
    {
      case C_Client:    ClientOperation();      break;
      case C_Server:    ServerOperation();      break;
      default:                                  break;
    }
  }
}

/***********************************************************
* 函数功能：对比函数
* 函数参数：Res 被对比的字符串  Des 对比字符串
* 返回值：fail 不匹配  finish 匹配 
***********************************************************/
uint8_t ContrastData(uint8_t *Res,uint8_t *Des)
{
  uint8_t i=0;
  for (i=0;i<sizeof(USART_Unit[3].USART_RxEffecDataBuf); i++)
  {
    if (memcmp(&Res[i],Des,strlen((const char *)Des)) == 0) 
    {
      WifiUartSignLoc = i + strlen((const char *)Des);
      WifiUartReLeng = USART_Unit[3].USART_RxLength - WifiUartSignLoc;
      memset(&Res[i], 0, strlen((const char *)Des));              //只将对比到的数据清空
      return success;
    }
  }           
  return fail;
}

/***********************************************************
* 函数功能：清除连接状态表
* 函数参数：无
* 返回值：无
struct NetConnectUnit NCU[5];
***********************************************************/
void RefreshConnectState(NetConnectUnit* List)
{
  uint8_t i = 0;
  while(List[i].Channel)
  {
    memset(&List[i].Channel,0,sizeof(List[i]));
    i++;
  }
}

/***********************************************************
* 函数功能：查看长字符串中指定的短字符串所含的数量
* 函数参数：string 长字符串 sign 指定的短字符串
* 返回值：所含数量
***********************************************************/
uint8_t TheNumOfShortString(uint8_t* string,uint8_t* ShortString)
{
  uint8_t i = 0;
  uint8_t Count = 0;
  for(i = 0;i < strlen((const char*)string);i++)
  {
    if(memcmp(&string[i],ShortString,strlen((const char *)ShortString)) == 0)
    {
      Count++;
    }
  }
  return Count;
}

/***********************************************************
* 函数功能：数据提取
* 函数参数：Loc起始位置  EndLoc 终止位置  String 源字符串  Data 得到的字符串
* 返回值：无
***********************************************************/
void GetString(uint8_t Loc,uint8_t EndLoc,uint8_t* String,uint8_t* Data)
{
  memcpy(Data,&String[Loc],EndLoc);
}

/***********************************************************
* 函数功能：查看长字符串中指定短字符串所在的位置
* 函数参数：string 长字符串 shortString 指定的短字符串  num 第几个
* 返回值：短字符串结束的下一个字符的位置
***********************************************************/
uint8_t LocationOfShortString(uint8_t* string,uint8_t* ShortString,uint8_t Num)
{
  uint8_t i = 0,Count = 0,Loc = 0;;
  for(i = 0;i < strlen((const char*)string);i++)
  {
    if(memcmp(&string[i],ShortString,strlen((const char *)ShortString)) == 0)
    {
      Count++;
      if(Count == Num)
        Loc = i+strlen((const char *)ShortString);
    }
  }
  return Loc;
}

/***********************************************************
* 函数功能：数据赋值给特定的结构体
* 函数参数：Struct 目标结构体  Data 需要被提取的数据
* 返回值：无
***********************************************************/
void SturctGetData(NetConnectUnit* Struct,uint8_t* Data)
{
  uint8_t Buf[5] = {0};
  uint8_t IPBuf[20] = {0};
  memcpy(Buf,Data,LocationOfSign(Data,',',1));
  Struct[0].Channel = Buf[0];                     //通道号
//  memcpy(Buf,&Data[LocationOfSign(Data,',',4)],strlen((const char *)Data));
//  Struct[0].ConnectState = String2Num(Buf);                //连接状态
  Struct[0].ConnectState |= 0x01;
  memcpy(IPBuf,&Data[LocationOfSign(Data,'"',3)+1],LocationOfSign(Data,'"',4) - LocationOfSign(Data,'"',3) - 1);
  memcpy(Struct[0].ConnectIP,IPBuf,strlen((const char*)IPBuf));                             //IP
}

/***********************************************************
* 函数功能：更新WIFI连接状态表
* 函数参数：List 连接状态表数组
* 返回值：无
***********************************************************/
void UpdateConnectState(uint8_t* List)
{
  uint8_t i = 0,Num = 0,j = 0,k = 0;
  uint8_t DataBuf[300] = {0};
  RefreshConnectState(NCU);
  Num = TheNumOfShortString(List,"+CIPSTATUS:");
  for(i = 0;i < Num;i++)
  {
    GetString(LocationOfShortString(List,"+CIPSTATUS:",i+1),LocationOfShortString(List,"\r\n",i+1)-3,List,DataBuf);
    while(NCU[j].Channel)               //检查是否有重复的IP
    {
      if(memcmp(NCU[j].ConnectIP,&DataBuf[LocationOfSign(DataBuf,'"',3)+1],LocationOfSign(DataBuf,'"',4) - LocationOfSign(DataBuf,'"',3) - 1) == 0)//IP重复
      {
        k++;
        SturctGetData(&NCU[j],DataBuf); //覆盖原IP信息
      }
      j++;
    }
    j = 0;
    if(!k)              //没有重复的IP
      SturctGetData(&NCU[i],DataBuf);
  }
}

/***********************************************************
* 函数功能：WIFI串口接受到数据后的数据处理
* 函数参数：无
* 返回值：无
***********************************************************/
void Usart_DataProcessing(UsartType* USART)
{
  uint8_t WifiListMesg[255] = {0};                       //WIFI连接状态表
  
  if(USART->USART_DataFinishFlag)
  {
    USART->USART_DataFinishFlag &= ~0x01;
    if(ContrastData(USART->USART_RxEffecDataBuf,"\r\n>"))
    {
      CMD_ReturnSign = success;
    }
    if(ContrastData(USART->USART_RxEffecDataBuf,"SEND OK\r\n"))
    {
      CMD_ReturnSendOK = success;
    }
    if(ContrastData(USART->USART_RxEffecDataBuf,"+IPD"))//收到数据置位WifiReDataFlag标志位
    {
      memcpy(WifiReNetData,&USART_Unit[3].USART_RxEffecDataBuf[WifiUartSignLoc],WifiUartReLeng);
      WifiReDataFlag = success;
    }
    if(ContrastData(USART->USART_RxEffecDataBuf,"ready\r\n")||ContrastData(USART->USART_RxEffecDataBuf,"invalid\r\n"))           //复位
    {
      CMD_ReturnReady = success;
      WifiModeSetDown &= ~0xff;
    }
    
    if((WifiServerModeFinishFlag || WifiClientModeFinishFlag))
    {
      if(ContrastData(USART->USART_RxEffecDataBuf,"CONNECT\r\n") || ContrastData(USART->USART_RxEffecDataBuf,"CLOSED\r\n") || ContrastData(USART->USART_RxEffecDataBuf,"CONNECT FAIL\r\n"))     //有设备连接
      {
        WifiCCCNum = fail;      //进度计数清零，发送查询网络连接状态命令
      }
      if(ContrastData(USART->USART_RxEffecDataBuf,"STATUS:3\r\n"))                                 //查询网络连接状态的返回值  3表示已连接或被连接    4当前无连接
      {
        NoneEquipOnlineFlag &= ~0x01;
        memcpy(WifiListMesg,&USART->USART_RxEffecDataBuf[WifiUartSignLoc],WifiUartReLeng);//提取设备信息  包括IP 通道号
        UpdateConnectState(WifiListMesg); 
      }
      else if(ContrastData(USART->USART_RxEffecDataBuf,"STATUS:4\r\n"))                         //查询网络连接状态的返回值  3表示已连接或被连接    4当前无连接
      {
        CMD_CountNum = 1;               //如果当前没有连接和被连接设备，则程序回转到建立端口的位置
        NoneEquipOnlineFlag |= 0x01;
        CMD_Count &= ~0x01;
      }
    }
    
    if(ContrastData(USART->USART_RxEffecDataBuf,"\r\nOK\r\n"))
    {
      if(!CMD_RSTFlag)                                  //非复位命令
        CMD_ReturnOK = success;
    }
  }
}

/***********************************************************
* 函数功能：WIFI串口接受和处理数据函数
* 函数参数：无
* 返回值：无
***********************************************************/
void Esp8266_Receive(void)
{
  USART_Unit_ReceiveData(&USART_Unit[3]);                         //WIFI串口
  
  Usart_DataProcessing(&USART_Unit[3]);                      //WIFI数据处理
}

/***********************************************************
* 函数功能：WIFI的相关操作
* 函数参数：无
* 返回值：无
***********************************************************/
void Wifi_Operation(void)
{
  Esp8266_Receive();
  
  WIFIMode(ESP8266_STA);
    
  WIFIChildMode(C_Client);
}