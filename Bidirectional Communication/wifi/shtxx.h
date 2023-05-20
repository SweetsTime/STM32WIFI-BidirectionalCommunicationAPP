#ifndef  SHTXX_H
#define  SHTXX_H


#define  SCL_GPIO_PIN    GPIO_Pin_1
#define  SDA_GPIO_PIN    GPIO_Pin_2


#define  SCL_GPIO_PORT	 GPIOB
#define  SCL_GPIO_CLK	 RCC_APB2Periph_GPIOB


#define  SDA_GPIO_PORT	 GPIOB
#define  SDA_GPIO_CLK	 RCC_APB2Periph_GPIOB


#define  Clr_SHTXX_SCL()   GPIO_ResetBits(SCL_GPIO_PORT, SCL_GPIO_PIN)
#define  Set_SHTXX_SCL()   GPIO_SetBits(SCL_GPIO_PORT, SCL_GPIO_PIN)

#define  Set_SHTXX_SDA()   GPIO_SetBits(SDA_GPIO_PORT, SDA_GPIO_PIN)
#define  Clr_SHTXX_SDA()   GPIO_ResetBits(SDA_GPIO_PORT, SDA_GPIO_PIN)
#define  READ_SDA()      GPIO_ReadInputDataBit(SDA_GPIO_PORT, SDA_GPIO_PIN)
#define  WRITE_SDA(x)    GPIO_WriteBit(SDA_GPIO_PORT, SDA_GPIO_PIN, x)      

enum {TEMP,HUMI};

void SHTXX_GPIOConfig(void);
void SDA_OUT(void);
void SDA_IN(void);

void SHTXX_Init(void);
static char WriteByte(unsigned char value);
static char ReadByte(unsigned char ack);
void ConnectionReset(void);
static void TransStart(void);
unsigned char Measure(unsigned char *p_value, unsigned char *p_checkum, unsigned char mode);
void SHTXX_Cal(unsigned short *temp, unsigned short *humi, float *f_temp, float *f_humi );

#endif