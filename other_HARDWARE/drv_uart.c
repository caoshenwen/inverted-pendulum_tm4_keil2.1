#include "drv_uart.h"

//*****************************************************************************
// Configure the UART and its pins.  This must be called before UARTprintf().
//*****************************************************************************
void UART_Init(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	// Other Uart: 1,2,3,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	// Un-lock PD7 for its NMI property
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;	// unlock
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;				// make sure
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;				// lock again

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);
	// Other Uart: 1,2,3,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART1);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART2);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART3);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART7);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	// Other Uart: 1,2,3,7
	ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_GPIOPinConfigure(GPIO_PD6_U2RX);
    ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	ROM_GPIOPinConfigure(GPIO_PE0_U7RX);
    ROM_GPIOPinConfigure(GPIO_PE1_U7TX);
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	// Other Uart: 1,2,3,7
	UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
	UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);
	UARTClockSourceSet(UART3_BASE, UART_CLOCK_PIOSC);
	UARTClockSourceSet(UART7_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 500000, 16000000);
	// Other Uart: 1,2,3,7
	UARTStdioConfig(1, 115200, 16000000);
	UARTStdioConfig(2, 115200, 16000000);
	UARTStdioConfig(3, 115200, 16000000);
	UARTStdioConfig(7, 115200, 16000000);
	
	// Enable the UART interrupt.
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_TX | UART_INT_RT );
	// Other Uart: 1,2,3,7
	ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_TX | UART_INT_RT );
	ROM_IntEnable(INT_UART2);
    ROM_UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_TX | UART_INT_RT );
	ROM_IntEnable(INT_UART3);
    ROM_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_TX | UART_INT_RT );
	ROM_IntEnable(INT_UART7);
    ROM_UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_TX | UART_INT_RT );
}

//*****************************************************************************
// Send a string to the UART0.
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    // Loop while there are more characters to send.
    while(ui32Count--) {
        // Write the next character to the UART.
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

uint8_t _cnt_pcdata = 0;
uint8_t PCData_len = 0;

void UART0IntHandler(void)
{	
	uint32_t ui32Status;
	BaseType_t pxHigherPriorityTaskWoken; 

    // Get the interrrupt status of the UART0.
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART0_BASE, ui32Status);
	
	/* Transfer */
	// NULL
	
	/* Recieve */
	while (ROM_UARTCharsAvail(UART0_BASE)) {
		g_ui8RxBuf[_cnt_pcdata++]=ROM_UARTCharGetNonBlocking(UART0_BASE);
	}
	
	if (ui32Status == UART_INT_RT) {
		PCData_len = _cnt_pcdata;
		_cnt_pcdata = 0;
		if(BinSem_DataDeal != NULL) {
			xSemaphoreGiveFromISR(BinSem_DataDeal, &pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		}
	}		
}
void UART1IntHandler(void)
{
	uint32_t ui32Status;
	//BaseType_t pxHigherPriorityTaskWoken; 

    // Get the interrrupt status of the UART1.
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART1_BASE, ui32Status);
	
	/* Transfer */
	// NULL
	
	/* Recieve */
	// NULL
}
void UART2IntHandler(void)
{
	uint32_t ui32Status;
	//BaseType_t pxHigherPriorityTaskWoken; 

    // Get the interrrupt status of the UART2.
    ui32Status = ROM_UARTIntStatus(UART2_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART2_BASE, ui32Status);
	
	/* Transfer */
	// NULL
	
	/* Recieve */
	// NULL
}
void UART3IntHandler(void)
{
	uint32_t ui32Status;
	//BaseType_t pxHigherPriorityTaskWoken; 

    // Get the interrrupt status of the UART3.
    ui32Status = ROM_UARTIntStatus(UART3_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART3_BASE, ui32Status);
	
	/* Transfer */
	// NULL
	
	/* Recieve */
	// NULL
}
void UART7IntHandler(void)
{
	uint32_t ui32Status;
	//BaseType_t pxHigherPriorityTaskWoken; 

    // Get the interrrupt status of the UART7.
    ui32Status = ROM_UARTIntStatus(UART7_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART7_BASE, ui32Status);
	
	/* Transfer */
	// NULL
	
	/* Recieve */
	// NULL
}

