/*
 * LCD_Display.h
 *
 *  Created on: 2021��5��28��
 *      Author: �����
 */

#ifndef FUNCTION_LCD_DISPLAY_H_
#define FUNCTION_LCD_DISPLAY_H_

extern void TimerA_Init();
extern void Display_Time();
extern void Display_Date();

//�����ա�ʱ�����ּӼ�
extern void Minute_Add();
extern void Hour_Add();
extern void Day_Add();
extern void Month_Add();
extern void Minute_Sub();
extern void Hour_Sub();
extern void Day_Sub();
extern void Month_Sub();

//������ʾ�����ּӼ�
extern void Display_Alarm();
extern void Alarm_Minute_Sub();
extern void Alarm_Minute_Add();
extern void Alarm_Hour_Add();
extern void Alarm_Hour_Sub();

//���㱨ʱ����
extern void Hourly_Chime();
//���Ӻ���
extern void Alarm();
extern void Open_Alarm();
extern void Close_Alarm();

#endif /* FUNCTION_LCD_DISPLAY_H_ */
