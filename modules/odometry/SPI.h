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
void SPI_waitForClearance(void);

#endif /* ODOMETRY_SPI_H_ */
