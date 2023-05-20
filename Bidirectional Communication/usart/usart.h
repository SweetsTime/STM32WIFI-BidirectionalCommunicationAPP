#ifndef _USART_H_
#define _USART_H_

#include "handle.h"

#define USART_MaxNum 6

#define UartRxMaxLength 255

typedef struct{
  uint8_t USART_Receiving;                      //串口正在接收数据标志
  uint8_t USART_RxCount;                        //串口接受数据计数
  uint8_t USART_ANum;                           //用来确定串口接收数距是否完成
  uint8_t USART_DataFinishFlag;                 //串口接受完成表示
  uint8_t USART_RxLength;                       //串口接受到数据的长度
  uint8_t USART_RxBuf[UartRxMaxLength];         //串口接受到的有效数据
  uint8_t USART_RxEffecDataBuf[UartRxMaxLength];//从串口BUF中提取出的有效数据
}UsartType;

void Initial_USART1(u32 baudrate);
void Initial_USART2(u32 baudrate);
void Initial_USART3(u32 baudrate);
void Initial_UART4(u32 baudrate);
void Initial_UART5(u32 baudrate);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num);
void UART_NVIC_Configuration(void);

void USART_Unit_ReceiveData(UsartType* USART);

extern uint8_t USART_TimCount;                                 //串口1接收计数
extern UsartType USART_Unit[USART_MaxNum];

#endif

