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

void PID_trigger(void);

#endif /* PID_PID_H_ */
