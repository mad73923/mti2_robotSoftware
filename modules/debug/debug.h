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

void debug_init(void);
void debug_printf(const char* format, ...);

#endif /* DEBUG_DEBUG_H_ */
