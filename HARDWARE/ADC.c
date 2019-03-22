/*
 * ADC.c
 *
 *  Created on: 2018Äê5ÔÂ8ÈÕ
 *      Author: Dell
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
//#include "inc/hw_ints.h"
#include "inc/hw_adc.h"
#include <inc/hw_sysctl.h>
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "adc.h"
#include "ADC.h"
#include "my_include.h"


int32_t Ch0Value, Ch1Value; 
int16_t angle=0;
float    a=360/3300.0;
volatile bool  bDataReady = true;     // The flag indicates is data ready.
//
// Init the adc.
//
void ADC_Init(void)
{
    //
    // Enable the peripheral adc0.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    //
    // Init the peripheral GPIOE.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
        ;

    //
    // Configure the GPIOE2,3 to peripheral function.
    //
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 |GPIO_PIN_3);

    //
    // Sets the clock configuration for the ADC: use piosc, full rate.
    //
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);

    //
    // Set the reference voltage.
    // Attention: tm4c123x default reference voltage is 3.3v, can't change.
    //
    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);

    //
    // Enable the first sample sequencer to capture the value of channel 0
    // and channel 1 when the processor trigger occurs.
    //
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure a step of the sample sequencer.
    //! \param ui32Base is the base address of the ADC module.
    //! \param ui32SequenceNum is the sample sequence number.
    //! \param ui32Step is the step to be configured.
    //! \param ui32Config is the configuration of this step;
    // *Specific information refer to data sheet and drverlib.
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1 |ADC_CTL_END | ADC_CTL_IE);

    //
    // Configure the hardware over sample value: 64.
    // param ui32Factor is the number of samples to be averaged.
    //
    ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    //
    // Enables sample sequence0.
    //
    ADCSequenceEnable(ADC0_BASE, 0);

    //
    // Register the interrupt handler.
    //
    ADCIntRegister(ADC0_BASE, 0, ADC0IntHander);

    //
    // Enable the interrupt sequence0 of ADC0.
    //
    ADCIntEnable(ADC0_BASE, 0);

    //
    // set the interrut priority.
    //
    IntPrioritySet (INT_SSI0, 1<<6);

    //
    // Enable the interrupt of sequence0.
    //
    IntEnable(INT_SSI0);
}

//
// Trigger a interrupt of ADC0.
//
void ADC_Trig(void)
{
    ADCProcessorTrigger(ADC0_BASE, 0);
}




//
// ADC0 interrupt handler.
//
void ADC0IntHander(void)
{
    uint32_t ui32AdcValue[2] = {0};

    // Clear the interrupt flag.
    ADCIntClear(ADC0_BASE, 0);

    // Get the register value.
    ADCSequenceDataGet(ADC0_BASE, 0, ui32AdcValue);

    // Calculate the voltage, unit in mv.
    angle = ((float)ui32AdcValue[0])*(360.0/4096.0)-98-10;   //-98 ~360-98   
    //Ch1Value = ((float)ui32AdcValue[1])*(3300.0/4096.0);
	
	if(angle<0)  angle+=360;
    //angle    = (uint16_t)(360*Ch1Value/3300);

    // Set the flag.
    bDataReady = true;
	//
}

