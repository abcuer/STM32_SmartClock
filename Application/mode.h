#ifndef _mode_h
#define _mode_h


void Key_Control(void);
void OLED_Show(void);

void AlarmSet(void);
void AlarmReact(void);
void DateSet(void);
void MonthDaySet(void);
void ClockSet(void);
void Update_AlarmState(void);

int8_t isLeepYear(int16_t year);

#endif
