#ifndef _MY_INCLUDE_H_
#define _MY_INCLUDE_H_

#include  <string.h>
#include  <stdbool.h>
#include  <stdint.h>
#include  <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "utils/uartstdio.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_pwm.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_i2c.h"

#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/fpu.h"
#include "driverlib/qei.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "driverlib/i2c.h"
#include "utils/uartstdio.h"


#include "UART_Init.h"
#include "Timer_Init.h"
#include "PWM_Model.h"
#include "PID.h"
#include "GPIO_Init.h"
#include "QEI_Model.h"
#include "ADC.h"
#include "key.h"
#include "drv_led.h"
#include "adc.h"
#include "UART_Init.h"



extern void  BSP_SysInit (void);




#endif
















