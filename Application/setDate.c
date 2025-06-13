#include "headfile.h"

uint8_t Clock_Day = 0; // 当前月份对应的天数上限

/**
  * 函    数：DateSet
  * 功    能：通过按键设置当前日期（年、月、日）；
  *           设置完成后可切换到时间设置模式
  * 参    数：无
  * 返 回 值：无
  */
void DateSet(void)
{
	if(KeyNum == 1) // 1号按键：调整年份
	{
		MyRTC_Time[0]++;
		if(MyRTC_Time[0] > 2035) MyRTC_Time[0] = 2015; // 限制年份范围
		MyRTC_SetTime(); // 写入RTC
	}
	if(KeyNum == 2) // 2号按键：调整月份
	{
		MyRTC_Time[1]++;
		if(MyRTC_Time[1] > 12) MyRTC_Time[1] = 1;
		MyRTC_SetTime();
	}
	if(KeyNum == 3) // 3号按键：调整日期（根据月份和闰年情况限制）
	{
		MonthDaySet(); // 获取当前月份允许的最大天数
		MyRTC_Time[2]++;
		if(MyRTC_Time[2] > Clock_Day) MyRTC_Time[2] = 1;
		MyRTC_SetTime();
	}
	if(KeyNum == 4) // 4号按键：切换到时间设置模式
	{
		Mode = 2;
	}			
}

/**
  * 函    数：isLeepYear
  * 功    能：判断传入的年份是否为闰年
  * 参    数：year —— 需要判断的年份
  * 返 回 值：1 表示闰年，2 表示平年
  */
int8_t isLeepYear(int16_t year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		return 1;  // 闰年
	else
		return 2;  // 平年
}

/**
  * 函    数：MonthDaySet
  * 功    能：根据当前年份与月份设置该月的最大天数（保存至 Clock_Day）
  * 参    数：无（使用全局变量 MyRTC_Time）
  * 返 回 值：无（结果保存在 Clock_Day 中）
  */
void MonthDaySet(void)
{
	if (isLeepYear(MyRTC_Time[0]) == 1) // 闰年
	{
		switch (MyRTC_Time[1])
		{
			case 2:
				Clock_Day = 29;
				break;
			case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				Clock_Day = 31;
				break;
			default:
				Clock_Day = 30;
				break;
		}
	}
	else // 平年
	{
		switch (MyRTC_Time[1])
		{
			case 2:
				Clock_Day = 28;
				break;
			case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				Clock_Day = 31;
				break;
			default:
				Clock_Day = 30;
				break;
		}
	}
}
