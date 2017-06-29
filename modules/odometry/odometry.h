/*
 * odometry.h
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#ifndef ODOMETRY_ODOMETRY_H_
#define ODOMETRY_ODOMETRY_H_

#include "TLE5012B.h"
#include <math.h>

typedef struct{
	float angle;
	int16_t revolutions;
}wheel;

typedef struct{
	float posX;
	float posY;
}point;

typedef struct{
	wheel left;
	wheel right;
	point position;
	float theta;
}odo_status;

// all measures in mm
#define ODO_wheelRadius 32.5

#ifdef ODO_halfAxialLength_69
#define ODO_halfAxialLength 69.0
#elif ODO_halfAxialLength_60
#define ODO_halfAxialLength 60.0
#else
#error "Please define axial Length in Compiler Symbols"
#endif

void odometry_init(void);
void odometry_updateStatus_async(void);
odo_status odometry_getStatus(void);
void odometry_setStatus(float x, float y, float theta);

#endif /* ODOMETRY_ODOMETRY_H_ */
