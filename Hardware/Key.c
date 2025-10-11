#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //开启GPIOA端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           //引脚设置为输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;  //PA5 PA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//信号传输速度50MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                //端口初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //开启GPIOB端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;  //PB1 PB11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
}

#define LONG_PRESS_TIME  15   // 每20ms调用一次，大约1秒长按

uint8_t Key_GetNum(void)
{
    static uint8_t key_last[4] = {1,1,1,1};
    static uint16_t key_cnt[4] = {0};
    uint8_t key_now[4];
    uint8_t KeyNum = 0;

    key_now[0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    key_now[1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
    key_now[2] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    key_now[3] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);

    for(uint8_t i=0; i<4; i++)
    {
        if(key_last[i] == 1 && key_now[i] == 0) // 按下瞬间
        {
            key_cnt[i] = 0;
        }
        else if(key_last[i] == 0 && key_now[i] == 0) // 一直按着
        {
            if(key_cnt[i] < LONG_PRESS_TIME)
                key_cnt[i]++;
            else if(key_cnt[i] == LONG_PRESS_TIME) // 到达长按阈值
            {
                KeyNum = (i + 1) | 0x80; // 高位标识长按
                key_cnt[i]++; // 防止重复触发
            }
        }
        else if(key_last[i] == 0 && key_now[i] == 1) // 松开
        {
            if(key_cnt[i] < LONG_PRESS_TIME)
                KeyNum = i + 1; // 短按事件
            key_cnt[i] = 0;
        }

        key_last[i] = key_now[i];
    }

    return KeyNum;
}


//uint8_t Key_GetNum(void)
//{
//	static uint8_t key_last[4] = {1, 1, 1, 1};
//	uint8_t KeyNum = 0;
//	uint8_t key_now[4];

//	key_now[0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
//	key_now[1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
//	key_now[2] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
//	key_now[3] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);

//	// 检测下降沿（按下瞬间）
//	for(uint8_t i = 0; i < 4; i++)
//	{
//		if (key_last[i] == 1 && key_now[i] == 0)
//		{
//			KeyNum = i + 1;	
//			break;			
//		}
//	}

//	for(uint8_t i = 0; i < 4; i++)
//	{
//		key_last[i] = key_now[i];
//	}

//	return KeyNum;
//}


/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~4，返回0代表没有按键按下
  * 注意事项：此函数1~3号按键有长按功能，当1~3号按键按住不放时，可以连续返回键码值
  */
//uint8_t Key_GetNum(void)
//{
//	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
//	uint32_t Temp = 600000;	//临时用的计时变量
//	
//	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键1按下
//	{
//		vTaskDelay(pdMS_TO_TICKS(20));											//延时消抖
//		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)	//等待按键松手，或长按连续返回值
//		{
//			Temp--;
//			if(Temp == 0)
//			{
//				vTaskDelay(pdMS_TO_TICKS(20));
//				return 1;
//			}
//		}
//		vTaskDelay(pdMS_TO_TICKS(20));											//延时消抖
//		KeyNum = 1;												//置键码为1
//	}
//	
//	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)			//读PB10输入寄存器的状态，如果为0，则代表按键2按下
//	{
//		vTaskDelay(pdMS_TO_TICKS(20));											//延时消抖
//		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)	//等待按键松手，或长按连续返回值
//		{
//			Temp--;
//			if(Temp == 0)
//			{
//				vTaskDelay(pdMS_TO_TICKS(20));
//				return 2;
//			}			
//		}
//		vTaskDelay(pdMS_TO_TICKS(20));											//延时消抖
//		KeyNum = 2;												//置键码为2
//	}
//	
//	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键2按下
//	{
//		vTaskDelay(pdMS_TO_TICKS(20));									//延时消抖
//		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)	//等待按键松手，或长按连续返回值
//		{
//			Temp--;
//			if(Temp == 0)
//			{
//				vTaskDelay(pdMS_TO_TICKS(20));
//				return 3;
//			}
//		}
//		vTaskDelay(pdMS_TO_TICKS(20));											//延时消抖
//		KeyNum = 3;												//置键码为3
//	}
//	
//	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)			//读PB0输入寄存器的状态，如果为0，则代表按键2按下
//	{
//		vTaskDelay(pdMS_TO_TICKS(20));										//延时消抖
//		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0);	//等待按键松手
//		vTaskDelay(pdMS_TO_TICKS(20));											//延时消抖
//		KeyNum = 4;												//置键码为4
//	}

//	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
//}
