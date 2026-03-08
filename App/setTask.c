#include "FreeRTOS.h"
#include "task.h"
#include "headfile.h"

void rtcTask(void *pvParameters)
{
	while(1)
	{
		RTC_ReadTime();   // 更新全局时间数组
		vTaskDelay(pdMS_TO_TICKS(1000)); // 每1秒读取一次
	}
}

void keyTask(void *pvParameters)
{
	while(1)
	{
		Key_Control();
		vTaskDelay(pdMS_TO_TICKS(20)); // 20ms扫描一次
	}
}

void oledTask(void *pvParameters)
{
	while(1)
	{
		OLED_Show();
		vTaskDelay(pdMS_TO_TICKS(200)); // 200ms刷新一次
	}
}

void UpdateAlarmTask(void *pvParameters)
{
	while(1)
	{
		Update_AlarmState();
		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

