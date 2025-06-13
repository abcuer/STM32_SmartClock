#ifndef _headfile_h
#define _headfile_h

#include "stm32f10x.h"                  // Device header
#include "Delay.h"						//延时函数
#include "MyRTC.h"						//RTC模块

#include "Led.h"
#include "Key.h"						//按键模块
#include "Buzzer.h"						//蜂鸣器模块
#include "OLED.h"						//OLED显示模块

#include "mode.h" 

extern uint16_t MyRTC_Time[];

#endif
