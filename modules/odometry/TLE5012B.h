/*
 * TLE5012B.h
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#ifndef ODOMETRY_TLE5012B_H_
#define ODOMETRY_TLE5012B_H_

#include "SPI.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_bus.h"

/*
 * Hardware CS (Chip Select)
 * Managed by this Module, NOT SPI!
 */

#define TLE5012B_CS_EnableGPIOClock_L() LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define TLE5012B_CS_PORT_L	GPIOA
#define TLE5012B_CS_PIN_L	LL_GPIO_PIN_15

/*
 * Registers
 */

#define TLE5012B_RW_POS			15
typedef enum{
	TLE_WRITE	= 0x00,
	TLE_READ	= 0x01
}TLE5012B_RW_t;

#define TLE5012B_LOCK_POS		11
#define TLE5012B_LOCK_0_4		0b0000
#define TLE5012B_LOCK_5_11		0b1010

#define TLE5012B_UPD_POS		10
#define TLE5012B_UPD_CUR		0x00
#define TLE5012B_UPD_BUF		0x01

#define TLE5012B_ND_POS			0x00

#define TLE5012B_ADDR_POS		4
typedef enum{
	STAT		= 0x00,
	ACSTAT		= 0x01,
	AVAL		= 0x02,
	ASPD		= 0x03,
	AREV		= 0x04,
	FSYNC		= 0x05,
	MOD_1		= 0x06,
	SIL			= 0x07,
	MOD_2		= 0x08,
	MOD_3		= 0x09,
	OFFX		= 0x0A,
	OFFY		= 0x0B,
	SYNCH		= 0x0C,
	IFAB		= 0x0D,
	MOD_4		= 0x0E,
	TCO_Y		= 0x0F,
	ADC_X 		= 0x10,
	ADC_Y		= 0x11,
	IIF_CNT		= 0x20
}TLE5012B_REG_t;

#define TLE5012B_AS_RST		0x01
#define TLE5012B_AS_FUSE	(0x01<<3)

typedef enum{
	TLE_LEFT,
	TLE_RIGHT
}TLE5012B_ACT_t;

void sensor_init(void);

float sensor_getAngle(TLE5012B_ACT_t side);
int16_t sensor_getRevolutions(TLE5012B_ACT_t side);

void sensor_getAngle_async(TLE5012B_ACT_t side, float* angle);
void sensor_getRevolutions_async(TLE5012B_ACT_t side, int16_t* revos);

void sensor_hardwareReset(TLE5012B_ACT_t side);
void sensor_setAngleTo0(TLE5012B_ACT_t side);
void sensor_disableCRCMonitoring(TLE5012B_ACT_t side);

#endif /* ODOMETRY_TLE5012B_H_ */
