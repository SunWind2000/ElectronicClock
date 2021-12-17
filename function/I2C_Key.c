/*
 * I2C_Key.c
 *
 *  Created on: 2021��5��29��
 *      Author: �����
 */

#include <MSP430G2553.h>
#include "TCA6416A.h"
#include "HT1621.h"
#include "LCD_Display.h"
#include "Sec_Clock.h"

/********WDT��ʼ������**********/
void WDT_init()
{
    //-----�趨WDTΪ-----------
    WDTCTL=WDT_ADLY_16;
    //-----WDT�ж�ʹ��----------------------
    IE1 |= WDTIE;
}
/***********WDT��ʱ�жϺ���*************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    __bic_SR_register_on_exit(LPM0_bits );
}

/**********I2C��չIO�ж��¼�������***************/
/******************************************************************************************************
 * ��       �ƣ�I2C_IO10_Onclick()
 * ��       �ܣ��趨����KEY1��
 * ��ڲ�����IO10_Onclick_Num����������
 * ���ڲ�������
 * ˵       ��������KEY1����ʾ����ѡ����Ҫ�趨�Ĳ�������ֵ��Ƶ�ʡ���ֱλ�ƣ�
 ******************************************************************************************************/
void I2C_IO11_Onclick(unsigned int IO11_Onclick_Num)
{
    _enable_interrupts(); //�������ж�
    switch(IO11_Onclick_Num)
    {
    case 1:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,0);//���������趨�·�
        PinOUT(5,1);
        PinOUT(6,1);
        PinOUT(7,1);
        break;
    case 2:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,1);
        PinOUT(5,0);//���������趨����
        PinOUT(6,1);
        PinOUT(7,1);
        break;
    case 3:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,1);
        PinOUT(5,1);
        PinOUT(6,0);//���������趨Сʱ
        PinOUT(7,1);
        break;
    case 4:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,1);
        PinOUT(5,1);
        PinOUT(6,1);
        PinOUT(7,0);//���������趨����
        break;
    default:
        Display_Time();
        PinOUT(4,1);
        PinOUT(5,1);
        PinOUT(6,1);
        PinOUT(7,1);
        break;
    }
}

void I2C_IO12_Onclick(unsigned int IO11_Onclick_Num,unsigned int  IO10_Onclick_Num)
{
    if(IO10_Onclick_Num<2)
    {
        switch(IO11_Onclick_Num)
        {
        case 1:Month_Add();break;
        case 2:Day_Add();break;
        case 3:Hour_Add();break;
        case 4:Minute_Add();break;
        default:break;
        }
    }
    if(IO10_Onclick_Num==2)
    {
        switch(IO11_Onclick_Num)
        {
        case 0:Start_Sec_Clock();PinOUT(0,0);break;
        default:break;
        }
    }
    if(IO10_Onclick_Num==3)
    {
        switch(IO11_Onclick_Num)
        {
        case 0:Open_Alarm();break;
        case 3:Alarm_Hour_Add();break;
        case 4:Alarm_Minute_Add();break;
        default:break;
        }
    }
}

void I2C_IO13_Onclick(unsigned int IO11_Onclick_Num,unsigned int  IO10_Onclick_Num)
{
    if(IO10_Onclick_Num<2)
    {
        switch(IO11_Onclick_Num)
        {
        case 1:Month_Sub();break;
        case 2:Day_Sub();break;
        case 3:Hour_Sub();break;
        case 4:Minute_Sub();break;
        default:break;
        }
    }
    if(IO10_Onclick_Num==2)
        {
            switch(IO11_Onclick_Num)
            {
            case 0:Stop_Sec_Clock();PinOUT(0,1);break;
            default:break;
            }
        }
    if(IO10_Onclick_Num==3)
    {
        switch(IO11_Onclick_Num)
        {
        case 0:Close_Alarm();break;
        case 3:Alarm_Hour_Sub();break;
        case 4:Alarm_Minute_Sub();break;
        default:break;
        }
    }
}

unsigned int  I2C_IODect()                            //����¼�ȷʵ������
{
    static unsigned char KEY_Past=0,KEY_Now=0;
    static unsigned int  IO10_Onclick_Num = 0;
    static unsigned char IO11_Onclick_Num = 0;
    if(IO10_Onclick_Num>3)
    {
        IO10_Onclick_Num = 0;
    }
    if(IO11_Onclick_Num>=5)
    {
        IO11_Onclick_Num = 0;
    }
    KEY_Past=KEY_Now;
    //----�ж�I2C_IO10������KEY1�����Ƿ񱻰���------
    if((TCA6416A_InputBuffer&BIT8) == BIT8)
        KEY_Now |=BIT0;
    else
        KEY_Now &=~BIT0;
    if(((KEY_Past&BIT0)==BIT0)&&(KEY_Now&BIT0) !=BIT0)
    {
        IO10_Onclick_Num += 1;
//        I2C_IO10_Onclick(IO10_Onclick_Num);
    }
    //----�ж�I2C_IO11������KEY2�����Ƿ񱻰���------
    if((TCA6416A_InputBuffer&BIT9)== BIT9)
        KEY_Now |=BIT1;
    else
        KEY_Now &=~BIT1;
    if(((KEY_Past&BIT1)==BIT1)&&(KEY_Now&BIT1)!=BIT1)
    {
        IO11_Onclick_Num += 1;
        I2C_IO11_Onclick(IO11_Onclick_Num);
    }
    //----�ж�I2C_IO12������KEY3�����Ƿ񱻰���------
    if((TCA6416A_InputBuffer&BITA) == BITA)
        KEY_Now |=BIT2;
    else
        KEY_Now &=~BIT2;
    if(((KEY_Past&BIT2)==BIT2)&&(KEY_Now&BIT2) ==0)
        I2C_IO12_Onclick(IO11_Onclick_Num,IO10_Onclick_Num);
    //----�ж�I2C_IO13������KEY4�����Ƿ񱻰���------
    if((TCA6416A_InputBuffer&BITB) ==  BITB)
        KEY_Now |=BIT3;
    else
        KEY_Now &=~BIT3;
    if(((KEY_Past&BIT3) == BIT3)&& (KEY_Now&BIT3) == 0)
    {
        I2C_IO13_Onclick(IO11_Onclick_Num,IO10_Onclick_Num);
    }
    return IO10_Onclick_Num;
}

