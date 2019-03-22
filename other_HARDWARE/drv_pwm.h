#ifndef  _DRV_PWM_H_
#define  _DRV_PWM_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#define PWM_FREQUENCY 400

void PWM_Init(void);
void output_PWM_to_ESC(uint16_t PulseWidth1, uint16_t PulseWidth2,
					   uint16_t PulseWidth3, uint16_t PulseWidth4);
#endif

