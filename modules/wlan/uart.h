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
uint8_t UARTcheckForNewConnection(void);
uint8_t UARTwaitForStartIndicator(uint32_t cyclesTimeout);
uint8_t UARTwaitForSendOK(uint32_t cyclesTimeout);
uint8_t UARTwaitForReady(uint32_t cyclesTimeout);

//CLK
#define WLAN_UART_GPIO_CLK_ENABLE()	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
//TX
#define WLAN_UART_GPIO_TX_PIN		LL_GPIO_PIN_10
#define WLAN_UART_GPIO_TX_PORT		GPIOC
#define WLAN_UART_GPIO_TX_AF		LL_GPIO_AF_7
#define WLAN_UART_SET_TX_AF			LL_GPIO_SetAFPin_8_15
//RX
#define WLAN_UART_GPIO_RX_PIN		LL_GPIO_PIN_11
#define WLAN_UART_GPIO_RX_PORT		GPIOC
#define WLAN_UART_GPIO_RX_AF		LL_GPIO_AF_7
#define WLAN_UART_SET_RX_AF			LL_GPIO_SetAFPin_8_15

#endif /* WLAN_UART_H_ */
