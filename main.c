/******************************************************************************************
 * main.c
 * ����ʱ��������
 * ����������
 * ��1����ͨ��������������ʱ��֮���л���ʾ��
 * ��2�����ɰ����������ں�ʱ�䣻
 * ��3�������㱨ʱ����ཡ�ཡ�������
 * ��4�����趨ʱ����ʱʱ�䵽������ཡ�ཡ�����
 * ��5����������ܡ�
 * Created on: 2021-5-28
 * Author: �����
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

    DAC8411_Init();        //��ʼ����Ƶ���ʷŴ�������˿�
    TCA6416A_Init();      // ��ʼ��IO��չ��
    HT1621_init();          // ��ʼ��lcd_128
    TimerA_Init();           //��ʼ��ʱ�Ӷ�ʱ��
    Sec_TimerA_Init();     //��ʼ�����ʱ��

    while(1)
    {
        PinIN();
        HT1621_Reflash(LCD_Buffer);
        IO10_Onclick_Num = I2C_IODect();
        switch(IO10_Onclick_Num)
        {
        case 0:
            Display_Time();                //Ĭ����ʾʱ����
            break;
        case 1:
            Display_Date();                //��һ����ʾ������
            break;
        case 2:
            Display_Sec_Clock();        //��������ʾ����ܣ�
            break;
        case 3:
            Display_Alarm();              //��������ʾ����
            break;
        default:
            Refresh_Sec_Clock();         //������ֵ��λ��
            break;
        }
        Hourly_Chime();                    //���㱨ʱ������
        Alarm();                               //���Ӻ������������ӿ��ع���
    }
}


