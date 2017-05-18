/*
 * uart.h
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#ifndef WLAN_UART_H_
#define WLAN_UART_H_

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_dma.h"
#include <string.h>
#include "../modules/debug/debug.h"

void UARTinit(void);
void UARTStartTransfers(const char* Command);
void UARTwaitEndOfTransfer(void);
uint8_t UARTwaitForOkOrError(uint32_t cyclesTimeout);



#endif /* WLAN_UART_H_ */
