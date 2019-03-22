/*
 * GPIO_Init.h
 *
 *  Created on: 2018Äê4ÔÂ6ÈÕ
 *      Author: admin
 */

#ifndef GPIO_INIT_GPIO_INIT_H_
#define GPIO_INIT_GPIO_INIT_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "delay.h"

#define RED1   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
#define BLUE1  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
#define GREEN1 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
//#define BEEP1  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7);
#define RED0   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
#define BLUE0  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
#define GREEN0 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
//#define BEEP0  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);

extern void PF123_RGB_Output(void);
extern void PF1_Red_Output(void);
extern void PF2_Blue_Output(void);
extern void PF3_Green_Output(void);
extern void PA7_Beep_Output(void);
//extern void Beep_Di(uint8_t ui8count, uint16_t DiTime);

#endif /* GPIO_INIT_GPIO_INIT_H_ */
