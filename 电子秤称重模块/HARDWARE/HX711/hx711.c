//////////////////////
//һǧ������
//DOUT��PB12
//SCK��PB13
//300g���̵�
//DOUT��PA4
//SCK��PA5
///////////////////////

#include "hx711.h"
#include "adc.h"
#include "delay.h"
#include "stm32f10x.h"

#define BigPD_SCK_Low GPIO_ResetBits(GPIOB,GPIO_Pin_12)//ʹ��AD��PD_SCK�õͣ�
#define BigPD_SCK_High GPIO_SetBits(GPIOB,GPIO_Pin_12)//PD_SCK�øߣ��������壩
#define BigPD_DOUT_Low GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define BigPD_DOUT_High GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define SmallPD_DOUT_Low GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define SmallPD_DOUT_High GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define SmallPD_SCK_Low GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define SmallPD_SCK_High GPIO_SetBits(GPIOA,GPIO_Pin_4)

void HX711_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure; 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A��Bʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 //SCK 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
GPIO_Init(GPIOB,&GPIO_InitStructure); 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 //DOUT 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  
GPIO_Init(GPIOB, &GPIO_InitStructure);	 //
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 //SCK 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
GPIO_Init(GPIOA,&GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 //DOUT 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  
GPIO_Init(GPIOA, &GPIO_InitStructure);	 //	
}

unsigned long ReadCount(void)//һǧ�����̵ĳ��ش�����
{
	unsigned long val = 0; 
unsigned char i = 0; 

BigPD_DOUT_High;   //DOUT=1 
BigPD_SCK_Low;	   //SCK=0 
while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13));   //??DOUT=0  
delay_us(1); 
for(i=0;i<24;i++) 
{ 
BigPD_SCK_High;	   //SCK=1 
val=val<<1; 
delay_us(1);  
BigPD_SCK_Low;	   //SCK=0 
if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))   //DOUT=1 
val++; 
delay_us(1); 
} 
BigPD_SCK_High; 
val = val^0x800000; 
delay_us(1); 
BigPD_SCK_Low; 
delay_us(1);  
return val; 	
}

////////////////////////////////////////
//300g�Ķ���
///////////////////////////////////////
unsigned long readcount(void)//300���̵ĳ��ش�����
{
	unsigned long val = 0; 
unsigned char i = 0; 

SmallPD_DOUT_High;	   //DOUT=1 
SmallPD_SCK_Low;	   //SCK=0 
while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5));   //??DOUT=0  
delay_us(1); 
for(i=0;i<24;i++) 
{ 
SmallPD_SCK_High;	   //SCK=1 
val=val<<1; 
delay_us(1);  
SmallPD_SCK_Low;	   //SCK=0 
if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))   //DOUT=1 
val++; 
delay_us(1); 
} 
SmallPD_SCK_High; 
val = val^0x800000; 
delay_us(1); 
SmallPD_SCK_Low; 
delay_us(1);  
return val; 	
}

//////////////////////////////
//1ǧ�˵Ļ�ȡ�����㷨
/////////////////////////////////
void hx711_filter(int a,double b)
{
	unsigned long hx711_value,sum=0;
	unsigned long hx711_filter[16]={0};
	int Weight_shiwu=0;//���ʵ������
	int i;
	for(i=0;i<16;i++)
		{
			hx711_filter[i] = ReadCount();
			sum+=hx711_filter[i];
			//delay_ms(100);
		}
		hx711_value = sum/16;//��ֵ�˲�
		//hx711_value = ((hx711_value-8263850)/1288490)*1000;//8263850����ǳ�ʼֵ
		Weight_shiwu = (hx711_value-8263850)/100;
		Weight_shiwu = (unsigned int)(((double)Weight_shiwu-a)/b+0.05);//0.05����������  368���  
		//printf("ѹ����%dg\r\n",Weight_shiwu);
		printf("%dg\r\n",Weight_shiwu);
}

//////////////////////////////////
//300g��
/////////////////////////////////
void Hx711_Filter(int a,int b)
{
	unsigned long hx711_value,sum=0;
	unsigned long hx711_filter[16]={0};
	int Weight_shiwu=0;//���ʵ������
	int i;
	for(i=0;i<16;i++)
		{
			hx711_filter[i] = readcount();
			sum+=hx711_filter[i];
			//delay_ms(100);
		}
		hx711_value = sum/16;//��ֵ�˲�
		//hx711_value = ((hx711_value-8545300)/4293524)*1000;//8263850����ǳ�ʼֵ
		Weight_shiwu = ((hx711_value))/100;
		Weight_shiwu = (unsigned int)(((float)Weight_shiwu-a)/b+0.05);//0.05����������  368���  
		printf("ѹ����%dg\r\n",Weight_shiwu);
}