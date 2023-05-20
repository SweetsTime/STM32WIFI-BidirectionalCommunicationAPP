#include "tim.h"


/*变量定义*/
uint8_t USART_TimCount = 0;                                 //串口1接收计数
uint8_t KeyPushedTimCount = 0;                               //按键被按下的计数

uint8_t CMD_TimCount = 0;                                    //命令设置周期
uint8_t WifiSendDataTimCount = 0;                            //wifi发送数据周期


/*变量定义*/

void time2_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
       
    //打开TIM2外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
                          
   //**************************************************************************
   //     定时器2设置： 72分频，5ms中断一次，向上计数
   //**************************************************************************
    TIM_TimeBaseStructure.TIM_Period = 5000;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //开定时器中断
    
    TIM_Cmd(TIM2, ENABLE); //使能定时器

     // 使能TIM2中断
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void time3_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ; 
  
    //打开TIM3外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
   //**************************************************************************
   //     定时器3设置： 720分频，250ms中断一次，向上计数
   //**************************************************************************    
    TIM_TimeBaseStructure.TIM_Period = 25000;
    TIM_TimeBaseStructure.TIM_Prescaler = 720-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //开定时器中断
    
    TIM_Cmd(TIM3, ENABLE); //使能定时器    
    
    // 使能TIM3中断
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);    
}

void time4_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;

   //打开TIM4外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
    
   //**************************************************************************
   //     定时器4设置： 1440分频，1000ms中断一次，向上计数
   //**************************************************************************
    
    TIM_TimeBaseStructure.TIM_Period = 50000;
    TIM_TimeBaseStructure.TIM_Prescaler = 1439;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //开定时器中断
    
    TIM_Cmd(TIM4, ENABLE);    //使能定时器  
    
    // 使能TIM4中断
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);    
}

void TIM2_IRQHandler(void)                      //5ms
{ 
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    USART_TimCount++;
    if(USART_TimCount >= 250)
      USART_TimCount = 0;
    
    KeyPushedTimCount++;
    if(KeyPushedTimCount >= 250)
      KeyPushedTimCount = 0;
    
    CMD_TimCount++;
    if(CMD_TimCount >= 250)
      CMD_TimCount = 0;
    
    CMD_ReSendTim++;
    if(CMD_ReSendTim >= 65530)
      CMD_ReSendTim = 0;
  }    
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
    {
      TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }  
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{ 
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
   {
     TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
   }  
}
