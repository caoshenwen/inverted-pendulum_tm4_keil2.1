#include "drv_uDMA.h"

//*****************************************************************************
// The transmit and receive buffers used for the UART transfers.  There is one
// transmit buffer and a pair of recieve ping-pong buffers.
//*****************************************************************************
uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];
uint8_t g_ui8RxBuf[UART_RXBUF_SIZE];

//*****************************************************************************
// The control table used by the uDMA controller.  This table must be aligned
// to a 1024 byte boundary.
//*****************************************************************************
#if defined(ewarm)
#pragma data_alignment=1024
uint8_t ui8ControlTable[1024];
#elif defined(ccs)
#pragma DATA_ALIGN(ui8ControlTable, 1024)
uint8_t ui8ControlTable[1024];
#else
uint8_t ui8ControlTable[1024] __attribute__ ((aligned(1024)));
#endif

void uDMA_Init(void) {

	// Enable the uDMA controller at the system level.  Enable it to continue
	// to run while the processor is in sleep.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UDMA);

	// Enable the uDMA controller error interrupt.  This interrupt will occur
	// if there is a bus error during a transfer.
	ROM_IntEnable(INT_UDMAERR);

	// Enable the uDMA controller.
	ROM_uDMAEnable();

	// Point at the control table to use for channel control structures.
	ROM_uDMAControlBaseSet(ui8ControlTable);

	// Initialize the uDMA UART transfers.
	InitUART0Transfer();
					   
	// Assign ISR handler for uDMA0_Error.
//	BSP_IntVectSet((CPU_INT08U   )BSP_INT_ID_UDAM0_ERR,
//                   (CPU_FNCT_VOID)uDMAErrorHandler);
}

//*****************************************************************************
//
// Initializes the UART0 peripheral and sets up the TX and RX uDMA channels.
// The uDMA RX channel will receive incoming data into buffers in basic mode.
//
//*****************************************************************************
void
InitUART0Transfer(void)
{
    // Enable the UART peripheral, and configure it to operate even if the CPU
    // is in sleep.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);

    // Set both the TX and RX trigger thresholds to 4.  This will be used by
    // the uDMA controller to signal when more data should be transferred.  The
    // uDMA TX and RX channels will be configured so that it can transfer 4
    // bytes in a burst when the UART is ready to transfer more data.
    ROM_UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);

    // Enable the UART for operation, and enable the uDMA interface for both TX
    // and RX channels.
    ROM_UARTEnable(UART0_BASE);
    ROM_UARTDMAEnable(UART0_BASE, UART_DMA_TX);

    // Enable the UART peripheral interrupts.  Note that no UART interrupts
    // were enabled, but the uDMA controller will cause an interrupt on the
    // UART interrupt signal when a uDMA transfer is complete.
    ROM_IntEnable(INT_UART0);

    // Put the attributes in a known state for the uDMA UART0TX channel.  These
    // should already be disabled by default.
    ROM_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART0TX,
                                    UDMA_ATTR_ALTSELECT |
                                    UDMA_ATTR_HIGH_PRIORITY |
                                    UDMA_ATTR_REQMASK);

    // Set the USEBURST attribute for the uDMA UART TX channel.  This will
    // force the controller to always use a burst when transferring data from
    // the TX buffer to the UART.  This is somewhat more effecient bus usage
    // than the default which allows single or burst transfers.
    //
    ROM_uDMAChannelAttributeEnable(UDMA_CHANNEL_UART0TX, UDMA_ATTR_USEBURST);

    // Configure the control parameters for the UART TX.  The uDMA UART TX
    // channel is used to transfer a block of data from a buffer to the UART.
    // The data size is 8 bits.  The source address increment is 8-bit bytes
    // since the data is coming from a buffer.  The destination increment is
    // none since the data is to be written to the UART data register.  The
    // arbitration size is set to 4, which matches the UART TX FIFO trigger
    // threshold.
    ROM_uDMAChannelControlSet(UDMA_CHANNEL_UART0TX | UDMA_PRI_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE |
                              UDMA_ARB_4);

    // Set up the transfer parameters for the uDMA UART TX channel.  This will
    // configure the transfer source and destination and the transfer size.
    // Basic mode is used because the peripheral is making the uDMA transfer
    // request.  The source is the TX buffer and the destination is the UART
    // data register.
    ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART0TX | UDMA_PRI_SELECT,
                               UDMA_MODE_BASIC, g_ui8TxBuf,
                               (void *)(UART0_BASE + UART_O_DR),
                               sizeof(g_ui8TxBuf));

    // Now both the uDMA UART TX and RX channels are primed to start a
    // transfer.  As soon as the channels are enabled, the peripheral will
    // issue a transfer request and the data transfers will begin.
    //ROM_uDMAChannelEnable(UDMA_CHANNEL_UART0RX);
    //ROM_uDMAChannelEnable(UDMA_CHANNEL_UART0TX);
}

// BSP_INT_ID_UDMA0_SOFT
//*****************************************************************************
// The count of times the uDMA interrupt occurred but the uDMA transfer was not
// complete.  This should remain 0.
//*****************************************************************************
//static uint32_t g_ui32BadISR = 0;

//*****************************************************************************
// The count of memory uDMA transfer blocks.  This value is incremented by the
// uDMA interrupt handler whenever a memory block transfer is completed.
//*****************************************************************************
//static uint32_t g_ui32MemXferCount = 0;

//*****************************************************************************
// The count of uDMA errors.  This value is incremented by the uDMA error
// handler.
//*****************************************************************************
static uint32_t g_ui32uDMAErrCount = 0;

//*****************************************************************************
// The interrupt handler for uDMA errors.  This interrupt will occur if the
// uDMA encounters a bus error while trying to perform a transfer.  This
// handler just increments a counter if an error occurs.
//*****************************************************************************
void uDMAErrorHandler(void)
{
    uint32_t ui32Status;

    // Check for uDMA error bit
    ui32Status = ROM_uDMAErrorStatusGet();

    // If there is a uDMA error, then clear the error and increment
    // the error counter.
    if(ui32Status)
    {
        ROM_uDMAErrorStatusClear();
        g_ui32uDMAErrCount++;
    }
}
