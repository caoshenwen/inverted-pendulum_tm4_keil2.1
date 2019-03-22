/*
 * ADC.h
 *
 *  Created on: 2018Äê5ÔÂ8ÈÕ
 *      Author: Dell
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "my_include.h"

extern void ADC_Init(void);
extern void ADC0IntHander(void);
extern void ADC_Trig(void);




extern int32_t Ch0Value, Ch1Value;          // Savve the voltage value of ch0 and ch1.
extern int16_t angle;
extern float    a;
extern volatile bool  bDataReady ;     // The flag indicates is data ready.
#endif /* ADC_ADC_H_ */
