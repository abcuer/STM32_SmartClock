#include "headfile.h"

uint8_t Mode = 0;							//按键调节闹钟/日期/时间，0为调节闹钟，1为调节日期，2为调节时间
uint8_t KeyNum = 0;

/*按键控制函数*/
void Key_Control(void)
{
	KeyNum = Key_GetNum();	//读取按键键码
	// 纭閿暱鎸夛紝 淇濆瓨鏁版嵁鍒癴lash
	if (KeyNum == 0x84) 
    {
        Flash_Save();    
        OLED_ShowString(4, 1, "  Data Saved!  "); 
        return;        
    }
	if (KeyNum > 0)
    {
        KeyNum = KeyNum & 0x7F; 
    }
	
	if(Mode == 0) 	  AlarmSet();			//调节闹钟
	else if(Mode == 1) DateSet();		    //调节日期
	else if(Mode == 2) ClockSet();		    //调节时间
}	

/*OLED显示函数*/
void OLED_Show(void)
{
	OLED_ShowNum(1, 6, RTC_Time[0], 4);		//显示RTC_Time数组中的时间值，年
	OLED_ShowNum(1, 11, RTC_Time[1], 2);		//月
	OLED_ShowNum(1, 14, RTC_Time[2], 2);		//日
	OLED_ShowNum(2, 7, RTC_Time[3], 2);		//时
	OLED_ShowNum(2, 10, RTC_Time[4], 2);		//分
	OLED_ShowNum(2, 13, RTC_Time[5], 2);		//秒
	
	if(AlarmCount)										//正在计时，则显示闹钟响起剩余时间
	{
		AlarmReact();
	}
	else												//不在计时，则显示需要设定的闹钟时间
	{
		OLED_ShowNum(3,7,AlarmHour,2);
		OLED_ShowNum(3,10,AlarmMin,2);
		OLED_ShowNum(3,13,AlarmSec,2);
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
