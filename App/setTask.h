#ifndef _setTask_h
#define _setTask_h

void rtcTask(void *pvParameters);
void keyTask(void *pvParameters);
void oledTask(void *pvParameters);
void UpdateAlarmTask(void *pvParameters);

#endif
