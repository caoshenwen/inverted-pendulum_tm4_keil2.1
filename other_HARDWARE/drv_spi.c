#include "drv_spi.h"

void SPI2_ReadRegs(uint8_t reg, uint8_t length, uint8_t *data)
{
	uint8_t _cnt = 0;
	SPI2_WriteRead_Byte(reg|0x80);
	for (_cnt=0; _cnt<length; _cnt++) {
		data[_cnt] = SPI2_WriteRead_Byte(0xff);
	}
}

uint8_t SPI2_WriteRead_Byte(uint8_t TxData)
{
	uint32_t pui32DataRx;
	while(ROM_SSIBusy(SSI2_BASE)==true);
	ROM_SSIDataPut(SSI2_BASE, (uint32_t)TxData);
	
	while(ROM_SSIBusy(SSI2_BASE)==true);
	ROM_SSIDataGet(SSI2_BASE, &pui32DataRx);
	return ((uint8_t *)&pui32DataRx)[0];
}

void SPI2_Init(void)
{
	uint32_t pui32DataRx;
	
	// The SSI2 peripheral must be enabled for use.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
	
	// SSI2 is used with PortB[4,6,7].
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
	// CS are used with PortB[2] and PortD[2,3]
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// Configure the pin muxing for SSI2 functions on PortB[4,6,7].
	// ROM_GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    ROM_GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    ROM_GPIOPinConfigure(GPIO_PB6_SSI2RX);
    ROM_GPIOPinConfigure(GPIO_PB7_SSI2TX);
	
	// Configure the GPIO settings for the SSI pins. 
	GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 |GPIO_PIN_6 | GPIO_PIN_7 );
	
	// Configure CS: icm-20602: PortB 2
	// 				 LIS3MDL  : PortD 2
	//				 SPL06-001: PortD 3
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
	
	// Configure and enable the SSI port for SPI master mode.  Use SSI2,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
	ROM_SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
						   SSI_MODE_MASTER, 1000000, 8);
	
	// Enable the SSI2 module.
    SSIEnable(SSI2_BASE);					   
	
	// Read any residual data from the SSI port.  This makes sure the receive
    // FIFOs are empty, so we don't read any unwanted junk.  This is done here
    // because the SPI SSI mode is full-duplex, which allows you to send and
    // receive at the same time.  The SSIDataGetNonBlocking function returns
    // "true" when data was returned, and "false" when no data was returned.
    // The "non-blocking" function checks if there is any data in the receive
    // FIFO and does not "hang" if there isn't.
    while(SSIDataGetNonBlocking(SSI2_BASE, (uint32_t *)&pui32DataRx));
}

