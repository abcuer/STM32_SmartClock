#include "FreeRTOS.h"
#include "task.h"
#include "headfile.h"

uint16_t MyRTC_Time[] = {2025,6,14,8,0,0};		//定义全局的时间数组，数组内容分别为年、月、日、时、分、秒

int main(void)
{
	/*模块初始化*/
	Led_Init();
	OLED_Init();		//OLED初始化
	MyRTC_Init();		//RTC初始化
	Key_Init();			//按键初始化
	Buzzer_Init();		//蜂鸣器初始化
		
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Date:XXXX/XX/XX");
	OLED_ShowString(2, 1, "Clock:XX:XX:XX");
	OLED_ShowString(3, 1, "Alarm:XX:XX:XX");
	
	xTaskCreate(rtcTask, "RTC", 128, NULL, 2, NULL);
	xTaskCreate(keyTask, "KEY", 128, NULL, 3, NULL);
	xTaskCreate(oledTask, "OLED", 128, NULL, 1, NULL);
	xTaskCreate(UpdateAlarmTask, "ALARM", 128, NULL, 2, NULL);
	
	vTaskStartScheduler();
	while(1)
	{
//		MyRTC_ReadTime();							//RTC读取时间，最新的时间存储到MyRTC_Time数组中
//		Key_Control();								//调用按键控制函数
//		OLED_Show();								//显示OLED内容
//		Update_AlarmState();						//检测并更新蜂鸣器状态
	}
}
