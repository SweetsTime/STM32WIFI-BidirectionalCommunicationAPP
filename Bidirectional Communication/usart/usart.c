#include "usart.h"

/*变量定义*/
uint8_t SpecialEffecDataFlag = 0;
/*变量定义*/

//注意IAR要加上 _DLIB_FILE_DESCRIPTOR 宏定义
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


UsartType USART_Unit[USART_MaxNum];

/***********************************************************
* 名称: Initial_USART1()
* 功能: 串口1初始化程序，将串口初始化为
        - 波特率 = baudrate
        - 字长 = 8 位
        - 无停止位
        - 无奇偶校验位
        - 禁止硬件流控制
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Initial_USART1(u32 baudrate)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

  /* 配置串口1 Tx（PA9）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口1 Rx（PA10）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口1 的各种参数 */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  /* 串口1开始工作 */
  USART_Cmd(USART1, ENABLE);
  
  USART_ClearFlag(USART1,USART_FLAG_TC);
}

/***********************************************************
* 名称: Initial_USART2()
* 功能: 串口2初始化程序，将串口初始化为
        - 波特率 = baudrate
        - 字长 = 8 位
        - 无停止位
        - 无奇偶校验位
        - 禁止硬件流控制
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Initial_USART2(u32 baudrate)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* 配置串口2 Tx（PA2）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口2 Rx（PA3）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口2 的各种参数 */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  /* 串口1开始工作 */
  USART_Cmd(USART2, ENABLE);
  
  USART_ClearFlag(USART2,USART_FLAG_TC);
}

/***********************************************************
* 名称: Initial_USART3()
* 功能: 串口3初始化程序，将串口初始化为
        - 波特率 = baudrate
        - 字长 = 8 位
        - 无停止位
        - 无奇偶校验位
        - 禁止硬件流控制
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Initial_USART3(u32 baudrate)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* 配置串口3 Tx（PB10）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* 配置串口3 Rx（PB11）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* 配置串口3 的各种参数 */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  /* 串口3开始工作 */
  USART_Cmd(USART3, ENABLE);
  
  USART_ClearFlag(USART3,USART_FLAG_TC);
}

/***********************************************************
* 名称: Initial_UART4()
* 功能: 串口4初始化程序，将串口初始化为
        - 波特率 = baudrate
        - 字长 = 8 位
        - 无停止位
        - 无奇偶校验位
        - 禁止硬件流控制
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Initial_UART4(u32 baudrate)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); //开启USART4时钟

  /* 配置串口4 Tx（PC10）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* 配置串口4 Rx（PC11）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* 配置串口4 的各种参数 */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(UART4, &USART_InitStructure);
  //使能USART4接收中断
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
  //使能USART4
  USART_Cmd(UART4, ENABLE); 
  USART_ClearFlag(UART4,USART_FLAG_TC);
}

/***********************************************************
* 名称: Initial_UART5()
* 功能: 串口5初始化程序，将串口初始化为
        - 波特率 = baudrate
        - 字长 = 8 位
        - 无停止位
        - 无奇偶校验位
        - 禁止硬件流控制
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Initial_UART5(u32 baudrate)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

  /* 配置串口5 Tx（PC12）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* 配置串口5 Rx（PD2）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* 配置串口5 的各种参数 */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(UART5, &USART_InitStructure);
  USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
  /* 串口5开始工作 */
  USART_Cmd(UART5, ENABLE);
  USART_ClearFlag(UART5,USART_FLAG_TC);
}

/***********************************************************
* 名称: USART_ReceiveData(UsartType USART_Unit* USART)
* 功能: 判断串口接收数据是否完成
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void USART_Unit_ReceiveData(UsartType* USART)
{
  if(USART_TimCount >= 2 && USART->USART_Receiving)                                     //10ms
  {  
    USART_TimCount = 0;
    if(USART->USART_ANum == USART->USART_RxCount)                                                 //数据接收完成
    {
      USART->USART_DataFinishFlag |= 0X01;
      USART->USART_Receiving &= ~0x01;                                                    //串口接受数据结束
      USART->USART_RxLength = USART->USART_RxCount;                                   //提取接收到数据长度
      USART->USART_RxCount &= ~0xff;
      
      memcpy(USART->USART_RxEffecDataBuf,&USART->USART_RxBuf,USART->USART_RxLength);
      
      Usart_SendNumString(USART1,USART->USART_RxEffecDataBuf,USART->USART_RxLength);
      
      memset(USART->USART_RxBuf,0,sizeof(USART->USART_RxBuf));//清空接收缓冲区
    }
    else
    {
      USART->USART_ANum = USART->USART_RxCount;
    }
  }
}

/***********************************************************
* 名称: USART_Unit_Special_ReceiveData(UsartType USART_Unit* USART)
* 功能: 判断串口接收数据是否完成
* 参数: 无
* 返回: 无
* 修改:
* 注释: 因为此次接受的数据比较特殊 会通过串口大量的不停的传输，因此需要根据
        接受到的数据来判断是否接收到有效数据
***********************************************************/
void USART_Unit_Special_ReceiveData(UsartType* USART,USART_TypeDef* USARTx)
{
  uint8_t i = 0;
  
  if(USART->USART_Receiving)
  {
    for(i = 0;i < USART->USART_RxCount;i++)
    {
      if(USART->USART_RxBuf[i] == 0xff && (USART->USART_RxBuf[i+1] == 0xc0 || USART->USART_RxBuf[i+1] == 0xc7 || USART->USART_RxBuf[i+1] == 0xcb || USART->USART_RxBuf[i+1] == 0xcc))
      {
        SpecialEffecDataFlag |= 0x01;
        break;
      }
    }
    if(SpecialEffecDataFlag && USART->USART_RxBuf[i+2])
    {
      if(USART->USART_RxCount - i >= (USART->USART_RxBuf[i+2] + 2))
      {
        memcpy(USART->USART_RxEffecDataBuf,&USART->USART_RxBuf[i],USART->USART_RxBuf[i+2] + 2);
        SpecialEffecDataFlag &= ~0x01;
        
        USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);//关闭串口中断
        USART->USART_Receiving &= ~0x01;
        USART->USART_DataFinishFlag |= 0x01;
        USART->USART_RxCount &= ~0xff;
        USART->USART_RxLength = USART->USART_RxBuf[i+2] + 2;
        memset(USART->USART_RxBuf,0,250);//清空接收缓冲区
      }
    }
  }
}

/***********************************************************
* 名称: UART_NVIC_Configuration
* 功能: 中断配置
* 参数: 无
* 返回: 无
* 修改:
* 注释: 添加中断以及配置中断的优先级
***********************************************************/
void UART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //串口
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
//  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStruct);
//  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
//  NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStruct);
//  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
//  NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStruct);
}

/***********************************************************
* 名称: PUTCHAR_PROTOTYPE
* 功能: 输出一个字节到串口
* 参数: 无
* 返回: 无
* 修改:
* 注释: 这个函数在iar中实际上是putchar函数(文件开头定义)，实现了putchar这个函数
后，编译器就能够使用printf函数。
***********************************************************/
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART2, (uint8_t) ch);
  
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {}
  
  return ch;
}

/***********************************************************
* 名称: fgetc
* 功能: 从串口接收一个字节
* 参数: 无
* 返回: 无
* 修改:
* 注释: 只要实现了这个函数就可以使用scanf了。
***********************************************************/
int fgetc(FILE *f)
{

    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {}

    return (int)USART_ReceiveData(USART2);
}

/***********************************************************
* 名称: Usart_SendByte()
* 功能: 发送一个字符
* 参数: pUSARTx 串口  ch  需要发送的字符
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
  USART_SendData(pUSARTx,ch);
          
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/***********************************************************
* 名称: Usart_SendString(USART_TypeDef * pUSARTx, uint8_t *str)
* 功能: 发送一个字符串
* 参数: pUSARTx 串口  ch  需要发送的字符串
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
  unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(200);
      k++;
  } while(*(str + k)!='\0');

  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***********************************************************
* 名称: Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num)
* 功能: 发送固定长度的字符串
* 参数: pUSARTx 串口  str  需要发送的字符串  Num需要发送的长度
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num)           
{
  unsigned int k=0;
  for(k = 0;k < Num;k++)
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(200);
  }
  
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***********************************************************
* 名称: USART_Unit_IRQ_ReceiveData(UsartType* USART,USART_TypeDef* USARTx)
* 功能: 从串口中断中提取数据  USARTx 串口号
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void USART_Unit_IRQ_ReceiveData(UsartType* USART,USART_TypeDef* USARTx)
{
  uint8_t USART_RxData = 0;
  
  USART->USART_Receiving = 1; 
      
  USART_RxData = USART_ReceiveData(USARTx);
  
  if (USART->USART_RxCount < sizeof(USART->USART_RxBuf)-5)
  {
    USART->USART_RxBuf[USART->USART_RxCount++] = USART_RxData;
  }
  else 
  {
    USART->USART_RxCount = 0;
  }
}

/***********************************************************
* 名称: USART1_IRQHandler()
* 功能: USART1 中断函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void USART1_IRQHandler(void)        
{
  if(USART_GetFlagStatus(USART1,USART_IT_RXNE))   //接收中断 (接收寄存器非空) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[1],USART1); 
  } 
}

/***********************************************************
* 名称: USART2_IRQHandler()
* 功能: USART2 ESP8266中断函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void USART2_IRQHandler(void)       
{
  if(USART_GetFlagStatus(USART2,USART_IT_RXNE))   //接收中断 (接收寄存器非空) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[2],USART2);
  } 
}

/***********************************************************
* 名称: USART3_IRQHandler()
* 功能: USART3 中断函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void USART3_IRQHandler(void)      
{
  if(USART_GetFlagStatus(USART3,USART_IT_RXNE))   //接收中断 (接收寄存器非空) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[3],USART3);
  } 
}

/***********************************************************
* 名称: UART4_IRQHandler()
* 功能: UART4 中断函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void UART4_IRQHandler(void)        
{
  if(USART_GetFlagStatus(UART4,USART_IT_RXNE))   //接收中断 (接收寄存器非空) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[4],UART4);
  } 
}

/***********************************************************
* 名称: UART5_IRQHandler()
* 功能: UART5 中断函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void UART5_IRQHandler(void)        
{
  if(USART_GetFlagStatus(UART5,USART_IT_RXNE))   //接收中断 (接收寄存器非空) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[5],UART5);
  } 
}