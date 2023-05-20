#ifndef _WIFI_H_
#define _WIFI_H_

#include "handle.h"

#define error 0xfe

#define MaxNum 5

#define Net_Online      0x01
#define Net_Offline     0x02

#define CMD_Type        1               //普通命令
#define NetData_Type    2               //网络数据命令
#define ATTest_Type     3               //AT测试命令
#define Spec_Type       4               //特殊命令，此命令发出后返回值携带有需要判断的参数
#define Choice_type     5               //选择性命令，此命令如果返回失败，程序从特定的位置开始继续执行

typedef enum{
  ESP8266_NULL = 0,
  ESP8266_STA,
  ESP8266_AP,                   //暂不开放
  ESP8266_STAandAP              //暂不开放
}WifiState;

typedef enum
{
  C_Null = 0,
  C_Client,
  C_Server
}WifiChildState;

typedef struct
{
  uint8_t Channel;              //通道号
  uint8_t ConnectState;         //连接状态
  uint8_t ConnectIP[20];           //IP
}NetConnectUnit;

typedef struct            //接收到的数据单元
{
  uint8_t RxChannel;
  uint8_t RxLength;
  uint8_t RxData[255];
}RxNetDataUnit;

typedef struct             //发送的数据单元
{
  uint8_t TxIP[20];
  uint8_t TxLength;
  uint8_t TxData[255];
  uint8_t TxDataFinish;         //当一帧数据包中需要提取多个有效数据时需要 例如 一帧数据包中需要提取10个温度信息
}TxNetDataUnit;

void WiFi_Init(void);
void Wifi_Operation(void);
void ServerClientRxTxData(void);
void WifiSendCmd(uint8_t* data,uint8_t Leng);

extern int WifiConfigFlag;//WiFi配置完成

extern TxNetDataUnit  TNDU;
extern uint16_t CMD_ReSendTim;
extern uint8_t NetModeAbnormal;
extern uint8_t Target_IP[15];
extern uint8_t WifiTxDataFlag;
extern uint8_t WifiModeSetDown;
extern uint8_t CMD_CountNum;
extern uint8_t WifiCCCNum;
extern uint8_t CMD_OtherCount;
extern uint8_t AT_CWJAP[50];
extern uint8_t AT_CIPSTA[30];
extern uint8_t AT_CIPSTART[50];
extern uint8_t AT_CIPSEND[20];
extern uint8_t AT_CIPSERVER[25];

void WifiSendDataFromIP(uint8_t* IP,uint8_t* Data,uint8_t Length);

#endif
