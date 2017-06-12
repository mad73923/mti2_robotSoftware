/*
 * TLE5012B.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "TLE5012B.h"

/*
 * Variables
 */

volatile uint8_t ressourceBusy;

void (*readRegister_async_callback)(uint16_t);
uint16_t readRegister_async_RXBuffer[2];

void (*module_done_callback)(void);

float* asyncGetAngle;
int16_t* asyncGetRevos;

/*
 * Private function prototypes
 */

void CS_init(void);
void CS_activateLeft(void);
void CS_resetAll(void);
void CS_activateSide(TLE5012B_ACT_t side);

void mutex_lock(void);
void mutex_unlock(void);

uint16_t sensor_readRegister(TLE5012B_REG_t reg, TLE5012B_ACT_t side);
void sensor_readRegister_async(TLE5012B_REG_t reg, TLE5012B_ACT_t side, void (*callback)(uint16_t));
void readRegister_async_done(void);

void sensor_writeRegister(TLE5012B_REG_t reg, uint16_t value, TLE5012B_ACT_t side);
void sensor_writeRegister_async(TLE5012B_REG_t reg, uint16_t value, TLE5012B_ACT_t side);

void sensor_getAngle_async_callback(uint16_t registerValue);
void sensor_getRevos_async_callback(uint16_t registerValue);

void prepareTXBuffer(TLE5012B_REG_t reg, TLE5012B_RW_t readOrWrite, uint16_t* txBuffer);

float calculateAngleValue(uint16_t registerValue);
int16_t calculateRevolutionValue(uint16_t registerValue);

/*
 * Public functions
 */

void sensor_init(void){
	SPI_init();
	CS_init();

	mutex_unlock();

	sensor_hardwareReset(TLE_LEFT);
	sensor_disableCRCMonitoring(TLE_LEFT);
	sensor_setAngleTo0(TLE_LEFT);
}

float sensor_getAngle(TLE5012B_ACT_t side){
	int16_t val = sensor_readRegister(AVAL, side);
	return calculateAngleValue(val);
}

void sensor_getAngle_async(TLE5012B_ACT_t side, float* angle, void (*callback)){
	mutex_lock();
	asyncGetAngle = angle;
	module_done_callback = callback;
	sensor_readRegister_async(AVAL, side, sensor_getAngle_async_callback);
}

int16_t sensor_getRevolutions(TLE5012B_ACT_t side){
	int16_t val = sensor_readRegister(AREV, side);
	return calculateRevolutionValue(val);
}

void sensor_getRevolutions_async(TLE5012B_ACT_t side, int16_t* revos, void (*callback)){
	mutex_lock();
	asyncGetRevos = revos;
	module_done_callback = callback;
	sensor_readRegister_async(AREV, side, sensor_getRevos_async_callback);
}

void sensor_hardwareReset(TLE5012B_ACT_t side){
	sensor_writeRegister_async(ACSTAT, TLE5012B_AS_RST, side);
}

void sensor_disableCRCMonitoring(TLE5012B_ACT_t side){
	sensor_writeRegister_async(ACSTAT, TLE5012B_AS_FUSE, side);
}

void sensor_setAngleTo0(TLE5012B_ACT_t side){
	int16_t val = sensor_readRegister(AVAL, side);
	val &= 0b0111111111111111;
	if(val & 0b0100000000000000){
		val = 0b1111100000000000+((val&0b0011111111111111)>>3);
	}else{
		val = val >> 3;
	}
	int16_t baseVal = sensor_readRegister(MOD_3, side);
	baseVal &= 0b1111111111110000;
		if(baseVal & 0b1000000000000000){
			baseVal = 0b1111000000000000|(baseVal>>4);
		}
	int16_t newVal = baseVal - val;
	newVal = newVal<<4;
	sensor_writeRegister_async(MOD_3, newVal, side);
}

/*
 * Private functions
 */

void mutex_lock(void){
	while(ressourceBusy);
	ressourceBusy = 1;
}

void mutex_unlock(void){
	ressourceBusy = 0;
}

uint16_t sensor_readRegister(TLE5012B_REG_t reg, TLE5012B_ACT_t side){
	SPI_waitForClearance();
	uint16_t rxBuffer[2];
	uint16_t txBuffer = 0;

	prepareTXBuffer(reg, TLE_READ, &txBuffer);

	CS_activateSide(side);

	SPI_communicate_sync(&txBuffer, 1, &rxBuffer[0], 2);

	CS_resetAll();

	return rxBuffer[0];
}

void sensor_readRegister_async(TLE5012B_REG_t reg, TLE5012B_ACT_t side, void (*callback)(uint16_t)){
	SPI_waitForClearance();
	readRegister_async_RXBuffer[0] = 0;
	readRegister_async_RXBuffer[1] = 0;
	uint16_t txBuffer = 0;

	readRegister_async_callback = callback;

	prepareTXBuffer(reg, TLE_READ, &txBuffer);

	CS_activateSide(side);

	SPI_communicate_async(&txBuffer, 1, &readRegister_async_RXBuffer[0], 2, readRegister_async_done);

	return;
}

void readRegister_async_done(void){
	CS_resetAll();
	if(readRegister_async_callback != 0){
		readRegister_async_callback(readRegister_async_RXBuffer[0]);
		readRegister_async_callback = 0;
	}
	mutex_unlock();
	if(module_done_callback != 0){
		module_done_callback();
		module_done_callback = 0;
	}
}

void sensor_writeRegister(TLE5012B_REG_t reg, uint16_t value, TLE5012B_ACT_t side){
	SPI_waitForClearance();
	uint16_t txBuffer[2];
	txBuffer[0] = 0;
	txBuffer[1] = value;

	prepareTXBuffer(reg, TLE_WRITE, &txBuffer[0]);

	CS_activateSide(side);

	SPI_communicate_sync(&txBuffer[0], 2, 0, 1);

	CS_resetAll();

	return;
}

void sensor_writeRegister_async(TLE5012B_REG_t reg, uint16_t value, TLE5012B_ACT_t side){
	SPI_waitForClearance();
	uint16_t txBuffer[2];
	txBuffer[0] = 0;
	txBuffer[1] = value;

	prepareTXBuffer(reg, TLE_WRITE, &txBuffer[0]);

	CS_activateSide(side);

	SPI_communicate_async(&txBuffer[0], 2, 0, 1, CS_resetAll);
}

void prepareTXBuffer(TLE5012B_REG_t reg, TLE5012B_RW_t readOrWrite, uint16_t* txBuffer){
	txBuffer[0] |= readOrWrite << TLE5012B_RW_POS;

	if(reg<0x05){
		txBuffer[0] |= TLE5012B_LOCK_0_4<<TLE5012B_LOCK_POS;
	}else{
		txBuffer[0] |= TLE5012B_LOCK_5_11<<TLE5012B_LOCK_POS;
	}

	txBuffer[0] |= reg << TLE5012B_ADDR_POS;
	txBuffer[0] |= 0x01 << TLE5012B_ND_POS;
}

float calculateAngleValue(uint16_t registerValue){
	int16_t val = registerValue;
	val &= 0b0111111111111111;
	if(val & 0b0100000000000000){
		val = 0b1100000000000000+(val&0b0011111111111111);
	}
	float ret = (360.0*val)/32768.0;
	return ret;
}

void sensor_getAngle_async_callback(uint16_t registerValue){
	*asyncGetAngle = calculateAngleValue(registerValue);
}

int16_t calculateRevolutionValue(uint16_t registerValue){
	int16_t val = registerValue;
	val &= 0b111111111;
	if(val & 0b100000000){
		val = 0b1111111100000000+(val&0b11111111);
	}
	return val;
}

void sensor_getRevos_async_callback(uint16_t registerValue){
	*asyncGetRevos = calculateRevolutionValue(registerValue);
}

/*
 * @brief	This function configures the CS pins.
 */
void CS_init(void){
	TLE5012B_CS_EnableGPIOClock_L();
	LL_GPIO_SetPinMode(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetOutputPin(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L);
}

void CS_activateSide(TLE5012B_ACT_t side){
	if(side == TLE_LEFT){
		CS_activateLeft();
	}
}

void CS_activateLeft(void){
	LL_GPIO_ResetOutputPin(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L);
}

void CS_resetAll(void){
	LL_GPIO_SetOutputPin(TLE5012B_CS_PORT_L, TLE5012B_CS_PIN_L);
	//TODO right side
}
