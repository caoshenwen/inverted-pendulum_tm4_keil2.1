/*
 * PWM_Model.h
 *
 *  Created on: 2018Äê1ÔÂ29ÈÕ
 *      Author: bobo
 */

#ifndef PWM_MODEL_H_
#define PWM_MODEL_H_

#include "my_include.h"


#define PWMPERIOD   2500


extern void PWM_Config(void);
extern bool PWMDutyCycleSet(int32_t Val);
extern void PWMOUTSet(int16_t Duty1);

#endif /* PWM_MODEL_H_ */
