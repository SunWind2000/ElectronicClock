/*
 * Sec_Clock.c
 *
 *  Created on: 2021年5月30日
 *      Author: 孙昊阳
 */
#include <MSP430G2553.h>
#include "LCD_128.h"
#include "TCA6416A.h"

unsigned char ms_num = 0;
unsigned char sec_num = 0;
unsigned char min_num = 0;
unsigned char cur_ms_num = 0;
unsigned char cur_sec_num = 0;
unsigned char cur_min_num = 0;
unsigned char sec_flag = 0;
unsigned char run_flag = 0;

/****************************************************************************
  * 名    称：Sec_TimerA_Init()
  * 功    能：初始化定时器timerA
  * 入口参数：无
  * 出口参数：无
  * 说    明: 初始化定时器，定义系统时钟频率，定时周期等
  ****************************************************************************/
void Sec_TimerA_Init()
{
    TA1CCR0 = 400;       //定义中断计数周期1ms
    TA1CCTL0 = CCIE;         //TA0CCR1捕获/比较中断计时器中断使能
    TA1CTL = 0;                //清除以前设置；
    TA1CTL = TASSEL_1 + MC_0 + TACLR;         //ACLK时钟源，停止计数，计数值复位；
}

/****************************************************************************
  * 名    称：Sec_TimeA_ISR(void)
  * 功    能：定时器timerA中断服务函数
  * 入口参数：无
  * 出口参数：无
  * 说    明: 秒表定时器中断服务函数
  ****************************************************************************/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Sec_TimeA_ISR(void)
{
    if(sec_flag==1)
    {
        sec_flag = 0;
        ms_num += 1;
        if(ms_num >= 99)
        {
            ms_num = 0;
            sec_num += 1;
            if(sec_num>=60)
            {
                sec_num = 0;
                min_num += 1;
                if(min_num>=60)
                {
                    min_num = 0;
                }
            }
        }
    }
    else if(sec_flag==0)
    {
        sec_flag = 1;
        ms_num += 1;
        if(ms_num >= 99)
        {
            ms_num = 0;
            sec_num += 1;
            if(sec_num>=60)
            {
                sec_num = 0;
                min_num += 1;
                if(min_num>=60)
                {
                    min_num = 0;
                }
            }
        }
    }
    else
    {
        sec_flag = 2;
    }
}

/****************************************************************************
  * 名    称：Display_Sec_Clock()
  * 功    能：在LCD显示屏的1-6位数码管上显示秒表时间
  * 入口参数：无
  * 出口参数：无
  * 说    明: 无
  ****************************************************************************/
void Display_Sec_Clock()
{
    LCD_Clear();
    LCD_DisplayDigit(ms_num%10,6);
    LCD_DisplayDigit(ms_num/10,5);
    LCD_DisplaySeg(_LCD_COLON1);//显示冒号
    LCD_DisplayDigit(sec_num%10,4);
    LCD_DisplayDigit(sec_num/10,3);
    LCD_DisplaySeg(_LCD_COLON0);//显示冒号
    LCD_DisplayDigit(min_num%10,2);
    LCD_DisplayDigit(min_num/10,1);
    switch(run_flag)
    {
    case 0:
        LCD_ClearSeg(_LCD_RUN);
        LCD_DisplaySeg(_LCD_PAUSE);
        break;
    case 1:
        LCD_DisplaySeg(_LCD_RUN);
        LCD_ClearSeg(_LCD_PAUSE);
        break;
    default:break;
    }
}

/****************************************************************************
  * 名    称：Refresh_Sec_Clock()
  * 功    能：重置秒表时间
  * 入口参数：无
  * 出口参数：无
  * 说    明: 无
  ****************************************************************************/
void Refresh_Sec_Clock()
{
    ms_num = 0;
    sec_num = 0;
    min_num = 0;
    sec_flag = 0;
    run_flag = 0;
}

/****************************************************************************
  * 名    称：Open_Sec_Clock()
  * 功    能：秒表计时开始
  * 入口参数：无
  * 出口参数：无
  * 说    明: 无
  ****************************************************************************/
void Start_Sec_Clock()
{
    TA1CTL = 0;
    TA1CTL = TASSEL_1 + MC_1 + TACLR;         //ACLK时钟源，增计数模式，计数值复位；
    run_flag = 1;
}

/****************************************************************************
  * 名    称：Close_Sec_Clock()
  * 功    能：秒表计时结束
  * 入口参数：无
  * 出口参数：无
  * 说    明: 无
  ****************************************************************************/
void Stop_Sec_Clock()
{
    TA1CTL = 0;
    TA1CTL = TASSEL_1 + MC_0 + TACLR;         //ACLK时钟源，停止计数，计数值复位；
    run_flag = 0;
}

