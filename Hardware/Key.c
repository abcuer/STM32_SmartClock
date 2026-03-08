#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x.h"                  // Device header
#include "key.h"

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

uint8_t Key_GetNum(void)
{
    static uint8_t key_last[4] = {1,1,1,1};
    static uint16_t key_cnt[4] = {0};
    uint8_t key_now[4];
    uint8_t KeyNum = 0;
    
    key_now[0] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
    key_now[1] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    key_now[2] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
    key_now[3] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    
    for(uint8_t i=0; i<4; i++)
    {
        if(key_now[i] == 0) // 当前按下
        {
            key_cnt[i]++;
            // --- 关键改进：长按连发逻辑 ---
            if(key_cnt[i] >= LONG_PRESS_START)
            {
                // 使用取余操作实现周期性触发 KeyNum
                if((key_cnt[i] - LONG_PRESS_START) % LONG_PRESS_SPEED == 0)
                {
                    KeyNum = (i + 1) | 0x80; // 返回长按键码
                }
            }
        }
        else // 当前松开
        {
            // 只有在没达到长按标准时松开，才判定为短按一次
            if(key_cnt[i] > 2 && key_cnt[i] < LONG_PRESS_START) 
            {
                KeyNum = i + 1; 
            }
            key_cnt[i] = 0; // 清零计数
        }
        key_last[i] = key_now[i];
    }
    return KeyNum;
}
