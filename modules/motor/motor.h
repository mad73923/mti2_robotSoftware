/*
 * motor.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "motor_timer.h"

void configure_gpio(void);
int32_t motor_setSpeed(char cMotor, int32_t iSpeed);
void motor_stop(char cMotor);
void horn_enable(uint8_t enable);

//
#endif /* MOTOR_MOTOR_H_ */
