#ifndef  _UDMA_H_
#define  _UDMA_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
//#include  "bsp_int.h"
//#include  <cpu.h>

//*****************************************************************************
// The size of the UART transmit and receive buffers.  They do not need to be
// the same size.
//*****************************************************************************
#define UART_TXBUF_SIZE         64
#define UART_RXBUF_SIZE         256

extern uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];
extern uint8_t g_ui8RxBuf[UART_RXBUF_SIZE];

void uDMA_Init(void);
void InitUART0Transfer(void);
void uDMAErrorHandler(void);

#endif
