#include "headfile.h"

/**
  * 函    数：ClockSet
  * 功    能：通过按键调整当前系统时间（小时、分钟、秒）；
  *           设置完后可切换到闹钟设置模式
  * 参    数：无
  * 返 回 值：无
  */
void ClockSet(void)
{
	if(KeyNum == 1) // 1号按键：调整小时
	{
		RTC_Time[3]++;
		RTC_SetTime(); // 更新RTC时间
	}
	else if(KeyNum == 2) // 2号按键：调整分钟
	{
		RTC_Time[4]++;
		RTC_SetTime();
	}
	else if(KeyNum == 3) // 3号按键：调整秒
	{
		RTC_Time[5]++;
		RTC_SetTime();
	}
	else if(KeyNum == 4) // 4号按键：切换到闹钟设置模式
	{
		Mode = 0;
		OLED_ShowString(4, 1, "                "); // 清除OLED提示
	}			
}

