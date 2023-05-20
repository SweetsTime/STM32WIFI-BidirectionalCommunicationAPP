
#include "adc.h"
/***********************************************************
* 名称: RCC_Configuration()
* 功能: adc时钟配置程序
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void RCC_Configuration(void)
{
  /* Enable ADC1 and GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
}

/***********************************************************
* 名称: GPIO_Configuration()
* 功能: stm32 adc GPIO配置程序
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure PA.4 (ADC Channel8) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/***********************************************************
* 名称: adc_init()
* 功能: stm32 adc 初始化
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void adc_init(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  // ADC1 配置
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 通道8配置 */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC1 reset calibration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
  
}

/***********************************************************
* 名称: read_ADC()
* 功能: 读取ADC1的采回来的值
* 参数: 无
* 返回: 输出 16位的adc采样值
* 修改:
* 注释: 
***********************************************************/
uint16_t read_ADC(void)
{
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //启动ADC1转换
  while(! ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待ADC转换完毕
  return ADC_GetConversionValue(ADC1);        //读取adc数值
}
