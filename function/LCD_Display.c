/*
 * LCD_Display.c
 *
 *  Created on: 2021年5月28日
 *      Author: 孙昊阳
 */

#include <MSP430G2553.h>
#include <stdint.h>
#include "TCA6416A.h"
#include "LCD_128.h"
#include "DAC8411.h"

#define myCLK 32768

//uint32_t currenttime = 40500;      //设置电子时钟重置默认时间11:15:00
//unsigned int charbuff[] = {1,1,1,1,1,1};  //LCD显示屏显示的时间，分别为时，分，秒的个位和十位
unsigned char flag = 0;
//定义全局变量年、月、日、时、分、秒
uint32_t year = 2021;
unsigned char month = 5;
unsigned char day = 28;
unsigned char max_day_num = 30;
unsigned char hour = 11;
unsigned char minute = 15;
unsigned char second = 0;
//闹钟时分
unsigned char alarm_hour = 0;
unsigned char alarm_minute = 0;
//闹钟开关标志位，0代表闹钟关闭
unsigned char alarm_flag = 0;

/****************************************************************************
  * 名    称：Get_Max_DayNum()
  * 功    能：返回每个月份的最大天数
  * 入口参数：无
  * 出口参数：无
  * 说    明: 初始化定时器，定义系统时钟频率，定时周期等
  ****************************************************************************/
void Get_Max_DayNum()
{
    switch(month)
    {
    case 1:max_day_num = 31;break;
    case 2:max_day_num = 28;break;
    case 3:max_day_num = 31;break;
    case 4:max_day_num = 30;break;
    case 5:max_day_num = 31;break;
    case 6:max_day_num = 30;break;
    case 7:max_day_num = 31;break;
    case 8:max_day_num = 31;break;
    case 9:max_day_num = 30;break;
    case 10:max_day_num = 31;break;
    case 11:max_day_num = 30;break;
    case 12:max_day_num = 31;break;
    default:break;
    }
}

/****************************************************************************
  * 名    称：TimerA_Init()
  * 功    能：初始化定时器timerA
  * 入口参数：无
  * 出口参数：无
  * 说    明: 初始化定时器，定义系统时钟频率，定时周期等
  ****************************************************************************/
void TimerA_Init()
{
    TA0CCR0 = myCLK;       //定义中断计数周期1s
    TACCTL0 = CCIE;         //TA0CCR0捕获/比较中断计时器中断使能
    TA0CTL = 0;                //清除以前设置；
    TA0CTL = TASSEL_1 + MC_1 + TACLR;         //ACLK时钟源，增计数模式，计数值复位；
}

/****************************************************************************
  * 名    称：Time_A(void)
  * 功    能：定时器timerA中断服务函数
  * 入口参数：无
  * 出口参数：无
  * 说    明: 每次中断时更新year，month，day，hour，minute，second
  *             等信息；
  ****************************************************************************/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Time_A(void)
{
    if(flag==1)
        {
            flag = 0;
            second += 1;
            if(second>=60)
            {
                second = 0;
                minute += 1;
                if(minute>=60)
                {
                    minute = 0;
                    hour += 1;
                    if(hour>=24)
                    {
                        hour = 0;
                        day += 1;
                        if(day>=max_day_num)
                        {
                            day = 1;
                            month += 1;
                            if(month>=13)
                            {
                                month = 1;
                                year += 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            flag = 1;
            second += 1;
            if(second>=60)
            {
                second = 0;
                minute += 1;
                if(minute>=60)
                {
                    minute = 0;
                    hour += 1;
                    if(hour>=24)
                    {
                        hour = 0;
                        day += 1;
                        if(day>=max_day_num)
                        {
                            day = 1;
                            month += 1;
                            if(month>=13)
                            {
                                month = 1;
                                year += 1;
                            }
                        }
                    }
                }
            }
        }
}

/****************************************************************************
  * 名    称：Display_Time()
  * 功    能：在LCD显示屏的1-6位数码管上显示当前时间
  * 入口参数：无
  * 出口参数：无
  * 说    明: 无
  ****************************************************************************/
void Display_Time()
{
    LCD_Clear();
    //PinOUT(0,flag);
    LCD_DisplayDigit(second%10,6);

    LCD_DisplayDigit(second/10,5);
    LCD_DisplaySeg(_LCD_COLON1);//显示冒号

    LCD_DisplayDigit(minute%10,4);

    LCD_DisplayDigit(minute/10,3);
    LCD_DisplaySeg(_LCD_COLON0);//显示冒号

    LCD_DisplayDigit(hour%10,2);

    LCD_DisplayDigit(hour/10,1);
    if(hour>=12)
    {
        LCD_ClearSeg(_LCD_AM);
        LCD_DisplaySeg(_LCD_PM);
    }
    else
    {
        LCD_ClearSeg(_LCD_PM);
        LCD_DisplaySeg(_LCD_AM);
    }
    LCD_DisplayDigit(day%10,7);
    LCD_DisplayDigit(day/10,8);
    LCD_DisplaySeg(_LCD_DOT6);
    LCD_DisplayDigit(month%10,9);
    LCD_DisplayDigit(month/10,10);
    if(alarm_flag==1)
    {
        LCD_DisplaySeg(_LCD_BELL);
    }
    else
    {
        LCD_ClearSeg(_LCD_BELL);
    }
}

void Display_Date()
{
    LCD_Clear();
    LCD_DisplayDigit(year/1000,10);
    LCD_DisplayDigit(year/100%10,9);
    LCD_DisplayDigit(year%1000/10,8);
    LCD_DisplayDigit(year%1000%10,7);
    LCD_DisplayDigit(day%10,6);
    LCD_DisplayDigit(day/10,5);
    LCD_DisplaySeg(_LCD_DOT3);
    LCD_DisplayDigit(month%10,4);
    LCD_DisplayDigit(month/10,3);
    if(alarm_flag==1)
    {
        LCD_DisplaySeg(_LCD_BELL);
    }
    else
    {
        LCD_ClearSeg(_LCD_BELL);
    }
}

/****************************************************************************
  * 名    称：Display_Alarm()
  * 功    能：在LCD显示屏的闹钟事件
  * 入口参数：无
  * 出口参数：无
  * 说    明: 无
  ****************************************************************************/
void Display_Alarm()
{
    LCD_Clear();
    LCD_DisplaySeg(_LCD_BELL);
    LCD_DisplayDigit(alarm_minute%10,6);
    LCD_DisplayDigit(alarm_minute/10,5);
    LCD_DisplaySeg(_LCD_COLON1);
    LCD_DisplayDigit(alarm_hour%10,4);
    LCD_DisplayDigit(alarm_hour/10,3);
}

//闹钟数字更改
void Alarm_Minute_Add()
{
    alarm_minute += 1;
    if(alarm_minute>=60)
    {
        alarm_minute = 0;
    }
}
void Alarm_Minute_Sub()
{
    alarm_minute -= 1;
    if(alarm_minute<=1)
    {
        alarm_minute = 59;
    }
}
void Alarm_Hour_Add()
{
    alarm_hour += 1;
    if(alarm_hour>23)
    {
        alarm_hour = 0;
    }
}
void Alarm_Hour_Sub()
{
    alarm_hour -= 1;
    if(alarm_hour<=1)
    {
        alarm_hour = 23;
    }
}

//数字更改
void Minute_Add()
{
    minute += 1;
    if(minute>=60)
    {
        minute = 0;
    }
}

void Hour_Add()
{
    hour += 1;
    if(hour>23)
    {
        hour = 0;
    }
}

void Day_Add()
{
    day += 1;
    Get_Max_DayNum();
    if(day>max_day_num)
    {
        day = 1;
    }
}

void Month_Add()
{
    month += 1;
    if(month>12)
    {
        month = 1;
        year += 1;
    }
}


//数字更改
void Minute_Sub()
{
    minute -= 1;
    if(minute<1)
    {
        minute = 59;
    }
}

void Hour_Sub()
{
    hour -= 1;
    if(hour<1)
    {
        hour = 23;
    }
}

void Day_Sub()
{
    day -= 1;
    if(day<1)
    {
        day =  30;
    }
}

void Month_Sub()
{
    month -= 1;
    if(month<1)
    {
        month = 12;
        year -= 1;
    }
}

/****************************************************************************
  * 名    称：Hourly_Chime()
  * 功    能：整点报时功能
  * 入口参数：无
  * 出口参数：无
  * 说    明: 当电子时钟时间处于整点的5s内LED闪烁，同时蜂鸣器报时
  ****************************************************************************/
void Hourly_Chime()
{
    if(minute==0&&second<=5)
    {
        PinOUT(2,flag);
        //下面是蜂鸣器报时函数....
        Open_Beep();
    }
    else
    {
        PinOUT(2,1);
        //下面是关闭蜂鸣器函数....
        Close_Beep();
    }
}

/****************************************************************************
  * 名    称：Alarm()
  * 功    能：闹钟功能
  * 入口参数：无
  * 出口参数：无
  * 说    明: 当电子时钟时间处于设置的闹钟时间的20s内，LED闪烁同时
  *             蜂鸣器报时
  ****************************************************************************/
void Alarm()
{
    /*if(alarm_hour!=0||alarm_minute!=0)
    {
        alarm_flag = 1;
        LCD_DisplaySeg(_LCD_BELL);
        PinOUT(3,0);
    }
    else
    {
        alarm_flag =0;
        LCD_ClearSeg(_LCD_BELL);
        PinOUT(3,1);
    }*/
    switch(alarm_flag)
    {
    case 0:break;
    case 1:
        if(hour==alarm_hour&&minute==alarm_minute&&second<=20)
        {
            PinOUT(1,flag);
            //下面是蜂鸣器报时函数....
            Open_Beep();
        }
        else
        {
            PinOUT(1,1);
            //下面是关闭蜂鸣器函数....
            Close_Beep();
        }
        break;
    default:break;
    }
}
//闹钟功能开启函数
void Open_Alarm()
{
    LCD_DisplaySeg(_LCD_BELL);
    PinOUT(3,0);
    alarm_flag = 1;
}
//闹钟功能关闭函数
void Close_Alarm()
{
    LCD_ClearSeg(_LCD_BELL);
    PinOUT(3,1);
    alarm_flag = 0;
}
