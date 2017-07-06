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

#define DEBUG_UART_DMA_CLK_INIT()	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define DEBUG_UART_TX_DMA_INST		DMA1
#define DEBUG_UART_TX_DMA_CH		LL_DMA_CHANNEL_7
#define DEBUG_UART_TX_DMA_REQ		LL_DMA_REQUEST_2
#define DEBUG_UART_TX_DMA_IRQn		DMA1_Channel7_IRQn
#define DEBUG_UART_TX_DMA_PRIO		13
#define DEBUG_UART_TX_DMA_HANDLER()	DMA1_Channel7_IRQHandler()

#define DEBUG_UART_INST				USART2
#define DEBUG_UART_CLK_EN()			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define DEBUG_UART_CLK_SRC			LL_RCC_USART2_CLKSOURCE_PCLK1

#define DEBUG_UART_TX_GPIO_PORT		GPIOA
#define DEBUG_UART_TX_GPIO_PIN		LL_GPIO_PIN_2
#define DEBUG_UART_GPIO_CLK_INIT()	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define DEBUG_UART_TX_AF			LL_GPIO_AF_7
#define DEBUG_UART_SET_AF			LL_GPIO_SetAFPin_0_7

#define DEBUG_LED_GPIO_PORT			GPIOA
#define DEBUG_LED_GPIO_PIN			LL_GPIO_PIN_5
#define DEBUG_LED_GPIO_CLK_INIT()	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)

#endif /* DEBUG_DEBUG_HARDWARE_H_ */
