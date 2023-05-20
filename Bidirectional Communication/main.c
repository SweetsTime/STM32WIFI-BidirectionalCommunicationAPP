#include "handle.h"
#include "shtxx.h"
#include "LQ12864.h"
#include "wifi.h"
/***********************************************************
* 名称: main()
* 功能: 主函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
int WifiConfigFlag = 0;
void main(void)
{
  //int count = 300;
   uint8_t Error, checksum;
  uint16_t humi_val, temp_val;
  char temp[16] = {0},humi[16] = {0};
  float f_humi, f_temp;
  EquipInit();
  //Wifi_Operation();                           //WIFI相关操作
  for(;;)
  {
    if(!WifiConfigFlag)
    {
      Wifi_Operation();                           //WIFI相关操作
    
      USART_Operation();                          //串口相关操作
    
      NetStateMonitor();                          //连接异常监测 
    }
    else
    {
     Error = 0;
    Error += Measure((unsigned char*) &temp_val,&checksum,TEMP);  //测量温度
    Error += Measure((unsigned char*) &humi_val,&checksum,HUMI);  //测量湿度
    
    if (Error == 0) 
    {
      //通过公式将读取的温湿度的值转换成可识别的值
      SHTXX_Cal(&temp_val, &humi_val, &f_temp, &f_humi);
      sprintf(temp, "Temp = %.1fC", f_temp);
      sprintf(humi, "Humi = %.1f%%", f_humi);
      
      OLED_Clear_Line(OLED_Line_3);
      OLED_Clear_Line(OLED_Line_4);
      OLED_Display(OLED_Line_3, temp);   //OLED屏打印温湿度信息
      OLED_Display(OLED_Line_4, humi);
      sprintf(TNDU.TxData,"%s,%s\n",temp,humi);
      WifiSendCmd(TNDU.TxData,strlen(TNDU.TxData));
     // printf("Temp = %.1f℃ , Humi = %.1f%%\r\n", f_temp, f_humi);  //串口打印温湿度信息
    }
    delay_ms(2000);
    }
  }
  
}