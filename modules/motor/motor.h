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
int setSpeed(char cDir, int iSpeed);
int setDirection(char cMotor,char cDir);
int setSpeedDir(char cMotor, char cDir, int iSpeed);
int stop(char cMotor);


#endif /* MOTOR_MOTOR_H_ */
