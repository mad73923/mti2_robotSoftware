/*
 * TLE5012B.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "TLE5012B.h"

/*
 * Private function prototypes
 */

void CS_init(void);
void CS_activateLeft(void);
void CS_resetAll(void);

/*
 * Public functions
 */

void sensor_init(void){
	SPI_init();
	CS_init();
}

uint16_t sensor_readRegister(TLE5012B_REG_t reg, TLE5012B_ACT_t side){
	uint16_t rxBuffer = 0;
	uint16_t txBuffer = 0;

	txBuffer |= TLE5012B_RW_READ<<TLE5012B_RW_POS;

	if(reg<0x05){
		txBuffer |= TLE5012B_LOCK_0_4<<TLE5012B_LOCK_POS;
	}else{
		txBuffer |= TLE5012B_LOCK_5_11<<TLE5012B_LOCK_POS;
	}

	txBuffer |= reg << TLE5012B_ADDR_POS;

	txBuffer |= 0x01 << TLE5012B_ND_POS;

	if(side == TLE_LEFT){
		CS_activateLeft();
	}

	SPI_communicate_sync(&txBuffer, 1, &rxBuffer, 1);

	CS_resetAll();

	return rxBuffer;
}

/*
 * Private functions
 */

/*
 * @brief	This function configures the CS pins.
 */
void CS_init(void){
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_15);
}

void CS_activateLeft(void){
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);
}

void CS_resetAll(void){
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_15);
	//TODO right side
}
