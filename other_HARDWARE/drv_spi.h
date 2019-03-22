#ifndef  _DRV_SPI_H_
#define  _DRV_SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"

void SPI2_Init(void);
void SPI2_ReadRegs(uint8_t reg, uint8_t length, uint8_t *data);
uint8_t SPI2_WriteRead_Byte(uint8_t TxData);
#endif
