/*
 * servo.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_SERVO_H_
#define ENV_SENSOR_SERVO_H_


#include "servo__timer.h"

short sServoLUT[]={1045,1215,1405,1495,1660,1825,2040,2245,2500,2710,2945,3135,3335,3510,3810,4050,4270,4520,4750};

int servo_set_angle(int iAngle);

#endif /* ENV_SENSOR_SERVO_H_ */
