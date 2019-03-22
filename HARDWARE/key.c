/*
 * key.c
 *
 *  Created on: 2018Äê5ÔÂ12ÈÕ
 *      Author: Dell
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "key.h"


uint8_t ReadPin;           // Save the value of read from GPIO.
int  KeyPress ;  // The flag indicates the key is press.

void KEY_init(void)
{
        //
        // Enable the GPIO port that is used for the on-board LED and KEY.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        //
        // Check if the peripheral access is enabled.
        //
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
            ;
        //
        // Enable the GPIO pin for the KEY1 (PF4).  Set the direction as input, and
        // enable the GPIO pin for digital function.
        //
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
        //
        // Configure the gpio mode, Push-pull with weak pull-up.
        //
        GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}




void key_scan(void)
{
    // Read the key-pin.
    ReadPin = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
    if((ReadPin&GPIO_PIN_4) != GPIO_PIN_4)// If key1 is press now.
    {
        // Delay 20ms for dithering elimination.
        SysCtlDelay(20*(SysCtlClockGet()/3000));
        // Read the key-pin again.
        ReadPin = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        if((ReadPin&GPIO_PIN_4) != GPIO_PIN_4)
        { // If key1 is press still.
           
			KeyPress ++;
			
			if(KeyPress>3)KeyPress=0;
            // Wait key release.
            while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
            {
                ;
            }
        }
    }
}

