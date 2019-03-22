



//ADC����
//�� 3.3
//�� GND
//�� PE3


//������
//a��PD6
//b��PD7


//���
//pb6  pb7

#include "my_include.h"



void  BSP_SysInit (void)
{
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
	ROM_FPUEnable();
	ROM_FPULazyStackingEnable();
	
    // Set the clocking to run from the PLL at 50 MHz.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
	
	// Enable peripherals to operate when CPU is in sleep.
    ROM_SysCtlPeripheralClockGating(true);
}




















