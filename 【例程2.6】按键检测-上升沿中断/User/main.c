/************************************************************************************** 
*   日期    ：2013年11月6日 (版权所有，翻版必究) 
*   目标    ：ARM Cortex-M0 LPC1114 
*   编译环境：KEIL 4.60
*   外部晶振：10Mhz(主频50M)
*   功能    ：学会使用GPIO引脚上升沿中断
*   作者    ：Ration
*   官方网址：http://www.rationmcu.com
*   淘宝店  ：http://ration.taobao.com
**************************************************************************************/
#include "LPC11XX.H"

#define LED1_ON  LPC_GPIO1->DATA &= ~(1<<0)
#define LED1_OFF LPC_GPIO1->DATA |= (1<<0)
#define LED2_ON  LPC_GPIO1->DATA &= ~(1<<1)
#define LED2_OFF LPC_GPIO1->DATA |= (1<<1)

#define KEY1_DOWN (LPC_GPIO1->DATA&(1<<9))!=(1<<9)
#define KEY1_UP   (LPC_GPIO1->DATA&(1<<9))==(1<<9)
#define KEY2_DOWN (LPC_GPIO1->DATA&(1<<10))!=(1<<10)
#define KEY2_UP   (LPC_GPIO1->DATA&(1<<10))==(1<<10)
/***********************************/
/* 函数名称：按键消抖延时          */
/***********************************/
void delay_xd()
{
	uint16_t i,j;
	
	for(i=0;i<50000;i++)
		for(j=0;j<10;j++);	
}
/***********************************/
/* 函数名称：延时                  */
/***********************************/
void delay()
{
	uint16_t i,j;
	
	for(i=0;i<50000;i++)
		for(j=0;j<80;j++);
}
/***********************************/
/* 函数名称：LED灯初始化           */
/***********************************/
void led_init()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // 使能IOCON时钟 
	LPC_IOCON->R_PIO1_0 &= ~0x07;    
	LPC_IOCON->R_PIO1_0 |= 0x01; //把P1.0脚设置为GPIO
	LPC_IOCON->R_PIO1_1 &= ~0x07;	
	LPC_IOCON->R_PIO1_1 |= 0x01; //把P1.1脚设置为GPIO
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // 禁能IOCON时钟
	
	LPC_GPIO1->DIR |= (1<<0);
	LPC_GPIO1->DATA |= (1<<0);
	LPC_GPIO1->DIR |= (1<<1);
	LPC_GPIO1->DATA |= (1<<1);
}
/***********************************/
/* 函数名称：GPIO1中断函数         */
/***********************************/
void PIOINT1_IRQHandler()
{
		if((LPC_GPIO1->MIS&(1<<9))==(1<<9))
		{
			delay_xd();
			if(KEY1_UP)
			{
				LED1_ON;
				delay();
				LED1_OFF;
			}
			LPC_GPIO1->IC = 0XFFF;  // 清RIS状态位
		}
		if((LPC_GPIO1->MIS&(1<<10))==(1<<10))
		{
			delay_xd();
			if(KEY2_UP)
			{
				LED2_ON;
				delay();
				LED2_OFF;
			}
			LPC_GPIO1->IC = 0XFFF;  // 清RIS状态位
		}
}
/***********************************/
/* 函数名称：主函数                */
/***********************************/
int main()
{
	led_init();
	LPC_GPIO1->IEV |= (1<<9); // 设置P1.9上升沿触发
	LPC_GPIO1->IEV |= (1<<10); // 设置P1.10上升沿触发
	LPC_GPIO1->IE |= (1<<9); //
	LPC_GPIO1->IE |= (1<<10);
	NVIC_EnableIRQ(EINT1_IRQn);
	
	while(1)
	{
		;
	}
}

