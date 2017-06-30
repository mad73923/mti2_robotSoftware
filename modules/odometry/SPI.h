/*
 * SPI.h
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#ifndef ODOMETRY_SPI_H_
#define ODOMETRY_SPI_H_

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_spi.h"

void SPI_init(void);

void SPI_communicate_sync(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX);
void SPI_communicate_async(uint16_t* pdataTX, uint32_t ndataTX, uint16_t* pdataRX, uint32_t ndataRX, void* doneCallback);
void SPI_waitForClearance(void);

#define TLE_SPI_INST		SPI2
#define TLE_SPI_IRQN		SPI2_IRQn
#define TLE_SPI_IRQ_PRIO	5
#define TLE_SPI_CLK_INIT()	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2)
#define TLE_SPI_IRQ_HANDLER	SPI2_IRQHandler

#define TLE_GPIO_SCK_CLK_INIT()		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define TLE_GPIO_SCK		GPIOB
#define TLE_GPIO_SCK_PIN	LL_GPIO_PIN_13
#define TLE_GPIO_SCK_SETAF	LL_GPIO_SetAFPin_8_15
#define TLE_GPIO_SCK_AF		LL_GPIO_AF_5

#define TLE_GPIO_MOSI_CLK_INIT()	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define TLE_GPIO_MOSI		GPIOC
#define TLE_GPIO_MOSI_PIN	LL_GPIO_PIN_3
#define TLE_GPIO_MOSI_SETAF	LL_GPIO_SetAFPin_0_7
#define TLE_GPIO_MOSI_AF	LL_GPIO_AF_5

#endif /* ODOMETRY_SPI_H_ */
