/*
 * servo.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_SERVO_H_
#define ENV_SENSOR_SERVO_H_


#include "servo_timer.h"

__IO uint16_t sServoLUT[]={1045,1215,1405,1495,1660,1825,2040,2245,2500,2710,2945,3135,3335,3510,3810,4050,4270,4520,4750};

uint16_t servo_set_angle(uint16_t iAngle);

#endif /* ENV_SENSOR_SERVO_H_ */
