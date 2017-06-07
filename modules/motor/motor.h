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
uint16_t motor_setSpeed(char cMotor, int iSpeed);
uint16_t motor_setDirection(char cMotor,char cDir);
uint16_t motor_setSpeedDir(char cMotor, char cDir, int iSpeed);
void motor_stop(char cMotor);

//
#endif /* MOTOR_MOTOR_H_ */
