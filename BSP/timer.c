#include "headfile.h"

uint32_t alarm_time_ms = 0; // 1分钟
uint32_t beep_timer_ms = 0;

void TIM2_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;       // 自动重装值（1ms中断）
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;      // 72MHz / 72 = 1MHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void Alarm_Start(void)
{
    alarm_time_ms = 60 * 1000; // 1分钟
    beep_timer_ms = 0;
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        if (alarm_time_ms)
        {
            alarm_time_ms--;

            if (beep_timer_ms > 0)
            {
                beep_timer_ms--;
            }
            else
            {
                static uint8_t beep_state = 0;
                beep_state = !beep_state;

                if (beep_state)
                {
                    Buzzer_ON();
					Red_ON();
                    beep_timer_ms = 200; // 响200ms
                }
                else
                {
                    Buzzer_OFF();
					Red_OFF();
                    beep_timer_ms = 300; // 间隔300ms
                }
            }

            // 超时后自动停止
            if (alarm_time_ms == 0)
            {
                Buzzer_OFF();
				Red_OFF();
            }
        }
    }
}
