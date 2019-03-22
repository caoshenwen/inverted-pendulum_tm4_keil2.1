#ifndef  _DRV_I2C_H_
#define  _DRV_I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

void I2C3_Init(void);
void I2C3_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data);
uint8_t I2C3_Read_Byte(uint8_t addr,uint8_t reg);

#endif

