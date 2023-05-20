#include "stm32f10x.h"
/*************三轴加速度传感器模拟IIC协议引脚***********************/
#ifdef ACCELERATION
#define SCL_H         GPIOA->BSRR = GPIO_Pin_6
#define SCL_L         GPIOA->BRR  = GPIO_Pin_6    
#define SDA_H         GPIOB->BSRR = GPIO_Pin_0
#define SDA_L         GPIOB->BRR  = GPIO_Pin_0
#define SCL_read      GPIOA->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_0
#endif
void I2C_GPIO_Config(void);
void I2C_delay(void);
int I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
int I2C_WaitAck(void) ;
void I2C_SendByte(u8 SendByte);
u8 I2C_ReceiveByte(void);
