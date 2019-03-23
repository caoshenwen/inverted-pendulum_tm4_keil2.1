/*
 * UART_Init.c
 *
 *  Created on: 2018年2月2日
 *      Author: admin
 */
#include "UART_Init.h"

void uart0_intial(void)//串口0接受
{
   

    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);


//    IntPrioritySet(INT_UART1, 0x0);
//    UARTIntRegister(UART1_BASE, inthandler_uart1);
//    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
//    IntEnable(INT_UART1);
}
//void UART0_Init(uint32_t ui32Baud)
//{
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//
//    GPIOPinConfigure(GPIO_PA0_U0RX);
//    GPIOPinConfigure(GPIO_PA1_U0TX);
//    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//
//    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);//Clock 16MHz
//    UARTStdioConfig(0, ui32Baud, 16000000);
//
//    UARTIntRegister(UART0_BASE, IntHandler_UART0);
//    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
//    IntEnable(INT_UART0);
//}
//void UART0printf(char *pString)
//{
//    uint16_t i = 0;
//    while(pString[i] != '\0')
//        UARTCharPut(UART0_BASE, pString[i++]);
//}

//void UART1_Init(uint32_t ui32Baud)
//{
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//
//    GPIOPinConfigure(GPIO_PB0_U1RX);
//    GPIOPinConfigure(GPIO_PB1_U1TX);
//    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//
//    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);//Clock 16MHz
//    UARTStdioConfig(1, ui32Baud, 16000000);
//}
//void UART0printf(char *pString)
//{
//    uint16_t i = 0;
//    while(pString[i] != '\0')
//        UARTCharPut(UART1_BASE, pString[i++]);
//}



void InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);





}





