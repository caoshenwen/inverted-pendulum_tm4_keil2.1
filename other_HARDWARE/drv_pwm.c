#include "drv_pwm.h"

volatile uint32_t ui32Load  = 0;
volatile uint8_t  ui8Adjust = 4;

void PWM_Init(void)
{
	volatile uint32_t ui32PWMClock;
	
	// Set divider to 50M/4=12.5M --> 0.08us
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
	
	// Enable PWM peripheral
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	// delay_ms(2)
	ROM_SysCtlDelay(SysCtlClockGet()/6/500);
	
	ROM_GPIOPinConfigure(GPIO_PE4_M0PWM4);		// PE4 --> M0PWM4
	ROM_GPIOPinConfigure(GPIO_PE5_M0PWM5);		// PE5 --> M0PWM5
	ROM_GPIOPinConfigure(GPIO_PA6_M1PWM2);		// PA6 --> M1PWM2
	ROM_GPIOPinConfigure(GPIO_PA7_M1PWM3);		// PA7 --> M1PWM3
	//ROM_GPIOPinConfigure(GPIO_PD0_M0PWM6);	// PD0 --> M0PWM6
	//ROM_GPIOPinConfigure(GPIO_PD1_M0PWM7);	// PD0 --> M0PWM7
	
	ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);	//M0PWM4
	ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);	//M0PWM5
	ROM_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);	//M1PWM2
	ROM_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_7);	//M1PWM3
	//ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);	//M0PWM6
	//ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);	//M0PWM7
	
	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN );
	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN );
	//PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN );
	
	// Set period
	ui32PWMClock = SysCtlClockGet() / 4;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, ui32Load); 
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, ui32Load);
	//PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, ui32Load);
		
	ROM_PWMOutputState(	PWM0_BASE, 
						PWM_OUT_4_BIT|PWM_OUT_5_BIT, 
						true);
						
	ROM_PWMOutputState(	PWM1_BASE, 
						PWM_OUT_2_BIT|PWM_OUT_3_BIT, 
						true);
						
	//ROM_PWMOutputState(	PWM0_BASE, 
	//					PWM_OUT_6_BIT|PWM_OUT_7_BIT, 
	//					true);
						
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_1);
	//ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_3);
	
	// Set default output
	output_PWM_to_ESC(900,900,900,900);
}

void output_PWM_to_ESC(uint16_t PulseWidth1, uint16_t PulseWidth2,
					   uint16_t PulseWidth3, uint16_t PulseWidth4)
{		
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PulseWidth1 * ui8Adjust * ui32Load / 10000);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PulseWidth2 * ui8Adjust * ui32Load / 10000);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, PulseWidth3 * ui8Adjust * ui32Load / 10000);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, PulseWidth4 * ui8Adjust * ui32Load / 10000);

	//ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 1230 * ui8Adjust * ui32Load / 10000);
	//ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 1560 * ui8Adjust * ui32Load / 10000);
}

