/*
 * delay.h
 *
 *  Created on: 2018Äê2ÔÂ2ÈÕ
 *      Author: admin
 */

#ifndef DELAY_INIT_DELAY_H_
#define DELAY_INIT_DELAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"

extern void delays (uint16_t s );
extern void delayms(uint16_t ms);
extern void delayus(uint16_t us);

#endif /* DELAY_INIT_DELAY_H_ */
