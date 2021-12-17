/******************************************************************************************
 * main.c
 * 电子时钟主函数
 * 功能描述：
 * （1）可通过按键在日历和时间之间切换显示；
 * （2）可由按键调整日期和时间；
 * （3）可整点报时（“嘟、嘟”声）；
 * （4）可设定时，定时时间到发出“嘟、嘟”声；
 * （5）具有秒表功能。
 * Created on: 2021-5-28
 * Author: 孙昊阳
 ******************************************************************************************/
#include <msp430.h>
#include "TCA6416A.h"
#include "HT1621.h"
#include "LCD_Display.h"
#include "I2C_Key.h"
#include "Sec_Clock.h"
#include "DAC8411.h"


void main(void) {
    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	
    unsigned char IO10_Onclick_Num = 0;

    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;

    DAC8411_Init();        //初始化音频功率放大器输出端口
    TCA6416A_Init();      // 初始化IO扩展口
    HT1621_init();          // 初始化lcd_128
    TimerA_Init();           //初始化时钟定时器
    Sec_TimerA_Init();     //初始化秒表定时器

    while(1)
    {
        PinIN();
        HT1621_Reflash(LCD_Buffer);
        IO10_Onclick_Num = I2C_IODect();
        switch(IO10_Onclick_Num)
        {
        case 0:
            Display_Time();                //默认显示时分秒
            break;
        case 1:
            Display_Date();                //按一次显示年月日
            break;
        case 2:
            Display_Sec_Clock();        //按两次显示秒表功能；
            break;
        case 3:
            Display_Alarm();              //按三次显示闹钟
            break;
        default:
            Refresh_Sec_Clock();         //秒表计数值复位；
            break;
        }
        Hourly_Chime();                    //整点报时函数；
        Alarm();                               //闹钟函数，具有闹钟开关功能
    }
}


