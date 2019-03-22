/*
 * GPIO_Init.c
 *
 *  Created on: 2018Äê4ÔÂ6ÈÕ
 *      Author: admin
 */
#include "GPIO_Init.h"

void PF123_RGB_Output(void)
{
    PF1_Red_Output();
    PF3_Green_Output();
    PF2_Blue_Output();
}
void PF1_Red_Output(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
}
void PF2_Blue_Output(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
}
void PF3_Green_Output(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}
void PA7_Beep_Output(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);
}
//void Beep_Di(uint8_t ui8count, uint16_t DiTime)
//{
//    switch(ui8count)
//    {
//    case 1:
//        BEEP1;delayms(DiTime);BEEP0;
//        break;
//    case 2:
//        BEEP1;delayms(DiTime);BEEP0;delayms(DiTime);
//        BEEP1;delayms(DiTime);BEEP0;
//        break;
//    case 3:
//        BEEP1;delayms(DiTime);BEEP0;delayms(DiTime);
//        BEEP1;delayms(DiTime);BEEP0;delayms(DiTime);
//        BEEP1;delayms(DiTime);BEEP0;
//        break;
//    case 4:
//        BEEP1;delayms(DiTime);BEEP0;delayms(DiTime);
//        BEEP1;delayms(DiTime);BEEP0;delayms(DiTime);
//        BEEP1;delayms(DiTime);BEEP0;delayms(DiTime);
//        BEEP1;delayms(DiTime);BEEP0;
//        break;
//    default:
//        break;
//    }
//}

