//////////////////////
//一千克量程
//DOUT接PB12
//SCK接PB13
//300g量程的
//DOUT接PA4
//SCK接PA5
///////////////////////

#include "hx711.h"
#include "adc.h"
#include "delay.h"
#include "stm32f10x.h"

#define BigPD_SCK_Low GPIO_ResetBits(GPIOB,GPIO_Pin_12)//使能AD（PD_SCK置低）
#define BigPD_SCK_High GPIO_SetBits(GPIOB,GPIO_Pin_12)//PD_SCK置高（发送脉冲）
#define BigPD_DOUT_Low GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define BigPD_DOUT_High GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define SmallPD_DOUT_Low GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define SmallPD_DOUT_High GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define SmallPD_SCK_Low GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define SmallPD_SCK_High GPIO_SetBits(GPIOA,GPIO_Pin_4)

void HX711_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure; 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能A，B时钟

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

unsigned long ReadCount(void)//一千克量程的称重传感器
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
//300g的读数
///////////////////////////////////////
unsigned long readcount(void)//300量程的称重传感器
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
//1千克的获取波的算法
/////////////////////////////////
void hx711_filter(int a,double b)
{
	unsigned long hx711_value,sum=0;
	unsigned long hx711_filter[16]={0};
	int Weight_shiwu=0;//存放实际重量
	int i;
	for(i=0;i<16;i++)
		{
			hx711_filter[i] = ReadCount();
			sum+=hx711_filter[i];
			//delay_ms(100);
		}
		hx711_value = sum/16;//均值滤波
		//hx711_value = ((hx711_value-8263850)/1288490)*1000;//8263850这个是初始值
		Weight_shiwu = (hx711_value-8263850)/100;
		Weight_shiwu = (unsigned int)(((double)Weight_shiwu-a)/b+0.05);//0.05是四舍五入  368误差  
		//printf("压力是%dg\r\n",Weight_shiwu);
		printf("%dg\r\n",Weight_shiwu);
}

//////////////////////////////////
//300g的
/////////////////////////////////
void Hx711_Filter(int a,int b)
{
	unsigned long hx711_value,sum=0;
	unsigned long hx711_filter[16]={0};
	int Weight_shiwu=0;//存放实际重量
	int i;
	for(i=0;i<16;i++)
		{
			hx711_filter[i] = readcount();
			sum+=hx711_filter[i];
			//delay_ms(100);
		}
		hx711_value = sum/16;//均值滤波
		//hx711_value = ((hx711_value-8545300)/4293524)*1000;//8263850这个是初始值
		Weight_shiwu = ((hx711_value))/100;
		Weight_shiwu = (unsigned int)(((float)Weight_shiwu-a)/b+0.05);//0.05是四舍五入  368误差  
		printf("压力是%dg\r\n",Weight_shiwu);
}