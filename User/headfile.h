#ifndef _headfile_h
#define _headfile_h

#include "stm32f10x.h"                  // Device header
#include "delay.h"						//延时函数
#include "myrtc.h"						//RTC模块

#include "led.h"
#include "key.h"						//按键模块
#include "buzzer.h"						//蜂鸣器模块
#include "OLED.h"						//OLED显示模块

#include "mode.h"
#include "setAlarm.h"
#include "setClock.h"
#include "setDate.h"

extern uint16_t MyRTC_Time[];
extern uint8_t Mode;
extern uint8_t KeyNum;

extern uint8_t AlarmHour;
extern uint8_t AlarmMin;
extern uint8_t AlarmSec;
extern uint8_t AlarmCount;

#endif
