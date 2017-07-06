/*
 * pid.h
 *
 *  Created on: 05.07.2017
 *      Author: matthias
 */

#ifndef PID_PID_H_
#define PID_PID_H_

#include "stm32l4xx.h"
#include "../modules/odometry/odometry.h"
#include "../modules/motor/motor.h"

typedef struct{
volatile int32_t inputValue;
volatile int32_t inputValueOld;
volatile int32_t inputValueOld2;
} inputValues;

typedef struct{
	volatile int32_t controlValue;
	volatile int32_t controlValueOld;
	volatile int32_t controlValueOld2;
} controlValues;


void PID_trigger(void);

#endif /* PID_PID_H_ */
