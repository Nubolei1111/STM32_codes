#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

/************************************************
PA0.PB6
************************************************/
 

void upspeed();
void downspeed();
extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM4CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM4CH1_CAPTURE_VAL;	//输入捕获值	
u32 tempup=0;
float speedup=0.0,Tempup=0.0;
u32 tempdown=0;
	float speeddown=0.0,Tempdown=0.0;
 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
// 	LED_Init();			     //LED端口初始化
 
 	TIM3_PWM_Init(899,0); 		//不分频。PWM频率=72000/(899+1)=80Khz
 	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	TIM4_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
   	while(1)
	{
 		upspeed();
		downspeed();
	}
 }

void upspeed()
{
	delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);

		if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次下降沿
		{
			tempup=TIM5CH1_CAPTURE_STA&0X3F;
			tempup*=65536;//溢出时间总和
			tempup+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			printf("timeup:%d us\r\n",tempup);//打印总的高点平时间
			Tempup=tempup/(float)1000000;
		  speedup = 0.005/Tempup;
	    printf("%fm/s\r\n",speedup);
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
}

void downspeed()
{
	delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);

		if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
		 		 
 		if(TIM4CH1_CAPTURE_STA&0X80)//成功捕获到了一次下降沿
		{
			tempdown=TIM4CH1_CAPTURE_STA&0X3F;
			tempdown*=65536;//溢出时间总和
			tempdown+=TIM4CH1_CAPTURE_VAL;//得到总的高电平时间
			printf("timedown:%d us\r\n",tempdown);//打印总的高点平时间
			Tempdown=tempdown/(float)1000000;
		  speeddown = 0.005/Tempdown;
	    printf("%fm/s\r\n",speeddown);
			TIM4CH1_CAPTURE_STA=0;//开启下一次捕获
		}
}