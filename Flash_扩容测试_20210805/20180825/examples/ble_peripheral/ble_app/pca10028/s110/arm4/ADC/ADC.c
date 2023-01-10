#include "adc.h"

void adc_init(u8 ADC_res,u8 ADC_input_selection,u8 ADC_interrupt_enabled)
{
 	
   if(ADC_input_selection <= 7)
     {                     //多小位                         //BBB=2
       NRF_ADC->CONFIG = ADC_res << ADC_CONFIG_RES_Pos |  ADC_CONFIG_INPSEL_AnalogInputNoPrescaling/*AnalogInputOneThirdPrescaling*/ << ADC_CONFIG_INPSEL_Pos |
                          ADC_CONFIG_REFSEL_VBG/*SupplyOneThirdPrescaling*/ << ADC_CONFIG_REFSEL_Pos | (1 << ADC_input_selection) << ADC_CONFIG_PSEL_Pos;
                          //cc=3
			// NRF_ADC->CONFIG |= ADC_CONFIG_REFSEL_VBG
			 NRF_ADC->ENABLE = 1;
				nrf_gpio_cfg_output(adc_nf);
				ADC_F();
				
    }
 }
 
u16 nrf_adc_read(void)  //读取数据
{
  u16 adc_data;
	ADC_N(); 
	nrf_delay_ms(10);  //等待电压稳定
  NRF_ADC->TASKS_START = 1;
  while(NRF_ADC->EVENTS_END == 0);
  NRF_ADC->EVENTS_END = 0;
  adc_data = NRF_ADC->RESULT;
	ADC_F();
  return adc_data;
}

void adc_stop()
{
	NRF_ADC->ENABLE =0;  //关闭ADC
}

void ADC_N() //开启ADC采样电源
{
	nrf_gpio_cfg_output(adc_nf);
	nrf_gpio_pin_set(adc_nf);
}

void ADC_F() //关闭ADC采样电源
{
//	nrf_gpio_cfg_output(adc_nf);
//	nrf_gpio_pin_clear(adc_nf);
	nrf_gpio_cfg_input(adc_nf,NRF_GPIO_PIN_PULLDOWN);//设置为下拉输入
//	//nrf_gpio_cfg_default(adc_nf);
}

