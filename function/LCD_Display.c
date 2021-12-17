/*
 * LCD_Display.c
 *
 *  Created on: 2021��5��28��
 *      Author: �����
 */

#include <MSP430G2553.h>
#include <stdint.h>
#include "TCA6416A.h"
#include "LCD_128.h"
#include "DAC8411.h"

#define myCLK 32768

//uint32_t currenttime = 40500;      //���õ���ʱ������Ĭ��ʱ��11:15:00
//unsigned int charbuff[] = {1,1,1,1,1,1};  //LCD��ʾ����ʾ��ʱ�䣬�ֱ�Ϊʱ���֣���ĸ�λ��ʮλ
unsigned char flag = 0;
//����ȫ�ֱ����ꡢ�¡��ա�ʱ���֡���
uint32_t year = 2021;
unsigned char month = 5;
unsigned char day = 28;
unsigned char max_day_num = 30;
unsigned char hour = 11;
unsigned char minute = 15;
unsigned char second = 0;
//����ʱ��
unsigned char alarm_hour = 0;
unsigned char alarm_minute = 0;
//���ӿ��ر�־λ��0�������ӹر�
unsigned char alarm_flag = 0;

/****************************************************************************
  * ��    �ƣ�Get_Max_DayNum()
  * ��    �ܣ�����ÿ���·ݵ��������
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��ʼ����ʱ��������ϵͳʱ��Ƶ�ʣ���ʱ���ڵ�
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
  * ��    �ƣ�TimerA_Init()
  * ��    �ܣ���ʼ����ʱ��timerA
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��ʼ����ʱ��������ϵͳʱ��Ƶ�ʣ���ʱ���ڵ�
  ****************************************************************************/
void TimerA_Init()
{
    TA0CCR0 = myCLK;       //�����жϼ�������1s
    TACCTL0 = CCIE;         //TA0CCR0����/�Ƚ��жϼ�ʱ���ж�ʹ��
    TA0CTL = 0;                //�����ǰ���ã�
    TA0CTL = TASSEL_1 + MC_1 + TACLR;         //ACLKʱ��Դ��������ģʽ������ֵ��λ��
}

/****************************************************************************
  * ��    �ƣ�Time_A(void)
  * ��    �ܣ���ʱ��timerA�жϷ�����
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ÿ���ж�ʱ����year��month��day��hour��minute��second
  *             ����Ϣ��
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
  * ��    �ƣ�Display_Time()
  * ��    �ܣ���LCD��ʾ����1-6λ���������ʾ��ǰʱ��
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��
  ****************************************************************************/
void Display_Time()
{
    LCD_Clear();
    //PinOUT(0,flag);
    LCD_DisplayDigit(second%10,6);

    LCD_DisplayDigit(second/10,5);
    LCD_DisplaySeg(_LCD_COLON1);//��ʾð��

    LCD_DisplayDigit(minute%10,4);

    LCD_DisplayDigit(minute/10,3);
    LCD_DisplaySeg(_LCD_COLON0);//��ʾð��

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
  * ��    �ƣ�Display_Alarm()
  * ��    �ܣ���LCD��ʾ���������¼�
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��
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

//�������ָ���
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

//���ָ���
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


//���ָ���
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
  * ��    �ƣ�Hourly_Chime()
  * ��    �ܣ����㱨ʱ����
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ������ʱ��ʱ�䴦�������5s��LED��˸��ͬʱ��������ʱ
  ****************************************************************************/
void Hourly_Chime()
{
    if(minute==0&&second<=5)
    {
        PinOUT(2,flag);
        //�����Ƿ�������ʱ����....
        Open_Beep();
    }
    else
    {
        PinOUT(2,1);
        //�����ǹرշ���������....
        Close_Beep();
    }
}

/****************************************************************************
  * ��    �ƣ�Alarm()
  * ��    �ܣ����ӹ���
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ������ʱ��ʱ�䴦�����õ�����ʱ���20s�ڣ�LED��˸ͬʱ
  *             ��������ʱ
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
            //�����Ƿ�������ʱ����....
            Open_Beep();
        }
        else
        {
            PinOUT(1,1);
            //�����ǹرշ���������....
            Close_Beep();
        }
        break;
    default:break;
    }
}
//���ӹ��ܿ�������
void Open_Alarm()
{
    LCD_DisplaySeg(_LCD_BELL);
    PinOUT(3,0);
    alarm_flag = 1;
}
//���ӹ��ܹرպ���
void Close_Alarm()
{
    LCD_ClearSeg(_LCD_BELL);
    PinOUT(3,1);
    alarm_flag = 0;
}
