#include "adc.h"

void adc_init(u8 ADC_res,u8 ADC_input_selection,u8 ADC_interrupt_enabled)
{
 	
   if(ADC_input_selection <= 7)
     {                     //��Сλ                         //BBB=2
       NRF_ADC->CONFIG = ADC_res << ADC_CONFIG_RES_Pos |  ADC_CONFIG_INPSEL_AnalogInputNoPrescaling/*AnalogInputOneThirdPrescaling*/ << ADC_CONFIG_INPSEL_Pos |
                          ADC_CONFIG_REFSEL_VBG/*SupplyOneThirdPrescaling*/ << ADC_CONFIG_REFSEL_Pos | (1 << ADC_input_selection) << ADC_CONFIG_PSEL_Pos;
                          //cc=3
			// NRF_ADC->CONFIG |= ADC_CONFIG_REFSEL_VBG
			 NRF_ADC->ENABLE = 1;
				nrf_gpio_cfg_output(adc_nf);
				ADC_F();
				
    }
 }
 
u16 nrf_adc_read(void)  //��ȡ����
{
  u16 adc_data;
	ADC_N(); 
	nrf_delay_ms(10);  //�ȴ���ѹ�ȶ�
  NRF_ADC->TASKS_START = 1;
  while(NRF_ADC->EVENTS_END == 0);
  NRF_ADC->EVENTS_END = 0;
  adc_data = NRF_ADC->RESULT;
	ADC_F();
  return adc_data;
}

void adc_stop()
{
	NRF_ADC->ENABLE =0;  //�ر�ADC
}

void ADC_N() //����ADC������Դ
{
	nrf_gpio_cfg_output(adc_nf);
	nrf_gpio_pin_set(adc_nf);
}

void ADC_F() //�ر�ADC������Դ
{
//	nrf_gpio_cfg_output(adc_nf);
//	nrf_gpio_pin_clear(adc_nf);
	nrf_gpio_cfg_input(adc_nf,NRF_GPIO_PIN_PULLDOWN);//����Ϊ��������
//	//nrf_gpio_cfg_default(adc_nf);
}

