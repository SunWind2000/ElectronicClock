/*
 * I2C_Key.c
 *
 *  Created on: 2021年5月29日
 *      Author: 孙昊阳
 */

#include <MSP430G2553.h>
#include "TCA6416A.h"
#include "HT1621.h"
#include "LCD_Display.h"
#include "Sec_Clock.h"

/********WDT初始化函数**********/
void WDT_init()
{
    //-----设定WDT为-----------
    WDTCTL=WDT_ADLY_16;
    //-----WDT中断使能----------------------
    IE1 |= WDTIE;
}
/***********WDT定时中断函数*************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    __bic_SR_register_on_exit(LPM0_bits );
}

/**********I2C扩展IO中断事件处理函数***************/
/******************************************************************************************************
 * 名       称：I2C_IO10_Onclick()
 * 功       能：设定按键KEY1。
 * 入口参数：IO10_Onclick_Num，按键次数
 * 出口参数：无
 * 说       明：按下KEY1后，提示可以选择需要设定的参数（幅值、频率、垂直位移）
 ******************************************************************************************************/
void I2C_IO11_Onclick(unsigned int IO11_Onclick_Num)
{
    _enable_interrupts(); //开启总中断
    switch(IO11_Onclick_Num)
    {
    case 1:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,0);//代表正在设定月份
        PinOUT(5,1);
        PinOUT(6,1);
        PinOUT(7,1);
        break;
    case 2:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,1);
        PinOUT(5,0);//代表正在设定日期
        PinOUT(6,1);
        PinOUT(7,1);
        break;
    case 3:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,1);
        PinOUT(5,1);
        PinOUT(6,0);//代表正在设定小时
        PinOUT(7,1);
        break;
    case 4:
        HT1621_Reflash(LCD_Buffer);
        PinOUT(4,1);
        PinOUT(5,1);
        PinOUT(6,1);
        PinOUT(7,0);//代表正在设定分钟
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

unsigned int  I2C_IODect()                            //检测事件确实发生了
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
    //----判断I2C_IO10所连的KEY1按键是否被按下------
    if((TCA6416A_InputBuffer&BIT8) == BIT8)
        KEY_Now |=BIT0;
    else
        KEY_Now &=~BIT0;
    if(((KEY_Past&BIT0)==BIT0)&&(KEY_Now&BIT0) !=BIT0)
    {
        IO10_Onclick_Num += 1;
//        I2C_IO10_Onclick(IO10_Onclick_Num);
    }
    //----判断I2C_IO11所连的KEY2按键是否被按下------
    if((TCA6416A_InputBuffer&BIT9)== BIT9)
        KEY_Now |=BIT1;
    else
        KEY_Now &=~BIT1;
    if(((KEY_Past&BIT1)==BIT1)&&(KEY_Now&BIT1)!=BIT1)
    {
        IO11_Onclick_Num += 1;
        I2C_IO11_Onclick(IO11_Onclick_Num);
    }
    //----判断I2C_IO12所连的KEY3按键是否被按下------
    if((TCA6416A_InputBuffer&BITA) == BITA)
        KEY_Now |=BIT2;
    else
        KEY_Now &=~BIT2;
    if(((KEY_Past&BIT2)==BIT2)&&(KEY_Now&BIT2) ==0)
        I2C_IO12_Onclick(IO11_Onclick_Num,IO10_Onclick_Num);
    //----判断I2C_IO13所连的KEY4按键是否被按下------
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

