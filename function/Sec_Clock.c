/*
 * Sec_Clock.c
 *
 *  Created on: 2021��5��30��
 *      Author: �����
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
  * ��    �ƣ�Sec_TimerA_Init()
  * ��    �ܣ���ʼ����ʱ��timerA
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��ʼ����ʱ��������ϵͳʱ��Ƶ�ʣ���ʱ���ڵ�
  ****************************************************************************/
void Sec_TimerA_Init()
{
    TA1CCR0 = 400;       //�����жϼ�������1ms
    TA1CCTL0 = CCIE;         //TA0CCR1����/�Ƚ��жϼ�ʱ���ж�ʹ��
    TA1CTL = 0;                //�����ǰ���ã�
    TA1CTL = TASSEL_1 + MC_0 + TACLR;         //ACLKʱ��Դ��ֹͣ����������ֵ��λ��
}

/****************************************************************************
  * ��    �ƣ�Sec_TimeA_ISR(void)
  * ��    �ܣ���ʱ��timerA�жϷ�����
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ���ʱ���жϷ�����
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
  * ��    �ƣ�Display_Sec_Clock()
  * ��    �ܣ���LCD��ʾ����1-6λ���������ʾ���ʱ��
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��
  ****************************************************************************/
void Display_Sec_Clock()
{
    LCD_Clear();
    LCD_DisplayDigit(ms_num%10,6);
    LCD_DisplayDigit(ms_num/10,5);
    LCD_DisplaySeg(_LCD_COLON1);//��ʾð��
    LCD_DisplayDigit(sec_num%10,4);
    LCD_DisplayDigit(sec_num/10,3);
    LCD_DisplaySeg(_LCD_COLON0);//��ʾð��
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
  * ��    �ƣ�Refresh_Sec_Clock()
  * ��    �ܣ��������ʱ��
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��
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
  * ��    �ƣ�Open_Sec_Clock()
  * ��    �ܣ�����ʱ��ʼ
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��
  ****************************************************************************/
void Start_Sec_Clock()
{
    TA1CTL = 0;
    TA1CTL = TASSEL_1 + MC_1 + TACLR;         //ACLKʱ��Դ��������ģʽ������ֵ��λ��
    run_flag = 1;
}

/****************************************************************************
  * ��    �ƣ�Close_Sec_Clock()
  * ��    �ܣ�����ʱ����
  * ��ڲ�������
  * ���ڲ�������
  * ˵    ��: ��
  ****************************************************************************/
void Stop_Sec_Clock()
{
    TA1CTL = 0;
    TA1CTL = TASSEL_1 + MC_0 + TACLR;         //ACLKʱ��Դ��ֹͣ����������ֵ��λ��
    run_flag = 0;
}

