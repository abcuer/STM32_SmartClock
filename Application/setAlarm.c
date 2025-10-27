#include "headfile.h"

uint8_t AlarmHour = 0, AlarmMin = 0, AlarmSec = 0;        // 用来调整闹钟时间的变量
uint8_t AlarmCount = 0;                                   // 是否在计时标志，0为不在计时
uint8_t alarm_triggered = 0;                              // 闹钟触发标志
uint32_t alarm_on_tick = 0;                               // 记录蜂鸣器开始响的时间（单位：秒）
uint32_t Alarm_CNT = 0, Alarm_Time = 0, Alarm_Time_Rest = 0; // 闹钟相关变量，单位都是秒

/**
  * 函    数：AlarmSet
  * 功    能：通过按键设置闹钟时间（小时、分钟、秒），并启用或关闭闹钟
  *           若蜂鸣器响起则关闭蜂鸣器并清空OLED提示；否则设置闹钟
  * 参    数：无
  * 返 回 值：无
  */
void AlarmSet(void)
{
	if(KeyNum == 1) // 1号按键：调整小时
	{
		AlarmHour++;
		if(AlarmHour > 23) AlarmHour = 0;
	}
	else if(KeyNum == 2) // 2号按键：调整分钟
	{
		AlarmMin++;
		if(AlarmMin > 59) AlarmMin = 0;
	}
	else if(KeyNum == 3) // 3号按键：调整秒
	{
		AlarmSec++;
		if(AlarmSec > 59) AlarmSec = 0;
	}

	if(KeyNum == 4) // 4号按键：确认并启动闹钟，或关闭蜂鸣器
	{
		if(Buzzer_State() == 0) // 若蜂鸣器未响，则设置闹钟
		{
			TimerAlarmSet();
		}
		else // 若蜂鸣器已响，则关闭并清除显示
		{
			AlarmHour = 0, AlarmMin = 0, AlarmSec = 0;
			Buzzer_OFF();
			Red_OFF();
			OLED_ShowString(4, 1, "                ");
		}
	}
}

/**
  * 函    数：TimerAlarmSet
  * 功    能：将用户设置的闹钟时间转换为秒并计算与当前时间的差值，
  *           然后设置RTC闹钟寄存器，实现闹钟计时功能
  * 参    数：无
  * 返 回 值：无
  */
void TimerAlarmSet(void)
{
	// 把设定的时间转换为当天的总秒数
	Alarm_Time = AlarmHour * 3600 + AlarmMin * 60 + AlarmSec;
	uint32_t current_seconds = MyRTC_Time[3] * 3600 + MyRTC_Time[4] * 60 + MyRTC_Time[5];

	// 计算距离下一次闹钟触发还有多少秒
	int32_t delta_seconds;
	if (Alarm_Time > current_seconds)
		delta_seconds = Alarm_Time - current_seconds;
	else
		delta_seconds = (24 * 3600 - current_seconds) + Alarm_Time; // 次日触发

	if(Alarm_Time > 0)
	{
		Alarm_CNT = RTC_GetCounter() + delta_seconds;
		RTC_SetAlarm(Alarm_CNT); // 设置RTC闹钟
		AlarmCount = 1;
	}
	else
	{
		Mode = 1; // 时间为0，转入日期设置模式
	}
}

/**
  * 函    数：AlarmReact
  * 功    能：实时显示距离闹钟触发的剩余时间；
  *           检测闹钟触发并执行响铃提示，清空设置
  * 参    数：无
  * 返 回 值：无
  */
void AlarmReact(void)
{
	uint32_t current_seconds = MyRTC_Time[3] * 3600 + MyRTC_Time[4] * 60 + MyRTC_Time[5];
	uint32_t alarm_seconds = AlarmHour * 3600 + AlarmMin * 60 + AlarmSec;

	int32_t delta_seconds;
	if (alarm_seconds > current_seconds)
		delta_seconds = alarm_seconds - current_seconds;
	else
		delta_seconds = (24 * 3600 - current_seconds) + alarm_seconds;

	// 显示剩余时间
	OLED_ShowNum(3, 7, delta_seconds / 3600, 2);         // 小时
	OLED_ShowNum(3, 10, (delta_seconds % 3600) / 60, 2); // 分钟
	OLED_ShowNum(3, 13, delta_seconds % 60, 2);          // 秒

	if (RTC_GetFlagStatus(RTC_FLAG_ALR) == SET)
	{
		if (!alarm_triggered)
		{
			RTC_ClearFlag(RTC_FLAG_ALR);
			alarm_triggered = 1;
			OLED_ShowString(4, 1, "    Time Out   ");
			// 启动闹钟
			Alarm_Start();
			alarm_on_tick = RTC_GetCounter();

			// 清除闹钟设置
			AlarmCount = 0;
			Alarm_Time = 0;
			AlarmHour = 0;
			AlarmMin = 0;
			AlarmSec = 0;
		}
	}
	else
	{
		OLED_ShowString(4, 1, "    Counting   ");
	}
}

/**
  * 函    数：Update_AlarmState
  * 功    能：判断蜂鸣器响铃是否超过设定时间（1秒），超时则关闭蜂鸣器及LED并清除OLED提示
  * 参    数：无
  * 返 回 值：无
  */
void Update_AlarmState(void)
{
	if (alarm_triggered)
	{
		if (RTC_GetCounter() - alarm_on_tick >= 1)
		{
			Buzzer_OFF();
			Red_OFF();
			OLED_ShowString(4, 1, "                "); // 清除提示
			alarm_triggered = 0;
		}
	}
}
