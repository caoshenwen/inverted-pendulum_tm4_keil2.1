#include "drv_i2c.h"

void I2C3_Init(void)
{	
	// The I2C3 peripheral must be enabled before use.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// Configure the pin muxing for I2C3 functions on port D0 and D1.
    ROM_GPIOPinConfigure(GPIO_PD0_I2C3SCL);
    ROM_GPIOPinConfigure(GPIO_PD1_I2C3SDA);
	
	// Select the I2C function for these pins.  This function will also
    // configure the GPIO pins pins for I2C operation, setting them to
    // open-drain operation with weak pull-ups.  Consult the data sheet
    // to see which functions are allocated per pin.
    GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
    ROM_GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	// Keep only some parts of the systems running while in sleep mode.
    ROM_SysCtlPeripheralClockGating(true);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_I2C3);
	
	ROM_I2CMasterInitExpClk(I2C3_BASE, 16000000, true);
	ROM_I2CMasterEnable(I2C3_BASE);
}

void I2C3_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data)
{
    ROM_I2CMasterSlaveAddrSet(I2C3_BASE,addr,false);
    ROM_I2CMasterDataPut(I2C3_BASE,reg);
    ROM_I2CMasterControl(I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_START);
    while(ROM_I2CMasterBusy(I2C3_BASE));
    ROM_I2CMasterDataPut(I2C3_BASE,data);
    ROM_I2CMasterControl(I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);
    while(ROM_I2CMasterBusy(I2C3_BASE));
    ROM_I2CMasterControl(I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);
    
}

uint8_t I2C3_Read_Byte(uint8_t addr,uint8_t reg)
{
	uint8_t res;
    uint32_t raw_res;  
    ROM_I2CMasterSlaveAddrSet(I2C3_BASE,addr,false);
    ROM_I2CMasterDataPut(I2C3_BASE,reg);
    ROM_I2CMasterControl(I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_START);
    while(ROM_I2CMasterBusy(I2C3_BASE));
    ROM_I2CMasterSlaveAddrSet(I2C3_BASE,addr,true);
    ROM_I2CMasterControl(I2C3_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(ROM_I2CMasterBusy(I2C3_BASE));
    raw_res=ROM_I2CMasterDataGet(I2C3_BASE);
    res=(uint8_t)(raw_res);
    return res;
}


