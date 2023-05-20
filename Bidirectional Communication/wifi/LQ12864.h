#ifndef LQ12864_H_
#define LQ12864_H_


#define OLED_Line_1     0x01
#define OLED_Line_2     0x02
#define OLED_Line_3     0x04
#define OLED_Line_4     0x08

void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_WrDat(unsigned char IIC_Data);	 
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_Set_Pos(unsigned char x, unsigned char y); 
void OLED_Fill(unsigned char bmp_dat); 
void OLED_Clear_Line(uint8_t LineNum);
void OLED_CLS(void);
void OLED_Init(void);
extern void OLED_P8x16Str(unsigned char x,unsigned char y,char ch[]);
void OLED_Display(uint8_t line,char *str);
void OLED_delay(unsigned int z);
void IIC_GPIO_Config(void);
void OLED_itoa(unsigned char x,unsigned char y,unsigned char *puls);
void OLED_P8x16StrLen(unsigned char x,unsigned char y,char ch[],unsigned char len);

#endif

