#include "headfile.h"

uint8_t AlarmMode = 0; //倒计时闹钟 || 定时闹钟
uint8_t AlarmCount = 0;								//是否在计时标志，0为不在计时
uint8_t Mode = 0;							//按键调节闹钟/日期/时间，0为调节闹钟，1为调节日期，2为调节时间
uint8_t Hour = 0, Min = 0, Sec = 0;							//用来调整闹钟时间的变量
uint32_t Alarm_CNT = 0, Alarm_Time = 0, Alarm_Time_Rest = 0;	//闹钟相关变量，单位都是秒
uint8_t KeyNum = 0;
uint8_t Clock_Day = 0;
/*
Mode
 0：闹钟
 1：日期
 2：时间
*/
/*按键控制函数*/
void Key_Control(void)
{
	KeyNum = Key_GetNum();	//读取按键键码
	
	if(Mode == 0) 	  AlarmSet();			//调节闹钟
	else if(Mode == 1) DateSet();		    //调节日期
	else if(Mode == 2) ClockSet();		    //调节时间
}	

void OLED_Show(void)
{
	OLED_ShowNum(1, 6, MyRTC_Time[0], 4);		//显示MyRTC_Time数组中的时间值，年
	OLED_ShowNum(1, 11, MyRTC_Time[1], 2);		//月
	OLED_ShowNum(1, 14, MyRTC_Time[2], 2);		//日
	OLED_ShowNum(2, 7, MyRTC_Time[3], 2);		//时
	OLED_ShowNum(2, 10, MyRTC_Time[4], 2);		//分
	OLED_ShowNum(2, 13, MyRTC_Time[5], 2);		//秒
	
	if(AlarmCount)										//正在计时，则显示闹钟响起剩余时间
	{
		AlarmReact();
	}
	else												//不在计时，则显示需要设定的闹钟时间
	{
		OLED_ShowNum(3,7,Hour,2);
		OLED_ShowNum(3,10,Min,2);
		OLED_ShowNum(3,13,Sec,2);
	}
	
	if(Mode == 1)		//显示“调节日期”
	{
		OLED_ShowString(4,1," Change Date ");
	}
	else if(Mode == 2)	//显示“调节时间”
	{
		OLED_ShowString(4,1," Change Clock");
	}
	else if(Mode == 0 && AlarmCount == 0)
	{
		OLED_ShowString(4,1," Change Alarm");
	}
}

void AlarmSet(void)
{
	if(KeyNum == 1)		//1号按键调整小时
	{
		Hour++;
		if(Hour > 23) Hour = 0;
	}
	else if(KeyNum == 2)	//2号按键调整分钟
	{
		Min++;
		if(Min > 59) Min = 0;
	}
	else if(KeyNum == 3)	//3号按键调整秒
	{
		Sec++;
		if(Sec > 59) Sec = 0;
	}

	if(KeyNum == 4)	//4号按键
	{
		if(Buzzer_State()==0)		//若蜂鸣器没响
		{
			 // 把设定的时间转换为当天的总秒数
			Alarm_Time = Hour * 3600 + Min * 60 + Sec;
			uint32_t current_seconds = MyRTC_Time[3] * 3600 + MyRTC_Time[4] * 60 + MyRTC_Time[5];

			// 计算距离下一次闹钟触发还有多少秒
			int32_t delta_seconds;
			if (Alarm_Time > current_seconds) {
				delta_seconds = Alarm_Time - current_seconds;
			} else {
				delta_seconds = (24 * 3600 - current_seconds) + Alarm_Time; // 次日
			}
			
			if(Alarm_Time > 0)
			{
				// 设置闹钟值
				Alarm_CNT = RTC_GetCounter() + delta_seconds;
				RTC_SetAlarm(Alarm_CNT); // 写入闹钟值到RTC的ALR寄存器
				AlarmCount = 1;
			}
			else	//若闹钟时长为0，则转到按键调节日期
			{
				Mode = 1;	
			}
		}
		else	//若蜂鸣器响
		{
			Hour = 0, Min = 0, Sec = 0;
			Buzzer_OFF();		//关闭蜂鸣器
			Red_OFF();
			OLED_ShowString(4,1,"        ");	//刷新oled第四行	
		}
	}
}

void AlarmReact(void)
{
    // 获取当前时间的当天总秒数
    uint32_t current_seconds = MyRTC_Time[3] * 3600 + MyRTC_Time[4] * 60 + MyRTC_Time[5];
    uint32_t alarm_seconds = Hour * 3600 + Min * 60 + Sec;

    // 计算剩余时间（单位：秒）
    int32_t delta_seconds;
    if (alarm_seconds > current_seconds) {
        delta_seconds = alarm_seconds - current_seconds;
    } else {
        delta_seconds = (24 * 3600 - current_seconds) + alarm_seconds; // 次日
    }

	uint8_t hours = delta_seconds / 3600;
	uint8_t minutes = (delta_seconds % 3600) / 60;
	uint8_t seconds = delta_seconds % 60;
    // 显示剩余时间
    OLED_ShowNum(3, 7, hours, 2);         // 小时
    OLED_ShowNum(3, 10, minutes, 2); // 分钟
    OLED_ShowNum(3, 13, seconds, 2); // 秒

    // 检查闹钟是否触发
    if (RTC_GetFlagStatus(RTC_FLAG_ALR) == SET)
    {
        static uint8_t alarm_triggered = 0;
        if (!alarm_triggered)
        {
            RTC_ClearFlag(RTC_FLAG_ALR); // 立即清除标志
            alarm_triggered = 1;
			// 显示并蜂鸣
            OLED_ShowString(4, 1, "   Time Out  ");
            Buzzer_ON();
			Red_ON();
            // 重置参数
            AlarmCount = 0;
            Alarm_Time = 0;
            Hour = 0;
            Min = 0;
            Sec = 0;
        }
    }
    else
    {
        OLED_ShowString(4, 1, "   Counting  ");
    }
}
//void AlarmReact(void)
//{
//	Alarm_Time_Rest = Alarm_CNT-RTC_GetCounter()+1;	//计算闹钟响起剩余时间
//	if(Alarm_Time_Rest > Alarm_Time)				//防止溢出错误
//		Alarm_Time_Rest = 0;
//	
//	OLED_ShowNum(3,7,Alarm_Time_Rest/3600,2);		//显示剩余小时
//	OLED_ShowNum(3,10,(Alarm_Time_Rest%3600)/60,2);	//显示剩余分钟
//	OLED_ShowNum(3,13,(Alarm_Time_Rest%3600)%60,2);	//显示剩余秒
//	
//	if(RTC_GetFlagStatus(RTC_FLAG_ALR) == 1)		//闹钟时间到，检查标志位为1
//	{
//		RTC_ClearFlag(RTC_FLAG_ALR);				//清除标志位
//		AlarmCount = 0;Alarm_Time = 0;				//重置相关参数
//		Hour = 0;Min = 0; Sec = 0;
//		
//		Buzzer_ON();								//打开蜂鸣器
//		OLED_ShowString(4,1,"   Time Out  ");
//		Delay_ms(100);
//		Buzzer_OFF();
//	}
//	else											//闹钟时间未到
//	{
//		OLED_ShowString(4,1,"   Counting  ");			//显示正在计时
//	}
//}

void DateSet(void)
{
	if(KeyNum == 1)			//1号按键调整年
	{
		MyRTC_Time[0]++;
		if(MyRTC_Time[0] > 2030) MyRTC_Time[0] = 2020;
		MyRTC_SetTime();
	}
	if(KeyNum == 2)	//2号按键调整月
	{
		MyRTC_Time[1]++;
		if(MyRTC_Time[1] > 12) MyRTC_Time[1] = 1;
		MyRTC_SetTime();
	}
	if(KeyNum == 3)	//3号按键调整日
	{
		MonthDaySet();
		MyRTC_Time[2]++;
		if(MyRTC_Time[2] > Clock_Day) MyRTC_Time[2] = 1;
		MyRTC_SetTime();
	}
	if(KeyNum == 4)	//4号按键，改为调整时间
	{
		Mode = 2;
	}			
}

void ClockSet(void)
{
	if(KeyNum == 1)			//1号按键调整小时
	{
		MyRTC_Time[3]++;
		MyRTC_SetTime();
	}
	else if(KeyNum == 2)	//2号按键调整分钟
	{
		MyRTC_Time[4]++;
		MyRTC_SetTime();
	}
	else if(KeyNum == 3)	//3号按键调整秒
	{
		MyRTC_Time[5]++;
		MyRTC_SetTime();
	}
	else if(KeyNum == 4)	//4号按键
	{
		Mode = 0;	//改为调整闹钟
		OLED_ShowString(4,1,"            ");	//刷新oled第四行
	}			
}

int8_t isLeepYear(int16_t year)
{
	if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 	return 1;  // 闰年
    else return 2; // 平年
}

void MonthDaySet(void)
{
	if(isLeepYear(MyRTC_Time[0])) // 闰年
	{
		switch(MyRTC_Time[1]) 	// 检查月份
		{
			case 2:
				Clock_Day = 29;
				break;
			case 1: 
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				Clock_Day = 31;
				break;
			default:
				Clock_Day = 30;
				break;
		}
	}
	else if(isLeepYear(MyRTC_Time[0]) == 2)
	{
		switch(MyRTC_Time[1]) 	// 检查月份
		{
			case 2:
				Clock_Day = 28;
				break;
			case 1: 
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				Clock_Day = 31;
				break;
			default:
				Clock_Day = 30;
				break;
		}
	}		
}
