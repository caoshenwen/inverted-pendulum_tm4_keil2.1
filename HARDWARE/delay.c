/*
 * delay.c
 *
 *  Created on: 2018Äê2ÔÂ2ÈÕ
 *      Author: admin
 */
#include "delay.h"

void delays(uint16_t s)
{
    uint16_t i;
    for(i=0; i<s; i++)
        SysCtlDelay(SysCtlClockGet()/3);
}
void delayms(uint16_t ms)
{
    uint16_t i;
    for(i=0; i<ms; i++)
        SysCtlDelay(SysCtlClockGet()/3000);
}
void delayus(uint16_t us)
{
    uint16_t i;
    for(i=0; i<us; i++)
        SysCtlDelay(SysCtlClockGet()/3000000);
}

