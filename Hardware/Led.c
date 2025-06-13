#include "headfile.h"

void Led_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOA,GPIOB的时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将引脚初始化为上拉输入
	
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void Red_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void Red_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}
