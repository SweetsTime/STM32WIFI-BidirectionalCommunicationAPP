
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "iic2.h"
#include "delay.h"
/***********************************************************
* 名称: IIC_GPIO_Config()
* 功能: IIC初始化
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
/********************三轴加速度传感器IIC引脚配置**********************/
#ifdef ACCELERATION
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOA, ENABLE);
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}
/***********************************************************
* 名称: I2C_delay()
* 功能: IIC延时函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void I2C_delay(void)
{
    u8 i = 50; //这里可以优化速度	，经测试最低到5还能写入
    while(i)
    {
        i--;
    }
}
/***********************************************************
* 名称: I2C_Start()
* 功能: IIC开始函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
int I2C_Start(void)
{
  SDA_H;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SDA_L;
  I2C_delay();
  SCL_L;
  I2C_delay();
  return 0;
}

/***********************************************************
* 名称: Stop()
* 功能: IIC停止函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void I2C_Stop(void)
{
  SCL_L;
  I2C_delay();
  SDA_L;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SDA_H;
  I2C_delay();
}

/***********************************************************
* 名称: I2C_Ack()
* 功能: 主机发送应答信号
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void I2C_Ack(void)
{	
  SCL_L;
  I2C_delay();
  SDA_L;   //SDA低电平
  I2C_delay();
  SCL_H; //SCL高电平
  I2C_delay();
  SCL_L;
  I2C_delay();
}

/***********************************************************
* 名称: I2C_NoAck()
* 功能: 不发送确认信号
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void I2C_NoAck(void)
{	
  SCL_L;
  I2C_delay();
  SDA_H;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SCL_L;
  I2C_delay();
}
/***********************************************************
* 名称: I2C_WaitAck()
* 功能: 等待从设备发送应答信号
* 参数: 无
* 返回: 1为无ACK,0为有ACK
* 修改:
* 注释: 
***********************************************************/
int I2C_WaitAck(void) 	
{
  SCL_L;
  I2C_delay();
  SDA_H;			
  I2C_delay();
  SCL_H;
  I2C_delay();
  if(SDA_read)
  {
    SCL_L;
    return 1;
  }
  SCL_L;
  return 0;
}

/***********************************************************
* 名称: I2C_SendByte()
* 功能: 发送八位数据
* 参数: 输入 SendByte 需要发送的字节
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
  u8 i=8;
  while(i--)
  {
    SCL_L;
    I2C_delay();
    if(SendByte&0x80)
    {
      SDA_H;
    }
    else 
    {
      SDA_L;
    }
    SendByte<<=1;
    I2C_delay();
    SCL_H;
    I2C_delay(); 
  }
}

/***********************************************************
* 名称: I2C_ReceiveByte()
* 功能: 读取八位数据
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
u8 I2C_ReceiveByte(void)  //数据从高位到低位//
{ 
  u8 i=8;
  u8 ReceiveByte=0;
  SDA_H;
  while(i--)
  {
    ReceiveByte<<=1;      
    SCL_L;
    I2C_delay();
    SCL_H;//读数据时 SCL为高电平
    I2C_delay();	
    if(SDA_read)
    {
      ReceiveByte|=0x01;
    }
  }
  return ReceiveByte;
}

