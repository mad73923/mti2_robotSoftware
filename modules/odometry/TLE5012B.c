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
	SPI_waitForClearance();
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

float sensor_getAngle(TLE5012B_ACT_t side){
	int16_t val = sensor_readRegister(AVAL, side);
	val &= 0b0111111111111111;
	if(val & 0b0100000000000000){
		val = 0b1100000000000000+(val&0b0011111111111111);
	}
	float ret = (360.0*val)/32768.0;
	return ret;
}

int16_t sensor_getRevolutions(TLE5012B_ACT_t side){
	int16_t val = sensor_readRegister(AREV, side);
	val &= 0b111111111;
	if(val & 0b100000000){
		val = 0b1111111100000000+(val&0b11111111);
	}
	return val;
}

/*
 * Private functions
 */

/*
 * @brief	This function configures the CS pins.
 */
void CS_init(void){
	TLE5012B_CS_EnableGPIOClock_L();
	LL_GPIO_SetPinMode(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetOutputPin(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L);
}

void CS_activateLeft(void){
	LL_GPIO_ResetOutputPin(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L);
}

void CS_resetAll(void){
	LL_GPIO_SetOutputPin(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L);
	//TODO right side
}
