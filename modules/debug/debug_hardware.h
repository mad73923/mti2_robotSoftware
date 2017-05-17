/*
 * debug_hardware.h
 *
 *  Created on: 17.05.2017
 *      Author: matthias
 */

#ifndef DEBUG_DEBUG_HARDWARE_H_
#define DEBUG_DEBUG_HARDWARE_H_

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_rcc.h"

void debug_hardware_init(void);
void uart_send(uint8_t* buffer, uint32_t length);
void uart_waitTransmissionComplete(void);

#endif /* DEBUG_DEBUG_HARDWARE_H_ */
