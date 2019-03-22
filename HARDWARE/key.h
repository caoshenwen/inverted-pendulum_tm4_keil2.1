/*
 * key.h
 *
 *  Created on: 2018Äê5ÔÂ12ÈÕ
 *      Author: Dell
 */

#ifndef KEY_KEY_H_
#define KEY_KEY_H_


#include "my_include.h"


extern uint8_t ReadPin;           // Save the value of read from GPIO.
extern int  KeyPress ;  // The flag indicates the key is press.
extern void KEY_init(void);
extern void key_scan(void);

#endif /* KEY_KEY_H_ */
