#ifndef  _USART_H_
#define  _USART_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
//#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"

#include  "drv_uDMA.h"
#include  "ProcessPCData.h"

extern uint8_t PCData_len;

void UART_Init(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UART0IntHandler(void);
void UART1IntHandler(void);
void UART2IntHandler(void);
void UART3IntHandler(void);
void UART7IntHandler(void);

#endif
