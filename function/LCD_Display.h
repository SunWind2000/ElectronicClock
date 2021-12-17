/*
 * LCD_Display.h
 *
 *  Created on: 2021年5月28日
 *      Author: 孙昊阳
 */

#ifndef FUNCTION_LCD_DISPLAY_H_
#define FUNCTION_LCD_DISPLAY_H_

extern void TimerA_Init();
extern void Display_Time();
extern void Display_Date();

//年月日、时分数字加减
extern void Minute_Add();
extern void Hour_Add();
extern void Day_Add();
extern void Month_Add();
extern void Minute_Sub();
extern void Hour_Sub();
extern void Day_Sub();
extern void Month_Sub();

//闹钟显示及数字加减
extern void Display_Alarm();
extern void Alarm_Minute_Sub();
extern void Alarm_Minute_Add();
extern void Alarm_Hour_Add();
extern void Alarm_Hour_Sub();

//整点报时函数
extern void Hourly_Chime();
//闹钟函数
extern void Alarm();
extern void Open_Alarm();
extern void Close_Alarm();

#endif /* FUNCTION_LCD_DISPLAY_H_ */
