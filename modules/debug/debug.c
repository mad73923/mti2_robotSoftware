/*
 * debug.c
 *
 *  Created on: 17.05.2017
 *      Author: matthias
 */

#include "debug.h"

/*
 * Variables
 */

uint8_t txBuffer[DEBUG_BUFFERSIZE];

/*
 * Private functions prototypes
 */

/*
 * Public functions
 */

void debug_init(void){
	debug_hardware_init();
}


void debug_printf(const char* format, ...){
	va_list args;
	va_start(args, format);
	vsprintf(txBuffer, format, args);
	va_end(args);

	uint32_t length = 0;
	while(txBuffer[length] != 0 && length<DEBUG_BUFFERSIZE-2){
		length++;
	}

	uart_waitTransmissionComplete();
	uart_send(txBuffer, length);
}
