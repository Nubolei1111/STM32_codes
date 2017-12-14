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
extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ	
extern u8  TIM4CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ	
u32 tempup=0;
float speedup=0.0,Tempup=0.0;
u32 tempdown=0;
	float speeddown=0.0,Tempdown=0.0;
 int main(void)
 {		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
// 	LED_Init();			     //LED�˿ڳ�ʼ��
 
 	TIM3_PWM_Init(899,0); 		//����Ƶ��PWMƵ��=72000/(899+1)=80Khz
 	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
	TIM4_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
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
		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ���½���
		{
			tempup=TIM5CH1_CAPTURE_STA&0X3F;
			tempup*=65536;//���ʱ���ܺ�
			tempup+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			printf("timeup:%d us\r\n",tempup);//��ӡ�ܵĸߵ�ƽʱ��
			Tempup=tempup/(float)1000000;
		  speedup = 0.005/Tempup;
	    printf("%fm/s\r\n",speedup);
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
		}
}

void downspeed()
{
	delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);

		if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
		 		 
 		if(TIM4CH1_CAPTURE_STA&0X80)//�ɹ�������һ���½���
		{
			tempdown=TIM4CH1_CAPTURE_STA&0X3F;
			tempdown*=65536;//���ʱ���ܺ�
			tempdown+=TIM4CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			printf("timedown:%d us\r\n",tempdown);//��ӡ�ܵĸߵ�ƽʱ��
			Tempdown=tempdown/(float)1000000;
		  speeddown = 0.005/Tempdown;
	    printf("%fm/s\r\n",speeddown);
			TIM4CH1_CAPTURE_STA=0;//������һ�β���
		}
}