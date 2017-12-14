
//精英板供电是4.76V
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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART	
	HX711_Init();
 	Adc_Init();		  		//ADC初始化       
	while(1)
	{
		//Hx711_Filter(85580,36);300g
		hx711_filter(333,20.15);
}
 }
