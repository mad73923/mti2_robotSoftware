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
#include <stdlib.h>

void UARTinit(void);
void UARTStartTransfers(const char* Command);
void UARTStartTransfersCB(const char* Command, void(*callback)(char*,uint16_t));
void UARTwaitEndOfTransfer(void);
uint8_t UARTwaitForOkOrError(uint32_t cyclesTimeout);
uint8_t UARTcheckForNewConnection(void);
uint8_t UARTwaitForStartIndicator(uint32_t cyclesTimeout);
uint8_t UARTwaitForSendOK(uint32_t cyclesTimeout);
uint8_t UARTwaitForReady(uint32_t cyclesTimeout);
const char* UARTCheckForIPD(void);
uint8_t UARTcheckForStartIndicator(void);
uint8_t UARTcheckForSendOK(void);
uint8_t UARTcheckEndOfTransfer(void);
void UARTsetNewLineCallback(void(*callback)(char*,uint16_t));
void UARTclearBuffer(void);

//CLK
#define WLAN_UART_GPIO_CLK_ENABLE()	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
//TX GPIO
#define WLAN_UART_GPIO_TX_PIN		LL_GPIO_PIN_10
#define WLAN_UART_GPIO_TX_PORT		GPIOC
#define WLAN_UART_GPIO_TX_AF		LL_GPIO_AF_7
#define WLAN_UART_SET_TX_AF			LL_GPIO_SetAFPin_8_15
//RX GPIO
#define WLAN_UART_GPIO_RX_PIN		LL_GPIO_PIN_11
#define WLAN_UART_GPIO_RX_PORT		GPIOC
#define WLAN_UART_GPIO_RX_AF		LL_GPIO_AF_7
#define WLAN_UART_SET_RX_AF			LL_GPIO_SetAFPin_8_15
//UART
#define WLAN_UART_INST				USART3
#define WLAN_UART_CLK_ENABLE()		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define WLAN_UART_CLK_SRC			LL_RCC_USART3_CLKSOURCE_PCLK1
#define WLAN_UART_IRQn				USART3_IRQn
#define WLAN_UART_RX_PRIO			10
//DMA
#define WLAN_UART_DMA_CLK_INIT()	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define WLAN_UART_TX_DMA_INST		DMA1
#define WLAN_UART_TX_DMA_CH			LL_DMA_CHANNEL_2
#define WLAN_UART_TX_DMA_REQ		LL_DMA_REQUEST_2
#define WLAN_UART_TX_DMA_IRQn		DMA1_Channel2_IRQn
#define WLAN_UART_TX_DMA_PRIO		10
#define WLAN_UART_TX_DMA_HANDLER()	DMA1_Channel2_IRQHandler()
#define WLAN_UART_TX_DMA_CompleteFlag()	LL_DMA_IsActiveFlag_TC2(WLAN_UART_TX_DMA_INST)
#define WLAN_UART_TX_DMA_ClearFlag()	LL_DMA_ClearFlag_GI2(WLAN_UART_TX_DMA_INST)
#define WLAN_UART_TX_DMA_ErrorFlag()	LL_DMA_IsActiveFlag_TE2(WLAN_UART_TX_DMA_INST)
#endif /* WLAN_UART_H_ */
