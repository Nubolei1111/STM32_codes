
//��Ӣ�幩����4.76V
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "hx711.h"
#include "usmart.h"
    
 int main(void)
 {	
	 u16 adcx;
	float temp;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	usmart_dev.init(SystemCoreClock/1000000);	//��ʼ��USMART	
	HX711_Init();
 	Adc_Init();		  		//ADC��ʼ��       
	while(1)
	{
		//Hx711_Filter(85580,36);300g
		hx711_filter(333,20.15);
}
 }
