/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                                      Texas Instruments TM4C129x
*                                                on the
*                                             DK-TM4C129X
*                                           Development Kit
*       		Modified by Dr. Samir A. Rawashdeh, for the TM4C123GH6PM microcontroller on the 
*						TM4C123G Tiva C Series Launchpad (TM4C123GXL), November 2014.
*
* Filename      : bsp_led.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDES
*********************************************************************************************************
*/

#include  "drv_led.h"
/*
*********************************************************************************************************
*                                       LED INITIALIZATION
*
* Description: This function is used to initialize the LEDs on the board.
*
* Argument(s): None.
*
* Return(s)  : None.
*
* Caller(s)  : Application.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  LED_Init (void)
{	
	// Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the GPIO pins for the LED (PF1-RED,PF2-BLUE,PF3-GREEN).
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

