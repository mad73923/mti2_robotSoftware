/*
 * debug.h
 *
 *  Created on: 17.05.2017
 *      Author: matthias
 */

#ifndef DEBUG_DEBUG_H_
#define DEBUG_DEBUG_H_


#include "debug_hardware.h"
#include <stdarg.h>
#include <stdio.h>

#define DEBUG_BUFFERSIZE	300

void debug_init(void);
void debug_printf(const char* format, ...);

void debug_led_on(void);
void debug_led_off(void);
void debug_led_toggle(void);

#endif /* DEBUG_DEBUG_H_ */
